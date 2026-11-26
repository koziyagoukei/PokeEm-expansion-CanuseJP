#include "global.h"
#include "test/battle.h"

// M4-B1c-1b-fix (gba-gc-extension docs/shell-bell.md section 12): a contact
// drain move against a contact ability. The drain is the move's additional
// effect (MOVE_EFFECT_ABSORB, HandleSetEffectAbsorb: this strike's
// moveDamage * percent / 100, at least 1, the attacker alive), run in the hit
// script; the contact abilities come later, at MOVEEND_ABILITIES; Shell Bell
// and Life Orb at MOVEEND_LIFE_ORB_SHELL_BELL. So a holder at 1 HP drains
// before Rough Skin and lives. No MESSAGE checks (the text is Japanese); the
// values are printed as CD_* lines.

#define BELL(battler) ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, battler)

ASSUMPTIONS
{
    ASSUME(MoveMakesContact(MOVE_DRAIN_PUNCH));
    ASSUME(GetMoveAdditionalEffectById(MOVE_DRAIN_PUNCH, 0)->moveEffect == MOVE_EFFECT_ABSORB);
    ASSUME(GetMoveAdditionalEffectById(MOVE_DRAIN_PUNCH, 0)->argument.absorbPercentage == 50);
    ASSUME(!MoveMakesContact(MOVE_GIGA_DRAIN));
    ASSUME(gItemsInfo[ITEM_SHELL_BELL].holdEffect == HOLD_EFFECT_SHELL_BELL);
}

SINGLE_BATTLE_TEST("GC contact drain reference: Drain Punch drains before Rough Skin, then Shell Bell (1 HP lives)", s16 dealt)
{
    u32 ability = ABILITY_ROUGH_SKIN;
    PARAMETRIZE { ability = ABILITY_ROUGH_SKIN; }
    PARAMETRIZE { ability = ABILITY_IRON_BARBS; }
    static s16 drain, barb, heal;
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); MaxHP(400); HP(1); Item(ITEM_SHELL_BELL); Moves(MOVE_DRAIN_PUNCH); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ability); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAIN_PUNCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAIN_PUNCH, player);
        HP_BAR(opponent, captureDamage: &results[i].dealt);
        HP_BAR(player, captureDamage: &drain);
        ABILITY_POPUP(opponent, ability);
        HP_BAR(player, captureDamage: &barb);
        BELL(player);
        HP_BAR(player, captureDamage: &heal);
    } THEN {
        const s16 d = results[i].dealt;
        Test_MgbaPrintf("CD_ORDER ability=%d dealt=%d drain=%d contact=%d bell=%d hp=%d", ability, d, -drain, barb,
                        -heal, player->hp);
        EXPECT_EQ(-drain, d / 2);
        EXPECT_EQ(barb, 400 / 8);
        EXPECT_EQ(-heal, d / 8);
        EXPECT_EQ(player->hp, 1 + d / 2 - 400 / 8 + d / 8);
    }
}

SINGLE_BATTLE_TEST("GC contact drain reference: too little drain, Rough Skin knocks the holder out: no Shell Bell")
{
    static s16 dealt, drain;
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(5); MaxHP(400); HP(1); Item(ITEM_SHELL_BELL); Moves(MOVE_DRAIN_PUNCH); }
        OPPONENT(SPECIES_SNORLAX) { Level(100); Ability(ABILITY_ROUGH_SKIN); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAIN_PUNCH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &drain);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player);
        NOT BELL(player);
    } THEN {
        Test_MgbaPrintf("CD_FAINT dealt=%d drain=%d hp=%d", dealt, -drain, player->hp);
        EXPECT_LT(1 - drain, 51);
        EXPECT_EQ(player->hp, 0);
    }
}

SINGLE_BATTLE_TEST("GC contact drain reference: Drain Punch drains before Static's paralysis")
{
    static s16 dealt, drain;
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); MaxHP(400); HP(100); Moves(MOVE_DRAIN_PUNCH); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_STATIC); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAIN_PUNCH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &drain);
        ABILITY_POPUP(opponent, ABILITY_STATIC);
        STATUS_ICON(player, paralysis: TRUE);
    } THEN {
        Test_MgbaPrintf("CD_STATIC dealt=%d drain=%d hp=%d", dealt, -drain, player->hp);
        EXPECT_EQ(-drain, min(dealt / 2, 300));  // up to the max HP
    }
}

SINGLE_BATTLE_TEST("GC contact drain reference: Life Orb's loss after the drain and Rough Skin")
{
    static s16 dealt, drain, barb, loss;
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); MaxHP(400); HP(100); Item(ITEM_LIFE_ORB); Moves(MOVE_DRAIN_PUNCH); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_ROUGH_SKIN); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAIN_PUNCH); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &drain);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, captureDamage: &barb);
        HP_BAR(player, captureDamage: &loss);
    } THEN {
        Test_MgbaPrintf("CD_ORB dealt=%d drain=%d rough=%d orb=%d hp=%d", dealt, -drain, barb, loss, player->hp);
        EXPECT_EQ(-drain, min(dealt / 2, 300));  // up to the max HP
        EXPECT_EQ(barb, 50);
        EXPECT_EQ(loss, 40);
        EXPECT_EQ(player->hp, 100 + min(dealt / 2, 300) - 50 - 40);
    }
}

SINGLE_BATTLE_TEST("GC contact drain reference: a drain move without contact never meets Rough Skin")
{
    static s16 dealt, drain;
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(100); MaxHP(400); HP(1); Moves(MOVE_GIGA_DRAIN); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_ROUGH_SKIN); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_GIGA_DRAIN); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &drain);
        NOT ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
    } THEN {
        Test_MgbaPrintf("CD_NOCONTACT dealt=%d drain=%d hp=%d", dealt, -drain, player->hp);
        EXPECT_EQ(player->hp, 1 + dealt / 2);
    }
}

// The case of the independent review (Codex: 322 dealt, 161 drained, 50 Rough
// Skin, 40 Shell Bell, 152 left) with this build's species: Machamp level 72,
// damage roll 13 deals 324 (322 is not reachable here). The GC native test
// test_fix_contact_drain plays the same 324 from 1 HP.
SINGLE_BATTLE_TEST("GC contact drain reference: 324 dealt from 1 HP: 162 drained, 50 Rough Skin, 40 Shell Bell, 153 left")
{
    static s16 dealt, drain, barb, heal;
    GIVEN {
        PLAYER(SPECIES_MACHAMP) { Level(72); MaxHP(400); HP(1); Item(ITEM_SHELL_BELL); Moves(MOVE_DRAIN_PUNCH); }
        OPPONENT(SPECIES_SNORLAX) { Level(50); Ability(ABILITY_ROUGH_SKIN); MaxHP(999); HP(999); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_DRAIN_PUNCH, WITH_RNG(RNG_DAMAGE_MODIFIER, 13), criticalHit: FALSE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &dealt);
        HP_BAR(player, captureDamage: &drain);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, captureDamage: &barb);
        BELL(player);
        HP_BAR(player, captureDamage: &heal);
    } THEN {
        Test_MgbaPrintf("CD_324 dealt=%d drain=%d rough=%d bell=%d hp=%d", dealt, -drain, barb, -heal, player->hp);
        EXPECT_EQ(dealt, 324);
        EXPECT_EQ(-drain, 162);
        EXPECT_EQ(barb, 50);
        EXPECT_EQ(-heal, 40);
        EXPECT_EQ(player->hp, 153);
    }
}
