#include "global.h"
#include "battle_main.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Dynamax preserves Speed Swap before activation and after expiry")
{
    bool32 gigantamax;
    enum Item item;
    PARAMETRIZE { gigantamax = FALSE; item = ITEM_NONE; }
    PARAMETRIZE { gigantamax = TRUE; item = ITEM_NONE; }
    PARAMETRIZE { gigantamax = FALSE; item = ITEM_CHOICE_SCARF; }
    PARAMETRIZE { gigantamax = TRUE; item = ITEM_CHOICE_SCARF; }
    GIVEN {
        WITH_CONFIG(B_MEGA_EVO_SPEED_SWAP, GEN_8);
        PLAYER(SPECIES_SNORLAX) { Speed(100); }
        PLAYER(SPECIES_MIRAIDON) { Speed(200); }
        OPPONENT(SPECIES_CORVIKNIGHT) { Speed(300); GigantamaxFactor(gigantamax); Item(item); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_WATER_GUN); MOVE(player, MOVE_SPEED_SWAP); }
        TURN { MOVE(opponent, MOVE_WATER_GUN, gimmick: GIMMICK_DYNAMAX); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_WATER_GUN); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_WATER_GUN); SWITCH(player, 1); }
        TURN { MOVE(opponent, MOVE_WATER_GUN); MOVE(player, MOVE_POUND); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SPEED_SWAP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GEYSER, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GEYSER, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GEYSER, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_WATER_GUN, opponent);
    } THEN {
        EXPECT_EQ(opponent->speed, 100);
        EXPECT_EQ(GetBattlerTotalSpeedStat(B_BATTLER_1, ABILITY_NONE,
                  item == ITEM_CHOICE_SCARF ? HOLD_EFFECT_CHOICE_SCARF : HOLD_EFFECT_NONE),
                  item == ITEM_CHOICE_SCARF ? 150 : 100);
    }
}

SINGLE_BATTLE_TEST("Triage boosts Draining Kiss but not the Max Starfall it becomes")
{
    enum Gimmick gimmick;
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    GIVEN {
        PLAYER(SPECIES_MIRAIDON) { Speed(200); }
        OPPONENT(SPECIES_COMFEY) { Speed(100); Ability(ABILITY_TRIAGE); }
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); MOVE(opponent, MOVE_DRAINING_KISS, gimmick: gimmick); }
    } SCENE {
        if (gimmick == GIMMICK_DYNAMAX) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STARFALL, opponent);
        } else {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAINING_KISS, opponent);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
        }
    }
}
