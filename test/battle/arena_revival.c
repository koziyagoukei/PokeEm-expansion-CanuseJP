#include "global.h"
#include "test/battle.h"
#include "battle_controllers.h"
#include "battle_util.h"

#define USE_ARENA_RULES() (gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA)

SINGLE_BATTLE_TEST("Battle Arena: Revival Blessing is unusable only in Arena battles")
{
    bool32 arena;
    PARAMETRIZE { arena = FALSE; }
    PARAMETRIZE { arena = TRUE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_REVIVAL_BLESSING) == EFFECT_REVIVAL_BLESSING);
        if (arena)
            USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_REVIVAL_BLESSING, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { HP(0); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_REVIVAL_BLESSING, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WYNAUT) { HP(0); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        for (u32 battler = 0; battler < gBattlersCount; battler++)
        {
            EXPECT_EQ(CheckMoveLimitations(battler, 0, MOVE_LIMITATIONS_ALL) & 1, arena);
            EXPECT_EQ(CheckMoveLimitations(battler, 0, MOVE_LIMITATIONS_ALL) & 2, 0);
        }
    }
}

SINGLE_BATTLE_TEST("Battle Arena: choosing Revival Blessing is refused, even as a Z-Move")
{
    bool32 arena, zMove;
    PARAMETRIZE { arena = FALSE; zMove = FALSE; }
    PARAMETRIZE { arena = TRUE;  zMove = FALSE; }
    PARAMETRIZE { arena = TRUE;  zMove = TRUE; }
    GIVEN {
        if (arena)
            USE_ARENA_RULES();
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_REVIVAL_BLESSING, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { HP(0); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        gBattleResources->bufferB[B_BATTLER_0][2] = 0;
        if (zMove)
        {
            gBattleResources->bufferB[B_BATTLER_0][2] |= RET_GIMMICK;
            gBattleStruct->gimmick.usableGimmick[B_BATTLER_0] = GIMMICK_Z_MOVE;
        }
        EXPECT_EQ(TrySetCantSelectMoveBattleScript(B_BATTLER_0) != 0, arena);
        EXPECT_EQ(GetMonData(&gPlayerParty[1], MON_DATA_HP), 0);
    }
}

AI_SINGLE_BATTLE_TEST("Battle Arena: the AI does not choose Revival Blessing")
{
    GIVEN {
        USE_ARENA_RULES();
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_SMART_MON_CHOICES | AI_FLAG_OMNISCIENT);
        PLAYER(SPECIES_BULBASAUR);
        OPPONENT(SPECIES_AUDINO) { Moves(MOVE_REVIVAL_BLESSING, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_PIDGEY) { HP(0); Moves(MOVE_GUST); }
    } WHEN {
        TURN { EXPECT_MOVE(opponent, MOVE_CELEBRATE); }
    }
}

#undef USE_ARENA_RULES
