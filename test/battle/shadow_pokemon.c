#include "global.h"
#include "battle_anim_scripts.h"
#include "battle_dynamax.h"
#include "battle_info.h"
#include "battle_gimmick.h"
#include "battle_util.h"
#include "battle_z_move.h"
#include "config_changes.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "main.h"
#include "palette.h"
#include "load_save.h"
#include "move_relearner.h"
#include "party_roamer_park.h"
#include "bg.h"
#include "dma3.h"
#include "window.h"
#include "constants/shadow_pokemon.h"
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

SINGLE_BATTLE_TEST("Shadow: a battle opens the heart of each sent-out mon once, by its nature")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Nature(NATURE_BRAVE); Speed(100); Moves(MOVE_POUND); }
        PLAYER(SPECIES_EEVEE) { Shadow(TRUE); Speed(50); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); Speed(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); }
    } THEN {
        struct Pokemon *party = gParties[B_TRAINER_PLAYER];
        EXPECT_EQ(gBattleOutcome, B_OUTCOME_WON);
        // Brave: 130 per battle (Pokemon XD). Wobbuffet has no row: a gauge of 3000.
        EXPECT_EQ(GetShadowGauge(&party[0]), SHADOW_DEFAULT_HEART_GAUGE - 130);
        EXPECT_EQ(GetShadowGauge(&party[1]), GetShadowGaugeMax(&party[1]));
        AdvanceParticipatingShadowPokemon();
        EXPECT_EQ(GetShadowGauge(&party[0]), SHADOW_DEFAULT_HEART_GAUGE - 130);
    }
}

SINGLE_BATTLE_TEST("Shadow: told to use a Shadow move, a Shadow Pokemon may go into Hyper Mode instead, which lasts after switching")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Moves(MOVE_POUND); }
        PLAYER(SPECIES_EEVEE);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0, WITH_RNG(RNG_SHADOW_HYPER_MODE, TRUE)); }
        TURN { SWITCH(player, 1); }
        TURN { SWITCH(player, 0); }
    } SCENE {
        NONE_OF { ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, player); HP_BAR(opponent); }
    } THEN {
        EXPECT_EQ(opponent->hp, opponent->maxHP);
        EXPECT(IsBattlerInHyperMode(B_BATTLER_0));
        EXPECT(IsShadowPokemonInHyperMode(&gParties[B_TRAINER_PLAYER][0]));
        EXPECT_EQ(player->moves[0], MOVE_SHADOW_RUSH);
        EXPECT_EQ(player->pp[0], GetMovePP(MOVE_SHADOW_RUSH));
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PP1), GetMovePP(MOVE_POUND));
    }
}

SINGLE_BATTLE_TEST("Shadow: Hyper Mode coexists with Dynamax or Tera, and Call brings the Pokemon out of it")
{
    enum Gimmick gimmick;
    PARAMETRIZE { gimmick = GIMMICK_NONE; }
    PARAMETRIZE { gimmick = GIMMICK_DYNAMAX; }
    PARAMETRIZE { gimmick = GIMMICK_TERA; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); Nature(NATURE_DOCILE); if (gimmick == GIMMICK_TERA) TeraType(TYPE_WATER); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0, gimmick: gimmick, WITH_RNG(RNG_SHADOW_HYPER_MODE, TRUE)); }
        TURN { SHADOW_CALL(player); }
    } THEN {
        EXPECT(!IsBattlerInHyperMode(B_BATTLER_0));
        EXPECT_EQ(GetActiveGimmick(B_BATTLER_0), gimmick);
        EXPECT_EQ(player->statStages[STAT_ACC], DEFAULT_STAT_STAGE);
        // Docile: a Call out of Hyper Mode opens the heart by 360, and the battle by 100
        EXPECT_EQ(GetShadowGauge(&gParties[B_TRAINER_PLAYER][0]), SHADOW_DEFAULT_HEART_GAUGE - 360 - 100);
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
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DYNAPHOBIA, player);
        MESSAGE("{JPN}くらやみが 空を おおった！");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DARKNESS_CONTINUES);
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

