#include "global.h"
#include "battle.h"
#include "battle_info.h"
#include "battle_gimmick.h"
#include "battle_main.h"
#include "battle_message.h"
#include "battle_util.h"
#include "bg.h"
#include "dma3.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"

#define INFO_WIDTH 28
#define INFO_HEIGHT 18
#define INFO_TILEMAP_BYTES 0x1000
#define INFO_PIXELS (INFO_WIDTH * INFO_HEIGHT * 32)
#define INFO_TILE_BYTES (INFO_PIXELS + 32)
#define INFO_PALETTE 15
#define INFO_FIRST_PALETTE 9
#define INFO_PALETTE_COLORS ((16 - INFO_FIRST_PALETTE) * 16)
#define INFO_PAGES 4

struct BattleInfoView
{
    u16 tilemap[INFO_TILEMAP_BYTES / 2];
    u16 *tiles;
    u16 unfaded[INFO_PALETTE_COLORS];
    u16 faded[INFO_PALETTE_COLORS];
    u16 backdropUnfaded;
    u16 backdropFaded;
    u16 displayControl;
    u16 blendControl;
    u16 blendAlpha;
    u16 blendY;
    u16 bgX;
    u16 bgY;
    u8 invisible[MAX_SPRITES / 8];
    u8 *pixels;
    u8 window;
    u8 owner;
    u8 page;
    u8 battlers[NUM_BATTLE_SIDES];
};

// Presentation state only: never saved or included in a battle/link packet.
static EWRAM_DATA u8 sInitialDuration[8] = {0};
static EWRAM_DATA struct BattleInfoView *sView = NULL;
static EWRAM_DATA u32 sBlockedFrame = 0;
static EWRAM_DATA bool32 sBlockInput = FALSE;

static const u16 sPalette[16] =
{
    RGB(30, 31, 31), RGB(3, 4, 4), RGB(1, 13, 6), RGB(23, 4, 4),
    RGB(17, 20, 20), RGB(24, 27, 28), RGB(12, 15, 16), RGB(30, 31, 31),
};

// Keep these small UI labels uncompressed so opening the panel needs no extra
// decompression buffer or OBJ tiles/palettes from the live battle.
static const u8 sTypeLabels[][32 * 16 / 2] =
{
    [TYPE_NONE] = INCGFX_U8("graphics/types/none.png", ".4bpp"),
    [TYPE_NORMAL] = INCGFX_U8("graphics/types/normal.png", ".4bpp"),
    [TYPE_FIGHTING] = INCGFX_U8("graphics/types/fight.png", ".4bpp"),
    [TYPE_FLYING] = INCGFX_U8("graphics/types/flying.png", ".4bpp"),
    [TYPE_POISON] = INCGFX_U8("graphics/types/poison.png", ".4bpp"),
    [TYPE_GROUND] = INCGFX_U8("graphics/types/ground.png", ".4bpp"),
    [TYPE_ROCK] = INCGFX_U8("graphics/types/rock.png", ".4bpp"),
    [TYPE_BUG] = INCGFX_U8("graphics/types/bug.png", ".4bpp"),
    [TYPE_GHOST] = INCGFX_U8("graphics/types/ghost.png", ".4bpp"),
    [TYPE_STEEL] = INCGFX_U8("graphics/types/steel.png", ".4bpp"),
    [TYPE_MYSTERY] = INCGFX_U8("graphics/types/mystery.png", ".4bpp"),
    [TYPE_FIRE] = INCGFX_U8("graphics/types/fire.png", ".4bpp"),
    [TYPE_WATER] = INCGFX_U8("graphics/types/water.png", ".4bpp"),
    [TYPE_GRASS] = INCGFX_U8("graphics/types/grass.png", ".4bpp"),
    [TYPE_ELECTRIC] = INCGFX_U8("graphics/types/electric.png", ".4bpp"),
    [TYPE_PSYCHIC] = INCGFX_U8("graphics/types/psychic.png", ".4bpp"),
    [TYPE_ICE] = INCGFX_U8("graphics/types/ice.png", ".4bpp"),
    [TYPE_DRAGON] = INCGFX_U8("graphics/types/dragon.png", ".4bpp"),
    [TYPE_DARK] = INCGFX_U8("graphics/types/dark.png", ".4bpp"),
    [TYPE_FAIRY] = INCGFX_U8("graphics/types/fairy.png", ".4bpp"),
    [TYPE_STELLAR] = INCGFX_U8("graphics/types/stellar.png", ".4bpp"),
};
static const u8 sStatusLabels[] = INCGFX_U8("graphics/interface/status_icons.png", ".4bpp");

static const u8 *const sPageNames[INFO_PAGES] =
{
    COMPOUND_STRING("{JPN}能力変化"), COMPOUND_STRING("{JPN}壁と補助"),
    COMPOUND_STRING("{JPN}設置技と継続効果"), COMPOUND_STRING("{JPN}全体の場"),
};

