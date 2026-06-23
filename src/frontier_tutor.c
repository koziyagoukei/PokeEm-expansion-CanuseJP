#include "global.h"
#include "constants/species.h"
#include "constants/moves.h"
#include "data/pokemon/frontier_full_learnsets.h"

const u16 *GetFrontierFullLearnset(u16 species)
{
    if (species >= NUM_SPECIES)
        return NULL;

    return sFrontierFullLearnsets[species];
}