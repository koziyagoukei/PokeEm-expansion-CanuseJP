#include "global.h"
#include "test/battle.h"

// GameCube battle request V18 (gba-gc-extension docs/items.md): the GC's
// end-of-turn Leftovers and Black Sludge must give exactly these HP changes,
// in this order. Each case is one turn of Celebrate with fixed max HP, HP and
// Speed; the GC core's native test test_v18_end_turn_reference has the same
// cases (same numbers, same order of events). The item's own HP_BAR follows
// its B_ANIM_HELD_ITEM_EFFECT (a heal) or B_ANIM_MON_HIT (Black Sludge's
// damage), which tells it from the Grassy Terrain heal of the same amount.
// No MESSAGE checks: the text of this build is Japanese.

ASSUMPTIONS
{
    ASSUME(GetItemHoldEffect(ITEM_LEFTOVERS) == HOLD_EFFECT_LEFTOVERS);
    ASSUME(GetItemHoldEffect(ITEM_BLACK_SLUDGE) == HOLD_EFFECT_BLACK_SLUDGE);
    ASSUME(GetSpeciesType(SPECIES_EKANS, 0) == TYPE_POISON);
    ASSUME(GetSpeciesType(SPECIES_WOBBUFFET, 0) != TYPE_POISON && GetSpeciesType(SPECIES_WOBBUFFET, 1) != TYPE_POISON);
}

#define FOE OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Speed(50); }
#define ITEM_HEAL(battler, amount) \
    ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, battler); HP_BAR(battler, damage: -(amount))
#define ITEM_HURT(battler, amount) \
    ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, battler); HP_BAR(battler, damage: (amount))

