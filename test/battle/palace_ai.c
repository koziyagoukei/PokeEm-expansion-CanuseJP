#include "global.h"
#include "test/battle.h"
#include "battle_ai_main.h"
#include "battle_controllers.h"
#include "battle_gfx_sfx_util.h"
#include "battle_gimmick.h"
#include "battle_setup.h"
#include "battle_terastal.h"
#include "event_data.h"
#include "random.h"
#include "constants/battle_frontier.h"

static void ExpectPalaceAttack(enum Move revealed, enum Move expected, u32 level, enum Type defenderType)
{
    const enum BattlerId attacker = B_BATTLER_0;
    const enum BattlerId defender = B_BATTLER_1;
    u32 savedFlags = gBattleTypeFlags;
    u16 savedTrainer = TRAINER_BATTLE_PARAM.opponentA;
    u8 savedPlayerController = gBattlerBattleController[attacker];
    u8 savedOpponentController = gBattlerBattleController[defender];
    rng_value_t savedRng = gRngValue;
    u32 savedLevel = VarGet(VAR_FRONTIER_AI_LEVEL);
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_PALACE;
    TRAINER_BATTLE_PARAM.opponentA = 1;
    gBattlerBattleController[attacker] = BATTLE_CONTROLLER_PLAYER;
    gBattlerBattleController[defender] = BATTLE_CONTROLLER_OPPONENT;
    VarSet(VAR_FRONTIER_AI_LEVEL, level);
    BattleAI_SetupFlags();
    EXPECT_EQ(gAiThinkingStruct->aiFlags[attacker], AI_FLAG_BASIC_TRAINER);
    EXPECT_EQ(gAiThinkingStruct->aiFlags[defender], AI_FLAG_BASIC_TRAINER);
    struct ChooseMoveStruct *info = (struct ChooseMoveStruct *)&gBattleResources->bufferA[attacker][4];
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        enum Move move = gBattleMons[attacker].moves[i];
        info->moves[i] = move;
        info->currentPP[i] = gBattleMons[attacker].pp[i];
        gBattleHistory->usedMoves[attacker][i] = move == revealed ? move : MOVE_NONE;
        gAiPartyData->mons[B_TRAINER_PLAYER][gBattlerPartyIndexes[attacker]].moves[i] = gBattleHistory->usedMoves[attacker][i];
    }
    if (defenderType != TYPE_NONE)
    {
        SetActiveGimmick(defender, GIMMICK_TERA);
        EXPECT_EQ(GetBattlerTeraType(defender), defenderType);
    }
    gBattleWeather = B_WEATHER_NONE;
    gFieldTimers.terrain = B_TERRAIN_NONE;
    gBattleStruct->palaceFlags = 0;
    SetAiLogicDataForTurn(gAiLogicData);

    // Reuse the normal selector and only count the nature rolls that ask for an attack.
    const u8 cutoff = gNaturesInfo[GetNatureFromPersonality(gBattleMons[attacker].personality)].battlePalacePercents[0];
    u32 attackRolls = 0;
    for (u32 seed = 0; seed < 16; seed++)
    {
        SeedRng(seed);
        rng_value_t copy = gRngValue;
        if (LocalRandom(&copy) % 100 >= cutoff)
            continue;
        u32 chosen = ChooseMoveAndTargetInBattlePalace(attacker) & RET_MOVE_POSITION_MASK;
        EXPECT(!gProtectStructs[attacker].palaceUnableToUseMove);
        EXPECT_EQ(info->moves[chosen], expected);
        attackRolls++;
    }
    EXPECT_GT(attackRolls, 0);
    EXPECT(!gAiLogicData->palacePlayerAi);
    EXPECT_EQ(gBattleHistory->usedMoves[attacker][0],
        info->moves[0] == revealed ? revealed : MOVE_NONE);
    gBattleTypeFlags = savedFlags;
    TRAINER_BATTLE_PARAM.opponentA = savedTrainer;
    gBattlerBattleController[attacker] = savedPlayerController;
    gBattlerBattleController[defender] = savedOpponentController;
    VarSet(VAR_FRONTIER_AI_LEVEL, savedLevel);
    gRngValue = savedRng;
}

