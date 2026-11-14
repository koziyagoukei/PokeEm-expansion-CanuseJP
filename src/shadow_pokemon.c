#include "global.h"
#include "battle.h"
#include "battle_util.h"
#include "caps.h"
#include "config/shadow_pokemon.h"
#include "event_data.h"
#include "evolution_scene.h"
#include "main.h"
#include "overworld.h"
#include "move.h"
#include "party_menu.h"
#include "pokemon.h"
#include "random.h"
#include "shadow_pokemon.h"
#include "test_runner.h"
#include "constants/vars.h"

/*
    Shadow Pokemon (Pokemon Colosseum / XD).

    Save data, meaningful only while the Pokemon carries the hasShadowState tag (MON_DATA_HAS_SHADOW_STATE):
    - MON_DATA_SHADOW_GAUGE: the heart gauge left (Pokemon XD), or SHADOW_GAUGE_PURIFIED once purified.
    - MON_DATA_SHADOW_HELD_EXP: EXP held until purification, in SHADOW_HELD_EXP_UNIT steps.
    - MON_DATA_SHADOW_HYPER_MODE / MON_DATA_SHADOW_REVERSE_MODE: Hyper Mode (Colosseum) and Reverse Mode (XD). Both
      last after the battle, until the Pokemon is called back, faints or is purified.
    A Pokemon without the tag is an ordinary Pokemon whatever those bits hold.
*/

#include "data/shadow_pokemon.h"
#include "data/shadow_purify_moves.h"

// Per-nature heart gauge reductions of Pokemon XD: battle, Call, 256 steps, scent.
static const u16 sShadowGaugeReduction[NUM_NATURES][SHADOW_ACTION_COUNT] =
{
    [NATURE_HARDY]   = {110, 300, 100,  90},
    [NATURE_LONELY]  = { 70, 330, 100, 130},
    [NATURE_BRAVE]   = {130, 270,  90,  80},
    [NATURE_ADAMANT] = {110, 270, 110,  80},
    [NATURE_NAUGHTY] = {120, 270, 110,  70},
    [NATURE_BOLD]    = {110, 270,  90, 100},
    [NATURE_DOCILE]  = {100, 360,  80, 120},
    [NATURE_RELAXED] = { 90, 270, 110, 100},
    [NATURE_IMPISH]  = {120, 300, 100,  80},
    [NATURE_LAX]     = {100, 270,  90, 110},
    [NATURE_TIMID]   = { 70, 330, 110, 120},
    [NATURE_HASTY]   = {130, 300,  70, 100},
    [NATURE_SERIOUS] = {100, 330, 110,  90},
    [NATURE_JOLLY]   = {120, 300,  90,  90},
    [NATURE_NAIVE]   = {100, 300, 120,  80},
    [NATURE_MODEST]  = { 70, 300, 120, 110},
    [NATURE_MILD]    = { 80, 270, 100, 120},
    [NATURE_QUIET]   = {100, 300, 100, 100},
    [NATURE_BASHFUL] = { 80, 270,  90, 130},
    [NATURE_RASH]    = { 90, 300,  90, 120},
    [NATURE_CALM]    = { 80, 300, 110, 110},
    [NATURE_GENTLE]  = { 70, 300, 100, 100},
    [NATURE_SASSY]   = {130, 240, 100,  90},
    [NATURE_CAREFUL] = { 90, 300, 100, 110},
    [NATURE_QUIRKY]  = {130, 270,  80,  90},
};

// Pokemon XD: % chance of Reverse Mode after a Shadow move, by aggression (1 very high .. 5 very low) and by how much
// of the gauge is left (column 0: 99.9% or more, 1: 80%, 2: 60%, 3: 40%, 4: less).
static const u8 sShadowReverseChance[5][5] =
{
    {0, 10, 20, 35, 50},
    {0, 10, 15, 25, 40},
    {0, 10, 15, 20, 30},
    {0,  5, 10, 15, 20},
    {0,  2,  5, 10, 15},
};

// Colosseum: what a Pokemon in Hyper Mode does when told to use a move that is not a Shadow move, in % by nature
// (enum ShadowHyperAction order: random move, attack the foe's Trainer, attack its own Trainer, hit itself, nothing,
// use its held item, return to its Poke Ball, obey).
static const u16 sHyperModeActionJolly[SHADOW_HYPER_ACTION_COUNT]   = {40,  5,  5,  5,  5, 25,  5, 10};
static const u16 sHyperModeActionMild[SHADOW_HYPER_ACTION_COUNT]    = {20, 10, 10, 10, 10, 10, 20, 10};
static const u16 sHyperModeActionBashful[SHADOW_HYPER_ACTION_COUNT] = {20, 20, 20, 10,  5,  5, 10, 10};
static const u16 sHyperModeActionSassy[SHADOW_HYPER_ACTION_COUNT]   = {30, 10,  5,  5, 30,  5,  5, 10};
static const u16 sHyperModeActionRash[SHADOW_HYPER_ACTION_COUNT]    = { 5, 10,  5, 20, 20,  5, 25, 10};
static const u16 sHyperModeActionModest[SHADOW_HYPER_ACTION_COUNT]  = {12, 12, 12, 12, 12, 12, 12, 16};

