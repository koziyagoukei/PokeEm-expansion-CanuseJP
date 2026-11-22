#include "global.h"
#include "test/battle.h"

// GameCube battle request V19 (gba-gc-extension docs/items.md section 12):
// consumable held items. The GC's Oran / Sitrus Berry, the status-cure
// berries and Focus Sash must act exactly here, in this order, and be gone
// afterwards. The GC core's native test test_v19_reference has the same cases
// (same order of events). No MESSAGE checks: the text of this build is
// Japanese; a berry shows as B_ANIM_HELD_ITEM_BERRY, Focus Sash as
// B_ANIM_HANGED_ON.

ASSUMPTIONS
{
    ASSUME(GetItemHoldEffect(ITEM_ORAN_BERRY) == HOLD_EFFECT_RESTORE_HP);
    ASSUME(GetItemHoldEffectParam(ITEM_ORAN_BERRY) == 10);
    ASSUME(GetItemHoldEffect(ITEM_SITRUS_BERRY) == HOLD_EFFECT_RESTORE_PCT_HP);
    ASSUME(GetItemHoldEffectParam(ITEM_SITRUS_BERRY) == 25);
    ASSUME(GetItemHoldEffect(ITEM_CHERI_BERRY) == HOLD_EFFECT_CURE_PAR);
    ASSUME(GetItemHoldEffect(ITEM_CHESTO_BERRY) == HOLD_EFFECT_CURE_SLP);
    ASSUME(GetItemHoldEffect(ITEM_PECHA_BERRY) == HOLD_EFFECT_CURE_PSN);
    ASSUME(GetItemHoldEffect(ITEM_RAWST_BERRY) == HOLD_EFFECT_CURE_BRN);
    ASSUME(GetItemHoldEffect(ITEM_ASPEAR_BERRY) == HOLD_EFFECT_CURE_FRZ);
    ASSUME(GetItemHoldEffect(ITEM_PERSIM_BERRY) == HOLD_EFFECT_CURE_CONFUSION);
    ASSUME(GetItemHoldEffect(ITEM_LUM_BERRY) == HOLD_EFFECT_CURE_STATUS);
    ASSUME(GetItemHoldEffect(ITEM_FOCUS_SASH) == HOLD_EFFECT_FOCUS_SASH);
}

#define BERRY(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, battler)
#define SASH(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HANGED_ON, battler)

// ------------------------------------------------------------ HP berries