WILD_BATTLE_TEST("Shadow: Sudowoodo's battle message prints the move name")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
        OPPONENT(SPECIES_SUDOWOODO) { Shadow(TRUE); }
    } WHEN {
        TURN { MOVE(opponent, moveSlot: 0); MOVE(player, MOVE_CELEBRATE); }
    } SCENE {
        MESSAGE("{JPN}{JPN}野生の {JPN}ウソッキーの {JPN}ダークラッシュ！");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, opponent);
    }
}

TEST("Shadow: all Shadow moves use the dedicated animation")
{
    for (enum Move move = FIRST_SHADOW_MOVE; move <= LAST_SHADOW_MOVE; move++)
        EXPECT(GetMoveAnimationScript(move) == gBattleAnimMove_Shadow);
    EXPECT(GetMoveAnimationScript(MOVE_DYNAPHOBIA) == gBattleAnimMove_Shadow);
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
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_SKY, player);
        MESSAGE("{JPN}くらやみが 空を おおった！");
        MESSAGE("{JPN}くらやみの矢が 落ちてくる");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_DARKNESS_CONTINUES);
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

SINGLE_BATTLE_TEST("Shadow: in Hyper Mode a Pokemon told to use another move acts by its nature")
{
    enum ShadowHyperAction action;
    u32 slot = 1;
    PARAMETRIZE { action = SHADOW_HYPER_OBEY; }
    PARAMETRIZE { action = SHADOW_HYPER_DO_NOTHING; }
    PARAMETRIZE { action = SHADOW_HYPER_HIT_SELF; }
    PARAMETRIZE { action = SHADOW_HYPER_ATTACK_FOE_TRAINER; }
    PARAMETRIZE { action = SHADOW_HYPER_RANDOM_MOVE; }
    PARAMETRIZE { action = SHADOW_HYPER_DO_NOTHING; slot = 0; } // a Shadow move is always used as told
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(1); ShadowHyperMode(TRUE); Nature(NATURE_JOLLY); Moves(MOVE_POUND, MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: slot, WITH_RNG(RNG_SHADOW_HYPER_ACTION, action)); }
    } SCENE {
        if (slot == 0)
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, player);
        else if (action == SHADOW_HYPER_OBEY)
        {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        }
        else if (action == SHADOW_HYPER_RANDOM_MOVE)
        {
            // Its only other usable move is its Shadow move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, player);
        }
        else
        {
            NONE_OF { ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player); HP_BAR(opponent); }
            if (action == SHADOW_HYPER_HIT_SELF)
                HP_BAR(player);
        }
    } THEN {
        EXPECT(IsBattlerInHyperMode(B_BATTLER_0));
    }
}

SINGLE_BATTLE_TEST("Shadow: Hyper Mode raises the critical hit stage of Shadow moves only")
{
    bool32 hyper;
    PARAMETRIZE { hyper = FALSE; }
    PARAMETRIZE { hyper = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); if (hyper) ShadowHyperMode(TRUE); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); }
    } THEN {
        struct DamageContext ctx = {0};
        ctx.battlerAtk = B_BATTLER_0;
        ctx.battlerDef = B_BATTLER_1;
        ctx.move = MOVE_SHADOW_RUSH;
        EXPECT_EQ(CalcCritChanceStage(&ctx), hyper ? SHADOW_HYPER_MODE_CRIT_STAGES : 0);
        ctx.move = MOVE_TACKLE;
        EXPECT_EQ(CalcCritChanceStage(&ctx), 0);
    }
}

