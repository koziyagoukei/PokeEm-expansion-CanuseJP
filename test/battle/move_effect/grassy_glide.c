#include "global.h"
#include "battle_main.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_GRASSY_GLIDE) == EFFECT_GRASSY_GLIDE);
    ASSUME(GetMovePriority(MOVE_GRASSY_GLIDE) == 0);
    ASSUME(GetMovePriority(MOVE_SCRATCH) == 0);
}

SINGLE_BATTLE_TEST("Grassy Glide's priority increases by 1 when the user is affected by Grassy Terrain")
{
    GIVEN {
        PLAYER(SPECIES_TAPU_BULU) { Ability(ABILITY_GRASSY_SURGE); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_GLIDE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_GRASSY_SURGE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
    } THEN {
        EXPECT_EQ(GetBattleMovePriority(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), ABILITY_GRASSY_SURGE, MOVE_GRASSY_GLIDE), 1);
    }
}

SINGLE_BATTLE_TEST("Grassy Glide only gains priority in Grassy Terrain, including terrain set by a move")
{
    enum Move terrainMove;
    PARAMETRIZE { terrainMove = MOVE_CELEBRATE; }
    PARAMETRIZE { terrainMove = MOVE_GRASSY_TERRAIN; }
    PARAMETRIZE { terrainMove = MOVE_ELECTRIC_TERRAIN; }
    PARAMETRIZE { terrainMove = MOVE_MISTY_TERRAIN; }
    PARAMETRIZE { terrainMove = MOVE_PSYCHIC_TERRAIN; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, terrainMove); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_GRASSY_GLIDE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, terrainMove, player);
        if (terrainMove == MOVE_GRASSY_TERRAIN)
        {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        }
        else
        {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, player);
        }
    } THEN {
        EXPECT_EQ(GetBattleMovePriority(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), ABILITY_SHADOW_TAG, MOVE_GRASSY_GLIDE), terrainMove == MOVE_GRASSY_TERRAIN ? 1 : 0);
    }
}

SINGLE_BATTLE_TEST("Grassy Glide does not gain priority for airborne users")
{
    enum Species species;
    enum Ability ability;
    enum Item item;
    PARAMETRIZE { species = SPECIES_TROPIUS; ability = ABILITY_CHLOROPHYLL; item = ITEM_NONE; }
    PARAMETRIZE { species = SPECIES_GASTLY; ability = ABILITY_LEVITATE; item = ITEM_NONE; }
    PARAMETRIZE { species = SPECIES_WOBBUFFET; ability = ABILITY_SHADOW_TAG; item = ITEM_AIR_BALLOON; }
    GIVEN {
        PLAYER(species) { Ability(ability); Item(item); Speed(1); }
        OPPONENT(SPECIES_TAPU_BULU) { Ability(ABILITY_GRASSY_SURGE); Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_GLIDE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_GRASSY_SURGE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, player);
    } THEN {
        EXPECT_EQ(GetBattleMovePriority(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), ability, MOVE_GRASSY_GLIDE), 0);
    }
}

SINGLE_BATTLE_TEST("Grassy Glide also gains priority on the opponent's side")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_TAPU_BULU) { Ability(ABILITY_GRASSY_SURGE); Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_GRASSY_GLIDE); }
    } SCENE {
        ABILITY_POPUP(opponent, ABILITY_GRASSY_SURGE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
    } THEN {
        EXPECT_EQ(GetBattleMovePriority(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), ABILITY_GRASSY_SURGE, MOVE_GRASSY_GLIDE), 1);
    }
}

SINGLE_BATTLE_TEST("Grassy Glide does not mistake Magic Room for Grassy Terrain")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_MAGIC_ROOM); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_GRASSY_GLIDE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAGIC_ROOM, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, player);
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_NONE);
        EXPECT(gFieldStatuses & STATUS_FIELD_MAGIC_ROOM);
        EXPECT_EQ(GetBattleMovePriority(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), ABILITY_SHADOW_TAG, MOVE_GRASSY_GLIDE), 0);
    }
}

SINGLE_BATTLE_TEST("Grassy Glide keeps its Grassy Terrain priority during Trick Room")
{
    GIVEN {
        PLAYER(SPECIES_TAPU_BULU) { Ability(ABILITY_GRASSY_SURGE); Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_TRICK_ROOM); MOVE(opponent, MOVE_CELEBRATE); }
        TURN { MOVE(player, MOVE_GRASSY_GLIDE); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_GRASSY_SURGE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK_ROOM, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GRASSY_GLIDE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
    } THEN {
        EXPECT(gFieldStatuses & STATUS_FIELD_TRICK_ROOM);
        EXPECT_EQ(GetBattleMovePriority(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), ABILITY_GRASSY_SURGE, MOVE_GRASSY_GLIDE), 1);
    }
}

SINGLE_BATTLE_TEST("Dynamax: Grassy Glide's priority does not increase for the Max Move it becomes")
{
    GIVEN {
        PLAYER(SPECIES_TAPU_BULU) { Ability(ABILITY_GRASSY_SURGE); Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_GLIDE, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_SCRATCH); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_GRASSY_SURGE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_OVERGROWTH, player);
    }
}
