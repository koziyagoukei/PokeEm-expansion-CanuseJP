#include "global.h"
#include "test/battle.h"
#include "battle_frontier.h"
#include "constants/battle_frontier_mons.h"

// M4-B1c-1a (gba-gc-extension docs/shell-bell.md): Shell Bell in this build,
// for the GC core's later request V21. No MESSAGE checks (the text is
// Japanese): HP bars, substitute hits, the item animation and the final HP
// carry the result. Each test prints SB_* lines (the values measured).
//
// Source (SOURCE_CONFIRMED): TryShellBell heals SetHealAmount(savedDmg /
// holdEffectParam 8), at least 1, when savedDmg > 0, the move was used
// (!unableToUseMove), it is not Future Sight, the holder is the battler that
// started the move (originalBattlerPartyId), it is below its max HP and not
// under Heal Block (Gen 5+); ItemBattleEffects needs the holder alive. It runs
// at MOVEEND_LIFE_ORB_SHELL_BELL (MoveEndLifeOrbShellBell), skipped by
// MOVEEND_SHEER_FORCE for the moves Sheer Force affects. savedDmg is reset
// per move and summed at MOVEEND_SET_VALUES from moveDamage of each target
// of each strike: TryMoveDamageUpdate cuts moveDamage to the HP the target
// had (a faint) and to the substitute's HP when it breaks.

#define SB(d) ((d) / 8 ? (d) / 8 : 1)
#define BELL(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, battler)
#define BERRY(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_BERRY, battler)

ASSUMPTIONS
{
    ASSUME(gItemsInfo[ITEM_SHELL_BELL].holdEffect == HOLD_EFFECT_SHELL_BELL);
    ASSUME(gItemsInfo[ITEM_SHELL_BELL].holdEffectParam == 8);
    ASSUME(GetMoveEffect(MOVE_SEISMIC_TOSS) == EFFECT_LEVEL_DAMAGE);
}

// ---------------------------------------------------------------- the amount