SINGLE_BATTLE_TEST("Palace AI: Celesteela vs Galarian Articuno")
{
    GIVEN {
        PLAYER(SPECIES_CELESTEELA) {
            Level(50); MaxHP(201); HP(137); Nature(NATURE_ADAMANT); Attack(142);
            Item(ITEM_LEFTOVERS); Moves(MOVE_HEAVY_SLAM, MOVE_STOMPING_TANTRUM, MOVE_FLY, MOVE_LEECH_SEED);
        }
        OPPONENT(SPECIES_ARTICUNO_GALAR) { Level(50); MaxHP(170); HP(85); Defense(105); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } THEN {
        ExpectPalaceAttack(MOVE_FLY, MOVE_HEAVY_SLAM, FRONTIER_AI_LEVEL_NORMAL, TYPE_NONE);
    }
}

SINGLE_BATTLE_TEST("Palace AI: Celesteela vs Qwilfish")
{
    GIVEN {
        PLAYER(SPECIES_CELESTEELA) {
            Level(50); MaxHP(201); HP(131); Nature(NATURE_ADAMANT); Attack(142);
            Item(ITEM_LEFTOVERS); Moves(MOVE_HEAVY_SLAM, MOVE_STOMPING_TANTRUM, MOVE_FLY, MOVE_LEECH_SEED);
        }
        OPPONENT(SPECIES_QWILFISH) { Level(50); MaxHP(150); HP(150); Defense(105); Ability(ABILITY_INTIMIDATE); }
    } WHEN {
        TURN { MOVE(player, MOVE_LEECH_SEED); }
    } THEN {
        ExpectPalaceAttack(MOVE_FLY, MOVE_STOMPING_TANTRUM, FRONTIER_AI_LEVEL_NORMAL, TYPE_NONE);
    }
}

SINGLE_BATTLE_TEST("Palace AI: Chien-Pao vs Greedent on normal and strongest")
{
    GIVEN {
        PLAYER(SPECIES_CHIEN_PAO) {
            Level(50); MaxHP(155); HP(127); Nature(NATURE_JOLLY); Attack(172);
            Moves(MOVE_ICE_SPINNER, MOVE_CRUNCH, MOVE_SACRED_SWORD, MOVE_PROTECT);
        }
        OPPONENT(SPECIES_GREEDENT) { Level(50); MaxHP(227); HP(170); Defense(115); TeraType(TYPE_STEEL); }
    } WHEN {
        TURN { MOVE(player, MOVE_PROTECT); }
    } THEN {
        ExpectPalaceAttack(MOVE_ICE_SPINNER, MOVE_CRUNCH, FRONTIER_AI_LEVEL_NORMAL, TYPE_STEEL);
        ExpectPalaceAttack(MOVE_ICE_SPINNER, MOVE_CRUNCH, FRONTIER_AI_LEVEL_STRONGEST, TYPE_STEEL);
    }
}



static void SetPalaceMoveBuffer(enum BattlerId battler, u32 nature)
{
    struct ChooseMoveStruct *info = (struct ChooseMoveStruct *)&gBattleResources->bufferA[battler][4];
    gBattleMons[battler].personality = nature;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        info->moves[i] = gBattleMons[battler].moves[i];
        info->currentPP[i] = gBattleMons[battler].pp[i];
    }
}

SINGLE_BATTLE_TEST("Palace AI: a disabled move triggers the existing fallback roll")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Moves(MOVE_POUND, MOVE_RECOVER, MOVE_THUNDER_WAVE); }
        OPPONENT(SPECIES_PIKACHU);
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); }
    } THEN {
        enum BattlerId battler = B_BATTLER_0;
        gBattleTypeFlags |= BATTLE_TYPE_PALACE;
        gBattleStruct->palaceFlags = 0;
        SetPalaceMoveBuffer(battler, NATURE_TIMID);
        gBattleMons[battler].volatiles.disabledMove = MOVE_RECOVER;
        u32 fallbackAttacks = 0, fallbackSupport = 0, idle = 0;
        rng_value_t savedRng = gRngValue;
        for (u32 seed = 0; seed < 240; seed++)
        {
            SeedRng(seed);
            rng_value_t copy = gRngValue;
            u32 roll = LocalRandom(&copy) % 100;
            if (roll < 62 || roll >= 72)
                continue;
            gProtectStructs[battler].palaceUnableToUseMove = FALSE;
            u32 moveIndex = ChooseMoveAndTargetInBattlePalace(battler) & RET_MOVE_POSITION_MASK;
            if (gProtectStructs[battler].palaceUnableToUseMove)
                idle++;
            else if (moveIndex == 0)
                fallbackAttacks++;
            else if (moveIndex == 2)
                fallbackSupport++;
            else
                EXPECT(FALSE);
        }
        gRngValue = savedRng;
        EXPECT_GT(fallbackAttacks, 0);
        EXPECT_GT(fallbackSupport, 0);
        EXPECT_GT(idle, 0);
    }
}

SINGLE_BATTLE_TEST("Palace AI: choosing a move preserves the other battler's inability to act")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Moves(MOVE_POUND); }
        OPPONENT(SPECIES_PIKACHU) { Moves(MOVE_THUNDERBOLT); }
    } WHEN {
        TURN { MOVE(player, MOVE_POUND); MOVE(opponent, MOVE_THUNDERBOLT); }
    } THEN {
        gProtectStructs[B_BATTLER_0].palaceUnableToUseMove = TRUE;
        gProtectStructs[B_BATTLER_0].laggingTail = TRUE;
        EXPECT_EQ(BattleAI_ChoosePalaceMoveIndex(B_BATTLER_1, B_BATTLER_0, 1), 0);
        EXPECT(gProtectStructs[B_BATTLER_0].palaceUnableToUseMove);
        EXPECT(gProtectStructs[B_BATTLER_0].laggingTail);
    }
}

