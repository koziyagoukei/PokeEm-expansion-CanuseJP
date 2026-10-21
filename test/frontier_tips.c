#include "global.h"
#include "event_data.h"
#include "frontier_util.h"
#include "line_break.h"
#include "pokemon.h"
#include "text.h"
#include "ultra_help.h"
#include "test/test.h"

TEST("Frontier TIPS: all pages and choices fit the explanation window")
{
    u32 info = 0;
    for (u32 i = 0; i < INFO_COUNT; i++)
        PARAMETRIZE { info = i; }
    const struct InfoViewerEntry *entry = GetInfoViewerEntry(info);
    EXPECT(GetStringWidth(FONT_NORMAL, entry->title, 0) <= 208);
    EXPECT(entry->pageCount != 0 || entry->choiceCount != 0);
    for (u32 page = 0; page < entry->pageCount; page++)
    {
        EXPECT(GetStringWidth(FONT_SMALL, entry->pages[page], 0) <= 208);
        EXPECT((CountLineBreaks((u8 *)entry->pages[page]) + 1) * 14 <= 98);
    }
    for (u32 choice = 0; choice < entry->choiceCount; choice++)
    {
        u16 target = entry->choices[choice].nextInfoId;
        EXPECT(GetStringWidth(FONT_SMALL, entry->choices[choice].text, 0) <= 188);
        EXPECT(target < INFO_COUNT || target == INFO_VIEWER_BACK || target == INFO_VIEWER_CLOSE);
    }
}

TEST("Frontier TIPS: all seven facilities and rule topics are reachable from TIPS")
{
    bool8 seen[INFO_COUNT] = {0};
    seen[INFO_TIPS] = TRUE;
    for (u32 pass = 0; pass < INFO_COUNT; pass++)
        for (u32 id = 0; id < INFO_COUNT; id++)
            if (seen[id])
            {
                const struct InfoViewerEntry *entry = GetInfoViewerEntry(id);
                for (u32 i = 0; i < entry->choiceCount; i++)
                    if (entry->choices[i].nextInfoId < INFO_COUNT)
                        seen[entry->choices[i].nextInfoId] = TRUE;
            }
    EXPECT(seen[INFO_FRONTIER_RULES]);
    EXPECT(seen[INFO_BANNED_MONS]);
    EXPECT(seen[INFO_GIMMICK_HELP]);
    for (u32 id = INFO_TIPS_TOWER; id <= INFO_TIPS_PYRAMID; id++)
        EXPECT(seen[id]);
    EXPECT(!seen[INFO_DIARY_TEST]);
    EXPECT(!seen[INFO_CHOICE_TEST]);
}

TEST("Frontier TIPS: banned list covers actual admission rules including all forms")
{
    bool8 listed[NUM_SPECIES] = {0};
    u32 count = 0;
    enum Species species;
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    while ((species = GetInfoViewerBannedSpecies(count)) != SPECIES_NONE)
    {
        EXPECT(!listed[species]);
        listed[species] = TRUE;
        EXPECT(IsFrontierSpeciesBannedByCurrentRules(species));
        EXPECT(GetStringWidth(FONT_SMALL, GetSpeciesName(species), 0) <= 100);
        count++;
        EXPECT(count < NUM_SPECIES);
    }
    EXPECT(count > 30);
    EXPECT(listed[SPECIES_MEW]);
    EXPECT(listed[SPECIES_COSMOG]);
    EXPECT(listed[SPECIES_NECROZMA]);
    EXPECT(!listed[SPECIES_PIKACHU]);
    EXPECT_EQ(GetInfoViewerPageCount(INFO_BANNED_MONS), GetInfoViewerEntry(INFO_BANNED_MONS)->pageCount + (count + 9) / 10);
    for (u32 i = 1; i < NUM_SPECIES; i++)
    {
        if (!IsSpeciesEnabled(i))
            continue;
        enum Species base = GET_BASE_SPECIES_ID(i);
        // Mega forms are battle transformations, not separately entered mons.
        if (gSpeciesInfo[i].isMegaEvolution)
            continue;
        EXPECT_EQ(listed[i] || listed[base], IsFrontierSpeciesBannedByCurrentRules(i));
    }
    FlagSet(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
    for (u32 i = 0; i < count; i++)
    {
        species = GetInfoViewerBannedSpecies(i);
        EXPECT(listed[species]);
        EXPECT(!IsFrontierSpeciesBannedByCurrentRules(species));
        EXPECT(IsFrontierSpeciesSubjectToBannedLimit(species));
    }
    EXPECT_EQ(GetInfoViewerBannedSpecies(count), SPECIES_NONE);
    FlagClear(FLAG_FRONTIER_ALLOW_BANNED_SPECIES);
}
