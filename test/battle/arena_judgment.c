#include "global.h"
#include "battle_frontier.h"
#include "test/battle.h"

AI_SINGLE_BATTLE_TEST("Battle Arena: two consecutive tied judgments advance to the third pair")
{
    GIVEN {
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA;
        gBattleTestRunnerState->data.recordedBattle.opponentA = 0;
        gFacilityTrainers = gBattleFrontierTrainers;
        AI_FLAGS(0);
        for (u32 i = 0; i < 3; i++)
        {
            PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); Ability(ABILITY_SHADOW_TAG); }
            OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); Ability(ABILITY_SHADOW_TAG); }
        }
    } WHEN {
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); SEND_OUT(player, 1); EXPECT_SEND_OUT(opponent, 1); }
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); SEND_OUT(player, 2); EXPECT_SEND_OUT(opponent, 2); }
        TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 2);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 2);
        EXPECT_EQ(gBattleStruct->arenaLostPlayerMons, 3);
        EXPECT_EQ(gBattleStruct->arenaLostOpponentMons, 3);
        EXPECT_GT(gBattleStruct->arenaStartHp[0], 0);
        EXPECT_GT(gBattleStruct->arenaStartHp[1], 0);
        EXPECT_EQ(gBattleResources->battleScriptsStack->size, 0);
    }
}

AI_SINGLE_BATTLE_TEST("Battle Arena: a tied judgment on the final pair ends the battle")
{
    u32 count = 1;
    PARAMETRIZE { count = 1; }
    PARAMETRIZE { count = 3; }
    GIVEN {
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA;
        gBattleTestRunnerState->data.recordedBattle.opponentA = 0;
        gFacilityTrainers = gBattleFrontierTrainers;
        AI_FLAGS(0);
        for (u32 i = 0; i < count; i++)
        {
            PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
            OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        }
    } WHEN {
        for (u32 i = 0; i < count; i++)
        {
            TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
            TURN { MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH); }
            TURN {
                MOVE(player, MOVE_SPLASH); EXPECT_MOVE(opponent, MOVE_SPLASH);
                if (i + 1 < count)
                {
                    SEND_OUT(player, i + 1);
                    EXPECT_SEND_OUT(opponent, i + 1);
                }
            }
        }
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_DREW);
    }
}

