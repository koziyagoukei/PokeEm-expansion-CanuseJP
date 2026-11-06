#include "global.h"
#include "battle_dynamax.h"
#include "battle_gimmick.h"
#include "battle_util.h"
#include "battle_z_move.h"
#include "config_changes.h"
#include "event_data.h"
#include "load_save.h"
#include "pokerus.h"
#include "shadow_pokemon.h"
#include "test/battle.h"
#include "test/overworld_script.h"

TEST("Shadow: legacy Pokerus values and the old Shadow bit do not opt in")
{
    struct Pokemon mon;
    const u8 legacyShadowBit = TRUE;

    CreateMon(&mon, SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PLAYER_ID);
    SetMonData(&mon, MON_DATA_IS_SHADOW, &legacyShadowBit);
    for (u32 raw = 0; raw <= 0xFF; raw++)
    {
        u8 value = raw;
        SetMonData(&mon, MON_DATA_POKERUS, &value);
        EXPECT_EQ(GetShadowPokemonState(&mon), SHADOW_STATE_NORMAL);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_POKERUS), raw);
    }
}

TEST("Shadow: only the event purifies a ready mon, and party save/load retains both states")
{
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_PIKACHU, 50;
        givemon SPECIES_EEVEE, 50;
        setvar VAR_0x8004, 0;
        specialvar VAR_RESULT, Special_ShadowPokemon_Mark
    );
    struct Pokemon *party = gParties[B_TRAINER_PLAYER];
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT(MarkShadowPokemon(&party[1]));
    EXPECT(!PurifyShadowPokemon(&party[0]));
    AddShadowHeartGauge(&party[0], 100);
    AddShadowHeartGauge(&party[1], 3);
    EXPECT(IsShadowPokemon(&party[0]));
    EXPECT(CanPurifyShadowPokemon(&party[0]));
    EXPECT_EQ(GetShadowHeartGauge(&party[0]), 5);

    SavePlayerParty();
    ZeroPlayerPartyMons();
    LoadPlayerParty();
    EXPECT_EQ(GetShadowPokemonState(&party[0]), SHADOW_STATE_READY);
    EXPECT_EQ(GetShadowPokemonState(&party[1]), SHADOW_STATE_STAGE_3);
    RUN_OVERWORLD_SCRIPT(
        setvar VAR_0x8004, 0;
        specialvar VAR_RESULT, Special_ShadowPokemon_Purify
    );
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT(!IsShadowPokemon(&party[0]));
    EXPECT(!MarkShadowPokemon(&party[0]));

    SavePlayerParty();
    ZeroPlayerPartyMons();
    LoadPlayerParty();
    EXPECT_EQ(GetShadowPokemonState(&party[0]), SHADOW_STATE_PURIFIED);
}

TEST("Shadow: legacy infection, spread, day changes and raw writes leave tagged mons alone")
{
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_PIKACHU, 50;
        givemon SPECIES_EEVEE, 50
    );
    struct Pokemon *party = gParties[B_TRAINER_PLAYER];
    EXPECT(MarkShadowPokemon(&party[0]));
    EXPECT(!CheckMonHasHadPokerus(&party[0]));
    SetConfig(CONFIG_POKERUS_ENABLED, TRUE);
    EXPECT(!IsPokerusInParty());
    EXPECT(!ShouldPokemonShowActivePokerus(&party[0]));
    EXPECT(!ShouldPokemonShowCuredPokerus(&party[0]));
    SET_RNG(RNG_POKERUS_INFECTION, 0);
    RandomlyGivePartyPokerus();
    SET_RNG(RNG_POKERUS_SPREAD, 0);
    PartySpreadPokerus();
    UpdatePartyPokerusTime(10);
    const u8 zero = 0;
    SetMonData(&party[0], MON_DATA_POKERUS, &zero);
    EXPECT_EQ(GetShadowPokemonState(&party[0]), SHADOW_STATE_STAGE_0);
}

WILD_BATTLE_TEST("Shadow: Sudowoodo shows its aura on encounter and stays Shadow when caught")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_SUDOWOODO) { Shadow(TRUE); }
    } WHEN {
        TURN { USE_ITEM(player, ITEM_MASTER_BALL); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SHADOW_AURA, opponent);
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_CAUGHT);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), SPECIES_SUDOWOODO);
        EXPECT_EQ(GetShadowPokemonState(&gParties[B_TRAINER_PLAYER][1]), SHADOW_STATE_STAGE_0);
    }
}

