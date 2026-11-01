#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "overworld.h"
#include "party_bath_visitors.h"
#include "random.h"
#include "string_util.h"
#include "text.h"
#include "test/test.h"
#include "constants/characters.h"
#include "constants/event_objects.h"
#include "constants/frontier_guest.h"
#include "constants/maps.h"

static const u16 sMaps[] = {MAP_PARTY_BATH_ROCK, MAP_PARTY_BATH_WATERFALL, MAP_PARTY_BATH_SAUNA,
    MAP_PARTY_BATH_DEEP, MAP_PARTY_BATH_UNDERWATER};
static const u16 sBuilt[] = {FLAG_BATH_ROCK_BUILT, FLAG_BATH_WATERFALL_BUILT, FLAG_BATH_SAUNA_BUILT,
    FLAG_BATH_DEEP_BUILT, FLAG_BATH_DEEP_BUILT};

static void Prepare(u32 area)
{
    u16 map = sMaps[area];
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    for (u32 id = 0; id < NAMED_STORY_COUNT; id++)
        FrontierStory_SetNamedState(id, NAMED_STORY_INFECTED);
    for (u32 flag = FLAG_STORY_RESCUED_TOWER; flag <= FLAG_STORY_RESCUED_CONTEST; flag++)
        FlagClear(flag);
    for (u32 i = 0; i < ARRAY_COUNT(sBuilt); i++)
        FlagClear(sBuilt[i]);
    FlagClear(FLAG_STORY_DARKRAI_JOINED);
    FlagClear(FLAG_STORY_CURE_FOUND);
    FlagClear(FLAG_STORY_POKEVIRUS_FADED);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(map), MAP_NUM(map));
    EXPECT_EQ(gMapHeader.events->objectEventCount, BATH_VISITOR_SLOTS);
    memcpy(gSaveBlock1Ptr->objectEventTemplates, gMapHeader.events->objectEvents,
           BATH_VISITOR_SLOTS * sizeof(struct ObjectEventTemplate));
    Test_PartyBathApplyVisit(BATH_VISIT_NONE);
    SeedRng(7);
}

static void RescueEveryone(void)
{
    for (u32 id = 0; id < NAMED_STORY_COUNT; id++)
        FrontierStory_SetNamedState(id, NAMED_STORY_RESCUED);
    for (u32 flag = FLAG_STORY_RESCUED_TOWER; flag <= FLAG_STORY_RESCUED_CONTEST; flag++)
        FlagSet(flag);
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    FlagSet(FLAG_STORY_CURE_FOUND);
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
}

static u32 VisibleCount(void)
{
    u32 count = 0;
    for (u32 slot = 0; slot < BATH_VISITOR_SLOTS; slot++)
        count += !FlagGet(FLAG_BATH_VISITOR_HIDDEN + slot);
    return count;
}

static void CheckMessage(void)
{
    u8 text[sizeof(gStringVar4)];
    EXPECT(IsJapaneseTextMode(gStringVar4));
    EXPECT_LT(StringLength(gStringVar4), sizeof(text));
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

TEST("Bath visitors: locked floors and inactive stories have no visitors")
{
    u32 area = 0;
    for (u32 i = 0; i < ARRAY_COUNT(sMaps); i++)
        PARAMETRIZE { area = i; }
    Prepare(area);
    RescueEveryone();
    PartyBath_PickVisitors();
    EXPECT_EQ(VarGet(VAR_BATH_VISIT), BATH_VISIT_NONE);
    EXPECT_EQ(VisibleCount(), 0);
    FlagSet(sBuilt[area]);
    VarSet(VAR_FRONTIER_INTRO_STATE, 0);
    PartyBath_PickVisitors();
    EXPECT_EQ(VarGet(VAR_BATH_VISIT), BATH_VISIT_NONE);
    EXPECT_EQ(VisibleCount(), 0);
}

TEST("Bath visitors: a group needs every member fully rescued and appears together")
{
    Prepare(BATH_VISITOR_AREA_ROCK);
    FlagSet(FLAG_BATH_ROCK_BUILT);
    FrontierStory_SetNamedState(TRAINER_FRONTIER_STEVEN - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_RESCUED);
    FrontierStory_SetNamedState(TRAINER_FRONTIER_ROXANNE - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_RECOVERED);
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_STONE_STUDY));
    Test_PartyBathApplyVisit(BATH_VISIT_STONE_STUDY);
    EXPECT_EQ(VisibleCount(), 0);
    FrontierStory_SetNamedState(TRAINER_FRONTIER_ROXANNE - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_RESCUED);
    EXPECT(Test_PartyBathVisitEligible(BATH_VISIT_STONE_STUDY));
    Test_PartyBathApplyVisit(BATH_VISIT_STONE_STUDY);
    EXPECT_EQ(VisibleCount(), 2);
    EXPECT_EQ(VarGet(VAR_BATH_VISITOR_GFX), OBJ_EVENT_GFX_STEVEN);
    EXPECT_EQ(VarGet(VAR_BATH_VISITOR_GFX + 1), OBJ_EVENT_GFX_ROXANNE);
    FrontierStory_SetNamedState(TRAINER_FRONTIER_STEVEN - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_INFECTED);
    gSpecialVar_LastTalked = BATH_VISITOR_LOCAL_ID;
    PartyBath_BufferVisitorText();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT_EQ(gStringVar4[0], EOS);
}

