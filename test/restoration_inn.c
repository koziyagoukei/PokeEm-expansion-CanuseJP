#include "global.h"
#include "overworld.h"
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
        // The ferry and its crew at the outside pier; inside, the cured who rest here.
        EXPECT_EQ(map->events->objectEventCount, ((const u32[]){2, 3, 2, 4})[i]);
        EXPECT_EQ(map->events->coordEventCount,0);
    }
}

TEST("Restoration inn: room doors and exit mats have appropriate warp behavior")
{
    const struct Tileset *sets[]={&gTileset_RestorationInnExterior,&gTileset_RestorationInnInterior};
    for(u32 i=0;i<ARRAY_COUNT(sets);i++)
    {
        const u16 *attrs=sets[i]->metatileAttributes;
        EXPECT_EQ(attrs[64],MB_NON_ANIMATED_DOOR);
        EXPECT_EQ(attrs[65],MB_SOUTH_ARROW_WARP);
    }
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