static const u16 *const sHyperModeActions[NUM_NATURES] =
{
    [NATURE_JOLLY]   = sHyperModeActionJolly,
    [NATURE_MILD]    = sHyperModeActionMild,
    [NATURE_NAIVE]   = sHyperModeActionMild,
    [NATURE_GENTLE]  = sHyperModeActionMild,
    [NATURE_HARDY]   = sHyperModeActionMild,
    [NATURE_BASHFUL] = sHyperModeActionBashful,
    [NATURE_ADAMANT] = sHyperModeActionBashful,
    [NATURE_RELAXED] = sHyperModeActionBashful,
    [NATURE_BRAVE]   = sHyperModeActionBashful,
    [NATURE_SASSY]   = sHyperModeActionSassy,
    [NATURE_BOLD]    = sHyperModeActionSassy,
    [NATURE_QUIRKY]  = sHyperModeActionSassy,
    [NATURE_LAX]     = sHyperModeActionSassy,
    [NATURE_NAUGHTY] = sHyperModeActionSassy,
    [NATURE_RASH]    = sHyperModeActionRash,
    [NATURE_IMPISH]  = sHyperModeActionRash,
    [NATURE_HASTY]   = sHyperModeActionRash,
    [NATURE_LONELY]  = sHyperModeActionRash,
    [NATURE_MODEST]  = sHyperModeActionModest,
    [NATURE_TIMID]   = sHyperModeActionModest,
    [NATURE_CALM]    = sHyperModeActionModest,
    [NATURE_QUIET]   = sHyperModeActionModest,
    // Not in the source table: the even spread of Modest and the others
    [NATURE_SERIOUS] = sHyperModeActionModest,
    [NATURE_CAREFUL] = sHyperModeActionModest,
    [NATURE_DOCILE]  = sHyperModeActionModest,
};

// ---- the species table ------------------------------------------------------------------------------------------------

static const struct ShadowPokemonInfo *GetShadowPokemonInfo(enum Species species)
{
    enum Species base = GET_BASE_SPECIES_ID(species);
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sShadowPokemonInfo); i++)
    {
        if (sShadowPokemonInfo[i].species == species)
            return &sShadowPokemonInfo[i];
    }
    for (i = 0; i < ARRAY_COUNT(sShadowPokemonInfo); i++)
    {
        if (sShadowPokemonInfo[i].species == base)
            return &sShadowPokemonInfo[i];
    }
    return NULL;
}

static u32 GetSpeciesShadowGaugeMax(enum Species species)
{
    const struct ShadowPokemonInfo *info = GetShadowPokemonInfo(species);
    u32 max = (info != NULL && info->heartGauge != 0) ? info->heartGauge : SHADOW_DEFAULT_HEART_GAUGE;
    return min(max, (u32)SHADOW_GAUGE_MAX_VALUE);
}

static bool32 SpeciesHasType(enum Species species, enum Type type)
{
    return GetSpeciesType(species, 0) == type || GetSpeciesType(species, 1) == type;
}

// The Shadow moves of a species without its own (1-3), from its types and base stats:
// - Shadow Rush, as most of Pokemon XD's Shadow Pokemon have;
// - a Fire, Electric or Ice type's Shadow move (Shadow Fire / Bolt / Chill);
// - else a special attacker's spread move (Shadow Wave, Rave or Storm by base stat total), or a strong physical
//   attacker's harder hit (Shadow Break; Shadow Blast from SHADOW_DEFAULT_RARE_BST);
// - from SHADOW_DEFAULT_STRONG_BST, a support move: Shadow Down for a bulky one, Shadow Panic for a fast one.
static u32 GetDefaultShadowMoves(enum Species species, enum Move moves[MAX_MON_MOVES])
{
    u32 count = 0, total = GetSpeciesBaseStatTotal(species);
    u32 attack = GetSpeciesBaseStat(species, STAT_ATK), spAttack = GetSpeciesBaseStat(species, STAT_SPATK);
    u32 speed = GetSpeciesBaseStat(species, STAT_SPEED);
    u32 bulk = GetSpeciesBaseStat(species, STAT_HP) + GetSpeciesBaseStat(species, STAT_DEF) + GetSpeciesBaseStat(species, STAT_SPDEF);

    moves[count++] = MOVE_SHADOW_RUSH;
    if (SpeciesHasType(species, TYPE_FIRE))
        moves[count++] = MOVE_SHADOW_FIRE;
    else if (SpeciesHasType(species, TYPE_ELECTRIC))
        moves[count++] = MOVE_SHADOW_BOLT;
    else if (SpeciesHasType(species, TYPE_ICE))
        moves[count++] = MOVE_SHADOW_CHILL;
    else if (spAttack > attack)
        moves[count++] = total >= SHADOW_DEFAULT_RARE_BST ? MOVE_SHADOW_STORM : total >= SHADOW_DEFAULT_STRONG_BST ? MOVE_SHADOW_RAVE : MOVE_SHADOW_WAVE;
    else if (total >= SHADOW_DEFAULT_STRONG_BST)
        moves[count++] = total >= SHADOW_DEFAULT_RARE_BST ? MOVE_SHADOW_BLAST : MOVE_SHADOW_BREAK;

    if (total >= SHADOW_DEFAULT_STRONG_BST)
    {
        if (bulk * 100 >= total * 55)
            moves[count++] = MOVE_SHADOW_DOWN;
        else if (speed >= attack && speed >= spAttack)
            moves[count++] = MOVE_SHADOW_PANIC;
    }
    return count;
}

