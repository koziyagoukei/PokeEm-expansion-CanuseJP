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
