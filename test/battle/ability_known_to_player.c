#include "global.h"
#include "battle_ai_record.h"
#include "test/battle.h"

// The move menu's type effectiveness uses only the abilities the player knows.

SINGLE_BATTLE_TEST("An opponent's ability is unknown to the player until it is shown")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BRONZONG) { Ability(ABILITY_LEVITATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(GetAbilityKnownToPlayer(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)), ABILITY_NONE);
        EXPECT_EQ(GetAbilityKnownToPlayer(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)), gBattleMons[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)].ability);
    }
}

SINGLE_BATTLE_TEST("An opponent's ability becomes known to the player once its pop-up shows")
{
    GIVEN {
        ASSUME(GetMoveType(MOVE_MUD_SLAP) == TYPE_GROUND);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_BRONZONG) { Ability(ABILITY_LEVITATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_MUD_SLAP); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_LEVITATE);
    } THEN {
        EXPECT_EQ(GetAbilityKnownToPlayer(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)), ABILITY_LEVITATE);
    }
}

SINGLE_BATTLE_TEST("An opponent's ability is known to the player when its species has no other")
{
    GIVEN {
        ASSUME(GetSpeciesAbility(SPECIES_GASTLY, 0) == ABILITY_LEVITATE);
        ASSUME(GetSpeciesAbility(SPECIES_GASTLY, 1) == ABILITY_NONE);
        ASSUME(GetSpeciesAbility(SPECIES_GASTLY, 2) == ABILITY_NONE);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GASTLY) { Ability(ABILITY_LEVITATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(GetAbilityKnownToPlayer(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)), ABILITY_LEVITATE);
    }
}
