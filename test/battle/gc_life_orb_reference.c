#include "global.h"
#include "test/battle.h"

// M4-B1c-0a (gba-gc-extension docs/items.md section 13): Life Orb in this
// build, for the GC core's later request V20. No MESSAGE checks (the text is
// Japanese): HP bars, damage captures, the item pop-up and animations carry
// the order.
//
// Damage: GetAttackerItemsModifier gives UQ_4_12_FLOORED(1.3) = 5324, one
// factor of GetOtherModifiers' chain (the attacker's abilities, the
// defender's, its partner's, the attacker's item, the defender's item; the
// slower attacker's own ability and item after the defender's), applied once
// with uq4_12_multiply_by_int_half_down after STAB, type and burn; then the
// damage minimum of 1. HP: TryLifeOrb at MOVEEND_LIFE_ORB_SHELL_BELL, max HP
// / 10 (at least 1, SetPassiveDamageAmount) when the move was used, a target
// was damaged (substitutes count) or damage was dealt, the holder is alive
// and has no Magic Guard.

// Life Orb on a damage d that has no other final factor.
#define LO(d) (((d) * 5324 + 2047) / 4096)
// What the nearest 4.12 factor (5325) would give: some rolls differ.
#define LO_NEAREST(d) (((d) * 5325 + 2047) / 4096)

#define LO_ROLLS                                                          \
    PARAMETRIZE { item = ITEM_NONE; roll = 0; }                           \
    PARAMETRIZE { item = ITEM_LIFE_ORB; roll = 0; }                       \
    PARAMETRIZE { item = ITEM_NONE; roll = 7; }                           \
    PARAMETRIZE { item = ITEM_LIFE_ORB; roll = 7; }                       \
    PARAMETRIZE { item = ITEM_NONE; roll = 15; }                          \
    PARAMETRIZE { item = ITEM_LIFE_ORB; roll = 15; }

// Each pair (no item, Life Orb), printed for the GC's native comparison.
#define EXPECT_LO_PAIRS()                                                                     \
    for (u32 k = 0; k < 6; k += 2)                                                            \
    {                                                                                         \
        Test_MgbaPrintf("LO_PAIR %d none=%d lo=%d", k / 2, results[k].damage, results[k + 1].damage); \
        EXPECT_EQ(results[k + 1].damage, LO(results[k].damage));                              \
    }

ASSUMPTIONS
{
    ASSUME(GetItemHoldEffect(ITEM_LIFE_ORB) == HOLD_EFFECT_LIFE_ORB);
}

// ---------------------------------------------------------------- damage

SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: physical hit x5324/4096, three rolls", s16 damage)
{
    enum Item item; u32 roll;
    LO_ROLLS
    GIVEN {
        ASSUME(GetMovePower(MOVE_STRENGTH) == 80 && GetMoveCategory(MOVE_STRENGTH) == DAMAGE_CATEGORY_PHYSICAL);
        PLAYER(SPECIES_SNORLAX) { Level(50); Attack(100); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LO_PAIRS();
    }
}

SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: special hit x5324/4096, three rolls", s16 damage)
{
    enum Item item; u32 roll;
    LO_ROLLS
    GIVEN {
        ASSUME(GetMovePower(MOVE_HYPER_VOICE) == 90 && GetMoveCategory(MOVE_HYPER_VOICE) == DAMAGE_CATEGORY_SPECIAL);
        PLAYER(SPECIES_SNORLAX) { Level(50); SpAttack(100); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_VOICE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LO_PAIRS();
    }
}

// The 4.12 factor is truncated: a strong hit where 5325 would round one higher.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: the truncated factor, not the nearest", s16 damage)
{
    enum Item item; u32 roll;
    LO_ROLLS
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(100); Attack(400); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(100); Defense(60); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        u32 differ = 0;
        EXPECT_LO_PAIRS();
        for (u32 k = 0; k < 6; k += 2)
        {
            Test_MgbaPrintf("LO_TRUNC roll=%d none=%d lo=%d nearest=%d", k / 2, results[k].damage, results[k + 1].damage,
                            LO_NEAREST(results[k].damage));
            differ += results[k + 1].damage != LO_NEAREST(results[k].damage);
        }
        EXPECT_GT(differ, 0);
    }
}

// Super effective, not very effective and a critical hit: the factor is the
// same (unlike Expert Belt) and comes after the type and the critical hit.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: the same factor at x4, x2 and on a critical hit", s16 damage)
{
    enum Item item; enum Species foe; bool32 crit;
    PARAMETRIZE { item = ITEM_NONE;     foe = SPECIES_GEODUDE; crit = FALSE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; foe = SPECIES_GEODUDE; crit = FALSE; }
    PARAMETRIZE { item = ITEM_NONE;     foe = SPECIES_CHARMANDER; crit = FALSE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; foe = SPECIES_CHARMANDER; crit = FALSE; }
    PARAMETRIZE { item = ITEM_NONE;     foe = SPECIES_SNORLAX; crit = TRUE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; foe = SPECIES_SNORLAX; crit = TRUE; }
    GIVEN {
        ASSUME(GetMoveType(MOVE_WATER_GUN) == TYPE_WATER);
        PLAYER(SPECIES_WOBBUFFET) { Level(50); SpAttack(150); Speed(100); Item(item); }
        OPPONENT(foe) { Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_WATER_GUN, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: crit); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LO_PAIRS();
    }
}

// With a defender's final factor (Multiscale x0.5 at full HP): one chain,
// applied once; the attacker faster (its item after both abilities) or slower
// (its item last).
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: one final chain with Multiscale, faster and slower", s16 damage)
{
    enum Item item; enum Ability ability; u32 speed;
    PARAMETRIZE { item = ITEM_NONE;     ability = ABILITY_INNER_FOCUS; speed = 100; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; ability = ABILITY_MULTISCALE;  speed = 100; }
    PARAMETRIZE { item = ITEM_NONE;     ability = ABILITY_INNER_FOCUS; speed = 10; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; ability = ABILITY_MULTISCALE;  speed = 10; }
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); Attack(200); Speed(speed); Item(item); }
        OPPONENT(SPECIES_DRAGONITE) { Ability(ability); Level(50); Defense(100); MaxHP(999); HP(999); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // hd(5324, 2048) = 2662: the chain's factor, then once on the damage.
        Test_MgbaPrintf("LO_MULTISCALE fast none=%d lo=%d slow none=%d lo=%d", results[0].damage, results[1].damage, results[2].damage, results[3].damage);
        EXPECT_EQ(results[1].damage, (results[0].damage * 2662 + 2047) / 4096);
        EXPECT_EQ(results[3].damage, (results[2].damage * 2662 + 2047) / 4096);
    }
}

// A fixed-damage move: no boost, but the HP loss.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: fixed damage is not boosted, the HP is lost")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); MaxHP(200); HP(200); Speed(100); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: 50);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 20);
    }
}

// Giga Drain: the drain is half of the boosted damage; the Life Orb loss after it.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: a drain move heals from the boosted damage, then the loss", s16 damage)
{
    enum Item item;
    s16 heal;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; }
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_GIGA_DRAIN, MOVE_EFFECT_ABSORB));
        PLAYER(SPECIES_SNORLAX) { Level(50); SpAttack(150); MaxHP(300); HP(100); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_GIGA_DRAIN, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        HP_BAR(player, captureDamage: &heal);
        if (item == ITEM_LIFE_ORB)
        {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player, damage: 30);
        }
    } THEN {
        EXPECT_EQ(-heal, results[i].damage / 2);
    } FINALLY {
        Test_MgbaPrintf("LO_DRAIN %d %d", results[0].damage, results[1].damage);
        EXPECT_EQ(results[1].damage, LO(results[0].damage));
    }
}

// Double-Edge: the recoil (from the boosted damage) first, then the loss.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: recoil first, then the loss")
{
    s16 damage, recoil;
    GIVEN {
        ASSUME(GetMoveRecoil(MOVE_DOUBLE_EDGE) == 33);
        PLAYER(SPECIES_SNORLAX) { Level(50); Attack(100); MaxHP(300); HP(300); Speed(100); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_EDGE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &damage);
        HP_BAR(player, captureDamage: &recoil);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 30);
    } THEN {
        EXPECT_EQ(recoil, damage * 33 / 100);
    }
}

// A substitute takes the boosted hit; the holder still loses HP.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: a hit on a substitute is boosted and costs HP", u16 damage)
{
    enum Item item;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; }
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); Attack(60); MaxHP(300); HP(300); Speed(10); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(150); MaxHP(400); HP(400); Speed(30); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        SUB_HIT(opponent, captureDamage: &results[i].damage);
        if (item == ITEM_LIFE_ORB)
        {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player, damage: 30);
        }
    } FINALLY {
        Test_MgbaPrintf("LO_SUB %d %d", results[0].damage, results[1].damage);
        EXPECT_EQ(results[1].damage, LO(results[0].damage));
    }
}

