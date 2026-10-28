#include "global.h"
#include "test/test.h"
#include "battle.h"
#include "battle_info.h"
#include "battle_script_commands.h"
#include "battle_util.h"
#include "bg.h"
#include "dma3.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "palette.h"
#include "pokemon.h"
#include "random.h"
#include "sprite.h"
#include "string_util.h"
#include "window.h"
#include "constants/abilities.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/rgb.h"

const volatile bool8 gTestBattleInfoCapture = FALSE;

static void InitState(void)
{
    BattleInfo_Reset();
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    memset(gSideTimers, 0, sizeof(gSideTimers));
    memset(gSideStatuses, 0, sizeof(gSideStatuses));
    memset(&gFieldTimers, 0, sizeof(gFieldTimers));
    memset(gBattleMons, 0, sizeof(gBattleMons));
    gBattleWeather = 0;
    gFieldStatuses = 0;
    gAbsentBattlerFlags = 0;
    gBattlersCount = MAX_BATTLERS_COUNT;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE;
    for (u32 i = 0; i < gBattlersCount; i++)
    {
        gBattlerPositions[i] = i;
        gBattlerPartyIndexes[i] = i / 2;
        gBattleMons[i].hp = 100;
        gBattleMons[i].species = SPECIES_PIKACHU;
        for (u32 j = 0; j < NUM_BATTLE_STATS; j++)
            gBattleMons[i].statStages[j] = DEFAULT_STAT_STAGE;
    }
}

static void FreeState(void)
{
    BattleInfo_Free();
    FREE_AND_SET_NULL(gBattleStruct);
}

TEST("Battle info: extension remains hidden until the sixth active turn")
{
    InitState();
    enum BattleInfoEffect effect = BINFO_REFLECT;
    PARAMETRIZE { effect = BINFO_REFLECT; }
    PARAMETRIZE { effect = BINFO_LIGHT_SCREEN; }
    PARAMETRIZE { effect = BINFO_AURORA_VEIL; }
    PARAMETRIZE { effect = BINFO_WEATHER; }
    PARAMETRIZE { effect = BINFO_TERRAIN; }
    BattleInfo_RecordDuration(effect, B_SIDE_OPPONENT, 8);
    for (u32 elapsed = 1; elapsed <= 8; elapsed++)
    {
        u32 remaining = 9 - elapsed;
        gSideStatuses[B_SIDE_OPPONENT] = SIDE_STATUS_SCREEN_ANY;
        gSideTimers[B_SIDE_OPPONENT].reflectTimer = remaining;
        gSideTimers[B_SIDE_OPPONENT].lightscreenTimer = remaining;
        gSideTimers[B_SIDE_OPPONENT].auroraVeilTimer = remaining;
        gBattleWeather = B_WEATHER_RAIN_NORMAL;
        gBattleStruct->weatherDuration = remaining;
        gFieldTimers.terrain = B_TERRAIN_GRASSY;
        gFieldTimers.terrainTimer = remaining;
        struct BattleInfoTurns turns;
        BattleInfo_GetTurns(effect, B_SIDE_OPPONENT, &turns);
        EXPECT(turns.active);
        EXPECT(!turns.permanent);
        EXPECT_EQ(turns.elapsed, elapsed);
        EXPECT_EQ(turns.total, elapsed <= 5 ? 5 : 8);
    }
    FreeState();
}

TEST("Battle info: ordinary effects never advertise an extension")
{
    InitState();
    gSideStatuses[B_SIDE_PLAYER] = SIDE_STATUS_REFLECT;
    BattleInfo_RecordDuration(BINFO_REFLECT, B_SIDE_PLAYER, 5);
    for (u32 remaining = 5; remaining > 0; remaining--)
    {
        struct BattleInfoTurns turns;
        gSideTimers[B_SIDE_PLAYER].reflectTimer = remaining;
        BattleInfo_GetTurns(BINFO_REFLECT, B_SIDE_PLAYER, &turns);
        EXPECT_EQ(turns.elapsed, 6 - remaining);
        EXPECT_EQ(turns.total, 5);
    }
    FreeState();
}