SINGLE_BATTLE_TEST("Shadow: the Shadow move comes first and own moves open at stages 1 and 3 without replacing stored moves")
{
    u32 stage;
    PARAMETRIZE { stage = 0; }
    PARAMETRIZE { stage = 1; }
    PARAMETRIZE { stage = 2; }
    PARAMETRIZE { stage = 3; }
    PARAMETRIZE { stage = 4; }
    PARAMETRIZE { stage = 5; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(stage); Moves(MOVE_POUND, MOVE_TACKLE, MOVE_SCRATCH, MOVE_STRENGTH); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        static const enum Move moves[] = { MOVE_POUND, MOVE_TACKLE, MOVE_SCRATCH, MOVE_STRENGTH };
        // Wobbuffet has no row: Shadow Rush only. Its own moves follow it from slot 1.
        enum Move expected[MAX_MON_MOVES] = {
            MOVE_SHADOW_RUSH,
            stage >= 1 ? MOVE_TACKLE : MOVE_NONE,
            stage >= 3 ? MOVE_SCRATCH : MOVE_NONE,
            MOVE_NONE,
        };
        u32 sealed = (stage < 1 ? (1 << 1) : 0) | (stage < 3 ? (1 << 2) : 0) | (1 << 3);
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
        {
            EXPECT_EQ(player->moves[slot], expected[slot]);
            EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_MOVE1 + slot), moves[slot]);
        }
        EXPECT_EQ(GetBattlerShadowSealedSlots(B_BATTLER_0), sealed);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PP1), GetMovePP(MOVE_POUND));
    }
}

SINGLE_BATTLE_TEST("Shadow: a win advances only the sent-out mon and only once")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Speed(100); Moves(MOVE_POUND); }
        PLAYER(SPECIES_EEVEE) { Shadow(TRUE); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); }
    } THEN {
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
        EXPECT_EQ(GetShadowHeartGauge(&gParties[B_TRAINER_PLAYER][0]), 1);
        EXPECT_EQ(GetShadowHeartGauge(&gParties[B_TRAINER_PLAYER][1]), 0);
        AdvanceParticipatingShadowPokemon();
        EXPECT_EQ(GetShadowHeartGauge(&gParties[B_TRAINER_PLAYER][0]), 1);
    }
}

SINGLE_BATTLE_TEST("Shadow: switching out preserves Hyper Mode, and Shadow moves use no PP")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND); }
        PLAYER(SPECIES_EEVEE);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0, WITH_RNG(RNG_SHADOW_HYPER_MODE, TRUE)); }
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } THEN {
        EXPECT(IsBattlerInHyperMode(B_BATTLER_0));
        EXPECT_EQ(player->moves[0], MOVE_SHADOW_RUSH);
        EXPECT_EQ(player->pp[0], GetMovePP(MOVE_SHADOW_RUSH));
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PP1), GetMovePP(MOVE_POUND));
    }
}

SINGLE_BATTLE_TEST("Shadow: Hyper Mode coexists with Dynamax or Tera and can be calmed")
{
    enum Gimmick gimmick;
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); if (gimmick == GIMMICK_TERA) TeraType(TYPE_WATER); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0, gimmick: gimmick, WITH_RNG(RNG_SHADOW_HYPER_MODE, TRUE)); }
    } THEN {
        EXPECT(IsBattlerInHyperMode(B_BATTLER_0));
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), gimmick);
        EXPECT(CalmShadowPokemon(B_BATTLER_0));
        EXPECT(!CalmShadowPokemon(B_BATTLER_0));
        EXPECT(!IsBattlerInHyperMode(B_BATTLER_0));
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), gimmick);
    }
}

SINGLE_BATTLE_TEST("Shadow: a compatible stone or crystal does not enable Mega or Z")
{
    enum Item item;
    bool32 shadow;
    PARAMETRIZE { item = ITEM_CHARIZARDITE_X; shadow = FALSE; }
    PARAMETRIZE { item = ITEM_CHARIZARDITE_X; shadow = TRUE; }
    PARAMETRIZE { item = ITEM_NORMALIUM_Z; shadow = FALSE; }
    PARAMETRIZE { item = ITEM_NORMALIUM_Z; shadow = TRUE; }
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Shadow(shadow); Item(item); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); }
    } THEN {
        EXPECT_EQ(CanMegaEvolve(B_BATTLER_0), !shadow && item == ITEM_CHARIZARDITE_X);
        EXPECT_EQ(CanUseZMove(B_BATTLER_0), !shadow && item == ITEM_NORMALIUM_Z);
    }
}