SINGLE_BATTLE_TEST("GC Shell Bell reference: heal = damage / 8 rounded down, at least 1 (Seismic Toss = level)", s16 heal)
{
    u32 level = 1;
    PARAMETRIZE { level = 1; }
    PARAMETRIZE { level = 7; }
    PARAMETRIZE { level = 8; }
    PARAMETRIZE { level = 9; }
    PARAMETRIZE { level = 15; }
    PARAMETRIZE { level = 16; }
    PARAMETRIZE { level = 17; }
    PARAMETRIZE { level = 50; }
    PARAMETRIZE { level = 100; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(level); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: level);
        BELL(player);
        HP_BAR(player, captureDamage: &results[i].heal);
    } THEN {
        Test_MgbaPrintf("SB_AMOUNT dealt=%d heal=%d", level, -results[i].heal);
        EXPECT_EQ(-results[i].heal, SB(level));
        EXPECT_EQ(player->hp, 100 + SB(level));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: a calculated hit heals 1/8 of the HP it took", s16 damage, s16 heal)
{
    u32 roll = 0;
    PARAMETRIZE { roll = 0; }
    PARAMETRIZE { roll = 7; }
    PARAMETRIZE { roll = 15; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(60); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRENGTH, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        BELL(player);
        HP_BAR(player, captureDamage: &results[i].heal);
    } THEN {
        Test_MgbaPrintf("SB_HIT roll=%d dealt=%d heal=%d", roll, results[i].damage, -results[i].heal);
        EXPECT_EQ(-results[i].heal, SB(results[i].damage));
    }
}

// Overkill: the base is the HP the target had, not the damage calculated.
SINGLE_BATTLE_TEST("GC Shell Bell reference: a knock-out counts only the HP the target had", s16 heal)
{
    u32 foeHp = 10;
    PARAMETRIZE { foeHp = 7; }
    PARAMETRIZE { foeHp = 10; }
    PARAMETRIZE { foeHp = 20; }
    PARAMETRIZE { foeHp = 63; }
    PARAMETRIZE { foeHp = 64; }
    PARAMETRIZE { foeHp = 99; }
    PARAMETRIZE { foeHp = 100; }
    PARAMETRIZE { foeHp = 150; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(foeHp); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); if (foeHp <= 100) { SEND_OUT(opponent, 1); } }
    } SCENE {
        HP_BAR(opponent);
        BELL(player);
        HP_BAR(player, captureDamage: &results[i].heal);
    } THEN {
        const u32 lost = foeHp < 100 ? foeHp : 100;
        Test_MgbaPrintf("SB_OVERKILL foe_hp=%d calculated=100 lost=%d heal=%d (calculated/8=12)", foeHp, lost, -results[i].heal);
        EXPECT_EQ(-results[i].heal, SB(lost));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: at full HP nothing happens; near it the heal stops at max HP")
{
    u32 hp = 400;
    PARAMETRIZE { hp = 400; }
    PARAMETRIZE { hp = 399; }
    PARAMETRIZE { hp = 395; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(hp); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: 100);
        if (hp == 400) {
            NONE_OF { BELL(player); HP_BAR(player); }
        } else {
            BELL(player);
            HP_BAR(player, damage: -(400 - (s32)hp));
        }
    } THEN {
        Test_MgbaPrintf("SB_CAP hp=%d after=%d (heal 12 due)", hp, player->hp);
        EXPECT_EQ(player->hp, 400);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: no HP taken, no heal (False Swipe at 1 HP); 1 HP taken heals 1")
{
    enum Move move = MOVE_FALSE_SWIPE;
    PARAMETRIZE { move = MOVE_FALSE_SWIPE; }
    PARAMETRIZE { move = MOVE_SEISMIC_TOSS; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, move); if (move == MOVE_SEISMIC_TOSS) { SEND_OUT(opponent, 1); } }
    } SCENE {
        if (move == MOVE_FALSE_SWIPE) {
            NONE_OF { BELL(player); HP_BAR(player); }
        } else {
            HP_BAR(opponent, damage: 1);
            BELL(player);
            HP_BAR(player, damage: -1);
        }
    } THEN {
        Test_MgbaPrintf("SB_ZERO_ONE move=%d hp=%d", move, player->hp);
        EXPECT_EQ(player->hp, move == MOVE_FALSE_SWIPE ? 100 : 101);
    }
}

// ---------------------------------------------------------------- multi-hit