// The Shadow moves of a species: its row (or its base species' row), else GetDefaultShadowMoves. Returns how many
// (1-4), and the moves in moves[].
u32 GetShadowMovesForSpecies(enum Species species, enum Move moves[MAX_MON_MOVES])
{
    u32 i, count = 0;
    const struct ShadowPokemonInfo *info = GetShadowPokemonInfo(species);

    for (i = 0; i < MAX_MON_MOVES; i++)
        moves[i] = MOVE_NONE;

    if (info != NULL)
    {
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            if (IsShadowMove(info->moves[i]))
                moves[count++] = info->moves[i];
        }
    }

    if (count == 0)
        count = GetDefaultShadowMoves(species, moves);
    return count;
}

// ---- state ------------------------------------------------------------------------------------------------------------

static bool32 HasShadowTag(struct Pokemon *mon)
{
    return GetMonData(mon, MON_DATA_HAS_SHADOW_STATE)
        && GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE
        && GetMonData(mon, MON_DATA_SPECIES_OR_EGG) != SPECIES_EGG;
}

u32 GetShadowGaugeMax(struct Pokemon *mon)
{
    return GetSpeciesShadowGaugeMax(GetMonData(mon, MON_DATA_SPECIES));
}

u32 GetShadowGauge(struct Pokemon *mon)
{
    u32 gauge;

    if (!HasShadowTag(mon))
        return 0;
    gauge = GetMonData(mon, MON_DATA_SHADOW_GAUGE);
    if (gauge == SHADOW_GAUGE_PURIFIED)
        return 0;
    return min(gauge, GetShadowGaugeMax(mon));
}

static void SetShadowGauge(struct Pokemon *mon, u32 gauge)
{
    u16 value = gauge;
    SetMonData(mon, MON_DATA_SHADOW_GAUGE, &value);
}

enum ShadowState GetShadowPokemonState(struct Pokemon *mon)
{
    if (!HasShadowTag(mon))
        return SHADOW_STATE_NORMAL;
    if (GetMonData(mon, MON_DATA_SHADOW_GAUGE) == SHADOW_GAUGE_PURIFIED)
        return SHADOW_STATE_PURIFIED;
    return SHADOW_STATE_STAGE_0 + GetShadowHeartGauge(mon);
}

bool32 IsShadowPokemon(struct Pokemon *mon)
{
    return HasShadowTag(mon) && GetMonData(mon, MON_DATA_SHADOW_GAUGE) != SHADOW_GAUGE_PURIFIED;
}

bool32 IsShadowBoxPokemon(struct BoxPokemon *boxMon)
{
    return GetBoxMonData(boxMon, MON_DATA_HAS_SHADOW_STATE)
        && GetBoxMonData(boxMon, MON_DATA_SHADOW_GAUGE) != SHADOW_GAUGE_PURIFIED
        && GetBoxMonData(boxMon, MON_DATA_SPECIES) != SPECIES_NONE
        && GetBoxMonData(boxMon, MON_DATA_SPECIES_OR_EGG) != SPECIES_EGG;
}

// Makes a Pokemon a Shadow Pokemon with a full gauge. Every field of the Shadow state is written, so whatever the unused
// bits held before does not matter. A Pokemon that already has a Shadow state (or was purified) is left alone.
bool32 MarkShadowPokemon(struct Pokemon *mon)
{
    const bool8 tag = TRUE;
    const u16 zero = 0;
    const u8 off = FALSE;

    if (GetMonData(mon, MON_DATA_HAS_SHADOW_STATE)
     || GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE
     || GetMonData(mon, MON_DATA_SPECIES_OR_EGG) == SPECIES_EGG)
        return FALSE;

    SetMonData(mon, MON_DATA_HAS_SHADOW_STATE, &tag);
    SetShadowGauge(mon, GetShadowGaugeMax(mon));
    SetMonData(mon, MON_DATA_SHADOW_HELD_EXP, &zero);
    SetMonData(mon, MON_DATA_SHADOW_HYPER_MODE, &off);
    SetMonData(mon, MON_DATA_SHADOW_REVERSE_MODE, &off);
    return TRUE;
}