TEST("Battle info: expired effects are absent and permanent effects have no countdown")
{
    InitState();
    struct BattleInfoTurns turns;
    BattleInfo_RecordDuration(BINFO_REFLECT, B_SIDE_PLAYER, 8);
    BattleInfo_GetTurns(BINFO_REFLECT, B_SIDE_PLAYER, &turns);
    EXPECT(!turns.active);
    gBattleWeather = B_WEATHER_SUN_PRIMAL;
    BattleInfo_GetTurns(BINFO_WEATHER, B_SIDE_PLAYER, &turns);
    EXPECT(turns.active);
    EXPECT(turns.permanent);
    EXPECT_EQ(turns.total, 0);
    gFieldTimers.terrain = B_TERRAIN_ELECTRIC;
    BattleInfo_GetTurns(BINFO_TERRAIN, B_SIDE_PLAYER, &turns);
    EXPECT(turns.permanent);
    FreeState();
}

TEST("Battle info: Court Change moves the hidden duration with each wall")
{
    InitState();
    BattleInfo_RecordDuration(BINFO_REFLECT, B_SIDE_PLAYER, 8);
    BattleInfo_RecordDuration(BINFO_REFLECT, B_SIDE_OPPONENT, 5);
    BattleInfo_SwapSides();
    gSideStatuses[B_SIDE_PLAYER] = gSideStatuses[B_SIDE_OPPONENT] = SIDE_STATUS_REFLECT;
    gSideTimers[B_SIDE_PLAYER].reflectTimer = 3;
    gSideTimers[B_SIDE_OPPONENT].reflectTimer = 3;
    struct BattleInfoTurns turns;
    BattleInfo_GetTurns(BINFO_REFLECT, B_SIDE_OPPONENT, &turns);
    EXPECT_EQ(turns.elapsed, 6);
    EXPECT_EQ(turns.total, 8);
    BattleInfo_GetTurns(BINFO_REFLECT, B_SIDE_PLAYER, &turns);
    EXPECT_EQ(turns.elapsed, 3);
    EXPECT_EQ(turns.total, 5);
    FreeState();
}

TEST("Battle info: replacing a field resets its elapsed count")
{
    InitState();
    gBattleWeather = B_WEATHER_RAIN_NORMAL;
    BattleInfo_RecordDuration(BINFO_WEATHER, B_SIDE_PLAYER, 8);
    gBattleStruct->weatherDuration = 3;
    struct BattleInfoTurns turns;
    BattleInfo_GetTurns(BINFO_WEATHER, B_SIDE_PLAYER, &turns);
    EXPECT_EQ(turns.elapsed, 6);
    BattleInfo_RecordDuration(BINFO_WEATHER, B_SIDE_PLAYER, 5);
    gBattleStruct->weatherDuration = 5;
    BattleInfo_GetTurns(BINFO_WEATHER, B_SIDE_PLAYER, &turns);
    EXPECT_EQ(turns.elapsed, 1);
    EXPECT_EQ(turns.total, 5);
    FreeState();
}

static void InitUi(void)
{
    InitState();
    ResetSpriteData();
    ResetBgsAndClearDma3BusyFlags(0);
    static const struct BgTemplate background = {0, 0, 24, 2, 0, 0, 0};
    InitBgsFromTemplates(0, &background, 1);
    SetBgTilemapBuffer(0, AllocZeroed(0x1000));
    static const struct WindowTemplate windows[] =
    {
        {0, 1, 47, 28, 4, 5, 0x300}, DUMMY_WIN_TEMPLATE,
    };
    InitWindows(windows);
    FillWindowPixelBuffer(0, PIXEL_FILL(3));
    PutWindowTilemap(0);
    for (u32 side = 0; side < 2; side++)
        for (u32 i = 0; i < 2; i++)
        {
            CreateRandomMonWithIVs(&gParties[side][i], side ? SPECIES_GENGAR : SPECIES_PIKACHU, 50, 31);
            u32 battler = i * 2 + side;
            gBattleMons[battler].species = side ? SPECIES_GENGAR : SPECIES_PIKACHU;
            gBattleMons[battler].types[0] = GetSpeciesType(gBattleMons[battler].species, 0);
            gBattleMons[battler].types[1] = GetSpeciesType(gBattleMons[battler].species, 1);
            gBattleMons[battler].types[2] = TYPE_MYSTERY;
        }
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 160;
    gMain.newKeys = 0;
}

