#include "global.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "pokemon_storage_sort.h"
#include "test/test.h"
#include "constants/abilities.h"
#include "constants/moves.h"

static void ClearBoxes(void)
{
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (u32 pos = 0; pos < IN_BOX_COUNT; pos++)
            ZeroBoxMonAt(box, pos);
    }
}

static void PlaceMon(u32 box, u32 pos, enum Species species, u32 personality, enum Move move)
{
    struct BoxPokemon boxMon;
    enum Move moves[MAX_MON_MOVES] = {MOVE_TACKLE, move, MOVE_NONE, MOVE_NONE};

    CreateBoxMon(&boxMon, species, 50, personality, OTID_STRUCT_PLAYER_ID);
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetBoxMonData(&boxMon, MON_DATA_MOVE1 + i, &moves[i]);
    SetBoxMonAt(box, pos, &boxMon);
}

static void PlaceEgg(u32 box, u32 pos, enum Species species)
{
    bool32 isEgg = TRUE;

    PlaceMon(box, pos, species, 0, MOVE_NONE);
    SetBoxMonDataAt(box, pos, MON_DATA_IS_EGG, &isEgg);
}

#define EXPECT_SPECIES_AT(box, pos, species) EXPECT_EQ(GetBoxMonDataAt(box, pos, MON_DATA_SPECIES), species)

TEST("Box sort: National Dex order compacts empty slots and puts Eggs last")
{
    ClearBoxes();
    PlaceMon(0, 1, SPECIES_PIKACHU, 0x1234, MOVE_NONE);
    PlaceEgg(0, 3, SPECIES_TREECKO);
    PlaceMon(0, 5, SPECIES_BULBASAUR, 0x5678, MOVE_NONE);
    PlaceMon(0, 29, SPECIES_CHARMANDER, 0x9ABC, MOVE_NONE);

    EXPECT(SortStorageBoxes(0, 1, STORAGE_SORT_DEX, 0));

    EXPECT_SPECIES_AT(0, 0, SPECIES_BULBASAUR);
    EXPECT_SPECIES_AT(0, 1, SPECIES_CHARMANDER);
    EXPECT_SPECIES_AT(0, 2, SPECIES_PIKACHU);
    EXPECT_SPECIES_AT(0, 3, SPECIES_TREECKO);
    EXPECT(GetBoxMonDataAt(0, 3, MON_DATA_IS_EGG));
    for (u32 pos = 4; pos < IN_BOX_COUNT; pos++)
        EXPECT_SPECIES_AT(0, pos, SPECIES_NONE);
    // Moved Pokémon keep their own data.
    EXPECT_EQ(GetBoxMonDataAt(0, 0, MON_DATA_PERSONALITY), 0x5678);
    EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_PERSONALITY), 0x1234);
    EXPECT(!GetBoxMonDataAt(0, 0, MON_DATA_SANITY_IS_BAD_EGG));
}

TEST("Box sort: an already sorted box reports no change")
{
    ClearBoxes();
    PlaceMon(0, 0, SPECIES_BULBASAUR, 0, MOVE_NONE);
    PlaceMon(0, 1, SPECIES_CHARMANDER, 0, MOVE_NONE);

    EXPECT(!SortStorageBoxes(0, 1, STORAGE_SORT_DEX, 0));
    EXPECT(!SortStorageBoxes(0, 0, STORAGE_SORT_DEX, 0));
    EXPECT(!SortStorageBoxes(TOTAL_BOXES_COUNT - 1, 2, STORAGE_SORT_DEX, 0));
}

TEST("Box sort: Pokémon of the chosen type come first")
{
    ClearBoxes();
    ASSUME(GetSpeciesType(SPECIES_PIKACHU, 0) == TYPE_ELECTRIC);
    ASSUME(GetSpeciesType(SPECIES_MAGNEMITE, 0) == TYPE_ELECTRIC);
    ASSUME(GetSpeciesType(SPECIES_BULBASAUR, 0) != TYPE_ELECTRIC && GetSpeciesType(SPECIES_BULBASAUR, 1) != TYPE_ELECTRIC);
    PlaceMon(0, 0, SPECIES_BULBASAUR, 0, MOVE_NONE);
    PlaceMon(0, 1, SPECIES_MAGNEMITE, 0, MOVE_NONE);
    PlaceMon(0, 2, SPECIES_CHARMANDER, 0, MOVE_NONE);
    PlaceMon(0, 3, SPECIES_PIKACHU, 0, MOVE_NONE);

    EXPECT(SortStorageBoxes(0, 1, STORAGE_SORT_TYPE, TYPE_ELECTRIC));

    EXPECT_SPECIES_AT(0, 0, SPECIES_PIKACHU);
    EXPECT_SPECIES_AT(0, 1, SPECIES_MAGNEMITE);
    EXPECT_SPECIES_AT(0, 2, SPECIES_BULBASAUR);
    EXPECT_SPECIES_AT(0, 3, SPECIES_CHARMANDER);
}

