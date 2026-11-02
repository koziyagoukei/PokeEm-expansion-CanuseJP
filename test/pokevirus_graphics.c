#include "global.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "malloc.h"
#include "sprite.h"
#include "test/test.h"
#include "constants/event_objects.h"

extern const u16 gObjectEventPal_Pokevirus[];

TEST("Pokevirus: map graphic uses large six-frame follower layout and its own palette")
{
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS);
    EXPECT(info != NULL);
    EXPECT_EQ(info->width, 64);
    EXPECT_EQ(info->height, 64);
    EXPECT_EQ(info->size, 2048);
    EXPECT_EQ(info->images[0].size, 2048);
    EXPECT(info->images[0].relativeFrames);
    EXPECT_EQ(info->paletteTag, OBJ_EVENT_PAL_TAG_POKEVIRUS);
    EXPECT_EQ(info->tracks, TRACKS_NONE);
    EXPECT(!info->inanimate);
    EXPECT_EQ(info->anims, sAnimTable_Following);
    EXPECT_EQ((u32)info->oam->shape, SPRITE_SHAPE(64x64));
    EXPECT_EQ((u32)info->oam->size, SPRITE_SIZE(64x64));
}

TEST("Pokevirus: all dynamic object graphic variables resolve the new map graphic")
{
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS);
    for (u32 i = 0; i < 16; i++)
    {
        u16 previous = VarGet(VAR_OBJ_GFX_ID_0 + i);
        VarSet(VAR_OBJ_GFX_ID_0 + i, OBJ_EVENT_GFX_POKEVIRUS);
        const struct ObjectEventGraphicsInfo *resolved = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_VARS + i);
        VarSet(VAR_OBJ_GFX_ID_0 + i, previous);
        EXPECT_EQ(resolved, info);
    }
}

TEST("Pokevirus: all six map frames contain distinct pixel data")
{
    const u8 *data = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS)->images[0].data;
    for (u32 i = 0; i < 6; i++)
        for (u32 j = i + 1; j < 6; j++)
            EXPECT_NE(memcmp(data + i * 2048, data + j * 2048, 2048), 0);
}

TEST("Pokevirus: large overworld sprite keeps its upper half above counters and walls")
{
#if OW_LARGE_OW_SUPPORT
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS);
    const struct SubspriteTable *tables = info->subspriteTables;
    EXPECT_EQ(tables[1].subspriteCount, 2);
    EXPECT_EQ(tables[1].subsprites[0].x, -32);
    EXPECT_EQ(tables[1].subsprites[0].y, -32);
    EXPECT_EQ((u32)tables[1].subsprites[0].tileOffset, 0);
    EXPECT_EQ((u32)tables[1].subsprites[0].shape, SPRITE_SHAPE(64x32));
    EXPECT_EQ((u32)tables[1].subsprites[0].size, SPRITE_SIZE(64x32));
    EXPECT_EQ((u32)tables[1].subsprites[0].priority, 1);
    EXPECT_EQ(tables[1].subsprites[1].x, -32);
    EXPECT_EQ(tables[1].subsprites[1].y, 0);
    EXPECT_EQ((u32)tables[1].subsprites[1].tileOffset, 32);
    EXPECT_EQ((u32)tables[1].subsprites[1].shape, SPRITE_SHAPE(64x32));
    EXPECT_EQ((u32)tables[1].subsprites[1].size, SPRITE_SIZE(64x32));
    EXPECT_EQ((u32)tables[1].subsprites[1].priority, 2);
    EXPECT_EQ(tables[2].subspriteCount, 1);
    EXPECT_EQ((u32)tables[2].subsprites[0].priority, 1);
#endif
}