SINGLE_BATTLE_TEST("GC V18 reference: Leftovers heals max HP / 16, rounded down")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(175); HP(100); Speed(100); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(player, 10);
    } THEN {
        EXPECT_EQ(player->hp, 110);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Leftovers stops at max HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(155); Speed(100); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        HP_BAR(player);
    } THEN {
        EXPECT_EQ(player->hp, 160);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Leftovers does nothing at full HP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(160); Speed(100); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(player->hp, 160);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Leftovers heals at least 1, and from 1 HP")
{
    u32 maxHP, hp, healed;
    PARAMETRIZE { maxHP = 15; hp = 10; healed = 1; }
    PARAMETRIZE { maxHP = 160; hp = 1; healed = 10; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(maxHP); HP(hp); Speed(100); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(player, healed);
    } THEN {
        EXPECT_EQ(player->hp, hp + healed);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Black Sludge heals a Poison type by max HP / 16")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { MaxHP(161); HP(100); Speed(100); Item(ITEM_BLACK_SLUDGE); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(player, 10);
    } THEN {
        EXPECT_EQ(player->hp, 110);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Black Sludge does nothing to a Poison type at full HP")
{
    GIVEN {
        PLAYER(SPECIES_EKANS) { MaxHP(161); HP(161); Speed(100); Item(ITEM_BLACK_SLUDGE); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, player);
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(player->hp, 161);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Black Sludge hurts any other type by max HP / 8, at least 1")
{
    u32 maxHP, hurt;
    PARAMETRIZE { maxHP = 161; hurt = 20; }
    PARAMETRIZE { maxHP = 7; hurt = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(maxHP); HP(maxHP); Speed(100); Item(ITEM_BLACK_SLUDGE); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HURT(player, hurt);
    } THEN {
        EXPECT_EQ(player->hp, maxHP - hurt);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Black Sludge can make its holder faint")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(20); Speed(100); Item(ITEM_BLACK_SLUDGE); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HURT(player, 20);
    } THEN {
        EXPECT_EQ(player->hp, 0);
    }
}

#define GRASSY_FOE OPPONENT(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); MaxHP(200); HP(200); Speed(50); }

SINGLE_BATTLE_TEST("GC V18 reference: Grassy Terrain heals first, then Leftovers")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(100); Speed(100); Item(ITEM_LEFTOVERS); }
        GRASSY_FOE
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player, damage: -10);
        ITEM_HEAL(player, 10);
    } THEN {
        EXPECT_EQ(player->hp, 120);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Grassy Terrain fills the HP, Leftovers then does nothing")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(150); Speed(100); Item(ITEM_LEFTOVERS); }
        GRASSY_FOE
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player, damage: -10);
        NONE_OF { ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player); }
    } THEN {
        EXPECT_EQ(player->hp, 160);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Grassy Terrain heals first, then Black Sludge")
{
    u32 species, after;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; after = 90; }
    PARAMETRIZE { species = SPECIES_EKANS; after = 120; }
    GIVEN {
        PLAYER(species) { MaxHP(160); HP(100); Speed(100); Item(ITEM_BLACK_SLUDGE); }
        GRASSY_FOE
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player, damage: -10);
        if (species == SPECIES_EKANS) {
            ITEM_HEAL(player, 10);
        } else {
            ITEM_HURT(player, 20);
        }
    } THEN {
        EXPECT_EQ(player->hp, after);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Leftovers before poison, bad poison and burn damage")
{
    u32 status, hurt;
    PARAMETRIZE { status = STATUS1_POISON; hurt = 20; }
    PARAMETRIZE { status = STATUS1_TOXIC_POISON; hurt = 10; }
    PARAMETRIZE { status = STATUS1_BURN; hurt = 10; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(100); Speed(100); Status1(status); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(player, 10);
        HP_BAR(player, damage: hurt);
    } THEN {
        EXPECT_EQ(player->hp, 110 - hurt);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Sandstorm and hail damage first, then Leftovers")
{
    u32 foe, move;
    PARAMETRIZE { foe = SPECIES_TYRANITAR; move = MOVE_CELEBRATE; }
    PARAMETRIZE { foe = SPECIES_GLALIE; move = MOVE_HAIL; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HAIL) == EFFECT_WEATHER);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(100); Speed(100); Item(ITEM_LEFTOVERS); }
        OPPONENT(foe) { Ability(foe == SPECIES_TYRANITAR ? ABILITY_SAND_STREAM : ABILITY_INNER_FOCUS);
                        MaxHP(200); HP(200); Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        HP_BAR(player, damage: 10);
        ITEM_HEAL(player, 10);
    } THEN {
        EXPECT_EQ(player->hp, 100);
        if (foe == SPECIES_GLALIE)
            EXPECT(gBattleWeather & B_WEATHER_HAIL);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Rain Dish heals first, then Leftovers")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_RAIN_DISH); MaxHP(160); HP(100); Speed(100); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_PELIPPER) { Ability(ABILITY_DRIZZLE); MaxHP(200); HP(200); Speed(50); }
    } WHEN {
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ABILITY_RAIN_DISH);
        HP_BAR(player, damage: -10);
        ITEM_HEAL(player, 10);
    } THEN {
        EXPECT_EQ(player->hp, 120);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Shed Skin cures the poison, then Leftovers, no poison damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SHED_SKIN); MaxHP(160); HP(100); Speed(100);
                                    Status1(STATUS1_POISON); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE, WITH_RNG(RNG_SHED_SKIN, TRUE)); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_SHED_SKIN);
        ITEM_HEAL(player, 10);
        NONE_OF { HP_BAR(player); }
    } THEN {
        EXPECT_EQ(player->status1, 0);
        EXPECT_EQ(player->hp, 110);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Leftovers, then Speed Boost")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_SPEED_BOOST); MaxHP(160); HP(100); Speed(100); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(player, 10);
        ABILITY_POPUP(player, ABILITY_SPEED_BOOST);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_SPEED], DEFAULT_STAT_STAGE + 1);
    }
}

DOUBLE_BATTLE_TEST("GC V18 reference: each Pokemon in speed order, fastest first")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(3); Item(ITEM_LEFTOVERS); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(1); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(2); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(4); Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(opponentRight, 12);
        ITEM_HEAL(playerLeft, 12);
        ITEM_HEAL(opponentLeft, 12);
        ITEM_HEAL(playerRight, 12);
    }
}

