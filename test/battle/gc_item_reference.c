#include "global.h"
#include "test/battle.h"

// GameCube battle request V17 (gba-gc-extension docs/items.md): the GC's
// held items must give exactly these numbers and durations. The damage tests
// are one hit of this build's own damage code with fixed stats, the damage
// roll rigged to 100%, 93% and 85% and no critical hit; the expected values
// are the GC core's V17 results for the same inputs (its native test
// test_v17_expansion_reference uses the same table). Muscle Band and Wise
// Glasses truncate their factor (4505): the nearest factor (4506) would give
// 64/58/54 and 72/66/60 instead. The duration tests read the build's own
// weather and terrain counters after one turn. No MESSAGE checks: the text
// of this build is Japanese. This build keeps the Gen 3 power of Thunderbolt,
// Flamethrower and Surf (95).

#define ROLLS(a, b, c) \
    PARAMETRIZE { roll = 0; expected = a; } \
    PARAMETRIZE { roll = 7; expected = b; } \
    PARAMETRIZE { roll = 15; expected = c; }

#define ONE_HIT(attacker, move) \
    TURN { MOVE(attacker, move, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }

#define SNORLAX_TARGET(def, ...) \
    OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); def(100); MaxHP(999); HP(999); Speed(30); __VA_ARGS__ }