TEST("Bath visitors: the two dads wait for the guest Norman to be cured")
{
    Prepare(BATH_VISITOR_AREA_SAUNA);
    FlagSet(FLAG_BATH_SAUNA_BUILT);
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_PAPAS));
    FrontierStory_SetNamedState(TRAINER_FRONTIER_NORMAN - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_RESCUED);
    Test_PartyBathApplyVisit(BATH_VISIT_PAPAS);
    EXPECT_EQ(VisibleCount(), 2);
    EXPECT_EQ(VarGet(VAR_BATH_VISITOR_GFX), OBJ_EVENT_GFX_NORMAN);
    EXPECT_EQ(VarGet(VAR_BATH_VISITOR_GFX + 1), OBJ_EVENT_GFX_NORMAN);
}

TEST("Bath visitors: Darkrai watches Pecharunt only after the ending and cure")
{
    Prepare(BATH_VISITOR_AREA_WATERFALL);
    FlagSet(FLAG_BATH_WATERFALL_BUILT);
    FlagSet(FLAG_STORY_DARKRAI_JOINED);
    FlagSet(FLAG_STORY_CURE_FOUND);
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_PECHARUNT));
    FlagSet(FLAG_STORY_POKEVIRUS_FADED);
    Test_PartyBathApplyVisit(BATH_VISIT_PECHARUNT);
    EXPECT_EQ(VisibleCount(), 2);
    EXPECT_EQ(VarGet(VAR_BATH_VISITOR_GFX), OBJ_EVENT_GFX_SPECIES(PECHARUNT));
    EXPECT_EQ(VarGet(VAR_BATH_VISITOR_GFX + 1), OBJ_EVENT_GFX_SPECIES(DARKRAI));
    FlagClear(FLAG_STORY_CURE_FOUND);
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_PECHARUNT));
}

TEST("Bath visitors: all cured guests Brains and scenario targets can rest and twins stay together")
{
    Prepare(BATH_VISITOR_AREA_ROCK);
    FlagSet(FLAG_BATH_ROCK_BUILT);
    for (u32 id = 0; id < NAMED_STORY_COUNT + STORY_TARGET_COUNT; id++)
    {
        u32 visit = BATH_VISIT_SOLO_FIRST + id;
        EXPECT(!Test_PartyBathVisitEligible(visit));
        if (id < NAMED_STORY_COUNT)
        {
            FrontierStory_SetNamedState(id, NAMED_STORY_RECOVERED);
            EXPECT(!Test_PartyBathVisitEligible(visit));
            FrontierStory_SetNamedState(id, NAMED_STORY_RESCUED);
        }
        else
        {
            FlagSet(FLAG_STORY_RESCUED_TOWER + id - NAMED_STORY_COUNT);
        }
        EXPECT(Test_PartyBathVisitEligible(visit));
        Test_PartyBathApplyVisit(visit);
        EXPECT_EQ(VisibleCount(), id == TRAINER_FRONTIER_TATE_AND_LIZA - TRAINER_FRONTIER_GUEST_FIRST ? 2 : 1);
        for (u32 slot = 0; slot < VisibleCount(); slot++)
        {
            gSpecialVar_LastTalked = BATH_VISITOR_LOCAL_ID + slot;
            PartyBath_BufferVisitorText();
            EXPECT_EQ(gSpecialVar_Result, TRUE);
            CheckMessage();
        }
    }
}