// Bars of the heart gauge cleared, 0-5 (Pokemon XD: a bar still shows while any of it is left). 5 = empty: the Pokemon
// can be purified.
u32 GetShadowHeartGauge(struct Pokemon *mon)
{
    u32 gauge, max;

    if (!IsShadowPokemon(mon))
        return 0;
    gauge = GetShadowGauge(mon);
    max = GetShadowGaugeMax(mon);
    return SHADOW_GAUGE_BARS - (gauge * SHADOW_GAUGE_BARS + max - 1) / max;
}

// Clears more bars at once (debug and tests): the gauge drops to the end of the bar reached.
void AddShadowHeartGauge(struct Pokemon *mon, u32 bars)
{
    u32 max, target;

    if (!IsShadowPokemon(mon))
        return;
    max = GetShadowGaugeMax(mon);
    target = min(GetShadowHeartGauge(mon) + bars, (u32)SHADOW_GAUGE_BARS);
    SetShadowGauge(mon, min(GetShadowGauge(mon), max * (SHADOW_GAUGE_BARS - target) / SHADOW_GAUGE_BARS));
}

u32 GetShadowGaugeReduction(struct Pokemon *mon, enum ShadowAction action)
{
    if (action >= SHADOW_ACTION_COUNT)
        return 0;
    return sShadowGaugeReduction[GetNature(mon)][action];
}

// Lowers the heart gauge by an amount. Returns how much it went down.
u32 ReduceShadowHeartGaugeBy(struct Pokemon *mon, u32 amount)
{
    u32 gauge;

    if (!IsShadowPokemon(mon))
        return 0;
    gauge = GetShadowGauge(mon);
    amount = min(amount, gauge);
    SetShadowGauge(mon, gauge - amount);
    return amount;
}

// Lowers the heart gauge for an action (Pokemon XD's per-nature amounts). Returns how much it went down.
u32 ReduceShadowHeartGauge(struct Pokemon *mon, enum ShadowAction action, u32 times)
{
    return ReduceShadowHeartGaugeBy(mon, GetShadowGaugeReduction(mon, action) * times);
}

u32 GetShadowAggression(struct Pokemon *mon)
{
    const struct ShadowPokemonInfo *info = GetShadowPokemonInfo(GetMonData(mon, MON_DATA_SPECIES));
    u32 aggression = info != NULL ? info->aggression : 0;
    return (aggression >= 1 && aggression <= 5) ? aggression : SHADOW_DEFAULT_AGGRESSION;
}

bool32 IsShadowPokemonInHyperMode(struct Pokemon *mon)
{
    return IsShadowPokemon(mon) && GetMonData(mon, MON_DATA_SHADOW_HYPER_MODE);
}

bool32 IsShadowPokemonInReverseMode(struct Pokemon *mon)
{
    return IsShadowPokemon(mon) && GetMonData(mon, MON_DATA_SHADOW_REVERSE_MODE);
}

void SetShadowPokemonHyperMode(struct Pokemon *mon, bool32 on)
{
    u8 value = on && IsShadowPokemon(mon);
    SetMonData(mon, MON_DATA_SHADOW_HYPER_MODE, &value);
}

void SetShadowPokemonReverseMode(struct Pokemon *mon, bool32 on)
{
    u8 value = on && IsShadowPokemon(mon);
    SetMonData(mon, MON_DATA_SHADOW_REVERSE_MODE, &value);
}

void SetShadowBoxPokemonHyperMode(struct BoxPokemon *boxMon, bool32 on)
{
    u8 value = on && IsShadowBoxPokemon(boxMon);
    SetBoxMonData(boxMon, MON_DATA_SHADOW_HYPER_MODE, &value);
}

u32 GetShadowHeldExp(struct Pokemon *mon)
{
    if (!IsShadowPokemon(mon))
        return 0;
    return GetMonData(mon, MON_DATA_SHADOW_HELD_EXP) * SHADOW_HELD_EXP_UNIT;
}

// A Shadow Pokemon gains no EXP. Pokemon XD keeps 80% of it and pays it out at purification.
void HoldShadowPokemonExp(struct Pokemon *mon, u32 exp)
{
    u32 units;

    if (!IsShadowPokemon(mon) || exp == 0)
        return;
    exp = exp * SHADOW_HELD_EXP_PERCENT / 100;
    // Rounded to the nearest step, so small amounts are not always lost
    units = GetMonData(mon, MON_DATA_SHADOW_HELD_EXP) + (exp + SHADOW_HELD_EXP_UNIT / 2) / SHADOW_HELD_EXP_UNIT;
    units = min(units, 0x3FFFu);
    SetMonData(mon, MON_DATA_SHADOW_HELD_EXP, &units);
}