static void FreeUi(void)
{
    while (IsDma3ManagerBusyWithBgCopy())
        ProcessDma3Requests();
    FreeState();
    FreeAllWindowBuffers();
    void *tilemap = GetBgTilemapBuffer(0);
    UnsetBgTilemapBuffer(0);
    Free(tilemap);
}

static void FlushCopies(void)
{
    while (IsDma3ManagerBusyWithBgCopy())
        ProcessDma3Requests();
}

TEST("Battle info: Select opens only from actions and closing cannot select another battler's action")
{
    InitUi();
    gMain.newKeys = SELECT_BUTTON;
    EXPECT(!BattleInfo_HandleInput(B_BATTLER_0, FALSE));
    EXPECT(BattleInfo_HandleInput(B_BATTLER_0, TRUE));
    EXPECT(Test_BattleInfoWindow() != WINDOW_NONE);
    FlushCopies();
    // ReadKeys also emits A when the player's button mode is L=A.
    gMain.newKeys = L_BUTTON | A_BUTTON;
    EXPECT(BattleInfo_HandleInput(B_BATTLER_0, TRUE));
    EXPECT(Test_BattleInfoWindow() != WINDOW_NONE);
    FlushCopies();
    gMain.newKeys = B_BUTTON;
    EXPECT(BattleInfo_HandleInput(B_BATTLER_2, TRUE));
    EXPECT(Test_BattleInfoWindow() != WINDOW_NONE);
    EXPECT(BattleInfo_HandleInput(B_BATTLER_0, TRUE));
    EXPECT_EQ(Test_BattleInfoWindow(), WINDOW_NONE);
    EXPECT(BattleInfo_HandleInput(B_BATTLER_2, TRUE));
    gMain.vblankCounter1++;
    gMain.newKeys = 0;
    EXPECT(!BattleInfo_HandleInput(B_BATTLER_2, TRUE));
    FreeUi();
}

TEST("Battle info: doubles can change each column independently without selecting an absent battler")
{
    InitUi();
    EXPECT(Test_BattleInfoOpen(B_BATTLER_0));
    FlushCopies();
    gMain.newKeys = DPAD_LEFT;
    EXPECT(BattleInfo_HandleInput(B_BATTLER_0, TRUE));
    EXPECT_EQ(Test_BattleInfoSelected(0), B_BATTLER_2);
    EXPECT_EQ(Test_BattleInfoSelected(1), B_BATTLER_1);
    FlushCopies();
    gMain.newKeys = DPAD_RIGHT;
    EXPECT(BattleInfo_HandleInput(B_BATTLER_0, TRUE));
    EXPECT_EQ(Test_BattleInfoSelected(0), B_BATTLER_2);
    EXPECT_EQ(Test_BattleInfoSelected(1), B_BATTLER_3);
    FlushCopies();
    gAbsentBattlerFlags = 1u << B_BATTLER_1;
    EXPECT(BattleInfo_HandleInput(B_BATTLER_0, TRUE));
    EXPECT_EQ(Test_BattleInfoSelected(1), B_BATTLER_3);
    FlushCopies();
    Test_BattleInfoClose();
    gMain.newKeys = 0;
    FreeUi();
}

TEST("Battle info: low memory refuses the overlay without changing the battle window")
{
    InitUi();
    const struct MemBlock *first = HeapHead(), *block = first;
    u32 largest = 0;
    do
    {
        if (!block->allocated)
            largest = max(largest, block->size);
        block = block->next;
    } while (block != first);
    EXPECT(largest > 8192);
    void *reservation = AllocUnchecked(largest - 8192);
    EXPECT(reservation != NULL);
    EXPECT(!Test_BattleInfoOpen(B_BATTLER_0));
    EXPECT_EQ(Test_BattleInfoWindow(), WINDOW_NONE);
    EXPECT_EQ(gBattle_BG0_Y, 160);
    Free(reservation);
    FreeUi();
}

