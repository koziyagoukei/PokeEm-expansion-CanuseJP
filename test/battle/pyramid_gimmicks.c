#include "global.h"
#include "test/battle.h"
#include "battle_gimmick.h"
#include "event_data.h"

SINGLE_BATTLE_TEST("Pyramid: both sides can use Z-Moves without a ring")
{
    GIVEN {
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_PYRAMID;
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_NORMALIUM_Z); Moves(MOVE_QUICK_ATTACK); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_NORMALIUM_Z); Moves(MOVE_QUICK_ATTACK); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_QUICK_ATTACK, gimmick: GIMMICK_Z_MOVE);
            MOVE(opponent, MOVE_QUICK_ATTACK, gimmick: GIMMICK_Z_MOVE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, opponent);
    } THEN {
        EXPECT(HasTrainerUsedGimmick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), GIMMICK_Z_MOVE));
        EXPECT(HasTrainerUsedGimmick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), GIMMICK_Z_MOVE));
    }
}

SINGLE_BATTLE_TEST("Pyramid: Tera works without the orb and preserves field charge")
{
    bool32 charged;
    PARAMETRIZE { charged = FALSE; }
    PARAMETRIZE { charged = TRUE; }
    GIVEN {
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_PYRAMID;
        if (charged)
            FlagSet(B_FLAG_TERA_ORB_CHARGED);
        else
            FlagClear(B_FLAG_TERA_ORB_CHARGED);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); TeraType(TYPE_NORMAL); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); TeraType(TYPE_NORMAL); }
    } WHEN {
        TURN {
            MOVE(player, MOVE_CELEBRATE, gimmick: GIMMICK_TERA);
            MOVE(opponent, MOVE_CELEBRATE, gimmick: GIMMICK_TERA);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, player);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_TERA_ACTIVATE, opponent);
    } THEN {
        EXPECT_EQ(FlagGet(B_FLAG_TERA_ORB_CHARGED), charged);
        EXPECT(HasTrainerUsedGimmick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), GIMMICK_TERA));
        EXPECT(HasTrainerUsedGimmick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), GIMMICK_TERA));
    }
}
