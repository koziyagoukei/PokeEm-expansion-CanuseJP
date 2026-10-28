#include "global.h"
#include "battle.h"
#include "battle_pyramid.h"
#include "decoration.h"
#include "fieldmap.h"
#include "main.h"
#include "malloc.h"
#include "overworld.h"
#include "recorded_battle.h"
#include "sprite.h"
#include "util.h"
#include "test/test.h"
#include "constants/battle_pyramid.h"
#include "constants/battle_frontier.h"
#include "constants/decorations.h"
#include "constants/layouts.h"
#include "constants/maps.h"
#include "../src/data/map_group_count.h"

void Test_InitMapLayoutData(const struct MapHeader *mapHeader);
u8 Test_CreateDecorationSelector(u8 decor);
bool8 Test_LoadDecorationGraphics(u8 decor, u16 tilesTag, u16 paletteTag);
extern const struct MapLayout *const gMapLayouts[];

static u32 FreeHeapBytes(void)
{
    const struct MemBlock *head = HeapHead();
    const struct MemBlock *block = head;
    u32 bytes = 0;
    do
    {
        if (!block->allocated)
            bytes += block->size;
        block = block->next;
    } while (block != head);
    return bytes;
}

static void DestroySelector(u32 spriteId)
{
    struct Sprite *sprite = &gSprites[spriteId];
    u16 tiles = GetSpriteTileTagByTileStart(sprite->sheetTileStart);
    u16 palette = GetSpritePaletteTagByPaletteNum(sprite->oam.paletteNum);
    DestroySprite(sprite);
    FreeSpriteTilesByTag(tiles);
    FreeSpritePaletteByTag(palette);
}

TEST("Memory cleanup: map capacity includes borders and rejects invalid or overflowing dimensions")
{
    EXPECT_EQ(MAX_MAP_DATA_SIZE, 8192);
    EXPECT(MapLayoutFitsBuffer(72, 72));
    EXPECT(MapLayoutFitsBuffer(17, 242));
    EXPECT(!MapLayoutFitsBuffer(18, 242));
    EXPECT(!MapLayoutFitsBuffer(0, 10));
    EXPECT(!MapLayoutFitsBuffer(10, 0));
    EXPECT(!MapLayoutFitsBuffer(0xFFFFFFFF, 1));
    EXPECT(!MapLayoutFitsBuffer(1, 0xFFFFFFFF));
    EXPECT(!MapLayoutFitsBuffer(0x10000, 0x10000));
}

TEST("Memory cleanup: all retained maps and generated pyramid floors fit the smaller buffer")
{
    for (u32 group = 0; group < ARRAY_COUNT(MAP_GROUP_COUNT); group++)
        for (u32 map = 0; map < MAP_GROUP_COUNT[group]; map++)
        {
            const struct MapHeader *header = Overworld_GetMapHeaderByGroupAndId(group, map);
            EXPECT(header != NULL);
            EXPECT(MapLayoutFitsBuffer(header->mapLayout->width, header->mapLayout->height));
        }
    for (u32 i = 0; i < NUM_PYRAMID_FLOOR_SQUARES; i++)
    {
        const struct MapLayout *layout = gMapLayouts[LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR + i];
        EXPECT(MapLayoutFitsBuffer(layout->width * PYRAMID_FLOOR_SQUARES_WIDE,
                                   layout->height * PYRAMID_FLOOR_SQUARES_HIGH));
    }
}

TEST("Memory cleanup: largest outdoor map loads all four corners inside the buffer")
{
    struct MapHeader oldHeader = gMapHeader;
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_BATTLE_FRONTIER_OUTSIDE_EAST), MAP_NUM(MAP_BATTLE_FRONTIER_OUTSIDE_EAST));
    const struct MapLayout *layout = gMapHeader.mapLayout;
    Test_InitMapLayoutData(&gMapHeader);
    EXPECT_EQ(gBackupMapLayout.width, layout->width + MAP_OFFSET_W);
    EXPECT_EQ(gBackupMapLayout.height, layout->height + MAP_OFFSET_H);
    for (u32 y = 0; y < layout->height; y += layout->height - 1)
        for (u32 x = 0; x < layout->width; x += layout->width - 1)
            EXPECT_EQ(sBackupMapData[(y + MAP_OFFSET) * gBackupMapLayout.width + x + MAP_OFFSET], layout->map[y * layout->width + x]);
    gMapHeader = oldHeader;
}

