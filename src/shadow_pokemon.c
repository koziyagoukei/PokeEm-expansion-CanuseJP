#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "config/shadow_pokemon.h"
#include "event_data.h"
#include "move.h"
#include "pokemon.h"
#include "random.h"
#include "shadow_pokemon.h"
#include "test_runner.h"
#include "data/shadow_pokemon_moves.h"

enum ShadowState GetShadowPokemonState(struct Pokemon *mon)
{
    enum ShadowState state = GetMonData(mon, MON_DATA_SHADOW_STATE);

    if (state < SHADOW_STATE_STAGE_0 || state > SHADOW_STATE_PURIFIED
     || GetMonData(mon, MON_DATA_SPECIES_OR_EGG) == SPECIES_EGG
     || GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE)
        return SHADOW_STATE_NORMAL;
    return state;
}

bool32 IsShadowPokemon(struct Pokemon *mon)
{
    enum ShadowState state = GetShadowPokemonState(mon);
    return state >= SHADOW_STATE_STAGE_0 && state <= SHADOW_STATE_READY;
}

bool32 MarkShadowPokemon(struct Pokemon *mon)
{
    if (GetShadowPokemonState(mon) != SHADOW_STATE_NORMAL
     || GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE
     || GetMonData(mon, MON_DATA_SPECIES_OR_EGG) == SPECIES_EGG)
        return FALSE;

    const u8 state = SHADOW_STATE_STAGE_0;
    SetMonData(mon, MON_DATA_SHADOW_STATE, &state);
    return TRUE;
}

bool32 CanPurifyShadowPokemon(struct Pokemon *mon)
{
    return GetShadowPokemonState(mon) == SHADOW_STATE_READY;
}

bool32 PurifyShadowPokemon(struct Pokemon *mon)
{
    // Event/field only: never change the move layout of an active battle.
    if (gMain.inBattle || !CanPurifyShadowPokemon(mon))
        return FALSE;

    const u8 state = SHADOW_STATE_PURIFIED;
    SetMonData(mon, MON_DATA_SHADOW_STATE, &state);
    return TRUE;
}

u32 GetShadowHeartGauge(struct Pokemon *mon)
{
    enum ShadowState state = GetShadowPokemonState(mon);
    return state >= SHADOW_STATE_STAGE_0 ? min(state, SHADOW_STATE_READY) - SHADOW_STATE_STAGE_0 : 0;
}

void AddShadowHeartGauge(struct Pokemon *mon, u32 stages)
{
    enum ShadowState state = GetShadowPokemonState(mon);
    if (state >= SHADOW_STATE_STAGE_0 && state < SHADOW_STATE_READY)
    {
        const u8 updated = state + min(stages, (u32)(SHADOW_STATE_READY - state));
        SetMonData(mon, MON_DATA_SHADOW_STATE, &updated);
    }
}

bool32 UsesShadowBattleMoves(void)
{
    u32 flags = gBattleTypeFlags;
    // The battle test harness feeds local actions through recorded playback.
    if (TESTING && gTestRunnerEnabled)
        flags &= ~(BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK);

    // R is local input, not part of the link or recorded action stream.
    return !(flags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK
                    | BATTLE_TYPE_TOWER_LINK_MULTI | BATTLE_TYPE_SAFARI | BATTLE_TYPE_CATCH_TUTORIAL | BATTLE_TYPE_POKEDUDE));
}

bool32 IsBattlerShadowPokemon(enum BattlerId battler)
{
    return gBattlerPartyIndexes[battler] < PARTY_SIZE && IsShadowPokemon(GetBattlerMon(battler));
}

enum BattlerId FindShadowEncounterBattler(void)
{
    if (UsesShadowBattleMoves())
    {
        for (enum BattlerId battler = 0; battler < gBattlersCount; battler++)
        {
            if (!IsOnPlayerSide(battler) && IsBattlerAlive(battler) && IsBattlerShadowPokemon(battler))
                return battler;
        }
    }
    return MAX_BATTLERS_COUNT;
}

