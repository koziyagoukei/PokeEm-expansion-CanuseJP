#include "global.h"
#include "test/battle.h"

// The substitute doll is a sprite flag kept apart from the substitute itself.
// A replacement must only show the doll when it really came in behind a substitute.

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
    ASSUME(GetMoveEffect(MOVE_U_TURN) == EFFECT_HIT_ESCAPE);
    ASSUME(GetMoveEffect(MOVE_BATON_PASS) == EFFECT_BATON_PASS);
}

SINGLE_BATTLE_TEST("Substitute doll: using Substitute shows the doll")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); }
    } THEN {
        EXPECT(gBattleMons[B_BATTLER_1].volatiles.substitute);
        EXPECT(gBattleSpritesDataPtr->battlerData[B_BATTLER_1].behindSubstitute);
    }
}

SINGLE_BATTLE_TEST("Substitute doll: the Pokemon sent in after U-turn does not take over the doll")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT(SPECIES_WYNAUT) { Speed(5); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); }
        TURN { MOVE(opponent, MOVE_U_TURN); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, opponent);
    } THEN {
        EXPECT_EQ(gBattleMons[B_BATTLER_1].species, SPECIES_WYNAUT);
        EXPECT(!gBattleMons[B_BATTLER_1].volatiles.substitute);
        EXPECT(!gBattleSpritesDataPtr->battlerData[B_BATTLER_1].behindSubstitute);
    }
}

SINGLE_BATTLE_TEST("Substitute doll: Baton Pass hands the doll over with the substitute")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); }
        TURN { MOVE(opponent, MOVE_BATON_PASS); SEND_OUT(opponent, 1); }
    } THEN {
        EXPECT_EQ(gBattleMons[B_BATTLER_1].species, SPECIES_WYNAUT);
        EXPECT(gBattleMons[B_BATTLER_1].volatiles.substitute);
        EXPECT(gBattleSpritesDataPtr->battlerData[B_BATTLER_1].behindSubstitute);
    }
}
