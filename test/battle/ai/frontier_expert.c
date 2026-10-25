#include "global.h"
#include "test/battle.h"
#include "battle_ai_frontier.h"
#include "battle_ai_util.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "event_data.h"
#include "frontier_guest.h"
#include "constants/battle_frontier.h"
#include "constants/trainers.h"

#define EXPERT_AI_FLAGS (AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT | AI_FLAG_OMNISCIENT | AI_FLAG_FRONTIER_EXPERT)

// Turns the AI test battle into a Battle Tower single battle against the given trainer.
// Test battles are replayed as recorded battles, so the recording's flags are changed.
static void SetUpTowerBattle(u16 opponent)
{
    gBattleTestRunnerState->data.recordedBattle.battleFlags |= BATTLE_TYPE_BATTLE_TOWER;
    gBattleTestRunnerState->data.recordedBattle.opponentA = opponent;
    gFacilityTrainers = gBattleFrontierTrainers;
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
}

// The usual AI only sometimes heals when it is faster and not in danger.
AI_SINGLE_BATTLE_TEST("Frontier expert AI: recovers when healing outpaces the damage it takes")
{
    PASSES_RANDOMLY(100, 100, RNG_AI_SHOULD_RECOVER);
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_RECOVER) == EFFECT_RESTORE_HP);
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_LINOONE) { Speed(2); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(5); HP(200); MaxHP(400); Moves(MOVE_TACKLE, MOVE_RECOVER); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); EXPECT_MOVE(opponent, MOVE_RECOVER); }
    }
}

// At 60% HP the usual AI adds nothing for Recover, so only the race check keeps it attacking.
AI_SINGLE_BATTLE_TEST("Frontier expert AI: keeps attacking when it already wins the damage race")
{
    GIVEN {
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Speed(2); HP(450); MaxHP(450); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_GARCHOMP) { Speed(5); HP(240); MaxHP(400); Moves(MOVE_EARTHQUAKE, MOVE_RECOVER); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); EXPECT_MOVE(opponent, MOVE_EARTHQUAKE); }
    }
}

