#ifndef GUARD_SHADOW_POKEMON_H
#define GUARD_SHADOW_POKEMON_H

#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/species.h"
#include "move.h"

enum ShadowState
{
    SHADOW_STATE_NORMAL,
    SHADOW_STATE_STAGE_0 = 0xE8,
    SHADOW_STATE_STAGE_1,
    SHADOW_STATE_STAGE_2,
    SHADOW_STATE_STAGE_3,
    SHADOW_STATE_STAGE_4,
    SHADOW_STATE_READY,
    SHADOW_STATE_PURIFIED,
};

struct Pokemon;

// A row of src/data/shadow_pokemon_moves.h
struct ShadowPokemonMoves
{
    u16 species;
    u16 moves[MAX_MON_MOVES];
};

// What a Shadow Pokemon has in each move slot (GetShadowMoveLayout)
struct ShadowMoveLayout
{
    enum Move moves[MAX_MON_MOVES];
    u8 shadowSlots; // slots holding a Shadow move
    u8 sealedSlots; // own moves its heart has not opened yet (shown as "？？？")
};

enum ShadowState GetShadowPokemonState(struct Pokemon *mon);
bool32 IsShadowPokemon(struct Pokemon *mon);
bool32 MarkShadowPokemon(struct Pokemon *mon);
bool32 CanPurifyShadowPokemon(struct Pokemon *mon);
bool32 PurifyShadowPokemon(struct Pokemon *mon);
u32 GetShadowHeartGauge(struct Pokemon *mon);
void AddShadowHeartGauge(struct Pokemon *mon, u32 stages);
u32 GetShadowMovesForSpecies(enum Species species, enum Move moves[MAX_MON_MOVES]);
void GetShadowMoveLayout(struct Pokemon *mon, bool32 sealOwnMoves, struct ShadowMoveLayout *layout);

bool32 UsesShadowBattleMoves(void);
bool32 IsBattlerShadowPokemon(enum BattlerId battler);
enum BattlerId FindShadowEncounterBattler(void);
u32 GetShadowSealedMoveMask(struct Pokemon *mon);
u32 GetBattlerShadowSealedSlots(enum BattlerId battler);
void ApplyShadowBattleMoves(enum BattlerId battler);
void AdvanceParticipatingShadowPokemon(void);
bool32 IsBattlerInHyperMode(enum BattlerId battler);
void TryEnterShadowHyperMode(enum BattlerId battler, enum Move move);
bool32 CalmShadowPokemon(enum BattlerId battler);

#endif
