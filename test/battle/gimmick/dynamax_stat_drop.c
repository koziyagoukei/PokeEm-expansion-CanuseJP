#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("Dynamax stat drops: the surviving partner is affected even when the target faints")
{
    enum Move move = MOVE_NONE;
    enum Stat stat = STAT_HP;
    u32 position = 0;
    bool32 knockout = FALSE;
    for (u32 slot = 0; slot < MAX_BATTLERS_COUNT; slot++)
    {
        for (u32 ko = 0; ko < 2; ko++)
        {
            PARAMETRIZE { position = slot; knockout = ko; move = MOVE_SCRATCH; stat = STAT_SPEED; }
            PARAMETRIZE { position = slot; knockout = ko; move = MOVE_FURY_CUTTER; stat = STAT_SPATK; }
            PARAMETRIZE { position = slot; knockout = ko; move = MOVE_LICK; stat = STAT_DEF; }
            PARAMETRIZE { position = slot; knockout = ko; move = MOVE_DRAGON_CLAW; stat = STAT_ATK; }
            PARAMETRIZE { position = slot; knockout = ko; move = MOVE_CRUNCH; stat = STAT_SPDEF; }
        }
    }
    struct BattlePokemon *target = &gBattleMons[position];
    struct BattlePokemon *partner = &gBattleMons[position ^ BIT_FLANK];
    struct BattlePokemon *user = &gBattleMons[(position ^ BIT_SIDE) & BIT_SIDE];
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(1000); HP(knockout && position == 0 ? 1 : 1000); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(1000); HP(knockout && position == 2 ? 1 : 1000); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(1000); HP(knockout && position == 1 ? 1 : 1000); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(1000); HP(knockout && position == 3 ? 1 : 1000); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {
            MOVE(user, move, target: target, gimmick: GIMMICK_DYNAMAX);
            if (knockout) SEND_OUT(target, 2);
        }
    } SCENE {
        if (knockout) HP_BAR(target, hp: 0);
        else HP_BAR(target);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, partner);
    } THEN {
        EXPECT_EQ(partner->statStages[stat], DEFAULT_STAT_STAGE - 1);
        EXPECT_EQ(target->statStages[stat], DEFAULT_STAT_STAGE - !knockout);
        EXPECT_EQ(user->statStages[stat], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("Dynamax stat drops: the surviving partner keeps its own stat protections and abilities")
{
    enum Ability ability = ABILITY_SHADOW_TAG;
    enum Item item = ITEM_NONE;
    s32 change = -1;
    PARAMETRIZE { ability = ABILITY_SHADOW_TAG; item = ITEM_NONE; change = -1; }
    PARAMETRIZE { ability = ABILITY_CLEAR_BODY; item = ITEM_NONE; change = 0; }
    PARAMETRIZE { ability = ABILITY_SHADOW_TAG; item = ITEM_CLEAR_AMULET; change = 0; }
    PARAMETRIZE { ability = ABILITY_CONTRARY; item = ITEM_NONE; change = 1; }
    PARAMETRIZE { ability = ABILITY_SIMPLE; item = ITEM_NONE; change = -2; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Ability(ABILITY_CLEAR_BODY); }
        OPPONENT(SPECIES_WOBBUFFET) { Ability(ability); Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft, gimmick: GIMMICK_DYNAMAX); SEND_OUT(opponentLeft, 2); }
    } THEN {
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + change);
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}

DOUBLE_BATTLE_TEST("Dynamax stat drops: an immune target does not trigger the side effect")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GENGAR);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft, gimmick: GIMMICK_DYNAMAX); }
    } THEN {
        EXPECT_EQ(opponentLeft->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
        EXPECT_EQ(opponentRight->statStages[STAT_SPEED], DEFAULT_STAT_STAGE);
    }
}