SINGLE_BATTLE_TEST("GC Shell Bell reference: a multi-hit move heals once, 1/8 of the sum of its hits (2-5 hits)", s16 heal)
{
    u32 hits = 2;
    static s16 d[5];
    PARAMETRIZE { hits = 2; }
    PARAMETRIZE { hits = 3; }
    PARAMETRIZE { hits = 4; }
    PARAMETRIZE { hits = 5; }
    GIVEN {
        ASSUME(IsMultiHitMove(MOVE_BULLET_SEED));
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(999); HP(999); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_BULLET_SEED, WITH_RNG(RNG_HITS, hits), criticalHit: FALSE); }
    } SCENE {
        for (u32 h = 0; h < hits; h++) {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
            HP_BAR(opponent, captureDamage: &d[h]);
            if (h + 1 < hits)
                NOT HP_BAR(player);  // nothing between the hits
        }
        BELL(player);
        HP_BAR(player, captureDamage: &results[i].heal);
    } THEN {
        s32 sum = 0, floors = 0;
        for (u32 h = 0; h < hits; h++) { sum += d[h]; floors += d[h] / 8; }
        Test_MgbaPrintf("SB_MULTI hits=%d d=%d,%d,%d,%d,%d sum=%d heal=%d per_hit_floors=%d", hits, d[0], d[1], d[2], d[3], d[4], sum, -results[i].heal, floors);
        EXPECT_EQ(-results[i].heal, SB(sum));
        EXPECT_EQ(player->hp, 100 + SB(sum));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: a multi-hit move that knocks out at the 2nd hit counts the HP the foe had")
{
    static s16 d1, d2, heal;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(999); HP(30); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_BULLET_SEED, WITH_RNG(RNG_HITS, 5), criticalHit: FALSE); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        HP_BAR(opponent, captureDamage: &d1);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        HP_BAR(opponent, captureDamage: &d2);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);  // no third hit
        BELL(player);
        HP_BAR(player, captureDamage: &heal);
    } THEN {
        Test_MgbaPrintf("SB_MULTI_KO d1=%d d2=%d (foe had 30) heal=%d", d1, d2, -heal);
        EXPECT_EQ(d1 + d2, 30);
        EXPECT_EQ(-heal, SB(30));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: a substitute broken mid multi-hit counts its HP, then the hits on the Pokemon")
{
    static u16 s1, s2;
    static s16 h3, h4, h5, heal;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(10); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(84); HP(84); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_BULLET_SEED, WITH_RNG(RNG_HITS, 5), criticalHit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        SUB_HIT(opponent, captureDamage: &s1);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        SUB_HIT(opponent, captureDamage: &s2, subBreak: TRUE);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        HP_BAR(opponent, captureDamage: &h3);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        HP_BAR(opponent, captureDamage: &h4);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        HP_BAR(opponent, captureDamage: &h5);
        BELL(player);
        HP_BAR(player, captureDamage: &heal);
    } THEN {
        const s32 sum = s1 + s2 + h3 + h4 + h5;
        Test_MgbaPrintf("SB_SUB_MULTI sub=%d+%d (sub had 21) hp=%d,%d,%d sum=%d heal=%d", s1, s2, h3, h4, h5, sum, -heal);
        EXPECT_EQ(s1 + s2, 21);
        EXPECT_EQ(-heal, SB(sum));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: a hit that takes no HP (Disguise) adds nothing to the sum")
{
    static s16 d[4], heal;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_MIMIKYU_DISGUISED) { Ability(ABILITY_DISGUISE); Level(50); Defense(80); MaxHP(999); HP(999); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_BULLET_SEED, WITH_RNG(RNG_HITS, 5), criticalHit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BULLET_SEED, player);
        ABILITY_POPUP(opponent, ABILITY_DISGUISE);
        HP_BAR(opponent, captureDamage: &d[0]);
        HP_BAR(opponent, captureDamage: &d[1]);
        HP_BAR(opponent, captureDamage: &d[2]);
        HP_BAR(opponent, captureDamage: &d[3]);
        BELL(player);
        HP_BAR(player, captureDamage: &heal);
    } THEN {
        const s32 sum = d[0] + d[1] + d[2] + d[3];
        Test_MgbaPrintf("SB_ZERO_HIT disguise hit=0 then %d,%d,%d,%d sum=%d foe_lost=%d heal=%d", d[0], d[1], d[2], d[3], sum, 999 - opponent->hp, -heal);
        EXPECT_EQ(999 - opponent->hp, sum);  // Gen 7 Disguise: no chip
        EXPECT_EQ(-heal, SB(sum));
    }
}

// ---------------------------------------------------------------- several targets

DOUBLE_BATTLE_TEST("GC Shell Bell reference: a spread move heals once, 1/8 of the sum over its targets", s16 left, s16 right, s16 heal)
{
    u32 roll = 0;
    for (u32 r = 0; r < 16; r += 3)
        PARAMETRIZE { roll = r; }
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_ROCK_SLIDE) == TARGET_BOTH);
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(70); MaxHP(999); HP(999); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(95); MaxHP(999); HP(999); Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROCK_SLIDE, WITH_RNG(RNG_DAMAGE_MODIFIER, roll), criticalHit: FALSE, hit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &results[i].left);
        HP_BAR(opponentRight, captureDamage: &results[i].right);
        BELL(playerLeft);
        HP_BAR(playerLeft, captureDamage: &results[i].heal);
    } FINALLY {
        bool32 discriminating = FALSE;
        for (u32 k = 0; k < 6; k++)
        {
            const s32 a = results[k].left, b = results[k].right;
            Test_MgbaPrintf("SB_SPREAD run=%d left=%d right=%d sum=%d heal=%d per_target_floors=%d", k, a, b, a + b, -results[k].heal, a / 8 + b / 8);
            EXPECT_EQ(-results[k].heal, SB(a + b));
            if (a / 8 + b / 8 != (a + b) / 8)
                discriminating = TRUE;
        }
        EXPECT(discriminating);  // some run tells "sum, then / 8" from "/ 8 per target"
    }
}