// A spread move: each target's hit boosted after the spread factor; one loss for the move.
DOUBLE_BATTLE_TEST("GC V20 reference: Life Orb: a spread move, both targets boosted, one loss", s16 damage)
{
    enum Item item;
    s16 right;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; }
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_HYPER_VOICE) == TARGET_BOTH);
        PLAYER(SPECIES_SNORLAX) { Level(50); SpAttack(150); MaxHP(300); HP(300); Speed(100); Item(item); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(30); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); SpDefense(100); MaxHP(999); HP(999); Speed(25); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_HYPER_VOICE, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponentLeft, captureDamage: &results[i].damage);
        HP_BAR(opponentRight, captureDamage: &right);
        if (item == ITEM_LIFE_ORB)
        {
            ITEM_POPUP(playerLeft, ITEM_LIFE_ORB);
            HP_BAR(playerLeft, damage: 30);
        }
    } THEN {
        EXPECT_EQ(right, results[i].damage);
        EXPECT_EQ(playerLeft->hp, item == ITEM_LIFE_ORB ? 270 : 300);
    } FINALLY {
        Test_MgbaPrintf("LO_SPREAD %d %d", results[0].damage, results[1].damage);
        EXPECT_EQ(results[1].damage, LO(results[0].damage));
    }
}

// A two-hit move: each hit boosted; one loss after the last hit.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: a two-hit move, both hits boosted, one loss at the end", s16 damage)
{
    enum Item item;
    s16 second;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; }
    GIVEN {
        ASSUME(GetMoveStrikeCount(MOVE_DOUBLE_KICK) == 2);
        PLAYER(SPECIES_SNORLAX) { Level(50); Attack(150); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        HP_BAR(opponent, captureDamage: &second);
        if (item == ITEM_LIFE_ORB)
        {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player, damage: 30);
        }
    } THEN {
        EXPECT_EQ(second, results[i].damage);
        EXPECT_EQ(player->hp, item == ITEM_LIFE_ORB ? 270 : 300);
    } FINALLY {
        Test_MgbaPrintf("LO_TWOHIT %d %d", results[0].damage, results[1].damage);
        EXPECT_EQ(results[1].damage, LO(results[0].damage));
    }
}

// ---------------------------------------------------------------- HP loss

SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: the loss is max HP / 10, at least 1")
{
    u32 maxHP, loss;
    PARAMETRIZE { maxHP = 105; loss = 10; }
    PARAMETRIZE { maxHP = 109; loss = 10; }
    PARAMETRIZE { maxHP = 9;   loss = 1; }
    PARAMETRIZE { maxHP = 1;   loss = 1; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(maxHP); HP(maxHP); Speed(100); Item(ITEM_LIFE_ORB); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        if (maxHP == 1)
            TURN { MOVE(player, MOVE_TACKLE); SEND_OUT(player, 1); }
        else
            TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: loss);
    } THEN {
        if (maxHP != 1)
            EXPECT_EQ(player->hp, maxHP - loss);
    }
}

// HP 1: the holder faints from the loss after the hit; the foe still moves later.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: at 1 HP the holder faints after its hit")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(1); Speed(100); Item(ITEM_LIFE_ORB); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); MOVE(opponent, MOVE_CELEBRATE); SEND_OUT(player, 1); }
    } SCENE {
        HP_BAR(opponent);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, hp: 0);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
    }
}

// The holder knocks the target out and then faints from the loss: the target
// faints first (MOVEEND_FAINT_BLOCK), the holder at the Life Orb stage.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: a knock-out, then the holder's own faint")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(1); Speed(100); Item(ITEM_LIFE_ORB); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(100); HP(1); Speed(30); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent, hp: 0);
        MESSAGE("{JPN}{JPN}相手の {JPN}カビゴンは 倒れた！");
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, hp: 0);
        MESSAGE("{JPN}{JPN}ソーナンスは 倒れた！");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(opponent->species, SPECIES_WYNAUT);
    }
}

