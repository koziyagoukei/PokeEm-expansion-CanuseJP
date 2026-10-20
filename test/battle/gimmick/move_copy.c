#include "global.h"
#include "test/battle.h"
#include "battle_move_resolution.h"
#include "constants/battle_move_resolution.h"

static const struct
{
    enum Species species;
    enum Item item;
    enum Move baseMove;
    enum Move specialMove;
    enum Gimmick gimmick;
    bool8 gigantamax;
} sSpecialMoves[] =
{
    {SPECIES_WOBBUFFET, ITEM_NORMALIUM_Z, MOVE_POUND, MOVE_BREAKNECK_BLITZ, GIMMICK_Z_MOVE, FALSE},
    {SPECIES_EEVEE, ITEM_EEVIUM_Z, MOVE_LAST_RESORT, MOVE_EXTREME_EVOBOOST, GIMMICK_Z_MOVE, FALSE},
    {SPECIES_RAICHU_ALOLA, ITEM_ALORAICHIUM_Z, MOVE_THUNDERBOLT, MOVE_STOKED_SPARKSURFER, GIMMICK_Z_MOVE, FALSE},
    {SPECIES_WOBBUFFET, ITEM_NONE, MOVE_POUND, MOVE_MAX_STRIKE, GIMMICK_DYNAMAX, FALSE},
    {SPECIES_CHARIZARD, ITEM_NONE, MOVE_EMBER, MOVE_G_MAX_WILDFIRE, GIMMICK_DYNAMAX, TRUE},
};

TEST("Gimmick move copy: every Z Max and G-Max ID is banned from copying and calling")
{
    for (enum Move move = FIRST_Z_MOVE; move <= LAST_MAX_MOVE; move++)
    {
        EXPECT(IsMoveMimicBanned(move));
        EXPECT(IsMoveSketchBanned(move));
        EXPECT(IsMoveMirrorMoveBanned(move));
        EXPECT(IsMoveCopycatBanned(move));
        EXPECT(IsMoveMeFirstBanned(move));
        EXPECT(IsMoveMetronomeBanned(move));
        EXPECT(IsMoveAssistBanned(move));
        EXPECT(IsMoveSleepTalkBanned(move));
        EXPECT(IsMoveInstructBanned(move));
    }
}

