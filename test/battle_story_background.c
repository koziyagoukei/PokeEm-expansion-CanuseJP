#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_bg.h"
#include "battle_setup.h"
#include "decompress.h"
#include "frontier_story.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/opponents.h"

struct BattleBackgroundGraphics
{
    const void *tileset;
    const void *tilemap;
    const void *palette;
};

#include "../src/data/battle_frontier_backgrounds.h"

static const struct
{
    u16 trainer;
    const struct BattleBackgroundGraphics *graphics;
} sReleaseBackgrounds[] =
{
    {TRAINER_KAZUKI_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_TOWER].graphics},
    {TRAINER_REIKA_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_DOME].graphics},
    {TRAINER_IKUE_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_PALACE].graphics},
    {TRAINER_EISUKE_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_ARENA].graphics},
    {TRAINER_TOSHIO_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_FACTORY].graphics},
    {TRAINER_YAYOI_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_PIKE].graphics},
    {TRAINER_NOBUO_RELEASE, &sFrontierBattleBackgrounds[FRONTIER_FACILITY_PYRAMID].graphics},
    {TRAINER_YORIKO_RELEASE, &sStoryBackground_Contest},
    {TRAINER_POKEVIRUS_FINAL, &sStoryBackground_Pokevirus},
};

static void PrepareScene(u32 index)
{
    gMain.inBattle = TRUE;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI
                    | BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_TWO_OPPONENTS;
    TRAINER_BATTLE_PARAM.opponentA = sReleaseBackgrounds[index].trainer;
    gBattleEnvironment = BATTLE_ENVIRONMENT_BUILDING;
    gFieldTimers.terrain = B_TERRAIN_NONE;
}

static void ExpectBackground(const struct BattleBackgroundGraphics *graphics)
{
    u32 size = GetDecompressedDataSize(graphics->tileset);
    EXPECT_LE(size, BG_CHAR_SIZE);
    u8 *tiles = malloc_and_decompress(graphics->tileset, NULL);
    const volatile u8 *loaded = (const void *)BG_CHAR_ADDR(2);
    for (u32 i = 0; i < size; i++)
        EXPECT_EQ(loaded[i], tiles[i]);
    Free(tiles);
    u16 *map = malloc_and_decompress(graphics->tilemap, NULL);
    EXPECT_EQ(GetDecompressedDataSize(graphics->tilemap), 2 * BG_SCREEN_SIZE);
    const volatile u16 *loadedMap = (const void *)BG_SCREEN_ADDR(26);
    for (u32 i = 0; i < BG_SCREEN_SIZE; i++)
        EXPECT_EQ(loadedMap[i], map[i]);
    Free(map);
    const u16 *palette = graphics->palette;
    for (u32 i = 0; i < 48; i++)
    {
        EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(2) + i], palette[i]);
        EXPECT_EQ(gPlttBufferFaded[BG_PLTT_ID(2) + i], palette[i]);
    }
}

static u32 BackgroundHash(void)
{
    u32 hash = 2166136261u;
    const volatile u8 *tiles = (const void *)BG_CHAR_ADDR(2);
    const volatile u8 *map = (const void *)BG_SCREEN_ADDR(26);
    for (u32 i = 0; i < BG_CHAR_SIZE; i++)
        hash = (hash ^ tiles[i]) * 16777619;
    for (u32 i = 0; i < 2 * BG_SCREEN_SIZE; i++)
        hash = (hash ^ map[i]) * 16777619;
    for (u32 i = 0; i < 48; i++)
        hash = (hash ^ gPlttBufferUnfaded[BG_PLTT_ID(2) + i]) * 16777619;
    return hash;
}

TEST("Story backgrounds: every release and final battle loads and restores its own artwork")
{
    u32 index = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sReleaseBackgrounds); i++)
        PARAMETRIZE { index = i; }
    PrepareScene(index);
    u32 flags = gBattleTypeFlags;
    FrontierStory_PrepareReleaseBattle();
    if (index == ARRAY_COUNT(sReleaseBackgrounds) - 1)
        FrontierStory_PrepareFinalBattle();
    DrawMainBattleBackground();
    ExpectBackground(sReleaseBackgrounds[index].graphics);
    for (u32 terrain = B_TERRAIN_GRASSY; terrain <= B_TERRAIN_PSYCHIC; terrain++)
    {
        gFieldTimers.terrain = terrain;
        DrawTerrainTypeBattleBackground();
        gFieldTimers.terrain = B_TERRAIN_NONE;
        DrawTerrainTypeBattleBackground();
        ExpectBackground(sReleaseBackgrounds[index].graphics);
    }
    LoadMoveBg(BG_TRICK_ROOM);
    for (u32 phase = 3; phase <= 5; phase++)
        LoadChosenBattleElement(phase);
    ExpectBackground(sReleaseBackgrounds[index].graphics);
    EXPECT_EQ(gBattleTypeFlags, flags);
    EXPECT_EQ(gBattleEnvironment, BATTLE_ENVIRONMENT_BUILDING);
    FrontierStory_EndStoryBattle();
}

TEST("Story backgrounds: link and recorded battles do not inherit story artwork")
{
    u32 flags = BATTLE_TYPE_LINK;
    PARAMETRIZE { flags = BATTLE_TYPE_LINK; }
    PARAMETRIZE { flags = BATTLE_TYPE_RECORDED; }
    PARAMETRIZE { flags = BATTLE_TYPE_RECORDED_LINK; }
    PrepareScene(ARRAY_COUNT(sReleaseBackgrounds) - 1);
    gBattleTypeFlags |= flags;
    DrawMainBattleBackground();
    u32 before = BackgroundHash();
    FrontierStory_PrepareFinalBattle();
    DrawMainBattleBackground();
    EXPECT_EQ(BackgroundHash(), before);
    FrontierStory_EndStoryBattle();
}

TEST("Story backgrounds: the next ordinary battle returns to its original background")
{
    PrepareScene(ARRAY_COUNT(sReleaseBackgrounds) - 1);
    DrawMainBattleBackground();
    u32 before = BackgroundHash();
    FrontierStory_PrepareFinalBattle();
    DrawMainBattleBackground();
    EXPECT_NE(BackgroundHash(), before);
    FrontierStory_EndStoryBattle();
    // The shorter original tileset need not clear unused VRAM. Compare only loaded data.
    CpuFill32(0, (void *)BG_CHAR_ADDR(2), BG_CHAR_SIZE);
    DrawMainBattleBackground();
    EXPECT(!FrontierStory_IsReleaseBattle());
    u32 after = BackgroundHash();
    PrepareScene(ARRAY_COUNT(sReleaseBackgrounds) - 1);
    CpuFill32(0, (void *)BG_CHAR_ADDR(2), BG_CHAR_SIZE);
    DrawMainBattleBackground();
    EXPECT_EQ(BackgroundHash(), after);
}