TEST("Box sort: Pokémon that know the chosen move come first")
{
    ClearBoxes();
    PlaceMon(0, 0, SPECIES_BULBASAUR, 0, MOVE_NONE);
    PlaceMon(0, 1, SPECIES_PIKACHU, 0, MOVE_EARTHQUAKE);
    PlaceMon(0, 2, SPECIES_CHARMANDER, 0, MOVE_NONE);
    PlaceEgg(0, 3, SPECIES_CHIKORITA);
    SetBoxMonDataAt(0, 3, MON_DATA_MOVE2, &(enum Move){MOVE_EARTHQUAKE});
    PlaceMon(0, 4, SPECIES_SQUIRTLE, 0, MOVE_EARTHQUAKE);

    EXPECT(SortStorageBoxes(0, 1, STORAGE_SORT_MOVE, MOVE_EARTHQUAKE));

    EXPECT_SPECIES_AT(0, 0, SPECIES_SQUIRTLE);
    EXPECT_SPECIES_AT(0, 1, SPECIES_PIKACHU);
    EXPECT_SPECIES_AT(0, 2, SPECIES_BULBASAUR);
    EXPECT_SPECIES_AT(0, 3, SPECIES_CHARMANDER);
    // Eggs never match and stay last.
    EXPECT_SPECIES_AT(0, 4, SPECIES_CHIKORITA);
}

TEST("Box sort: Pokémon with the chosen ability come first")
{
    u32 abilityNum = 1;

    ClearBoxes();
    ASSUME(GetAbilityBySpecies(SPECIES_MAGNEMITE, 0) != ABILITY_STURDY);
    ASSUME(GetAbilityBySpecies(SPECIES_MAGNEMITE, 1) == ABILITY_STURDY);
    ASSUME(GetAbilityBySpecies(SPECIES_PIKACHU, 0) != ABILITY_STURDY);
    PlaceMon(0, 0, SPECIES_MAGNEMITE, 0, MOVE_NONE);
    PlaceMon(0, 1, SPECIES_PIKACHU, 0, MOVE_NONE);
    PlaceMon(0, 2, SPECIES_MAGNEMITE, 0, MOVE_NONE);
    SetBoxMonDataAt(0, 2, MON_DATA_ABILITY_NUM, &abilityNum);

    EXPECT(SortStorageBoxes(0, 1, STORAGE_SORT_ABILITY, ABILITY_STURDY));

    EXPECT_SPECIES_AT(0, 0, SPECIES_MAGNEMITE);
    EXPECT_EQ(GetBoxMonDataAt(0, 0, MON_DATA_ABILITY_NUM), 1);
    EXPECT_SPECIES_AT(0, 1, SPECIES_PIKACHU);
    EXPECT_SPECIES_AT(0, 2, SPECIES_MAGNEMITE);
    EXPECT_EQ(GetBoxMonDataAt(0, 2, MON_DATA_ABILITY_NUM), 0);
}

TEST("Box sort: sorting one box leaves the others alone")
{
    ClearBoxes();
    PlaceMon(0, 0, SPECIES_PIKACHU, 0, MOVE_NONE);
    PlaceMon(1, 5, SPECIES_CHARMANDER, 0, MOVE_NONE);
    PlaceMon(1, 9, SPECIES_BULBASAUR, 0, MOVE_NONE);

    EXPECT(SortStorageBoxes(1, 1, STORAGE_SORT_DEX, 0));

    EXPECT_SPECIES_AT(0, 0, SPECIES_PIKACHU);
    EXPECT_SPECIES_AT(1, 0, SPECIES_BULBASAUR);
    EXPECT_SPECIES_AT(1, 1, SPECIES_CHARMANDER);
}

TEST("Box sort: sorting all boxes packs Pokémon from the first box")
{
    ClearBoxes();
    PlaceMon(TOTAL_BOXES_COUNT - 1, 29, SPECIES_BULBASAUR, 0, MOVE_NONE);
    PlaceMon(3, 7, SPECIES_PIKACHU, 0, MOVE_NONE);
    PlaceMon(0, 0, SPECIES_SQUIRTLE, 0, MOVE_NONE);
    for (u32 pos = 0; pos < IN_BOX_COUNT; pos++)
        PlaceMon(2, pos, SPECIES_CHARMANDER, pos, MOVE_NONE);

    EXPECT(SortStorageBoxes(0, TOTAL_BOXES_COUNT, STORAGE_SORT_DEX, 0));

    EXPECT_SPECIES_AT(0, 0, SPECIES_BULBASAUR);
    for (u32 i = 1; i <= IN_BOX_COUNT; i++)
    {
        EXPECT_SPECIES_AT(i / IN_BOX_COUNT, i % IN_BOX_COUNT, SPECIES_CHARMANDER);
        // Equal keys keep their previous order.
        EXPECT_EQ(GetBoxMonDataAt(i / IN_BOX_COUNT, i % IN_BOX_COUNT, MON_DATA_PERSONALITY), i - 1);
    }
    EXPECT_SPECIES_AT(1, 1, SPECIES_SQUIRTLE);
    EXPECT_SPECIES_AT(1, 2, SPECIES_PIKACHU);
    EXPECT_SPECIES_AT(1, 3, SPECIES_NONE);
    EXPECT_SPECIES_AT(TOTAL_BOXES_COUNT - 1, 29, SPECIES_NONE);
}

