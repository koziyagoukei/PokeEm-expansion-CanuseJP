#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_interface.h"
#include "event_data.h"
#include "main.h"
#include "pokemon.h"
#include "sprite.h"
#include "task.h"
#include "test/battle.h"
#include "type_icons.h"

bool32 Test_LoadTypeLabels(void);
bool32 Test_LoadTypeIconResources(void);
u32 Test_CreateTypeIcon(enum Type type);
void Test_FreeTypeLabels(void);
void Test_GetTypeIconXY(s32 *x, s32 *y, s32 monX, s32 monTop, s32 partnerX, u32 typeNum, u32 typeCount, bool32 useLabels);
bool32 Test_ShouldShowBattlerTypeIcons(enum BattlerId battler, enum BattlerId target);
void Test_OptionMenuSaveTypeIcons(u8 taskId, bool32 off);
u32 Test_OptionMenuLastRowBottom(void);

TEST("Frontier type labels: only living opposing battlers are eligible including link perspective")
{
    u32 oldAbsent = gAbsentBattlerFlags;
    u32 oldCount = gBattlersCount;
    gBattlersCount = MAX_BATTLERS_COUNT;
    gAbsentBattlerFlags = 0;
    for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        gBattlerPositions[i] = i;
        gBattleMons[i].hp = 1;
    }
    for (u32 swapped = FALSE; swapped <= TRUE; swapped++)
    {
        // Link player IDs do not necessarily match on-screen battler positions.
        for (u32 i = 0; i < MAX_BATTLERS_COUNT; i++)
            gBattlerPositions[i] = i ^ swapped;
        for (u32 local = 0; local < MAX_BATTLERS_COUNT; local++)
        {
            for (u32 target = 0; target < MAX_BATTLERS_COUNT; target++)
                EXPECT_EQ(Test_ShouldShowBattlerTypeIcons(local, target), GetBattlerSide(local) != GetBattlerSide(target));
        }
    }
    gBattleMons[1].hp = 0;
    EXPECT(!Test_ShouldShowBattlerTypeIcons(0, 1));
    gBattleMons[1].hp = 1;
    gAbsentBattlerFlags = 1 << 1;
    EXPECT(!Test_ShouldShowBattlerTypeIcons(0, 1));
    gAbsentBattlerFlags = oldAbsent;
    gBattlersCount = oldCount;
}

TEST("Frontier type labels: names are centered above the head and stay within the screen")
{
    s32 x, y;
    Test_GetTypeIconXY(&x, &y, 176, 30, -1, 0, 1, TRUE);
    EXPECT_EQ(x, 176);
    EXPECT_EQ(y, 20);
    EXPECT_LT(y + 8, 30);
    for (s32 monX = 0; monX <= 240; monX += 8)
        for (s32 top = -32; top <= 128; top += 8)
            for (u32 type = 0; type < 2; type++)
            {
                Test_GetTypeIconXY(&x, &y, monX, top, -1, type, 2, TRUE);
                EXPECT_GE(x - 16, 0);
                EXPECT_LE(x + 16, DISPLAY_WIDTH);
                EXPECT_GE(y - 8, 0);
                EXPECT_LE(y + 8, 112);
            }
}

TEST("Frontier type labels: dual type opponents occupy separate columns in doubles")
{
    s32 nearX, farX, y;
    for (u32 labels = FALSE; labels <= TRUE; labels++)
    {
        s32 halfWidth = labels ? 16 : 4;
        Test_GetTypeIconXY(&nearX, &y, 200, 8, 152, 0, 2, labels);
        Test_GetTypeIconXY(&farX, &y, 152, 0, 200, 1, 2, labels);
        EXPECT_GT(nearX - halfWidth, farX + halfWidth);
        Test_GetTypeIconXY(&nearX, &y, 200, 8, 152, 1, 2, labels);
        EXPECT_LE(nearX + halfWidth, DISPLAY_WIDTH);
    }
}

TEST("Frontier type labels: disabled setting allocates no sprite resources")
{
    bool32 oldOff = FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    ResetSpriteData();
    FreeAllSpritePalettes();
    FlagSet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    LoadTypeIcons(0);
    EXPECT_EQ(GetSpriteTileStartByTag(TYPE_ICON_TAG), TAG_NONE);
    EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG), 0xFF);
    for (u32 i = 0; i < MAX_SPRITES; i++)
        EXPECT(!gSprites[i].inUse);
    if (!oldOff)
        FlagClear(FLAG_HIDE_BATTLE_TYPE_ICONS);
}