SINGLE_BATTLE_TEST("Shadow: Dynaphobia sets Darkness and Protect does not stop its weather effect")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_PROTECT); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0, gimmick: GIMMICK_DYNAMAX); MOVE(opponent, MOVE_PROTECT); }
    } THEN {
        EXPECT_EQ(GetMaxMove(B_BATTLER_0, MOVE_SHADOW_RUSH), MOVE_DYNAPHOBIA);
        EXPECT_EQ(GetMaxMove(B_BATTLER_0, MOVE_SHADOW_MIST), MOVE_MAX_GUARD);
        EXPECT_EQ(gBattleWeather, B_WEATHER_DARKNESS);
        EXPECT_EQ(gBattleStruct->weatherDuration, 4);
        EXPECT_EQ(player->hp, player->maxHP);
        EXPECT_LT(opponent->hp, opponent->maxHP - opponent->maxHP / 16);
        EXPECT_EQ(GetDynamicMoveType(GetBattlerMon(B_BATTLER_1), MOVE_WEATHER_BALL, B_BATTLER_1,
                                    ABILITY_NONE, HOLD_EFFECT_NONE, MON_IN_BATTLE), TYPE_MYSTERY);
    }
}

DOUBLE_BATTLE_TEST("Shadow: Darkness damages a non-Shadow partner exactly five times, then fades")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND, MOVE_CELEBRATE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Defense(300); }
        OPPONENT(SPECIES_WOBBUFFET) { Defense(300); }
    } WHEN {
        TURN { MOVE(playerLeft, moveSlot: 0, target: opponentLeft, gimmick: GIMMICK_DYNAMAX); }
        TURN { MOVE(playerLeft, moveSlot: 0, target: opponentLeft); }
        TURN { MOVE(playerLeft, moveSlot: 0, target: opponentLeft); }
        TURN { MOVE(playerLeft, moveSlot: 0, target: opponentLeft); }
        TURN { MOVE(playerLeft, moveSlot: 0, target: opponentRight); }
        TURN { MOVE(playerLeft, moveSlot: 0, target: opponentRight); }
    } THEN {
        EXPECT_EQ(gBattleWeather, B_WEATHER_NONE);
        EXPECT_EQ(playerLeft->hp, playerLeft->maxHP);
        EXPECT_EQ(playerRight->hp, playerRight->maxHP - 5 * (playerRight->maxHP / 16));
    }
}

TEST("Shadow: a species row sets the Shadow moves; others get Shadow Rush and their type's Shadow move")
{
    enum Move moves[MAX_MON_MOVES];

    EXPECT_EQ(GetShadowMovesForSpecies(SPECIES_TEDDIURSA, moves), 1);
    EXPECT_EQ(moves[0], MOVE_SHADOW_RUSH);
    EXPECT_EQ(GetShadowMovesForSpecies(SPECIES_WOBBUFFET, moves), 1);
    EXPECT_EQ(moves[0], MOVE_SHADOW_RUSH);
    EXPECT_EQ(GetShadowMovesForSpecies(SPECIES_GROWLITHE, moves), 2);
    EXPECT_EQ(moves[1], MOVE_SHADOW_FIRE);
    EXPECT_EQ(GetShadowMovesForSpecies(SPECIES_PIKACHU, moves), 2);
    EXPECT_EQ(moves[1], MOVE_SHADOW_BOLT);
    EXPECT_EQ(GetShadowMovesForSpecies(SPECIES_SNORUNT, moves), 2);
    EXPECT_EQ(moves[1], MOVE_SHADOW_CHILL);
    for (enum Move move = FIRST_SHADOW_MOVE; move <= LAST_SHADOW_MOVE; move++)
        EXPECT(IsShadowMove(move));
}