// Max Hailstorm would take more than the Substitute costs; the usual AI scores both moves the same.
AI_SINGLE_BATTLE_TEST("Frontier expert AI: sets up a Substitute against a Dynamaxed target when faster")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SUBSTITUTE) == EFFECT_SUBSTITUTE);
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Speed(1); Moves(MOVE_ICE_BEAM); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(1); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_GARCHOMP) { Speed(100); Moves(MOVE_EARTHQUAKE, MOVE_SUBSTITUTE); }
    } WHEN {
        TURN { MOVE(player, MOVE_ICE_BEAM, gimmick: GIMMICK_DYNAMAX); EXPECT_SEND_OUT(opponent, 1); }
        TURN { MOVE(player, MOVE_ICE_BEAM); EXPECT_MOVE(opponent, MOVE_SUBSTITUTE); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier expert AI: wears down Wonder Guard with status, hazards and phazing")
{
    enum Move aiMove = MOVE_NONE;
    PARAMETRIZE { aiMove = MOVE_TOXIC; }
    PARAMETRIZE { aiMove = MOVE_STEALTH_ROCK; }
    PARAMETRIZE { aiMove = MOVE_ROAR; }
    GIVEN {
        ASSUME(GetSpeciesAbility(SPECIES_SHEDINJA, 0) == ABILITY_WONDER_GUARD);
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_SHEDINJA) { Moves(MOVE_SCRATCH); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_WATER_GUN, MOVE_SWORDS_DANCE, aiMove); }
    } WHEN {
        if (aiMove == MOVE_TOXIC) // The poison knocks Shedinja out at the end of the turn.
            TURN { MOVE(player, MOVE_SCRATCH); EXPECT_MOVE(opponent, aiMove); SEND_OUT(player, 1); }
        else
            TURN { MOVE(player, MOVE_SCRATCH); EXPECT_MOVE(opponent, aiMove); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier expert AI: saves Dynamax for an attacking turn while setting up")
{
    GIVEN {
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SWORDS_DANCE); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE, gimmick: GIMMICK_NONE); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier expert AI: Dynamaxes to survive when its last Pokémon is threatened")
{
    GIVEN {
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Moves(MOVE_SWORDS_DANCE); DynamaxLevel(10); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE, gimmick: GIMMICK_DYNAMAX); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier expert AI: saves Terastallization for an attacking turn while setting up")
{
    GIVEN {
        AI_FLAGS(EXPERT_AI_FLAGS);
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_SWORDS_DANCE); TeraType(TYPE_NORMAL); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); EXPECT_MOVE(opponent, MOVE_SWORDS_DANCE, gimmick: GIMMICK_NONE); }
    }
}

AI_SINGLE_BATTLE_TEST("Frontier AI forfeits when none of its moves can hurt the target")
{
    GIVEN {
        SetUpTowerBattle(1);
        VAR_SET(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_GENGAR) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ZIGZAGOON) { Moves(MOVE_TACKLE, MOVE_HEADBUTT); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
    }
}

AI_SINGLE_BATTLE_TEST("Frontier AI forfeits when it is surely knocked out before it can move")
{
    enum Move aiMove = MOVE_NONE;
    bool32 forfeits = FALSE;
    PARAMETRIZE { aiMove = MOVE_TACKLE; forfeits = TRUE; }
    PARAMETRIZE { aiMove = MOVE_QUICK_ATTACK; forfeits = FALSE; }
    GIVEN {
        ASSUME(GetMovePriority(MOVE_QUICK_ATTACK) > 0);
        SetUpTowerBattle(1);
        VAR_SET(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_WOBBUFFET) { Speed(100); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(1); Moves(aiMove); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } THEN {
        EXPECT_EQ(gBattleOutcome == B_OUTCOME_WON && gBattleMons[B_BATTLER_1].hp != 0, forfeits);
    }
}

AI_SINGLE_BATTLE_TEST("Frontier AI does not forfeit outside the Frontier, against the 7th battle's named trainer or a Frontier Brain")
{
    u16 trainerId = 0;
    PARAMETRIZE { trainerId = 0; } // Not a Frontier battle.
    PARAMETRIZE { trainerId = TRAINER_FRONTIER_NORMAN; }
    PARAMETRIZE { trainerId = TRAINER_FRONTIER_BRAIN; }
    GIVEN {
        if (trainerId != 0)
            SetUpTowerBattle(trainerId);
        VAR_SET(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE);
        PLAYER(SPECIES_GENGAR) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_ZIGZAGOON) { Moves(MOVE_TACKLE, MOVE_HEADBUTT); }
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, player);
    } THEN {
        EXPECT_NE(gBattleOutcome, B_OUTCOME_WON);
    }
}

TEST("Frontier AI never forfeits for the player's side or outside single Frontier trainer battles")
{
    static const struct { u32 flags; u16 opponent; } cases[] = {
        { BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER, TRAINER_FRONTIER_BRAIN },
        { BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER, TRAINER_FRONTIER_GUEST_FIRST },
        { BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_DOUBLE, 1 },
        { BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_LINK, 1 },
        { BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_RECORDED_LINK, 1 },
        { BATTLE_TYPE_TRAINER, 1 },
        { BATTLE_TYPE_BATTLE_TOWER, 1 },
    };

    for (u32 i = 0; i < ARRAY_COUNT(cases); i++)
    {
        gBattleTypeFlags = cases[i].flags;
        TRAINER_BATTLE_PARAM.opponentA = cases[i].opponent;
        EXPECT(!ShouldAiForfeitFrontierBattle(B_BATTLER_1));
    }
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    TRAINER_BATTLE_PARAM.opponentA = 1;
    EXPECT(!ShouldAiForfeitFrontierBattle(B_BATTLER_0));
    gBattleTypeFlags = 0;
    TRAINER_BATTLE_PARAM.opponentA = 0;
}