DOUBLE_BATTLE_TEST("GC Shell Bell reference: an ally hit by the user's spread move counts; an immune foe does not")
{
    static s16 foe, ally, heal;
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == TARGET_FOES_AND_ALLY);
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(999); HP(999); Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(999); HP(999); Speed(10); }
        OPPONENT(SPECIES_PIDGEOT) { Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_EARTHQUAKE, criticalHit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_EARTHQUAKE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &foe);
        HP_BAR(playerRight, captureDamage: &ally);
        NOT HP_BAR(opponentRight);
        BELL(playerLeft);
        HP_BAR(playerLeft, captureDamage: &heal);
    } THEN {
        Test_MgbaPrintf("SB_ALLY foe=%d ally=%d immune=0 heal=%d (foe only would be %d)", foe, ally, -heal, SB(foe));
        EXPECT_EQ(-heal, SB(foe + ally));
    }
}

DOUBLE_BATTLE_TEST("GC Shell Bell reference: a protected foe adds nothing; the other foe's HP counts")
{
    static s16 right, heal;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(10); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(999); HP(999); Speed(30); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_PROTECT); MOVE(playerLeft, MOVE_ROCK_SLIDE, criticalHit: FALSE, hit: TRUE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        NOT HP_BAR(opponentLeft);
        HP_BAR(opponentRight, captureDamage: &right);
        BELL(playerLeft);
        HP_BAR(playerLeft, captureDamage: &heal);
    } THEN {
        Test_MgbaPrintf("SB_PROTECT right=%d heal=%d", right, -heal);
        EXPECT_EQ(-heal, SB(right));
    }
}

DOUBLE_BATTLE_TEST("GC Shell Bell reference: a foe's substitute counts what it took; a fainting foe counts its HP")
{
    static u16 sub;
    static s16 heal;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(10); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(80); MaxHP(400); HP(400); Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(11); Speed(30); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(30); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SUBSTITUTE); MOVE(playerLeft, MOVE_ROCK_SLIDE, criticalHit: FALSE, hit: TRUE); SEND_OUT(opponentRight, 2); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, playerLeft);
        HP_BAR(opponentRight, damage: 11);
        SUB_HIT(opponentLeft, captureDamage: &sub);
        BELL(playerLeft);
        HP_BAR(playerLeft, captureDamage: &heal);
    } THEN {
        Test_MgbaPrintf("SB_SUB_FAINT sub=%d fainted_foe_had=11 heal=%d", sub, -heal);
        EXPECT_EQ(-heal, SB(sub + 11));
    }
}

DOUBLE_BATTLE_TEST("GC Shell Bell reference: when every foe faints the heal still comes, from the HP they had")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(20); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(9); Speed(10); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(14); Speed(10); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_ROCK_SLIDE, criticalHit: FALSE, hit: TRUE); }
    } SCENE {
        HP_BAR(opponentLeft, damage: 9);
        HP_BAR(opponentRight, damage: 14);
        BELL(playerLeft);
        HP_BAR(playerLeft, damage: -SB(23));
    } THEN {
        Test_MgbaPrintf("SB_ALL_FAINT had=9+14 heal=%d", playerLeft->hp - 100);
        EXPECT_EQ(playerLeft->hp, 100 + SB(23));
    }
}

// ---------------------------------------------------------------- no heal

