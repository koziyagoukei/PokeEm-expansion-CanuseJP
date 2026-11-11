// Pokemon application of GC Extension Mode.
//
// Everything here only reads game state. Records are built field by field
// from GetMonData; sprites are resolved through LoadSpecialPokePicIsEgg and
// GetMonSpritePalFromSpeciesAndPersonalityIsEgg, so gender, shiny, Unown,
// Spinda spots and eggs follow the game's own rules. The host never sees ROM
// addresses: it receives decompressed 4bpp tiles and a BGR555 palette.
#include "global.h"
#include "decompress.h"
#include "event_data.h"
#include "item.h"
#include "malloc.h"
#include "menu.h"
#include "move.h"
#include "pokemon.h"
#include "pokemon_spots.h"
#include "string_util.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "gc_companion_pokemon.h"
#include "gc_extension.h"

#define SPRITE_FRAME_BYTES MON_PIC_SIZE                     // one 64x64 4bpp frame
#define DECOMPRESS_BYTES (MON_PIC_SIZE * MAX_MON_PIC_FRAMES)
#define LABEL_WIDTH_TILES 16
#define LABEL_HEIGHT_TILES 2
#define PALETTE_BYTES 32
#define ASSET_BUFFER_BYTES (DECOMPRESS_BYTES + PALETTE_BYTES)

struct GcPokemonState
{
    u8 asset[ASSET_BUFFER_BYTES];
    u8 labelWindowId;
    const struct GcPokemonUi *ui;
    struct GcStateDigest entry;
    u32 buildId;
};

static EWRAM_DATA struct GcPokemonState *sPkmn = NULL;
static EWRAM_DATA struct Pokemon *sExtraMons = NULL;
static EWRAM_DATA u8 sExtraCount = 0;

static u16 HandleApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength);
static bool ResolveAsset(const AssetKey *key, AssetInfo *info, const u8 **data);

// Filled in by GcPokemon_Init (initialised .data is not linked in this ROM).
static EWRAM_DATA struct GcClientConfig sClientConfig = {0};

// Text colours of label images: transparent, white text, dark shadow.
static const u8 sLabelColors[3] = {0, 1, 2};
static const u16 sLabelPalette[16] = {RGB(0, 0, 0), RGB(31, 31, 31), RGB(6, 7, 9)};

static u32 Crc(u32 state, const void *data, u32 length)
{
    return wire_crc(state, data, length);
}

// Changes whenever the ROM's data layout changes, so a host-side asset cache
// keyed by it never mixes assets from different builds.
static u32 ComputeBuildId(void)
{
    static const char sBuildStamp[] = __DATE__ " " __TIME__;
    u32 words[5] = {(u32)gSpeciesInfo, (u32)gMovesInfo, (u32)gAbilitiesInfo, NUM_SPECIES, MOVES_COUNT};
    u32 crc = Crc(UINT32_MAX, (const u8 *)0x080000A0, 0x20); // title, game code, maker, version
    crc = Crc(crc, words, sizeof(words));
    crc = Crc(crc, sBuildStamp, sizeof(sBuildStamp) - 1);
    return ~crc;
}

u32 GcPokemon_GetBuildId(void)
{
    return sPkmn != NULL ? sPkmn->buildId : 0;
}

// Bag quantities are XOR-encrypted with a key the game changes on every map
// load, so the bag digest covers the decrypted (item, quantity) pairs. The
// save block 1 digest is raw and only comparable within one mode visit.
static u32 BagDigest(void)
{
    u32 pocket, slot, crc = UINT32_MAX;
    for (pocket = 0; pocket < POCKETS_COUNT; pocket++)
    {
        for (slot = 0; slot < gBagPockets[pocket].capacity; slot++)
        {
            struct ItemSlot item = BagPocket_GetSlotData(&gBagPockets[pocket], slot);
            u16 pair[2] = {item.itemId, item.quantity};
            crc = Crc(crc, pair, sizeof(pair));
        }
    }
    return ~crc;
}

void GcPokemon_ComputeDigest(struct GcStateDigest *digest)
{
    u8 count = gPartiesCount[B_TRAINER_PLAYER];
    u32 crc = Crc(UINT32_MAX, &count, 1);
    digest->crc[0] = ~Crc(crc, gParties[B_TRAINER_PLAYER], sizeof(gParties[B_TRAINER_PLAYER]));
    digest->crc[1] = BagDigest();
    digest->crc[2] = ~Crc(UINT32_MAX, gSaveBlock1Ptr->flags, sizeof(gSaveBlock1Ptr->flags));
    digest->crc[3] = ~Crc(UINT32_MAX, gSaveBlock1Ptr->vars, sizeof(gSaveBlock1Ptr->vars));
    digest->crc[4] = ~Crc(UINT32_MAX, gSaveBlock1Ptr, sizeof(*gSaveBlock1Ptr));
}

