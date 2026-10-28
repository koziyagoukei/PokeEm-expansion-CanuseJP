#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "overworld.h"
#include "test/test.h"
#include "constants/battle_partner.h"
#include "constants/map_event_ids.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/opponents.h"
#include "constants/script_commands.h"

extern const u8 FrontierStory_EventScript_TargetReleaseDispatch[];
extern const u8 PyramidStory_EventScript_ReleaseBattle[];
extern const u8 InnStory_EventScript_SetupWestWing[];

static u32 ReadHalfword(const u8 *p)
{
    return p[0] | (p[1] << 8);
}

static u32 ReadWord(const u8 *p)
{
    return ReadHalfword(p) | (ReadHalfword(p + 2) << 16);
}

TEST("Frontier story scripts: all seven shared release routes have a battle destination")
{
    const u8 *script = FrontierStory_EventScript_TargetReleaseDispatch;
    u32 seen = 0;
    EXPECT_EQ(script[0], SCR_OP_COPYVAR);
    script += 5;
    for (u32 i = 0; i < STORY_TARGET_COUNT - 1; i++, script += 11)
    {
        u32 target = ReadHalfword(script + 3);
        EXPECT_EQ(script[0], SCR_OP_COMPARE_VAR_TO_VALUE);
        EXPECT_GE(target, STORY_TARGET_REIKA);
        EXPECT_LT(target, STORY_TARGET_COUNT);
        EXPECT_EQ(seen & (1u << target), 0);
        seen |= 1u << target;
        EXPECT_EQ(script[5], SCR_OP_GOTO_IF);
        EXPECT_NE(ReadWord(script + 7), 0);
        if (target == STORY_TARGET_NOBUO)
            EXPECT_EQ(ReadWord(script + 7), (u32)PyramidStory_EventScript_ReleaseBattle);
    }
    EXPECT_EQ(seen, ((1u << STORY_TARGET_COUNT) - 1) & ~1u);
}

TEST("Frontier story scripts: Nobuo fights beside Pokevirus against the cook")
{
    const u8 *script = PyramidStory_EventScript_ReleaseBattle;
    EXPECT_EQ(script[0], SCR_OP_CALLNATIVE);
    EXPECT_EQ(ReadHalfword(script + 5), TRAINER_NOBUO_RELEASE);
    EXPECT_EQ(ReadHalfword(script + 11), TRAINER_POKEVIRUS);
    EXPECT_EQ(ReadHalfword(script + 17), PARTNER_ROCKET_COOK);
    EXPECT_EQ(script[19], SCR_OP_CALL);
}

TEST("Frontier story scripts: the Pyramid lobby hides its scene actors until the release battle")
{
    const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(
        MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY));
    const u8 *transition = (const u8 *)ReadWord(map->mapScripts + 1);
    EXPECT_EQ(transition[0], SCR_OP_SETFLAG);
    EXPECT_EQ(ReadHalfword(transition + 1), FLAG_TEMP_1D);
    EXPECT_EQ(transition[3], SCR_OP_SETFLAG);
    EXPECT_EQ(ReadHalfword(transition + 4), FLAG_TEMP_1E);
    u32 found = 0;
    for (u32 i = 0; i < map->events->objectEventCount; i++)
    {
        const struct ObjectEventTemplate *object = &map->events->objectEvents[i];
        if (object->localId == LOCALID_PYRAMID_LOBBY_COOK)
        {
            found++;
            EXPECT_EQ(object->flagId, FLAG_TEMP_1D);
        }
        if (object->localId == LOCALID_PYRAMID_LOBBY_POKEVIRUS)
        {
            found++;
            EXPECT_EQ(object->flagId, FLAG_TEMP_1E);
        }
    }
    EXPECT_EQ(found, 2);
}

TEST("Frontier story scripts: Nobuo appears at the inn only after his cure")
{
    const u8 *script = InnStory_EventScript_SetupWestWing;
    EXPECT_EQ(script[0], SCR_OP_SETFLAG);
    EXPECT_EQ(ReadHalfword(script + 1), FLAG_TEMP_11);
    EXPECT_EQ(script[6], SCR_OP_CHECKFLAG);
    EXPECT_EQ(ReadHalfword(script + 7), FLAG_STORY_RESCUED_PYRAMID);
    EXPECT_EQ(script[9], SCR_OP_CALL_IF);
    const u8 *show = (const u8 *)ReadWord(script + 11);
    EXPECT_EQ(show[0], SCR_OP_CLEARFLAG);
    EXPECT_EQ(ReadHalfword(show + 1), FLAG_TEMP_11);
    EXPECT_EQ(show[3], SCR_OP_RETURN);
}

TEST("Frontier story scripts: every shared rescue has all three actors in its actual lobby")
{
    static const u16 maps[][2] =
    {
        [STORY_TARGET_REIKA] = {MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY)},
        [STORY_TARGET_IKUE] = {MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PALACE_LOBBY)},
        [STORY_TARGET_EISUKE] = {MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_ARENA_LOBBY)},
        [STORY_TARGET_TOSHIO] = {MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_FACTORY_LOBBY)},
        [STORY_TARGET_YAYOI] = {MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PIKE_LOBBY)},
        [STORY_TARGET_NOBUO] = {MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_LOBBY)},
        [STORY_TARGET_YORIKO] = {MAP_GROUP(MAP_LILYCOVE_CITY_CONTEST_LOBBY), MAP_NUM(MAP_LILYCOVE_CITY_CONTEST_LOBBY)},
    };
    for (u32 target = STORY_TARGET_REIKA; target < STORY_TARGET_COUNT; target++)
    {
        gSpecialVar_0x8004 = target;
        FrontierStory_StartRescueScene();
        FrontierStory_GetRescueScene();
        u16 ids[] = {gSpecialVar_0x8004, gSpecialVar_0x8005, gSpecialVar_0x8006};
        const u16 flags[] = {FLAG_TEMP_1F, FLAG_TEMP_1D, FLAG_TEMP_1E};
        const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(maps[target][0], maps[target][1]);
        for (u32 actor = 0; actor < 3; actor++)
        {
            u32 found = 0;
            for (u32 i = 0; i < map->events->objectEventCount; i++)
            {
                const struct ObjectEventTemplate *object = &map->events->objectEvents[i];
                if (object->localId == ids[actor])
                {
                    found++;
                    EXPECT_EQ(object->flagId, flags[actor]);
                }
            }
            EXPECT_EQ(found, 1);
        }
    }
}