SINGLE_BATTLE_TEST("GC V19 reference: Sitrus Berry at half HP or less heals max HP / 4, at least 1")
{
    u32 maxHP, hp, healed;
    PARAMETRIZE { maxHP = 200; hp = 125; healed = 50; }  // poison 25: 100 <= 200 / 2
    PARAMETRIZE { maxHP = 200; hp = 126; healed = 0; }   // 101: not yet
    PARAMETRIZE { maxHP = 201; hp = 125; healed = 50; }  // poison 25: 100 <= 100; 201 / 4 = 50
    PARAMETRIZE { maxHP = 3; hp = 2; healed = 1; }       // poison 1 (at least 1): 1 <= 1; 3 / 4 = 0 -> 1
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(maxHP); HP(hp); Status1(STATUS1_POISON); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player);
        if (healed)
        {
            BERRY(player);
            HP_BAR(player, damage: -(s32)healed);
        }
        else
        {
            NOT BERRY(player);
        }
    } THEN {
        EXPECT_EQ(player->item, healed ? ITEM_NONE : ITEM_SITRUS_BERRY);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Oran Berry heals 10, not above max HP")
{
    u32 maxHP, hp, after;
    PARAMETRIZE { maxHP = 30; hp = 18; after = 25; }  // poison 3: 15 <= 15, +10
    PARAMETRIZE { maxHP = 16; hp = 10; after = 16; }  // poison 2: 8 <= 8, +10 stops at 16
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(maxHP); HP(hp); Status1(STATUS1_POISON); Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player);
        BERRY(player);
        HP_BAR(player);
    } THEN {
        EXPECT_EQ(player->hp, after);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a berry is eaten once")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(125); Status1(STATUS1_POISON); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
        TURN {}
        TURN {}
    } SCENE {
        HP_BAR(player);
        BERRY(player);
        HP_BAR(player, damage: -50);
        HP_BAR(player);
        HP_BAR(player);
        NOT BERRY(player);
    } THEN {
        EXPECT_EQ(player->hp, 100);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: sandstorm damage, then the holder's berry, then the next Pokemon's damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(90); Speed(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SANDSTORM, opponent);
        HP_BAR(player, damage: 10);
        BERRY(player);
        HP_BAR(player, damage: -40);
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: poison damage, then the berry, before the next Pokemon's poison")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(125); Speed(50); Status1(STATUS1_POISON); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(125); Speed(100); Status1(STATUS1_POISON); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(opponent, damage: 25);
        BERRY(opponent);
        HP_BAR(opponent, damage: -50);
        HP_BAR(player, damage: 25);
        BERRY(player);
        HP_BAR(player, damage: -50);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: after a hit, the attacker's berry after its recoil comes before the target's")
{
    GIVEN {
        ASSUME(GetMoveRecoil(MOVE_TAKE_DOWN) == 25);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(40); HP(22); Speed(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(50); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_TAKE_DOWN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TAKE_DOWN, player);
        HP_BAR(opponent);
        HP_BAR(player);
        BERRY(player);
        HP_BAR(player, damage: -10);
        BERRY(opponent);
        HP_BAR(opponent, damage: -50);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a multi-hit move: the berry between the hits")
{
    GIVEN {
        ASSUME(GetMoveStrikeCount(MOVE_DOUBLE_KICK) == 2);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(50); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_KICK, player);
        HP_BAR(opponent);
        BERRY(opponent);
        HP_BAR(opponent, damage: -50);
        HP_BAR(opponent);
    }
}

DOUBLE_BATTLE_TEST("GC V19 reference: a spread move: the targets' berries after both hits, in battler order")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_ROCK_SLIDE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(90); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(50); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(40); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROCK_SLIDE, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
        BERRY(opponentLeft);
        HP_BAR(opponentLeft, damage: -50);
        BERRY(opponentRight);
        HP_BAR(opponentRight, damage: -50);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a draining move: the target's berry after the user's drain")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_GIGA_DRAIN) == EFFECT_HIT);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(50); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(50); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_GIGA_DRAIN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GIGA_DRAIN, player);
        HP_BAR(opponent);
        HP_BAR(player);
        BERRY(opponent);
        HP_BAR(opponent, damage: -50);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a confusion self-hit, then the berry, before the next action")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_CONFUSE_RAY) == EFFECT_CONFUSE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(105); Speed(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
        TURN { MOVE(player, MOVE_TACKLE, WITH_RNG(RNG_CONFUSION, TRUE)); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CONFUSE_RAY, opponent);
        HP_BAR(player);
        BERRY(player);
        HP_BAR(player, damage: -50);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    }
}