u32 GcPokemon_CompareWithEntry(const struct GcStateDigest *digest)
{
    u32 i, match = 0;
    for (i = 0; i < STATE_DIGEST_FIELDS; i++)
    {
        if (digest->crc[i] == sPkmn->entry.crc[i])
            match |= 1u << i;
    }
    return match;
}

bool8 GcPokemon_Init(u8 labelWindowId, const struct GcPokemonUi *ui)
{
    if (sPkmn == NULL)
        sPkmn = AllocZeroedUnchecked(sizeof(*sPkmn));
    if (sPkmn == NULL)
        return FALSE;
    sPkmn->labelWindowId = labelWindowId;
    sPkmn->ui = ui;
    sPkmn->buildId = ComputeBuildId();
    GcPokemon_ComputeDigest(&sPkmn->entry);
    sClientConfig.peerType = PEER_POKEEMERALD_EXPANSION;
    sClientConfig.appId = APP_POKEMON;
    sClientConfig.appVersion = PKMN_APP_VERSION;
    sClientConfig.resolveAsset = ResolveAsset;
    sClientConfig.handleApp = HandleApp;
    sClientConfig.buildId = sPkmn->buildId;
    memcpy(sClientConfig.tag, (const u8 *)0x080000AC, 4); // ROM header game code
    return TRUE;
}

void GcPokemon_Free(void)
{
    TRY_FREE_AND_SET_NULL(sPkmn);
    sExtraMons = NULL;
    sExtraCount = 0;
}

void GcPokemon_SetExtraMons(struct Pokemon *mons, u8 count)
{
    sExtraMons = mons;
    sExtraCount = mons != NULL && count <= GC_MAX_EXTRA_MONS ? count : 0;
}

const struct GcClientConfig *GcPokemon_GetClientConfig(void)
{
    return &sClientConfig;
}

static struct Pokemon *PartyMon(u32 slot)
{
    struct Pokemon *mon;
    if (slot >= GC_EXTRA_SLOT_BASE && slot - GC_EXTRA_SLOT_BASE < sExtraCount)
        mon = &sExtraMons[slot - GC_EXTRA_SLOT_BASE];
    else if (slot < PARTY_SIZE && slot < gPartiesCount[B_TRAINER_PLAYER])
        mon = &gParties[B_TRAINER_PLAYER][slot];
    else
        return NULL;
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE)
        return NULL;
    return mon;
}

struct Pokemon *GcPokemon_GetMon(u32 slot)
{
    return PartyMon(slot);
}

static bool32 HasFemaleGraphics(enum Species species, bool32 isShiny)
{
#if P_GENDER_DIFFERENCES
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    return info->frontPicFemale != NULL || (isShiny ? info->shinyPaletteFemale : info->paletteFemale) != NULL;
#else
    return FALSE;
#endif
}

// The key fully describes the pixels the game would draw for this Pokemon.
static AssetKey SpriteKeyForMon(struct Pokemon *mon)
{
    AssetKey key = {ASSET_PKMN_FRONT, 0, 0, 0};
    enum Species species = SanitizeSpeciesId(GetMonData(mon, MON_DATA_SPECIES));
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
    bool32 isShiny = GetMonData(mon, MON_DATA_IS_SHINY);
    bool32 isEgg = GetMonData(mon, MON_DATA_IS_EGG);

    if (isEgg)
    {
        key.id = species;
        key.variant = PKMN_SPRITE_EGG;
        return key;
    }
    if (species == SPECIES_UNOWN)
        species = GetUnownSpeciesId(personality);
    key.id = species;
    if (isShiny)
        key.variant |= PKMN_SPRITE_SHINY;
    if (HasFemaleGraphics(species, isShiny) && IsPersonalityFemale(species, personality))
        key.variant |= PKMN_SPRITE_FEMALE;
    if (ShouldDrawSpotsOnSpecies(species))
        key.extra = personality;
    return key;
}

