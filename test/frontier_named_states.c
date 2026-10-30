#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "test/test.h"
#include "constants/characters.h"
#include "constants/flags.h"
#include "constants/frontier_story.h"

TEST("Frontier named states: each person's state can be set to every step without touching the others")
{
    u32 id = 0;

    PARAMETRIZE { id = 0; }
    PARAMETRIZE { id = NAMED_STORY_GUEST_COUNT - 1; }
    PARAMETRIZE { id = NAMED_STORY_BRAIN_FIRST; }
    PARAMETRIZE { id = NAMED_STORY_COUNT - 1; }

    for (u32 i = 0; i < NAMED_STORY_COUNT; i++)
        FrontierStory_SetNamedState(i, NAMED_STORY_INFECTED);
    for (u32 state = NAMED_STORY_INFECTED; state <= NAMED_STORY_RESCUED; state++)
    {
        FrontierStory_SetNamedState(id, state);
        EXPECT_EQ(FrontierStory_GetNamedState(id), state);
        EXPECT_EQ(FlagGet(FLAG_STORY_NAMED_STATES_START + id * 2), state & 1);
        EXPECT_EQ(FlagGet(FLAG_STORY_NAMED_STATES_START + id * 2 + 1), (state >> 1) & 1);
        for (u32 other = 0; other < NAMED_STORY_COUNT; other++)
            if (other != id)
                EXPECT_EQ(FrontierStory_GetNamedState(other), NAMED_STORY_INFECTED);
    }
    // Out of range ids and states are ignored.
    FrontierStory_SetNamedState(id, NAMED_STORY_RESCUED + 1);
    EXPECT_EQ(FrontierStory_GetNamedState(id), NAMED_STORY_RESCUED);
    FrontierStory_SetNamedState(NAMED_STORY_COUNT, NAMED_STORY_RESCUED);
    EXPECT_EQ(FrontierStory_GetNamedState(NAMED_STORY_COUNT), NAMED_STORY_INFECTED);
    EXPECT_NE(FrontierStory_GetNamedName(id)[0], EOS);
    FrontierStory_SetNamedState(id, NAMED_STORY_INFECTED);
}