TEST("Memory cleanup: generated pyramid floors preserve buffer canaries and unused tail")
{
    struct MapHeader oldHeader = gMapHeader;
    struct BackupMapLayout oldBackup = gBackupMapLayout;
    u16 oldFloor = gSaveBlock2Ptr->frontier.curChallengeBattleNum;
    u16 oldRandoms[4];
    memcpy(oldRandoms, gSaveBlock2Ptr->frontier.pyramidRandoms, sizeof(oldRandoms));
    u16 *guarded = Alloc((MAX_MAP_DATA_SIZE + 2) * sizeof(u16));
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR), MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR));
    for (u32 floor = 0; floor < FRONTIER_STAGES_PER_CHALLENGE; floor++)
        for (u32 seed = 0; seed < 32; seed++)
        {
            for (u32 i = 0; i < MAX_MAP_DATA_SIZE + 2; i++)
                guarded[i] = 0xA55A;
            gSaveBlock2Ptr->frontier.curChallengeBattleNum = floor;
            for (u32 i = 0; i < 4; i++)
                gSaveBlock2Ptr->frontier.pyramidRandoms[i] = 1234 * seed + 4321 * i;
            GenerateBattlePyramidFloorLayout(guarded + 1, TRUE);
            EXPECT_EQ(gBackupMapLayout.width, 32 + MAP_OFFSET_W);
            EXPECT_EQ(gBackupMapLayout.height, 32 + MAP_OFFSET_H);
            EXPECT_EQ(guarded[0], 0xA55A);
            for (u32 i = gBackupMapLayout.width * gBackupMapLayout.height + 1; i < MAX_MAP_DATA_SIZE + 2; i++)
                EXPECT_EQ(guarded[i], 0xA55A);
        }
    gMapHeader = oldHeader;
    gBackupMapLayout = oldBackup;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = oldFloor;
    memcpy(gSaveBlock2Ptr->frontier.pyramidRandoms, oldRandoms, sizeof(oldRandoms));
    Free(guarded);
}

TEST("Memory cleanup: every tile decoration uses VRAM without keeping a heap image")
{
    ResetSpriteData();
    FreeAllSpritePalettes();
    u32 before = FreeHeapBytes();
    for (u32 decor = 1; decor < NUM_DECORATIONS; decor++)
    {
        if (gDecorations[decor].permission == DECORPERM_SPRITE)
            continue;
        u32 spriteId = Test_CreateDecorationSelector(decor);
        EXPECT_LT(spriteId, MAX_SPRITES);
        EXPECT(gSprites[spriteId].usingSheet);
        EXPECT_EQ(gSprites[spriteId].images, NULL);
        EXPECT_EQ(FreeHeapBytes(), before);
        u32 start = gSprites[spriteId].sheetTileStart * TILE_SIZE_4BPP;
        u32 checksum = CalcByteArraySum((const u8 *)OBJ_VRAM0 + start, 128);
        void *otherImage = Alloc(0x1000);
        memset(otherImage, 0xA5, 0x1000);
        AnimateSprite(&gSprites[spriteId]);
        EXPECT_EQ(CalcByteArraySum((const u8 *)OBJ_VRAM0 + start, 128), checksum);
        Free(otherImage);
        DestroySelector(spriteId);
        EXPECT_EQ(CountFreePaletteSlots(), 16);
        EXPECT_EQ(FreeHeapBytes(), before);
    }
}

TEST("Memory cleanup: decoration previews release their temporary graphics after upload")
{
    ResetSpriteData();
    FreeAllSpritePalettes();
    u32 before = FreeHeapBytes();
    for (u32 repeat = 0; repeat < 20; repeat++)
    {
        u32 spriteId = AddDecorationIconObject(DECOR_SMALL_DESK, 20, 84, 1, 0x5000, 0x5001);
        EXPECT_LT(spriteId, MAX_SPRITES);
        EXPECT_EQ(FreeHeapBytes(), before);
        DestroySelector(spriteId);
    }
    EXPECT_EQ(FreeHeapBytes(), before);
}