SINGLE_BATTLE_TEST("Shadow: Reverse Mode costs 1/16 of the max HP each turn and Call ends it")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(1); ShadowReverseMode(TRUE); Nature(NATURE_HARDY); MaxHP(160); HP(160); Moves(MOVE_POUND, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 1); }
        TURN { SHADOW_CALL(player); }
        TURN { MOVE(player, moveSlot: 1); }
    } SCENE {
        HP_BAR(player, damage: 10);
        NOT HP_BAR(player);
    } THEN {
        EXPECT(!IsBattlerInReverseMode(B_BATTLER_0));
        EXPECT_EQ(player->hp, 150);
        // Hardy: a Call out of Reverse Mode opens the heart by 300 and the battle by 110, on top of the bar cleared
        EXPECT_EQ(GetShadowGauge(&gParties[B_TRAINER_PLAYER][0]), SHADOW_DEFAULT_HEART_GAUGE * 4 / 5 - 300 - 110);
    }
}

SINGLE_BATTLE_TEST("Shadow: using a Shadow move may send the Pokemon into Reverse Mode at the end of the turn")
{
    PASSES_RANDOMLY(30, 100, RNG_SHADOW_REVERSE_MODE);
    GIVEN {
        // Aggression 3 (no row) with less than 40% of the gauge left: 30%
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(4); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); }
    } THEN {
        EXPECT(IsBattlerInReverseMode(B_BATTLER_0));
    }
}

SINGLE_BATTLE_TEST("Shadow: Call wakes a sleeping Shadow Pokemon, or else raises its accuracy, and takes the turn")
{
    bool32 asleep;
    PARAMETRIZE { asleep = FALSE; }
    PARAMETRIZE { asleep = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Shadow(TRUE); if (asleep) Status1(STATUS1_SLEEP_TURN(3)); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { SHADOW_CALL(player); }
    } SCENE {
        NOT ANIMATION(ANIM_TYPE_MOVE, MOVE_SHADOW_RUSH, player);
    } THEN {
        EXPECT_EQ(player->status1 & STATUS1_SLEEP, 0);
        EXPECT_EQ(player->statStages[STAT_ACC], DEFAULT_STAT_STAGE + (asleep ? 0 : 1));
        // Only the battle opened the heart (Hardy 110): Call does not when there was no Hyper or Reverse Mode
        EXPECT_EQ(GetShadowGauge(&gParties[B_TRAINER_PLAYER][0]), SHADOW_DEFAULT_HEART_GAUGE - 110);
    }
}

WILD_BATTLE_TEST("Shadow: a Shadow Pokemon gains no EXP but holds 80% of it", s32 exp)
{
    bool32 shadow;
    PARAMETRIZE { shadow = FALSE; }
    PARAMETRIZE { shadow = TRUE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Level(20); Shadow(shadow); Moves(MOVE_POUND); }
        OPPONENT(SPECIES_CATERPIE) { Level(10); HP(1); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 0); }
    } SCENE {
        if (shadow)
            NOT EXPERIENCE_BAR(player);
        else
            EXPERIENCE_BAR(player, captureGainedExp: &results[i].exp);
    } THEN {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
        if (shadow)
        {
            EXPECT_EQ(GetMonData(mon, MON_DATA_EXP), gExperienceTables[gSpeciesInfo[SPECIES_WOBBUFFET].growthRate][20]);
            results[i].exp = GetShadowHeldExp(mon);
        }
    } FINALLY {
        s32 held = results[0].exp * SHADOW_HELD_EXP_PERCENT / 100;
        EXPECT_GT(results[0].exp, 0);
        EXPECT_LE(results[1].exp, held + SHADOW_HELD_EXP_UNIT / 2);
        EXPECT_GE(results[1].exp + SHADOW_HELD_EXP_UNIT / 2, held);
    }
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

