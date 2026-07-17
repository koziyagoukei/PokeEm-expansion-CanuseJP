#include "global.h"
#include "constants/species.h"
#include "constants/moves.h"
#include "pokemon.h"
#include "data/pokemon/frontier_full_learnsets.h"
#include "data/pokemon/frontier_event_learnsets.h"

static const u16 *GetFrontierLearnsetWithFormFallback(const u16 *const *learnsets, u16 species)
{
    u16 baseSpecies;

    if (species >= NUM_SPECIES)
        return NULL;

    if (learnsets[species] != NULL)
        return learnsets[species];

    baseSpecies = GET_BASE_SPECIES_ID(species);
    if (baseSpecies >= NUM_SPECIES || baseSpecies == species)
        return NULL;

    return learnsets[baseSpecies];
}

const u16 *GetFrontierFullLearnset(u16 species)
{
    return GetFrontierLearnsetWithFormFallback(sFrontierFullLearnsets, species);
}

const u16 *GetFrontierEventLearnset(u16 species)
{
    return GetFrontierLearnsetWithFormFallback(sFrontierEventLearnsets, species);
}