TEST("Gimmick move copy: ordinary moves retain their individual ban flags")
{
    for (enum Move move = MOVE_NONE; move < MOVES_COUNT; move++)
    {
        EXPECT_EQ(IsMoveMimicBanned(move), gMovesInfo[move].mimicBanned);
        EXPECT_EQ(IsMoveSketchBanned(move), gMovesInfo[move].sketchBanned);
        EXPECT_EQ(IsMoveMirrorMoveBanned(move), gMovesInfo[move].mirrorMoveBanned);
        EXPECT_EQ(IsMoveCopycatBanned(move), gMovesInfo[move].copycatBanned);
        EXPECT_EQ(IsMoveMeFirstBanned(move), gMovesInfo[move].meFirstBanned);
        EXPECT_EQ(IsMoveMetronomeBanned(move), gMovesInfo[move].metronomeBanned);
        EXPECT_EQ(IsMoveAssistBanned(move), gMovesInfo[move].assistBanned);
        EXPECT_EQ(IsMoveSleepTalkBanned(move), gMovesInfo[move].sleepTalkBanned);
        EXPECT_EQ(IsMoveInstructBanned(move), gMovesInfo[move].instructBanned);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Mimic and Sketch cannot learn an opponent's actual Z Max or G-Max move")
{
    u32 index = 0;
    enum Move copyMove = MOVE_MIMIC;
    for (u32 c = 0; c < 2; c++)
        for (u32 s = 0; s < ARRAY_COUNT(sSpecialMoves); s++)
            PARAMETRIZE { copyMove = c == 0 ? MOVE_MIMIC : MOVE_SKETCH; index = s; }
    GIVEN {
        PLAYER(SPECIES_SMEARGLE) { Speed(1); MaxHP(500); HP(500); Defense(500); SpDefense(500); Moves(copyMove, MOVE_CELEBRATE); }
        OPPONENT(sSpecialMoves[index].species) { Speed(2); Item(sSpecialMoves[index].item); if (sSpecialMoves[index].gigantamax) GigantamaxFactor(TRUE); }
    } WHEN {
        TURN { MOVE(opponent, sSpecialMoves[index].baseMove, gimmick: sSpecialMoves[index].gimmick); MOVE(player, copyMove); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, sSpecialMoves[index].specialMove, opponent);
        NOT ANIMATION(ANIM_TYPE_MOVE, copyMove, player);
    } THEN {
        EXPECT_EQ(player->moves[0], copyMove);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE1), copyMove);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: opponents cannot learn the player's special move either")
{
    enum Move copyMove = MOVE_MIMIC;
    PARAMETRIZE { copyMove = MOVE_MIMIC; }
    PARAMETRIZE { copyMove = MOVE_SKETCH; }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_NORMALIUM_Z); }
        OPPONENT(SPECIES_SMEARGLE) { Speed(1); MaxHP(500); HP(500); Defense(500); Moves(copyMove); }
    } WHEN {
        TURN { MOVE(player, MOVE_POUND, gimmick: GIMMICK_Z_MOVE); MOVE(opponent, copyMove); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, player);
        NOT ANIMATION(ANIM_TYPE_MOVE, copyMove, opponent);
    } THEN {
        EXPECT_EQ(opponent->moves[0], copyMove);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_MOVE1), copyMove);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: ordinary Mimic is temporary and ordinary Sketch remains permanent")
{
    enum Move copyMove = MOVE_MIMIC;
    PARAMETRIZE { copyMove = MOVE_MIMIC; }
    PARAMETRIZE { copyMove = MOVE_SKETCH; }
    GIVEN {
        PLAYER(SPECIES_SMEARGLE) { Speed(1); Moves(copyMove); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_POUND); MOVE(player, copyMove); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, opponent);
        ANIMATION(ANIM_TYPE_MOVE, copyMove, player);
    } THEN {
        EXPECT_EQ(player->moves[0], MOVE_POUND);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE1), copyMove == MOVE_SKETCH ? MOVE_POUND : MOVE_MIMIC);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: a previously stored special move cannot execute without its gimmick")
{
    GIVEN {
        PLAYER(SPECIES_SMEARGLE);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
    } THEN {
        gBattlerAttacker = B_BATTLER_0;
        gBattlerTarget = B_BATTLER_1;
        SetActiveGimmick(gBattlerAttacker, GIMMICK_NONE);
        for (u32 index = 0; index < ARRAY_COUNT(sSpecialMoves); index++)
        {
            gCurrentMove = gChosenMove = sSpecialMoves[index].specialMove;
            gBattleStruct->eventState.atkCanceler = CANCELER_Z_MOVES;
            EXPECT_EQ(DoAttackCanceler(), CANCELER_RESULT_FAILURE);
        }
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Mirror Move and Copycat cannot borrow an opponent's Z move")
{
    enum Move copyMove = MOVE_MIRROR_MOVE;
    PARAMETRIZE { copyMove = MOVE_MIRROR_MOVE; }
    PARAMETRIZE { copyMove = MOVE_COPYCAT; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); Item(ITEM_NORMALIUM_Z); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_POUND, gimmick: GIMMICK_Z_MOVE); MOVE(player, copyMove); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, player);
            HP_BAR(opponent);
        }
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Copycat copies a Max move's base move as an ordinary move")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_POUND, gimmick: GIMMICK_DYNAMAX); MOVE(player, MOVE_COPYCAT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Me First fails against a queued Z or Max move")
{
    enum Gimmick gimmick = GIMMICK_Z_MOVE;
    PARAMETRIZE { gimmick = GIMMICK_Z_MOVE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); if (gimmick == GIMMICK_Z_MOVE) Item(ITEM_NORMALIUM_Z); }
    } WHEN {
        TURN { MOVE(player, MOVE_ME_FIRST); MOVE(opponent, MOVE_POUND, gimmick: gimmick); }
    } SCENE {
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
            HP_BAR(opponent);
        }
        if (gimmick == GIMMICK_Z_MOVE)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, opponent);
        else
            ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, opponent);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Assist rejects special IDs left in another party member's moves")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_ASSIST); }
        PLAYER(SPECIES_SMEARGLE) { Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ASSIST); }
    } SCENE {
    } THEN {
        gBattlerAttacker = B_BATTLER_0;
        gBattlerTarget = B_BATTLER_1;
        for (u32 index = 0; index < ARRAY_COUNT(sSpecialMoves); index++)
        {
            SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][1], sSpecialMoves[index].specialMove, 0);
            gCurrentMove = gChosenMove = MOVE_ASSIST;
            gBattleStruct->eventState.atkCanceler = CANCELER_CALLSUBMOVE;
            DoAttackCanceler();
            EXPECT_EQ((u32)gBattleStruct->submoveAnnouncement, SUBMOVE_FAILURE);
        }
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Sleep Talk rejects special IDs left in its move slots")
{
    GIVEN {
        PLAYER(SPECIES_SMEARGLE) { Moves(MOVE_SLEEP_TALK, MOVE_POUND, MOVE_SKETCH, MOVE_MIMIC); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); }
    } SCENE {
    } THEN {
        gBattlerAttacker = B_BATTLER_0;
        gBattlerTarget = B_BATTLER_1;
        player->status1 = STATUS1_SLEEP_TURN(3);
        for (u32 index = 0; index < ARRAY_COUNT(sSpecialMoves); index++)
        {
            player->moves[1] = sSpecialMoves[index].specialMove;
            gCurrentMove = gChosenMove = MOVE_SLEEP_TALK;
            gBattleStruct->eventState.atkCanceler = CANCELER_CALLSUBMOVE;
            DoAttackCanceler();
            EXPECT_EQ((u32)gBattleStruct->submoveAnnouncement, SUBMOVE_FAILURE);
        }
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: the caller's own Z-Power still converts an ordinary called move")
{
    enum Move copyMove = MOVE_MIRROR_MOVE;
    PARAMETRIZE { copyMove = MOVE_MIRROR_MOVE; }
    PARAMETRIZE { copyMove = MOVE_COPYCAT; }
    PARAMETRIZE { copyMove = MOVE_SLEEP_TALK; }
    PARAMETRIZE { copyMove = MOVE_ME_FIRST; }
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_WOBBUFFET) {
            Item(copyMove == MOVE_MIRROR_MOVE ? ITEM_FLYINIUM_Z : ITEM_NORMALIUM_Z);
            Speed(copyMove == MOVE_ME_FIRST ? 3 : 1);
            Moves(copyMove, MOVE_POUND);
            if (copyMove == MOVE_SLEEP_TALK)
                Status1(STATUS1_SLEEP_TURN(3));
        }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_POUND); MOVE(player, copyMove, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, player);
    } THEN {
        EXPECT(HasTrainerUsedGimmick(B_BATTLER_0, GIMMICK_Z_MOVE));
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE1), copyMove);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE2), MOVE_POUND);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Z-Copycat converts the ordinary base of a Max move using its own Z-Power")
{
    GIVEN {
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Item(ITEM_NORMALIUM_Z); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_POUND, gimmick: GIMMICK_DYNAMAX); MOVE(player, MOVE_COPYCAT, gimmick: GIMMICK_Z_MOVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_ZMOVE_ACTIVATE, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BREAKNECK_BLITZ, player);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_STRIKE, player);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Copycat does not bypass a base move's ban through Max Guard")
{
    GIVEN {
        ASSUME(IsMoveCopycatBanned(MOVE_PROTECT));
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_PROTECT, gimmick: GIMMICK_DYNAMAX); MOVE(player, MOVE_COPYCAT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, opponent);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_PROTECT, player);
            ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, player);
        }
    } THEN {
        // The end-of-move script clears submoveAnnouncement; inspect the resolver directly.
        gBattlerAttacker = B_BATTLER_0;
        gBattlerTarget = B_BATTLER_1;
        gCurrentMove = gChosenMove = MOVE_COPYCAT;
        gLastUsedMove = MOVE_MAX_GUARD;
        gBattleStruct->dynamax.lastUsedBaseMove = MOVE_PROTECT;
        gBattleStruct->eventState.atkCanceler = CANCELER_CALLSUBMOVE;
        DoAttackCanceler();
        EXPECT_EQ((u32)gBattleStruct->submoveAnnouncement, SUBMOVE_FAILURE);
    }
}