// The Shadow moves of a species: its row in sShadowPokemonMoves (or its base species' row), else Shadow Rush and the
// Shadow move of a Fire, Electric or Ice type. Returns how many (1-4), and the moves in moves[].
u32 GetShadowMovesForSpecies(enum Species species, enum Move moves[MAX_MON_MOVES])
{
    u32 i, count = 0;
    enum Species base = GET_BASE_SPECIES_ID(species);
    const struct ShadowPokemonMoves *row = NULL;

    for (i = 0; i < MAX_MON_MOVES; i++)
        moves[i] = MOVE_NONE;

    for (i = 0; i < ARRAY_COUNT(sShadowPokemonMoves) && row == NULL; i++)
    {
        if (sShadowPokemonMoves[i].species == species)
            row = &sShadowPokemonMoves[i];
    }
    for (i = 0; i < ARRAY_COUNT(sShadowPokemonMoves) && row == NULL; i++)
    {
        if (sShadowPokemonMoves[i].species == base)
            row = &sShadowPokemonMoves[i];
    }

    if (row != NULL)
    {
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            if (IsShadowMove(row->moves[i]))
                moves[count++] = row->moves[i];
        }
    }

    if (count == 0)
    {
        moves[count++] = MOVE_SHADOW_RUSH;
        if (GetSpeciesType(species, 0) == TYPE_FIRE || GetSpeciesType(species, 1) == TYPE_FIRE)
            moves[count++] = MOVE_SHADOW_FIRE;
        else if (GetSpeciesType(species, 0) == TYPE_ELECTRIC || GetSpeciesType(species, 1) == TYPE_ELECTRIC)
            moves[count++] = MOVE_SHADOW_BOLT;
        else if (GetSpeciesType(species, 0) == TYPE_ICE || GetSpeciesType(species, 1) == TYPE_ICE)
            moves[count++] = MOVE_SHADOW_CHILL;
    }
    return count;
}

// The n-th own move after the Shadow moves (0 = first) opens at these heart gauge stages; the others stay sealed until
// purification (Pokemon XD).
static bool32 IsShadowOwnMoveOpen(u32 ownIndex, u32 stage)
{
    if (ownIndex == 0)
        return stage >= SHADOW_OWN_MOVE_1_STAGE;
    if (ownIndex == 1)
        return stage >= SHADOW_OWN_MOVE_2_STAGE;
    return FALSE;
}

// What a Shadow Pokemon has in each move slot, Pokemon XD style: its Shadow moves in the first slots, then its own
// moves from the slot after them. With sealOwnMoves, own moves the heart has not opened yet are left out
// (MOVE_NONE, sealedSlots). The moves in the Pokemon's data are never changed: the slots under the Shadow moves come
// back when it is purified, and so do the sealed ones.
void GetShadowMoveLayout(struct Pokemon *mon, bool32 sealOwnMoves, struct ShadowMoveLayout *layout)
{
    enum Move shadowMoves[MAX_MON_MOVES];
    u32 i, count = 0, stage;

    layout->shadowSlots = 0;
    layout->sealedSlots = 0;
    for (i = 0; i < MAX_MON_MOVES; i++)
        layout->moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);

    if (!IsShadowPokemon(mon))
        return;

    count = GetShadowMovesForSpecies(GetMonData(mon, MON_DATA_SPECIES), shadowMoves);
    stage = GetShadowHeartGauge(mon);
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (i < count)
        {
            layout->moves[i] = shadowMoves[i];
            layout->shadowSlots |= 1u << i;
        }
        else if (sealOwnMoves && layout->moves[i] != MOVE_NONE && !IsShadowOwnMoveOpen(i - count, stage))
        {
            layout->moves[i] = MOVE_NONE;
            layout->sealedSlots |= 1u << i;
        }
    }
}

// The player's own Shadow Pokemon keep their own moves sealed; a Trainer's Shadow Pokemon use theirs (XD).
static bool32 SealsOwnShadowMoves(struct Pokemon *mon)
{
    return mon >= &gParties[B_TRAINER_PLAYER][0] && mon < &gParties[B_TRAINER_PLAYER][PARTY_SIZE];
}

// The move slots whose battle move is not the one in the Pokemon's data (Shadow moves and sealed moves). Writes back
// to the party skip them.
u32 GetShadowSealedMoveMask(struct Pokemon *mon)
{
    struct ShadowMoveLayout layout;

    if (!UsesShadowBattleMoves() || !IsShadowPokemon(mon))
        return 0;

    GetShadowMoveLayout(mon, SealsOwnShadowMoves(mon), &layout);
    return layout.shadowSlots | layout.sealedSlots;
}

// The slots a battler shows as "？？？": own moves its heart has not opened yet.
u32 GetBattlerShadowSealedSlots(enum BattlerId battler)
{
    struct ShadowMoveLayout layout;
    struct Pokemon *mon;

    if (!UsesShadowBattleMoves() || !IsBattlerShadowPokemon(battler))
        return 0;

    mon = GetBattlerMon(battler);
    GetShadowMoveLayout(mon, SealsOwnShadowMoves(mon), &layout);
    return layout.sealedSlots;
}