static const u8 *const sEffectNames[] =
{
    [BINFO_REFLECT] = COMPOUND_STRING("{JPN}リフレクター"),
    [BINFO_LIGHT_SCREEN] = COMPOUND_STRING("{JPN}ひかりのかべ"),
    [BINFO_AURORA_VEIL] = COMPOUND_STRING("{JPN}オーロラベール"),
    [BINFO_TAILWIND] = COMPOUND_STRING("{JPN}おいかぜ"),
    [BINFO_SAFEGUARD] = COMPOUND_STRING("{JPN}しんぴのまもり"),
    [BINFO_MIST] = COMPOUND_STRING("{JPN}しろいきり"),
    [BINFO_LUCKY_CHANT] = COMPOUND_STRING("{JPN}おまじない"),
    [BINFO_RAINBOW] = COMPOUND_STRING("{JPN}虹"),
    [BINFO_SEA_OF_FIRE] = COMPOUND_STRING("{JPN}火の海"),
    [BINFO_SWAMP] = COMPOUND_STRING("{JPN}湿原"),
    [BINFO_DAMAGE_NON_TYPES] = COMPOUND_STRING("{JPN}継続ダメージ"),
    [BINFO_WEATHER] = COMPOUND_STRING("{JPN}天候"),
    [BINFO_TERRAIN] = COMPOUND_STRING("{JPN}フィールド"),
    [BINFO_TRICK_ROOM] = COMPOUND_STRING("{JPN}トリックルーム"),
    [BINFO_WONDER_ROOM] = COMPOUND_STRING("{JPN}ワンダールーム"),
    [BINFO_MAGIC_ROOM] = COMPOUND_STRING("{JPN}マジックルーム"),
    [BINFO_GRAVITY] = COMPOUND_STRING("{JPN}じゅうりょく"),
    [BINFO_MUD_SPORT] = COMPOUND_STRING("{JPN}どろあそび"),
    [BINFO_WATER_SPORT] = COMPOUND_STRING("{JPN}みずあそび"),
    [BINFO_FAIRY_LOCK] = COMPOUND_STRING("{JPN}フェアリーロック"),
};

static const enum Stat sStats[] = {STAT_ATK, STAT_DEF, STAT_SPATK, STAT_SPDEF, STAT_SPEED, STAT_ACC, STAT_EVASION};

// Halfword run-length backup. A zero token introduces (count, value); other
// tokens are literals. Two passes avoid reserving a whole extra 16 KiB on GBA.
static u32 BackupTiles(u16 *out)
{
    const volatile u16 *src = (const volatile u16 *)BG_CHAR_ADDR(0);
    u32 words = 0;
    for (u32 i = 0; i < INFO_TILE_BYTES / 2;)
    {
        u16 value = src[i];
        u32 count = 1;
        while (i + count < INFO_TILE_BYTES / 2 && src[i + count] == value)
            count++;
        if (value == 0 || count >= 4)
        {
            if (out != NULL)
            {
                out[words] = 0;
                out[words + 1] = count;
                out[words + 2] = value;
            }
            words += 3;
        }
        else
        {
            for (u32 j = 0; j < count; j++)
                if (out != NULL)
                    out[words + j] = value;
            words += count;
        }
        i += count;
    }
    return words * sizeof(u16);
}

static void RestoreTiles(const u16 *src)
{
    volatile u16 *dest = (volatile u16 *)BG_CHAR_ADDR(0);
    for (u32 i = 0; i < INFO_TILE_BYTES / 2;)
    {
        u16 value = *src++;
        u32 count = 1;
        if (value == 0)
        {
            count = *src++;
            value = *src++;
        }
        for (u32 j = 0; j < count; j++)
            dest[i++] = value;
    }
}

static s32 DurationSlot(enum BattleInfoEffect effect, enum BattleSide side)
{
    if (effect <= BINFO_AURORA_VEIL && side < NUM_BATTLE_SIDES)
        return side * 3 + effect;
    if (effect == BINFO_WEATHER)
        return 6;
    if (effect == BINFO_TERRAIN)
        return 7;
    return -1;
}

void BattleInfo_RecordDuration(enum BattleInfoEffect effect, enum BattleSide side, u32 duration)
{
    s32 slot = DurationSlot(effect, side);
    if (slot >= 0)
        sInitialDuration[slot] = min(duration, 255);
}

void BattleInfo_SwapSides(void)
{
    for (u32 i = 0; i < 3; i++)
    {
        u8 temp = sInitialDuration[i];
        sInitialDuration[i] = sInitialDuration[i + 3];
        sInitialDuration[i + 3] = temp;
    }
}