TEST("Shadow: the heart gauge follows Pokemon XD: a full gauge per species, five bars and drops by nature")
{
    struct Pokemon mon;

    // Personality 0: Hardy (battle 110, Call 300, walk 100, scent 90)
    CreateMon(&mon, SPECIES_LUGIA, 50, NATURE_HARDY, OTID_STRUCT_PLAYER_ID);
    EXPECT(MarkShadowPokemon(&mon));
    EXPECT_EQ(GetShadowGaugeMax(&mon), 12000);
    EXPECT_EQ(GetShadowGauge(&mon), 12000);
    EXPECT_EQ(GetShadowAggression(&mon), 1);
    EXPECT_EQ(GetShadowHeartGauge(&mon), 0);

    EXPECT_EQ(ReduceShadowHeartGauge(&mon, SHADOW_ACTION_BATTLE, 1), 110);
    EXPECT_EQ(ReduceShadowHeartGauge(&mon, SHADOW_ACTION_CALL, 1), 300);
    EXPECT_EQ(ReduceShadowHeartGauge(&mon, SHADOW_ACTION_WALK, 2), 200);
    EXPECT_EQ(ReduceShadowHeartGauge(&mon, SHADOW_ACTION_SCENT, 1), 90);
    EXPECT_EQ(GetShadowGauge(&mon), 12000 - 700);
    // A bar shows while any of it is left
    EXPECT_EQ(GetShadowHeartGauge(&mon), 0);
    ReduceShadowHeartGaugeBy(&mon, 12000 - 700 - 9600);
    EXPECT_EQ(GetShadowHeartGauge(&mon), 1);
    ReduceShadowHeartGaugeBy(&mon, 9600 - 1);
    EXPECT_EQ(GetShadowHeartGauge(&mon), 4);
    EXPECT(!CanPurifyShadowPokemon(&mon));
    EXPECT_EQ(ReduceShadowHeartGaugeBy(&mon, 500), 1);
    EXPECT_EQ(GetShadowHeartGauge(&mon), 5);
    EXPECT_EQ(GetShadowPokemonState(&mon), SHADOW_STATE_READY);
    EXPECT(CanPurifyShadowPokemon(&mon));

    // No row: the default gauge
    CreateMon(&mon, SPECIES_WOBBUFFET, 50, NATURE_HARDY, OTID_STRUCT_PLAYER_ID);
    EXPECT(MarkShadowPokemon(&mon));
    EXPECT_EQ(GetShadowGaugeMax(&mon), SHADOW_DEFAULT_HEART_GAUGE);
    EXPECT_EQ(GetShadowAggression(&mon), SHADOW_DEFAULT_AGGRESSION);
}

TEST("Shadow: the Reverse Mode chance follows aggression and the gauge left")
{
    struct Pokemon mon;

    CreateMon(&mon, SPECIES_LUGIA, 50, NATURE_HARDY, OTID_STRUCT_PLAYER_ID); // aggression 1
    MarkShadowPokemon(&mon);
    EXPECT_EQ(GetShadowReverseModeChance(&mon), 0);
    ReduceShadowHeartGaugeBy(&mon, 12000 / 5);      // 80% left
    EXPECT_EQ(GetShadowReverseModeChance(&mon), 10);
    ReduceShadowHeartGaugeBy(&mon, 12000 / 5 + 1);  // under 60%
    EXPECT_EQ(GetShadowReverseModeChance(&mon), 35);
    ReduceShadowHeartGaugeBy(&mon, 12000 * 2 / 5);  // under 20%
    EXPECT_EQ(GetShadowReverseModeChance(&mon), 50);
}

TEST("Shadow: 256 steps, a meal and 5 seconds in the bath open the hearts of the Shadow Pokemon in the party")
{
    struct Pokemon *party = gParties[B_TRAINER_PLAYER];

    ZeroPlayerPartyMons();
    CreateMon(&party[0], SPECIES_WOBBUFFET, 50, NATURE_LONELY, OTID_STRUCT_PLAYER_ID); // walk 100, scent 130
    CreateMon(&party[1], SPECIES_WOBBUFFET, 50, NATURE_LONELY, OTID_STRUCT_PLAYER_ID);
    MarkShadowPokemon(&party[0]);
    CalculatePlayerPartyCount();

    VarSet(VAR_SHADOW_STEP_COUNTER, 0);
    for (u32 i = 0; i < 255; i++)
        ShadowPokemon_OnStep();
    EXPECT_EQ(GetShadowGauge(&party[0]), SHADOW_DEFAULT_HEART_GAUGE);
    ShadowPokemon_OnStep();
    EXPECT_EQ(GetShadowGauge(&party[0]), SHADOW_DEFAULT_HEART_GAUGE - 100);

    ShadowPokemon_OnMeal(&party[0]);
    EXPECT_EQ(GetShadowGauge(&party[0]), SHADOW_DEFAULT_HEART_GAUGE - 100 - 130);

    ShadowPokemon_OnBathTick();
    EXPECT_EQ(GetShadowGauge(&party[0]), SHADOW_DEFAULT_HEART_GAUGE - 100 - 130 - SHADOW_BATH_GAUGE_REDUCTION);
    EXPECT(!IsShadowPokemon(&party[1]));
    EXPECT_EQ(GetShadowGauge(&party[1]), 0);
}