SINGLE_BATTLE_TEST("Palace AI: the player's own ability is applied in damage estimates")
{
    GIVEN {
        PLAYER(SPECIES_HAXORUS) {
            Level(50); Attack(170); Ability(ABILITY_MOLD_BREAKER); Moves(MOVE_EARTHQUAKE, MOVE_SCRATCH);
        }
        OPPONENT(SPECIES_ROTOM_HEAT) { Level(50); Defense(130); HP(500); MaxHP(500); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } THEN {
        ExpectPalaceAttack(MOVE_SCRATCH, MOVE_EARTHQUAKE, FRONTIER_AI_LEVEL_NORMAL, TYPE_NONE);
    }
}

DOUBLE_BATTLE_TEST("Palace AI: chooses and scores the move against the same foe")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU) { Moves(MOVE_THUNDERBOLT, MOVE_GRASS_KNOT); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GYARADOS) { HP(400); MaxHP(400); }
        OPPONENT(SPECIES_GASTRODON) { HP(300); MaxHP(300); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_THUNDERBOLT, target: opponentLeft); }
    } THEN {
        gBattleTypeFlags |= BATTLE_TYPE_PALACE;
        gBattlerBattleController[B_BATTLER_0] = BATTLE_CONTROLLER_PLAYER;
        gBattlerBattleController[B_BATTLER_1] = BATTLE_CONTROLLER_OPPONENT;
        gBattlerBattleController[B_BATTLER_3] = BATTLE_CONTROLLER_OPPONENT;
        gAiThinkingStruct->aiFlags[B_BATTLER_0] = AI_FLAG_BASIC_TRAINER;
        gBattleStruct->palaceFlags = 0;
        opponentLeft->hp = opponentLeft->maxHP;
        opponentRight->hp = opponentRight->maxHP;
        SetPalaceMoveBuffer(B_BATTLER_0, NATURE_TIMID);
        rng_value_t savedRng = gRngValue;
        u32 checked = 0;
        for (u32 seed = 0; seed < 8; seed++)
        {
            SeedRng(seed);
            rng_value_t copy = gRngValue;
            if (LocalRandom(&copy) % 100 >= 62)
                continue;
            // Timid targets the foe with less HP. Gastrodon is immune to Electric moves.
            u32 choice = ChooseMoveAndTargetInBattlePalace(B_BATTLER_0);
            EXPECT_EQ(choice & RET_MOVE_POSITION_MASK, 1);
            EXPECT_EQ((choice >> RET_TARGET_SHIFT) & 0xFF, B_BATTLER_3);
            checked++;
        }
        EXPECT_GT(checked, 0);
        gRngValue = savedRng;
    }
}

SINGLE_BATTLE_TEST("Palace AI: the player keeps its own held item in damage estimates")
{
    GIVEN {
        PLAYER(SPECIES_PIKACHU) {
            Level(50); SpAttack(100); Item(ITEM_LIGHT_BALL); Moves(MOVE_THUNDERBOLT, MOVE_THUNDER_SHOCK);
        }
        OPPONENT(SPECIES_SNORLAX) { Level(50); SpDefense(100); HP(95); MaxHP(95); }
    } WHEN {
        TURN { MOVE(player, MOVE_THUNDER_SHOCK); }
    } THEN {
        // Light Ball doubles the estimate and makes Thunderbolt a one-hit KO.
        u32 savedFlags = gBattleTypeFlags;
        u16 savedTrainer = TRAINER_BATTLE_PARAM.opponentA;
        u8 savedController = gBattlerBattleController[B_BATTLER_0];
        gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_PALACE;
        TRAINER_BATTLE_PARAM.opponentA = 1;
        gBattlerBattleController[B_BATTLER_0] = BATTLE_CONTROLLER_PLAYER;
        gAiPartyData->mons[B_TRAINER_PLAYER][0].heldEffect = HOLD_EFFECT_NONE;
        gAiThinkingStruct->aiFlags[B_BATTLER_0] = AI_FLAG_BASIC_TRAINER;
        SetAiLogicDataForTurn(gAiLogicData);
        EXPECT_EQ(gAiLogicData->holdEffects[B_BATTLER_0], HOLD_EFFECT_NONE);
        player->hp = player->maxHP;
        opponent->hp = opponent->maxHP;
        EXPECT_EQ(BattleAI_ChoosePalaceMoveIndex(B_BATTLER_0, B_BATTLER_1, 1u), 0);
        s32 withItem = gAiBattleData->finalScore[B_BATTLER_0][B_BATTLER_1][0];
        EXPECT_EQ(player->item, ITEM_LIGHT_BALL);
        player->item = ITEM_NONE;
        EXPECT_EQ(BattleAI_ChoosePalaceMoveIndex(B_BATTLER_0, B_BATTLER_1, 1u), 0);
        EXPECT_GT(withItem, gAiBattleData->finalScore[B_BATTLER_0][B_BATTLER_1][0]);
        player->item = ITEM_LIGHT_BALL;
        EXPECT_EQ(gAiLogicData->holdEffects[B_BATTLER_0], HOLD_EFFECT_NONE);
        EXPECT(!gAiLogicData->palacePlayerAi);
        gBattlerBattleController[B_BATTLER_0] = savedController;
        TRAINER_BATTLE_PARAM.opponentA = savedTrainer;
        gBattleTypeFlags = savedFlags;
    }
}
