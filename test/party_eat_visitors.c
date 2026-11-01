#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "overworld.h"
#include "party_eat_visitors.h"
#include "random.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/characters.h"
#include "constants/event_objects.h"
#include "constants/frontier_guest.h"
#include "constants/maps.h"

static void Prepare(void)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PARTY_EAT_TIME);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTY_EAT_TIME);
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    for (u32 id = 0; id < NAMED_STORY_COUNT; id++)
        FrontierStory_SetNamedState(id, NAMED_STORY_INFECTED);
    for (u32 flag = FLAG_STORY_RESCUED_TOWER; flag <= FLAG_STORY_RESCUED_CONTEST; flag++)
        FlagClear(flag);
}

static u32 Count(void)
{
    return !FlagGet(FLAG_EAT_VISITOR_HIDDEN) + !FlagGet(FLAG_EAT_VISITOR_HIDDEN + 1);
}

TEST("Dining visitors: only released people can come and old stories remain inactive")
{
    Prepare();
    PartyEat_PickVisitors();
    EXPECT_EQ(Count(), 0);
    FrontierStory_SetNamedState(0, NAMED_STORY_RECOVERED);
    PartyEat_PickVisitors();
    EXPECT_EQ(Count(), 0);
    FrontierStory_SetNamedState(0, NAMED_STORY_RESCUED);
    PartyEat_PickVisitors();
    EXPECT_EQ(Count(), 1);
    EXPECT_EQ(VarGet(VAR_EAT_VISITOR), 0);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    PartyEat_PickVisitors();
    EXPECT_EQ(Count(), 0);
}

TEST("Dining visitors: one or two customers are selected without duplicates")
{
    u32 choice = 1;
    PARAMETRIZE { choice = 1; }
    PARAMETRIZE { choice = 2; }
    Prepare();
    for (u32 id = 0; id < 6; id++)
        FrontierStory_SetNamedState(id, NAMED_STORY_RESCUED);
    SET_RNG(RNG_NONE, choice);
    PartyEat_PickVisitors();
    EXPECT_EQ(Count(), choice);
    if (choice == 2)
        EXPECT_NE(VarGet(VAR_EAT_VISITOR), VarGet(VAR_EAT_VISITOR + 1));
}

TEST("Dining visitors: the rescued twins always come together")
{
    Prepare();
    u32 twins = TRAINER_FRONTIER_TATE_AND_LIZA - TRAINER_FRONTIER_GUEST_FIRST;
    FrontierStory_SetNamedState(twins, NAMED_STORY_RESCUED);
    PartyEat_PickVisitors();
    EXPECT_EQ(Count(), 2);
    EXPECT_EQ(VarGet(VAR_EAT_VISITOR), twins);
    EXPECT_EQ(VarGet(VAR_EAT_VISITOR + 1), twins);
    EXPECT_EQ(VarGet(VAR_EAT_VISITOR_GFX), OBJ_EVENT_GFX_TATE);
    EXPECT_EQ(VarGet(VAR_EAT_VISITOR_GFX + 1), OBJ_EVENT_GFX_LIZA);
}

TEST("Dining visitors: all rescued names fit Japanese meal text and conversation never rerolls")
{
    Prepare();
    for (u32 id = 0; id < NAMED_STORY_COUNT + STORY_TARGET_COUNT; id++)
    {
        if (id < NAMED_STORY_COUNT)
            FrontierStory_SetNamedState(id, NAMED_STORY_RESCUED);
        else
            FlagSet(FLAG_STORY_RESCUED_TOWER + id - NAMED_STORY_COUNT);
        VarSet(VAR_EAT_VISITOR, id);
        FlagClear(FLAG_EAT_VISITOR_HIDDEN);
        gSpecialVar_LastTalked = EAT_VISITOR_LOCAL_ID;
        rng_value_t rng = gRngValue;
        PartyEat_BufferVisitorText();
        EXPECT_EQ(gSpecialVar_Result, TRUE);
        EXPECT_EQ(VarGet(VAR_EAT_VISITOR), id);
        EXPECT_EQ(memcmp(&rng, &gRngValue, sizeof(rng)), 0);
        EXPECT(IsJapaneseTextMode(gStringVar4));
        u8 text[sizeof(gStringVar4)];
        StringCopy(text, gStringVar4);
        for (u8 *p = text; *p != EOS;)
        {
            if (*p == EXT_CTRL_CODE_BEGIN)
                p += 1 + GetExtCtrlCodeLength(p[1]);
            else
            {
                if (*p == CHAR_PROMPT_CLEAR || *p == CHAR_PROMPT_SCROLL)
                    *p = CHAR_NEWLINE;
                p++;
            }
        }
        EXPECT_LE(GetStringWidth(FONT_NORMAL, text, 0), 208);
    }
    gSpecialVar_LastTalked = 7;
    PartyEat_BufferVisitorText();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
}

TEST("Dining visitors: party graphics and existing NPC positions are untouched")
{
    Prepare();
    FrontierStory_SetNamedState(0, NAMED_STORY_RESCUED);
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        VarSet(VAR_OBJ_GFX_ID_0 + i, OBJ_EVENT_GFX_SPECIES(PIKACHU) + i);
        FlagClear(FLAG_TEMP_11 + i);
    }
    PartyEat_PickVisitors();
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        EXPECT_EQ(VarGet(VAR_OBJ_GFX_ID_0 + i), OBJ_EVENT_GFX_SPECIES(PIKACHU) + i);
        EXPECT(!FlagGet(FLAG_TEMP_11 + i));
    }
    const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_PARTY_EAT_TIME), MAP_NUM(MAP_PARTY_EAT_TIME));
    EXPECT_EQ(map->events->objectEventCount, 9);
    for (u32 slot = 0; slot < EAT_VISITOR_SLOTS; slot++)
    {
        const struct ObjectEventTemplate *obj = &map->events->objectEvents[7 + slot];
        EXPECT_EQ(obj->localId, EAT_VISITOR_LOCAL_ID + slot);
        EXPECT_EQ(UNPACK_COLLISION(map->mapLayout->map[obj->y * map->mapLayout->width + obj->x]), 0);
        for (u32 i = 0; i < 7; i++)
            EXPECT(obj->x != map->events->objectEvents[i].x || obj->y != map->events->objectEvents[i].y);
    }
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_PARTY_BATH_ROCK);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_PARTY_BATH_ROCK);
    VarSet(VAR_EAT_VISITOR, 123);
    PartyEat_PickVisitors();
    EXPECT_EQ(VarGet(VAR_EAT_VISITOR), 123);
}
