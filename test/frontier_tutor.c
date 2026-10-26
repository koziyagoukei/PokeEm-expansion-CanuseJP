#include "global.h"
#include "frontier_tutor.h"
#include "move.h"
#include "pokemon.h"
#include "move_relearner.h"
#include "test/test.h"

static bool32 LearnsetContains(const u16 *learnset, enum Move move)
{
    u32 i;

    if (learnset == NULL)
        return FALSE;

    for (i = 0; learnset[i] != MOVE_UNAVAILABLE; i++)
    {
        if (learnset[i] == move)
            return TRUE;
    }

    return FALSE;
}

TEST("Frontier Move uses the parent learnset for forms without a table")
{
    EXPECT_EQ(
        GetFrontierFullLearnset(SPECIES_ALCREMIE_BERRY_RUBY_CREAM),
        GetFrontierFullLearnset(SPECIES_ALCREMIE));
    EXPECT_EQ(
        GetFrontierFullLearnset(SPECIES_OGERPON_WELLSPRING),
        GetFrontierFullLearnset(SPECIES_OGERPON));
    EXPECT_EQ(
        GetFrontierFullLearnset(SPECIES_OGERPON_HEARTHFLAME_TERA),
        GetFrontierFullLearnset(SPECIES_OGERPON));
}

TEST("Frontier Move keeps a form-specific learnset when one exists")
{
    EXPECT_NE(
        GetFrontierFullLearnset(SPECIES_VULPIX_ALOLA),
        GetFrontierFullLearnset(SPECIES_VULPIX));
}

TEST("Frontier Move includes web-sourced event distribution moves")
{
    EXPECT(LearnsetContains(GetFrontierEventLearnset(SPECIES_PIKACHU), MOVE_FLY));
    EXPECT(LearnsetContains(GetFrontierEventLearnset(SPECIES_PIKACHU), MOVE_CELEBRATE));
    EXPECT(LearnsetContains(GetFrontierEventLearnset(SPECIES_ALCREMIE), MOVE_CELEBRATE));
    EXPECT_EQ(
        GetFrontierEventLearnset(SPECIES_ALCREMIE_STAR_RAINBOW_SWIRL),
        GetFrontierEventLearnset(SPECIES_ALCREMIE));
}

TEST("Frontier Move includes moves first learnable in Pokemon Champions")
{
    EXPECT(LearnsetContains(GetFrontierChampionsLearnset(SPECIES_SWAMPERT), MOVE_WAVE_CRASH));
    EXPECT(LearnsetContains(GetFrontierChampionsLearnset(SPECIES_SCEPTILE), MOVE_EARTH_POWER));
    EXPECT(LearnsetContains(GetFrontierChampionsLearnset(SPECIES_AEGISLASH), MOVE_POLTERGEIST));
    EXPECT(LearnsetContains(GetFrontierChampionsLearnset(SPECIES_TAUROS_PALDEA_AQUA), MOVE_MEGAHORN));
    // Only the species Champions teaches it to, not the rest of its line.
    EXPECT(!LearnsetContains(GetFrontierChampionsLearnset(SPECIES_MARSHTOMP), MOVE_WAVE_CRASH));
    // Moves an earlier game already teaches stay in the other tables.
    EXPECT(!LearnsetContains(GetFrontierChampionsLearnset(SPECIES_SWAMPERT), MOVE_EARTHQUAKE));
}

TEST("Frontier Move offers the Champions additions")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    u32 count, i, found = 0;

    CreateMon(&mon, SPECIES_SWAMPERT, 50, 0, OTID_STRUCT_PRESET(0));
    count = Test_GetRelearnerFrontierFullMoves(&mon.box, moves);
    for (i = 0; i < count; i++)
        found += moves[i] == MOVE_WAVE_CRASH;
    EXPECT_EQ(found, 1);
}

TEST("Frontier Move rejects an invalid species")
{
    EXPECT_EQ(GetFrontierFullLearnset(NUM_SPECIES), NULL);
    EXPECT_EQ(GetFrontierEventLearnset(NUM_SPECIES), NULL);
    EXPECT_EQ(GetFrontierChampionsLearnset(NUM_SPECIES), NULL);
}

static u32 CountMoveInList(const u16 *moves, u32 count, enum Move move)
{
    u32 occurrences = 0;

    for (u32 i = 0; i < count; i++)
    {
        if (moves[i] == move)
            occurrences++;
    }
    return occurrences;
}

TEST("Frontier Move offers Tera Blast and Hidden Power once for every enabled species and form")
{
    enum Species species = SPECIES_BULBASAUR;
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    u32 count;

    for (enum Species i = SPECIES_NONE + 1; i < NUM_SPECIES; i++)
    {
        if (gSpeciesInfo[i].baseHP != 0)
            PARAMETRIZE_LABEL("species %d", i) { species = i; }
    }

    CreateMon(&mon, species, 50, 0, OTID_STRUCT_PRESET(0));
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&mon, MOVE_NONE, i);

    count = Test_GetRelearnerFrontierFullMoves(&mon.box, moves);
    EXPECT_LE(count, MAX_RELEARNER_MOVES);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_TERA_BLAST), 1);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_HIDDEN_POWER), 1);
    EXPECT(HasMoveToRelearn(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));
}