SINGLE_BATTLE_TEST("Gimmick move copy: Copycat can still call Trick Room after Max Guard")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TRICK_ROOM, gimmick: GIMMICK_DYNAMAX); MOVE(player, MOVE_COPYCAT); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TRICK_ROOM, player);
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_MAX_GUARD, player);
    } THEN {
        EXPECT(gFieldTimers.trickRoomTimer);
    }
}

DOUBLE_BATTLE_TEST("Gimmick move copy: Instruct cannot repeat an ally's Z Max or G-Max move")
{
    u32 index = 0;
    for (u32 s = 0; s < ARRAY_COUNT(sSpecialMoves); s++)
        PARAMETRIZE { index = s; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); MaxHP(500); HP(500); Defense(500); SpDefense(500); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(sSpecialMoves[index].species) { Speed(4); Item(sSpecialMoves[index].item); if (sSpecialMoves[index].gigantamax) GigantamaxFactor(TRUE); }
        OPPONENT(SPECIES_ORANGURU) { Speed(3); }
    } WHEN {
        TURN {
            MOVE(opponentLeft, sSpecialMoves[index].baseMove, gimmick: sSpecialMoves[index].gimmick, target: playerLeft);
            MOVE(opponentRight, MOVE_INSTRUCT, target: opponentLeft);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, sSpecialMoves[index].specialMove, opponentLeft);
        NONE_OF {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_INSTRUCT, opponentRight);
            ANIMATION(ANIM_TYPE_MOVE, sSpecialMoves[index].specialMove, opponentLeft);
        }
    }
}