// The berry acts behind the new substitute, so its animation is not shown.
SINGLE_BATTLE_TEST("GC V19 reference: Substitute's HP cost, then the user's berry, before the next action")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(140); Speed(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_SUBSTITUTE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, player);
        HP_BAR(player, damage: 50);
        HP_BAR(player, damage: -50);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    } THEN {
        EXPECT_EQ(player->hp, 140);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Rough Skin damage, then the attacker's berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(85); Speed(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, damage: 20);
        BERRY(player);
        HP_BAR(player, damage: -40);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a Pokemon switched in at half HP eats its berry on entry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { MaxHP(200); HP(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
    } SCENE {
        BERRY(player);
        HP_BAR(player, damage: -50);
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: battle start: each lead's entry ability and its berry, in speed order")
{
    u32 playerSpeed;
    PARAMETRIZE { playerSpeed = 100; }
    PARAMETRIZE { playerSpeed = 10; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(100); Speed(playerSpeed); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_GYARADOS) { Ability(ABILITY_INTIMIDATE); Speed(50); }
    } WHEN {
        TURN {}
    } SCENE {
        if (playerSpeed > 50)
        {
            BERRY(player);
            ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
        }
        else
        {
            ABILITY_POPUP(opponent, ABILITY_INTIMIDATE);
            BERRY(player);
        }
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a berry eaten before a switch is not back after it")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(125); Status1(STATUS1_POISON); Item(ITEM_SITRUS_BERRY); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } SCENE {
        BERRY(player);
        NOT BERRY(player);
    } THEN {
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

// ------------------------------------------------------- status berries

SINGLE_BATTLE_TEST("GC V19 reference: a status berry cures the status a move gives, at once")
{
    enum Move move;
    enum Item item;
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; item = ITEM_CHERI_BERRY; }
    PARAMETRIZE { move = MOVE_THUNDER_WAVE; item = ITEM_LUM_BERRY; }
    PARAMETRIZE { move = MOVE_HYPNOSIS; item = ITEM_CHESTO_BERRY; }
    PARAMETRIZE { move = MOVE_TOXIC; item = ITEM_PECHA_BERRY; }
    PARAMETRIZE { move = MOVE_WILL_O_WISP; item = ITEM_RAWST_BERRY; }
    PARAMETRIZE { move = MOVE_WILL_O_WISP; item = ITEM_LUM_BERRY; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Item(item); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        BERRY(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    } THEN {
        EXPECT_EQ(opponent->status1, STATUS1_NONE);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Aspear Berry thaws a freeze from a hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Item(ITEM_ASPEAR_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_BEAM, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ICE_BEAM, player);
        HP_BAR(opponent);
        STATUS_ICON(opponent, freeze: TRUE);
        BERRY(opponent);
        STATUS_ICON(opponent, freeze: FALSE);
    } THEN {
        EXPECT_EQ(opponent->status1, STATUS1_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a status from a hit: damage, status, berry")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_BODY_SLAM, MOVE_EFFECT_PARALYSIS));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Item(ITEM_CHERI_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_BODY_SLAM, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BODY_SLAM, player);
        HP_BAR(opponent);
        STATUS_ICON(opponent, paralysis: TRUE);
        BERRY(opponent);
        STATUS_ICON(opponent, paralysis: FALSE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Synchronize: the status given back is cured first, then the target's")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_CHERI_BERRY); }
        OPPONENT(SPECIES_ABRA) { Ability(ABILITY_SYNCHRONIZE); Speed(50); Item(ITEM_LUM_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_WAVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_THUNDER_WAVE, player);
        STATUS_ICON(opponent, paralysis: TRUE);
        ABILITY_POPUP(opponent, ABILITY_SYNCHRONIZE);
        STATUS_ICON(player, paralysis: TRUE);
        BERRY(player);
        STATUS_ICON(player, paralysis: FALSE);
        BERRY(opponent);
        STATUS_ICON(opponent, paralysis: FALSE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Static paralyses the attacker, its berry cures it at once")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Item(ITEM_CHERI_BERRY); }
        OPPONENT(SPECIES_PIKACHU) { Ability(ABILITY_STATIC); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE, WITH_RNG(RNG_STATIC, TRUE)); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_STATIC);
        STATUS_ICON(player, paralysis: TRUE);
        BERRY(player);
        STATUS_ICON(player, paralysis: FALSE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: confusion is cured at the end of the move (Confuse Ray, Swagger)")
{
    enum Move move;
    enum Item item;
    PARAMETRIZE { move = MOVE_CONFUSE_RAY; item = ITEM_PERSIM_BERRY; }
    PARAMETRIZE { move = MOVE_CONFUSE_RAY; item = ITEM_LUM_BERRY; }
    PARAMETRIZE { move = MOVE_SWAGGER; item = ITEM_LUM_BERRY; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Item(item); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, player);
        if (move == MOVE_SWAGGER)
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STATS_CHANGE, opponent);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_CONFUSION, opponent);
        BERRY(opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    } THEN {
        EXPECT(opponent->volatiles.confusionTimer == 0);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a status berry cures a status the Pokemon enters with")
{
    u32 status;
    enum Item item;
    PARAMETRIZE { status = STATUS1_POISON; item = ITEM_PECHA_BERRY; }
    PARAMETRIZE { status = STATUS1_TOXIC_POISON; item = ITEM_LUM_BERRY; }
    PARAMETRIZE { status = STATUS1_SLEEP; item = ITEM_CHESTO_BERRY; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WYNAUT) { Status1(status); Item(item); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SWITCH(player, 1); }
    } SCENE {
        BERRY(player);
    } THEN {
        EXPECT_EQ(player->status1, STATUS1_NONE);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a berry used up does not cure the next status")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Item(ITEM_CHERI_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_WAVE); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_PARALYSIS, FALSE)); }
        TURN { MOVE(player, MOVE_THUNDER_WAVE); MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_PARALYSIS, FALSE)); }
    } SCENE {
        BERRY(opponent);
        NOT BERRY(opponent);
    } THEN {
        EXPECT(opponent->status1 & STATUS1_PARALYSIS);
    }
}

// ------------------------------------------------------------ Focus Sash

SINGLE_BATTLE_TEST("GC V19 reference: Focus Sash at full HP leaves 1 HP and is used up; not below full HP")
{
    u32 hp;
    PARAMETRIZE { hp = 20; }
    PARAMETRIZE { hp = 19; }
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(20); HP(hp); Speed(50); Item(ITEM_FOCUS_SASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_CLOSE_COMBAT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLOSE_COMBAT, player);
        HP_BAR(opponent);
        if (hp == 20)
            SASH(opponent);
        else
            NOT SASH(opponent);
    } THEN {
        EXPECT_EQ(opponent->hp, hp == 20 ? 1 : 0);
        if (hp == 20)
            EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Focus Sash holds once in a multi-hit move")
{
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(20); HP(20); Speed(50); Item(ITEM_FOCUS_SASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_KICK, player);
        HP_BAR(opponent);
        SASH(opponent);
        HP_BAR(opponent);
    } THEN {
        EXPECT_EQ(opponent->hp, 0);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Sturdy holds first; Focus Sash is kept")
{
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); Speed(100); }
        OPPONENT(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); MaxHP(20); HP(20); Speed(50); Item(ITEM_FOCUS_SASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_CLOSE_COMBAT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CLOSE_COMBAT, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_STURDY);
        NOT SASH(opponent);
    } THEN {
        EXPECT_EQ(opponent->hp, 1);
        EXPECT_EQ(opponent->item, ITEM_FOCUS_SASH);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Focus Sash does nothing against poison damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(1); HP(1); Status1(STATUS1_POISON); Item(ITEM_FOCUS_SASH); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN {}
    } SCENE {
        HP_BAR(player);
        NOT SASH(player);
    } THEN {
        EXPECT_EQ(player->hp, 0);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Focus Sash holds against a confusion self-hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(2); HP(2); Speed(100); Item(ITEM_FOCUS_SASH); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CONFUSE_RAY); }
        TURN { MOVE(player, MOVE_CELEBRATE, WITH_RNG(RNG_CONFUSION, TRUE)); }
    } SCENE {
        HP_BAR(player);
        SASH(player);
    } THEN {
        EXPECT_EQ(player->hp, 1);
        EXPECT_EQ(player->item, ITEM_NONE);
    }
}

DOUBLE_BATTLE_TEST("GC V19 reference: a spread move: every target is hit, then Focus Sash")
{
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(90); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(20); HP(20); Speed(50); Item(ITEM_FOCUS_SASH); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(20); HP(20); Speed(40); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROCK_SLIDE, WITH_RNG(RNG_SECONDARY_EFFECT, FALSE)); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
        SASH(opponentLeft);
    } THEN {
        EXPECT_EQ(opponentLeft->hp, 1);
        EXPECT_EQ(opponentRight->hp, 0);
    }
}

DOUBLE_BATTLE_TEST("GC V19 reference: a spread move: every target is hit, then Sturdy")
{
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(90); }
        OPPONENT(SPECIES_GEODUDE) { Ability(ABILITY_STURDY); MaxHP(20); HP(20); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(20); HP(20); Speed(40); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE); }
    } SCENE {
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
        ABILITY_POPUP(opponentLeft, ABILITY_STURDY);
    }
}