void BattleInfo_Reset(void)
{
    // A new battle may follow a link-error heap reset; do not dereference old UI pointers.
    sView = NULL;
    sBlockInput = FALSE;
    memset(sInitialDuration, 0, sizeof(sInitialDuration));
}

void BattleInfo_GetTurns(enum BattleInfoEffect effect, enum BattleSide side, struct BattleInfoTurns *turns)
{
    u32 remaining = 0, normal = 5, active = FALSE;
    s32 slot = DurationSlot(effect, side);
    memset(turns, 0, sizeof(*turns));
    if (side >= NUM_BATTLE_SIDES || gBattleStruct == NULL)
        return;
    switch (effect)
    {
    case BINFO_REFLECT: remaining = gSideTimers[side].reflectTimer; active = gSideStatuses[side] & SIDE_STATUS_REFLECT; break;
    case BINFO_LIGHT_SCREEN: remaining = gSideTimers[side].lightscreenTimer; active = gSideStatuses[side] & SIDE_STATUS_LIGHTSCREEN; break;
    case BINFO_AURORA_VEIL: remaining = gSideTimers[side].auroraVeilTimer; active = gSideStatuses[side] & SIDE_STATUS_AURORA_VEIL; break;
    case BINFO_TAILWIND: remaining = gSideTimers[side].tailwindTimer; active = gSideStatuses[side] & SIDE_STATUS_TAILWIND; normal = GetConfig(B_TAILWIND_TURNS) >= GEN_5 ? 4 : 3; break;
    case BINFO_SAFEGUARD: remaining = gSideTimers[side].safeguardTimer; active = gSideStatuses[side] & SIDE_STATUS_SAFEGUARD; break;
    case BINFO_MIST: remaining = gSideTimers[side].mistTimer; active = gSideStatuses[side] & SIDE_STATUS_MIST; break;
    case BINFO_LUCKY_CHANT: remaining = gSideTimers[side].luckyChantTimer; active = gSideStatuses[side] & SIDE_STATUS_LUCKY_CHANT; break;
    case BINFO_RAINBOW: remaining = gSideTimers[side].rainbowTimer; active = gSideStatuses[side] & SIDE_STATUS_RAINBOW; normal = 4; break;
    case BINFO_SEA_OF_FIRE: remaining = gSideTimers[side].seaOfFireTimer; active = gSideStatuses[side] & SIDE_STATUS_SEA_OF_FIRE; normal = 4; break;
    case BINFO_SWAMP: remaining = gSideTimers[side].swampTimer; active = gSideStatuses[side] & SIDE_STATUS_SWAMP; normal = 4; break;
    case BINFO_DAMAGE_NON_TYPES: remaining = gSideTimers[side].damageNonTypesTimer; active = gSideStatuses[side] & SIDE_STATUS_DAMAGE_NON_TYPES; break;
    case BINFO_WEATHER: remaining = gBattleStruct->weatherDuration; active = gBattleWeather != B_WEATHER_NONE; break;
    case BINFO_TERRAIN: remaining = gFieldTimers.terrainTimer; active = gFieldTimers.terrain != B_TERRAIN_NONE; break;
    case BINFO_TRICK_ROOM: remaining = gFieldTimers.trickRoomTimer; active = gFieldStatuses & STATUS_FIELD_TRICK_ROOM; break;
    case BINFO_WONDER_ROOM: remaining = gFieldTimers.wonderRoomTimer; active = gFieldStatuses & STATUS_FIELD_WONDER_ROOM; break;
    case BINFO_MAGIC_ROOM: remaining = gFieldTimers.magicRoomTimer; active = gFieldStatuses & STATUS_FIELD_MAGIC_ROOM; break;
    case BINFO_GRAVITY: remaining = gFieldTimers.gravityTimer; active = gFieldStatuses & STATUS_FIELD_GRAVITY; break;
    case BINFO_MUD_SPORT: remaining = gFieldTimers.mudSportTimer; active = gFieldStatuses & STATUS_FIELD_MUDSPORT; break;
    case BINFO_WATER_SPORT: remaining = gFieldTimers.waterSportTimer; active = gFieldStatuses & STATUS_FIELD_WATERSPORT; break;
    case BINFO_FAIRY_LOCK: remaining = gFieldTimers.fairyLockTimer; active = gFieldStatuses & STATUS_FIELD_FAIRY_LOCK; normal = 2; break;
    }
    if (!active)
        return;
    turns->active = TRUE;
    if (remaining == 0)
    {
        turns->permanent = TRUE;
        return;
    }
    u32 initial = slot >= 0 ? sInitialDuration[slot] : normal;
    // Untracked custom starting states use their normal duration without exposing an item.
    if (initial == 0)
        initial = normal;
    initial = max(initial, remaining);
    turns->elapsed = initial - remaining + 1;
    turns->total = turns->elapsed <= normal ? normal : initial;
}

