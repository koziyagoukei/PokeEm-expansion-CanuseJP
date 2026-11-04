#include "global.h"
#include "test/battle.h"

// In a multi battle each Trainer keeps a party of their own. These moves and items
// used to treat the side as one party split in halves, or the partner's slot number
// as a slot of the user's own party.

MULTI_BATTLE_TEST("Multi party: Roar switches out the right-hand opponent")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ROAR) == EFFECT_ROAR);
        PLAYER(SPECIES_WOBBUFFET);
        PARTNER(SPECIES_WOBBUFFET);
        OPPONENT_A(SPECIES_WOBBUFFET);
        OPPONENT_B(SPECIES_WOBBUFFET);
        OPPONENT_B(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROAR, target: opponentRight); }
    } THEN {
        EXPECT_EQ(gBattleMons[B_BATTLER_3].species, SPECIES_WYNAUT);
    }
}

MULTI_BATTLE_TEST("Multi party: Roar switches out the player's partner")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ROAR) == EFFECT_ROAR);
        PLAYER(SPECIES_WOBBUFFET);
        PARTNER(SPECIES_WOBBUFFET);
        PARTNER(SPECIES_WYNAUT);
        OPPONENT_A(SPECIES_WOBBUFFET);
        OPPONENT_B(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_ROAR, target: playerRight); }
    } THEN {
        EXPECT_EQ(gBattleMons[B_BATTLER_2].species, SPECIES_WYNAUT);
    }
}

MULTI_BATTLE_TEST("Multi party: Revival Blessing does not send the revived Pokemon to the partner's empty slot")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_REVIVAL_BLESSING) == EFFECT_REVIVAL_BLESSING);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); Speed(1); }
        PLAYER(SPECIES_WYNAUT) { Speed(1); }
        PARTNER(SPECIES_WOBBUFFET) { HP(1); Speed(1); }
        OPPONENT_A(SPECIES_WOBBUFFET) { Speed(10); }
        OPPONENT_B(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerLeft); MOVE(opponentRight, MOVE_SCRATCH, target: playerRight); SEND_OUT(playerLeft, 1); }
        TURN { MOVE(playerLeft, MOVE_REVIVAL_BLESSING, partyIndex: 0); }
    } THEN {
        // The player's Wobbuffet is revived in the party; the partner's fainted one stays down.
        EXPECT_NE(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 0);
        EXPECT_EQ(gBattleMons[B_BATTLER_2].hp, 0);
        EXPECT(gAbsentBattlerFlags & (1u << B_BATTLER_2));
    }
}

MULTI_BATTLE_TEST("Multi party: Heal Bell cures the partner in its own party too")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HEAL_BELL) == EFFECT_HEAL_BELL);
        PLAYER(SPECIES_WOBBUFFET);
        PARTNER(SPECIES_WOBBUFFET) { Status1(STATUS1_BURN); }
        OPPONENT_A(SPECIES_WOBBUFFET);
        OPPONENT_B(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HEAL_BELL); }
    } THEN {
        EXPECT_EQ(gBattleMons[B_BATTLER_2].status1, 0);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PARTNER][0], MON_DATA_STATUS), 0);
    }
}

MULTI_BATTLE_TEST("Multi party: a Potion for a benched Pokemon does not heal the partner with the same slot number")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { HP(1); }
        PARTNER(SPECIES_WOBBUFFET);
        PARTNER(SPECIES_WYNAUT) { HP(1); }
        OPPONENT_A(SPECIES_WOBBUFFET);
        OPPONENT_B(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(playerRight, 1); }
        TURN { USE_ITEM(playerLeft, ITEM_POTION, partyIndex: 1); }
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_HP), 21);
        EXPECT_EQ(gBattleMons[B_BATTLER_2].hp, 1);
    }
}
