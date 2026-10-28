#include "global.h"
#include "field_camera.h"
#include "fieldmap.h"
#include "malloc.h"
#include "overworld.h"
#include "tilesets.h"
#include "tileset_anims.h"
#include "test/test.h"
#include "constants/maps.h"
#include "constants/metatile_behaviors.h"
#include "constants/metatile_labels.h"
#include "constants/pokevirus_depths.h"

#define MEMBRANE_SIZE (POKEVIRUS_MEMBRANE_TILE_COUNT * TILE_SIZE_4BPP)
#define MEMBRANE_VRAM ((u16 *)(BG_VRAM + TILE_OFFSET_4BPP(NUM_TILES_IN_PRIMARY + POKEVIRUS_MEMBRANE_TILE_OFFSET)))

extern const u16 gTilesetAnims_PokevirusMembrane_Frame0[];
extern const u16 gTilesetAnims_PokevirusMembrane_Frame1[];
extern const u16 gTilesetAnims_PokevirusMembrane_Frame2[];
extern const u16 gTilesetAnims_PokevirusMembrane_Frame3[];

static const u32 sMaps[] =
{
    MAP_POKEVIRUS_DEPTHS_APPROACH,
    MAP_POKEVIRUS_DEPTHS_HELIX,
    MAP_POKEVIRUS_DEPTHS_CORE,
};

static const struct MapHeader *GetDungeonMap(u32 index)
{
    return Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(sMaps[index]), MAP_NUM(sMaps[index]));
}

TEST("Pokevirus depths: three dedicated maps have no NPCs or active scenario triggers")
{
    for (u32 i = 0; i < ARRAY_COUNT(sMaps); i++)
    {
        const struct MapHeader *map = GetDungeonMap(i);
        const struct Tileset *tileset = map->mapLayout->secondaryTileset;
        EXPECT_EQ(map->mapLayout->primaryTileset, &gTileset_General);
        EXPECT(tileset == (i == 2 ? &gTileset_PokevirusCore : &gTileset_PokevirusDepths));
        EXPECT(tileset->isCompressed);
        EXPECT(tileset->isSecondary);
        EXPECT(tileset->callback == InitTilesetAnim_PokevirusDepths);
        EXPECT_EQ(map->events->objectEventCount, 0);
        EXPECT_EQ(map->events->coordEventCount, 0);
        EXPECT_EQ(map->events->bgEventCount, 0);
        EXPECT_EQ(map->mapScripts[0], 0);
        EXPECT(!map->mapLayout->isFrlg);
        for (u32 j = 0; j < 4; j++)
        {
            EXPECT_NE(UNPACK_COLLISION(map->mapLayout->border[j]), 0);
            EXPECT_GE(UNPACK_METATILE(map->mapLayout->border[j]), NUM_METATILES_IN_PRIMARY);
            EXPECT_LE(UNPACK_METATILE(map->mapLayout->border[j]), METATILE_PokevirusDepths_BioWall_Last);
        }
        EXPECT(!map->allowCycling);
        EXPECT(!map->allowEscaping);
    }
    EXPECT(gTileset_PokevirusCore.tiles == gTileset_PokevirusDepths.tiles);
    EXPECT(gTileset_PokevirusCore.palettes != gTileset_PokevirusDepths.palettes);
}

TEST("Pokevirus depths: every ladder has a reciprocal destination inside the isolated dungeon")
{
    for (u32 i = 0; i < ARRAY_COUNT(sMaps); i++)
    {
        const struct MapHeader *map = GetDungeonMap(i);
        EXPECT_EQ(map->events->warpCount, i == 1 ? 2 : 1);
        for (u32 j = 0; j < map->events->warpCount; j++)
        {
            const struct WarpEvent *warp = &map->events->warps[j];
            EXPECT_EQ(warp->mapGroup, MAP_GROUP(sMaps[0]));
            EXPECT_LT(warp->mapNum, ARRAY_COUNT(sMaps));
            const struct MapHeader *dest = GetDungeonMap(warp->mapNum);
            EXPECT_GE(warp->warpId, 0);
            EXPECT_LT(warp->warpId, dest->events->warpCount);
            const struct WarpEvent *back = &dest->events->warps[warp->warpId];
            EXPECT_EQ(back->mapGroup, MAP_GROUP(sMaps[i]));
            EXPECT_EQ(back->mapNum, MAP_NUM(sMaps[i]));
            EXPECT_EQ(back->warpId, j);
            u16 cell = map->mapLayout->map[warp->y * map->mapLayout->width + warp->x];
            EXPECT_EQ(UNPACK_COLLISION(cell), 0);
            EXPECT_EQ(UNPACK_BEHAVIOR(map->mapLayout->secondaryTileset->metatileAttributes[UNPACK_METATILE(cell) - 512]), MB_LADDER);
        }
    }
}

