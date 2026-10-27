#include "global.h"
#include "battle.h"
#include "battle_interface.h"
#include "pokemon.h"
#include "sprite.h"
#include "test/test.h"
#include "type_icons.h"

bool32 Test_LoadTypeLabels(void);
bool32 Test_LoadTypeIconResources(void);
u32 Test_CreateTypeIcon(enum Type type);
void Test_FreeTypeLabels(void);
s32 Test_GetTypeIconFinalX(u32 position, bool32 doubles, u32 typeNum, bool32 useLabels);

TEST("Frontier type labels: player labels do not cover either gimmick trigger")
{
    u32 oldFlags = gBattleTypeFlags;
    u8 oldPartyCount = gPartiesCount[B_TRAINER_PLAYER];
    u8 oldPositions[MAX_BATTLERS_COUNT];
    memcpy(oldPositions, gBattlerPositions, sizeof(oldPositions));
    gBattlerPositions[0] = B_POSITION_PLAYER_LEFT;
    gBattlerPositions[2] = B_POSITION_PLAYER_RIGHT;
    gPartiesCount[B_TRAINER_PLAYER] = 2;
    for (u32 doubles = FALSE; doubles <= TRUE; doubles++)
    {
        gBattleTypeFlags = doubles ? BATTLE_TYPE_DOUBLE : 0;
        for (u32 battler = 0; battler <= (doubles ? 2 : 0); battler += 2)
        {
            s16 healthX, healthY;
            GetBattlerHealthboxCoords(battler, &healthX, &healthY);
            for (u32 labelBattler = 0; labelBattler <= (doubles ? 2 : 0); labelBattler += 2)
                for (u32 type = 0; type < 2; type++)
                    for (u32 useLabels = FALSE; useLabels <= TRUE; useLabels++)
                        EXPECT_LE(Test_GetTypeIconFinalX(gBattlerPositions[labelBattler], doubles, type, useLabels)
                                  + (useLabels ? 16 : 4), healthX - 30 - 16);
        }
    }
    memcpy(gBattlerPositions, oldPositions, sizeof(oldPositions));
    gBattleTypeFlags = oldFlags;
    gPartiesCount[B_TRAINER_PLAYER] = oldPartyCount;
}

TEST("Frontier type labels: graphics leave room for the gimmick palette and tiles")
{
    static const u32 triggerTiles[256] = {0};
    static const u32 infoTiles[128] = {0};
    const struct SpriteSheet triggerSheet = {triggerTiles, sizeof(triggerTiles), TAG_GIMMICK_TRIGGER_TILE};
    const struct SpriteSheet infoSheet = {infoTiles, sizeof(infoTiles), 0xE722};
    ResetSpriteData();
    FreeAllSpritePalettes();
    EXPECT(Test_LoadTypeLabels());
    EXPECT_NE(GetSpriteTileStartByTag(TYPE_ICON_TAG), TAG_NONE);
    EXPECT_NE(IndexOfSpritePaletteTag(TYPE_ICON_TAG), 0xFF);
    EXPECT_NE(IndexOfSpritePaletteTag(TYPE_ICON_TAG_2), 0xFF);
    EXPECT_NE(IndexOfSpritePaletteTag(TYPE_ICON_TAG_3), 0xFF);
    EXPECT_GE(CountFreePaletteSlots(), 2);
    LoadSpriteSheet(&infoSheet);
    EXPECT_NE(GetSpriteTileStartByTag(infoSheet.tag), TAG_NONE);
    LoadSpriteSheet(&triggerSheet);
    EXPECT_NE(GetSpriteTileStartByTag(TAG_GIMMICK_TRIGGER_TILE), TAG_NONE);
    FreeSpriteTilesByTag(infoSheet.tag);
    FreeSpriteTilesByTag(TAG_GIMMICK_TRIGGER_TILE);
    Test_FreeTypeLabels();
    EXPECT_EQ(GetSpriteTileStartByTag(TYPE_ICON_TAG), TAG_NONE);
    EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG), 0xFF);
}

TEST("Frontier type labels: type name sprites use the correct image and palette")
{
    ResetSpriteData();
    FreeAllSpritePalettes();
    EXPECT(Test_LoadTypeIconResources());
    u32 id = Test_CreateTypeIcon(TYPE_WATER);
    EXPECT_LT(id, MAX_SPRITES);
    struct Sprite *sprite = &gSprites[id];
    EXPECT_EQ((u32)sprite->oam.shape, SPRITE_SHAPE(32x16));
    EXPECT_EQ((u32)sprite->oam.size, SPRITE_SIZE(32x16));
    EXPECT_EQ((u32)sprite->oam.paletteNum, IndexOfSpritePaletteTag(TYPE_ICON_TAG_2));
    EXPECT(!sprite->hFlip);
    AnimateSprite(sprite);
    EXPECT_EQ((u32)sprite->oam.tileNum, GetSpriteTileStartByTag(TYPE_ICON_TAG) + TYPE_WATER * 8);
    DestroySprite(sprite);
    Test_FreeTypeLabels();
}

TEST("Frontier type labels: low palette space falls back without consuming UI slots")
{
    static const u16 palette[16] = {0};
    ResetSpriteData();
    FreeAllSpritePalettes();
    for (u32 i = 0; i < 12; i++)
        EXPECT_NE(LoadSpritePaletteWithTag(palette, 0x5000 + i), 0xFF);
    EXPECT(Test_LoadTypeIconResources());
    EXPECT_NE(IndexOfSpritePaletteTag(TYPE_ICON_TAG), 0xFF);
    EXPECT_NE(IndexOfSpritePaletteTag(TYPE_ICON_TAG_2), 0xFF);
    EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG_3), 0xFF);
    EXPECT_GE(CountFreePaletteSlots(), 2);
    Test_FreeTypeLabels();
    FreeAllSpritePalettes();
}

TEST("Frontier type labels: insufficient palette space leaves the gimmick slot free")
{
    static const u16 palette[16] = {0};
    ResetSpriteData();
    FreeAllSpritePalettes();
    for (u32 i = 0; i < 15; i++)
        EXPECT_NE(LoadSpritePaletteWithTag(palette, 0x5000 + i), 0xFF);
    EXPECT(!Test_LoadTypeIconResources());
    EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG), 0xFF);
    EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG_2), 0xFF);
    EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG_3), 0xFF);
    EXPECT_EQ(CountFreePaletteSlots(), 1);
    FreeAllSpritePalettes();
}
