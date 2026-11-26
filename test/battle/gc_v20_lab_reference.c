#include "global.h"
#include "test/battle.h"

// M4-B1c-0c (gba-gc-extension docs/items.md section 15): this build's damage
// for the hits of the V20 Lab fixtures that the GC ran in Dolphin, with the
// stats those fixtures send (test "GC V20 export: Lab fixtures"), at every
// damage roll, with and without Life Orb. The HP is raised so nothing faints
// (the GC logs the damage before the HP limit). The Dolphin values must be
// a (none, Life Orb) pair of one roll (docs/items.md 15.3).

#define LO(d) (((d) * 5324 + 2047) / 4096)

#define LAB_ROLLS                                               \
    for (u32 r = 0; r < 16; r++)                                \
    {                                                           \
        PARAMETRIZE { item = ITEM_NONE; roll = r; }             \
        PARAMETRIZE { item = ITEM_LIFE_ORB; roll = r; }         \
    }

#define PRINT_LAB_PAIRS(tag)                                                                  \
    for (u32 k = 0; k < 32; k += 2)                                                           \
    {                                                                                         \
        Test_MgbaPrintf(tag " roll=%d none=%d lo=%d", k / 2, results[k].damage, results[k + 1].damage); \
        EXPECT_EQ(results[k + 1].damage, LO(results[k].damage));                              \
    }

SINGLE_BATTLE_TEST("GC V20 Lab reference: L73/L79 Body Slam, Snorlax into Blissey", s16 damage)
{
    enum Item item = ITEM_NONE; u32 roll = 0;
    LAB_ROLLS
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_THICK_FAT); Attack(122); SpAttack(77); MaxHP(227); HP(227); Speed(42); Item(item); }
        OPPONENT(SPECIES_BLISSEY) { Level(50); Ability(ABILITY_SERENE_GRACE); Defense(24); SpDefense(147); MaxHP(999); HP(999); Speed(67); }
    } WHEN {
        TURN { MOVE(player, MOVE_BODY_SLAM, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        PRINT_LAB_PAIRS("V20LABREF L73 body_slam");
    }
}

SINGLE_BATTLE_TEST("GC V20 Lab reference: L73 Giga Drain, Snorlax into Snorlax", s16 damage)
{
    enum Item item = ITEM_NONE; u32 roll = 0;
    LAB_ROLLS
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_THICK_FAT); Attack(122); SpAttack(77); MaxHP(227); HP(227); Speed(42); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_IMMUNITY); Defense(84); SpDefense(109); MaxHP(999); HP(999); Speed(42); }
    } WHEN {
        TURN { MOVE(player, MOVE_GIGA_DRAIN, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        PRINT_LAB_PAIRS("V20LABREF L73 giga_drain");
    }
}

SINGLE_BATTLE_TEST("GC V20 Lab reference: L73 Double Hit (first hit), Snorlax into Snorlax", s16 damage)
{
    enum Item item = ITEM_NONE; u32 roll = 0;
    LAB_ROLLS
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_THICK_FAT); Attack(122); SpAttack(77); MaxHP(227); HP(227); Speed(42); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_IMMUNITY); Defense(84); SpDefense(109); MaxHP(999); HP(999); Speed(42); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_HIT, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE, hit: TRUE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        PRINT_LAB_PAIRS("V20LABREF L73 double_hit");
    }
}

SINGLE_BATTLE_TEST("GC V20 Lab reference: L78/L80 Double-Edge and Body Slam into Snorlax", s16 damage)
{
    enum Item item = ITEM_NONE; u32 roll = 0;
    LAB_ROLLS
    GIVEN {
        PLAYER(SPECIES_SALAMENCE) { Level(50); Ability(ABILITY_MOXIE); Attack(147); MaxHP(162); HP(162); Speed(112); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_IMMUNITY); Defense(84); SpDefense(122); MaxHP(999); HP(999); Speed(42); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_EDGE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        PRINT_LAB_PAIRS("V20LABREF L78 double_edge");
    }
}

// The order of L75's turn 1 (Dolphin, M4-B1c-0b GC: the loss first): the
// target's Sitrus Berry comes BEFORE the attacker's Life Orb loss. Every
// Pokemon but the attacker eats its HP berry at MOVEEND_KEE_MARANGA_HP_
// THRESHOLD_ITEM_TARGET, then MOVEEND_LIFE_ORB_SHELL_BELL; the GC follows
// since M4-B1c-0c (docs/items.md 15.4).
SINGLE_BATTLE_TEST("GC V20 Lab reference: L75 the target's Sitrus Berry before the attacker's Life Orb")
{
    GIVEN {
        PLAYER(SPECIES_AERODACTYL) { Level(50); Ability(ABILITY_ROCK_HEAD); Attack(117); MaxHP(147); HP(147); Speed(142); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_IMMUNITY); Defense(84); MaxHP(227); HP(136); Speed(42); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE, criticalHit: FALSE, hit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponent);
        HP_BAR(opponent, damage: -56);
        HP_BAR(player, damage: 14);
    }
}

// The same order is not met when the loss comes first.
SINGLE_BATTLE_TEST("GC V20 Lab reference: L75 no Life Orb loss before the target's Sitrus Berry")
{
    GIVEN {
        PLAYER(SPECIES_AERODACTYL) { Level(50); Ability(ABILITY_ROCK_HEAD); Attack(117); MaxHP(147); HP(147); Speed(142); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_IMMUNITY); Defense(84); MaxHP(227); HP(136); Speed(42); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE, criticalHit: FALSE, hit: TRUE); }
    } SCENE {
        HP_BAR(opponent);
        NONE_OF { HP_BAR(player, damage: 14); }
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, opponent);
    }
}