TEST("Shadow: purification gives the held EXP once, teaches the moves of the levels reached and gives the National Ribbon")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    enum GrowthRate growthRate = gSpeciesInfo[SPECIES_BULBASAUR].growthRate;
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(SPECIES_BULBASAUR);
    const u16 tackle = MOVE_TACKLE, none = MOVE_NONE;
    enum Move expected[MAX_MON_MOVES] = { MOVE_TACKLE };
    u32 count = 1;
    u16 units;

    ZeroPlayerPartyMons();
    CreateMon(mon, SPECIES_BULBASAUR, 5, NATURE_HARDY, OTID_STRUCT_PLAYER_ID);
    CalculatePlayerPartyCount();
    SetMonData(mon, MON_DATA_MOVE1, &tackle);
    for (u32 i = 1; i < MAX_MON_MOVES; i++)
        SetMonData(mon, MON_DATA_MOVE1 + i, &none);
    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END && count < MAX_MON_MOVES; i++)
    {
        if (learnset[i].level > 5 && learnset[i].level <= 10)
            expected[count++] = learnset[i].move;
    }
    EXPECT_GT(count, 1);

    MarkShadowPokemon(mon);
    units = (gExperienceTables[growthRate][10] - GetMonData(mon, MON_DATA_EXP) + SHADOW_HELD_EXP_UNIT - 1) / SHADOW_HELD_EXP_UNIT;
    SetMonData(mon, MON_DATA_SHADOW_HELD_EXP, &units);
    SetShadowPokemonHyperMode(mon, TRUE);
    SetShadowPokemonReverseMode(mon, TRUE);
    EXPECT_EQ(GetShadowHeldExp(mon), units * SHADOW_HELD_EXP_UNIT);

    AddShadowHeartGauge(mon, SHADOW_GAUGE_BARS);
    EXPECT(PurifyShadowPokemon(mon));
    EXPECT_EQ(GetShadowPokemonState(mon), SHADOW_STATE_PURIFIED);
    EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 10);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + i), expected[i]);
    EXPECT(GetMonData(mon, MON_DATA_NATIONAL_RIBBON));
    EXPECT(!IsShadowPokemonInHyperMode(mon));
    EXPECT(!IsShadowPokemonInReverseMode(mon));
    EXPECT_EQ(GetMonData(mon, MON_DATA_SHADOW_HELD_EXP), 0);
    EXPECT(!PurifyShadowPokemon(mon));
    EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 10);
}