// No loss: a miss, a type immunity, an absorbing ability, Protect, a status move.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: no loss without a hit (miss, immune, absorbed, protected, status)")
{
    u32 kind;
    enum Species foe; enum Ability foeAbility; enum Move move, foeMove;
    PARAMETRIZE { kind = 0; foe = SPECIES_SNORLAX; foeAbility = ABILITY_THICK_FAT;   move = MOVE_SLAM;        foeMove = MOVE_CELEBRATE; }
    PARAMETRIZE { kind = 1; foe = SPECIES_GASTLY;  foeAbility = ABILITY_LEVITATE;    move = MOVE_TACKLE;      foeMove = MOVE_CELEBRATE; }
    PARAMETRIZE { kind = 2; foe = SPECIES_LANTURN; foeAbility = ABILITY_VOLT_ABSORB; move = MOVE_THUNDER_SHOCK; foeMove = MOVE_CELEBRATE; }
    PARAMETRIZE { kind = 3; foe = SPECIES_SNORLAX; foeAbility = ABILITY_THICK_FAT;   move = MOVE_TACKLE;      foeMove = MOVE_PROTECT; }
    PARAMETRIZE { kind = 4; foe = SPECIES_SNORLAX; foeAbility = ABILITY_THICK_FAT;   move = MOVE_GROWL;       foeMove = MOVE_CELEBRATE; }
    GIVEN {
        ASSUME(GetMoveAccuracy(MOVE_SLAM) < 100);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Speed(10); Item(ITEM_LIFE_ORB); }
        OPPONENT(foe) { Ability(foeAbility); MaxHP(300); HP(300); Speed(100); }
    } WHEN {
        if (kind == 0)
            TURN { MOVE(opponent, foeMove); MOVE(player, move, hit: FALSE); }
        else
            TURN { MOVE(opponent, foeMove); MOVE(player, move); }
    } SCENE {
        NONE_OF {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(player->hp, 200);
    }
}

// Hyper Beam: the loss on the attack turn, none on the recharge turn.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: no loss on the recharge turn")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { MaxHP(300); HP(300); Speed(100); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_HYPER_BEAM); }
        TURN { SKIP_TURN(player); }
    } SCENE {
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 30);
    } THEN {
        EXPECT_EQ(player->hp, 270);
    }
}

// The holder knocked out by Rough Skin during its move: no Life Orb.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: none when the holder fainted during its move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(5); Speed(100); Item(ITEM_LIFE_ORB); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(200); HP(200); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, hp: 0);
        NOT ITEM_POPUP(player, ITEM_LIFE_ORB);
    }
}

// U-turn: the loss before the switch (MOVEEND_LIFE_ORB_SHELL_BELL before MOVEEND_HIT_ESCAPE).
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: a pivot move loses HP before the switch")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_U_TURN) == EFFECT_HIT_ESCAPE);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Speed(100); Item(ITEM_LIFE_ORB); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_U_TURN); SEND_OUT(player, 1); }
    } SCENE {
        HP_BAR(opponent);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 20);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 180);
    }
}

// ---------------------------------------------------------------- abilities

// Magic Guard: the boost stays, no loss.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: Magic Guard keeps the boost and loses no HP", s16 damage)
{
    enum Item item;
    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; }
    GIVEN {
        PLAYER(SPECIES_CLEFABLE) { Ability(ABILITY_MAGIC_GUARD); Level(50); Attack(100); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        NONE_OF {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(player->hp, 300);
    } FINALLY {
        Test_MgbaPrintf("LO_MAGIC_GUARD %d %d", results[0].damage, results[1].damage);
        EXPECT_EQ(results[1].damage, LO(results[0].damage));
    }
}

// Gastro Acid on the Magic Guard holder: the loss comes back.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: a suppressed Magic Guard loses HP")
{
    GIVEN {
        PLAYER(SPECIES_CLEFABLE) { Ability(ABILITY_MAGIC_GUARD); MaxHP(300); HP(300); Speed(10); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_GASTRO_ACID); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_GASTRO_ACID, opponent);
        HP_BAR(opponent);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 30);
    }
}

// Sheer Force: a move with a secondary effect it removes gets both boosts and
// no loss; a move without one loses HP as usual.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: Sheer Force skips the loss only when it applies", s16 damage)
{
    enum Item item; enum Move move;
    PARAMETRIZE { item = ITEM_NONE;     move = MOVE_ROCK_SLIDE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; move = MOVE_ROCK_SLIDE; }
    PARAMETRIZE { item = ITEM_NONE;     move = MOVE_STRENGTH; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; move = MOVE_STRENGTH; }
    GIVEN {
        ASSUME(MoveIsAffectedBySheerForce(MOVE_ROCK_SLIDE));
        ASSUME(!MoveIsAffectedBySheerForce(MOVE_STRENGTH));
        PLAYER(SPECIES_NIDOKING) { Ability(ABILITY_SHEER_FORCE); Level(50); Attack(100); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, move, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        if (item == ITEM_LIFE_ORB && move == MOVE_STRENGTH)
        {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player, damage: 30);
        }
        else
        {
            NONE_OF { ITEM_POPUP(player, ITEM_LIFE_ORB); HP_BAR(player); }
        }
    } FINALLY {
        Test_MgbaPrintf("LO_SHEER_FORCE %d %d %d %d", results[0].damage, results[1].damage, results[2].damage, results[3].damage);
        EXPECT_EQ(results[1].damage, LO(results[0].damage));
        EXPECT_EQ(results[3].damage, LO(results[2].damage));
    }
}