SINGLE_BATTLE_TEST("GC Shell Bell reference: no heal for a miss, Protect, a type immunity, an absorbing ability or a status move")
{
    u32 k = 0;
    enum Move move = MOVE_FOCUS_BLAST;
    u16 foe = SPECIES_WOBBUFFET;
    PARAMETRIZE { k = 0; move = MOVE_FOCUS_BLAST; foe = SPECIES_WOBBUFFET; }  // miss
    PARAMETRIZE { k = 1; move = MOVE_TACKLE; foe = SPECIES_WOBBUFFET; }       // Protect
    PARAMETRIZE { k = 2; move = MOVE_TACKLE; foe = SPECIES_GENGAR; }          // Normal into Ghost
    PARAMETRIZE { k = 3; move = MOVE_THUNDERBOLT; foe = SPECIES_LANTURN; }    // Volt Absorb
    PARAMETRIZE { k = 4; move = MOVE_SWORDS_DANCE; foe = SPECIES_WOBBUFFET; } // status move
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(100); Speed(10); Item(ITEM_SHELL_BELL); }
        OPPONENT(foe) { Ability(foe == SPECIES_LANTURN ? ABILITY_VOLT_ABSORB : foe == SPECIES_GENGAR ? ABILITY_CURSED_BODY : ABILITY_TELEPATHY); Speed(100); }
    } WHEN {
        if (k == 0)
            TURN { MOVE(player, move, hit: FALSE); }
        else if (k == 1)
            TURN { MOVE(opponent, MOVE_PROTECT); MOVE(player, move); }
        else
            TURN { MOVE(player, move); }
    } SCENE {
        NONE_OF { BELL(player); HP_BAR(player); }
    } THEN {
        Test_MgbaPrintf("SB_NONE case=%d hp=%d", k, player->hp);
        EXPECT_EQ(player->hp, 100);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: no heal under Heal Block, Embargo or with Klutz")
{
    u32 k = 0;
    PARAMETRIZE { k = 0; }  // Heal Block (Gen 5+ blocks it)
    PARAMETRIZE { k = 1; }  // Embargo
    PARAMETRIZE { k = 2; }  // Klutz
    GIVEN {
        ASSUME(GetConfig(B_HEAL_BLOCKING) >= GEN_5);
        if (k == 2)
            PLAYER(SPECIES_LOPUNNY) { Ability(ABILITY_KLUTZ); Level(50); MaxHP(400); HP(100); Speed(10); Item(ITEM_SHELL_BELL); }
        else
            PLAYER(SPECIES_WOBBUFFET) { Level(50); MaxHP(400); HP(100); Speed(10); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); Speed(100); }
    } WHEN {
        if (k == 0)
            TURN { MOVE(opponent, MOVE_HEAL_BLOCK); MOVE(player, MOVE_SEISMIC_TOSS); }
        else if (k == 1)
            TURN { MOVE(opponent, MOVE_EMBARGO); MOVE(player, MOVE_SEISMIC_TOSS); }
        else
            TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: 50);
        NONE_OF { BELL(player); HP_BAR(player); }
    } THEN {
        Test_MgbaPrintf("SB_BLOCKED case=%d hp=%d", k, player->hp);
        EXPECT_EQ(player->hp, 100);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Sheer Force skips it for the moves it boosts only; Magic Guard keeps it")
{
    u32 k = 0;
    PARAMETRIZE { k = 0; }  // Sheer Force, Flamethrower (a secondary effect): no heal
    PARAMETRIZE { k = 1; }  // Sheer Force, Strength (none): heal
    PARAMETRIZE { k = 2; }  // Magic Guard: heal
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_FLAMETHROWER, MOVE_EFFECT_BURN));
        if (k < 2)
            PLAYER(SPECIES_NIDOKING) { Ability(ABILITY_SHEER_FORCE); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        else
            PLAYER(SPECIES_CLEFABLE) { Ability(ABILITY_MAGIC_GUARD); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, k == 0 ? MOVE_FLAMETHROWER : MOVE_STRENGTH, criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent);
        if (k == 0) {
            NONE_OF { BELL(player); HP_BAR(player); }
        } else {
            BELL(player);
            HP_BAR(player);
        }
    } THEN {
        Test_MgbaPrintf("SB_ABILITY case=%d hp=%d", k, player->hp);
        if (k == 0)
            EXPECT_EQ(player->hp, 100);
        else
            EXPECT_GT(player->hp, 100);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Hyper Beam heals on its turn, not on the recharge turn; a charging turn does not")
{
    u32 k = 0;
    PARAMETRIZE { k = 0; }  // Hyper Beam
    PARAMETRIZE { k = 1; }  // Solar Beam (no sun): charge, then the hit
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); Speed(10); }
    } WHEN {
        if (k == 0) {
            TURN { MOVE(player, MOVE_HYPER_BEAM, criticalHit: FALSE); }
            TURN { SKIP_TURN(player); }
        } else {
            TURN { MOVE(player, MOVE_SOLAR_BEAM, criticalHit: FALSE); }
            TURN { SKIP_TURN(player); }
        }
    } SCENE {
        if (k == 0) {
            HP_BAR(opponent);
            BELL(player);
            HP_BAR(player);
            NONE_OF { BELL(player); HP_BAR(player); }  // the recharge turn
        } else {
            NONE_OF { HP_BAR(opponent); BELL(player); }  // the charging turn ends without a hit
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SOLAR_BEAM, player);
            HP_BAR(opponent);
            BELL(player);
            HP_BAR(player);
        }
    } THEN {
        Test_MgbaPrintf("SB_TURNS case=%d hp=%d", k, player->hp);
        EXPECT_GT(player->hp, 100);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Struggle heals 1/8 after its recoil; none when the recoil knocks the holder out")
{
    u32 hp = 200;
    static s16 dealt, recoil, heal;
    PARAMETRIZE { hp = 200; }
    PARAMETRIZE { hp = 50; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(200); HP(hp); Speed(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_STRUGGLE, criticalHit: FALSE); if (hp == 50) { SEND_OUT(player, 1); } }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRUGGLE, player);
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &recoil);
        if (hp == 200) {
            BELL(player);
            HP_BAR(player, captureDamage: &heal);
        } else {
            NONE_OF { BELL(player); }
        }
    } THEN {
        Test_MgbaPrintf("SB_STRUGGLE hp=%d dealt=%d recoil=%d heal=%d", hp, dealt, recoil, hp == 200 ? -heal : 0);
        EXPECT_EQ(recoil, 50);
        if (hp == 200)
            EXPECT_EQ(-heal, SB(dealt));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: no heal when Rough Skin knocks the holder out first")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(1); Speed(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); }
        OPPONENT(SPECIES_GIBLE) { Ability(ABILITY_ROUGH_SKIN); MaxHP(999); HP(999); Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(player, 1); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, hp: 0);
        NONE_OF { BELL(player); }
    } THEN {
        Test_MgbaPrintf("SB_HOLDER_DOWN hp=%d", GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: no heal for Future Sight's hit (the user on the field or not)")
{
    u32 k = 0;
    PARAMETRIZE { k = 0; }
    PARAMETRIZE { k = 1; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_FUTURE_SIGHT) == EFFECT_FUTURE_SIGHT);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_FUTURE_SIGHT); }
        TURN { if (k == 1) { SWITCH(player, 1); } }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FUTURE_SIGHT, player);
        HP_BAR(opponent);
        NONE_OF { BELL(player); }
    } THEN {
        Test_MgbaPrintf("SB_FUTURE case=%d holder_hp=%d", k, GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP));
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 100);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: a Red Card that forces the user out first stops the heal")
{
    GIVEN {
        ASSUME(gItemsInfo[ITEM_RED_CARD].holdEffect == HOLD_EFFECT_RED_CARD);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(100); Speed(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WYNAUT) { Speed(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); Speed(10); Item(ITEM_RED_CARD); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent);
        NONE_OF { BELL(player); HP_BAR(player); }
    } THEN {
        Test_MgbaPrintf("SB_RED_CARD holder_hp=%d on_field=%d", GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), player->species);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 100);
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
    }
}

