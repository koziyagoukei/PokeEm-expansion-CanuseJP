// Frontier GC Battle: exports this game's own battle presentation so the
// GameCube can draw the battle the way the GBA does.
//
// Everything is taken from the game's data and drawing code: the terrain
// background and textbox (tiles, tilemap, palettes), the healthbox sprite
// sheets with nickname/gender/level printed by the game's text printer at the
// same coordinates battle_interface.c uses, the HP bar tiles, healthbox-font
// digits, and battle messages built from the game's battle text templates.
#include "global.h"
#include "battle.h"
#include "battle_environment.h"
#include "battle_message.h"
#include "data.h"
#include "decompress.h"
#include "graphics.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "constants/characters.h"
#include "gc_extension.h"
#include "frontier_gc_battle.h"

#define TILE_BYTES 32
#define VISIBLE_TILES_W 30
#define VISIBLE_TILES_H 20
#define TEXTBOX_TOP 14
#define TEXTBOX_ROWS 6
#define HEALTHBOX_BG_INDEX 2

// Element indices in gHealthboxElementsGfxTable (see battle_interface.c).
#define ELEMENT_HP_LABEL 1   // "H", "P"
#define ELEMENT_BAR_GREEN 3
#define ELEMENT_BAR_YELLOW 50
#define ELEMENT_BAR_RED 59

static EWRAM_DATA u8 *sGfx = NULL;  // the last resolved asset (valid until the next one)

static const u8 sHealthboxTextColors[3] = {0, 1, 3};  // sHealthBoxTextColor
static const u8 sMessageColors[3] = {15, 1, 6};       // B_WIN_MSG: background 15, foreground 1, shadow 6

// Game battle text templates (battle_message.c) for messages that are selected
// in code rather than through gBattleStringsTable.
static const u8 sText_Trainer1WantsToBattle[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}が\n勝負を しかけてきた！");
static const u8 sText_Trainer1SentOutPkmn[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}は\n{B_OPPONENT_MON1_NAME}を くりだした！");
static const u8 sText_GoPkmn[] = _("{JPN}ゆけっ！ {B_PLAYER_MON1_NAME}！");
static const u8 sText_AttackerUsedX[] = _("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_BUFF3}！");
static const u8 sText_PlayerDefeatedTrainer1[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}との\n勝負に かった！");
static const u8 sText_PlayerLostAgainstTrainer1[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}との\n 勝負に まけた!");
static const u8 sText_FoePkmnPrefix[] = _("{JPN}相手の ");
static const u8 sText_FoePkmnPrefix2[] = _("{JPN}相手の");
static const u8 sText_Eng[] = _("{ENG}");

void FrontierGcBattle_FreeGfx(void)
{
    TRY_FREE_AND_SET_NULL(sGfx);
}

static u8 *NewGfx(u32 size)
{
    FrontierGcBattle_FreeGfx();
    sGfx = AllocZeroedUnchecked(size);
    return sGfx;
}

static void SetImageInfo(AssetInfo *info, u32 w, u32 h, u32 length, u8 pixelFormat, u8 paletteFormat)
{
    info->width = w;
    info->height = h;
    info->pixel_format = pixelFormat;
    info->palette_format = paletteFormat;
    info->frames = 1;
    info->length = length;
}

// --------------------------------------------------------------- tilemaps

