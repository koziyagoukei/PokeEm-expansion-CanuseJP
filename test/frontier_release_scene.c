#include "global.h"
#include "event_data.h"
#include "fieldmap.h"
#include "frontier_story.h"
#include "overworld.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/map_event_ids.h"
#include "constants/maps.h"

void Test_InitMapLayoutData(const struct MapHeader *mapHeader);

enum
{
    PLAYER,
    INFECTED,
    COOK,
    POKEVIRUS,
    ATTENDANT,
    BLOCKER,
};

static void PlaceObject(u32 slot, u8 localId, s16 x, s16 y)
{
    struct ObjectEvent *object = &gObjectEvents[slot];
    memset(object, 0, sizeof(*object));
    object->active = TRUE;
    object->inanimate = TRUE;
    object->localId = localId;
    object->mapNum = gSaveBlock1Ptr->location.mapNum;
    object->mapGroup = gSaveBlock1Ptr->location.mapGroup;
    object->spriteId = slot;
    object->graphicsId = OBJ_EVENT_GFX_MAN_3;
    object->currentCoords.x = object->previousCoords.x = x + MAP_OFFSET;
    object->currentCoords.y = object->previousCoords.y = y + MAP_OFFSET;
}

static void SetUpDomeScene(void)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_DOME_LOBBY);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum);
    Test_InitMapLayoutData(&gMapHeader);
    memset(gObjectEvents, 0, sizeof(gObjectEvents));
    memset(gSaveBlock1Ptr->objectEventTemplates, 0, sizeof(gSaveBlock1Ptr->objectEventTemplates));
    memcpy(gSaveBlock1Ptr->objectEventTemplates, gMapHeader.events->objectEvents,
           gMapHeader.events->objectEventCount * sizeof(struct ObjectEventTemplate));
    gPlayerAvatar.objectEventId = PLAYER;
    PlaceObject(PLAYER, LOCALID_PLAYER, 17, 11);
    PlaceObject(INFECTED, LOCALID_DOME_LOBBY_REIKA, 16, 11);
    PlaceObject(COOK, LOCALID_DOME_LOBBY_COOK, 15, 12);
    PlaceObject(POKEVIRUS, LOCALID_DOME_LOBBY_POKEVIRUS, 17, 12);
    PlaceObject(ATTENDANT, LOCALID_DOME_ATTENDANT_DOUBLES, 17, 10);
    gSpecialVar_0x8004 = LOCALID_DOME_LOBBY_REIKA;
    gSpecialVar_0x8005 = LOCALID_DOME_LOBBY_COOK;
    gSpecialVar_0x8006 = LOCALID_DOME_LOBBY_POKEVIRUS;
}

static void ExpectPosition(u32 slot, s16 x, s16 y)
{
    EXPECT_EQ(gObjectEvents[slot].currentCoords.x, x + MAP_OFFSET);
    EXPECT_EQ(gObjectEvents[slot].currentCoords.y, y + MAP_OFFSET);
}

TEST("Release scene: Dome doubles frees Pokevirus's old tile instead of overlapping the attendant")
{
    SetUpDomeScene();
    FrontierStory_ArrangeReleaseScene();
    ExpectPosition(COOK, 17, 12);
    ExpectPosition(POKEVIRUS, 14, 11);
    ExpectPosition(ATTENDANT, 17, 10);
    ExpectPosition(INFECTED, 16, 11);
    for (u32 i = 0; i < gMapHeader.events->objectEventCount; i++)
    {
        const struct ObjectEventTemplate *object = &gSaveBlock1Ptr->objectEventTemplates[i];
        if (object->localId == LOCALID_DOME_LOBBY_COOK)
        {
            EXPECT_EQ(object->x, 17);
            EXPECT_EQ(object->y, 12);
        }
    }
}

TEST("Release scene: when both sides are occupied the cook stands behind the player")
{
    SetUpDomeScene();
    PlaceObject(BLOCKER, 2, 17, 12);
    FrontierStory_ArrangeReleaseScene();
    ExpectPosition(COOK, 18, 11);
    ExpectPosition(BLOCKER, 17, 12);
    ExpectPosition(ATTENDANT, 17, 10);
}

TEST("Release scene: when no candidate is free the cook is not forced onto an NPC or a wall")
{
    SetUpDomeScene();
    PlaceObject(BLOCKER, 2, 17, 12);
    MapGridSetMetatileImpassabilityAt(18 + MAP_OFFSET, 11 + MAP_OFFSET, TRUE);
    MapGridSetMetatileImpassabilityAt(19 + MAP_OFFSET, 11 + MAP_OFFSET, TRUE);
    FrontierStory_ArrangeReleaseScene();
    ExpectPosition(COOK, 15, 12);
    ExpectPosition(BLOCKER, 17, 12);
    ExpectPosition(ATTENDANT, 17, 10);
}

TEST("Release scene: all four approach directions preserve the cook's preferred free side")
{
    s32 dx = 0, dy = 1;
    PARAMETRIZE { dx = 0; dy = 1; }
    PARAMETRIZE { dx = 0; dy = -1; }
    PARAMETRIZE { dx = 1; dy = 0; }
    PARAMETRIZE { dx = -1; dy = 0; }
    SetUpDomeScene();
    PlaceObject(INFECTED, LOCALID_DOME_LOBBY_REIKA, 13, 13);
    PlaceObject(PLAYER, LOCALID_PLAYER, 13 + dx, 13 + dy);
    FrontierStory_ArrangeReleaseScene();
    ExpectPosition(COOK, 13 + dx + (dx == 0), 13 + dy + (dy == 0));
    ExpectPosition(ATTENDANT, 17, 10);
}
