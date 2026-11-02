#include "global.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "field_player_avatar.h"
#include "palette.h"
#include "sprite.h"
#include "test/test.h"
#include "constants/event_objects.h"
#include "constants/field_effects.h"
#include "constants/maps.h"

static void SetMap(u16 map)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
}

static const struct ObjectEventGraphicsInfo *Prepare(struct ObjectEvent *object, struct Sprite *sprite, u16 gfx)
{
    const struct ObjectEventGraphicsInfo *info = GetObjectEventGraphicsInfo(gfx);
    memset(object, 0, sizeof(*object));
    memset(sprite, 0, sizeof(*sprite));
    object->active = TRUE;
    object->graphicsId = gfx;
    object->localId = 1;
    object->mapGroup = MAP_GROUP(MAP_PARTY_BATH_WATERFALL);
    object->mapNum = MAP_NUM(MAP_PARTY_BATH_WATERFALL);
    object->previousElevation = 3;
    object->inHotSprings = TRUE;
    sprite->inUse = TRUE;
    sprite->oam = *info->oam;
    sprite->subspriteTables = info->subspriteTables;
    sprite->subspriteTableNum = 1;
    sprite->subspriteMode = SUBSPRITES_ON;
    return info;
}

static void CheckVisibleTiles(const struct ObjectEventGraphicsInfo *info, const struct Sprite *sprite)
{
    u8 tiles[8][8] = {0};
    const struct SubspriteTable *table = &sprite->subspriteTables[sprite->subspriteTableNum];
    EXPECT_GT(table->subspriteCount, 0);
    for (u32 i = 0; i < table->subspriteCount; i++)
    {
        const struct Subsprite *sub = &table->subsprites[i];
        const struct OamDimensions *size = &gOamDimensions[sub->shape][sub->size];
        s32 x = sub->x + info->width / 2, y = sub->y + info->height / 2;
        EXPECT_GE(x, 0);
        EXPECT_GE(y, 0);
        EXPECT_EQ(x % 8, 0);
        EXPECT_EQ(y % 8, 0);
        EXPECT_LE(x + size->width, info->width);
        EXPECT_LE(y + size->height, info->height - 8);
        for (u32 row = 0; row < size->height / 8; row++)
            for (u32 col = 0; col < size->width / 8; col++)
            {
                EXPECT_EQ(sub->tileOffset + row * (size->width / 8) + col,
                          (y / 8 + row) * (info->width / 8) + x / 8 + col);
                tiles[y / 8 + row][x / 8 + col]++;
            }
    }
    for (u32 y = 0; y < info->height / 8; y++)
        for (u32 x = 0; x < info->width / 8; x++)
            EXPECT_EQ(tiles[y][x], y < info->height / 8 - 1 ? 1 : 0);
}

TEST("Bath rendering: every supported human and Pokemon sheet clips only its bottom eight pixels")
{
    struct ObjectEvent object;
    struct Sprite sprite;
    u32 checked = 0;
    SetMap(MAP_PARTY_BATH_WATERFALL);
    for (u32 id = 0; id < NUM_OBJ_EVENT_GFX + NUM_SPECIES; id++)
    {
        u16 gfx = id < NUM_OBJ_EVENT_GFX ? id : OBJ_EVENT_MON | (id - NUM_OBJ_EVENT_GFX);
        if (gfx >= OBJ_EVENT_GFX_VARS && gfx <= OBJ_EVENT_GFX_VAR_F)
            continue;
        const struct ObjectEventGraphicsInfo *info = Prepare(&object, &sprite, gfx);
        if (!TrySetBathHotSpringsSubsprites(&object, &sprite))
            continue;
        EXPECT_LE(info->width, 64);
        EXPECT_LE(info->height, 64);
        CheckVisibleTiles(info, &sprite);
        checked++;
    }
    EXPECT_GT(checked, NUM_SPECIES);
}

TEST("Bath rendering: clipping is limited to water baths and restores on leaving the water")
{
    static const u16 maps[] = {MAP_PARTYBATHTIME, MAP_PARTY_BATH_WATERFALL, MAP_PARTY_BATH_DEEP,
        MAP_PARTY_BATH_ROCK, MAP_PARTY_BATH_SAUNA, MAP_PARTY_BATH_UNDERWATER, MAP_LAVARIDGE_TOWN};
    struct ObjectEvent object;
    struct Sprite sprite;
    for (u32 i = 0; i < ARRAY_COUNT(maps); i++)
    {
        Prepare(&object, &sprite, OBJ_EVENT_GFX_BRENDAN_NORMAL);
        SetMap(maps[i]);
        EXPECT_EQ(TrySetBathHotSpringsSubsprites(&object, &sprite), i < 3);
        object.inHotSprings = FALSE;
        EXPECT(!TrySetBathHotSpringsSubsprites(&object, &sprite));
        EXPECT_EQ((u32)sprite.subspriteTableNum, 1);
    }
    SetMap(MAP_PARTY_BATH_WATERFALL);
    Prepare(&object, &sprite, OBJ_EVENT_GFX_MAY_NORMAL);
    EXPECT(TrySetBathHotSpringsSubsprites(&object, &sprite));
    object.previousElevation = 4;
    object.inHotSprings = FALSE;
    EXPECT(!TrySetBathHotSpringsSubsprites(&object, &sprite));
    EXPECT_EQ((u32)sprite.subspriteTableNum, 2);
}