void ApplyShadowBattleMoves(enum BattlerId battler)
{
    struct ShadowMoveLayout layout;
    struct Pokemon *mon;
    u32 overlay;

    if (!UsesShadowBattleMoves() || !IsBattlerShadowPokemon(battler))
        return;

    mon = GetBattlerMon(battler);
    GetShadowMoveLayout(mon, SealsOwnShadowMoves(mon), &layout);
    overlay = layout.shadowSlots | layout.sealedSlots;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        if (!(overlay & (1u << i)))
            continue;
        // Shadow moves use no PP, so they always show their full PP.
        gBattleMons[battler].moves[i] = layout.moves[i];
        gBattleMons[battler].pp[i] = layout.moves[i] != MOVE_NONE ? GetMovePP(layout.moves[i]) : 0;
        gBattleMons[battler].ppBonuses &= ~(3 << (2 * i));
    }
}

void AdvanceParticipatingShadowPokemon(void)
{
    if (gBattleStruct->shadowProgressApplied)
        return;
    gBattleStruct->shadowProgressApplied = TRUE;

    if (!UsesShadowBattleMoves()
     || gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TOWER_LINK_MULTI | BATTLE_TYPE_TRAINER_HILL
                          | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_SECRET_BASE)
     || (gBattleOutcome != B_OUTCOME_WON && gBattleOutcome != B_OUTCOME_CAUGHT))
        return;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (gBattleStruct->partyState[B_TRAINER_PLAYER][i].sentOut)
            AddShadowHeartGauge(&gParties[B_TRAINER_PLAYER][i], SHADOW_STAGES_PER_BATTLE);
    }
}

bool32 IsBattlerInHyperMode(enum BattlerId battler)
{
    // Like Colosseum, switching does not calm the Pokemon. This project's
    // Hyper Mode ends with the battle, rather than being stored in the save.
    return GetBattlerPartyState(battler)->shadowHyperMode;
}

// Only the player's own Shadow Pokemon go into Hyper Mode (Colosseum), so it can always be calmed.
void TryEnterShadowHyperMode(enum BattlerId battler, enum Move move)
{
    if (IsShadowMove(move) && UsesShadowBattleMoves() && IsBattlerShadowPokemon(battler)
     && SealsOwnShadowMoves(GetBattlerMon(battler))
     && !IsBattlerInHyperMode(battler) && RandomPercentage(RNG_SHADOW_HYPER_MODE, SHADOW_HYPER_MODE_CHANCE))
        GetBattlerPartyState(battler)->shadowHyperMode = TRUE;
}

bool32 CalmShadowPokemon(enum BattlerId battler)
{
    if (!UsesShadowBattleMoves() || !IsBattlerShadowPokemon(battler) || !IsBattlerInHyperMode(battler))
        return FALSE;

    GetBattlerPartyState(battler)->shadowHyperMode = FALSE;
    return TRUE;
}

static struct Pokemon *GetSpecialShadowMon(void)
{
    if (gSpecialVar_0x8004 >= PARTY_SIZE)
        return NULL;
    return &gParties[B_TRAINER_PLAYER][gSpecialVar_0x8004];
}

// Specials take a zero-based party slot in VAR_0x8004 and return a boolean.
bool32 Special_ShadowPokemon_IsShadow(void)
{
    struct Pokemon *mon = GetSpecialShadowMon();
    return mon != NULL && IsShadowPokemon(mon);
}

bool32 Special_ShadowPokemon_CanPurify(void)
{
    struct Pokemon *mon = GetSpecialShadowMon();
    return mon != NULL && CanPurifyShadowPokemon(mon);
}

bool32 Special_ShadowPokemon_Purify(void)
{
    struct Pokemon *mon = GetSpecialShadowMon();
    return mon != NULL && PurifyShadowPokemon(mon);
}

bool32 Special_ShadowPokemon_Mark(void)
{
    struct Pokemon *mon = GetSpecialShadowMon();
    return !gMain.inBattle && mon != NULL && MarkShadowPokemon(mon);
}

// Call immediately after setwildbattle, before dowildbattle.
void Special_ShadowPokemon_MarkWild(void)
{
    if (!gMain.inBattle)
        MarkShadowPokemon(&gParties[B_TRAINER_OPPONENT_A][0]);
}
