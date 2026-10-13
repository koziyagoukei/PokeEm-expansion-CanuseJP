#include "global.h"
#include "frontier_tutor.h"
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

TEST("Frontier Move rejects an invalid species")
{
    EXPECT_EQ(GetFrontierFullLearnset(NUM_SPECIES), NULL);
    EXPECT_EQ(GetFrontierEventLearnset(NUM_SPECIES), NULL);
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
