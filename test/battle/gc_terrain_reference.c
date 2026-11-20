#include "global.h"
#include "test/battle.h"

// GameCube battle request V16 (gba-gc-extension docs/terrain.md): the GC's
// terrain must give exactly these numbers and behaviours. The damage tests
// are one hit of this build's own damage code on a terrain set by a surge
// ability at the battle start, with fixed stats, the damage roll rigged to
// 100%, 93% and 85% and no critical hit; the expected values are the GC
// core's V16 results for the same inputs (its native test
// test_v16_expansion_reference uses the same table). The behaviour tests
// check what the GC's native tests check: priority blocking per target,
// status prevention, and the Grassy heal before the poison damage.
// No MESSAGE checks: the text of this build is Japanese. This build keeps
// the Gen 3 power of Thunderbolt (95) and the Gen 4 Dragon Pulse (90).

#define ROLLS(a, b, c) \
    PARAMETRIZE { roll = 0; expected = a; } \
    PARAMETRIZE { roll = 7; expected = b; } \
    PARAMETRIZE { roll = 15; expected = c; }

#define ONE_HIT(attacker, move) \
    TURN { MOVE(attacker, move, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }

#define SNORLAX_TARGET(def) \
    OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_IMMUNITY); Level(50); def(100); MaxHP(999); HP(999); Speed(30); }