// Klutz: neither the boost nor the loss; suppressed Klutz (Gastro Acid): both.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: Klutz turns it off, a suppressed Klutz does not", s16 damage)
{
    enum Item item; bool32 suppressed;
    PARAMETRIZE { item = ITEM_NONE;     suppressed = FALSE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; suppressed = FALSE; }
    PARAMETRIZE { item = ITEM_NONE;     suppressed = TRUE; }
    PARAMETRIZE { item = ITEM_LIFE_ORB; suppressed = TRUE; }
    GIVEN {
        PLAYER(SPECIES_LOPUNNY) { Ability(ABILITY_KLUTZ); Level(50); Attack(100); MaxHP(300); HP(300); Speed(10); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, suppressed ? MOVE_GASTRO_ACID : MOVE_CELEBRATE);
               MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, 0), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        if (item == ITEM_LIFE_ORB && suppressed)
        {
            ITEM_POPUP(player, ITEM_LIFE_ORB);
            HP_BAR(player, damage: 30);
        }
        else
        {
            NONE_OF { ITEM_POPUP(player, ITEM_LIFE_ORB); HP_BAR(player); }
        }
    } FINALLY {
        Test_MgbaPrintf("LO_KLUTZ %d %d %d %d", results[0].damage, results[1].damage, results[2].damage, results[3].damage);
        EXPECT_EQ(results[1].damage, results[0].damage);
        EXPECT_EQ(results[3].damage, LO(results[2].damage));
    }
}

// ---------------------------------------------------------------- M4-B1c-0b additions

// Struggle: the final chain has no exception for it, so Life Orb boosts it.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: Struggle is boosted", s16 damage)
{
    enum Item item; u32 roll;
    LO_ROLLS
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Level(50); Attack(100); MaxHP(300); HP(300); Speed(100); Item(item); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Defense(100); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_LO_PAIRS();
    }
}

// Struggle's own recoil (max HP / 4 from Gen 4) first, then the Life Orb loss.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: Struggle's recoil, then the loss")
{
    GIVEN {
        ASSUME(B_STRUGGLE_RECOIL >= GEN_4);
        PLAYER(SPECIES_SNORLAX) { MaxHP(300); HP(300); Speed(100); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(opponent);
        HP_BAR(player, damage: 75);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 30);
    } THEN {
        EXPECT_EQ(player->hp, 195);
    }
}

// Struggle's recoil knocks the holder out: no Life Orb.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: none after Struggle's recoil knocked the holder out")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { MaxHP(300); HP(50); Speed(100); Item(ITEM_LIFE_ORB); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE); SEND_OUT(player, 1); }
    } SCENE {
        HP_BAR(opponent);
        HP_BAR(player, hp: 0);
        NOT ITEM_POPUP(player, ITEM_LIFE_ORB);
    }
}

// The last foe knocked out: the loss still comes (the move end runs before the battle ends).
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: the loss after knocking out the last foe")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(200); Speed(100); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(100); HP(1); Speed(30); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, hp: 0);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 20);
    } THEN {
        EXPECT_EQ(player->hp, 180);
    }
}

// Moxie (MOVEEND_ABILITY_EFFECT_FOES_FAINTED) comes before the Life Orb stage.
SINGLE_BATTLE_TEST("GC V20 reference: Life Orb: Moxie's boost, then the loss")
{
    GIVEN {
        PLAYER(SPECIES_SALAMENCE) { Ability(ABILITY_MOXIE); MaxHP(300); HP(300); Speed(100); Item(ITEM_LIFE_ORB); }
        OPPONENT(SPECIES_SNORLAX) { MaxHP(100); HP(1); Speed(30); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent, hp: 0);
        ABILITY_POPUP(player, ABILITY_MOXIE);
        ITEM_POPUP(player, ITEM_LIFE_ORB);
        HP_BAR(player, damage: 30);
    } THEN {
        EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
    }
}