static void BuildRecord(u32 slot, struct Pokemon *mon, PokemonRecord *r)
{
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    u32 i, ppBonuses;
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u8 gender = GetMonGender(mon);

    memset(r, 0, sizeof(*r));
    r->version = PKMN_RECORD_VERSION;
    r->slot = slot;
    r->length = PKMN_RECORD_BYTES;
    r->personality = GetMonData(mon, MON_DATA_PERSONALITY);
    r->ot_id = GetMonData(mon, MON_DATA_OT_ID);
    r->species = species;
    r->national_dex = SpeciesToNationalPokedexNum(species);
    r->form = GetFormIdFromFormSpeciesId(species);
    r->level = GetMonData(mon, MON_DATA_LEVEL);
    r->nature = GetNature(mon);
    r->tera_type = GetMonData(mon, MON_DATA_TERA_TYPE);
    r->gender = gender == MON_MALE ? 0 : gender == MON_FEMALE ? 1 : 2;
    if (GetMonData(mon, MON_DATA_IS_SHINY))
        r->flags |= PKMN_FLAG_SHINY;
    if (GetMonData(mon, MON_DATA_IS_EGG))
        r->flags |= PKMN_FLAG_EGG;
    r->ability_slot = GetMonData(mon, MON_DATA_ABILITY_NUM);
    r->friendship = GetMonData(mon, MON_DATA_FRIENDSHIP);
    r->hp = GetMonData(mon, MON_DATA_HP);
    r->max_hp = GetMonData(mon, MON_DATA_MAX_HP);
    r->attack = GetMonData(mon, MON_DATA_ATK);
    r->defense = GetMonData(mon, MON_DATA_DEF);
    r->speed = GetMonData(mon, MON_DATA_SPEED);
    r->sp_attack = GetMonData(mon, MON_DATA_SPATK);
    r->sp_defense = GetMonData(mon, MON_DATA_SPDEF);
    r->ability = GetMonAbility(mon);
    r->held_item = GetMonData(mon, MON_DATA_HELD_ITEM);
    r->status = GetMonData(mon, MON_DATA_STATUS);
    ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        r->moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
        r->pp[i] = GetMonData(mon, MON_DATA_PP1 + i);
        r->max_pp[i] = r->moves[i] != MOVE_NONE ? CalculatePPWithBonus(r->moves[i], ppBonuses, i) : 0;
    }
    r->language = GetMonData(mon, MON_DATA_LANGUAGE);
    r->encoding = PKMN_ENCODING_POKEEMERALD;
    GetMonData(mon, MON_DATA_NICKNAME, name);
    memset(r->nickname, EOS, sizeof(r->nickname));
    for (i = 0; i < PKMN_NICKNAME_BYTES && name[i] != EOS; i++)
        r->nickname[i] = name[i];
    r->nickname_length = i;
    r->sprite = SpriteKeyForMon(mon);
}

bool8 GcPokemon_BuildRecord(u32 slot, PokemonRecord *record)
{
    struct Pokemon *mon = PartyMon(slot);
    if (mon == NULL)
        return FALSE;
    BuildRecord(slot, mon, record);
    return TRUE;
}

static bool ResolveSprite(const AssetKey *key, AssetInfo *info, const u8 **data)
{
    u32 i, slot;
    for (i = 0; i < PARTY_SIZE + GC_MAX_EXTRA_MONS; i++)
    {
        struct Pokemon *mon;
        AssetKey monKey;
        enum Species species;
        u32 personality, size;
        bool32 isEgg;
        const u16 *palette;

        slot = i < PARTY_SIZE ? i : GC_EXTRA_SLOT_BASE + i - PARTY_SIZE;
        mon = PartyMon(slot);
        if (mon == NULL)
            continue;
        monKey = SpriteKeyForMon(mon);
        if (!asset_key_equal(key, &monKey))
            continue;

        species = SanitizeSpeciesId(GetMonData(mon, MON_DATA_SPECIES));
        personality = GetMonData(mon, MON_DATA_PERSONALITY);
        isEgg = GetMonData(mon, MON_DATA_IS_EGG);
        // Same call and buffer size (MAX_MON_PIC_FRAMES frames) as battle sprites.
        LoadSpecialPokePicIsEgg(sPkmn->asset, species, personality, TRUE, isEgg);
        palette = GetMonSpritePalFromSpeciesAndPersonalityIsEgg(species, GetMonData(mon, MON_DATA_IS_SHINY), personality, isEgg);
        // The record keeps only the first animation frame; the palette follows it.
        memcpy(sPkmn->asset + SPRITE_FRAME_BYTES, palette, PALETTE_BYTES);
        size = SPRITE_FRAME_BYTES + PALETTE_BYTES;
        info->width = MON_PIC_WIDTH;
        info->height = MON_PIC_HEIGHT;
        info->pixel_format = PIXEL_4BPP_TILED;
        info->palette_format = PALETTE_BGR555_16;
        info->frames = 1;
        info->length = size;
        *data = sPkmn->asset;
        return true;
    }
    return false;
}

