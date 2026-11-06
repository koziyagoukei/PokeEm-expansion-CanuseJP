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

SINGLE_BATTLE_TEST("Dynamax and Gigantamax preserve types changed by Soak or Magic Powder")
{
    u32 variant = 0;
    for (u32 j = 0; j < 8; j++)
        PARAMETRIZE { variant = j; }
    bool32 gigantamax = (variant & 1) != 0;
    bool32 during = (variant & 2) != 0;
    enum Move move = (variant & 4) ? MOVE_MAGIC_POWDER : MOVE_SOAK;
    enum Type type = (variant & 4) ? TYPE_PSYCHIC : TYPE_WATER;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); MaxHP(999); HP(999); }
        OPPONENT(SPECIES_CORVIKNIGHT) { Speed(300); GigantamaxFactor(gigantamax); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_GUST); MOVE(player, during ? MOVE_CELEBRATE : move); }
        TURN { MOVE(opponent, MOVE_GUST, gimmick: GIMMICK_DYNAMAX); MOVE(player, during ? move : MOVE_MUD_SHOT, hit: TRUE); }
        TURN { MOVE(opponent, MOVE_GUST); MOVE(player, MOVE_TRICK_OR_TREAT); }
        TURN { MOVE(opponent, MOVE_GUST); MOVE(player, MOVE_MUD_SHOT, hit: TRUE); }
        TURN { MOVE(opponent, MOVE_GUST); MOVE(player, MOVE_MUD_SHOT, hit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, gigantamax ? MOVE_G_MAX_WIND_RAGE : MOVE_MAX_AIRSTREAM, opponent);
        if (!during) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_MUD_SHOT, player);
            HP_BAR(opponent);
        }
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK_OR_TREAT, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MUD_SHOT, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GUST, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MUD_SHOT, player);
        HP_BAR(opponent);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_CORVIKNIGHT);
        EXPECT_EQ(opponent->types[0], type);
        EXPECT_EQ(opponent->types[1], type);
        EXPECT_EQ(opponent->types[2], TYPE_GHOST);
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