bool32 CanPurifyShadowPokemon(struct Pokemon *mon)
{
    return IsShadowPokemon(mon) && GetShadowGauge(mon) == 0;
}

static void LearnShadowPurificationMoves(struct Pokemon *mon, u32 oldLevel, u32 newLevel)
{
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(GetMonData(mon, MON_DATA_SPECIES));

    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
    {
        if (learnset[i].level > oldLevel && learnset[i].level <= newLevel)
            GiveMoveToMon(mon, learnset[i].move); // A full move list keeps its moves (the Move Relearner has them)
    }
}

// Purification (an event's special only; an empty gauge never purifies by itself):
// the Shadow state ends (Hyper and Reverse Mode too), the moves under the Shadow moves come back, the held EXP is
// given once (up to the level cap), moves of the levels it reached are learned while there is room, and it gets the
// National Ribbon (given to purified Shadow Pokemon in Colosseum/XD). The purified mark stays.
static bool32 PurifyShadowPokemonInternal(struct Pokemon *mon, bool32 learnPurifyMove)
{
    const u16 purified = SHADOW_GAUGE_PURIFIED;
    const u16 zero = 0;
    const u8 off = FALSE, on = TRUE;
    u32 oldLevel, newLevel, exp, maxExp, held;

    if (gMain.inBattle || !CanPurifyShadowPokemon(mon))
        return FALSE;

    held = GetShadowHeldExp(mon);
    oldLevel = GetMonData(mon, MON_DATA_LEVEL);
    SetMonData(mon, MON_DATA_SHADOW_HYPER_MODE, &off);
    SetMonData(mon, MON_DATA_SHADOW_REVERSE_MODE, &off);
    SetMonData(mon, MON_DATA_SHADOW_HELD_EXP, &zero);
    SetMonData(mon, MON_DATA_SHADOW_GAUGE, &purified);

    exp = GetMonData(mon, MON_DATA_EXP);
    maxExp = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][GetCurrentLevelCap()];
    if (exp < maxExp)
    {
        exp = min(exp + held, maxExp);
        SetMonData(mon, MON_DATA_EXP, &exp);
    }
    CalculateMonStats(mon);
    newLevel = GetMonData(mon, MON_DATA_LEVEL);
    if (newLevel > oldLevel)
        LearnShadowPurificationMoves(mon, oldLevel, newLevel);

    SetMonData(mon, MON_DATA_NATIONAL_RIBBON, &on);

    // A Battle Connector Shadow Pokemon also learns its purification move, while there is room (the scene offers to
    // forget a move for it)
    if (learnPurifyMove)
    {
        enum Move move = GetShadowPurifyMove(mon);
        if (move != MOVE_NONE && !MonKnowsMove(mon, move))
            GiveMoveToMon(mon, move);
    }
    return TRUE;
}

bool32 PurifyShadowPokemon(struct Pokemon *mon)
{
    return PurifyShadowPokemonInternal(mon, TRUE);
}

// For the purification scene, which teaches the purification move itself (ShadowTryLearningPurifyMove)
bool32 PurifyShadowPokemonInScene(struct Pokemon *mon)
{
    return PurifyShadowPokemonInternal(mon, FALSE);
}

// The move a Shadow Pokemon of the Battle Connector learns when purified: from its species' Shadow row (the bosses) or
// src/data/shadow_purify_moves.h, one of up to two picked by its personality (so it is the same each time it is
// asked). MOVE_NONE for any other Pokemon.
static const u16 *GetSpeciesPurifyMoves(enum Species species)
{
    const struct ShadowPokemonInfo *info = GetShadowPokemonInfo(species);

    if (info != NULL && info->purifyMoves[0] != MOVE_NONE)
        return info->purifyMoves;
    // The room sets use base forms. Keep their reward available after an appliance or other form change.
    enum Species baseSpecies = GET_BASE_SPECIES_ID(species);
    for (u32 i = 0; i < ARRAY_COUNT(sShadowPurifyMoves); i++)
    {
        if (sShadowPurifyMoves[i].species == baseSpecies)
            return sShadowPurifyMoves[i].moves;
    }
    return NULL;
}

u32 GetSpeciesPurifyMoveCount(enum Species species)
{
    const u16 *moves = GetSpeciesPurifyMoves(species);
    u32 count = 0;

    while (moves != NULL && count < 2 && moves[count] != MOVE_NONE)
        count++;
    return count;
}