TEST("Box sort: candidates list each type once in type order and skip Eggs")
{
    u16 candidates[NUMBER_OF_MON_TYPES];
    u32 count;

    ClearBoxes();
    ASSUME(GetSpeciesType(SPECIES_MAGNEMITE, 1) == TYPE_STEEL);
    PlaceMon(0, 0, SPECIES_MAGNEMITE, 0, MOVE_NONE);
    PlaceMon(0, 1, SPECIES_PIKACHU, 0, MOVE_NONE);
    PlaceEgg(0, 2, SPECIES_CHARMANDER);

    count = GetStorageSortCandidates(0, 1, STORAGE_SORT_TYPE, candidates, ARRAY_COUNT(candidates));

    EXPECT_EQ(count, 2);
    EXPECT_EQ(candidates[0], TYPE_STEEL);
    EXPECT_EQ(candidates[1], TYPE_ELECTRIC);
}

TEST("Box sort: move candidates are listed in gojuon order")
{
    u16 candidates[MOVES_COUNT];
    u32 count;

    ClearBoxes();
    PlaceMon(0, 0, SPECIES_PIKACHU, 0, MOVE_EARTHQUAKE);
    PlaceMon(5, 0, SPECIES_PIKACHU, 0, MOVE_THUNDERBOLT);

    count = GetStorageSortCandidates(0, TOTAL_BOXES_COUNT, STORAGE_SORT_MOVE, candidates, ARRAY_COUNT(candidates));

    EXPECT_EQ(count, 3);
    for (u32 i = 1; i < count; i++)
        EXPECT_LE(CompareKanaStrings(GetStorageSortCandidateName(STORAGE_SORT_MOVE, candidates[i - 1]),
                                     GetStorageSortCandidateName(STORAGE_SORT_MOVE, candidates[i])), 0);
    EXPECT_EQ(GetStorageSortCandidates(0, 1, STORAGE_SORT_DEX, candidates, ARRAY_COUNT(candidates)), 0);
}

TEST("Box sort: kana names compare in gojuon order")
{
    // Katakana sorts with hiragana.
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}アイアンヘッド"), COMPOUND_STRING("{JPN}あくび")), 0);
    // Voiced kana sort with their base kana; the voiced mark only breaks ties.
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}がか"), COMPOUND_STRING("{JPN}かき")), 0);
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}かか"), COMPOUND_STRING("{JPN}がか")), 0);
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}はね"), COMPOUND_STRING("{JPN}ばね")), 0);
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}ばね"), COMPOUND_STRING("{JPN}ぱね")), 0);
    // Small kana sort with their full-size kana.
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}きょ"), COMPOUND_STRING("{JPN}きら")), 0);
    // Shorter names come first, and equal names compare equal.
    EXPECT_LT(CompareKanaStrings(COMPOUND_STRING("{JPN}かみ"), COMPOUND_STRING("{JPN}かみなり")), 0);
    EXPECT_EQ(CompareKanaStrings(COMPOUND_STRING("{JPN}じしん"), COMPOUND_STRING("{JPN}ジシン")), 0);
}

TEST("Box sort: hundreds of different moves are listed once each in gojuon order")
{
    u16 *candidates = Alloc(MOVES_COUNT * sizeof(*candidates));
    u8 *listed = AllocZeroed(MOVES_COUNT);
    u32 count;

    // Every move in some slot of the full boxes.
    for (u32 i = 0; i < TOTAL_BOXES_COUNT * IN_BOX_COUNT; i++)
    {
        struct BoxPokemon boxMon;

        CreateBoxMon(&boxMon, SPECIES_PIKACHU, 50, i, OTID_STRUCT_PLAYER_ID);
        for (u32 j = 0; j < MAX_MON_MOVES; j++)
        {
            enum Move move = 1 + (i * MAX_MON_MOVES + j) % (MOVES_COUNT - 1);
            SetBoxMonData(&boxMon, MON_DATA_MOVE1 + j, &move);
        }
        SetBoxMonAt(i / IN_BOX_COUNT, i % IN_BOX_COUNT, &boxMon);
    }

    count = GetStorageSortCandidates(0, TOTAL_BOXES_COUNT, STORAGE_SORT_MOVE, candidates, MOVES_COUNT);

    EXPECT_EQ(count, MOVES_COUNT - 1);
    for (u32 i = 0; i < count; i++)
    {
        EXPECT(!listed[candidates[i]]);
        listed[candidates[i]] = TRUE;
        if (i > 0)
        {
            s32 order = CompareKanaStrings(GetStorageSortCandidateName(STORAGE_SORT_MOVE, candidates[i - 1]),
                                           GetStorageSortCandidateName(STORAGE_SORT_MOVE, candidates[i]));
            // Moves with the same name keep ID order.
            EXPECT(order < 0 || (order == 0 && candidates[i - 1] < candidates[i]));
        }
    }
    Free(listed);
    Free(candidates);
}