// Packs the visible part of a GBA text BG: only referenced tiles are sent and
// palette banks are renumbered from firstBank. mapStride = entries per row of
// one 32x32 screen block (the visible area never crosses into the next block).
static bool PackTilemap(const u32 *compressedTiles, const u32 *compressedMap, u32 rowFrom, u32 rows,
                        const u16 *palettes, u8 firstBank, u8 banks, u8 flags, AssetInfo *info, const u8 **data)
{
    u32 tilesSize = GetDecompressedDataSize(compressedTiles), mapSize = GetDecompressedDataSize(compressedMap);
    u8 *tiles = AllocUnchecked(tilesSize);
    u16 *map = AllocUnchecked(mapSize);
    u16 *remap = NULL;
    u8 *out;
    u32 x, y, used = 0, length, cells = VISIBLE_TILES_W * rows;
    bool ok = FALSE;

    if (tiles == NULL || map == NULL)
        goto done;
    DecompressDataWithHeaderWram(compressedTiles, tiles);
    DecompressDataWithHeaderWram(compressedMap, map);
    remap = AllocUnchecked(1024 * sizeof(u16));
    if (remap == NULL || (rowFrom + rows) * 32 * sizeof(u16) > mapSize)
        goto done;
    memset(remap, 0xFF, 1024 * sizeof(u16));
    for (y = 0; y < rows; y++)
        for (x = 0; x < VISIBLE_TILES_W; x++)
        {
            u16 tile = map[(rowFrom + y) * 32 + x] & 0x3FF;
            if (remap[tile] == 0xFFFF && (tile + 1) * TILE_BYTES <= tilesSize)
                remap[tile] = used++;
        }
    length = TILEMAP_HEADER_BYTES + cells * 2 + used * TILE_BYTES + banks * 32;
    out = NewGfx(length);
    if (out == NULL)
        goto done;
    out[0] = VISIBLE_TILES_W; out[1] = 0;
    out[2] = rows; out[3] = 0;
    out[4] = used; out[5] = used >> 8;
    out[6] = banks;
    out[7] = flags;
    for (y = 0; y < rows; y++)
        for (x = 0; x < VISIBLE_TILES_W; x++)
        {
            u16 e = map[(rowFrom + y) * 32 + x], tile = e & 0x3FF, bank = (e >> 12) & 0xF;
            u16 packed = (remap[tile] == 0xFFFF ? 0 : remap[tile]) | (e & 0x0C00)
                       | ((bank >= firstBank && bank < firstBank + banks ? bank - firstBank : 0) << 12);
            out[TILEMAP_HEADER_BYTES + (y * VISIBLE_TILES_W + x) * 2] = packed;
            out[TILEMAP_HEADER_BYTES + (y * VISIBLE_TILES_W + x) * 2 + 1] = packed >> 8;
        }
    for (x = 0; x < 1024; x++)
        if (remap[x] != 0xFFFF)
            memcpy(out + TILEMAP_HEADER_BYTES + cells * 2 + remap[x] * TILE_BYTES, tiles + x * TILE_BYTES, TILE_BYTES);
    memcpy(out + TILEMAP_HEADER_BYTES + cells * 2 + used * TILE_BYTES, palettes, banks * 32);
    SetImageInfo(info, VISIBLE_TILES_W * 8, rows * 8, length, PIXEL_4BPP_TILEMAP, PALETTE_NONE);
    *data = out;
    ok = TRUE;
done:
    TRY_FREE_AND_SET_NULL(tiles);
    TRY_FREE_AND_SET_NULL(map);
    TRY_FREE_AND_SET_NULL(remap);
    return ok;
}

// --------------------------------------------------------------- healthbox

struct Image { u8 *pixels; u32 widthTiles; };

static void PutPixel(struct Image *img, u32 x, u32 y, u8 color)
{
    u8 *p = img->pixels + ((y / 8) * img->widthTiles + x / 8) * TILE_BYTES + (y % 8) * 4 + (x % 8) / 2;
    *p = (x & 1) ? ((*p & 0x0F) | (color << 4)) : ((*p & 0xF0) | color);
}

static void FillRect(struct Image *img, u32 x, u32 y, u32 w, u32 h, u8 color)
{
    u32 i, j;
    for (j = 0; j < h; j++)
        for (i = 0; i < w; i++)
            PutPixel(img, x + i, y + j, color);
}

// Prints like AddSpriteTextPrinterParameterized6: background colour 0 is
// transparent, so only glyph pixels are written.
static void PrintText(struct Image *img, u8 fontId, const u8 *str, u32 x, u32 y, u32 maxW, u32 maxH)
{
    const u8 *tiles = GcExtension_RenderTextTiles(fontId, sHealthboxTextColors, 0, str);
    u32 i, j;
    for (j = 0; j < 16 && y + j < maxH; j++)
        for (i = 0; i < GC_TEXT_WINDOW_WIDTH * 8 && x + i < maxW; i++)
        {
            const u8 *p = tiles + ((j / 8) * GC_TEXT_WINDOW_WIDTH + i / 8) * TILE_BYTES + (j % 8) * 4 + (i % 8) / 2;
            u8 color = (i & 1) ? (*p >> 4) : (*p & 0xF);
            if (color != 0)
                PutPixel(img, x + i, y + j, color);
        }
}