// ---------------------------------------------------------------- order

SINGLE_BATTLE_TEST("GC Shell Bell reference: Giga Drain's own drain first, then 1/8; none once the drain filled the HP")
{
    u32 hp = 100;
    static s16 dealt, drain, heal;
    PARAMETRIZE { hp = 100; }
    PARAMETRIZE { hp = 399; }
    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_GIGA_DRAIN, MOVE_EFFECT_ABSORB));
        PLAYER(SPECIES_WOBBUFFET) { Level(50); SpAttack(150); MaxHP(400); HP(hp); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); SpDefense(60); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_GIGA_DRAIN, criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &drain);
        if (hp == 100) {
            BELL(player);
            HP_BAR(player, captureDamage: &heal);
        } else {
            NONE_OF { BELL(player); }
        }
    } THEN {
        Test_MgbaPrintf("SB_DRAIN hp=%d dealt=%d drain=%d bell=%d", hp, dealt, -drain, hp == 100 ? -heal : 0);
        if (hp == 100)
        {
            EXPECT_EQ(-drain, dealt / 2);
            EXPECT_EQ(-heal, SB(dealt));
        }
        EXPECT_EQ(player->hp, hp == 100 ? 100 + dealt / 2 + SB(dealt) : 400);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Double-Edge's recoil first, then 1/8; none when the recoil knocks the holder out")
{
    u32 hp = 400;
    static s16 dealt, recoil, heal;
    PARAMETRIZE { hp = 400 - 10; }
    PARAMETRIZE { hp = 5; }
    GIVEN {
        ASSUME(GetMoveRecoil(MOVE_DOUBLE_EDGE) == 33);
        PLAYER(SPECIES_WOBBUFFET) { Level(50); Attack(150); MaxHP(400); HP(hp); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Level(50); Defense(60); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_EDGE, criticalHit: FALSE); if (hp == 5) { SEND_OUT(player, 1); } }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &recoil);
        if (hp != 5) {
            BELL(player);
            HP_BAR(player, captureDamage: &heal);
        } else {
            NONE_OF { BELL(player); }
        }
    } THEN {
        Test_MgbaPrintf("SB_RECOIL hp=%d dealt=%d recoil=%d bell=%d", hp, dealt, recoil, hp != 5 ? -heal : 0);
        if (hp != 5)
            EXPECT_EQ(-heal, SB(dealt));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Moxie after a knock-out, then the heal")
{
    GIVEN {
        PLAYER(SPECIES_SALAMENCE) { Ability(ABILITY_MOXIE); Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(40); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent, damage: 40);
        ABILITY_POPUP(player, ABILITY_MOXIE);
        BELL(player);
        HP_BAR(player, damage: -SB(40));
    } THEN {
        Test_MgbaPrintf("SB_MOXIE hp=%d", player->hp);
        EXPECT_EQ(player->hp, 100 + SB(40));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: knocking out the last foe still heals")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(40); }
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: 40);
        BELL(player);
        HP_BAR(player, damage: -SB(40));
    } THEN {
        Test_MgbaPrintf("SB_LAST_FOE hp=%d", player->hp);
        EXPECT_EQ(player->hp, 100 + SB(40));
    }
}

