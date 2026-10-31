#include "global.h"
#include "test/battle.h"

// Unseen Fist's pop-up belongs to hits on a Pokémon that is itself protected.
DOUBLE_BATTLE_TEST("Unseen Fist shows only when the target itself used Protect")
{
    bool32 hitProtected;
    PARAMETRIZE { hitProtected = TRUE; }
    PARAMETRIZE { hitProtected = FALSE; }
    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_URSHIFU) { Ability(ABILITY_UNSEEN_FIST); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN {
            MOVE(opponentLeft, MOVE_PROTECT);
            MOVE(playerLeft, MOVE_SCRATCH, target: hitProtected ? opponentLeft : opponentRight);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, opponentLeft);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, playerLeft);
        if (hitProtected)
            ABILITY_POPUP(playerLeft, ABILITY_UNSEEN_FIST);
        else
            NOT ABILITY_POPUP(playerLeft, ABILITY_UNSEEN_FIST);
    }
}

// The strong winds take away the Flying type's weaknesses; the message belongs to those hits only.
SINGLE_BATTLE_TEST("Delta Stream's message shows only when a Flying weakness is taken away")
{
    enum Move move;
    bool32 weakened;
    PARAMETRIZE { move = MOVE_DRAGON_CLAW; weakened = FALSE; } // Super effective through Dragon only
    PARAMETRIZE { move = MOVE_THUNDERBOLT; weakened = TRUE; }  // Flying weakness, resisted by Dragon
    PARAMETRIZE { move = MOVE_ICE_BEAM; weakened = TRUE; }     // Both weaknesses
    PARAMETRIZE { move = MOVE_TACKLE; weakened = FALSE; }
    GIVEN {
        ASSUME(GetMoveType(MOVE_DRAGON_CLAW) == TYPE_DRAGON);
        ASSUME(GetMoveType(MOVE_THUNDERBOLT) == TYPE_ELECTRIC);
        ASSUME(GetMoveType(MOVE_ICE_BEAM) == TYPE_ICE);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_RAYQUAZA) { Ability(ABILITY_DELTA_STREAM); }
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_STRONG_WINDS);
        if (weakened)
            MESSAGE("{JPN}らんきりゅうで こうげきが よわめられた！");
        else
            NOT MESSAGE("{JPN}らんきりゅうで こうげきが よわめられた！");
        ANIMATION(ANIM_TYPE_MOVE, move, player);
    }
}
