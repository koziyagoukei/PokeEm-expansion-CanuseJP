#include "global.h"
#include "battle_frontier.h"
#include "constants/battle_frontier_mons.h"
#include "test/battle.h"

TO_DO_BATTLE_TEST("Conversion changes the user's types to the target's current types (Gen 1)");
TO_DO_BATTLE_TEST("Conversion changes the user's types randomly to one of the user's move (Gen 2-5)");
TO_DO_BATTLE_TEST("Conversion ignores moves that share types with the user (Gen 2-5)");
TO_DO_BATTLE_TEST("Conversion fails if all the user's moves share types with the user (Gen 2-5)");
TO_DO_BATTLE_TEST("Conversion changes the user's types to the one in the user's first slot (Gen 6+)");
TO_DO_BATTLE_TEST("Conversion can read the user's first move slot even if that move cannot be selected (Gen 6+)"); //Eg. Disable
TO_DO_BATTLE_TEST("Conversion can change the user's types to Conversion's type");

SINGLE_BATTLE_TEST("Frontier Z-Conversion changes Porygon-Z to Electric and raises all five stats")
{
    const struct TrainerMon *set = &gBattleFrontierMons[FRONTIER_MON_PORYGON_Z_1];
    GIVEN {
        ASSUME(B_UPDATED_CONVERSION >= GEN_6);
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_PORYGON_Z) {
            Item(set->heldItem);
            Ability(set->ability);
            Moves(set->moves[0], set->moves[1], set->moves[2], set->moves[3]);
        }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CONVERSION, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, player);
    } THEN {
        EXPECT_EQ(player->types[0], TYPE_ELECTRIC);
        EXPECT_EQ(player->types[1], TYPE_ELECTRIC);
        for (enum Stat stat = STAT_ATK; stat <= STAT_SPDEF; stat++)
            EXPECT_EQ(player->statStages[stat], DEFAULT_STAT_STAGE + 1);
    }
}

SINGLE_BATTLE_TEST("(TERA) Conversion fails if used by a Terastallized Pokemon")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { TeraType(TYPE_PSYCHIC); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CONVERSION, gimmick: GIMMICK_TERA); }
    } SCENE {
        MESSAGE("Wobbuffet used Conversion!");
        MESSAGE("But it failed!");
    }
}
