#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "overworld.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/characters.h"
#include "constants/frontier_story.h"
#include "constants/maps.h"
#include "constants/map_groups.h"

extern const u8 FrontierStory_EventScript_CrowdTalk[];

// Where the passers-by on the shared script stand.
static const u16 sCrowdMaps[] =
{
    MAP_BATTLE_FRONTIER_OUTSIDE_WEST,
    MAP_BATTLE_FRONTIER_OUTSIDE_EAST,
    MAP_BATTLE_FRONTIER_BATTLE_TOWER_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY,
    MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY,
    MAP_BATTLE_FRONTIER_POKEMON_CENTER_1F,
    MAP_BATTLE_FRONTIER_MART,
    MAP_BATTLE_FRONTIER_RANKING_HALL,
    MAP_BATTLE_FRONTIER_EXCHANGE_SERVICE_CORNER,
    MAP_LILYCOVE_CITY_LILYCOVE_MUSEUM_1F,
    MAP_LILYCOVE_CITY_LILYCOVE_MUSEUM_2F,
    MAP_LILYCOVE_CITY_CONTEST_HALL,
};

static const u16 sRescuedFlags[] =
{
    FLAG_STORY_RESCUED_TOWER,
    FLAG_STORY_RESCUED_DOME,
    FLAG_STORY_RESCUED_PALACE,
    FLAG_STORY_RESCUED_ARENA,
    FLAG_STORY_RESCUED_FACTORY,
    FLAG_STORY_RESCUED_PIKE,
    FLAG_STORY_RESCUED_PYRAMID,
    FLAG_STORY_RESCUED_CONTEST,
};

static void SetRescued(u32 count)
{
    for (u32 i = 0; i < ARRAY_COUNT(sRescuedFlags); i++)
    {
        if (i < count)
            FlagSet(sRescuedFlags[i]);
        else
            FlagClear(sRescuedFlags[i]);
    }
}

TEST("Frontier crowd: every passer-by on the shared script talks in a voice for how they look")
{
    u32 count = 0;

    for (u32 i = 0; i < ARRAY_COUNT(sCrowdMaps); i++)
    {
        const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(sCrowdMaps[i]), MAP_NUM(sCrowdMaps[i]));

        for (u32 j = 0; j < map->events->objectEventCount; j++)
        {
            const struct ObjectEventTemplate *object = &map->events->objectEvents[j];

            if (object->script != FrontierStory_EventScript_CrowdTalk)
                continue;
            count++;
            EXPECT_LT(FrontierStory_GetCrowdVoice(object->graphicsId), CROWD_VOICE_COUNT);
        }
    }
    EXPECT_EQ(count, 70);
}

TEST("Frontier crowd: every stage has lines in every voice")
{
    for (u32 stage = 0; stage < CROWD_STAGE_COUNT; stage++)
    {
        for (u32 voice = 0; voice < CROWD_VOICE_COUNT; voice++)
        {
            EXPECT(FrontierStory_BufferCrowdLine(stage, voice));
            EXPECT_NE(gStringVar4[0], EOS);
        }
    }
    EXPECT(!FrontierStory_BufferCrowdLine(CROWD_STAGE_COUNT, CROWD_VOICE_CHILD));
    EXPECT(!FrontierStory_BufferCrowdLine(CROWD_STAGE_NONE_RESCUED, CROWD_VOICE_COUNT));
}

TEST("Frontier crowd: the talk follows the rescues, then the ending")
{
    FlagClear(FLAG_STORY_POKEVIRUS_FADED);
    SetRescued(0);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_NONE_RESCUED);
    SetRescued(1);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_FIRST_RESCUE);
    SetRescued(2);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_FEW_RESCUED);
    SetRescued(3);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_FEW_RESCUED);
    SetRescued(4);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_PATH_OPEN);
    SetRescued(8);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_PATH_OPEN);
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    EXPECT_EQ(FrontierStory_GetCrowdStage(), CROWD_STAGE_AFTER_ENDING);
    FlagClear(FLAG_STORY_POKEVIRUS_FADED);
    SetRescued(0);
}

TEST("Frontier crowd: the same line is not heard twice in a row")
{
    u8 last[400];

    FrontierStory_BufferCrowdLine(CROWD_STAGE_NONE_RESCUED, CROWD_VOICE_CHILD);
    for (u32 i = 0; i < 20; i++)
    {
        StringCopy(last, gStringVar4);
        FrontierStory_BufferCrowdLine(CROWD_STAGE_NONE_RESCUED, CROWD_VOICE_CHILD);
        EXPECT_NE(StringCompare(last, gStringVar4), 0);
    }
}
