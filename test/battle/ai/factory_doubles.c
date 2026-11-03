#include "global.h"
#include "battle_frontier.h"
#include "test/battle.h"

AI_DOUBLE_BATTLE_TEST("Factory doubles: early rounds do not randomly target an ally with a single-target attack")
{
    bool32 attacksFromRight = FALSE;
    PARAMETRIZE { attacksFromRight = FALSE; }
    PARAMETRIZE { attacksFromRight = TRUE; }

    GIVEN {
        ASSUME(GetMoveTarget(MOVE_SCRATCH) == TARGET_SELECTED);
        // Recreate the Factory's early-round AI flags in the recorded test battle.
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_FACTORY;
        gBattleTestRunnerState->data.recordedBattle.opponentA = 1;
        gFacilityTrainers = gBattleFrontierTrainers;
        AI_FLAGS(0);
        // Before the fix, these tie breakers pick the attacker's ally.
        TIE_BREAK_TARGET(attacksFromRight ? TARGET_TIE_CHOSEN : TARGET_TIE_HI, 1);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SCRATCH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SCRATCH); }
    } WHEN {
        TURN {
            if (attacksFromRight)
                EXPECT_MOVE(opponentRight, MOVE_SCRATCH, target: playerRight);
            else
                EXPECT_MOVE(opponentLeft, MOVE_SCRATCH, target: playerRight);
        }
    }
}
