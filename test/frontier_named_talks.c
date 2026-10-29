#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "overworld.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/frontier_guest.h"
#include "constants/frontier_story.h"
#include "constants/maps.h"
#include "constants/vars.h"

// The real table, to check it against the maps.
#include "../src/data/frontier_named_talks.h"

static void SetNamed(u32 id, u32 state)
{
    u32 flag = FLAG_STORY_NAMED_STATES_START + id * 2;

    if (state & 1)
        FlagSet(flag);
    else
        FlagClear(flag);
    if (state & 2)
        FlagSet(flag + 1);
    else
        FlagClear(flag + 1);
}

static void ResetTalks(void)
{
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    for (u32 id = 0; id < NAMED_STORY_COUNT; id++)
        SetNamed(id, NAMED_STORY_INFECTED);
    for (u32 talk = 0; talk < NAMED_TALK_COUNT; talk++)
        FlagClear(FLAG_STORY_NAMED_TALKS_START + talk);
}

static void CureGroup(u32 talk)
{
    for (u32 i = 0; i < sNamedTalks[talk].count; i++)
        SetNamed(sNamedTalks[talk].members[i], NAMED_STORY_RESCUED);
}

TEST("Named talks: each group is two to five different people, with a flag of its own")
{
    EXPECT_EQ(FLAG_STORY_NAMED_TALKS_START + NAMED_TALK_COUNT - 1, FLAG_STORY_NAMED_TALKS_END);
    for (u32 talk = 0; talk < NAMED_TALK_COUNT; talk++)
    {
        const struct NamedTalk *group = &sNamedTalks[talk];

        EXPECT_GE(group->count, 2);
        EXPECT_LE(group->count, NAMED_TALK_MAX_MEMBERS);
        for (u32 i = 0; i < group->count; i++)
        {
            EXPECT_LT(group->members[i], NAMED_STORY_COUNT);
            for (u32 j = i + 1; j < group->count; j++)
                EXPECT_NE(group->members[i], group->members[j]);
        }
        for (u32 other = talk + 1; other < NAMED_TALK_COUNT; other++)
        {
            if (sNamedTalks[other].map == group->map)
                EXPECT_NE(sNamedTalks[other].hideFlag, group->hideFlag);
        }
    }
}

TEST("Named talks: every group stands on its square with everyone in it, on one script")
{
    for (u32 talk = 0; talk < NAMED_TALK_COUNT; talk++)
    {
        const struct NamedTalk *group = &sNamedTalks[talk];
        const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(group->map), MAP_NUM(group->map));
        const u8 *script = NULL;
        u32 standing = 0;

        for (u32 j = 0; j < map->events->objectEventCount; j++)
        {
            const struct ObjectEventTemplate *object = &map->events->objectEvents[j];

            if (object->flagId != group->hideFlag)
                continue;
            standing++;
            if (script == NULL)
                script = object->script;
            EXPECT(object->script == script);
        }
        EXPECT_EQ(standing, group->count);
    }
}

TEST("Named talks: a group meets once all of it is cured, and only once")
{
    ResetTalks();
    SetNamed(sNamedTalks[NAMED_TALK_MAGMA_AQUA_LEADERS].members[0], NAMED_STORY_RESCUED);
    EXPECT(!FrontierStory_IsNamedTalkShown(NAMED_TALK_MAGMA_AQUA_LEADERS));
    SetNamed(sNamedTalks[NAMED_TALK_MAGMA_AQUA_LEADERS].members[1], NAMED_STORY_RECOVERED);
    EXPECT(!FrontierStory_IsNamedTalkShown(NAMED_TALK_MAGMA_AQUA_LEADERS));
    CureGroup(NAMED_TALK_MAGMA_AQUA_LEADERS);
    EXPECT(FrontierStory_IsNamedTalkShown(NAMED_TALK_MAGMA_AQUA_LEADERS));
    gSpecialVar_0x8004 = NAMED_TALK_MAGMA_AQUA_LEADERS;
    FrontierStory_FinishNamedTalk();
    EXPECT(!FrontierStory_IsNamedTalkShown(NAMED_TALK_MAGMA_AQUA_LEADERS));
    // Saves from before the opening keep the old Frontier.
    ResetTalks();
    CureGroup(NAMED_TALK_LUCY_NOLAND);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    EXPECT(!FrontierStory_IsNamedTalkShown(NAMED_TALK_LUCY_NOLAND));
    ResetTalks();
}

TEST("Named talks: someone in two groups meets the first group first")
{
    ResetTalks();
    CureGroup(NAMED_TALK_STEVEN_WALLACE);
    CureGroup(NAMED_TALK_BRANDON_STEVEN);
    EXPECT(FrontierStory_IsNamedTalkShown(NAMED_TALK_STEVEN_WALLACE));
    EXPECT(!FrontierStory_IsNamedTalkShown(NAMED_TALK_BRANDON_STEVEN));
    gSpecialVar_0x8004 = NAMED_TALK_STEVEN_WALLACE;
    FrontierStory_FinishNamedTalk();
    EXPECT(FrontierStory_IsNamedTalkShown(NAMED_TALK_BRANDON_STEVEN));
    // Groups without anyone in common meet at the same time.
    CureGroup(NAMED_TALK_ELITE_FOUR);
    EXPECT(FrontierStory_IsNamedTalkShown(NAMED_TALK_ELITE_FOUR));
    ResetTalks();
}
