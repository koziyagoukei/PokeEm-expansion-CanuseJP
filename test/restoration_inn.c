#include "global.h"
#include "overworld.h"
#include "field_camera.h"
#include "fieldmap.h"
#include "malloc.h"
#include "tilesets.h"
#include "test/test.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/metatile_behaviors.h"

static const u32 sMaps[][2] =
{
    {MAP_GROUP(MAP_RESTORATION_INN_OUTSIDE), MAP_NUM(MAP_RESTORATION_INN_OUTSIDE)},
    {MAP_GROUP(MAP_RESTORATION_INN_LOBBY), MAP_NUM(MAP_RESTORATION_INN_LOBBY)},
    {MAP_GROUP(MAP_RESTORATION_INN_EAST_WING), MAP_NUM(MAP_RESTORATION_INN_EAST_WING)},
    {MAP_GROUP(MAP_RESTORATION_INN_WEST_WING), MAP_NUM(MAP_RESTORATION_INN_WEST_WING)},
};

TEST("Restoration inn: four maps use dedicated secondary graphics and no unrelated tile animation")
{
    for (u32 i=0;i<ARRAY_COUNT(sMaps);i++)
    {
        const struct MapHeader *map=Overworld_GetMapHeaderByGroupAndId(sMaps[i][0],sMaps[i][1]);
        EXPECT(map != NULL);
        EXPECT_EQ(map->mapLayout->secondaryTileset, i==0 ? &gTileset_RestorationInnExterior : &gTileset_RestorationInnInterior);
        EXPECT(map->mapLayout->secondaryTileset->isSecondary);
        EXPECT(map->mapLayout->secondaryTileset->isCompressed);
        EXPECT_EQ(map->mapLayout->secondaryTileset->callback,NULL);
        // The ferry and its crew at the outside pier; elsewhere, the cured who rest here.
        EXPECT_EQ(map->events->objectEventCount, ((const u32[]){6, 7, 4, 4})[i]);
        EXPECT_EQ(map->events->coordEventCount,0);
    }
}

TEST("Restoration inn: room doors and exit mats have appropriate warp behavior")
{
    const struct Tileset *sets[]={&gTileset_RestorationInnExterior,&gTileset_RestorationInnInterior};
    for(u32 i=0;i<ARRAY_COUNT(sets);i++)
    {
        const u16 *attrs=sets[i]->metatileAttributes;
        EXPECT_EQ(UNPACK_BEHAVIOR(attrs[64]),MB_NON_ANIMATED_DOOR);
        EXPECT_EQ(UNPACK_BEHAVIOR(attrs[65]),MB_SOUTH_ARROW_WARP);
    }
}

TEST("Restoration inn: every walkable custom metatile uses floor layers instead of foreground")
{
    for (u32 i = 0; i < ARRAY_COUNT(sMaps); i++)
    {
        const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(sMaps[i][0], sMaps[i][1]);
        const u16 *attrs = map->mapLayout->secondaryTileset->metatileAttributes;
        for (u32 j = 0; j < map->mapLayout->width * map->mapLayout->height; j++)
        {
            u16 cell = map->mapLayout->map[j];
            u32 id = cell & MAPGRID_METATILE_ID_MASK;
            if (!(cell & MAPGRID_COLLISION_MASK) && id >= NUM_METATILES_IN_PRIMARY)
                EXPECT_EQ(UNPACK_LAYER_TYPE(attrs[id - NUM_METATILES_IN_PRIMARY]), METATILE_LAYER_TYPE_COVERED);
        }
    }
}

TEST("Restoration inn: lobby walls cannot be entered but doors remain passable")
{
    const struct MapHeader *map = Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_RESTORATION_INN_LOBBY), MAP_NUM(MAP_RESTORATION_INN_LOBBY));
    for (u32 i = 0; i < map->mapLayout->width * map->mapLayout->height; i++)
        if ((map->mapLayout->map[i] & MAPGRID_METATILE_ID_MASK) == NUM_METATILES_IN_PRIMARY + 8)
            EXPECT(map->mapLayout->map[i] & MAPGRID_COLLISION_MASK);
    for (u32 i = 0; i < map->events->warpCount; i++)
    {
        const struct WarpEvent *warp = &map->events->warps[i];
        EXPECT_EQ(map->mapLayout->map[warp->y * map->mapLayout->width + warp->x] & MAPGRID_COLLISION_MASK, 0);
    }
}