TEST("Memory cleanup: failed decoration sprite creation releases tiles palette and heap")
{
    ResetSpriteData();
    FreeAllSpritePalettes();
    for (u32 i = 0; i < MAX_SPRITES; i++)
        EXPECT_LT(CreateSpriteUnchecked(&gDummySpriteTemplate, 0, 0, 0), MAX_SPRITES);
    u32 before = FreeHeapBytes();
    EXPECT_EQ(Test_CreateDecorationSelector(DECOR_SMALL_DESK), MAX_SPRITES);
    EXPECT_EQ(FreeHeapBytes(), before);
    EXPECT_EQ(CountFreePaletteSlots(), 16);
    EXPECT(CanAllocSpriteTiles(1024));
    ResetSpriteData();
}

TEST("Memory cleanup: exhausted decoration palettes and tiles do not leak heap")
{
    static const u16 palette[16] = {0};
    ResetSpriteData();
    FreeAllSpritePalettes();
    for (u32 i = 0; i < 16; i++)
        EXPECT_NE(LoadSpritePaletteWithTag(palette, 0x5100 + i), 0xFF);
    u32 before = FreeHeapBytes();
    EXPECT(!Test_LoadDecorationGraphics(DECOR_SMALL_DESK, 0x5000, 0x5001));
    EXPECT_EQ(FreeHeapBytes(), before);
    EXPECT_EQ(GetSpriteTileStartByTag(0x5000), TAG_NONE);
    FreeAllSpritePalettes();
    EXPECT_GE(AllocSpriteTiles(1024), 0);
    EXPECT(!Test_LoadDecorationGraphics(DECOR_SMALL_DESK, 0x5000, 0x5001));
    EXPECT_EQ(FreeHeapBytes(), before);
    EXPECT_EQ(CountFreePaletteSlots(), 16);
    ResetSpriteData();
}

TEST("Memory cleanup: decoration allocation failure is recoverable")
{
    ResetSpriteData();
    FreeAllSpritePalettes();
    u32 before = FreeHeapBytes();
    void *block = AllocUnchecked(before);
    EXPECT(block != NULL);
    EXPECT_EQ(Test_CreateDecorationSelector(DECOR_SMALL_DESK), MAX_SPRITES);
    Free(block);
    EXPECT_EQ(FreeHeapBytes(), before);
    EXPECT_EQ(CountFreePaletteSlots(), 16);
}

static void ReturnFromDisabledPlayback(void)
{
}

TEST("Memory cleanup: disabled battle videos allocate no snapshot and leave parties unchanged")
{
    ASSUME(FREE_SPECIAL_SAVE_SECTORS);
    void *parties = Alloc(sizeof(gParties));
    memcpy(parties, gParties, sizeof(gParties));
    u32 before = FreeHeapBytes();
    MainCallback callback = gMain.callback2;
    for (u32 i = 0; i < 10; i++)
    {
        RecordedBattle_SaveParties();
        EXPECT(!CanCopyRecordedBattleSaveData());
        EXPECT(!MoveRecordedBattleToSaveData());
        PlayRecordedBattle(ReturnFromDisabledPlayback);
        EXPECT_EQ(FreeHeapBytes(), before);
        EXPECT_EQ(memcmp(parties, gParties, sizeof(gParties)), 0);
        EXPECT(gMain.callback2 == ReturnFromDisabledPlayback);
    }
    gMain.callback2 = callback;
    Free(parties);
}

TEST("Memory cleanup: link action recording remains available with video saves disabled")
{
    struct AiThinkingStruct *oldAi = gAiThinkingStruct;
    gAiThinkingStruct = AllocZeroed(sizeof(*gAiThinkingStruct));
    u8 data[32] = {0};
    RecordedBattle_Init(B_RECORD_MODE_RECORDING);
    RecordedBattle_SetBattlerAction(0, B_ACTION_USE_MOVE);
    RecordedBattle_SetBattlerAction(1, 2);
    EXPECT_EQ(RecordedBattle_BufferNewBattlerData(data), 6);
    EXPECT_EQ(data[0], 0);
    EXPECT_EQ(data[1], 1);
    EXPECT_EQ(data[2], B_ACTION_USE_MOVE);
    EXPECT_EQ(data[3], 1);
    EXPECT_EQ(data[4], 1);
    EXPECT_EQ(data[5], 2);
    EXPECT_EQ(RecordedBattle_BufferNewBattlerData(data), 0);
    Free(gAiThinkingStruct);
    gAiThinkingStruct = oldAi;
}
