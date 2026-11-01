#include "global.h"
#include "event_data.h"
#include "frontier_guest.h"
#include "frontier_story.h"
#include "overworld.h"
#include "party_bath_visitors.h"
#include "random.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/frontier_guest.h"
#include "constants/maps.h"

#define BATH_SOLO_VISITOR_COUNT (NAMED_STORY_COUNT + STORY_TARGET_COUNT)

struct BathActor
{
    u16 trainer;
    u16 graphicsId;
    u8 x, y, movement;
};

struct BathVisit
{
    u8 area, count, target;
    u16 flags[3];
    struct BathActor actors[BATH_VISITOR_SLOTS];
    const u8 *text;
};

struct BathRestSpot
{
    u8 x, y, movement;
};

#include "data/party_bath_visitors.h"

static u32 GetArea(void)
{
    u32 map = (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum;
    switch (map)
    {
    case MAP_PARTY_BATH_ROCK: return BATH_VISITOR_AREA_ROCK;
    case MAP_PARTY_BATH_WATERFALL: return BATH_VISITOR_AREA_WATERFALL;
    case MAP_PARTY_BATH_SAUNA: return BATH_VISITOR_AREA_SAUNA;
    case MAP_PARTY_BATH_DEEP: return BATH_VISITOR_AREA_DEEP;
    case MAP_PARTY_BATH_UNDERWATER: return BATH_VISITOR_AREA_UNDERWATER;
    default: return BATH_VISITOR_AREA_COUNT;
    }
}

static bool32 IsAreaOpen(u32 area)
{
    static const u16 flags[] = {FLAG_BATH_ROCK_BUILT, FLAG_BATH_WATERFALL_BUILT,
        FLAG_BATH_SAUNA_BUILT, FLAG_BATH_DEEP_BUILT, FLAG_BATH_DEEP_BUILT};
    return area < ARRAY_COUNT(flags) && VarGet(VAR_FRONTIER_INTRO_STATE) == FRONTIER_INTRO_COMPLETE
        && FlagGet(flags[area]);
}

static bool32 IsSoloRescued(u32 id)
{
    if (id < NAMED_STORY_COUNT)
        return FrontierStory_GetNamedState(id) == NAMED_STORY_RESCUED;
    return FrontierStory_IsTargetRescued(id - NAMED_STORY_COUNT);
}

static bool32 IsVisitEligible(u32 visit, u32 area)
{
    if (!IsAreaOpen(area))
        return FALSE;
    if (visit >= BATH_VISIT_SOLO_FIRST)
        return area != BATH_VISITOR_AREA_UNDERWATER
            && visit - BATH_VISIT_SOLO_FIRST < BATH_SOLO_VISITOR_COUNT
            && IsSoloRescued(visit - BATH_VISIT_SOLO_FIRST);

    const struct BathVisit *scene = &sBathVisits[visit];
    if (scene->area != area || (scene->target && !FrontierStory_IsTargetRescued(scene->target - 1)))
        return FALSE;
    for (u32 i = 0; i < ARRAY_COUNT(scene->flags); i++)
        if (scene->flags[i] && !FlagGet(scene->flags[i]))
            return FALSE;
    for (u32 i = 0; i < scene->count; i++)
        if (scene->actors[i].trainer && !FrontierStory_IsGuestRescued(scene->actors[i].trainer))
            return FALSE;
    return TRUE;
}

static void HideVisitors(void)
{
    VarSet(VAR_BATH_VISIT, BATH_VISIT_NONE);
    for (u32 slot = 0; slot < BATH_VISITOR_SLOTS; slot++)
    {
        FlagSet(FLAG_BATH_VISITOR_HIDDEN + slot);
        VarSet(VAR_BATH_VISITOR_GFX + slot, OBJ_EVENT_GFX_NINJA_BOY);
    }
}

static u16 GetSoloGraphics(u32 id)
{
    if (id < NAMED_STORY_COUNT)
        return FrontierStory_GetNamedObjectGfx(id);
    return FrontierStory_GetTargetObjectGfx(id - NAMED_STORY_COUNT);
}

static void PlaceActor(u32 slot, u16 graphicsId, u32 x, u32 y, u32 movement)
{
    if (gMapHeader.events == NULL)
        return;
    // Only this map's visitor templates are changed. No saved progress is added.
    for (u32 i = 0; i < gMapHeader.events->objectEventCount && i < OBJECT_EVENT_TEMPLATES_COUNT; i++)
    {
        struct ObjectEventTemplate *object = &gSaveBlock1Ptr->objectEventTemplates[i];
        if (object->localId != BATH_VISITOR_LOCAL_ID + slot)
            continue;
        object->x = x;
        object->y = y;
        object->movementType = movement;
        VarSet(VAR_BATH_VISITOR_GFX + slot, graphicsId);
        FlagClear(FLAG_BATH_VISITOR_HIDDEN + slot);
        return;
    }
}

static void ApplyVisit(u32 visit, u32 area)
{
    HideVisitors();
    if (!IsVisitEligible(visit, area))
        return;
    VarSet(VAR_BATH_VISIT, visit);
    if (visit >= BATH_VISIT_SOLO_FIRST)
    {
        const struct BathRestSpot *spot = &sBathRestSpots[area];
        u32 id = visit - BATH_VISIT_SOLO_FIRST;
        if (id == TRAINER_FRONTIER_TATE_AND_LIZA - TRAINER_FRONTIER_GUEST_FIRST)
        {
            PlaceActor(0, OBJ_EVENT_GFX_TATE, spot->x - 1, spot->y, spot->movement);
            PlaceActor(1, OBJ_EVENT_GFX_LIZA, spot->x + 1, spot->y, spot->movement);
        }
        else
        {
            PlaceActor(0, GetSoloGraphics(id), spot->x, spot->y, spot->movement);
        }
    }
    else
    {
        const struct BathVisit *scene = &sBathVisits[visit];
        for (u32 slot = 0; slot < scene->count; slot++)
        {
            const struct BathActor *actor = &scene->actors[slot];
            u16 graphics = actor->trainer ? GetFrontierGuestTrainer(actor->trainer)->objectGfx : actor->graphicsId;
            PlaceActor(slot, graphics, actor->x, actor->y, actor->movement);
        }
    }
}

void PartyBath_PickVisitors(void)
{
    u16 scenes[BATH_VISIT_SCENE_COUNT], solos[BATH_SOLO_VISITOR_COUNT];
    u32 area = GetArea(), sceneCount = 0, soloCount = 0;
    if (area >= BATH_VISITOR_AREA_COUNT)
        return;
    HideVisitors();
    if (!IsAreaOpen(area))
        return;
    for (u32 visit = 0; visit < BATH_VISIT_SCENE_COUNT; visit++)
        if (IsVisitEligible(visit, area))
            scenes[sceneCount++] = visit;
    if (area != BATH_VISITOR_AREA_UNDERWATER)
        for (u32 id = 0; id < BATH_SOLO_VISITOR_COUNT; id++)
            if (IsSoloRescued(id))
                solos[soloCount++] = BATH_VISIT_SOLO_FIRST + id;

    // Empty on one visit in four. Give authored scenes priority over the larger solo roster.
    if (sceneCount + soloCount == 0 || RandomUniform(RNG_NONE, 0, 3) == 0)
        return;
    if (sceneCount && (!soloCount || RandomUniform(RNG_NONE, 0, 3) != 0))
        ApplyVisit(scenes[RandomUniform(RNG_NONE, 0, sceneCount - 1)], area);
    else
        ApplyVisit(solos[RandomUniform(RNG_NONE, 0, soloCount - 1)], area);
}

void PartyBath_BufferVisitorText(void)
{
    u32 area = GetArea(), visit = VarGet(VAR_BATH_VISIT);
    u32 slot = gSpecialVar_LastTalked - BATH_VISITOR_LOCAL_ID;
    gStringVar4[0] = EOS;
    gSpecialVar_Result = FALSE;
    if (slot >= BATH_VISITOR_SLOTS || !IsVisitEligible(visit, area)
     || FlagGet(FLAG_BATH_VISITOR_HIDDEN + slot))
        return;
    if (visit < BATH_VISIT_SCENE_COUNT)
    {
        if (slot >= sBathVisits[visit].count)
            return;
        StringExpandPlaceholders(gStringVar4, sBathVisits[visit].text);
    }
    else
    {
        u32 id = visit - BATH_VISIT_SOLO_FIRST;
        if (slot != 0 && id != TRAINER_FRONTIER_TATE_AND_LIZA - TRAINER_FRONTIER_GUEST_FIRST)
            return;
        const u8 *name = id < NAMED_STORY_COUNT ? FrontierStory_GetNamedName(id)
            : FrontierStory_GetTargetName(id - NAMED_STORY_COUNT);
        StringCopy(gStringVar1, name);
        StringExpandPlaceholders(gStringVar4, sBathSoloTexts[area]);
    }
    gSpecialVar_Result = TRUE;
}

#if TESTING
bool32 Test_PartyBathVisitEligible(u32 visit)
{
    return IsVisitEligible(visit, GetArea());
}

void Test_PartyBathApplyVisit(u32 visit)
{
    ApplyVisit(visit, GetArea());
}
#endif
