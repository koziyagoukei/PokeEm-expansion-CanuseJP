#include "global.h"
#include "test/battle.h"
#include "battle_info.h"
#include "bg.h"
#include "dma3.h"
#include "constants/items.h"

static u32 HashVisibleBg0(void)
{
    const u16 *map = GetBgTilemapBuffer(0);
    const volatile u8 *tiles = (const volatile u8 *)BG_CHAR_ADDR(0);
    u32 hash = 2166136261u;
    for (u32 y = 0; y < DISPLAY_HEIGHT / 8; y++)
        for (u32 x = 0; x < DISPLAY_WIDTH / 8; x++)
        {
            u32 row = (gBattle_BG0_Y / 8 + y) % 64;
            u32 col = (gBattle_BG0_X / 8 + x) % 32;
            u32 tile = map[row * 32 + col] & 0x3ff;
            for (u32 i = 0; i < 32; i++)
                hash = (hash ^ tiles[tile * 32 + i]) * 16777619u;
        }
    return hash;
}

SINGLE_BATTLE_TEST("Battle info: Reflect records its real duration without initially revealing Light Clay")
{
    GIVEN {
        PLAYER(SPECIES_MR_MIME) { Item(ITEM_LIGHT_CLAY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_REFLECT); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        struct BattleInfoTurns turns;
        EXPECT_EQ(gSideTimers[B_SIDE_PLAYER].reflectTimer, 7);
        BattleInfo_GetTurns(BINFO_REFLECT, B_SIDE_PLAYER, &turns);
        EXPECT_EQ(turns.elapsed, 2);
        EXPECT_EQ(turns.total, 5);
        gSideTimers[B_SIDE_PLAYER].reflectTimer = 3;
        BattleInfo_GetTurns(BINFO_REFLECT, B_SIDE_PLAYER, &turns);
        EXPECT_EQ(turns.elapsed, 6);
        EXPECT_EQ(turns.total, 8);
    }
}

SINGLE_BATTLE_TEST("Battle info: weather and terrain extension are tracked through actual move execution")
{
    bool32 terrain = FALSE;
    PARAMETRIZE { terrain = FALSE; }
    PARAMETRIZE { terrain = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(terrain ? ITEM_TERRAIN_EXTENDER : ITEM_DAMP_ROCK); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, terrain ? MOVE_GRASSY_TERRAIN : MOVE_RAIN_DANCE); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        struct BattleInfoTurns turns;
        if (terrain)
            gFieldTimers.terrainTimer = 3;
        else
            gBattleStruct->weatherDuration = 3;
        BattleInfo_GetTurns(terrain ? BINFO_TERRAIN : BINFO_WEATHER, B_SIDE_PLAYER, &turns);
        EXPECT_EQ(turns.elapsed, 6);
        EXPECT_EQ(turns.total, 8);
    }
}

DOUBLE_BATTLE_TEST("Battle info: the overlay fits the live double battle heap and can be closed")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU);
        PLAYER(SPECIES_EEVEE);
        OPPONENT(SPECIES_GENGAR);
        OPPONENT(SPECIES_GARDEVOIR);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE); MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } THEN {
        while (IsDma3ManagerBusyWithBgCopy())
            ProcessDma3Requests();
        gBattle_BG0_Y = DISPLAY_HEIGHT;
        u32 before = HashVisibleBg0();
        EXPECT(Test_BattleInfoOpen(B_BATTLER_0));
        while (IsDma3ManagerBusyWithBgCopy())
            ProcessDma3Requests();
        Test_BattleInfoClose();
        while (IsDma3ManagerBusyWithBgCopy())
            ProcessDma3Requests();
        EXPECT_EQ(HashVisibleBg0(), before);
    }
}

SINGLE_BATTLE_TEST("Battle info: Light Screen and Aurora Veil use their real move durations")
{
    bool32 veil = FALSE;
    PARAMETRIZE { veil = FALSE; }
    PARAMETRIZE { veil = TRUE; }
    GIVEN {
        PLAYER(SPECIES_MR_MIME) { Item(ITEM_LIGHT_CLAY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SNOWSCAPE); MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, veil ? MOVE_AURORA_VEIL : MOVE_LIGHT_SCREEN); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        struct BattleInfoTurns turns;
        if (veil)
            gSideTimers[B_SIDE_PLAYER].auroraVeilTimer = 3;
        else
            gSideTimers[B_SIDE_PLAYER].lightscreenTimer = 3;
        BattleInfo_GetTurns(veil ? BINFO_AURORA_VEIL : BINFO_LIGHT_SCREEN, B_SIDE_PLAYER, &turns);
        EXPECT(turns.active);
        EXPECT_EQ(turns.elapsed, 6);
        EXPECT_EQ(turns.total, 8);
    }
}
