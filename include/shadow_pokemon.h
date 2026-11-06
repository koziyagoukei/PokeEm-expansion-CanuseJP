#ifndef GUARD_SHADOW_POKEMON_H
#define GUARD_SHADOW_POKEMON_H

#include "config/shadow_pokemon.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "move.h"

// The heart gauge (Pokemon XD): a full gauge per species, lowered by these actions. Five bars.
#define SHADOW_GAUGE_BARS        5
#define SHADOW_GAUGE_MAX_VALUE   0x3FFE // the largest full gauge a species can have
#define SHADOW_GAUGE_PURIFIED    0x3FFF // stored in place of the gauge once purified
#define SHADOW_HELD_EXP_UNIT     16     // held EXP is stored in these steps (14 bits, up to 262128)

enum ShadowAction
{
    SHADOW_ACTION_BATTLE, // took part in a battle (once per battle)
    SHADOW_ACTION_CALL,   // called back from Hyper or Reverse Mode
    SHADOW_ACTION_WALK,   // 256 steps in the party
    SHADOW_ACTION_SCENT,  // a scent; a meal at the dining room counts as one
    SHADOW_ACTION_COUNT,
};

enum ShadowState
{
    SHADOW_STATE_NORMAL,
    SHADOW_STATE_STAGE_0 = 0xE8, // no bar cleared
    SHADOW_STATE_STAGE_1,
    SHADOW_STATE_STAGE_2,
    SHADOW_STATE_STAGE_3,
    SHADOW_STATE_STAGE_4,
    SHADOW_STATE_READY,          // the gauge is empty: it can be purified
    SHADOW_STATE_PURIFIED,
};

// What a disobedient Pokemon in Hyper Mode does instead of the move it was told (Colosseum)
enum ShadowHyperAction
{
    SHADOW_HYPER_RANDOM_MOVE,
    SHADOW_HYPER_ATTACK_FOE_TRAINER,
    SHADOW_HYPER_ATTACK_OWN_TRAINER,
    SHADOW_HYPER_HIT_SELF,
    SHADOW_HYPER_DO_NOTHING,
    SHADOW_HYPER_USE_ITEM,
    SHADOW_HYPER_RETURN_TO_BALL,
    SHADOW_HYPER_OBEY,
    SHADOW_HYPER_ACTION_COUNT,
};

struct Pokemon;
struct BoxPokemon;

// A row of src/data/shadow_pokemon.h
struct ShadowPokemonInfo
{
    u16 species;
    u16 moves[MAX_MON_MOVES]; // Shadow moves, in their slot order
    u16 heartGauge;           // full gauge (0 = the default)
    u8 aggression;            // 1 (very high, often Reverse Mode) .. 5 (very low); 0 = the default
    u16 purifyMoves[2];       // a Battle Connector Shadow Pokemon learns one of these when purified
};

// What a Shadow Pokemon has in each move slot (GetShadowMoveLayout)
struct ShadowMoveLayout
{
    enum Move moves[MAX_MON_MOVES];
    u8 shadowSlots; // slots holding a Shadow move
    u8 sealedSlots; // own moves its heart has not opened yet (shown as "？？？")
};

// State
enum ShadowState GetShadowPokemonState(struct Pokemon *mon);
bool32 IsShadowPokemon(struct Pokemon *mon);
bool32 IsShadowBoxPokemon(struct BoxPokemon *boxMon);
bool32 MarkShadowPokemon(struct Pokemon *mon);
u32 GetShadowGauge(struct Pokemon *mon);
u32 GetShadowGaugeMax(struct Pokemon *mon);
u32 GetShadowHeartGauge(struct Pokemon *mon);
void AddShadowHeartGauge(struct Pokemon *mon, u32 bars);
u32 ReduceShadowHeartGauge(struct Pokemon *mon, enum ShadowAction action, u32 times);
u32 ReduceShadowHeartGaugeBy(struct Pokemon *mon, u32 amount);
u32 GetShadowGaugeReduction(struct Pokemon *mon, enum ShadowAction action);
u32 GetShadowAggression(struct Pokemon *mon);
bool32 IsShadowPokemonInHyperMode(struct Pokemon *mon);
bool32 IsShadowPokemonInReverseMode(struct Pokemon *mon);
void SetShadowPokemonHyperMode(struct Pokemon *mon, bool32 on);
void SetShadowPokemonReverseMode(struct Pokemon *mon, bool32 on);
void SetShadowBoxPokemonHyperMode(struct BoxPokemon *boxMon, bool32 on);
u32 GetShadowHeldExp(struct Pokemon *mon);
void HoldShadowPokemonExp(struct Pokemon *mon, u32 exp);
bool32 CanPurifyShadowPokemon(struct Pokemon *mon);
bool32 PurifyShadowPokemon(struct Pokemon *mon);
bool32 PurifyShadowPokemonInScene(struct Pokemon *mon);
enum Move GetShadowPurifyMove(struct Pokemon *mon);
u32 ShadowTryLearningPurifyMove(struct Pokemon *mon, bool32 firstMove);
bool32 IsBattleConnectorPurified(struct Pokemon *mon);
u32 CountShadowPurifyPartners(u32 partySlot);
void ShadowPokemon_OnStep(void);
void ShadowPokemon_OnMeal(struct Pokemon *mon);
void ShadowPokemon_OnBathTick(void);

// Moves
u32 GetShadowMovesForSpecies(enum Species species, enum Move moves[MAX_MON_MOVES]);
void GetShadowMoveLayout(struct Pokemon *mon, bool32 sealOwnMoves, struct ShadowMoveLayout *layout);

// Battle
bool32 UsesShadowBattleMoves(void);
bool32 IsBattlerShadowPokemon(enum BattlerId battler);
bool32 IsPlayerShadowBattler(enum BattlerId battler);
enum BattlerId FindShadowEncounterBattler(void);
u32 GetShadowSealedMoveMask(struct Pokemon *mon);
u32 GetBattlerShadowSealedSlots(enum BattlerId battler);
void ApplyShadowBattleMoves(enum BattlerId battler);
void AdvanceParticipatingShadowPokemon(void);
bool32 IsBattlerInHyperMode(enum BattlerId battler);
bool32 IsBattlerInReverseMode(enum BattlerId battler);
bool32 CanBattlerCallShadowPokemon(enum BattlerId battler);
u32 GetShadowHyperModeChance(struct Pokemon *mon);
enum ShadowHyperAction GetShadowHyperModeAction(struct Pokemon *mon);
u32 GetShadowReverseModeChance(struct Pokemon *mon);
void ClearShadowModesOnFaint(enum BattlerId battler);

#endif