static void Print(const u8 *text, u32 x, u32 y, u32 color)
{
    u32 background = y < 16 ? 5 : 7;
    u8 colors[] = {background, color, background};
    AddTextPrinterParameterized4(sView->window, FONT_SMALL, x, y, 0, 0, colors, TEXT_SKIP_DRAW, text);
}

static void FormatTurns(u8 *text, const struct BattleInfoTurns *turns)
{
    if (!turns->active)
        StringCopy(text, COMPOUND_STRING("{JPN}-"));
    else if (turns->permanent)
        StringCopy(text, COMPOUND_STRING("{JPN}継続"));
    else
    {
        u8 *end = StringCopy(text, COMPOUND_STRING("{ENG}"));
        end = ConvertIntToDecimalStringN(end, turns->elapsed, STR_CONV_MODE_LEFT_ALIGN, 3);
        end = StringCopy(end, COMPOUND_STRING("/"));
        ConvertIntToDecimalStringN(end, turns->total, STR_CONV_MODE_LEFT_ALIGN, 3);
    }
}

static bool32 PrintEffect(enum BattleInfoEffect effect, enum BattleSide side, u32 x, u32 y)
{
    struct BattleInfoTurns turns;
    u8 text[32];
    BattleInfo_GetTurns(effect, side, &turns);
    if (!turns.active)
        return FALSE;
    FormatTurns(text, &turns);
    Print(sEffectNames[effect], x, y, 1);
    Print(text, x + 79, y, 1);
    return TRUE;
}

static enum BattlerId NextBattler(enum BattleSide side, enum BattlerId current)
{
    for (u32 i = 1; i <= gBattlersCount; i++)
    {
        enum BattlerId candidate = (current + i) % gBattlersCount;
        if (GetBattlerSide(candidate) == side && !(gAbsentBattlerFlags & (1u << candidate)) && gBattleMons[candidate].hp != 0)
            return candidate;
    }
    return MAX_BATTLERS_COUNT;
}

static void SetTilePalette(u32 x, u32 y, u32 width, u32 height, u32 palette)
{
    u16 *map = GetBgTilemapBuffer(0);
    for (u32 row = 0; row < height / 8; row++)
        for (u32 col = 0; col < width / 8; col++)
        {
            u32 offset = (1 + y / 8 + row) * 32 + 1 + x / 8 + col;
            map[offset] = (map[offset] & 0x0fff) | (palette << 12);
        }
}

static void DrawMonDetails(u32 column)
{
    enum BattlerId battler = sView->battlers[column];
    if (battler >= gBattlersCount)
        return;
    u32 x = column * 112;
    struct Pokemon *mon = GetBattlerMon(battler);
    struct Pokemon *illusion = gBattleStruct->illusion[battler].state == ILLUSION_ON ? gBattleStruct->illusion[battler].mon : NULL;
    enum Species species = gBattleMons[battler].species;
    if (illusion != NULL)
    {
        mon = illusion;
        species = GetMonData(mon, MON_DATA_SPECIES);
    }
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY);
    enum Species iconSpecies = GetIconSpecies(species, personality);
    const u16 *iconPalette = GetValidMonIconPalettePtr(iconSpecies);
#if P_GENDER_DIFFERENCES
    if (gSpeciesInfo[iconSpecies].iconSpriteFemale != NULL && IsPersonalityFemale(iconSpecies, personality))
        iconPalette = gMonIconPaletteTable[gSpeciesInfo[iconSpecies].iconPalIndexFemale].data;
