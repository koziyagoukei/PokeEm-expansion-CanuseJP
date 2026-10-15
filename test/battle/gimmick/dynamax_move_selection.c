#include "global.h"
#include "test/battle.h"
#include "battle_dynamax.h"

DOUBLE_BATTLE_TEST("Dynamax selection: each move slot ignores the previous move's type")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_FLAMETHROWER, MOVE_SURF, MOVE_THUNDERBOLT, MOVE_PROTECT); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_PROTECT); }
    } THEN {
        static const enum Move expected[] = {MOVE_MAX_FLARE, MOVE_MAX_GEYSER, MOVE_MAX_LIGHTNING, MOVE_MAX_GUARD};
        enum BattlerId battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattleStruct->dynamicMoveType = TYPE_GHOST;
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
            EXPECT_EQ(GetMaxMove(battler, playerLeft->moves[slot]), expected[slot]);
        EXPECT_EQ(gBattleStruct->dynamicMoveType, TYPE_GHOST);
    }
}

DOUBLE_BATTLE_TEST("Dynamax selection: changing battlers and returning does not leak dynamic move types")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_WEATHER_BALL, MOVE_ENERGY_BALL, MOVE_ICE_BEAM, MOVE_PROTECT); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_HIDDEN_POWER, MOVE_FLAMETHROWER, MOVE_THUNDERBOLT, MOVE_PROTECT); HPIV(31); AttackIV(31); DefenseIV(31); SpeedIV(31); SpAttackIV(31); SpDefenseIV(31); }
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_PROTECT); MOVE(playerRight, MOVE_PROTECT); }
    } THEN {
        static const enum Move expected[2][MAX_MON_MOVES] = {
            {MOVE_MAX_GEYSER, MOVE_MAX_OVERGROWTH, MOVE_MAX_HAILSTORM, MOVE_MAX_GUARD},
            {MOVE_MAX_DARKNESS, MOVE_MAX_FLARE, MOVE_MAX_LIGHTNING, MOVE_MAX_GUARD},
        };
        gBattleWeather = B_WEATHER_RAIN;
        gBattleStruct->dynamicMoveType = TYPE_NONE;
        for (u32 pass = 0; pass < 3; pass++)
        {
            u32 side = pass % 2;
            enum BattlerId battler = GetBattlerAtPosition(side ? B_POSITION_PLAYER_RIGHT : B_POSITION_PLAYER_LEFT);
            for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
                EXPECT_EQ(GetMaxMove(battler, gBattleMons[battler].moves[slot]), expected[side][slot]);
        }
        EXPECT_EQ(gBattleStruct->dynamicMoveType, TYPE_NONE);
    }
}

SINGLE_BATTLE_TEST("Dynamax selection: previews preserve battle type, category, ability and gem state")
{
    GIVEN {
        PLAYER(SPECIES_SYLVEON) { Ability(ABILITY_PIXILATE); Item(ITEM_FIRE_GEM); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        enum BattlerId battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattleStruct->dynamicMoveType = TYPE_ICE;
        gBattleStruct->dynamicMoveCategory = DAMAGE_CATEGORY_SPECIAL;
        gBattleStruct->battlerState[battler].ateBoost = TRUE;
        gSpecialStatuses[battler].gemBoost = TRUE;
        gSpecialStatuses[battler].gemParam = 17;
        EXPECT_EQ(GetMaxMove(battler, MOVE_FLAMETHROWER), MOVE_MAX_FLARE);
        EXPECT_EQ(GetMaxMove(battler, MOVE_TACKLE), MOVE_MAX_STARFALL);
        EXPECT_EQ(GetMaxMove(battler, MOVE_PROTECT), MOVE_MAX_GUARD);
        EXPECT_EQ(GetMaxMove(battler, MOVE_NONE), MOVE_NONE);
        EXPECT_EQ(GetMaxMove(battler, MOVE_STRUGGLE), MOVE_STRUGGLE);
        EXPECT_EQ(gBattleStruct->dynamicMoveType, TYPE_ICE);
        EXPECT_EQ((u32)gBattleStruct->dynamicMoveCategory, DAMAGE_CATEGORY_SPECIAL);
        EXPECT(gBattleStruct->battlerState[battler].ateBoost);
        EXPECT(gSpecialStatuses[battler].gemBoost);
        EXPECT_EQ((u32)gSpecialStatuses[battler].gemParam, 17);
    }
}

SINGLE_BATTLE_TEST("Dynamax selection: G-Max conversion remains specific to each move type")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { GigantamaxFactor(TRUE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } THEN {
        enum BattlerId battler = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        gBattleStruct->dynamicMoveType = TYPE_FIRE;
        EXPECT_EQ(GetMaxMove(battler, MOVE_FLAMETHROWER), MOVE_G_MAX_WILDFIRE);
        EXPECT_EQ(GetMaxMove(battler, MOVE_AIR_SLASH), MOVE_MAX_AIRSTREAM);
        EXPECT_EQ(GetMaxMove(battler, MOVE_DRAGON_PULSE), MOVE_MAX_WYRMWIND);
        EXPECT_EQ(GetMaxMove(battler, MOVE_PROTECT), MOVE_MAX_GUARD);
    }
}
