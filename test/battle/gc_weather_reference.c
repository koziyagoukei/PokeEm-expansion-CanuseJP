#include "global.h"
#include "test/battle.h"

// GameCube battle request V15 (gba-gc-extension docs/weather.md): the GC's
// weather must give exactly these numbers. Each test is one hit of this
// build's own damage code in a weather set by an ability at the battle
// start, with fixed stats, the damage roll rigged to 100%, 93% and 85% and
// no critical hit; the expected values are the GC core's V15 results for
// the same inputs (its native test test_v15_expansion_reference uses the
// same table). No MESSAGE checks: the text of this build is Japanese.
// This build keeps the Gen 3 powers of Surf, Flamethrower (95) and Heat Wave (100).

#define ROLLS(a, b, c) \
    PARAMETRIZE { roll = 0; expected = a; } \
    PARAMETRIZE { roll = 7; expected = b; } \
    PARAMETRIZE { roll = 15; expected = c; }

#define ONE_HIT(attacker, move) \
    TURN { MOVE(attacker, move, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }

SINGLE_BATTLE_TEST("GC V15 reference: rain x1.5 on Surf at the weather step, before the roll")
{
    s16 dmg; u32 roll, expected;
    ROLLS(96, 88, 81)
    GIVEN {
        ASSUME(GetMovePower(MOVE_SURF) == 95);
        PLAYER(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Level(50); SpAttack(100); Speed(65); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        ONE_HIT(player, MOVE_SURF)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: rain x0.5 on Flamethrower")
{
    s16 dmg; u32 roll, expected;
    ROLLS(21, 19, 17)
    GIVEN {
        ASSUME(GetMovePower(MOVE_FLAMETHROWER) == 95);
        PLAYER(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Level(50); SpAttack(100); Speed(65); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        ONE_HIT(player, MOVE_FLAMETHROWER)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

DOUBLE_BATTLE_TEST("GC V15 reference: sun after the spread x0.75 (Heat Wave)")
{
    s16 left, right; u32 roll, expected;
    ROLLS(91, 84, 76)
    GIVEN {
        ASSUME(GetMovePower(MOVE_HEAT_WAVE) == 100);
        PLAYER(SPECIES_TORKOAL) { Ability(ABILITY_DROUGHT); Level(50); SpAttack(85); Speed(20); }
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(70); MaxHP(999); HP(999); Speed(30); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(70); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        ONE_HIT(playerLeft, MOVE_HEAT_WAVE)
    } SCENE {
        HP_BAR(opponentLeft, captureDamage: &left);
        HP_BAR(opponentRight, captureDamage: &right);
    } THEN {
        EXPECT_EQ(left, expected);
        EXPECT_EQ(right, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: sandstorm raises a Rock type's Sp. Def x1.5")
{
    s16 dmg; u32 roll, expected;
    ROLLS(168, 156, 140)
    GIVEN {
        ASSUME(GetMovePower(MOVE_SURF) == 95);
        PLAYER(SPECIES_TYRANITAR) { Ability(ABILITY_SAND_STREAM); Level(50); SpAttack(95); Speed(61); }
        OPPONENT(SPECIES_GOLEM) { Ability(ABILITY_ROCK_HEAD); Level(50); SpDefense(65); MaxHP(999); HP(999); Speed(45); }
    } WHEN {
        ONE_HIT(player, MOVE_SURF)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: snow raises an Ice type's Defense x1.5")
{
    s16 dmg; u32 roll, expected;
    ROLLS(30, 27, 25)
    GIVEN {
        ASSUME(GetMovePower(MOVE_BODY_SLAM) == 85);
        PLAYER(SPECIES_ABOMASNOW) { Ability(ABILITY_SNOW_WARNING); Level(50); Attack(92); Speed(60); }
        OPPONENT(SPECIES_GLALIE) { Ability(ABILITY_INNER_FOCUS); Level(50); Defense(80); MaxHP(999); HP(999); Speed(80); }
    } WHEN {
        ONE_HIT(player, MOVE_BODY_SLAM)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

// Power Herb skips the charging turn so the hit has its own rigged roll; the
// item does not change the damage.
SINGLE_BATTLE_TEST("GC V15 reference: Solar Beam power x0.5 in rain")
{
    s16 dmg; u32 roll, expected;
    ROLLS(28, 26, 23)
    GIVEN {
        ASSUME(GetMovePower(MOVE_SOLAR_BEAM) == 120);
        PLAYER(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Item(ITEM_POWER_HERB); Level(50); SpAttack(100); Speed(65); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        ONE_HIT(player, MOVE_SOLAR_BEAM)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: Weather Ball in sun is Fire with power 100")
{
    s16 dmg; u32 roll, expected;
    ROLLS(94, 87, 79)
    GIVEN {
        ASSUME(GetMovePower(MOVE_WEATHER_BALL) == 50);
        PLAYER(SPECIES_NINETALES) { Ability(ABILITY_DROUGHT); Level(50); SpAttack(101); Speed(120); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(110); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        ONE_HIT(player, MOVE_WEATHER_BALL)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: Sand Force x1.3 on Rock Slide in sandstorm")
{
    s16 dmg; u32 roll, expected;
    ROLLS(25, 23, 21)
    GIVEN {
        PLAYER(SPECIES_EXCADRILL) { Ability(ABILITY_SAND_FORCE); Level(50); Attack(155); Speed(108); }
        OPPONENT(SPECIES_HIPPOWDON) { Ability(ABILITY_SAND_STREAM); Level(50); Defense(138); MaxHP(999); HP(999); Speed(67); }
    } WHEN {
        ONE_HIT(player, MOVE_ROCK_SLIDE)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: Solar Power x1.5 and sun x1.5 on Flamethrower")
{
    s16 dmg; u32 roll, expected;
    ROLLS(77, 71, 65)
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); Level(50); SpAttack(129); Speed(120); }
        OPPONENT(SPECIES_NINETALES) { Ability(ABILITY_DROUGHT); Level(50); SpDefense(120); MaxHP(999); HP(999); Speed(100); }
    } WHEN {
        ONE_HIT(player, MOVE_FLAMETHROWER)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: Dry Skin x1.25 power and sun x1.5 on Flamethrower")
{
    s16 dmg; u32 roll, expected;
    ROLLS(144, 133, 121)
    GIVEN {
        PLAYER(SPECIES_NINETALES) { Ability(ABILITY_DROUGHT); Level(50); SpAttack(101); Speed(120); }
        OPPONENT(SPECIES_TOXICROAK) { Ability(ABILITY_DRY_SKIN); Level(50); SpDefense(85); MaxHP(999); HP(999); Speed(105); }
    } WHEN {
        ONE_HIT(player, MOVE_FLAMETHROWER)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V15 reference: Cloud Nine on the field cancels the rain boost")
{
    s16 dmg; u32 roll, expected;
    ROLLS(32, 29, 27)
    GIVEN {
        PLAYER(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Level(50); SpAttack(100); Speed(65); }
        OPPONENT(SPECIES_GOLDUCK) { Ability(ABILITY_CLOUD_NINE); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(105); }
    } WHEN {
        ONE_HIT(player, MOVE_SURF)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}