SINGLE_BATTLE_TEST("GC V17 reference: Charcoal x1.2 on Flamethrower")
{
    s16 dmg; u32 roll, expected;
    ROLLS(78, 72, 66)
    GIVEN {
        ASSUME(GetMovePower(MOVE_FLAMETHROWER) == 95);
        ASSUME(GetItemHoldEffect(ITEM_CHARCOAL) == HOLD_EFFECT_TYPE_POWER);
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_BLAZE); Level(50); SpAttack(100); Speed(100); Item(ITEM_CHARCOAL); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_FLAMETHROWER)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Sea Incense x1.2 on Surf")
{
    s16 dmg; u32 roll, expected;
    ROLLS(78, 72, 66)
    GIVEN {
        ASSUME(GetMovePower(MOVE_SURF) == 95);
        PLAYER(SPECIES_PELIPPER) { Ability(ABILITY_KEEN_EYE); Level(50); SpAttack(100); Speed(65); Item(ITEM_SEA_INCENSE); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_SURF)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Muscle Band x1.1 truncated on Body Slam 85")
{
    s16 dmg; u32 roll, expected;
    ROLLS(63, 58, 52)
    GIVEN {
        ASSUME(GetMovePower(MOVE_BODY_SLAM) == 85);
        ASSUME(GetItemHoldEffectParam(ITEM_MUSCLE_BAND) == 10);
        PLAYER(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); Attack(100); Speed(30); Item(ITEM_MUSCLE_BAND); }
        SNORLAX_TARGET(Defense)
    } WHEN {
        ONE_HIT(player, MOVE_BODY_SLAM)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Wise Glasses x1.1 truncated on Thunderbolt 95")
{
    s16 dmg; u32 roll, expected;
    ROLLS(70, 64, 58)
    GIVEN {
        ASSUME(GetMovePower(MOVE_THUNDERBOLT) == 95);
        PLAYER(SPECIES_RAICHU) { Ability(ABILITY_STATIC); Level(50); SpAttack(100); Speed(110); Item(ITEM_WISE_GLASSES); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_THUNDERBOLT)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Miracle Seed after Grassy Terrain on Energy Ball")
{
    s16 dmg; u32 roll, expected;
    ROLLS(94, 87, 79)
    GIVEN {
        PLAYER(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); Level(50); SpAttack(100); Speed(85); Item(ITEM_MIRACLE_SEED); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_ENERGY_BALL)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Technician then Charcoal on Ember 40")
{
    s16 dmg; u32 roll, expected;
    ROLLS(33, 30, 28)
    GIVEN {
        ASSUME(GetMovePower(MOVE_EMBER) == 40);
        PLAYER(SPECIES_SCIZOR) { Ability(ABILITY_TECHNICIAN); Level(50); SpAttack(100); Speed(65); Item(ITEM_CHARCOAL); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_EMBER)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Charcoal held by the target does not boost")
{
    s16 dmg; u32 roll, expected;
    ROLLS(64, 58, 54)
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_BLAZE); Level(50); SpAttack(100); Speed(100); }
        SNORLAX_TARGET(SpDefense, Item(ITEM_CHARCOAL);)
    } WHEN {
        ONE_HIT(player, MOVE_FLAMETHROWER)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Muscle Band does not boost a special move")
{
    s16 dmg; u32 roll, expected;
    ROLLS(61, 57, 51)
    GIVEN {
        ASSUME(GetMovePower(MOVE_HYPER_VOICE) == 90);
        PLAYER(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpAttack(100); Speed(30); Item(ITEM_MUSCLE_BAND); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_HYPER_VOICE)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

// Durations (TryChangeWeather / TryChangeBattleTerrain): 8 with the setter's
// rock or Terrain Extender, else 5; after one turn the counter shows 7 or 4.
SINGLE_BATTLE_TEST("GC V17 reference: Damp Rock makes Rain Dance last 8 turns")
{
    u32 item, left;
    PARAMETRIZE { item = ITEM_DAMP_ROCK; left = 7; }
    PARAMETRIZE { item = ITEM_HEAT_ROCK; left = 4; }
    PARAMETRIZE { item = ITEM_NONE; left = 4; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_RAIN_DANCE); }
    } THEN {
        EXPECT(gBattleWeather & B_WEATHER_RAIN);
        EXPECT_EQ(gBattleStruct->weatherDuration, left);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Heat, Smooth and Icy Rock on their own weather")
{
    u32 item, move, left;
    PARAMETRIZE { item = ITEM_HEAT_ROCK; move = MOVE_SUNNY_DAY; left = 7; }
    PARAMETRIZE { item = ITEM_SMOOTH_ROCK; move = MOVE_SANDSTORM; left = 7; }
    PARAMETRIZE { item = ITEM_ICY_ROCK; move = MOVE_SNOWSCAPE; left = 7; }
    PARAMETRIZE { item = ITEM_DAMP_ROCK; move = MOVE_SUNNY_DAY; left = 4; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); }
    } THEN {
        EXPECT_EQ(gBattleStruct->weatherDuration, left);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Drizzle with Damp Rock lasts 8 turns")
{
    u32 item, left;
    PARAMETRIZE { item = ITEM_DAMP_ROCK; left = 7; }
    PARAMETRIZE { item = ITEM_NONE; left = 4; }
    GIVEN {
        ASSUME(GetConfig(B_ABILITY_WEATHER) >= GEN_6);
        PLAYER(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } THEN {
        EXPECT(gBattleWeather & B_WEATHER_RAIN);
        EXPECT_EQ(gBattleStruct->weatherDuration, left);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Terrain Extender makes a terrain last 8 turns")
{
    u32 item, left;
    PARAMETRIZE { item = ITEM_TERRAIN_EXTENDER; left = 7; }
    PARAMETRIZE { item = ITEM_NONE; left = 4; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_GRASSY_TERRAIN); }
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_GRASSY);
        EXPECT_EQ(gFieldTimers.terrainTimer, left);
    }
}

SINGLE_BATTLE_TEST("GC V17 reference: Terrain Extender on a surge ability")
{
    u32 item, left;
    PARAMETRIZE { item = ITEM_TERRAIN_EXTENDER; left = 7; }
    PARAMETRIZE { item = ITEM_NONE; left = 4; }
    GIVEN {
        PLAYER(SPECIES_TAPU_KOKO) { Ability(ABILITY_ELECTRIC_SURGE); Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { }
    } THEN {
        EXPECT_EQ(gFieldTimers.terrain, B_TERRAIN_ELECTRIC);
        EXPECT_EQ(gFieldTimers.terrainTimer, left);
    }
}
