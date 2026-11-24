#include "global.h"
#include "test/battle.h"

// ABL-G04 (gba-gc-extension docs/ability-mechanics): the order of a contact
// ability and the faints on a knock-out hit, for the GC battle core's
// comparison (gamecube/battle/battle_test.c test_abl_g04). This build's text
// is Japanese, so the faint messages are matched in Japanese; HP, ability
// pop-ups and animations carry the order without depending on the language.
// Rough Skin hurts the attacker at MOVEEND_ABILITIES (the attacker faints in
// that script); the target faints later, at MOVEEND_FAINT_BLOCK.

SINGLE_BATTLE_TEST("GC V19 reference: contact KO: the target at 0 HP, Rough Skin, then the target faints")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(96); Speed(100); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(100); HP(1); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent, hp: 0);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, damage: 12);
        MESSAGE("{JPN}{JPN}相手の {JPN}キバニアは 倒れた！");
    } THEN {
        EXPECT_EQ(player->hp, 84);
        EXPECT_EQ(opponent->species, SPECIES_WYNAUT);  // the replacement, at the end of the turn
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: contact KO: Rough Skin knocks the attacker out before the foe moves")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(5); Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(200); HP(200); Speed(50); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); MOVE(opponent, MOVE_CELEBRATE); SEND_OUT(player, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, hp: 0);
        MESSAGE("{JPN}{JPN}ソーナンスは 倒れた！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);  // the foe still moves, after the faint
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: contact KO: both at 0 HP, the attacker faints first, then the target")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(5); Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(100); HP(1); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        HP_BAR(opponent, hp: 0);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, hp: 0);
        MESSAGE("{JPN}{JPN}ソーナンスは 倒れた！");
        MESSAGE("{JPN}{JPN}相手の {JPN}キバニアは 倒れた！");
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(opponent->species, SPECIES_WYNAUT);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: contact KO: a multi-hit move stops at the knock-out, one Rough Skin")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_DOUBLE_KICK));
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(96); Speed(100); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(100); HP(1); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_DOUBLE_KICK); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DOUBLE_KICK, player);
        HP_BAR(opponent, hp: 0);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, damage: 12);
        MESSAGE("{JPN}{JPN}相手の {JPN}キバニアは 倒れた！");
        MESSAGE("{JPN}1かい 当たった！");
    } THEN {
        EXPECT_EQ(player->hp, 84);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: contact KO: a hit on a substitute: no Rough Skin")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(96); Speed(50); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(200); HP(200); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_SUBSTITUTE); MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUBSTITUTE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, player);
        SUB_HIT(opponent);
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(player->hp, 96);
        EXPECT_EQ(opponent->hp, 150);
    }
}

SINGLE_BATTLE_TEST("GC V19 reference: contact KO: a miss: no Rough Skin")
{
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SLAM) && GetMoveAccuracy(MOVE_SLAM) < 100);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(96); Speed(50); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(200); HP(200); Speed(100); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, MOVE_SLAM, hit: FALSE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponent);
        NONE_OF {
            ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
            HP_BAR(player);
        }
    } THEN {
        EXPECT_EQ(player->hp, 96);
        EXPECT_EQ(opponent->hp, 200);
    }
}

DOUBLE_BATTLE_TEST("GC V19 reference: contact KO: Double, the target faints before the next action")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(96); Speed(100); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(30); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(100); HP(1); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(40); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_SCRATCH, target: opponentLeft);
            MOVE(opponentRight, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            SEND_OUT(opponentLeft, 2);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        HP_BAR(opponentLeft, hp: 0);
        ABILITY_POPUP(opponentLeft, ABILITY_ROUGH_SKIN);
        HP_BAR(playerLeft, damage: 12);
        MESSAGE("{JPN}{JPN}相手の {JPN}キバニアは 倒れた！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
    } THEN {
        EXPECT_EQ(playerLeft->hp, 84);
        EXPECT_EQ(opponentLeft->species, SPECIES_WYNAUT);
    }
}

// The Japanese faint text is what this build prints: the same KO with the
// English text the upstream tests use does not match (a language check, not a
// battle failure), while the HP order above holds.
SINGLE_BATTLE_TEST("GC V19 reference: contact KO: the order without any message")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(96); HP(5); Speed(100); }
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        OPPONENT(SPECIES_CARVANHA) { Ability(ABILITY_ROUGH_SKIN); MaxHP(100); HP(1); Speed(50); }
        OPPONENT(SPECIES_WYNAUT) { Speed(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent, hp: 0);
        ABILITY_POPUP(opponent, ABILITY_ROUGH_SKIN);
        HP_BAR(player, hp: 0);
    } THEN {
        EXPECT_EQ(player->species, SPECIES_WYNAUT);
        EXPECT_EQ(opponent->species, SPECIES_WYNAUT);
    }
}