TEST("Frontier Move excludes already known universal moves")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    u32 count;
    enum Move knownMove = MOVE_TERA_BLAST;

    PARAMETRIZE { knownMove = MOVE_TERA_BLAST; }
    PARAMETRIZE { knownMove = MOVE_HIDDEN_POWER; }

    CreateMon(&mon, SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PRESET(0));
    SetMonMoveSlot(&mon, knownMove, 0);
    count = Test_GetRelearnerFrontierFullMoves(&mon.box, moves);
    EXPECT_EQ(CountMoveInList(moves, count, knownMove), 0);
    EXPECT_EQ(CountMoveInList(moves, count, knownMove == MOVE_TERA_BLAST ? MOVE_HIDDEN_POWER : MOVE_TERA_BLAST), 1);
}

TEST("Frontier Move availability includes universal moves for Ditto")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];

    CreateMon(&mon, SPECIES_DITTO, 50, 0, OTID_STRUCT_PRESET(0));
    SetMonMoveSlot(&mon, MOVE_TRANSFORM, 0);
    EXPECT(HasMoveToRelearn(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));
    EXPECT_EQ(Test_GetRelearnerFrontierFullMoves(&mon.box, moves), 2);
    SetMonMoveSlot(&mon, MOVE_TERA_BLAST, 1);
    EXPECT(HasMoveToRelearn(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));
    SetMonMoveSlot(&mon, MOVE_HIDDEN_POWER, 2);
    EXPECT(!HasMoveToRelearn(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));
    EXPECT_EQ(Test_GetRelearnerFrontierFullMoves(&mon.box, moves), 0);
    EXPECT(!CanBoxMonRelearnMoves(&mon.box, MOVE_RELEARNER_TM_MOVES));
}

TEST("Frontier Move does not offer universal moves for empty slots or eggs")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    bool8 isEgg = TRUE;

    ZeroMonData(&mon);
    EXPECT_EQ(Test_GetRelearnerFrontierFullMoves(&mon.box, moves), 0);
    EXPECT(!HasMoveToRelearn(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));

    CreateMon(&mon, SPECIES_DITTO, 50, 0, OTID_STRUCT_PRESET(0));
    SetMonData(&mon, MON_DATA_IS_EGG, &isEgg);
    EXPECT(!CanBoxMonRelearnMoves(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));
}

TEST("Frontier Move offers every Sketch-compatible move once for Smeargle without truncation")
{
    struct Pokemon mon;
    struct
    {
        u16 moves[MAX_RELEARNER_MOVES];
        u16 guard;
    } list;
    u32 count;

    CreateMon(&mon, SPECIES_SMEARGLE, 50, 0, OTID_STRUCT_PRESET(0));
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&mon, MOVE_NONE, i);
    list.guard = 0xCAFE;

    count = Test_GetRelearnerFrontierFullMoves(&mon.box, list.moves);
    EXPECT_GT(count, 500);
    EXPECT_LE(count, MAX_RELEARNER_MOVES);
    EXPECT_EQ(list.guard, 0xCAFE);
    for (enum Move move = MOVE_NONE + 1; move < MOVES_COUNT; move++)
    {
        if (GetMovePP(move) != 0 && !IsMoveSketchBanned(move))
            EXPECT_EQ(CountMoveInList(list.moves, count, move), 1);
    }
    EXPECT_EQ(CountMoveInList(list.moves, count, MOVE_SKETCH), 1);
    EXPECT_EQ(CountMoveInList(list.moves, count, MOVE_TERA_BLAST), 1);
    EXPECT_EQ(CountMoveInList(list.moves, count, MOVE_HIDDEN_POWER), 1);
    EXPECT_EQ(CountMoveInList(list.moves, count, MOVE_UPPER_HAND), 1);
    EXPECT_EQ(CountMoveInList(list.moves, count, MOVE_MALIGNANT_CHAIN), 1);
    for (u32 i = 0; i < count; i++)
    {
        EXPECT_GT(list.moves[i], MOVE_NONE);
        EXPECT_LT(list.moves[i], MOVES_COUNT);
        EXPECT(list.moves[i] == MOVE_SKETCH
            || list.moves[i] == MOVE_TERA_BLAST
            || list.moves[i] == MOVE_HIDDEN_POWER
            || !IsMoveSketchBanned(list.moves[i]));
    }
}

TEST("Frontier Move excludes known and Sketch-banned moves from Smeargle's additions")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    u32 count;

    CreateMon(&mon, SPECIES_SMEARGLE, 50, 0, OTID_STRUCT_PRESET(0));
    SetMonMoveSlot(&mon, MOVE_SKETCH, 0);
    SetMonMoveSlot(&mon, MOVE_TERA_BLAST, 1);
    SetMonMoveSlot(&mon, MOVE_HIDDEN_POWER, 2);
    SetMonMoveSlot(&mon, MOVE_SPORE, 3);
    EXPECT(HasMoveToRelearn(&mon.box, MOVE_RELEARNER_FRONTIER_FULL_MOVES));

    count = Test_GetRelearnerFrontierFullMoves(&mon.box, moves);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_SKETCH), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_TERA_BLAST), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_HIDDEN_POWER), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_SPORE), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_STRUGGLE), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_CHATTER), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_THUNDERBOLT), 1);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_EXTREME_EVOBOOST), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_MAX_STRIKE), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_G_MAX_WILDFIRE), 0);
}

TEST("Frontier Move does not add Sketch-compatible moves to other species")
{
    struct Pokemon mon;
    u16 moves[MAX_RELEARNER_MOVES];
    u32 count;

    CreateMon(&mon, SPECIES_DITTO, 50, 0, OTID_STRUCT_PRESET(0));
    count = Test_GetRelearnerFrontierFullMoves(&mon.box, moves);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_SPORE), 0);
    EXPECT_EQ(CountMoveInList(moves, count, MOVE_THUNDERBOLT), 0);
}