DOUBLE_BATTLE_TEST("GC V19 reference: a spread move: every target is hit, then each one's status and berry")
{
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_DISCHARGE, MOVE_EFFECT_PARALYSIS));
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(90); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); Item(ITEM_CHERI_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); Item(ITEM_CHERI_BERRY); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_DISCHARGE, WITH_RNG(RNG_SECONDARY_EFFECT, TRUE)); }
    } SCENE {
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
        STATUS_ICON(opponentLeft, paralysis: TRUE);
        BERRY(opponentLeft);
        STATUS_ICON(opponentRight, paralysis: TRUE);
        BERRY(opponentRight);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Struggle's recoil, then the user's berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(110); Speed(100); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(opponent);
        HP_BAR(player, damage: 50);
        BERRY(player);
        HP_BAR(player, damage: -50);
    }
}

// Solar Power's and Dry Skin's sun damage checks no item (unlike poison, burn
// and sandstorm): the berry waits for the next check, here the end of the
// foe's move on the next turn (every Pokemon is checked there).
SINGLE_BATTLE_TEST("GC V19 reference: Solar Power or Dry Skin sun damage: the berry at the next move's end")
{
    enum Ability ability;
    enum Species species;
    PARAMETRIZE { species = SPECIES_CHARIZARD; ability = ABILITY_SOLAR_POWER; }
    PARAMETRIZE { species = SPECIES_TOXICROAK; ability = ABILITY_DRY_SKIN; }
    GIVEN {
        PLAYER(species) { Ability(ability); MaxHP(160); HP(90); Speed(50); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        ABILITY_POPUP(player, ability);
        HP_BAR(player, damage: 20);
        NOT BERRY(player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        BERRY(player);
        HP_BAR(player, damage: -40);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a pivot move: the target's berry before the user leaves")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(50); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_U_TURN); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, player);
        HP_BAR(opponent);
        BERRY(opponent);
        HP_BAR(opponent, damage: -50);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(opponent->item, ITEM_NONE);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: Dragon Tail drags the target out before its berry")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_DRAGON_TAIL) == EFFECT_HIT_SWITCH_TARGET);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(200); HP(101); Speed(50); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAGON_TAIL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_TAIL, player);
        HP_BAR(opponent);
        NOT BERRY(opponent);
    } THEN {
        EXPECT_EQ(opponent->species, SPECIES_WYNAUT);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: a fully paralysed Pokemon's turn still ends with the item checks")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_SOLAR_POWER); MaxHP(160); HP(90); Speed(50); Item(ITEM_SITRUS_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); Status1(STATUS1_PARALYSIS); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUNNY_DAY, WITH_RNG(RNG_PARALYSIS, FALSE)); MOVE(player, MOVE_CELEBRATE); }
        TURN { MOVE(opponent, MOVE_CELEBRATE, WITH_RNG(RNG_PARALYSIS, TRUE)); MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUNNY_DAY, opponent);
        HP_BAR(player, damage: 20);
        NOT BERRY(player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PRZ, opponent);
        BERRY(player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    }
}