enum Move GetShadowPurifyMove(struct Pokemon *mon)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u32 count;

    if (!GetMonData(mon, MON_DATA_HAS_SHADOW_STATE) || !GetMonData(mon, MON_DATA_BATTLE_CONNECTOR_ORIGIN))
        return MOVE_NONE;
    count = GetSpeciesPurifyMoveCount(species);
    if (count == 0)
        return MOVE_NONE;
    return GetSpeciesPurifyMoves(species)[(GetMonData(mon, MON_DATA_PERSONALITY) >> 8) % count];
}

// A purified Battle Connector Pokemon's purification move, so the Move Reminder can teach it again (declined in the
// purification scene, or forgotten later). MOVE_NONE for any other Pokemon.
enum Move GetBoxMonPurifiedConnectorMove(struct BoxPokemon *boxMon)
{
    enum Species species = GetBoxMonData(boxMon, MON_DATA_SPECIES);
    u32 count;

    if (!GetBoxMonData(boxMon, MON_DATA_HAS_SHADOW_STATE) || !GetBoxMonData(boxMon, MON_DATA_BATTLE_CONNECTOR_ORIGIN)
     || GetBoxMonData(boxMon, MON_DATA_SHADOW_GAUGE) != SHADOW_GAUGE_PURIFIED)
        return MOVE_NONE;
    count = GetSpeciesPurifyMoveCount(species);
    if (count == 0)
        return MOVE_NONE;
    return GetSpeciesPurifyMoves(species)[(GetBoxMonData(boxMon, MON_DATA_PERSONALITY) >> 8) % count];
}

// The purification scene's move to learn (like MonTryLearningNewMoveEvolution): the purification move, once
u32 ShadowTryLearningPurifyMove(struct Pokemon *mon, bool32 firstMove)
{
    enum Move move;

    if (!firstMove)
        return MOVE_NONE;
    move = GetShadowPurifyMove(mon);
    if (move == MOVE_NONE || MonKnowsMove(mon, move))
        return MOVE_NONE;
    gMoveToLearn = move;
    return GiveMoveToMon(mon, move);
}

// The lasting mark of a Battle Connector Shadow Pokemon that was purified
bool32 IsBattleConnectorPurified(struct Pokemon *mon)
{
    return GetShadowPokemonState(mon) == SHADOW_STATE_PURIFIED && GetMonData(mon, MON_DATA_BATTLE_CONNECTOR_ORIGIN);
}

// The other Pokemon in the party that would gather round a Shadow Pokemon to purify it: not Shadow Pokemon nor Eggs
// (purified ones count).
u32 CountShadowPurifyPartners(u32 partySlot)
{
    u32 count = 0;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (i != partySlot
         && GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE
         && !GetMonData(mon, MON_DATA_IS_EGG)
         && !IsShadowPokemon(mon))
            count++;
    }
    return count;
}

// Every 256 steps, each Shadow Pokemon in the party opens up a little (Pokemon XD).
void ShadowPokemon_OnStep(void)
{
    u16 *steps = GetVarPointer(VAR_SHADOW_STEP_COUNTER);

    *steps = (*steps + 1) % 256;
    if (*steps != 0)
        return;

    for (u32 i = 0; i < PARTY_SIZE; i++)
        ReduceShadowHeartGauge(&gParties[B_TRAINER_PLAYER][i], SHADOW_ACTION_WALK, 1);
}

// A meal at the dining room works like one of Pokemon XD's scents.
void ShadowPokemon_OnMeal(struct Pokemon *mon)
{
    ReduceShadowHeartGauge(mon, SHADOW_ACTION_SCENT, 1);
}

// Every PARTY_BATH_INTERVAL (5 seconds) in the party bath, each Shadow Pokemon in the party relaxes a little.
void ShadowPokemon_OnBathTick(void)
{
    for (u32 i = 0; i < PARTY_SIZE; i++)
        ReduceShadowHeartGaugeBy(&gParties[B_TRAINER_PLAYER][i], SHADOW_BATH_GAUGE_REDUCTION);
}

// ---- move layout ------------------------------------------------------------------------------------------------------

// The n-th own move after the Shadow moves (0 = first) opens with this many bars cleared; the others stay sealed until
// purification (Pokemon XD).
static bool32 IsShadowOwnMoveOpen(u32 ownIndex, u32 barsCleared)
{
    if (ownIndex == 0)
        return barsCleared >= SHADOW_OWN_MOVE_1_STAGE;
    if (ownIndex == 1)
        return barsCleared >= SHADOW_OWN_MOVE_2_STAGE;
    return FALSE;
}