TEST("Battle info: link multi inspection leaves synchronized state and random numbers unchanged")
{
    InitUi();
    gBattleTypeFlags |= BATTLE_TYPE_LINK | BATTLE_TYPE_MULTI;
    u32 flags = gBattleTypeFlags;
    u8 stages[MAX_BATTLERS_COUNT][NUM_BATTLE_STATS];
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
        memcpy(stages[i], gBattleMons[i].statStages, NUM_BATTLE_STATS);
    rng_value_t rng = gRngValue;
    EXPECT(Test_BattleInfoOpen(B_BATTLER_2));
    for (u32 page = 0; page < 4; page++)
    {
        FlushCopies();
        Test_BattleInfoPage(page);
    }
    FlushCopies();
    Test_BattleInfoClose();
    EXPECT_EQ(gBattleTypeFlags, flags);
    EXPECT_EQ(memcmp(&rng, &gRngValue, sizeof(rng)), 0);
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
        EXPECT_EQ(memcmp(stages[i], gBattleMons[i].statStages, NUM_BATTLE_STATS), 0);
    FreeUi();
}

TEST("Battle info: closing restores the tilemap palette scroll and sprite visibility")
{
    InitUi();
    u8 *map = Alloc(0x1000);
    memcpy(map, GetBgTilemapBuffer(0), 0x1000);
    u16 *tiles = Alloc(28 * 18 * 32 + 32);
    volatile u16 *vram = (volatile u16 *)BG_CHAR_ADDR(0);
    for (u32 i = 0; i < (28 * 18 * 32 + 32) / 2; i++)
        tiles[i] = vram[i] = i < 1000 ? 0 : i < 2000 ? 0x7777 : i % 7;
    u16 palette[7 * 16];
    for (u32 i = 0; i < ARRAY_COUNT(palette); i++)
        palette[i] = gPlttBufferUnfaded[BG_PLTT_ID(9) + i] = RGB(1, 2, i % 32);
    gPlttBufferUnfaded[0] = RGB(5, 6, 7);
    gSprites[0].invisible = FALSE;
    gSprites[1].invisible = TRUE;
    rng_value_t rng = gRngValue;
    EXPECT(Test_BattleInfoOpen(B_BATTLER_0));
    EXPECT_EQ(memcmp(&gRngValue, &rng, sizeof(rng)), 0);
    BattleInfo_HideSprites();
    EXPECT(gSprites[0].invisible);
    while (IsDma3ManagerBusyWithBgCopy())
        ProcessDma3Requests();
    Test_BattleInfoClose();
    EXPECT_EQ(memcmp(map, GetBgTilemapBuffer(0), 0x1000), 0);
    EXPECT_EQ(memcmp(tiles, (const void *)BG_CHAR_ADDR(0), 28 * 18 * 32 + 32), 0);
    EXPECT_EQ(memcmp(palette, &gPlttBufferUnfaded[BG_PLTT_ID(9)], sizeof(palette)), 0);
    EXPECT_EQ(gPlttBufferUnfaded[0], RGB(5, 6, 7));
    EXPECT_EQ(gBattle_BG0_Y, 160);
    EXPECT(!gSprites[0].invisible);
    EXPECT(gSprites[1].invisible);
    Free(map);
    Free(tiles);
    FreeUi();
}

