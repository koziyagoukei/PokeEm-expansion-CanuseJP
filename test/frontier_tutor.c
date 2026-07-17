#include "global.h"
#include "frontier_tutor.h"
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