DOUBLE_BATTLE_TEST("GC V18 reference: equal speeds keep the battler order")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(50); Item(ITEM_LEFTOVERS); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(50); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(50); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(50); Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN {}
    } SCENE {
        ITEM_HEAL(playerLeft, 12);
        ITEM_HEAL(opponentLeft, 12);
        ITEM_HEAL(playerRight, 12);
        ITEM_HEAL(opponentRight, 12);
    }
}

DOUBLE_BATTLE_TEST("GC V18 reference: Pokemon by Pokemon, the terrain heal and the item of one, then the next")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(4); Item(ITEM_LEFTOVERS); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(2); Item(ITEM_BLACK_SLUDGE); }
        OPPONENT(SPECIES_RILLABOOM) { Ability(ABILITY_GRASSY_SURGE); MaxHP(200); HP(100); Speed(3); Item(ITEM_BLACK_SLUDGE); }
        OPPONENT(SPECIES_EKANS) { MaxHP(200); HP(100); Speed(1); Item(ITEM_BLACK_SLUDGE); }
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(playerLeft, damage: -12);
        ITEM_HEAL(playerLeft, 12);
        HP_BAR(opponentLeft, damage: -12);
        ITEM_HURT(opponentLeft, 25);
        HP_BAR(playerRight, damage: -12);
        ITEM_HURT(playerRight, 25);
        HP_BAR(opponentRight, damage: -12);
        ITEM_HEAL(opponentRight, 12);
    } THEN {
        EXPECT_EQ(playerLeft->hp, 124);
        EXPECT_EQ(opponentLeft->hp, 87);
        EXPECT_EQ(playerRight->hp, 87);
        EXPECT_EQ(opponentRight->hp, 124);
    }
}

DOUBLE_BATTLE_TEST("GC V18 reference: a faint to Black Sludge, the others go on, then the status damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(10); Speed(4); Item(ITEM_BLACK_SLUDGE); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(2); Status1(STATUS1_POISON); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(3); Item(ITEM_LEFTOVERS); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(1); Status1(STATUS1_BURN); Item(ITEM_LEFTOVERS); }
    } WHEN {
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MON_HIT, playerLeft);
        HP_BAR(playerLeft);
        ITEM_HEAL(opponentLeft, 12);
        ITEM_HEAL(playerRight, 12);
        ITEM_HEAL(opponentRight, 12);
        HP_BAR(playerRight, damage: 25);
        HP_BAR(opponentRight, damage: 12);
    } THEN {
        EXPECT_EQ(playerLeft->hp, 0);
        EXPECT_EQ(opponentLeft->hp, 112);
        EXPECT_EQ(playerRight->hp, 87);
        EXPECT_EQ(opponentRight->hp, 100);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: after a switch the newcomer's Leftovers heals")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WYNAUT) { MaxHP(160); HP(100); Speed(100); Item(ITEM_LEFTOVERS); }
        FOE
    } WHEN {
        TURN { SWITCH(player, 1); }
    } SCENE {
        ITEM_HEAL(player, 10);
    } THEN {
        EXPECT_EQ(player->hp, 110);
    }
}

SINGLE_BATTLE_TEST("GC V18 reference: Magic Guard stops Black Sludge's damage, not its heal")
{
    u32 species, after;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; after = 160; }
    PARAMETRIZE { species = SPECIES_EKANS; after = 110; }
    GIVEN {
        PLAYER(species) { Ability(ABILITY_MAGIC_GUARD); MaxHP(160); HP(species == SPECIES_EKANS ? 100 : 160);
                          Speed(100); Item(ITEM_BLACK_SLUDGE); }
        FOE
    } WHEN {
        TURN {}
    } THEN {
        EXPECT_EQ(player->hp, after);
    }
}