TEST("Bath rendering: special poses and unsupported sprites retain their normal rendering")
{
    struct ObjectEvent object;
    struct Sprite sprite;
    SetMap(MAP_PARTY_BATH_WATERFALL);
    Prepare(&object, &sprite, OBJ_EVENT_GFX_BRENDAN_NORMAL);
    object.disableCoveringGroundEffects = TRUE;
    EXPECT(!TrySetBathHotSpringsSubsprites(&object, &sprite));
    object.disableCoveringGroundEffects = FALSE;
    sprite.oam.affineMode = ST_OAM_AFFINE_NORMAL;
    EXPECT(!TrySetBathHotSpringsSubsprites(&object, &sprite));
    sprite.oam.affineMode = ST_OAM_AFFINE_OFF;
    sprite.subspriteTables = NULL;
    EXPECT(!TrySetBathHotSpringsSubsprites(&object, &sprite));
    EXPECT_EQ((u32)sprite.subspriteTableNum, 1);
}

static u32 SpriteCount(void)
{
    u32 count = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++)
        count += gSprites[i].inUse;
    return count;
}

TEST("Bath rendering: the bath reveals background fog without creating opaque water sprites")
{
    ResetSpriteData();
    memset(gObjectEvents, 0, sizeof(gObjectEvents));
    Prepare(&gObjectEvents[0], &gSprites[0], OBJ_EVENT_GFX_SPECIES(WAILORD));
    SetMap(MAP_PARTY_BATH_WATERFALL);
    FieldEffectActiveListClear();
    gFieldEffectArguments[0] = 1;
    gFieldEffectArguments[1] = MAP_NUM(MAP_PARTY_BATH_WATERFALL);
    gFieldEffectArguments[2] = MAP_GROUP(MAP_PARTY_BATH_WATERFALL);
    FieldEffectStart(FLDEFF_HOT_SPRINGS_WATER);
    EXPECT_EQ(SpriteCount(), 1);
    EXPECT(!FieldEffectActiveListContains(FLDEFF_HOT_SPRINGS_WATER));
    EXPECT_EQ((u32)gSprites[0].subspriteTableNum, 6);
}

TEST("Bath rendering: vanilla hot springs keep their original water cover")
{
    ResetSpriteData();
    memset(gObjectEvents, 0, sizeof(gObjectEvents));
    Prepare(&gObjectEvents[0], &gSprites[0], OBJ_EVENT_GFX_BRENDAN_NORMAL);
    SetMap(MAP_LAVARIDGE_TOWN);
    gObjectEvents[0].mapNum = MAP_NUM(MAP_LAVARIDGE_TOWN);
    gObjectEvents[0].mapGroup = MAP_GROUP(MAP_LAVARIDGE_TOWN);
    FieldEffectActiveListClear();
    gFieldEffectArguments[0] = 1;
    gFieldEffectArguments[1] = MAP_NUM(MAP_LAVARIDGE_TOWN);
    gFieldEffectArguments[2] = MAP_GROUP(MAP_LAVARIDGE_TOWN);
    FieldEffectStart(FLDEFF_HOT_SPRINGS_WATER);
    EXPECT_EQ(SpriteCount(), 2);
    EXPECT(FieldEffectActiveListContains(FLDEFF_HOT_SPRINGS_WATER));
    EXPECT_EQ((u32)gSprites[0].subspriteTableNum, 1);
}

static void LoadPaletteColors(u16 map, u16 tag, u16 *colors)
{
    u32 paletteNum;

    SetMap(map);
    FreeAllSpritePalettes();
    paletteNum = LoadObjectEventPalette(tag);
    memcpy(colors, &gPlttBufferUnfaded[OBJ_PLTT_ID(paletteNum)], PLTT_SIZE_4BPP);
}

TEST("Bath rendering: underwater, NPCs take on the water's tone and the player keeps their colors")
{
    u16 above[16], below[16];
    u16 playerTag = GetObjectEventGraphicsInfo(GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_UNDERWATER, MALE))->paletteTag;

    LoadPaletteColors(MAP_PARTY_BATH_DEEP, OBJ_EVENT_PAL_TAG_NPC_1, above);
    LoadPaletteColors(MAP_PARTY_BATH_UNDERWATER, OBJ_EVENT_PAL_TAG_NPC_1, below);
    EXPECT_NE(memcmp(above, below, sizeof(above)), 0);

    LoadPaletteColors(MAP_PARTY_BATH_DEEP, playerTag, above);
    LoadPaletteColors(MAP_PARTY_BATH_UNDERWATER, playerTag, below);
    EXPECT_EQ(memcmp(above, below, sizeof(above)), 0);
    FreeAllSpritePalettes();
}