TEST("Shadow: a Shadow Pokemon cannot evolve or relearn moves until it is purified")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    bool32 canStopEvo;

    ZeroPlayerPartyMons();
    CreateMon(mon, SPECIES_PIKACHU, 50, NATURE_HARDY, OTID_STRUCT_PLAYER_ID);
    CalculatePlayerPartyCount();
    EXPECT_EQ(GetEvolutionTargetSpecies(mon, EVO_MODE_ITEM_USE, ITEM_THUNDER_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_RAICHU);
    MarkShadowPokemon(mon);
    EXPECT_EQ(GetEvolutionTargetSpecies(mon, EVO_MODE_ITEM_USE, ITEM_THUNDER_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_NONE);
    for (enum MoveRelearnerStates state = 0; state < MOVE_RELEARNER_COUNT; state++)
        EXPECT(!HasMoveToRelearn(&mon->box, state));
    AddShadowHeartGauge(mon, SHADOW_GAUGE_BARS);
    PurifyShadowPokemon(mon);
    EXPECT_EQ(GetEvolutionTargetSpecies(mon, EVO_MODE_ITEM_USE, ITEM_THUNDER_STONE, NULL, &canStopEvo, CHECK_EVO), SPECIES_RAICHU);
}

SINGLE_BATTLE_TEST("Shadow: in Hyper Mode a Pokemon may eat its Berry, or else only play with its item")
{
    enum Item item;
    PARAMETRIZE { item = ITEM_LIECHI_BERRY; }
    PARAMETRIZE { item = ITEM_LEFTOVERS; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(1); ShadowHyperMode(TRUE); Item(item); Moves(MOVE_POUND, MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 1, WITH_RNG(RNG_SHADOW_HYPER_ACTION, SHADOW_HYPER_USE_ITEM)); }
    } SCENE {
        NONE_OF { ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player); HP_BAR(opponent); }
    } THEN {
        if (item == ITEM_LIECHI_BERRY)
        {
            EXPECT_EQ(player->item, ITEM_NONE);
            EXPECT_EQ(player->statStages[STAT_ATK], DEFAULT_STAT_STAGE + 1);
        }
        else
        {
            EXPECT_EQ(player->item, ITEM_LEFTOVERS);
        }
    }
}

SINGLE_BATTLE_TEST("Shadow: in Hyper Mode a Pokemon may go back to its Poke Ball, and the player sends out another")
{
    u32 count;
    PARAMETRIZE { count = 1; }
    PARAMETRIZE { count = 2; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(1); ShadowHyperMode(TRUE); Moves(MOVE_POUND, MOVE_TACKLE); }
        if (count == 2)
        {
            PLAYER(SPECIES_EEVEE);
        }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        if (count == 2)
            TURN { MOVE(player, moveSlot: 1, WITH_RNG(RNG_SHADOW_HYPER_ACTION, SHADOW_HYPER_RETURN_TO_BALL)); SEND_OUT(player, 1); }
        else
            TURN { MOVE(player, moveSlot: 1, WITH_RNG(RNG_SHADOW_HYPER_ACTION, SHADOW_HYPER_RETURN_TO_BALL)); }
    } SCENE {
        NONE_OF { ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player); HP_BAR(opponent); }
    } THEN {
        EXPECT_EQ(gBattlerPartyIndexes[B_BATTLER_0], count == 2 ? 1 : 0);
        EXPECT(IsShadowPokemonInHyperMode(&gParties[B_TRAINER_PLAYER][0]));
    }
}

SINGLE_BATTLE_TEST("Shadow: the battle info view shows the heart gauge of a Shadow Pokemon in five bars")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { ShadowStage(2); Moves(MOVE_POUND, MOVE_CELEBRATE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(player, moveSlot: 1); }
    } THEN {
        // Two bars cleared, and the battle (Hardy: 110) has taken some of the third
        static const u16 points[][2] = { {9, 1}, {28, 1}, {47, 1}, {62, 0}, {66, 0}, {100, 0} };
        EXPECT_EQ(GetShadowGauge(&gParties[B_TRAINER_PLAYER][0]), SHADOW_DEFAULT_HEART_GAUGE * 3 / 5 - 110);
        EXPECT(Test_BattleInfoOpen(B_BATTLER_0));
        const u8 *pixels = (const u8 *)GetWindowAttribute(Test_BattleInfoWindow(), WINDOW_TILE_DATA);
        for (u32 i = 0; i < ARRAY_COUNT(points); i++)
        {
            u32 x = points[i][0], y = 83;
            u32 offset = ((y / 8) * 28 + x / 8) * 32 + (y % 8) * 4 + (x % 8) / 2;
            EXPECT_EQ((pixels[offset] >> ((x % 2) * 4)) & 15, points[i][1] ? 8 : 4);
        }
        // The opponent is not a Shadow Pokemon: no gauge
        u32 x = 112 + 10, y = 83;
        u32 offset = ((y / 8) * 28 + x / 8) * 32 + (y % 8) * 4 + (x % 8) / 2;
        EXPECT_EQ((pixels[offset] >> ((x % 2) * 4)) & 15, 7);
        while (IsDma3ManagerBusyWithBgCopy())
            ProcessDma3Requests();
        Test_BattleInfoClose();
    }
}

