#ifndef GUARD_GC_COMPANION_POKEMON_H
#define GUARD_GC_COMPANION_POKEMON_H

// Pokemon application of GC Extension Mode: versioned party records, front
// sprites resolved with the game's own loaders, and text labels rendered with
// the game's font. Read-only with respect to party, bag and save data.

#include "gc_ext/pkmn_protocol.h"
#include "gc_companion_protocol.h"

struct GcPokemonUi
{
    void (*getUi)(PartyUi *ui);
    void (*setPhase)(u8 phase, u8 done, u8 total);
};

struct GcStateDigest
{
    u32 crc[STATE_DIGEST_FIELDS]; // party, bag, flags, vars, save block 1
};

bool8 GcPokemon_Init(u8 labelWindowId, const struct GcPokemonUi *ui);
void GcPokemon_Free(void);
const struct GcClientConfig *GcPokemon_GetClientConfig(void);
void GcPokemon_ComputeDigest(struct GcStateDigest *digest);
// Bit n set when digest field n equals the snapshot taken at GcPokemon_Init.
u32 GcPokemon_CompareWithEntry(const struct GcStateDigest *digest);
u32 GcPokemon_GetBuildId(void);
// Export slots 0..5 = player party, GC_EXTRA_SLOT_BASE + i = registered extras.
void GcPokemon_SetExtraMons(struct Pokemon *mons, u8 count);
struct Pokemon *GcPokemon_GetMon(u32 slot);
bool8 GcPokemon_BuildRecord(u32 slot, PokemonRecord *record);

#endif // GUARD_GC_COMPANION_POKEMON_H
