#include "global.h"
#include "overworld.h"
#include "test/test.h"
#include "constants/maps.h"
#include "constants/metatile_behaviors.h"

extern const u8 PartyBathExpansion_EventScript_Dive[];
extern const u8 PartyBathExpansion_EventScript_Surface[];

static const struct MapHeader *GetBathMap(u16 map)
{
    return Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(map), MAP_NUM(map));
}

TEST("Bath access: every rock slab is walkable and draws below its occupants")
{
    const struct MapLayout *layout = GetBathMap(MAP_PARTY_BATH_ROCK)->mapLayout;
    u32 count = 0;
    for (u32 i = 0; i < layout->width * layout->height; i++)
    {
        u16 block = layout->map[i], tile = UNPACK_METATILE(block);
        if (tile < 0x3DF || tile > 0x3E2)
            continue;
        u16 attr = layout->secondaryTileset->metatileAttributes[tile - 0x200];
        EXPECT_EQ(UNPACK_COLLISION(block), 0);
        EXPECT_EQ(UNPACK_ELEVATION(block), ELEVATION_DEFAULT);
        EXPECT_EQ(UNPACK_BEHAVIOR(attr), MB_NORMAL);
        EXPECT_EQ(UNPACK_LAYER_TYPE(attr), METATILE_LAYER_TYPE_COVERED);
        count++;
    }
    EXPECT_GE(count, 24);
}

TEST("Bath access: all three stair lanes offer descent when stepped on or examined")
{
    const struct MapHeader *map = GetBathMap(MAP_PARTY_BATH_DEEP);
    for (u32 x = 11; x <= 13; x++)
    {
        u32 stepCount = 0, examineCount = 0;
        EXPECT_EQ(UNPACK_COLLISION(map->mapLayout->map[6 * map->mapLayout->width + x]), 0);
        EXPECT_EQ(UNPACK_COLLISION(map->mapLayout->map[5 * map->mapLayout->width + x]), 0);
        for (u32 i = 0; i < map->events->coordEventCount; i++)
        {
            const struct CoordEvent *event = &map->events->coordEvents[i];
            if (event->x != x || event->y != 6)
                continue;
            EXPECT_EQ(event->trigger, VAR_TEMP_4);
            EXPECT_EQ(event->index, 0);
            EXPECT_EQ(event->elevation, ELEVATION_TRANSITION);
            EXPECT_EQ(event->script, PartyBathExpansion_EventScript_Dive);
            stepCount++;
        }
        for (u32 i = 0; i < map->events->bgEventCount; i++)
        {
            const struct BgEvent *event = &map->events->bgEvents[i];
            if (event->x == x && event->y == 6 && event->bgUnion.script == PartyBathExpansion_EventScript_Dive)
                examineCount++;
        }
        EXPECT_EQ(stepCount, 1);
        EXPECT_EQ(examineCount, 1);
    }
}

TEST("Bath access: the underwater exit and both native dive connections remain available")
{
    const struct MapHeader *deep = GetBathMap(MAP_PARTY_BATH_DEEP);
    const struct MapHeader *underwater = GetBathMap(MAP_PARTY_BATH_UNDERWATER);
    const struct CoordEvent *exit = NULL;
    EXPECT_EQ(underwater->mapType, MAP_TYPE_UNDERWATER);
    EXPECT_EQ(deep->connections->count, 1);
    EXPECT_EQ(underwater->connections->count, 1);
    EXPECT_EQ(deep->connections->connections[0].direction, CONNECTION_DIVE);
    EXPECT_EQ(deep->connections->connections[0].mapGroup, MAP_GROUP(MAP_PARTY_BATH_UNDERWATER));
    EXPECT_EQ(deep->connections->connections[0].mapNum, MAP_NUM(MAP_PARTY_BATH_UNDERWATER));
    EXPECT_EQ(underwater->connections->connections[0].direction, CONNECTION_EMERGE);
    EXPECT_EQ(underwater->connections->connections[0].mapGroup, MAP_GROUP(MAP_PARTY_BATH_DEEP));
    EXPECT_EQ(underwater->connections->connections[0].mapNum, MAP_NUM(MAP_PARTY_BATH_DEEP));
    for (u32 i = 0; i < underwater->events->coordEventCount; i++)
        if (underwater->events->coordEvents[i].x == 12 && underwater->events->coordEvents[i].y == 3)
            exit = &underwater->events->coordEvents[i];
    EXPECT(exit != NULL);
    EXPECT_EQ(exit->script, PartyBathExpansion_EventScript_Surface);
    EXPECT_EQ(UNPACK_COLLISION(underwater->mapLayout->map[4 * underwater->mapLayout->width + 12]), 0);
    EXPECT_EQ(UNPACK_COLLISION(deep->mapLayout->map[4 * deep->mapLayout->width + 12]), 0);
}