TEST("Pokevirus: full body stays above map tiles regardless of elevation or ground clipping")
{
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS);
    for (u32 elevation = 0; elevation < 16; elevation++)
    {
        struct ObjectEvent object = {0};
        struct Sprite sprite = {0};
        object.graphicsId = OBJ_EVENT_GFX_POKEVIRUS;
        object.frozen = TRUE;
        object.fixedPriority = TRUE;
        object.currentElevation = object.previousElevation = elevation;
        sprite.oam = *info->oam;
        sprite.oam.priority = 3;
        sprite.subpriority = 255;
        sprite.subspriteTables = info->subspriteTables;
        sprite.subspriteTableNum = 6;
        sprite.subspriteMode = SUBSPRITES_IGNORE_PRIORITY;
        UpdateObjectEventCurrentMovement(&object, &sprite, NULL);
        EXPECT_EQ((u32)sprite.oam.priority, 1);
        EXPECT_EQ(sprite.subpriority, 0);
        EXPECT_EQ((u32)sprite.subspriteMode, SUBSPRITES_ON);
        const struct SubspriteTable *table = &sprite.subspriteTables[sprite.subspriteTableNum];
        EXPECT_EQ(table->subspriteCount, 1);
        EXPECT_EQ(table->subsprites[0].x, -32);
        EXPECT_EQ(table->subsprites[0].y, -32);
        EXPECT_EQ((u32)table->subsprites[0].shape, SPRITE_SHAPE(64x64));
        EXPECT_EQ((u32)table->subsprites[0].size, SPRITE_SIZE(64x64));
        EXPECT_EQ((u32)table->subsprites[0].tileOffset, 0);
        EXPECT_EQ((u32)table->subsprites[0].priority, 1);
        EXPECT(!object.disableCoveringGroundEffects);
        EXPECT_EQ((u32)object.previousElevation, elevation);
    }
}

TEST("Pokevirus: foreground policy does not change people, Pokemon or the rift graphic")
{
    static const u16 graphics[] = {OBJ_EVENT_GFX_ROCKET_COOK, OBJ_EVENT_MON | SPECIES_PIKACHU, OBJ_EVENT_GFX_POKEVIRUS_RIFT};
    for (u32 i = 0; i < ARRAY_COUNT(graphics); i++)
    {
        struct ObjectEvent object = {0};
        struct Sprite sprite = {0};
        object.graphicsId = graphics[i];
        object.frozen = TRUE;
        object.fixedPriority = TRUE;
        sprite.oam.priority = 3;
        sprite.subpriority = 50;
        sprite.subspriteTableNum = 1;
        sprite.subspriteMode = SUBSPRITES_ON;
        UpdateObjectEventCurrentMovement(&object, &sprite, NULL);
        EXPECT_EQ((u32)sprite.oam.priority, 3);
        EXPECT_EQ(sprite.subpriority, 50);
        EXPECT_EQ((u32)sprite.subspriteTableNum, 1);
    }
}

TEST("Pokevirus: switching an object back to another graphic restores ordinary draw order")
{
    const struct ObjectEventGraphicsInfo *virus = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS);
    const struct ObjectEventGraphicsInfo *rift = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_POKEVIRUS_RIFT);
    struct ObjectEvent object = {0};
    struct Sprite *sprite = &gSprites[0];
    memset(sprite, 0, sizeof(*sprite));
    object.graphicsId = OBJ_EVENT_GFX_POKEVIRUS;
    object.previousElevation = 3;
    object.frozen = TRUE;
    sprite->images = virus->images;
    sprite->subspriteTables = virus->subspriteTables;
    UpdateObjectEventCurrentMovement(&object, sprite, NULL);
    EXPECT_EQ((u32)sprite->oam.priority, 1);
    ObjectEventSetGraphicsId(&object, OBJ_EVENT_GFX_POKEVIRUS_RIFT);
    EXPECT_EQ(sprite->subspriteTables, rift->subspriteTables);
    EXPECT_EQ((u32)sprite->oam.priority, ElevationToPriority(3));
    EXPECT_EQ((u32)sprite->subspriteTableNum, 1);
    EXPECT_GT(sprite->subpriority, 0);
}

TEST("Pokevirus: opponent trainer picture decompresses into one frame without overflow")
{
    const struct TrainerFrontPicInfo *info = gTrainerPicInfo[TRAINER_PIC_POKEVIRUS].frontPic;
    EXPECT(info != NULL);
    EXPECT(info->imageData != NULL);
    EXPECT(info->paletteData != NULL);
    EXPECT_EQ(memcmp(info->paletteData, gObjectEventPal_Pokevirus, PLTT_SIZE_4BPP), 0);
    u32 *buffer = AllocZeroed(TRAINER_PIC_SIZE + 8);
    EXPECT(buffer != NULL);
    buffer[0] = 0x12345678;
    buffer[TRAINER_PIC_SIZE / 4 + 1] = 0x87654321;
    DecompressDataWithHeaderWram(GetTrainerFrontPicData(TRAINER_PIC_POKEVIRUS), buffer + 1);
    EXPECT_EQ(buffer[0], 0x12345678);
    EXPECT_EQ(buffer[TRAINER_PIC_SIZE / 4 + 1], 0x87654321);
    u32 nonzero = 0;
    for (u32 i = 1; i <= TRAINER_PIC_SIZE / 4; i++)
        nonzero |= buffer[i];
    EXPECT_NE(nonzero, 0);
    Free(buffer);
}
