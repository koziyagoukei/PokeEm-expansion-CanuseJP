#include "global.h"
#include "fieldmap.h"
#include "overworld.h"
#include "constants/layouts.h"
#include "constants/maps.h"

// Static rooms only. Never refresh a furnished local room or a secret base.
void Special_RestoreResortInterior(void)
{
    const struct MapLayout *layout;
    u16 layoutId;
    s16 safeX, safeY;
    s16 x = gSaveBlock1Ptr->pos.x;
    s16 y = gSaveBlock1Ptr->pos.y;
    u32 mapId = gSaveBlock1Ptr->location.mapNum | (gSaveBlock1Ptr->location.mapGroup << 8);

    switch (mapId)
    {
    case MAP_ULTRA_FRONTIE_HUB:
        layoutId = LAYOUT_ULTRA_FRONTIE_HUB;
        safeX = 13;
        safeY = 19;
        break;
    case MAP_PARTY_EAT_TIME:
        layoutId = LAYOUT_PARTY_EAT_TIME;
        safeX = 7;
        safeY = 7;
        break;
    case MAP_PARTYBATHTIME:
        layoutId = LAYOUT_PARTYBATHTIME;
        safeX = 18;
        safeY = 4;
        break;
    case MAP_MINI_MINI_VILLA_SHOP:
        layoutId = LAYOUT_MINI_MINI_VILLA_SHOP;
        safeX = 4;
        safeY = 6;
        break;
    default:
        return;
    }

    SetCurrentMapLayout(layoutId);
    layout = gMapHeader.mapLayout;
    // Replace the old save's cached floor before the bath applies painting flags.
    for (s32 row = 0; row < layout->height; row++)
        for (s32 column = 0; column < layout->width; column++)
            MapGridSetMetatileEntryAt(column + MAP_OFFSET, row + MAP_OFFSET,
                                     layout->map[row * layout->width + column]);

    if (x >= 0 && x < layout->width && y >= 0 && y < layout->height
        && !(layout->map[y * layout->width + x] & MAPGRID_COLLISION_MASK))
        return;

    // Keep valid doorway arrivals even when the door itself is marked solid.
    for (u32 i = 0; i < gMapHeader.events->warpCount; i++)
        if (gMapHeader.events->warps[i].x == x && gMapHeader.events->warps[i].y == y)
            return;

    gSaveBlock1Ptr->pos.x = safeX;
    gSaveBlock1Ptr->pos.y = safeY;
    for (u32 i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        struct ObjectEvent *object = &gObjectEvents[i];
        if (!object->active || !object->isPlayer)
            continue;
        // On-load runs before the return-to-field sprites are created.
        object->initialCoords.x = object->currentCoords.x = object->previousCoords.x = safeX + MAP_OFFSET;
        object->initialCoords.y = object->currentCoords.y = object->previousCoords.y = safeY + MAP_OFFSET;
        object->currentElevation = object->previousElevation = ELEVATION_DEFAULT;
    }
}