TEST("Restoration inn: roof and tall furniture keep their foreground occlusion")
{
    const u16 *outside = gTileset_RestorationInnExterior.metatileAttributes;
    const u16 *inside = gTileset_RestorationInnInterior.metatileAttributes;
    EXPECT_EQ(UNPACK_LAYER_TYPE(outside[14]), METATILE_LAYER_TYPE_NORMAL);
    EXPECT_EQ(UNPACK_LAYER_TYPE(outside[48]), METATILE_LAYER_TYPE_NORMAL);
    EXPECT_EQ(UNPACK_LAYER_TYPE(inside[8]), METATILE_LAYER_TYPE_NORMAL);
    EXPECT_EQ(UNPACK_LAYER_TYPE(inside[48]), METATILE_LAYER_TYPE_NORMAL);
}

TEST("Restoration inn: actual field renderer keeps floors below sprites and clears foreground")
{
    u32 mapIndex = 0, tile = 2;
    PARAMETRIZE { mapIndex = 0; tile = 2; }
    PARAMETRIZE { mapIndex = 0; tile = 4; }
    PARAMETRIZE { mapIndex = 0; tile = 39; }
    PARAMETRIZE { mapIndex = 1; tile = 0; }
    PARAMETRIZE { mapIndex = 1; tile = 21; }
    PARAMETRIZE { mapIndex = 1; tile = 40; }
    PARAMETRIZE { mapIndex = 1; tile = 41; }
    u16 *oldBg1 = gOverworldTilemapBuffer_Bg1;
    u16 *oldBg2 = gOverworldTilemapBuffer_Bg2;
    u16 *oldBg3 = gOverworldTilemapBuffer_Bg3;
    u16 *buffers = AllocZeroed(3 * 1024 * sizeof(u16));
    gOverworldTilemapBuffer_Bg1 = buffers;
    gOverworldTilemapBuffer_Bg2 = buffers + 1024;
    gOverworldTilemapBuffer_Bg3 = buffers + 2048;
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(sMaps[mapIndex][0], sMaps[mapIndex][1]);
    gSaveBlock1Ptr->location.mapGroup = sMaps[mapIndex][0];
    gSaveBlock1Ptr->location.mapNum = sMaps[mapIndex][1];
    InitMap();
    ResetFieldCamera();
    gSaveBlock1Ptr->pos.x = MAP_OFFSET;
    gSaveBlock1Ptr->pos.y = MAP_OFFSET;
    MapGridSetMetatileEntryAt(MAP_OFFSET, MAP_OFFSET, 0x3000 | (NUM_METATILES_IN_PRIMARY + tile));
    CurrentMapDrawMetatileAt(MAP_OFFSET, MAP_OFFSET);
    const u16 *tiles = gMapHeader.mapLayout->secondaryTileset->metatiles + tile * 8;
    const u16 offsets[] = {0, 1, 32, 33};
    for (u32 i = 0; i < ARRAY_COUNT(offsets); i++)
    {
        EXPECT_EQ(gOverworldTilemapBuffer_Bg3[offsets[i]], tiles[i]);
        EXPECT_EQ(gOverworldTilemapBuffer_Bg2[offsets[i]], tiles[i + 4]);
        EXPECT_EQ(gOverworldTilemapBuffer_Bg1[offsets[i]], 0);
    }
    gOverworldTilemapBuffer_Bg1 = oldBg1;
    gOverworldTilemapBuffer_Bg2 = oldBg2;
    gOverworldTilemapBuffer_Bg3 = oldBg3;
    Free(buffers);
}

TEST("Restoration inn: warps stay within the new isolated group")
{
    for(u32 i=0;i<ARRAY_COUNT(sMaps);i++)
    {
        const struct MapHeader *map=Overworld_GetMapHeaderByGroupAndId(sMaps[i][0],sMaps[i][1]);
        EXPECT_GT(map->events->warpCount,0);
        for(u32 j=0;j<map->events->warpCount;j++)
        {
            const struct WarpEvent *warp=&map->events->warps[j];
            EXPECT_EQ(warp->mapGroup,MAP_GROUP(MAP_RESTORATION_INN_OUTSIDE));
            EXPECT_LT(warp->mapNum,ARRAY_COUNT(sMaps));
            EXPECT_GE(warp->x,0);
            EXPECT_GE(warp->y,0);
            EXPECT_LT(warp->x,map->mapLayout->width);
            EXPECT_LT(warp->y,map->mapLayout->height);
            EXPECT_EQ((map->mapLayout->map[warp->y*map->mapLayout->width+warp->x]>>10)&3,0);
        }
    }
}