static const u8 *LabelString(u32 kind, u32 value, const AssetKey *key, u8 *buffer)
{
    struct Pokemon *mon;
    PokemonRecord record;

    switch (kind)
    {
    case LABEL_SPECIES:
        return value < NUM_SPECIES ? GetSpeciesName(value) : NULL;
    case LABEL_MOVE:
        return value != MOVE_NONE && value < MOVES_COUNT_ALL ? GetMoveName(value) : NULL;
    case LABEL_ABILITY:
        return value < ABILITIES_COUNT ? gAbilitiesInfo[value].name : NULL;
    case LABEL_ITEM:
        return value != ITEM_NONE && value < ITEMS_COUNT ? GetItemName(value) : NULL;
    case LABEL_NICKNAME:
        mon = PartyMon(value);
        if (mon == NULL)
            return NULL;
        BuildRecord(value, mon, &record);
        {
            AssetKey expected = pkmn_nickname_label(&record);
            if (!asset_key_equal(key, &expected))
                return NULL;
        }
        GetMonData(mon, MON_DATA_NICKNAME, buffer);
        return buffer;
    }
    return NULL;
}

// Renders a string with the game's font into the off-screen label window and
// copies the used tile columns out as a packed 4bpp image.
static bool ResolveLabel(const AssetKey *key, AssetInfo *info, const u8 **data)
{
    u8 buffer[POKEMON_NAME_BUFFER_SIZE];
    const u8 *str = LabelString(LABEL_KIND(key->id), LABEL_VALUE(key->id), key, buffer);
    const u8 *tiles;
    s32 width;
    u32 columns, x, y, size;

    if (str == NULL)
        return false;
    width = GetStringWidth(FONT_NORMAL, str, 0);
    if (width <= 0)
        width = 1;
    if (width > LABEL_WIDTH_TILES * 8)
        width = LABEL_WIDTH_TILES * 8;
    columns = (width + 7) / 8;

    FillWindowPixelBuffer(sPkmn->labelWindowId, PIXEL_FILL(0));
    AddTextPrinterParameterized3(sPkmn->labelWindowId, FONT_NORMAL, 0, 0, sLabelColors, TEXT_SKIP_DRAW, str);
    tiles = gWindows[sPkmn->labelWindowId].tileData;
    for (y = 0; y < LABEL_HEIGHT_TILES; y++)
    {
        for (x = 0; x < columns; x++)
            memcpy(sPkmn->asset + (y * columns + x) * TILE_SIZE_4BPP, tiles + (y * LABEL_WIDTH_TILES + x) * TILE_SIZE_4BPP, TILE_SIZE_4BPP);
    }
    size = columns * LABEL_HEIGHT_TILES * TILE_SIZE_4BPP;
    memcpy(sPkmn->asset + size, sLabelPalette, PALETTE_BYTES);
    info->width = columns * 8;
    info->height = LABEL_HEIGHT_TILES * 8;
    info->pixel_format = PIXEL_4BPP_TILED;
    info->palette_format = PALETTE_BGR555_16;
    info->frames = 1;
    info->length = size + PALETTE_BYTES;
    *data = sPkmn->asset;
    return true;
}

static bool ResolveAsset(const AssetKey *key, AssetInfo *info, const u8 **data)
{
    if (sPkmn == NULL)
        return false;
    switch (key->type)
    {
    case ASSET_PKMN_FRONT:
        return ResolveSprite(key, info, data);
    case ASSET_TEXT_LABEL:
        return ResolveLabel(key, info, data);
    }
    return false;
}

static u16 HandleApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength)
{
    PartyUi ui;
    PokemonRecord record;
    struct Pokemon *mon;
    struct GcStateDigest digest;
    u32 i, match;

    switch (opcode)
    {
    case PKMN_PARTY_INFO:
        sPkmn->ui->getUi(&ui);
        ui.count = gPartiesCount[B_TRAINER_PLAYER];
        ui.record_version = PKMN_RECORD_VERSION;
        ui.record_length = PKMN_RECORD_BYTES;
        *outLength = party_ui_encode(&ui, true, out);
        return APP_OK;
    case PKMN_RECORD:
        if (argLength != 1)
            return APP_BAD_ARGS;
        mon = PartyMon(args[0]);
        if (mon == NULL)
            return APP_UNAVAILABLE;
        BuildRecord(args[0], mon, &record);
        *outLength = pkmn_record_encode(&record, out);
        return APP_OK;
    case PKMN_UI_STATE:
        sPkmn->ui->getUi(&ui);
        *outLength = party_ui_encode(&ui, false, out);
        return APP_OK;
    case PKMN_SET_PHASE:
        if (argLength != 3)
            return APP_BAD_ARGS;
        sPkmn->ui->setPhase(args[0], args[1], args[2]);
        return APP_OK;
    case PKMN_STATE_DIGEST:
        GcPokemon_ComputeDigest(&digest);
        match = GcPokemon_CompareWithEntry(&digest);
        memset(out, 0, STATE_DIGEST_BYTES);
        for (i = 0; i < STATE_DIGEST_FIELDS; i++)
            wire_put32(out + 4 * i, digest.crc[i]);
        out[20] = match;
        *outLength = STATE_DIGEST_BYTES;
        return APP_OK;
    }
    return APP_UNKNOWN_OP;
}
