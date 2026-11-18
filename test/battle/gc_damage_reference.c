#include "global.h"
#include "test/battle.h"

// GameCube battle request V14 (gba-gc-extension docs/modifiers.md): the GC's
// fixed-point damage arithmetic must give exactly these numbers. Each test
// is one hit of this build's own damage code with fixed stats, the damage
// roll rigged to 100%, 93% and 85% and no critical hit; the expected values
// are the GC core's V14 results for the same inputs (its native test
// test_v14_expansion_reference uses the same table). The cases put several
// modifiers into one stage, where the old per-step flooring differed.
// No MESSAGE checks: the text of this build is Japanese.

#define ROLLS(a, b, c) \
    PARAMETRIZE { roll = 0; expected = a; } \
    PARAMETRIZE { roll = 7; expected = b; } \
    PARAMETRIZE { roll = 15; expected = c; }

SINGLE_BATTLE_TEST("GC V14 reference: Ice Fang x4 with STAB (Gen 5 example)")
{
    s16 dmg; u32 roll, expected;
    ROLLS(196, 180, 168)
    GIVEN {
        ASSUME(GetMovePower(MOVE_ICE_FANG) == 65);
        PLAYER(SPECIES_GLACEON) { Level(75); Attack(123); }
        OPPONENT(SPECIES_GARCHOMP) { Level(50); Defense(163); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_FANG, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Guts and Thick Fat chain the attack to x0.75")
{
    s16 dmg; u32 roll, expected;
    ROLLS(33, 30, 28)
    GIVEN {
        ASSUME(GetMovePower(MOVE_ICE_PUNCH) == 75);
        PLAYER(SPECIES_MACHAMP) { Ability(ABILITY_GUTS); Level(50); Attack(101); Status1(STATUS1_POISON); Speed(55); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_THICK_FAT); Level(50); Defense(80); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Blaze at 1/3 HP and Thick Fat")
{
    s16 dmg; u32 roll, expected;
    ROLLS(52, 48, 43)
    GIVEN {
        ASSUME(GetMovePower(MOVE_FIRE_PUNCH) == 75);
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_BLAZE); Level(50); Attack(103); MaxHP(150); HP(50); Speed(100); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_THICK_FAT); Level(50); Defense(77); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_FIRE_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Defeatist and Thick Fat chain the attack to x0.25")
{
    s16 dmg; u32 roll, expected;
    ROLLS(17, 15, 14)
    GIVEN {
        ASSUME(GetMovePower(MOVE_FIRE_PUNCH) == 75);
        PLAYER(SPECIES_ARCHEOPS) { Ability(ABILITY_DEFEATIST); Level(50); Attack(111); MaxHP(150); HP(75); Speed(110); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_THICK_FAT); Level(50); Defense(61); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_FIRE_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: a double resistance rounds x0.25 to the nearest")
{
    s16 dmg; u32 roll, expected;
    ROLLS(11, 10, 9)
    GIVEN {
        ASSUME(GetMovePower(MOVE_EMBER) == 40);
        PLAYER(SPECIES_CHARMANDER) { Level(50); SpAttack(90); Speed(65); }
        OPPONENT(SPECIES_OMASTAR) { Level(50); SpDefense(57); MaxHP(999); HP(999); Speed(55); }
    } WHEN {
        TURN { MOVE(player, MOVE_EMBER, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

DOUBLE_BATTLE_TEST("GC V14 reference: the spread x0.75 comes before the damage roll")
{
    s16 left, right; u32 roll, expected;
    ROLLS(60, 55, 51)
    GIVEN {
        ASSUME(GetMovePower(MOVE_ROCK_SLIDE) == 75);
        PLAYER(SPECIES_GOLEM) { Level(50); Attack(100); Speed(45); }
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Speed(20); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); Defense(64); MaxHP(999); HP(999); Speed(30); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); Defense(64); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROCK_SLIDE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponentLeft, captureDamage: &left);
        HP_BAR(opponentRight, captureDamage: &right);
    } THEN {
        EXPECT_EQ(left, expected);
        EXPECT_EQ(right, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Technician on each hit of Double Kick")
{
    s16 first, second; u32 roll, expected;
    ROLLS(66, 60, 56)
    GIVEN {
        ASSUME(GetMovePower(MOVE_DOUBLE_KICK) == 30);
        PLAYER(SPECIES_SCIZOR) { Ability(ABILITY_TECHNICIAN); Level(50); Attack(130); Speed(65); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); Defense(83); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &first);
        HP_BAR(opponent, captureDamage: &second);
    } THEN {
        EXPECT_EQ(first, expected);
        EXPECT_EQ(second, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Hex against a poisoned target")
{
    s16 dmg; u32 roll, expected;
    ROLLS(111, 102, 93)
    GIVEN {
        ASSUME(GetMovePower(MOVE_HEX) == 65);
        PLAYER(SPECIES_MISDREAVUS) { Level(50); SpAttack(85); Speed(85); }
        OPPONENT(SPECIES_MACHOP) { Level(50); SpDefense(67); MaxHP(999); HP(999); Status1(STATUS1_POISON); Speed(35); }
    } WHEN {
        TURN { MOVE(player, MOVE_HEX, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Brine at half HP, then Flare Boost")
{
    s16 dmg; u32 roll, expected;
    ROLLS(107, 99, 90)
    GIVEN {
        ASSUME(GetMovePower(MOVE_BRINE) == 65);
        PLAYER(SPECIES_DRIFBLIM) { Ability(ABILITY_FLARE_BOOST); Level(50); SpAttack(90); Status1(STATUS1_BURN); Speed(80); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); SpDefense(73); MaxHP(999); HP(499); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_BRINE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Toxic Boost")
{
    s16 dmg; u32 roll, expected;
    ROLLS(73, 67, 62)
    GIVEN {
        ASSUME(GetMovePower(MOVE_ICE_PUNCH) == 75);
        PLAYER(SPECIES_ZANGOOSE) { Ability(ABILITY_TOXIC_BOOST); Level(50); Attack(115); Status1(STATUS1_POISON); Speed(90); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); Defense(79); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V14 reference: Earthquake doubles against a Pokemon underground")
{
    s16 dmg; u32 roll, expected;
    ROLLS(170, 158, 144)
    GIVEN {
        ASSUME(GetMovePower(MOVE_EARTHQUAKE) == 100);
        PLAYER(SPECIES_GOLEM) { Level(50); Attack(110); Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); Defense(87); MaxHP(999); HP(999); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_DIG); MOVE(player, MOVE_EARTHQUAKE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}