TEST("Pokevirus depths: solid boundaries are closed and floor layers never become foreground")
{
    for (u32 i = 0; i < ARRAY_COUNT(sMaps); i++)
    {
        const struct MapLayout *layout = GetDungeonMap(i)->mapLayout;
        for (u32 y = 0; y < layout->height; y++)
        {
            for (u32 x = 0; x < layout->width; x++)
            {
                u16 cell = layout->map[y * layout->width + x];
                u32 id = UNPACK_METATILE(cell);
                EXPECT_GE(id, NUM_METATILES_IN_PRIMARY);
                EXPECT_LE(id, METATILE_PokevirusDepths_BioCliff_Last);
                if (x == 0 || y == 0 || x == layout->width - 1 || y == layout->height - 1)
                    EXPECT_NE(UNPACK_COLLISION(cell), 0);
                if (!UNPACK_COLLISION(cell))
                {
                    u32 layer = UNPACK_LAYER_TYPE(layout->secondaryTileset->metatileAttributes[id - 512]);
                    if (id >= METATILE_PokevirusDepths_Chain_TopLeft && id < METATILE_PokevirusDepths_Chain_BaseLeft)
                        EXPECT_EQ(layer, METATILE_LAYER_TYPE_NORMAL);
                    else
                        EXPECT_EQ(layer, METATILE_LAYER_TYPE_COVERED);
                }
            }
        }
    }
}

TEST("Pokevirus depths: biological cliff pieces are present and retain solid covered geometry")
{
    for (u32 i = 1; i < ARRAY_COUNT(sMaps); i++)
    {
        const struct MapLayout *layout = GetDungeonMap(i)->mapLayout;
        u32 count = 0;
        for (u32 j = 0; j < layout->width * layout->height; j++)
        {
            u16 cell = layout->map[j];
            u32 id = UNPACK_METATILE(cell);
            if (id >= METATILE_PokevirusDepths_BioCliff_TopLeft && id <= METATILE_PokevirusDepths_BioCliff_Last)
            {
                EXPECT_NE(UNPACK_COLLISION(cell), 0);
                EXPECT_EQ(UNPACK_ELEVATION(cell), ELEVATION_DEFAULT);
                EXPECT_EQ(UNPACK_LAYER_TYPE(layout->secondaryTileset->metatileAttributes[id - 512]), METATILE_LAYER_TYPE_COVERED);
                count++;
            }
        }
        EXPECT_GT(count, 0);
    }
}

TEST("Pokevirus depths: membrane animation is buffered and completes a bounded six step loop")
{
    const u16 *const frames[] =
    {
        gTilesetAnims_PokevirusMembrane_Frame0,
        gTilesetAnims_PokevirusMembrane_Frame1,
        gTilesetAnims_PokevirusMembrane_Frame2,
        gTilesetAnims_PokevirusMembrane_Frame3,
        gTilesetAnims_PokevirusMembrane_Frame2,
        gTilesetAnims_PokevirusMembrane_Frame1,
    };
    gMapHeader = *GetDungeonMap(2);
    InitTilesetAnimations();
    CpuCopy16(frames[0], MEMBRANE_VRAM, MEMBRANE_SIZE);
    CpuFill16(0xA55A, MEMBRANE_VRAM - TILE_SIZE_4BPP / 2, TILE_SIZE_4BPP);
    CpuFill16(0x5AA5, MEMBRANE_VRAM + MEMBRANE_SIZE / 2, TILE_SIZE_4BPP);
    for (u32 tick = 1; tick <= POKEVIRUS_MEMBRANE_FRAME_TICKS * ARRAY_COUNT(frames); tick++)
    {
        u32 before = ((tick - 1) / POKEVIRUS_MEMBRANE_FRAME_TICKS) % ARRAY_COUNT(frames);
        u32 after = (tick / POKEVIRUS_MEMBRANE_FRAME_TICKS) % ARRAY_COUNT(frames);
        UpdateTilesetAnimations();
        EXPECT_EQ(memcmp(MEMBRANE_VRAM, frames[before], MEMBRANE_SIZE), 0);
        TransferTilesetAnimsBuffer();
        EXPECT_EQ(memcmp(MEMBRANE_VRAM, frames[after], MEMBRANE_SIZE), 0);
    }
    const u16 *beforeGuard = MEMBRANE_VRAM - TILE_SIZE_4BPP / 2;
    for (u32 i = 0; i < TILE_SIZE_4BPP / 2; i++)
    {
        EXPECT_EQ(beforeGuard[i], 0xA55A);
        EXPECT_EQ(MEMBRANE_VRAM[MEMBRANE_SIZE / 2 + i], 0x5AA5);
    }
}