#endif
    LoadPalette(iconPalette, BG_PLTT_ID(13 + column), PLTT_SIZE_4BPP);
    FillWindowPixelRect(sView->window, PIXEL_FILL(0), x + 8, 48, 32, 32);
    BlitBitmapRectToWindow(sView->window, GetMonIconPtr(species, personality), 0, 0, 32, 64, x + 8, 48, 32, 32);
    SetTilePalette(x + 8, 48, 32, 32, 13 + column);

    enum Type types[3];
    GetBattlerTypes(battler, FALSE, types);
    // Do not reveal an unbroken Illusion's original typing before a public type change.
    if (illusion != NULL && GetActiveGimmick(battler) != GIMMICK_TERA
     && gBattleMons[battler].types[0] == GetSpeciesType(gBattleMons[battler].species, 0)
     && gBattleMons[battler].types[1] == GetSpeciesType(gBattleMons[battler].species, 1))
    {
        types[0] = GetSpeciesType(species, 0);
        types[1] = GetSpeciesType(species, 1);
    }
    u32 count = 0;
    for (u32 i = 0; i < ARRAY_COUNT(types); i++)
    {
        if (types[i] == TYPE_NONE || types[i] >= NUMBER_OF_MON_TYPES || (i > 0 && types[i] == TYPE_MYSTERY))
            continue;
        bool32 duplicate = FALSE;
        for (u32 j = 0; j < i; j++)
            duplicate |= types[i] == types[j];
        if (duplicate)
            continue;
        u32 dx = x + 40 + (count % 2) * 32, dy = 48 + (count / 2) * 16;
        FillWindowPixelRect(sView->window, PIXEL_FILL(0), dx, dy, 32, 16);
        BlitBitmapRectToWindow(sView->window, sTypeLabels[types[i]], 0, 0, 32, 16, dx, dy, 32, 16);
        SetTilePalette(dx, dy, 32, 16, 10 + gTypesInfo[types[i]].palette - 13);
        count++;
    }

    u32 status = gBattleMons[battler].status1;
    s32 statusFrame = -1;
    if (status & STATUS1_SLEEP) statusFrame = 2;
    else if (status & STATUS1_PSN_ANY) statusFrame = 0;
    else if (status & STATUS1_BURN) statusFrame = 4;
    else if (status & STATUS1_FREEZE) statusFrame = 3;
    else if (status & STATUS1_PARALYSIS) statusFrame = 1;
    else if (status & STATUS1_FROSTBITE) statusFrame = 7;
    if (statusFrame >= 0)
    {
        FillWindowPixelRect(sView->window, PIXEL_FILL(0), x + 72, 72, 32, 8);
        BlitBitmapRectToWindow(sView->window, sStatusLabels + statusFrame * 128, 0, 0, 32, 8, x + 72, 72, 32, 8);
        SetTilePalette(x + 72, 72, 32, 8, 9);
    }
}

static void PrintNickname(u8 *name, u32 x)
{
    u32 offset = 0, last = 0;
    while (name[offset] != EOS)
    {
        last = offset;
        if (name[offset] == EXT_CTRL_CODE_BEGIN)
            offset += 1 + max(1, GetExtCtrlCodeLength(name[offset + 1]));
        else if (name[offset] == CHAR_EXTRA_SYMBOL || name[offset] == CHAR_KEYPAD_ICON)
            offset += 2;
        else
            offset++;
        u8 saved = name[offset];
        name[offset] = EOS;
        if (GetStringWidth(FONT_SMALL, name, 0) > 104)
        {
            name[last] = EOS;
            break;
        }
        name[offset] = saved;
    }
    Print(name, x, 34, 1);
}

static void PrintStats(u32 column)
{
    u32 x = column ? 119 : 4;
    enum BattlerId battler = sView->battlers[column];
    u8 text[POKEMON_NAME_BUFFER_SIZE];
    if (battler >= gBattlersCount)
    {
        Print(COMPOUND_STRING("{JPN}ポケモンなし"), x, 34, 6);
        return;
    }
    struct Pokemon *mon = GetBattlerMon(battler);
    if (gBattleStruct->illusion[battler].state == ILLUSION_ON && gBattleStruct->illusion[battler].mon != NULL)
        mon = gBattleStruct->illusion[battler].mon;
    GetMonData(mon, MON_DATA_NICKNAME, text);
    StringGet_Nickname(text);
    PrintNickname(text, x);
    for (u32 i = 0; i < ARRAY_COUNT(sStats); i++)
    {
        u8 value[16];
        s32 stage = max(0, min(gBattleMons[battler].statStages[sStats[i]], MAX_STAT_STAGE)) - DEFAULT_STAT_STAGE;
        u8 *end = StringCopy(value, stage > 0 ? COMPOUND_STRING("{ENG}+") : stage < 0 ? COMPOUND_STRING("{ENG}-") : COMPOUND_STRING("{ENG}"));
        ConvertIntToDecimalStringN(end, abs(stage), STR_CONV_MODE_LEFT_ALIGN, 1);
        u32 sx = column * 112 + 4 + (i % 2) * 56, sy = 88 + (i / 2) * 13;
        Print(gStatNamesTable[sStats[i]], sx, sy, 1);
        Print(value, sx + 40, sy, stage > 0 ? 2 : stage < 0 ? 3 : 6);
    }
}

static bool32 PrintHazard(const u8 *name, u32 amount, u32 x, u32 y)
{
    if (amount == 0)
        return FALSE;
    u8 text[12];
    u8 *end = StringCopy(text, COMPOUND_STRING("{ENG}"));
    ConvertIntToDecimalStringN(end, amount, STR_CONV_MODE_LEFT_ALIGN, 1);
    Print(name, x, y, 1);
    Print(text, x + 79, y, 1);
    return TRUE;
}

static bool32 HasHazard(enum BattleSide side, enum Hazards hazard)
{
    for (u32 i = 0; i < gBattleStruct->numHazards[side]; i++)
        if (gBattleStruct->hazardsQueue[side][i] == hazard)
            return TRUE;
    return FALSE;
}