// What a Shadow Pokemon has in each move slot, Pokemon XD style: its Shadow moves in the first slots, then its own
// moves from the slot after them. With sealOwnMoves, own moves the heart has not opened yet are left out
// (MOVE_NONE, sealedSlots). The moves in the Pokemon's data are never changed: the slots under the Shadow moves come
// back when it is purified, and so do the sealed ones.
void GetShadowMoveLayout(struct Pokemon *mon, bool32 sealOwnMoves, struct ShadowMoveLayout *layout)
{
    enum Move shadowMoves[MAX_MON_MOVES];
    u32 i, count = 0, bars;

    layout->shadowSlots = 0;
    layout->sealedSlots = 0;
    for (i = 0; i < MAX_MON_MOVES; i++)
        layout->moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);

    if (!IsShadowPokemon(mon))
        return;

    count = GetShadowMovesForSpecies(GetMonData(mon, MON_DATA_SPECIES), shadowMoves);
    bars = GetShadowHeartGauge(mon);
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (i < count)
        {
            layout->moves[i] = shadowMoves[i];
            layout->shadowSlots |= 1u << i;
        }
        else if (sealOwnMoves && layout->moves[i] != MOVE_NONE && !IsShadowOwnMoveOpen(i - count, bars))
        {
            layout->moves[i] = MOVE_NONE;
            layout->sealedSlots |= 1u << i;
        }
    }
}

// The player's own Shadow Pokemon keep their own moves sealed and can go into Hyper or Reverse Mode; a Trainer's
// Shadow Pokemon use their own moves and do neither.
static bool32 IsPlayerPartyMon(struct Pokemon *mon)
{
    return mon >= &gParties[B_TRAINER_PLAYER][0] && mon < &gParties[B_TRAINER_PLAYER][PARTY_SIZE];
}

// ---- battle -----------------------------------------------------------------------------------------------------------

bool32 UsesShadowBattleMoves(void)
{
    u32 flags = gBattleTypeFlags;
    // The battle test harness feeds local actions through recorded playback.
    if (TESTING && gTestRunnerEnabled)
        flags &= ~(BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK);

    // Call is local input, not part of the link or recorded action stream.
    return !(flags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED | BATTLE_TYPE_RECORDED_LINK
                    | BATTLE_TYPE_TOWER_LINK_MULTI | BATTLE_TYPE_SAFARI | BATTLE_TYPE_CATCH_TUTORIAL | BATTLE_TYPE_POKEDUDE));
}

bool32 IsBattlerShadowPokemon(enum BattlerId battler)
{
    return gBattlerPartyIndexes[battler] < PARTY_SIZE && IsShadowPokemon(GetBattlerMon(battler));
}

// One of the player's own Shadow Pokemon, in a battle where the Shadow rules apply.
bool32 IsPlayerShadowBattler(enum BattlerId battler)
{
    return UsesShadowBattleMoves()
        && gBattlerPartyIndexes[battler] < PARTY_SIZE
        && IsPlayerPartyMon(GetBattlerMon(battler))
        && IsShadowPokemon(GetBattlerMon(battler));
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

// The move slots whose battle move is not the one in the Pokemon's data (Shadow moves and sealed moves). Writes back
// to the party skip them.
u32 GetShadowSealedMoveMask(struct Pokemon *mon)
{
    struct ShadowMoveLayout layout;

    if (!UsesShadowBattleMoves() || !IsShadowPokemon(mon))
        return 0;

    GetShadowMoveLayout(mon, IsPlayerPartyMon(mon), &layout);
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
    GetShadowMoveLayout(mon, IsPlayerPartyMon(mon), &layout);
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
    GetShadowMoveLayout(mon, IsPlayerPartyMon(mon), &layout);
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

// The heart gauge went down in battle (Call): the own moves among sealedBefore that are open now get their move and
// PP from the Pokemon's data, so they can be chosen from the next turn.
void OpenShadowBattleMoves(enum BattlerId battler, u32 sealedBefore)
{
    struct Pokemon *mon;
    u32 opened, ppBonuses;

    if (sealedBefore == 0)
        return;
    mon = GetBattlerMon(battler);
    opened = sealedBefore & ~GetBattlerShadowSealedSlots(battler);
    ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        if (!(opened & (1u << i)))
            continue;
        gBattleMons[battler].moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
        gBattleMons[battler].pp[i] = GetMonData(mon, MON_DATA_PP1 + i);
        gBattleMons[battler].ppBonuses = (gBattleMons[battler].ppBonuses & ~(3 << (2 * i))) | (ppBonuses & (3 << (2 * i)));
    }
}

// After the battle, each Shadow Pokemon of the player that took part opens up once (Pokemon XD: once per battle it
// was sent out in, whatever the outcome). Facility, link and special battles do not count.
void AdvanceParticipatingShadowPokemon(void)
{
    if (gBattleStruct->shadowProgressApplied)
        return;
    gBattleStruct->shadowProgressApplied = TRUE;

    if (!UsesShadowBattleMoves()
     || gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TOWER_LINK_MULTI | BATTLE_TYPE_TRAINER_HILL
                          | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_SECRET_BASE))
        return;

    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (gBattleStruct->partyState[B_TRAINER_PLAYER][i].sentOut)
            ReduceShadowHeartGauge(&gParties[B_TRAINER_PLAYER][i], SHADOW_ACTION_BATTLE, 1);
    }
}