TEST("Shadow: in the camp, an open-hearted Shadow Pokemon is purified only with four other Pokemon round it")
{
    struct Pokemon *party = gParties[B_TRAINER_PLAYER];

    ZeroPlayerPartyMons();
    for (u32 i = 0; i < 5; i++)
        CreateMon(&party[i], SPECIES_WOBBUFFET, 50, NATURE_HARDY, OTID_STRUCT_PLAYER_ID);
    CalculatePlayerPartyCount();
    gSpecialVar_LastTalked = 1; // the first party slot's Pokemon

    EXPECT_EQ(Special_GetPartyRoamerShadowState(), SHADOW_ROAMER_NORMAL);
    MarkShadowPokemon(&party[0]);
    EXPECT_EQ(Special_GetPartyRoamerShadowState(), SHADOW_ROAMER_CLOSED);
    EXPECT_EQ(gSpecialVar_0x8004, 0);
    AddShadowHeartGauge(&party[0], SHADOW_GAUGE_BARS);
    EXPECT_EQ(Special_GetPartyRoamerShadowState(), SHADOW_ROAMER_CAN_PURIFY);
    EXPECT_EQ(CountShadowPurifyPartners(0), 4);

    // Another Shadow Pokemon is no partner; once purified it is
    MarkShadowPokemon(&party[1]);
    EXPECT_EQ(Special_GetPartyRoamerShadowState(), SHADOW_ROAMER_NEEDS_PARTNERS);
    AddShadowHeartGauge(&party[1], SHADOW_GAUGE_BARS);
    EXPECT(PurifyShadowPokemon(&party[1]));
    EXPECT_EQ(Special_GetPartyRoamerShadowState(), SHADOW_ROAMER_CAN_PURIFY);

    ZeroMonData(&party[4]);
    CalculatePlayerPartyCount();
    EXPECT_EQ(Special_GetPartyRoamerShadowState(), SHADOW_ROAMER_NEEDS_PARTNERS);
}

static bool8 sPurificationSceneEnded;

static void CB2_TestPurificationSceneEnded(void)
{
    sPurificationSceneEnded = TRUE;
}

TEST("Shadow: the purification scene (the evolution scene) purifies the Pokemon and keeps its species")
{
    struct Pokemon *party = gParties[B_TRAINER_PLAYER];
    MainCallback callback = gMain.callback2;

    ZeroPlayerPartyMons();
    CreateMon(&party[0], SPECIES_WOBBUFFET, 50, NATURE_HARDY, OTID_STRUCT_PLAYER_ID);
    CalculatePlayerPartyCount();
    MarkShadowPokemon(&party[0]);
    AddShadowHeartGauge(&party[0], SHADOW_GAUGE_BARS);

    sPurificationSceneEnded = FALSE;
    gCB2_AfterEvolution = CB2_TestPurificationSceneEnded;
    BeginPurificationScene(0);
    for (u32 frame = 0; frame < 20000 && !sPurificationSceneEnded; frame++)
    {
        gMain.newKeys = (frame % 8 == 0) ? A_BUTTON : 0;
        gMain.callback2();
        if (gMain.vblankCallback != NULL)
            gMain.vblankCallback();
        else
            TransferPlttBuffer(); // as a VBlank would
    }
    gMain.newKeys = 0;
    EXPECT_EQ(GetShadowPokemonState(&party[0]), SHADOW_STATE_PURIFIED);
    EXPECT(sPurificationSceneEnded);
    EXPECT_EQ(GetMonData(&party[0], MON_DATA_SPECIES), SPECIES_WOBBUFFET);
    SetMainCallback2(callback);
}