// The order M4-B1c-0c fixed for Life Orb: the other Pokemon's HP berries
// (MOVEEND_KEE_MARANGA_HP_THRESHOLD_ITEM_TARGET) come before the stage.
SINGLE_BATTLE_TEST("GC Shell Bell reference: the target's Sitrus Berry first, then the heal")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(160); HP(160); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: 100);
        BERRY(opponent);
        HP_BAR(opponent, damage: -40);
        BELL(player);
        HP_BAR(player, damage: -SB(100));
    } THEN {
        Test_MgbaPrintf("SB_SITRUS foe=%d holder=%d", opponent->hp, player->hp);
        EXPECT_EQ(opponent->hp, 100);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: no heal before the target's Sitrus Berry")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(160); HP(160); Item(ITEM_SITRUS_BERRY); }
    } WHEN {
        TURN { MOVE(player, MOVE_SEISMIC_TOSS); }
    } SCENE {
        HP_BAR(opponent, damage: 100);
        NONE_OF { BELL(player); }
        BERRY(opponent);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Rough Skin's damage first, then the heal")
{
    static s16 rough;
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_GIBLE) { Ability(ABILITY_ROUGH_SKIN); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, captureDamage: &rough);
        BELL(player);
        HP_BAR(player);
    } THEN {
        Test_MgbaPrintf("SB_ROUGH_SKIN rough=%d hp=%d", rough, player->hp);
        EXPECT_EQ(rough, 50);
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: U-turn heals before the switch")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_U_TURN, criticalHit: FALSE); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_U_TURN, player);
        HP_BAR(opponent);
        BELL(player);
        HP_BAR(player);
    } THEN {
        Test_MgbaPrintf("SB_UTURN holder_hp=%d", GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP));
        EXPECT_GT(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP), 100);
        EXPECT_EQ(player->species, SPECIES_WYNAUT);  // healed, then switched
    }
}

