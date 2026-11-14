#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"

SINGLE_BATTLE_TEST("Gimmick damage: generic Z-Moves inherit power and category", s16 damage, u16 power)
{
    enum Move baseMove;
    enum Gimmick dynamax;
    PARAMETRIZE { baseMove = MOVE_DARK_PULSE; dynamax = GIMMICK_NONE; }
    PARAMETRIZE { baseMove = MOVE_CRUNCH; dynamax = GIMMICK_NONE; }
    PARAMETRIZE { baseMove = MOVE_DARK_PULSE; dynamax = GIMMICK_DYNAMAX; }
    PARAMETRIZE { baseMove = MOVE_CRUNCH; dynamax = GIMMICK_DYNAMAX; }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_CHI_YU) {
            Level(50); HP(131); MaxHP(131); Attack(76); SpAttack(187); Speed(167);
            Item(ITEM_DARKINIUM_Z); Ability(ABILITY_BEADS_OF_RUIN);
        }
        OPPONENT(SPECIES_URSALUNA) {
            Level(50); HP(237); MaxHP(237); Attack(20); Defense(125); SpDefense(101); Speed(190);
            Ability(ABILITY_GUTS); DynamaxLevel(10);
        }
    } WHEN {
        TURN {
            MOVE(player, baseMove, gimmick: GIMMICK_Z_MOVE);
            MOVE(opponent, MOVE_FACADE, gimmick: dynamax);
        }
    } SCENE {
        if (dynamax)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DYNAMAX_GROWTH, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BLACK_HOLE_ECLIPSE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        results[i].power = gBattleMovePower;
    } FINALLY {
        EXPECT_GT(results[0].damage, results[1].damage * 2);
        EXPECT_EQ(results[2].damage, results[0].damage);
        EXPECT_EQ(results[3].damage, results[1].damage);
        for (u32 j = 0; j < 4; j++)
            EXPECT_EQ(results[j].power, 160);
    }
}

SINGLE_BATTLE_TEST("Gimmick damage: Catastropika uses its signature power", s16 damage, u16 power)
{
    enum Item crystal;
    PARAMETRIZE { crystal = ITEM_ELECTRIUM_Z; }
    PARAMETRIZE { crystal = ITEM_PIKANIUM_Z; }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_PIKACHU) { Level(50); Attack(100); Item(crystal); Ability(ABILITY_STATIC); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); HP(500); MaxHP(500); Defense(100); Ability(ABILITY_IMMUNITY); }
    } WHEN {
        TURN { MOVE(player, MOVE_VOLT_TACKLE, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        if (crystal == ITEM_PIKANIUM_Z)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_CATASTROPIKA, player);
        else
            ANIMATION(ANIM_TYPE_MOVE, MOVE_GIGAVOLT_HAVOC, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        results[i].power = gBattleMovePower;
    } FINALLY {
        EXPECT_GT(results[1].damage, results[0].damage);
        EXPECT_EQ(results[0].power, 190);
        EXPECT_EQ(results[1].power, 210);
    }
}



SINGLE_BATTLE_TEST("Gimmick damage: Z-Sleep Talk inherits the called move's power and category", s16 damage, u16 power)
{
    bool32 sleepTalk;
    PARAMETRIZE { sleepTalk = FALSE; }
    PARAMETRIZE { sleepTalk = TRUE; }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        if (sleepTalk)
            PLAYER(SPECIES_CHI_YU) {
                Level(50); Attack(76); SpAttack(187); Status1(STATUS1_SLEEP);
                Item(ITEM_NORMALIUM_Z); Moves(MOVE_SLEEP_TALK, MOVE_DARK_PULSE);
            }
        else
            PLAYER(SPECIES_CHI_YU) {
                Level(50); Attack(76); SpAttack(187);
                Item(ITEM_DARKINIUM_Z); Moves(MOVE_DARK_PULSE);
            }
        OPPONENT(SPECIES_URSALUNA) {
            Level(50); HP(237); MaxHP(237); Defense(125); SpDefense(101);
        }
    } WHEN {
        if (sleepTalk)
            TURN { MOVE(player, MOVE_SLEEP_TALK, gimmick: GIMMICK_Z_MOVE); }
        else
            TURN { MOVE(player, MOVE_DARK_PULSE, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BLACK_HOLE_ECLIPSE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        results[i].power = gBattleMovePower;
    } FINALLY {
        EXPECT_EQ(results[1].damage, results[0].damage);
        EXPECT_EQ(results[0].power, 160);
        EXPECT_EQ(results[1].power, 160);
    }
}

SINGLE_BATTLE_TEST("Gimmick damage: AI predicts Max Wyrmwind when using Dragon Rage")
{
    s16 damage;
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); HP(500); MaxHP(500); Defense(100); SpDefense(100); Ability(ABILITY_IMMUNITY); }
        OPPONENT(SPECIES_DRAGONITE) { Level(50); Attack(100); SpAttack(200); Ability(ABILITY_INNER_FOCUS); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE, gimmick: GIMMICK_DYNAMAX, WITH_RNG(RNG_DAMAGE_MODIFIER, 0)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_WYRMWIND, opponent);
        HP_BAR(player, captureDamage: &damage);
    } THEN {
        enum BattlerId atk = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        enum BattlerId def = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        struct AiCalcValues calc = {.move = MOVE_DRAGON_RAGE, .gimmickAtk = GIMMICK_DYNAMAX};
        struct SimulatedDamage predicted = AI_CalcDamage(&calc, atk, def);
        EXPECT_GT(predicted.minimum, 40);
        EXPECT_EQ(predicted.maximum, damage);
    }
}

SINGLE_BATTLE_TEST("Gimmick damage: Struggle retains its power during Dynamax", u16 power, s16 damage)
{
    enum Gimmick gimmick;
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Attack(100); SpAttack(100); Speed(100); }
        OPPONENT(SPECIES_SNORLAX) { HP(1000); MaxHP(1000); Defense(100); SpDefense(100); Speed(20); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, gimmick: gimmick); }
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, gimmick == GIMMICK_DYNAMAX ? MOVE_MAX_STRIKE : MOVE_SCRATCH, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } THEN {
        results[i].power = gBattleMovePower;
    } FINALLY {
        EXPECT_EQ(results[0].power, 50);
        EXPECT_EQ(results[1].power, 50);
        EXPECT_EQ(results[1].damage, results[0].damage);
    }
}
