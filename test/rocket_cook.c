#include "global.h"
#include "data.h"
#include "event_object_movement.h"
#include "malloc.h"
#include "pokemon.h"
#include "sprite.h"
#include "trainer_pokemon_sprites.h"
#include "test/test.h"
#include "constants/event_objects.h"

TEST("Rocket cook: overworld uses an independent 16x32 walking sprite")
{
    const struct ObjectEventGraphicsInfo *cook = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_ROCKET_COOK);
    const struct ObjectEventGraphicsInfo *richBoy = GetObjectEventGraphicsInfo(OBJ_EVENT_GFX_RICH_BOY);

    EXPECT_NE(cook, richBoy);
    EXPECT_EQ(cook->width, 16);
    EXPECT_EQ(cook->height, 32);
    EXPECT_EQ(cook->images[0].size, 256);
    EXPECT(cook->images[0].relativeFrames);
    EXPECT(!cook->inanimate);
    EXPECT_NE(cook->images[0].data, richBoy->images[0].data);
    EXPECT_EQ(cook->paletteTag, OBJ_EVENT_PAL_TAG_ROCKET_COOK);
    EXPECT_NE(cook->paletteTag, richBoy->paletteTag);
    EXPECT(cook->anims == richBoy->anims);
}

TEST("Rocket cook: trainer front picture has its own image and palette")
{
    const struct TrainerFrontPicInfo *cook = gTrainerPicInfo[TRAINER_PIC_ROCKET_COOK].frontPic;
    const struct TrainerFrontPicInfo *richBoy = gTrainerPicInfo[TRAINER_PIC_RICH_BOY].frontPic;

    EXPECT(cook != NULL);
    EXPECT(cook->imageData != NULL);
    EXPECT(cook->paletteData != NULL);
    EXPECT_NE(cook->imageData, richBoy->imageData);
    EXPECT_NE(cook->paletteData, richBoy->paletteData);
}

TEST("Rocket cook: ally back picture provides four Hoenn animation frames")
{
    const struct TrainerBackPicInfo *cook = gTrainerPicInfo[TRAINER_PIC_ROCKET_COOK].backPic;
    const struct TrainerBackPicInfo *steven = gTrainerPicInfo[TRAINER_PIC_STEVEN].backPic;

    EXPECT(cook != NULL);
    EXPECT(cook->image.data != NULL);
    EXPECT_EQ(cook->coordinates.size, 8);
    EXPECT_EQ(cook->coordinates.y_offset, 4);
    EXPECT_EQ(cook->image.size, TRAINER_PIC_SIZE);
    EXPECT(cook->image.relativeFrames);
    EXPECT_EQ(cook->paletteData, GetTrainerFrontPicPalette(TRAINER_PIC_ROCKET_COOK));
    EXPECT_EQ(cook->animation, steven->animation);
    EXPECT_NE(cook->image.data, steven->image.data);
    EXPECT_NE(memcmp(cook->image.data, (u8 *)cook->image.data + TRAINER_PIC_SIZE, TRAINER_PIC_SIZE), 0);
    EXPECT_NE(memcmp(cook->image.data, (u8 *)cook->image.data + TRAINER_PIC_SIZE * 3, TRAINER_PIC_SIZE), 0);
}

TEST("Rocket cook: battle back-frame copy stays within the four-frame buffer")
{
    const u32 bytes = TRAINER_PIC_SIZE * 4;
    u32 *buffer = AllocZeroed(bytes + 8);
    EXPECT(buffer != NULL);
    buffer[0] = 0x12345678;
    buffer[bytes / 4 + 1] = 0x87654321;
    CopyTrainerBackspriteFramesToDest(TRAINER_PIC_ROCKET_COOK, (u8 *)(buffer + 1));
    EXPECT_EQ(buffer[0], 0x12345678);
    EXPECT_EQ(buffer[bytes / 4 + 1], 0x87654321);
    EXPECT_EQ(memcmp(buffer + 1, GetTrainerBackPicImage(TRAINER_PIC_ROCKET_COOK)->data, bytes), 0);
    Free(buffer);
}

TEST("Rocket cook: only the cook's battle back sprite is mirrored toward the opponents")
{
    enum BattlerPosition position = B_POSITION_PLAYER_LEFT;
    PARAMETRIZE { position = B_POSITION_PLAYER_LEFT; }
    PARAMETRIZE { position = B_POSITION_PLAYER_RIGHT; }
    SetMultiuseSpriteTemplateToTrainerBack(TRAINER_PIC_ROCKET_COOK, position);
    EXPECT_EQ(gMultiuseSpriteTemplate.affineAnims[0][0].frame.xScale, -0x100);
    EXPECT_EQ(gMultiuseSpriteTemplate.affineAnims[0][0].frame.yScale, 0x100);
    EXPECT(gMultiuseSpriteTemplate.anims == GetTrainerBackPicAnims(TRAINER_PIC_ROCKET_COOK));
    SetMultiuseSpriteTemplateToTrainerBack(TRAINER_PIC_STEVEN, position);
    EXPECT_EQ(gMultiuseSpriteTemplate.affineAnims[0][0].frame.xScale, 0x100);
    SetMultiuseSpriteTemplateToTrainerBack(TRAINER_PIC_BRENDAN, position);
    EXPECT_EQ(gMultiuseSpriteTemplate.affineAnims[0][0].frame.xScale, 0x100);
}

TEST("Rocket cook: idle throwing and pointing frames keep the same mirrored affine matrix")
{
    ResetSpriteData();
    SetMultiuseSpriteTemplateToTrainerBack(TRAINER_PIC_ROCKET_COOK, B_POSITION_PLAYER_RIGHT);
    gMultiuseSpriteTemplate.callback = SpriteCallbackDummy;
    u32 id = CreateSprite(&gMultiuseSpriteTemplate, 90, 80, 0);
    EXPECT_LT(id, MAX_SPRITES);
    struct Sprite *sprite = &gSprites[id];
    for (u32 anim = 0; anim < 3; anim++)
    {
        StartSpriteAnim(sprite, anim);
        for (u32 frame = 0; frame < 125; frame++)
        {
            AnimateSprites();
            EXPECT_EQ(gOamMatrices[sprite->oam.matrixNum].a, -0x100);
            EXPECT_EQ(gOamMatrices[sprite->oam.matrixNum].b, 0);
            EXPECT_EQ(gOamMatrices[sprite->oam.matrixNum].c, 0);
            EXPECT_EQ(gOamMatrices[sprite->oam.matrixNum].d, 0x100);
        }
    }
    FreeSpriteOamMatrix(sprite);
    DestroySprite(sprite);
}