// ---------------------------------------------------------------- items that move

SINGLE_BATTLE_TEST("GC Shell Bell reference: a Shell Bell taken by Thief or Covet heals for that move", s16 damage, s16 heal)
{
    enum Move move = MOVE_THIEF;
    PARAMETRIZE { move = MOVE_THIEF; }
    PARAMETRIZE { move = MOVE_COVET; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_THIEF) == EFFECT_STEAL_ITEM);
        ASSUME(GetMoveEffect(MOVE_COVET) == EFFECT_STEAL_ITEM);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(400); HP(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); Item(ITEM_SHELL_BELL); }
    } WHEN {
        TURN { MOVE(player, move, criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        BELL(player);
        HP_BAR(player, captureDamage: &results[i].heal);
    } THEN {
        Test_MgbaPrintf("SB_THIEF move=%d dealt=%d heal=%d", move, results[i].damage, -results[i].heal);
        EXPECT_EQ(player->item, ITEM_SHELL_BELL);
        EXPECT_EQ(opponent->item, ITEM_NONE);
        EXPECT_EQ(-results[i].heal, SB(results[i].damage));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: a Shell Bell taken by Magician heals for that move")
{
    static s16 dealt, heal;
    GIVEN {
        PLAYER(SPECIES_DELPHOX) { Ability(ABILITY_MAGICIAN); MaxHP(400); HP(100); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(999); HP(999); Item(ITEM_SHELL_BELL); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        ABILITY_POPUP(player, ABILITY_MAGICIAN);
        BELL(player);
        HP_BAR(player, captureDamage: &heal);
    } THEN {
        Test_MgbaPrintf("SB_MAGICIAN dealt=%d heal=%d", dealt, -heal);
        EXPECT_EQ(player->item, ITEM_SHELL_BELL);
        EXPECT_EQ(-heal, SB(dealt));
    }
}

SINGLE_BATTLE_TEST("GC Shell Bell reference: Pickpocket takes the Shell Bell after it healed")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_WOBBUFFET) { Level(100); MaxHP(400); HP(100); Item(ITEM_SHELL_BELL); }
        OPPONENT(SPECIES_SNEASEL) { Ability(ABILITY_PICKPOCKET); MaxHP(999); HP(999); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH, criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent);
        BELL(player);
        HP_BAR(player);
        ABILITY_POPUP(opponent, ABILITY_PICKPOCKET);
    } THEN {
        Test_MgbaPrintf("SB_PICKPOCKET holder_hp=%d item=%d foe_item=%d", player->hp, player->item, opponent->item);
        EXPECT_GT(player->hp, 100);
        EXPECT_EQ(player->item, ITEM_NONE);
        EXPECT_EQ(opponent->item, ITEM_SHELL_BELL);
    }
}

// ---------------------------------------------------------------- Battle Frontier

// The 2,040 gBattleFrontierMons sets (the GC exporter's measure): none holds
// Shell Bell, so its support adds no set; Life Orb (142) is the control.
TEST("GC Shell Bell reference: Battle Frontier sets holding it")
{
    u32 bell = 0, orb = 0;
    for (u32 set = 0; set < NUM_FRONTIER_MONS; set++)
    {
        bell += gBattleFrontierMons[set].heldItem == ITEM_SHELL_BELL;
        orb += gBattleFrontierMons[set].heldItem == ITEM_LIFE_ORB;
    }
    Test_MgbaPrintf("SB_FRONTIER sets=%d shell_bell=%d life_orb=%d", NUM_FRONTIER_MONS, bell, orb);
    EXPECT_EQ(bell, 0);
    EXPECT_EQ(orb, 142);
}