static const u8 *WeatherName(void)
{
    if (gBattleWeather & B_WEATHER_RAIN_PRIMAL) return COMPOUND_STRING("{JPN}大雨");
    if (gBattleWeather & B_WEATHER_SUN_PRIMAL) return COMPOUND_STRING("{JPN}強い日差し");
    if (gBattleWeather & B_WEATHER_RAIN) return COMPOUND_STRING("{JPN}雨");
    if (gBattleWeather & B_WEATHER_SUN) return COMPOUND_STRING("{JPN}日差し");
    if (gBattleWeather & B_WEATHER_SANDSTORM) return COMPOUND_STRING("{JPN}砂あらし");
    if (gBattleWeather & B_WEATHER_SNOW) return COMPOUND_STRING("{JPN}雪");
    if (gBattleWeather & B_WEATHER_HAIL) return COMPOUND_STRING("{JPN}あられ");
    if (gBattleWeather & B_WEATHER_STRONG_WINDS) return COMPOUND_STRING("{JPN}乱気流");
    if (gBattleWeather & B_WEATHER_FOG) return COMPOUND_STRING("{JPN}霧");
    return COMPOUND_STRING("{JPN}なし");
}

static const u8 *TerrainName(void)
{
    switch (gFieldTimers.terrain)
    {
    case B_TERRAIN_GRASSY: return COMPOUND_STRING("{JPN}グラス");
    case B_TERRAIN_MISTY: return COMPOUND_STRING("{JPN}ミスト");
    case B_TERRAIN_ELECTRIC: return COMPOUND_STRING("{JPN}エレキ");
    case B_TERRAIN_PSYCHIC: return COMPOUND_STRING("{JPN}サイコ");
    default: return COMPOUND_STRING("{JPN}なし");
    }
}

static void Draw(void)
{
    FillWindowPixelBuffer(sView->window, PIXEL_FILL(7));
    FillWindowPixelRect(sView->window, PIXEL_FILL(5), 0, 0, 224, 16);
    FillWindowPixelRect(sView->window, PIXEL_FILL(4), 112, 18, 1, 126);
    Print(COMPOUND_STRING("{L_BUTTON}"), 4, 1, 1);
    Print(COMPOUND_STRING("{R_BUTTON}"), 208, 1, 1);
    const u8 *title = sPageNames[sView->page];
    Print(title, (224 - GetStringWidth(FONT_SMALL, title, 0)) / 2, 1, 1);
    enum BattleSide ownSide = GetBattlerSide(sView->owner);
    if (sView->page != 3)
    {
        Print(COMPOUND_STRING("{JPN}自分"), 4, 19, 2);
        Print(COMPOUND_STRING("{JPN}相手"), 119, 19, 3);
    }
    if (sView->page == 0)
    {
        PrintStats(0);
        PrintStats(1);
    }
    else if (sView->page == 1)
    {
        static const enum BattleInfoEffect effects[] = {BINFO_REFLECT, BINFO_LIGHT_SCREEN, BINFO_AURORA_VEIL, BINFO_TAILWIND, BINFO_SAFEGUARD, BINFO_MIST, BINFO_LUCKY_CHANT};
        for (u32 column = 0; column < NUM_BATTLE_SIDES; column++)
        {
            u32 row = 0, x = column ? 119 : 4;
            for (u32 i = 0; i < ARRAY_COUNT(effects); i++)
                row += PrintEffect(effects[i], ownSide ^ column, x, 35 + row * 14);
            if (row == 0)
                Print(COMPOUND_STRING("{JPN}なし"), x, 35, 6);
        }
    }
    else if (sView->page == 2)
    {
        for (u32 column = 0; column < NUM_BATTLE_SIDES; column++)
        {
            enum BattleSide side = ownSide ^ column;
            u32 x = column ? 119 : 4, row = 0;
            row += PrintHazard(COMPOUND_STRING("{JPN}まきびし"), gSideTimers[side].spikesAmount, x, 35 + row * 13);
            row += PrintHazard(COMPOUND_STRING("{JPN}どくびし"), gSideTimers[side].toxicSpikesAmount, x, 35 + row * 13);
            row += PrintHazard(COMPOUND_STRING("{JPN}ステルスロック"), HasHazard(side, HAZARDS_STEALTH_ROCK), x, 35 + row * 13);
            row += PrintHazard(COMPOUND_STRING("{JPN}ねばねばネット"), HasHazard(side, HAZARDS_STICKY_WEB), x, 35 + row * 13);
            for (enum BattleInfoEffect effect = BINFO_RAINBOW; effect <= BINFO_DAMAGE_NON_TYPES; effect++)
                row += PrintEffect(effect, side, x, 35 + row * 13);
            if (row == 0)
                Print(COMPOUND_STRING("{JPN}なし"), x, 35, 6);
        }
    }
    else
    {
        FillWindowPixelRect(sView->window, PIXEL_FILL(7), 0, 17, 224, 127);
        u32 row = 0;
        for (enum BattleInfoEffect effect = BINFO_WEATHER; effect <= BINFO_FAIRY_LOCK; effect++)
        {
            struct BattleInfoTurns turns;
            u8 text[32];
            BattleInfo_GetTurns(effect, ownSide, &turns);
            if (!turns.active)
                continue;
            u32 y = 22 + row++ * 13;
            FormatTurns(text, &turns);
            Print(sEffectNames[effect], 4, y, turns.active ? 1 : 6);
            if (effect == BINFO_WEATHER)
                Print(WeatherName(), 113, y, 1);
            else if (effect == BINFO_TERRAIN)
                Print(TerrainName(), 113, y, 1);
            Print(text, 190, y, turns.active ? 1 : 6);
        }
        if (row == 0)
            Print(COMPOUND_STRING("{JPN}なし"), 4, 22, 6);
    }
    PutWindowTilemap(sView->window);
    if (sView->page == 0)
        for (u32 column = 0; column < NUM_BATTLE_SIDES; column++)
            DrawMonDetails(column);
    CopyWindowToVram(sView->window, COPYWIN_FULL);
}