SINGLE_BATTLE_TEST("Shadow: two Shadow moves push the own moves back by two slots")
{
    GIVEN {
        PLAYER(SPECIES_GROWLITHE) { ShadowStage(3); Moves(MOVE_EMBER, MOVE_BITE, MOVE_ROAR, MOVE_LEER); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(player->moves[0], MOVE_SHADOW_RUSH);
        EXPECT_EQ(player->moves[1], MOVE_SHADOW_FIRE);
        EXPECT_EQ(player->moves[2], MOVE_ROAR);
        EXPECT_EQ(player->moves[3], MOVE_LEER);
        EXPECT_EQ(GetBattlerShadowSealedSlots(B_BATTLER_0), 0);
    }
}

SINGLE_BATTLE_TEST("Shadow: a Trainer's Shadow Pokemon uses its own moves from the start")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND, MOVE_TACKLE, MOVE_SCRATCH, MOVE_STRENGTH); }
    } WHEN {
        TURN { MOVE(opponent, moveSlot: 3); MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STRENGTH, opponent);
    } THEN {
        EXPECT_EQ(opponent->moves[0], MOVE_SHADOW_RUSH);
        EXPECT_EQ(opponent->moves[1], MOVE_TACKLE);
        EXPECT_EQ(opponent->moves[3], MOVE_STRENGTH);
        EXPECT_EQ(GetBattlerShadowSealedSlots(B_BATTLER_1), 0);
    }
}

SINGLE_BATTLE_TEST("Shadow: Shadow moves are 2x against a non-Shadow Pokemon and 0.5x against a Shadow Pokemon", s16 damage)
{
    bool32 targetIsShadow;
    PARAMETRIZE { targetIsShadow = FALSE; }
    PARAMETRIZE { targetIsShadow = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Shadow(targetIsShadow); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, player);
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[1].damage, Q_4_12(4.0), results[0].damage);
    }
}

SINGLE_BATTLE_TEST("Shadow: Shadow moves hit a Ghost type")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_GENGAR) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, player);
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("Shadow: Shadow End's recoil is half of the user's HP left, even with Rock Head")
{
    enum Ability ability;
    PARAMETRIZE { ability = ABILITY_STURDY; }
    PARAMETRIZE { ability = ABILITY_ROCK_HEAD; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SHADOW_END) == EFFECT_SHADOW_END);
        PLAYER(SPECIES_GEODUDE) { Shadow(TRUE); Ability(ability); MaxHP(200); HP(151); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(500); HP(500); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0, hit: TRUE); MOVE(opponent, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_END, player);
        HP_BAR(opponent);
        HP_BAR(player, damage: 75);
    } THEN {
        EXPECT_EQ(player->hp, 76);
    }
}

DOUBLE_BATTLE_TEST("Shadow: Shadow Half halves the HP left of everyone on the field except a protected Pokemon or one at 1 HP")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SHADOW_HALF) == EFFECT_SHADOW_HALF);
        PLAYER(SPECIES_SNORLAX) { Shadow(TRUE); MaxHP(300); HP(201); Moves(MOVE_TACKLE); }
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(300); HP(300); Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(300); HP(300); Moves(MOVE_PROTECT, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { MaxHP(300); HP(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(opponentLeft, MOVE_PROTECT);
            MOVE(playerLeft, moveSlot: 0, target: opponentLeft);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_HALF, playerLeft);
    } THEN {
        EXPECT_EQ(playerLeft->hp, 101);
        EXPECT_EQ(playerRight->hp, 150);
        EXPECT_EQ(opponentLeft->hp, 300);
        EXPECT_EQ(opponentRight->hp, 1);
    }
}

SINGLE_BATTLE_TEST("Shadow: Shadow Shed ends Reflect and Safeguard, and fails when there are none")
{
    bool32 screens;
    PARAMETRIZE { screens = TRUE; }
    PARAMETRIZE { screens = FALSE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SHADOW_SHED) == EFFECT_SHADOW_SHED);
        PLAYER(SPECIES_SNORLAX) { Shadow(TRUE); Speed(1); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(10); MaxHP(999); HP(999); Moves(MOVE_REFLECT, MOVE_SAFEGUARD, MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(opponent, screens ? MOVE_REFLECT : MOVE_CELEBRATE); MOVE(player, moveSlot: 3); }
        TURN { MOVE(opponent, screens ? MOVE_SAFEGUARD : MOVE_CELEBRATE); MOVE(player, moveSlot: 3); }
        TURN { MOVE(opponent, MOVE_CELEBRATE); MOVE(player, moveSlot: 1); }
    } SCENE {
        if (screens)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_SHED, player);
        else
            NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_SHED, player);
    } THEN {
        EXPECT_EQ(gSideStatuses[B_SIDE_OPPONENT] & (SIDE_STATUS_REFLECT | SIDE_STATUS_SAFEGUARD), 0);
    }
}