AI_SINGLE_BATTLE_TEST("Battle Arena: one-sided judgments replace only the loser")
{
    bool32 playerWins = TRUE;
    PARAMETRIZE { playerWins = TRUE; }
    PARAMETRIZE { playerWins = FALSE; }
    GIVEN {
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA;
        gBattleTestRunnerState->data.recordedBattle.opponentA = 0;
        gFacilityTrainers = gBattleFrontierTrainers;
        AI_FLAGS(0);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(1000); HP(1000); Moves(playerWins ? MOVE_SCRATCH : MOVE_SPLASH); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(1000); HP(1000); Moves(playerWins ? MOVE_SPLASH : MOVE_SCRATCH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, playerWins ? MOVE_SCRATCH : MOVE_SPLASH); EXPECT_MOVE(opponent, playerWins ? MOVE_SPLASH : MOVE_SCRATCH); }
        TURN { MOVE(player, playerWins ? MOVE_SCRATCH : MOVE_SPLASH); EXPECT_MOVE(opponent, playerWins ? MOVE_SPLASH : MOVE_SCRATCH); }
        TURN {
            MOVE(player, playerWins ? MOVE_SCRATCH : MOVE_SPLASH); EXPECT_MOVE(opponent, playerWins ? MOVE_SPLASH : MOVE_SCRATCH);
            if (playerWins)
                EXPECT_SEND_OUT(opponent, 1);
            else
                SEND_OUT(player, 1);
        }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], !playerWins);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], playerWins);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: a tied judgment replaces both Dynamaxed Pokemon")
{
    enum Species species = SPECIES_WOBBUFFET;
    PARAMETRIZE { species = SPECIES_WOBBUFFET; }
    PARAMETRIZE { species = SPECIES_LAPRAS; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA;
        PLAYER(species) { Speed(2); Moves(MOVE_POUND); DynamaxLevel(10); GigantamaxFactor(species == SPECIES_LAPRAS); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_SPLASH); }
        OPPONENT(species) { Speed(1); Moves(MOVE_POUND); DynamaxLevel(10); GigantamaxFactor(species == SPECIES_LAPRAS); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); MOVE(opponent, MOVE_POUND); }
        TURN { MOVE(player, MOVE_POUND); MOVE(opponent, MOVE_POUND); }
        TURN { MOVE(player, MOVE_POUND, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_POUND, gimmick: GIMMICK_DYNAMAX); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 1);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 1);
        EXPECT_EQ(gBattleStruct->arenaLostPlayerMons, 1);
        EXPECT_EQ(gBattleStruct->arenaLostOpponentMons, 1);
        EXPECT_EQ((u32)gBattleStruct->battlerKOAnimsRunning, 0);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: Dynamax send-out animation preserves the judgment scores")
{
    bool32 opponentDynamaxes = FALSE;
    PARAMETRIZE { opponentDynamaxes = FALSE; }
    PARAMETRIZE { opponentDynamaxes = TRUE; }
    GIVEN {
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA;
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); DynamaxLevel(10); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); MOVE(opponent, MOVE_POUND); }
        TURN {
            MOVE(player, MOVE_POUND, gimmick: opponentDynamaxes ? GIMMICK_NONE : GIMMICK_DYNAMAX);
            MOVE(opponent, MOVE_POUND, gimmick: opponentDynamaxes ? GIMMICK_DYNAMAX : GIMMICK_NONE);
        }
    } THEN {
        EXPECT_EQ(gBattleStruct->arenaMindPoints[B_BATTLER_0], 2);
        EXPECT_EQ(gBattleStruct->arenaMindPoints[B_BATTLER_1], 2);
    }
}

SINGLE_BATTLE_TEST("Battle Arena: faint form changes cannot revive tied judgment losers")
{
    enum Species species = SPECIES_AEGISLASH_BLADE;
    enum Move move = MOVE_BULLET_PUNCH;
    enum Gimmick gimmick = GIMMICK_NONE;
    PARAMETRIZE { species = SPECIES_AEGISLASH_BLADE; move = MOVE_BULLET_PUNCH; gimmick = GIMMICK_NONE; }
    PARAMETRIZE { species = SPECIES_NECROZMA_DUSK_MANE; move = MOVE_POUND; gimmick = GIMMICK_ULTRA_BURST; }
    GIVEN {
        FORCE_MOVE_ANIM(TRUE);
        GIVE_PLAYER_ITEM(ITEM_Z_POWER_RING, 1);
        gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_ARENA;
        PLAYER(species) { Speed(2); Moves(move); Item(gimmick == GIMMICK_ULTRA_BURST ? ITEM_ULTRANECROZIUM_Z : ITEM_NONE); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); Moves(MOVE_SPLASH); }
        OPPONENT(species) { Speed(1); Moves(move); Item(gimmick == GIMMICK_ULTRA_BURST ? ITEM_ULTRANECROZIUM_Z : ITEM_NONE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_SPLASH); }
    } WHEN {
        TURN { MOVE(player, move, gimmick: gimmick); MOVE(opponent, move, gimmick: gimmick); }
        TURN { MOVE(player, move); MOVE(opponent, move); }
        TURN { MOVE(player, move); MOVE(opponent, move); SEND_OUT(player, 1); SEND_OUT(opponent, 1); }
        TURN { MOVE(player, MOVE_SPLASH); MOVE(opponent, MOVE_SPLASH); }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], 1);
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_1], 1);
        EXPECT_EQ(gBattleStruct->arenaLostPlayerMons, 1);
        EXPECT_EQ(gBattleStruct->arenaLostOpponentMons, 1);
        EXPECT_EQ((u32)gBattleStruct->battlerKOAnimsRunning, 0);
    }
}