TEST("Battle info: every page renders inside its fixed window")
{
    InitUi();
    gBattleMons[0].statStages[STAT_ATK] = 8;
    gBattleMons[0].statStages[STAT_DEF] = 4;
    gBattleMons[1].statStages[STAT_SPEED] = 9;
    gBattleMons[0].status1 = STATUS1_PARALYSIS;
    gBattleMons[1].status1 = STATUS1_BURN;
    gSideStatuses[0] = SIDE_STATUS_REFLECT | SIDE_STATUS_TAILWIND;
    gSideTimers[0].reflectTimer = 5;
    gSideTimers[0].tailwindTimer = 2;
    BattleInfo_RecordDuration(BINFO_REFLECT, B_SIDE_PLAYER, 8);
    gSideStatuses[1] = SIDE_STATUS_AURORA_VEIL;
    gSideTimers[1].auroraVeilTimer = 3;
    BattleInfo_RecordDuration(BINFO_AURORA_VEIL, B_SIDE_OPPONENT, 8);
    gSideTimers[0].spikesAmount = 3;
    gSideTimers[1].toxicSpikesAmount = 2;
    gBattleWeather = B_WEATHER_RAIN_NORMAL;
    gBattleStruct->weatherDuration = 5;
    BattleInfo_RecordDuration(BINFO_WEATHER, B_SIDE_PLAYER, 8);
    gFieldTimers.terrain = B_TERRAIN_GRASSY;
    gFieldTimers.terrainTimer = 3;
    BattleInfo_RecordDuration(BINFO_TERRAIN, B_SIDE_PLAYER, 8);
    EXPECT(Test_BattleInfoOpen(B_BATTLER_0));
    for (u32 page = 0; page < 4; page++)
    {
        Test_BattleInfoPage(page);
        u32 window = Test_BattleInfoWindow();
        EXPECT_EQ(GetWindowAttribute(window, WINDOW_WIDTH), 28);
        EXPECT_EQ(GetWindowAttribute(window, WINDOW_HEIGHT), 18);
        if (page == 0)
        {
            const u16 *map = GetBgTilemapBuffer(0);
            EXPECT_EQ(map[7 * 32 + 2] >> 12, 13);
            EXPECT_EQ(map[7 * 32 + 16] >> 12, 14);
            EXPECT_EQ(map[7 * 32 + 6] >> 12, 10);
            EXPECT_EQ(map[7 * 32 + 20] >> 12, 11);
            EXPECT_EQ(map[10 * 32 + 10] >> 12, 9);
        }
        const u8 *pixels = (const u8 *)GetWindowAttribute(window, WINDOW_TILE_DATA);
        u32 ink = 0;
        for (u32 i = 0; i < 28 * 18 * 32; i++)
        {
            u32 low = pixels[i] & 15, high = pixels[i] >> 4;
            if (page != 0)
                EXPECT(low != 0 && high != 0);
            ink += (low >= 1 && low <= 3) + (high >= 1 && high <= 3);
        }
        EXPECT(ink > 128);
        if (gTestBattleInfoCapture)
        {
            for (u32 i = 0; i < 256; i++)
                Test_MgbaPrintf("BINFO_PAL %d %d %d", page, i, gPlttBufferUnfaded[i]);
            const u16 *map = GetBgTilemapBuffer(0);
            for (u32 y = 0; y < 18; y++)
                for (u32 x = 0; x < 28; x++)
                    Test_MgbaPrintf("BINFO_MAP %d %d %d", page, y * 28 + x, map[(y + 1) * 32 + x + 1] >> 12);
            const u32 *words = (const u32 *)pixels;
            for (u32 i = 0; i < (28 * 18 * 32) / 32; i++)
                Test_MgbaPrintf("BINFO_PIX %d %d %d %d %d %d %d %d %d %d", page, i,
                    words[i * 8], words[i * 8 + 1], words[i * 8 + 2], words[i * 8 + 3],
                    words[i * 8 + 4], words[i * 8 + 5], words[i * 8 + 6], words[i * 8 + 7]);
        }
        FlushCopies();
    }
    while (IsDma3ManagerBusyWithBgCopy())
        ProcessDma3Requests();
    Test_BattleInfoClose();
    FreeUi();
}

static bool32 HasBlankEffectTail(u32 window, u32 top)
{
    const u8 *pixels = (const u8 *)GetWindowAttribute(window, WINDOW_TILE_DATA);
    for (u32 y = top; y < 142; y++)
        for (u32 x = 4; x < 109; x++)
        {
            u32 offset = ((y / 8) * 28 + x / 8) * 32 + (y % 8) * 4 + (x % 8) / 2;
            if (((pixels[offset] >> ((x % 2) * 4)) & 15) != 7)
                return FALSE;
        }
    return TRUE;
}

TEST("Battle info: inactive field effects and zero hazards are omitted")
{
    InitUi();
    EXPECT(Test_BattleInfoOpen(B_BATTLER_0));
    FlushCopies();
    for (u32 page = 1; page < 4; page++)
    {
        Test_BattleInfoPage(page);
        EXPECT(HasBlankEffectTail(Test_BattleInfoWindow(), 49));
        FlushCopies();
    }
    gSideStatuses[B_SIDE_PLAYER] = SIDE_STATUS_REFLECT;
    gSideTimers[B_SIDE_PLAYER].reflectTimer = 5;
    Test_BattleInfoPage(1);
    EXPECT(HasBlankEffectTail(Test_BattleInfoWindow(), 49));
    FlushCopies();
    gSideTimers[B_SIDE_PLAYER].spikesAmount = 3;
    Test_BattleInfoPage(2);
    EXPECT(HasBlankEffectTail(Test_BattleInfoWindow(), 49));
    FlushCopies();
    Test_BattleInfoClose();
    FreeUi();
}