SINGLE_BATTLE_TEST("Shadow: Shadow Sky brings Darkness for five turns")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Shadow(TRUE); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 2); MOVE(opponent, MOVE_CELEBRATE); }
    } THEN {
        EXPECT_EQ(gBattleWeather, B_WEATHER_DARKNESS);
        EXPECT_EQ(gBattleStruct->weatherDuration, 4);
    }
}

DOUBLE_BATTLE_TEST("Shadow: Shadow Wave hits both foes")
{
    GIVEN {
        PLAYER(SPECIES_SNORLAX) { Shadow(TRUE); Moves(MOVE_TACKLE); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, moveSlot: 3, target: opponentLeft); MOVE(playerRight, MOVE_CELEBRATE); MOVE(opponentLeft, MOVE_CELEBRATE); MOVE(opponentRight, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_WAVE, playerLeft);
        HP_BAR(opponentLeft);
        HP_BAR(opponentRight);
    }
}

DOUBLE_BATTLE_TEST("Shadow: Shadow Hold, Mist, Panic and Down affect both foes")
{
    u32 slot;
    PARAMETRIZE { slot = 0; }
    PARAMETRIZE { slot = 1; }
    PARAMETRIZE { slot = 2; }
    PARAMETRIZE { slot = 3; }
    GIVEN {
        PLAYER(SPECIES_MISDREAVUS) { Shadow(TRUE); Moves(MOVE_TACKLE); }
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN {
            MOVE(playerLeft, moveSlot: slot, target: opponentLeft, hit: TRUE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } THEN {
        for (enum BattlerId battler = B_BATTLER_1; battler < MAX_BATTLERS_COUNT; battler += 2)
        {
            if (slot == 0)
                EXPECT(gBattleMons[battler].volatiles.escapePrevention);
            else if (slot == 1)
                EXPECT_EQ(gBattleMons[battler].statStages[STAT_EVASION], DEFAULT_STAT_STAGE - 2);
            else if (slot == 2)
                EXPECT(gBattleMons[battler].volatiles.confusionTimer != 0);
            else
                EXPECT_EQ(gBattleMons[battler].statStages[STAT_DEF], DEFAULT_STAT_STAGE - 2);
        }
    }
}

TEST("Shadow: Shadow move data follows Pokemon XD")
{
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_BLITZ), 40);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_RUSH), 55);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_BREAK), 75);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_END), 120);
    EXPECT_EQ(GetMoveAccuracy(MOVE_SHADOW_END), 60);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_WAVE), 50);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_RAVE), 70);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_STORM), 95);
    EXPECT_EQ(GetMoveTarget(MOVE_SHADOW_STORM), TARGET_BOTH);
    EXPECT_EQ(GetMovePower(MOVE_SHADOW_BLAST), 80);
    EXPECT_EQ(GetMoveCategory(MOVE_SHADOW_BLAST), DAMAGE_CATEGORY_PHYSICAL);
    EXPECT_EQ(GetMoveCategory(MOVE_SHADOW_FIRE), DAMAGE_CATEGORY_SPECIAL);
    EXPECT_EQ(GetMoveAccuracy(MOVE_SHADOW_HOLD), 80);
    EXPECT_EQ(GetMoveAccuracy(MOVE_SHADOW_PANIC), 60);
    EXPECT_EQ(GetMoveWeatherType(MOVE_SHADOW_SKY), BATTLE_WEATHER_DARKNESS);
}

SINGLE_BATTLE_TEST("Shadow: a Trainer's Shadow Pokemon does not go into Hyper Mode")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND); }
    } WHEN {
        TURN { MOVE(opponent, moveSlot: 0, WITH_RNG(RNG_SHADOW_HYPER_MODE, TRUE)); MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, opponent);
    } THEN {
        EXPECT(!IsBattlerInHyperMode(B_BATTLER_1));
    }
}
