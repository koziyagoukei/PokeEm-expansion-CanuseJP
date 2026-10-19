#include "global.h"
#include "battle_frontier.h"
#include "frontier_util.h"
#include "move.h"
#include "test/test.h"
#include "constants/battle_frontier_mons.h"

TEST("Frontier curated sets: exactly one active set per Castform form and legacy IDs remain readable")
{
    u32 canonical = FRONTIER_MON_CASTFORM_NORMAL_1;
    PARAMETRIZE { canonical = FRONTIER_MON_CASTFORM_NORMAL_1; }
    PARAMETRIZE { canonical = FRONTIER_MON_CASTFORM_SUNNY_1; }
    PARAMETRIZE { canonical = FRONTIER_MON_CASTFORM_RAINY_1; }
    PARAMETRIZE { canonical = FRONTIER_MON_CASTFORM_SNOWY_1; }
    u32 count = 0;
    for (u32 id = 0; id < NUM_FRONTIER_MONS; id++)
        if (IsFrontierMonPoolEntryEnabled(id) && gBattleFrontierMons[id].species == gBattleFrontierMons[canonical].species)
            count++;
    EXPECT_EQ(count, 1);
    for (u32 variant = 0; variant < 3; variant++)
    {
        const struct TrainerMon *mon = &gBattleFrontierMons[canonical + variant];
        EXPECT_EQ(mon->species, gBattleFrontierMons[canonical].species);
        EXPECT_EQ(IsFrontierMonPoolEntryEnabled(canonical + variant), variant == 0);
        for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
            EXPECT_EQ(mon->moves[slot], gBattleFrontierMons[canonical].moves[slot]);
    }
}

TEST("Frontier curated sets: only duplicate Castform IDs are retired")
{
    u32 retired = 0;
    for (u32 id = 0; id < NUM_FRONTIER_MONS; id++)
    {
        EXPECT_NE(gBattleFrontierMons[id].species, SPECIES_NONE);
        if (!IsFrontierMonPoolEntryEnabled(id))
            retired++;
    }
    EXPECT_EQ(retired, 8);
    EXPECT(!IsFrontierMonPoolEntryEnabled(NUM_FRONTIER_MONS));
}

TEST("Frontier curated sets: Smeargle has three selectable Sketch-legal four-move sets")
{
    u32 id = FRONTIER_MON_SMEARGLE_1;
    PARAMETRIZE { id = FRONTIER_MON_SMEARGLE_1; }
    PARAMETRIZE { id = FRONTIER_MON_SMEARGLE_2; }
    PARAMETRIZE { id = FRONTIER_MON_SMEARGLE_3; }
    const struct TrainerMon *mon = &gBattleFrontierMons[id];
    EXPECT(IsFrontierMonPoolEntryEnabled(id));
    EXPECT_EQ(mon->species, SPECIES_SMEARGLE);
    EXPECT_EQ((u32)mon->gender, TRAINER_MON_RANDOM_GENDER);
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
    {
        EXPECT_NE(mon->moves[slot], MOVE_NONE);
        EXPECT_NE(GetMovePP(mon->moves[slot]), 0);
        EXPECT(!IsMoveSketchBanned(mon->moves[slot]));
    }
}