static bool ResolveHealthbox(const AssetKey *key, AssetInfo *info, const u8 **data)
{
    struct FrontierGcBattleScene scene;
    struct Pokemon *mon;
    bool8 player = key->id == 0;
    u32 heightTiles = player ? 8 : 4, rightTile = player ? 64 : 32, tx, ty, length;
    const u32 *sheet = player ? gHealthboxSinglesPlayerGfx : gHealthboxSinglesOpponentGfx;
    u8 *sheetTiles, name[POKEMON_NAME_LENGTH + 12], lv[16], *ptr, gender;
    struct Image img;
    s32 width;

    FrontierGcBattle_GetScene(&scene);
    mon = player ? scene.player : scene.opponent;
    if (mon == NULL || key->id > 1 || key->extra != GetMonData(mon, MON_DATA_PERSONALITY))
        return FALSE;
    sheetTiles = AllocUnchecked(GetDecompressedDataSize(sheet));
    if (sheetTiles == NULL)
        return FALSE;
    DecompressDataWithHeaderWram(sheet, sheetTiles);
    length = 16 * heightTiles * TILE_BYTES + 32;
    img.pixels = NewGfx(length);
    img.widthTiles = 16;
    if (img.pixels == NULL)
    {
        Free(sheetTiles);
        return FALSE;
    }
    // Left and right sprites side by side, as the two OAM sprites are placed.
    for (ty = 0; ty < heightTiles; ty++)
        for (tx = 0; tx < 16; tx++)
            memcpy(img.pixels + (ty * 16 + tx) * TILE_BYTES,
                   sheetTiles + ((tx < 8 ? 0 : rightTile) + ty * 8 + tx % 8) * TILE_BYTES, TILE_BYTES);
    Free(sheetTiles);

    // UpdateNickInHealthbox
    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringGet_Nickname(name);
    ptr = name + StringLength(name);
    gender = GetMonGender(mon);
    StringCopy(ptr, gender == MON_MALE ? gText_HealthboxGender_Male
                  : gender == MON_FEMALE ? gText_HealthboxGender_Female : gText_HealthboxGender_None);
    FillRect(&img, player ? 16 : 8, 5, 55, 11, HEALTHBOX_BG_INDEX);
    PrintText(&img, GetFontIdToFit(name, FONT_SMALL, 0, 55), name, player ? 16 : 8, 3, 128, heightTiles * 8);

    // UpdateLvlInHealthbox (right sprite starts at x = 64)
    ptr = StringCopy(lv, sText_Eng);
    *ptr++ = CHAR_EXTRA_SYMBOL;
    *ptr++ = CHAR_LV_2;
    ConvertIntToDecimalStringN(ptr, GetMonData(mon, MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
    width = GetStringWidth(FONT_SMALL, lv, 0);
    FillRect(&img, 64 + (player ? 8 : 0), 5, 24, 11, HEALTHBOX_BG_INDEX);
    PrintText(&img, FONT_SMALL, lv, 64 + (player ? 32 : 24) - width, 3, 128, heightTiles * 8);

    // The HP numbers (player side) are drawn by the host while the bar moves.
    if (player)
        FillRect(&img, 40, 24, 56, 8, HEALTHBOX_BG_INDEX);
    memcpy(img.pixels + 16 * heightTiles * TILE_BYTES, gBattleInterface_BallStatusBarPal, 32);
    SetImageInfo(info, 128, heightTiles * 8, length, PIXEL_4BPP_TILED, PALETTE_BGR555_16);
    *data = img.pixels;
    return TRUE;
}

static bool ResolveHpBar(AssetInfo *info, const u8 **data)
{
    u32 i, length = HPBAR_TILES * TILE_BYTES + 32;
    u8 *out = NewGfx(length);
    if (out == NULL)
        return FALSE;
    memcpy(out, gHealthboxElementsGfxTable[ELEMENT_HP_LABEL], 2 * TILE_BYTES);
    for (i = 0; i < 9; i++)
    {
        memcpy(out + (HPBAR_TILE_GREEN + i) * TILE_BYTES, gHealthboxElementsGfxTable[ELEMENT_BAR_GREEN + i], TILE_BYTES);
        memcpy(out + (HPBAR_TILE_YELLOW + i) * TILE_BYTES, gHealthboxElementsGfxTable[ELEMENT_BAR_YELLOW + i], TILE_BYTES);
        memcpy(out + (HPBAR_TILE_RED + i) * TILE_BYTES, gHealthboxElementsGfxTable[ELEMENT_BAR_RED + i], TILE_BYTES);
    }
    memcpy(out + HPBAR_TILES * TILE_BYTES, gBattleInterface_BallDisplayPal, 32);
    SetImageInfo(info, HPBAR_TILES * 8, 8, length, PIXEL_4BPP_TILED, PALETTE_BGR555_16);
    *data = out;
    return TRUE;
}

// Glyphs 0-9 and '/' of the healthbox font, one 8x16 cell each.
static bool ResolveDigits(AssetInfo *info, const u8 **data)
{
    u32 c, y, length = 11 * 2 * TILE_BYTES + 32;
    u8 *out = NewGfx(length);
    if (out == NULL)
        return FALSE;
    for (c = 0; c < 11; c++)
    {
        u8 str[4] = {0xFC, 0x16, c < 10 ? CHAR_0 + c : CHAR_SLASH, EOS};  // {ENG}<glyph>
        const u8 *tiles = GcExtension_RenderTextTiles(FONT_SMALL, sHealthboxTextColors, 0, str);
        for (y = 0; y < 2; y++)
            memcpy(out + (y * 11 + c) * TILE_BYTES, tiles + y * GC_TEXT_WINDOW_WIDTH * TILE_BYTES, TILE_BYTES);
    }
    memcpy(out + 22 * TILE_BYTES, gBattleInterface_BallStatusBarPal, 32);
    SetImageInfo(info, 88, 16, length, PIXEL_4BPP_TILED, PALETTE_BGR555_16);
    *data = out;
    return TRUE;
}

// --------------------------------------------------------------- messages

static u8 *AppendMonName(u8 *dst, struct Pokemon *mon, bool8 foe, const u8 *prefix)
{
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    if (foe && prefix != NULL)
        dst = StringCopy(dst, prefix);
    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringGet_Nickname(name);
    return StringCopy(dst, name);
}

// A small subset of BattleStringExpandPlaceholders: only what the templates
// above use. side = the battler the message is about.
static void ExpandMessage(const u8 *src, u8 *dst, const struct FrontierGcBattleScene *scene, u8 side, u16 move)
{
    struct Pokemon *subject = side ? scene->opponent : scene->player;
    u8 *end = dst + 200;
    while (*src != EOS && dst < end)
    {
        if (*src == B_BUFF_PLACEHOLDER_BEGIN)
        {
            u8 code = src[1];
            src += 2;
            switch (code)
            {
            case B_TXT_BUFF1:  // gText_WhatWillPkmnDo: the active battler's name
            case B_TXT_PLAYER_MON1_NAME: dst = AppendMonName(dst, scene->player, FALSE, NULL); break;
            case B_TXT_OPPONENT_MON1_NAME: dst = AppendMonName(dst, scene->opponent, FALSE, NULL); break;
            case B_TXT_ATK_NAME_WITH_PREFIX:
            case B_TXT_DEF_NAME_WITH_PREFIX:
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: dst = AppendMonName(dst, subject, side, sText_FoePkmnPrefix); break;
            case 0x3F: dst = AppendMonName(dst, subject, side, sText_FoePkmnPrefix2); break;  // B_DEF_NAME_WITH_PREFIX2
            case B_TXT_BUFF3: dst = StringCopy(dst, GetMoveName(move)); break;
            case B_TXT_PLAYER_NAME: dst = StringCopy(dst, gSaveBlock2Ptr->playerName); break;
            case 0x42: // B_TRAINER1_NAME_WITH_CLASS
                dst = StringCopy(dst, GetTrainerClassNameFromId(scene->trainerId));
                *dst++ = CHAR_SPACE;
                dst = StringCopy(dst, GetTrainerNameFromId(scene->trainerId));
                break;
            }
        }
        else if (*src == CHAR_PROMPT_CLEAR || *src == CHAR_PROMPT_SCROLL)
        {
            src++;  // \p and \l: the host shows each message on its own
        }
        else
        {
            *dst++ = *src++;
        }
    }
    *dst = EOS;
}

static const u8 *MessageTemplate(u8 msg)
{
    switch (msg)
    {
    case BMSG_TRAINER_INTRO: return sText_Trainer1WantsToBattle;
    case BMSG_TRAINER_SENT_OUT: return sText_Trainer1SentOutPkmn;
    case BMSG_GO_PKMN: return sText_GoPkmn;
    case BMSG_USED_MOVE: return sText_AttackerUsedX;
    case BMSG_MISSED: return gBattleStringsTable[STRINGID_PKMNAVOIDEDATTACK];
    case BMSG_SUPER_EFFECTIVE: return gBattleStringsTable[STRINGID_SUPEREFFECTIVE];
    case BMSG_NOT_VERY_EFFECTIVE: return gBattleStringsTable[STRINGID_NOTVERYEFFECTIVE];
    case BMSG_NO_EFFECT: return gBattleStringsTable[STRINGID_ITDOESNTAFFECT];
    case BMSG_FAINTED: return gBattleStringsTable[STRINGID_BATTLERFAINTED];
    case BMSG_WIN: return sText_PlayerDefeatedTrainer1;
    case BMSG_LOSE: return sText_PlayerLostAgainstTrainer1;
    case BMSG_FORFEIT: return gBattleStringsTable[STRINGID_FORFEITEDMATCH];
    case BMSG_WHAT_WILL_DO: return gText_WhatWillPkmnDo;
    }
    return NULL;
}

static bool ResolveMessage(const AssetKey *key, AssetInfo *info, const u8 **data)
{
    struct FrontierGcBattleScene scene;
    const u8 *template = MessageTemplate(key->id & 0xFF);
    u8 side = (key->id >> 8) & 1;
    u8 *text;
    bool ok;

    FrontierGcBattle_GetScene(&scene);
    if (template == NULL || key->variant >= MOVES_COUNT_ALL)
        return FALSE;
    text = AllocUnchecked(256);
    if (text == NULL)
        return FALSE;
    ExpandMessage(template, text, &scene, side, key->variant);
    // B_WIN_MSG: 26x4 tiles, FONT_NORMAL, filled with colour 15, textbox palette.
    ok = GcExtension_RenderTextAsset(FONT_NORMAL, sMessageColors, 15, text, 26, 4, gBattleTextboxPalette, info, data);
    Free(text);
    return ok;
}

// --------------------------------------------------------------- entry

bool FrontierGcBattle_ResolveAsset(const AssetKey *key, AssetInfo *info, const u8 **data)
{
    switch (key->type)
    {
    case ASSET_BATTLE_BG:
    {
        u32 env = key->id < BATTLE_ENVIRONMENT_COUNT ? key->id : BATTLE_ENVIRONMENT_PLAIN;
        return PackTilemap(gBattleEnvironmentInfo[env].background.tileset, gBattleEnvironmentInfo[env].background.tilemap,
                           0, VISIBLE_TILES_H, gBattleEnvironmentInfo[env].palette, 2, 3, TILEMAP_FLAG_OPAQUE, info, data);
    }
    case ASSET_BATTLE_TEXTBOX:
        return PackTilemap(gBattleTextboxTiles, gBattleTextboxTilemap, TEXTBOX_TOP, TEXTBOX_ROWS,
                           gBattleTextboxPalette, 0, 2, 0, info, data);
    case ASSET_BATTLE_HEALTHBOX:
        return ResolveHealthbox(key, info, data);
    case ASSET_BATTLE_HPBAR:
        return ResolveHpBar(info, data);
    case ASSET_BATTLE_DIGITS:
        return ResolveDigits(info, data);
    case ASSET_BATTLE_MESSAGE:
        return ResolveMessage(key, info, data);
    }
    return FALSE;
}