TEST("Frontier type labels: menu saves both display choices without changing neighboring flags")
{
    bool32 oldOff = FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    bool32 oldNext = FlagGet(FLAG_UNUSED_0x031);
    ResetTasks();
    FlagSet(FLAG_UNUSED_0x031);
    Test_OptionMenuSaveTypeIcons(0, TRUE);
    EXPECT(FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS));
    EXPECT(FlagGet(FLAG_UNUSED_0x031));
    Test_OptionMenuSaveTypeIcons(0, FALSE);
    EXPECT(!FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS));
    EXPECT(FlagGet(FLAG_UNUSED_0x031));
    EXPECT_LE(Test_OptionMenuLastRowBottom(), 14 * 8);
    if (oldOff)
        FlagSet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    if (!oldNext)
        FlagClear(FLAG_UNUSED_0x031);
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

static u32 CountTypeLabels(void)
{
    u32 count = 0;
    for (u32 i = 0; i < MAX_SPRITES; i++)
        if (gSprites[i].inUse && gSprites[i].template->tileTag == TYPE_ICON_TAG)
            count++;
    return count;
}

DOUBLE_BATTLE_TEST("Frontier type labels: battle display follows opponents updates types and releases resources")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHARIZARD);
        OPPONENT(SPECIES_SWAMPERT);
    } WHEN {
        TURN {
            MOVE(playerLeft, MOVE_CELEBRATE);
            MOVE(playerRight, MOVE_CELEBRATE);
            MOVE(opponentLeft, MOVE_CELEBRATE);
            MOVE(opponentRight, MOVE_CELEBRATE);
        }
    } THEN {
        bool32 oldOff = FlagGet(FLAG_HIDE_BATTLE_TYPE_ICONS);
        enum BattlerId local = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
        enum BattlerId target = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
        ResetSpriteData();
        FreeAllSpritePalettes();
        FlagClear(FLAG_HIDE_BATTLE_TYPE_ICONS);
        for (u32 i = 0; i < gBattlersCount; i++)
            gBattlerSpriteIds[i] = CreateSprite(&gDummySpriteTemplate, i == target ? 200 : 152, 80, 1);
        gBattlerControllerFuncs[local] = HandleInputChooseMove;
        LoadTypeIcons(local);
        EXPECT_EQ(CountTypeLabels(), 4);
        LoadTypeIcons(local);
        EXPECT_EQ(CountTypeLabels(), 4);
        gBattleMons[target].types[0] = TYPE_ICE;
        gBattleMons[target].types[1] = TYPE_ICE;
        LoadTypeIcons(local);
        EXPECT_EQ(CountTypeLabels(), 3);
        for (u32 i = 0; i < MAX_SPRITES; i++)
        {
            struct Sprite *label = &gSprites[i];
            if (!label->inUse || label->template->tileTag != TYPE_ICON_TAG)
                continue;
            EXPECT_NE(GetBattlerSide(label->tTargetBattler), GetBattlerSide(local));
            if (label->tTargetBattler == target)
            {
                s32 oldY = label->y;
                EXPECT_EQ((u32)label->animNum, TYPE_ICE);
                gSprites[gBattlerSpriteIds[target]].y2 = -3;
                label->callback(label);
                EXPECT_EQ(label->y, oldY - 3);
                gSprites[gBattlerSpriteIds[target]].invisible = TRUE;
                label->callback(label);
                EXPECT(label->invisible);
            }
        }
        gBattlerControllerFuncs[local] = NULL;
        for (u32 i = 0; i < MAX_SPRITES; i++)
            if (gSprites[i].inUse && gSprites[i].template->tileTag == TYPE_ICON_TAG)
                gSprites[i].callback(&gSprites[i]);
        EXPECT_EQ(CountTypeLabels(), 0);
        EXPECT_EQ(GetSpriteTileStartByTag(TYPE_ICON_TAG), TAG_NONE);
        EXPECT_EQ(IndexOfSpritePaletteTag(TYPE_ICON_TAG), 0xFF);
        ResetSpriteData();
        if (oldOff)
            FlagSet(FLAG_HIDE_BATTLE_TYPE_ICONS);
    }
}
