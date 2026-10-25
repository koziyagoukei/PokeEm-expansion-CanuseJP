#include "global.h"
#include "pokemon_storage_system.h"
#include "test/test.h"

// Reviewed sizes for the Frontier save format (signature 0x46520002): bag redesign and 18 boxes.
#define T_SAVEBLOCK1_SIZE 14160
#define T_SAVEBLOCK2_SIZE 3696
#define T_SAVEBLOCK3_SIZE 568
#define T_POKEMONSTORAGE_SIZE 43384

TEST("Save layout: SaveBlock1 matches the reviewed bag format")
{
    EXPECT_EQ(sizeof(struct SaveBlock1), T_SAVEBLOCK1_SIZE);
}

TEST("Save layout: SaveBlock2 matches the reviewed optional feature settings")
{
    EXPECT_EQ(sizeof(struct SaveBlock2), T_SAVEBLOCK2_SIZE);
}

TEST("Save layout: SaveBlock3 keeps the resort data and fusion partners")
{
    EXPECT_EQ(sizeof(struct SaveBlock3), T_SAVEBLOCK3_SIZE);
}

TEST("Save layout: PokemonStorage holds eighteen boxes")
{
    EXPECT_EQ(sizeof(struct PokemonStorage), T_POKEMONSTORAGE_SIZE);
}

#undef T_SAVEBLOCK1_SIZE
#undef T_SAVEBLOCK2_SIZE
#undef T_SAVEBLOCK3_SIZE
#undef T_POKEMONSTORAGE_SIZE