static bool32 Open(enum BattlerId battler)
{
    if (sView != NULL || gBattleStruct == NULL || gBattlersCount == 0 || battler >= gBattlersCount || GetBgTilemapBuffer(0) == NULL || GetBgAttribute(0, BG_ATTR_METRIC) != INFO_TILEMAP_BYTES)
        return FALSE;
    struct BattleInfoView *view = AllocZeroedUnchecked(sizeof(*view));
    if (view == NULL)
        return FALSE;
    view->pixels = AllocZeroedUnchecked(INFO_PIXELS);
    if (view->pixels == NULL)
    {
        Free(view);
        return FALSE;
    }
    view->tiles = AllocUnchecked(BackupTiles(NULL));
    if (view->tiles == NULL)
    {
        Free(view->pixels);
        Free(view);
        return FALSE;
    }
    BackupTiles(view->tiles);
    const struct WindowTemplate template = {0, 1, 1, INFO_WIDTH, INFO_HEIGHT, INFO_PALETTE, 1};
    view->window = AddWindowWithoutTileMap(&template);
    if (view->window == WINDOW_NONE)
    {
        Free(view->tiles);
        Free(view->pixels);
        Free(view);
        return FALSE;
    }
    gWindows[view->window].tileData = view->pixels;
    memcpy(view->tilemap, GetBgTilemapBuffer(0), sizeof(view->tilemap));
    memcpy(view->unfaded, &gPlttBufferUnfaded[BG_PLTT_ID(INFO_FIRST_PALETTE)], sizeof(view->unfaded));
    memcpy(view->faded, &gPlttBufferFaded[BG_PLTT_ID(INFO_FIRST_PALETTE)], sizeof(view->faded));
    view->backdropUnfaded = gPlttBufferUnfaded[0];
    view->backdropFaded = gPlttBufferFaded[0];
    view->bgX = gBattle_BG0_X;
    view->bgY = gBattle_BG0_Y;
    view->displayControl = GetGpuReg(REG_OFFSET_DISPCNT);
    view->blendControl = GetGpuReg(REG_OFFSET_BLDCNT);
    view->blendAlpha = GetGpuReg(REG_OFFSET_BLDALPHA);
    view->blendY = GetGpuReg(REG_OFFSET_BLDY);
    for (u32 i = 0; i < MAX_SPRITES; i++)
        if (gSprites[i].invisible)
            view->invisible[i / 8] |= 1u << (i % 8);
    view->owner = battler;
    view->battlers[0] = battler;
    view->battlers[1] = NextBattler(GetBattlerSide(battler) ^ BIT_SIDE, MAX_BATTLERS_COUNT - 1);
    sView = view;
    gPlayerDpadHoldFrames = 0;
    gBattle_BG0_X = gBattle_BG0_Y = 0;
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    LoadPalette(sPalette, BG_PLTT_ID(INFO_PALETTE), sizeof(sPalette));
    LoadPalette(sPalette, 0, sizeof(u16));
    LoadPalette(gStatusPal_Icons, BG_PLTT_ID(9), PLTT_SIZE_4BPP);
    LoadPalette(gMoveTypes_Pal, BG_PLTT_ID(10), 3 * PLTT_SIZE_4BPP);
    CpuFill16(0xF000, GetBgTilemapBuffer(0), INFO_TILEMAP_BYTES);
    // BG palette index zero is transparent; keep the information panel opaque.
    CpuFill16(0x7777, (void *)BG_CHAR_ADDR(0), 32);
    Draw();
    return TRUE;
}