TEST("Bath visitors: every scene is reachable has walkable positions and Japanese text that fits")
{
    static const u8 player[] = _("{JPN}アイウエオカキ");
    for (u32 area = 0; area < ARRAY_COUNT(sMaps); area++)
    {
        Prepare(area);
        FlagSet(sBuilt[area]);
        RescueEveryone();
        StringCopy(gSaveBlock2Ptr->playerName, player + 2);
        for (u32 visit = 0; visit < BATH_VISIT_SCENE_COUNT; visit++)
        {
            if (!Test_PartyBathVisitEligible(visit))
                continue;
            Test_PartyBathApplyVisit(visit);
            EXPECT_GT(VisibleCount(), 0);
            for (u32 slot = 0; slot < VisibleCount(); slot++)
            {
                const struct ObjectEventTemplate *object = &gSaveBlock1Ptr->objectEventTemplates[slot];
                EXPECT_EQ(object->localId, BATH_VISITOR_LOCAL_ID + slot);
                EXPECT_LT(object->x, gMapHeader.mapLayout->width);
                EXPECT_LT(object->y, gMapHeader.mapLayout->height);
                EXPECT_EQ(gMapHeader.mapLayout->map[object->y * gMapHeader.mapLayout->width + object->x] & 0x0C00, 0);
                gSpecialVar_LastTalked = BATH_VISITOR_LOCAL_ID + slot;
                PartyBath_BufferVisitorText();
                EXPECT_EQ(gSpecialVar_Result, TRUE);
                CheckMessage();
            }
        }
    }
}

TEST("Bath visitors: generic rest messages fit for all names on every non-underwater floor")
{
    for (u32 area = 0; area < BATH_VISITOR_AREA_UNDERWATER; area++)
    {
        Prepare(area);
        FlagSet(sBuilt[area]);
        RescueEveryone();
        for (u32 id = 0; id < NAMED_STORY_COUNT + STORY_TARGET_COUNT; id++)
        {
            Test_PartyBathApplyVisit(BATH_VISIT_SOLO_FIRST + id);
            gSpecialVar_LastTalked = BATH_VISITOR_LOCAL_ID;
            PartyBath_BufferVisitorText();
            EXPECT_EQ(gSpecialVar_Result, TRUE);
            CheckMessage();
        }
    }
}

TEST("Bath visitors: empty scenes and solo visits are selectable and dialogue never rerolls")
{
    u32 kind = 0;
    PARAMETRIZE { kind = 0; }
    PARAMETRIZE { kind = 1; }
    PARAMETRIZE { kind = 2; }
    Prepare(BATH_VISITOR_AREA_ROCK);
    FlagSet(FLAG_BATH_ROCK_BUILT);
    if (kind == 2)
    {
        FrontierStory_SetNamedState(TRAINER_FRONTIER_WALLY - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_RESCUED);
        FrontierStory_SetNamedState(TRAINER_FRONTIER_WINONA - TRAINER_FRONTIER_GUEST_FIRST, NAMED_STORY_RESCUED);
    }
    else
    {
        RescueEveryone();
    }
    SET_RNG(RNG_NONE, kind == 0 ? 0 : 1);
    PartyBath_PickVisitors();
    u32 visit = VarGet(VAR_BATH_VISIT);
    if (kind == 0)
    {
        EXPECT_EQ(visit, BATH_VISIT_NONE);
        EXPECT_EQ(VisibleCount(), 0);
        return;
    }
    EXPECT(Test_PartyBathVisitEligible(visit));
    EXPECT_EQ(visit >= BATH_VISIT_SOLO_FIRST, kind == 2);
    rng_value_t rng = gRngValue;
    for (u32 i = 0; i < 16; i++)
    {
        gSpecialVar_LastTalked = BATH_VISITOR_LOCAL_ID;
        PartyBath_BufferVisitorText();
        EXPECT_EQ(gSpecialVar_Result, TRUE);
        EXPECT_EQ(VarGet(VAR_BATH_VISIT), visit);
        EXPECT_EQ(memcmp(&gRngValue, &rng, sizeof(rng)), 0);
    }
}

TEST("Bath visitors: wrong rooms invalid ids and hidden slots cannot speak")
{
    Prepare(BATH_VISITOR_AREA_UNDERWATER);
    FlagSet(FLAG_BATH_DEEP_BUILT);
    RescueEveryone();
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_MOM));
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_SOLO_FIRST));
    EXPECT(!Test_PartyBathVisitEligible(BATH_VISIT_NONE));
    Test_PartyBathApplyVisit(BATH_VISIT_COOK);
    EXPECT_EQ(VisibleCount(), 1);
    gSpecialVar_LastTalked = BATH_VISITOR_LOCAL_ID + 1;
    PartyBath_BufferVisitorText();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    gSpecialVar_LastTalked = 0xFFFF;
    PartyBath_BufferVisitorText();
    EXPECT_EQ(gSpecialVar_Result, FALSE);

    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_ULTRA_FRONTIE_HUB);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_ULTRA_FRONTIE_HUB);
    VarSet(VAR_BATH_VISIT, 123);
    PartyBath_PickVisitors();
    EXPECT_EQ(VarGet(VAR_BATH_VISIT), 123);
}