bool32 IsBattlerInHyperMode(enum BattlerId battler)
{
    return gBattlerPartyIndexes[battler] < PARTY_SIZE && IsShadowPokemonInHyperMode(GetBattlerMon(battler));
}

bool32 IsBattlerInReverseMode(enum BattlerId battler)
{
    return gBattlerPartyIndexes[battler] < PARTY_SIZE && IsShadowPokemonInReverseMode(GetBattlerMon(battler));
}

// "Call" (R on the action menu) is offered for the player's own Shadow Pokemon.
bool32 CanBattlerCallShadowPokemon(enum BattlerId battler)
{
    return IsPlayerShadowBattler(battler);
}

// Colosseum: how readily a Pokemon goes into Hyper Mode depends on its nature. The source gives the natures, not the
// numbers: SHADOW_HYPER_MODE_CHANCE_* in include/config/shadow_pokemon.h.
u32 GetShadowHyperModeChance(struct Pokemon *mon)
{
    switch (GetNature(mon))
    {
    case NATURE_JOLLY:
    case NATURE_MILD:
    case NATURE_NAIVE:
    case NATURE_GENTLE:
    case NATURE_HARDY:
    case NATURE_BASHFUL:
    case NATURE_ADAMANT:
    case NATURE_RELAXED:
        return SHADOW_HYPER_MODE_CHANCE_HIGH;
    case NATURE_LONELY:
    case NATURE_MODEST:
    case NATURE_TIMID:
    case NATURE_CALM:
    case NATURE_QUIET:
    case NATURE_SERIOUS:
    case NATURE_CAREFUL:
    case NATURE_DOCILE:
        return SHADOW_HYPER_MODE_CHANCE_LOW;
    default:
        return SHADOW_HYPER_MODE_CHANCE_NORMAL;
    }
}

enum ShadowHyperAction GetShadowHyperModeAction(struct Pokemon *mon)
{
    return RandomWeightedArray(RNG_SHADOW_HYPER_ACTION, 100, SHADOW_HYPER_ACTION_COUNT, sHyperModeActions[GetNature(mon)]);
}

// Pokemon XD: % chance of Reverse Mode at the end of a turn the Pokemon used a Shadow move.
u32 GetShadowReverseModeChance(struct Pokemon *mon)
{
    u32 gauge, max, column;

    if (!IsShadowPokemon(mon))
        return 0;
    gauge = GetShadowGauge(mon);
    max = GetShadowGaugeMax(mon);
    if ((max - gauge) * 1000 <= max)  // 99.9% or more left
        column = 0;
    else if (gauge * 5 >= max * 4)
        column = 1;
    else if (gauge * 5 >= max * 3)
        column = 2;
    else if (gauge * 5 >= max * 2)
        column = 3;
    else
        column = 4;
    return sShadowReverseChance[GetShadowAggression(mon) - 1][column];
}

// A fainted Shadow Pokemon comes out of Hyper and Reverse Mode.
void ClearShadowModesOnFaint(enum BattlerId battler)
{
    struct Pokemon *mon;

    if (gBattlerPartyIndexes[battler] >= PARTY_SIZE)
        return;
    mon = GetBattlerMon(battler);
    if (!IsPlayerPartyMon(mon) || !IsShadowPokemon(mon))
        return;
    SetShadowPokemonHyperMode(mon, FALSE);
    SetShadowPokemonReverseMode(mon, FALSE);
}

// ---- specials ---------------------------------------------------------------------------------------------------------

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

// Plays the purification scene (the evolution scene's) for the party slot in VAR_0x8004 and purifies the Pokemon.
// The script waits with waitstate; it carries on in the field afterwards (also when the Pokemon cannot be purified).
void Special_ShadowPokemon_StartPurificationScene(void)
{
    if (gSpecialVar_0x8004 >= PARTY_SIZE || !CanPurifyShadowPokemon(&gParties[B_TRAINER_PLAYER][gSpecialVar_0x8004]))
    {
        SetMainCallback2(CB2_ReturnToFieldContinueScript);
        return;
    }
    gCB2_AfterEvolution = CB2_ReturnToFieldContinueScriptPlayMapMusic;
    BeginPurificationScene(gSpecialVar_0x8004);
}

// Call immediately after setwildbattle, before dowildbattle.
void Special_ShadowPokemon_MarkWild(void)
{
    if (!gMain.inBattle)
        MarkShadowPokemon(&gParties[B_TRAINER_OPPONENT_A][0]);
}
