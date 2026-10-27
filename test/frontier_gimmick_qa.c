#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Frontier QA: status Z-effect activates with a Z-Ring against priority blocking abilities")
{
    enum Species species;
    enum Ability ability;
    PARAMETRIZE { species = SPECIES_BRUXISH; ability = ABILITY_DAZZLING; }
    PARAMETRIZE { species = SPECIES_FARIGIRAF; ability = ABILITY_ARMOR_TAIL; }
    PARAMETRIZE { species = SPECIES_TSAREENA; ability = ABILITY_QUEENLY_MAJESTY; }

    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_WOBBUFFET) { Item(ITEM_FAIRIUM_Z); }
        OPPONENT(species) { Ability(ability); }
    } WHEN {
        TURN { MOVE(player, MOVE_BABY_DOLL_EYES, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, player);
        ABILITY_POPUP(opponent, ability);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_BABY_DOLL_EYES, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_DEF], DEFAULT_STAT_STAGE + 1);
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
    }
}

SINGLE_BATTLE_TEST("Frontier QA: Spicy Extract changes the target's stats")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SPICY_EXTRACT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPICY_EXTRACT, player);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponent->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 2);
        EXPECT_EQ(opponent->statStages[STAT_DEF], DEFAULT_STAT_STAGE - 2);
    }
}