TEST("Pokevirus depths: leaving for a normal cave discards pending membrane transfers")
{
    gMapHeader = *GetDungeonMap(2);
    InitTilesetAnimations();
    for (u32 tick = 1; tick < POKEVIRUS_MEMBRANE_FRAME_TICKS; tick++)
    {
        UpdateTilesetAnimations();
        TransferTilesetAnimsBuffer();
    }
    UpdateTilesetAnimations();
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ARTISAN_CAVE_1F), MAP_NUM(MAP_ARTISAN_CAVE_1F));
    EXPECT(gMapHeader.mapLayout->secondaryTileset->callback == InitTilesetAnim_Cave);
    InitTilesetAnimations();
    CpuFill16(0xA55A, MEMBRANE_VRAM, MEMBRANE_SIZE);
    TransferTilesetAnimsBuffer();
    for (u32 tick = 0; tick < POKEVIRUS_MEMBRANE_FRAME_TICKS * POKEVIRUS_MEMBRANE_LOOP_STEPS; tick++)
    {
        UpdateTilesetAnimations();
        TransferTilesetAnimsBuffer();
    }
    for (u32 i = 0; i < MEMBRANE_SIZE / 2; i++)
        EXPECT_EQ(MEMBRANE_VRAM[i], 0xA55A);
}

TEST("Pokevirus depths: final arena reserves a clear thirteen by nine floor for the last battle")
{
    const struct MapLayout *layout = GetDungeonMap(2)->mapLayout;
    for (u32 y = 10; y <= 18; y++)
        for (u32 x = 8; x <= 20; x++)
        {
            u16 cell = layout->map[y * layout->width + x];
            EXPECT_EQ(UNPACK_COLLISION(cell), 0);
            EXPECT_EQ(UNPACK_LAYER_TYPE(layout->secondaryTileset->metatileAttributes[UNPACK_METATILE(cell) - 512]), METATILE_LAYER_TYPE_COVERED);
        }
}

TEST("Pokevirus depths: field renderer keeps both the plain floor and mosaic below character sprites")
{
    u32 tile = METATILE_PokevirusDepths_Floor;
    PARAMETRIZE { tile = METATILE_PokevirusDepths_Floor; }
    PARAMETRIZE { tile = METATILE_PokevirusDepths_Mosaic_First; }
    PARAMETRIZE { tile = METATILE_PokevirusDepths_Mosaic_Last; }
    u16 *oldBg1 = gOverworldTilemapBuffer_Bg1;
    u16 *oldBg2 = gOverworldTilemapBuffer_Bg2;
    u16 *oldBg3 = gOverworldTilemapBuffer_Bg3;
    u16 *buffers = AllocZeroed(3 * 1024 * sizeof(u16));
    gOverworldTilemapBuffer_Bg1 = buffers;
    gOverworldTilemapBuffer_Bg2 = buffers + 1024;
    gOverworldTilemapBuffer_Bg3 = buffers + 2048;
    gMapHeader = *GetDungeonMap(2);
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(sMaps[2]);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(sMaps[2]);
    InitMap();
    ResetFieldCamera();
    gSaveBlock1Ptr->pos.x = MAP_OFFSET;
    gSaveBlock1Ptr->pos.y = MAP_OFFSET;
    MapGridSetMetatileEntryAt(MAP_OFFSET, MAP_OFFSET, 0x3000 | tile);
    CurrentMapDrawMetatileAt(MAP_OFFSET, MAP_OFFSET);
    const u16 *refs = gMapHeader.mapLayout->secondaryTileset->metatiles + (tile - 512) * 8;
    const u16 offsets[] = {0, 1, 32, 33};
    for (u32 i = 0; i < ARRAY_COUNT(offsets); i++)
    {
        EXPECT_EQ(gOverworldTilemapBuffer_Bg3[offsets[i]], refs[i]);
        EXPECT_EQ(gOverworldTilemapBuffer_Bg2[offsets[i]], refs[i + 4]);
        EXPECT_EQ(gOverworldTilemapBuffer_Bg1[offsets[i]], 0);
    }
    gOverworldTilemapBuffer_Bg1 = oldBg1;
    gOverworldTilemapBuffer_Bg2 = oldBg2;
    gOverworldTilemapBuffer_Bg3 = oldBg3;
    Free(buffers);
}