static void Close(bool32 restore)
{
    if (sView == NULL)
        return;
    struct BattleInfoView *view = sView;
    bool32 hasWindow = gWindows[view->window].tileData == view->pixels;
    if (hasWindow)
        RemoveWindow(view->window);
    if (restore && hasWindow && GetBgTilemapBuffer(0) != NULL)
    {
        memcpy(GetBgTilemapBuffer(0), view->tilemap, sizeof(view->tilemap));
        RestoreTiles(view->tiles);
        CopyBgTilemapBufferToVram(0);
        memcpy(&gPlttBufferUnfaded[BG_PLTT_ID(INFO_FIRST_PALETTE)], view->unfaded, sizeof(view->unfaded));
        memcpy(&gPlttBufferFaded[BG_PLTT_ID(INFO_FIRST_PALETTE)], view->faded, sizeof(view->faded));
        gPlttBufferUnfaded[0] = view->backdropUnfaded;
        gPlttBufferFaded[0] = view->backdropFaded;
        gBattle_BG0_X = view->bgX;
        gBattle_BG0_Y = view->bgY;
        SetGpuReg(REG_OFFSET_DISPCNT, view->displayControl);
        SetGpuReg(REG_OFFSET_BLDCNT, view->blendControl);
        SetGpuReg(REG_OFFSET_BLDALPHA, view->blendAlpha);
        SetGpuReg(REG_OFFSET_BLDY, view->blendY);
        for (u32 i = 0; i < MAX_SPRITES; i++)
            gSprites[i].invisible = (view->invisible[i / 8] >> (i % 8)) & 1;
    }
    sView = NULL;
    Free(view->tiles);
    Free(view);
    gPlayerDpadHoldFrames = 0;
    sBlockedFrame = gMain.vblankCounter1;
    sBlockInput = TRUE;
}

void BattleInfo_Free(void)
{
    Close(FALSE);
}

void BattleInfo_HideSprites(void)
{
    if (sView != NULL)
        for (u32 i = 0; i < MAX_SPRITES; i++)
            gSprites[i].invisible = TRUE;
}

bool32 BattleInfo_HandleInput(enum BattlerId battler, bool32 canOpen)
{
    if (sBlockInput)
    {
        if (sBlockedFrame == gMain.vblankCounter1)
            return TRUE;
        sBlockInput = FALSE;
    }
    if (sView != NULL)
    {
        if (sView->owner != battler || IsDma3ManagerBusyWithBgCopy())
            return TRUE;
        if (JOY_NEW(B_BUTTON | SELECT_BUTTON) || (JOY_NEW(A_BUTTON) && !JOY_NEW(L_BUTTON | R_BUTTON)))
        {
            Close(TRUE);
            PlaySE(SE_SELECT);
        }
        else if (JOY_NEW(L_BUTTON | R_BUTTON))
        {
            sView->page = (sView->page + (JOY_NEW(R_BUTTON) ? 1 : INFO_PAGES - 1)) % INFO_PAGES;
            Draw();
            PlaySE(SE_SELECT);
        }
        else if (sView->page == 0 && JOY_NEW(DPAD_ANY))
        {
            enum BattleSide side = GetBattlerSide(sView->owner);
            if (JOY_NEW(DPAD_LEFT | DPAD_UP | DPAD_DOWN))
                sView->battlers[0] = NextBattler(side, sView->battlers[0]);
            if (JOY_NEW(DPAD_RIGHT | DPAD_UP | DPAD_DOWN))
                sView->battlers[1] = NextBattler(side ^ BIT_SIDE, sView->battlers[1]);
            Draw();
            PlaySE(SE_SELECT);
        }
        return TRUE;
    }
    if (canOpen && JOY_NEW(SELECT_BUTTON) && !(DEBUG_BATTLE_MENU && JOY_HELD(L_BUTTON)))
    {
        if (!IsDma3ManagerBusyWithBgCopy())
        {
            Open(battler);
            PlaySE(SE_SELECT);
        }
        return TRUE;
    }
    return FALSE;
}

#if TESTING
bool32 Test_BattleInfoOpen(enum BattlerId battler) { return Open(battler); }
void Test_BattleInfoClose(void) { Close(TRUE); }
u32 Test_BattleInfoWindow(void) { return sView == NULL ? WINDOW_NONE : sView->window; }
void Test_BattleInfoPage(u32 page) { if (sView != NULL) { sView->page = page % INFO_PAGES; Draw(); } }
enum BattlerId Test_BattleInfoSelected(u32 column) { return sView == NULL || column >= NUM_BATTLE_SIDES ? MAX_BATTLERS_COUNT : sView->battlers[column]; }
#endif
