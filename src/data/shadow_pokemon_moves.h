// The Shadow moves of each Shadow Pokemon, by species (one row each). A species without a row gets the defaults of
// GetShadowMovesForSpecies: Shadow Rush, plus Shadow Fire / Shadow Bolt / Shadow Chill for a Fire, Electric or Ice
// type. A form without a row uses the row of its base species.
//
// The Shadow moves take the first move slots in battle, in this order. The Pokemon's own moves come after them (its
// moves from the slot after the last Shadow move): see GetShadowMoveLayout. 1-4 moves; leave the rest MOVE_NONE.
//
// Pokemon XD gives every Shadow Pokemon Shadow Rush first; the rows below follow its Shadow Pokemon where they are
// known and can be changed freely.
static const struct ShadowPokemonMoves sShadowPokemonMoves[] =
{
    { SPECIES_TEDDIURSA, { MOVE_SHADOW_RUSH } }, // XD's first Shadow Pokemon
#if TESTING
    // Only in the test build (test/battle/shadow_pokemon.c), to reach every Shadow move in battle
    { SPECIES_GEODUDE, { MOVE_SHADOW_END } },
    { SPECIES_SNORLAX, { MOVE_SHADOW_HALF, MOVE_SHADOW_SHED, MOVE_SHADOW_SKY, MOVE_SHADOW_WAVE } },
    { SPECIES_MISDREAVUS, { MOVE_SHADOW_HOLD, MOVE_SHADOW_MIST, MOVE_SHADOW_PANIC, MOVE_SHADOW_DOWN } },
#endif
};
