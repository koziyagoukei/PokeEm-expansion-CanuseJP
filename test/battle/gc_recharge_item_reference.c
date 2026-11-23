#include "global.h"
#include "test/battle.h"

// GameCube battle request V19, ACT-002 (gba-gc-extension docs/battle-reference,
// docs/items.md section 12): the turn a Pokemon must recharge after Hyper Beam.
// CancelerRecharge sends it to BattleScript_MoveUsedMustRecharge, which ends
// with moveendall: the move end's item checks run all the same (the user's,
// every other Pokemon's HP, then everyone's), before the next action, and the
// recharge costs no PP. The GC core's native test test_v19_recharge has the
// same cases. A berry shows as B_ANIM_HELD_ITEM_BERRY; this build's text is
// Japanese (see the probe below for the recharge message).

ASSUMPTIONS
{
    ASSUME(MoveHasAdditionalEffectSelf(MOVE_HYPER_BEAM, MOVE_EFFECT_RECHARGE) == TRUE);
    ASSUME(GetItemHoldEffect(ITEM_SITRUS_BERRY) == HOLD_EFFECT_RESTORE_PCT_HP);
    ASSUME(GetItemHoldEffectParam(ITEM_SITRUS_BERRY) == 25);
}

#define BERRY(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, battler)

// Codex's case: Solar Power's sun damage checks no item (it waits for the next
// move end), so the berry is due when the Pokemon recharges.
SINGLE_BATTLE_TEST("GC V19 reference: recharge: the move end's berry after the recharge, before the next action")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(100); Item(ITEM_SITRUS_BERRY);
                                    Moves(MOVE_HYPER_BEAM, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_BEAM); MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { SKIP_TURN(player); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        ABILITY_POPUP(player, ABILITY_SOLAR_POWER);
        HP_BAR(player, damage: 20);                 // 90 -> 70: <= 80, but no check here
        NONE_OF {
            BERRY(player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, player);
        }
        MESSAGE("{JPN}攻撃の 反動で\n{JPN}リザードンは 動けない!");
        BERRY(player);
        HP_BAR(player, damage: -40);                // 70 -> 110
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        ABILITY_POPUP(player, ABILITY_SOLAR_POWER);
        HP_BAR(player, damage: 20);                 // the turn's end: 110 -> 90
    } THEN {
        EXPECT_EQ(player->hp, 90);
        EXPECT_EQ(player->item, ITEM_NONE);
        EXPECT_EQ(player->pp[0], GetMovePP(MOVE_HYPER_BEAM) - 1);  // the recharge costs no PP
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: recharge: no berry, or Unnerve: nothing at the move end")
{
    enum Species foe;
    enum Ability foeAbility;
    u16 item;
    PARAMETRIZE { item = ITEM_NONE; foe = SPECIES_WOBBUFFET; foeAbility = ABILITY_SHADOW_TAG; }
    PARAMETRIZE { item = ITEM_SITRUS_BERRY; foe = SPECIES_MEWTWO; foeAbility = ABILITY_UNNERVE; }
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(100); Item(item);
                                    Moves(MOVE_HYPER_BEAM, MOVE_CELEBRATE); }
        OPPONENT(foe) { Ability(foeAbility); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_BEAM); MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { SKIP_TURN(player); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        HP_BAR(player, damage: 20);
        NOT BERRY(player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        HP_BAR(player, damage: 20);
    } THEN {
        EXPECT_EQ(player->hp, 50);
        EXPECT_EQ(player->item, item);
    }
}

// The recharging Pokemon's own berry first (the attacker's check), then every
// other Pokemon's HP in battler order (player left, opponent left, player
// right, opponent right), before the next action.
DOUBLE_BATTLE_TEST("GC V19 reference: recharge: its own berry first, then an ally's and a foe's in battler order, before the next action")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(100); Item(ITEM_SITRUS_BERRY);
                                    Moves(MOVE_HYPER_BEAM, MOVE_CELEBRATE); }
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(50); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(80); }
        OPPONENT(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(40); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_BEAM, target: opponentLeft); MOVE(opponentLeft, MOVE_SUNNY_DAY);
               MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
        TURN { SKIP_TURN(playerLeft); MOVE(opponentLeft, MOVE_CELEBRATE);
               MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, playerLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponentLeft);
        HP_BAR(playerLeft, damage: 20);
        HP_BAR(playerRight, damage: 20);
        HP_BAR(opponentRight, damage: 20);
        BERRY(playerLeft);                          // MOVEEND_ITEM_EFFECTS_ATTACKER_1
        HP_BAR(playerLeft, damage: -40);
        BERRY(playerRight);                         // MOVEEND_KEE_MARANGA_HP_THRESHOLD_ITEM_TARGET
        HP_BAR(playerRight, damage: -40);
        BERRY(opponentRight);
        HP_BAR(opponentRight, damage: -40);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentLeft);
    } THEN {
        EXPECT_EQ(playerLeft->pp[0], GetMovePP(MOVE_HYPER_BEAM) - 1);
    }
}

// A Pokemon knocked out before its recharge turn: the action is skipped, no
// recharge and no move end of its own; its berry stays.
SINGLE_BATTLE_TEST("GC V19 reference: recharge: knocked out before recharging: skipped, the berry stays")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(16); HP(9); Speed(50); Item(ITEM_SITRUS_BERRY);
                                    Moves(MOVE_HYPER_BEAM, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); MOVE(player, MOVE_HYPER_BEAM); }
        TURN { MOVE(opponent, MOVE_TACKLE); SKIP_TURN(player); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, player);
        HP_BAR(player, damage: 2);                  // 9 -> 7: <= 8, due at the next move end
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        NOT BERRY(player);
        HP_BAR(player, hp: 0);
    } THEN {
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM), ITEM_SITRUS_BERRY);
    }
}

// Compare: a sleeping Pokemon's lost turn (BattleScript_MoveUsedIsAsleep ->
// moveendall) gives the same order.
SINGLE_BATTLE_TEST("GC V19 reference: asleep: the move end's berry after the lost turn, before the next action")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(100); Item(ITEM_SITRUS_BERRY);
                                    Status1(STATUS1_SLEEP_TURN(3)); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_SUNNY_DAY); }
        TURN { MOVE(player, MOVE_CELEBRATE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_SLP, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        HP_BAR(player, damage: 20);
        NOT BERRY(player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_SLP, player);
        BERRY(player);
        HP_BAR(player, damage: -40);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    }
}

// recharge.c's first test without its English MESSAGE (this build's text is
// Japanese): exactly one turn lost.
SINGLE_BATTLE_TEST("GC V19 reference: recharge: exactly one turn lost (recharge.c without the English text)")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_BEAM); }
        TURN { SKIP_TURN(player); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, player);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_HYPER_BEAM, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
    }
}