SINGLE_BATTLE_TEST("GC V16 reference: Electric Terrain x1.3 on Thunderbolt from a grounded user")
{
    s16 dmg; u32 roll, expected;
    ROLLS(84, 78, 70)
    GIVEN {
        ASSUME(GetMovePower(MOVE_THUNDERBOLT) == 95);
        PLAYER(SPECIES_TAPU_KOKO) { Ability(ABILITY_ELECTRIC_SURGE); Level(50); SpAttack(100); Speed(130); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_THUNDERBOLT)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Electric Terrain does not boost a Flying user")
{
    s16 dmg; u32 roll, expected;
    ROLLS(32, 29, 27)
    GIVEN {
        PLAYER(SPECIES_ZAPDOS) { Ability(ABILITY_PRESSURE); Level(50); SpAttack(100); Speed(100); }
        OPPONENT(SPECIES_PINCURCHIN) { Ability(ABILITY_ELECTRIC_SURGE); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(15); }
    } WHEN {
        ONE_HIT(player, MOVE_THUNDERBOLT)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Grassy Terrain x1.3 on Energy Ball")
{
    s16 dmg; u32 roll, expected;
    ROLLS(79, 73, 67)
    GIVEN {
        ASSUME(GetMovePower(MOVE_ENERGY_BALL) == 90);
        PLAYER(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); Level(50); SpAttack(100); Speed(85); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_ENERGY_BALL)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Grassy Terrain halves Earthquake")
{
    s16 dmg; u32 roll, expected;
    ROLLS(24, 22, 20)
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_EARTHQUAKE) == EFFECT_EARTHQUAKE);
        PLAYER(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); Level(50); Attack(100); Speed(85); }
        SNORLAX_TARGET(Defense)
    } WHEN {
        ONE_HIT(player, MOVE_EARTHQUAKE)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Misty Terrain halves Dragon Pulse on a grounded target")
{
    s16 dmg; u32 roll, expected;
    ROLLS(21, 19, 17)
    GIVEN {
        ASSUME(GetMovePower(MOVE_DRAGON_PULSE) == 90);
        PLAYER(SPECIES_TAPU_FINI) { Ability(ABILITY_MISTY_SURGE); Level(50); SpAttack(100); Speed(85); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_DRAGON_PULSE)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Misty Terrain does not halve Dragon Pulse on a Flying target")
{
    s16 dmg; u32 roll, expected;
    ROLLS(41, 38, 34)
    GIVEN {
        PLAYER(SPECIES_TAPU_FINI) { Ability(ABILITY_MISTY_SURGE); Level(50); SpAttack(100); Speed(85); }
        OPPONENT(SPECIES_PIDGEOT) { Ability(ABILITY_KEEN_EYE); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        ONE_HIT(player, MOVE_DRAGON_PULSE)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Psychic Terrain x1.3 on Psychic")
{
    s16 dmg; u32 roll, expected;
    ROLLS(79, 73, 67)
    GIVEN {
        ASSUME(GetMovePower(MOVE_PSYCHIC) == 90);
        PLAYER(SPECIES_TAPU_LELE) { Ability(ABILITY_PSYCHIC_SURGE); Level(50); SpAttack(100); Speed(95); }
        SNORLAX_TARGET(SpDefense)
    } WHEN {
        ONE_HIT(player, MOVE_PSYCHIC)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

// Power Herb skips the charging turn so the hit has its own rigged roll.
SINGLE_BATTLE_TEST("GC V16 reference: rain and Grassy Terrain chain Solar Beam's power")
{
    s16 dmg; u32 roll, expected;
    ROLLS(54, 49, 45)
    GIVEN {
        PLAYER(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); Item(ITEM_POWER_HERB); Level(50); SpAttack(100); Speed(85); }
        OPPONENT(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(65); }
    } WHEN {
        ONE_HIT(player, MOVE_SOLAR_BEAM)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Grass Pelt raises Defense x1.5 on Grassy Terrain")
{
    s16 dmg; u32 roll, expected;
    ROLLS(26, 24, 22)
    GIVEN {
        PLAYER(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); Level(50); Attack(100); Speed(85); }
        OPPONENT(SPECIES_GOGOAT) { Ability(ABILITY_GRASS_PELT); Level(50); Defense(100); MaxHP(999); HP(999); Speed(68); }
    } WHEN {
        ONE_HIT(player, MOVE_BODY_SLAM)
    } SCENE {
        HP_BAR(opponent, captureDamage: &dmg);
    } THEN {
        EXPECT_EQ(dmg, expected);
    }
}

DOUBLE_BATTLE_TEST("GC V16 reference: Psychic Terrain stops a priority move per target, not on a Flying one")
{
    GIVEN {
        ASSUME(GetMovePriority(MOVE_QUICK_ATTACK) == 1);
        PLAYER(SPECIES_RATICATE) { Ability(ABILITY_GUTS); Speed(100); }
        PLAYER(SPECIES_RATICATE) { Ability(ABILITY_GUTS); Speed(90); }
        OPPONENT(SPECIES_TAPU_LELE) { Ability(ABILITY_PSYCHIC_SURGE); Speed(50); }
        OPPONENT(SPECIES_PIDGEOT) { Ability(ABILITY_KEEN_EYE); Speed(40); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_QUICK_ATTACK, target: opponentRight); MOVE(playerRight, MOVE_QUICK_ATTACK, target: opponentLeft);
               MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
    } SCENE {
        HP_BAR(opponentRight);
        NONE_OF { HP_BAR(opponentLeft); }
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Electric Terrain stops sleep on a grounded Pokemon")
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_ELECTRIC_SURGE; }
    PARAMETRIZE { ability = ABILITY_TELEPATHY; }
    GIVEN {
        PLAYER(SPECIES_TAPU_KOKO) { Ability(ability); Speed(10); }
        OPPONENT(SPECIES_BRELOOM) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SPORE); MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ((player->status1 & STATUS1_SLEEP) != 0, ability == ABILITY_TELEPATHY);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Misty Terrain stops Toxic on a grounded Pokemon")
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_MISTY_SURGE; }
    PARAMETRIZE { ability = ABILITY_TELEPATHY; }
    GIVEN {
        PLAYER(SPECIES_TAPU_FINI) { Ability(ability); Speed(10); }
        OPPONENT(SPECIES_ARBOK) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TOXIC); MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ((player->status1 & STATUS1_TOXIC_POISON) != 0, ability == ABILITY_TELEPATHY);
    }
}

SINGLE_BATTLE_TEST("GC V16 reference: Grassy Terrain heals 1/16 before the poison damage")
{
    s16 heal, poison;
    GIVEN {
        PLAYER(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); MaxHP(160); HP(100); Status1(STATUS1_POISON); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        HP_BAR(player, captureDamage: &heal);
        HP_BAR(player, captureDamage: &poison);
    } THEN {
        EXPECT_EQ(heal, -10);
        EXPECT_EQ(poison, 20);
    }
}
