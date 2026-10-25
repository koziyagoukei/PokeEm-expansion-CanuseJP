#include "global.h"
#include "decoration.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "local_room.h"
#include "overworld.h"
#include "palette.h"
#include "secret_base.h"
#include "special_doll.h"
#include "sprite.h"
#include "test/test.h"
#include "constants/decorations.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/map_groups.h"

#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    extern const u16 gSpecialDollShinyPalette_##symbol[];
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL

static const u16 *const sShinyPalettes[SPECIAL_DOLL_COUNT] =
{
#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    [SPECIAL_DOLL_##_id##_SHINY] = gSpecialDollShinyPalette_##symbol,
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL
};

static const u8 sShinyNamePrefix[] = _("{ENG}{STAR}{JPN}");

static void PrepareRoom(void)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_MINI_MINI_VILLA_LOCAL_ROOM);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_MINI_MINI_VILLA_LOCAL_ROOM);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum);
    InitMap();
    LocalRoom_EnsureSaveData();
    memset(gObjectEvents, 0, sizeof(gObjectEvents));
    for (u32 i = 0; i < LOCAL_ROOM_DECOR_SPRITES; i++)
        FlagSet(FLAG_DECORATION_1 + i);
}

static void PlaceDoll(u8 slot, u8 doll, u8 position)
{
    SpecialDoll_Add(doll);
    gSaveBlock3Ptr->specialDolls.decorations[slot] = doll;
    gSaveBlock3Ptr->specialDolls.positions[slot] = position;
    SpecialDoll_UpdateChecksum();
}

TEST("Special dolls: save extension preserves all pre-existing save offsets")
{
    EXPECT_EQ(offsetof(struct SaveBlock3, localRoom), 28);
    EXPECT_EQ(sizeof(struct LocalRoomSave), 44);
    EXPECT_EQ(offsetof(struct SaveBlock3, specialDolls), 72);
    EXPECT_EQ(sizeof(struct SpecialDollSave), 80);
    EXPECT_EQ(offsetof(struct SaveBlock3, frontierMusic), 152);
    EXPECT_EQ(offsetof(struct SaveBlock3, fusions), 168);
}

TEST("Special dolls: old save bytes initialize without clearing any existing furniture")
{
    u8 prefix[72];
    LocalRoom_EnsureSaveData();
    gSaveBlock3Ptr->localRoom.decorations[0] = DECOR_SMALL_DESK;
    gSaveBlock3Ptr->localRoom.decorationPositions[0] = 0x36;
    LocalRoom_UpdateChecksum();
    memcpy(prefix, gSaveBlock3Ptr, sizeof(prefix));
    memset(&gSaveBlock3Ptr->specialDolls, 0xFF, sizeof(struct SpecialDollSave));
    SpecialDoll_EnsureSaveData();
    EXPECT_EQ(memcmp(prefix, gSaveBlock3Ptr, sizeof(prefix)), 0);
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), 0);
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 0);
}

TEST("Special dolls: all 102 dolls have dedicated single-frame overworld graphics")
{
    EXPECT_EQ(SPECIAL_DOLL_NORMAL_COUNT - 1, 51);
    EXPECT_EQ(SPECIAL_DOLL_COUNT - 1, 102);
    for (u32 doll = 1; doll < SPECIAL_DOLL_COUNT; doll++)
    {
        const struct Decoration *decoration = SpecialDoll_GetDecoration(doll);
        const struct ObjectEventGraphicsInfo *graphics = GetObjectEventGraphicsInfo(decoration->tiles[0]);
        EXPECT_EQ(decoration->permission, DECORPERM_SPRITE);
        EXPECT_EQ(decoration->id, doll);
        EXPECT_EQ(graphics, SpecialDoll_GetGraphicsInfo(decoration->tiles[0]));
        EXPECT_EQ(graphics->paletteTag, SPECIAL_DOLL_PAL_FIRST + doll - 1);
        EXPECT_EQ((u32)graphics->inanimate, TRUE);
        EXPECT_EQ((u32)graphics->compressed, FALSE);
        EXPECT_EQ(graphics->size, graphics->width * graphics->height / 2);
        EXPECT_EQ(graphics->images[0].size, graphics->size);
        EXPECT_EQ((u32)graphics->anims[0][0].frame.imageValue, 0);
        EXPECT(graphics->images[0].data != NULL);
        EXPECT(graphics->width == 32 || graphics->width == 64);
    }
    EXPECT_EQ(SpecialDoll_GetDecoration(0), NULL);
    EXPECT_EQ(SpecialDoll_GetDecoration(SPECIAL_DOLL_COUNT), NULL);
    EXPECT_EQ(SpecialDoll_GetDecoration(0xFFFF), NULL);
    EXPECT_EQ(SpecialDoll_GetGraphicsInfo(OBJ_EVENT_GFX_NINJA_BOY), NULL);
}

TEST("Special dolls: every supported form can be placed on room floor")
{
    PrepareRoom();
    for (u32 doll = 1; doll < SPECIAL_DOLL_COUNT; doll++)
        EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_SPECIAL_DOLL, doll, 3, 9));
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_SPECIAL_DOLL, SPECIAL_DOLL_CHARIZARD_MEGA_X, 1, 4));
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_SPECIAL_DOLL, SPECIAL_DOLL_CHARIZARD_MEGA_X, 3, 2));
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_SPECIAL_DOLL, 255, 3, 9));
}

TEST("Special dolls: placement preview owns a separate palette and cannot free placed doll colors")
{
    u16 dollId = SPECIAL_DOLL_VENUSAUR_MEGA;
    PARAMETRIZE { dollId = SPECIAL_DOLL_VENUSAUR_MEGA; }
    PARAMETRIZE { dollId = SPECIAL_DOLL_VENUSAUR_MEGA_SHINY; }
    u16 previewTag = 0xBE5;
    ResetSpriteData();
    FreeAndReserveObjectSpritePalettes();
    const struct Decoration *doll = SpecialDoll_GetDecoration(dollId);
    const struct ObjectEventGraphicsInfo *graphics = SpecialDoll_GetGraphicsInfo(doll->tiles[0]);
    u8 placedPalette = LoadObjectEventPalette(graphics->paletteTag);
    u8 spriteId = CreateObjectGraphicsSpriteWithTag(doll->tiles[0], SpriteCallbackDummy, 0, 0, 1, previewTag);
    EXPECT(spriteId < MAX_SPRITES);
    EXPECT_EQ((u32)gSprites[spriteId].oam.paletteNum, IndexOfSpritePaletteTag(previewTag));
    EXPECT(gSprites[spriteId].oam.paletteNum != placedPalette);
    DestroySprite(&gSprites[spriteId]);
    FreeSpritePaletteByTag(previewTag);
    EXPECT_EQ(IndexOfSpritePaletteTag(graphics->paletteTag), placedPalette);
    FreeSpritePaletteByTag(graphics->paletteTag);
}

TEST("Special dolls: three dolls count toward the existing twelve-sprite furniture limit")
{
    PrepareRoom();
    for (u32 i = 0; i < 9; i++)
    {
        gSaveBlock3Ptr->localRoom.decorations[i] = DECOR_PIKACHU_DOLL;
        gSaveBlock3Ptr->localRoom.decorationPositions[i] = 0x56;
    }
    LocalRoom_UpdateChecksum();
    PlaceDoll(0, SPECIAL_DOLL_CHARIZARD_MEGA_X, 0x36);
    PlaceDoll(1, SPECIAL_DOLL_CHARIZARD_MEGA_Y_SHINY, 0x96);
    PlaceDoll(2, SPECIAL_DOLL_NECROZMA_ULTRA, 0xC9);
    SpecialDoll_EnsureSaveData();
    EXPECT_EQ(LocalRoom_GetSpriteDecorationCount(), 9);
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 3);
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PICHU_DOLL, 3, 9));
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_SPECIAL_DOLL, SPECIAL_DOLL_VENUSAUR_MEGA, 3, 9));
    SpecialDoll_ClearPlacements();
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PICHU_DOLL, 3, 9));
}

TEST("Special dolls: special-only limit remains three when normal sprite slots are empty")
{
    PrepareRoom();
    PlaceDoll(0, SPECIAL_DOLL_CHARIZARD_MEGA_X, 0x36);
    PlaceDoll(1, SPECIAL_DOLL_CHARIZARD_MEGA_Y, 0x96);
    PlaceDoll(2, SPECIAL_DOLL_NECROZMA_ULTRA, 0xC9);
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_SPECIAL_DOLL, SPECIAL_DOLL_VENUSAUR_MEGA, 3, 9));
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PICHU_DOLL, 3, 9));
}

TEST("Special dolls: malformed IDs positions and unowned duplicates are rejected")
{
    PrepareRoom();
    SpecialDoll_Add(SPECIAL_DOLL_CHARIZARD_MEGA_X);
    gSaveBlock3Ptr->specialDolls.decorations[0] = 255;
    gSaveBlock3Ptr->specialDolls.decorations[1] = SPECIAL_DOLL_CHARIZARD_MEGA_X;
    gSaveBlock3Ptr->specialDolls.positions[1] = 0xFF;
    gSaveBlock3Ptr->specialDolls.decorations[2] = SPECIAL_DOLL_NECROZMA_ULTRA;
    gSaveBlock3Ptr->specialDolls.positions[2] = 0x36;
    SpecialDoll_UpdateChecksum();
    SpecialDoll_EnsureSaveData();
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 0);
    gSaveBlock3Ptr->specialDolls.decorations[0] = SPECIAL_DOLL_CHARIZARD_MEGA_X;
    gSaveBlock3Ptr->specialDolls.positions[0] = 0x36;
    gSaveBlock3Ptr->specialDolls.decorations[1] = SPECIAL_DOLL_CHARIZARD_MEGA_X;
    gSaveBlock3Ptr->specialDolls.positions[1] = 0x96;
    SpecialDoll_UpdateChecksum();
    SpecialDoll_EnsureSaveData();
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 1);
}

TEST("Special dolls: inventory and placement survive a serialized save round trip")
{
    struct SaveBlock3 saved;
    PrepareRoom();
    PlaceDoll(0, SPECIAL_DOLL_NECROZMA_ULTRA, 0x36);
    PlaceDoll(1, SPECIAL_DOLL_GROUDON_PRIMAL_SHINY, 0x96);
    memcpy(&saved, gSaveBlock3Ptr, sizeof(saved));
    memset(gSaveBlock3Ptr, 0, sizeof(saved));
    memcpy(gSaveBlock3Ptr, &saved, sizeof(saved));
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), 2);
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 2);
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.decorations[0], SPECIAL_DOLL_NECROZMA_ULTRA);
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.positions[0], 0x36);
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.decorations[1], SPECIAL_DOLL_GROUDON_PRIMAL_SHINY);
}

TEST("Special dolls: clearing or deleting ordinary secret bases does not clear special inventory")
{
    PrepareRoom();
    PlaceDoll(0, SPECIAL_DOLL_NECROZMA_ULTRA, 0x36);
    ClearSecretBases();
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 1);
    gSaveBlock1Ptr->secretBases[0].decorations[0] = DECOR_PIKACHU_DOLL;
    gSaveBlock1Ptr->playerRoomDecorations[0] = DECOR_PICHU_DOLL;
    LocalRoom_ClearDecorationData();
    EXPECT_EQ(SpecialDoll_GetPlacedCount(), 0);
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), 1);
    EXPECT_EQ(gSaveBlock1Ptr->secretBases[0].decorations[0], DECOR_PIKACHU_DOLL);
    EXPECT_EQ(gSaveBlock1Ptr->playerRoomDecorations[0], DECOR_PICHU_DOLL);
}

TEST("Special dolls: three large doll objects reload without duplicates and clean up their sprites")
{
    u8 dollSprites[SPECIAL_DOLL_PLACEMENT_COUNT];
    PrepareRoom();
    ResetSpriteData();
    FreeAndReserveObjectSpritePalettes();
    memcpy(gSaveBlock1Ptr->objectEventTemplates, gMapHeader.events->objectEvents,
        gMapHeader.events->objectEventCount * sizeof(struct ObjectEventTemplate));
    PlaceDoll(0, SPECIAL_DOLL_GROUDON_PRIMAL, 0x36);
    PlaceDoll(1, SPECIAL_DOLL_GROUDON_PRIMAL_SHINY, 0x96);
    PlaceDoll(2, SPECIAL_DOLL_RAYQUAZA_MEGA_SHINY, 0xC9);
    for (u32 reload = 0; reload < 2; reload++)
    {
        u32 count = 0;
        LocalRoom_InitObjects();
        AnimateSprites();
        BuildOamBuffer();
        ProcessSpriteCopyRequests();
        for (u32 i = 0; i < OBJECT_EVENTS_COUNT; i++)
        {
            if (!gObjectEvents[i].active)
                continue;
            u8 spriteId = gObjectEvents[i].spriteId;
            dollSprites[count++] = spriteId;
            const struct ObjectEventGraphicsInfo *graphics = SpecialDoll_GetGraphicsInfo(gObjectEvents[i].graphicsId);
            EXPECT(graphics != NULL);
            EXPECT(spriteId < MAX_SPRITES);
            EXPECT(gSprites[spriteId].inUse);
            EXPECT_EQ((u32)gSprites[spriteId].oam.paletteNum, IndexOfSpritePaletteTag(graphics->paletteTag));
            EXPECT(gSprites[spriteId].oam.paletteNum < 16);
            EXPECT_EQ(memcmp((const void *)(OBJ_VRAM0 + gSprites[spriteId].oam.tileNum * TILE_SIZE_4BPP),
                graphics->images[0].data, graphics->size), 0);
        }
        EXPECT_EQ(count, 3);
    }
    HideSecretBaseDecorationSprites();
    for (u32 i = 0; i < OBJECT_EVENTS_COUNT; i++)
        EXPECT(!gObjectEvents[i].active);
    for (u32 i = 0; i < SPECIAL_DOLL_PLACEMENT_COUNT; i++)
        EXPECT(!gSprites[dollSprites[i]].inUse);
}

TEST("Special dolls: shiny variants share pixels but load their own palette without changing normal colors")
{
    ResetSpriteData();
    FreeAndReserveObjectSpritePalettes();
    for (u32 doll = 1; doll < SPECIAL_DOLL_NORMAL_COUNT; doll++)
    {
        u16 shinyDoll = doll + SPECIAL_DOLL_SHINY_OFFSET;
        const struct ObjectEventGraphicsInfo *normal = SpecialDoll_GetGraphicsInfo(SpecialDoll_GetDecoration(doll)->tiles[0]);
        const struct ObjectEventGraphicsInfo *shiny = SpecialDoll_GetGraphicsInfo(SpecialDoll_GetDecoration(shinyDoll)->tiles[0]);
        u8 normalPalette = LoadObjectEventPalette(normal->paletteTag);
        u8 shinyPalette = LoadObjectEventPalette(shiny->paletteTag);
        EXPECT(!SpecialDoll_IsShiny(doll));
        EXPECT(SpecialDoll_IsShiny(shinyDoll));
        EXPECT_EQ(memcmp(SpecialDoll_GetDecoration(shinyDoll)->name, sShinyNamePrefix, sizeof(sShinyNamePrefix) - 1), 0);
        EXPECT_EQ(normal->images, shiny->images);
        EXPECT(normalPalette < 16);
        EXPECT(shinyPalette < 16);
        EXPECT(normalPalette != shinyPalette);
        EXPECT_EQ(memcmp(&gPlttBufferUnfaded[OBJ_PLTT_ID(shinyPalette)], sShinyPalettes[shinyDoll], PLTT_SIZE_4BPP), 0);
        EXPECT(memcmp(&gPlttBufferUnfaded[OBJ_PLTT_ID(normalPalette)], sShinyPalettes[shinyDoll], PLTT_SIZE_4BPP) != 0);
        FreeSpritePaletteByTag(normal->paletteTag);
        FreeSpritePaletteByTag(shiny->paletteTag);
    }
    EXPECT(!SpecialDoll_IsShiny(SPECIAL_DOLL_NONE));
    EXPECT(!SpecialDoll_IsShiny(SPECIAL_DOLL_COUNT));
    EXPECT(!SpecialDoll_IsShiny(0xFFFF));
}

TEST("Special dolls: old normal IDs and valid version one save bytes remain unchanged")
{
    struct SpecialDollSave before;
    PrepareRoom();
    PlaceDoll(0, 2, 0x36);
    PlaceDoll(1, 50, 0x96);
    PlaceDoll(2, 51, 0xC9);
    before = gSaveBlock3Ptr->specialDolls;
    SpecialDoll_EnsureSaveData();
    EXPECT_EQ(memcmp(&before, &gSaveBlock3Ptr->specialDolls, sizeof(before)), 0);
    EXPECT_EQ(SPECIAL_DOLL_CHARIZARD_MEGA_X, 2);
    EXPECT_EQ(SPECIAL_DOLL_GROUDON_PRIMAL, 50);
    EXPECT_EQ(SPECIAL_DOLL_NECROZMA_ULTRA, 51);
    EXPECT_EQ(SPECIAL_DOLL_VENUSAUR_MEGA_SHINY, 52);
    EXPECT_EQ(SPECIAL_DOLL_NECROZMA_ULTRA_SHINY, 102);
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.version, 1);
}

TEST("Special dolls: normal and shiny dolls share the same bounded inventory without replacing each other")
{
    PrepareRoom();
    for (u32 slot = 0; slot < SPECIAL_DOLL_CAPACITY / 2; slot++)
    {
        EXPECT(SpecialDoll_Add(SPECIAL_DOLL_VENUSAUR_MEGA));
        EXPECT(SpecialDoll_Add(SPECIAL_DOLL_VENUSAUR_MEGA_SHINY));
    }
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), SPECIAL_DOLL_CAPACITY);
    EXPECT(!SpecialDoll_Add(SPECIAL_DOLL_NECROZMA_ULTRA_SHINY));
    gSaveBlock3Ptr->specialDolls.inventory[0] = SPECIAL_DOLL_NONE;
    SpecialDoll_CondenseInventory();
    EXPECT_EQ(SpecialDoll_GetOwnedCount(), SPECIAL_DOLL_CAPACITY - 1);
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.inventory[0], SPECIAL_DOLL_VENUSAUR_MEGA_SHINY);
    EXPECT(SpecialDoll_Add(SPECIAL_DOLL_NECROZMA_ULTRA_SHINY));
    EXPECT_EQ(gSaveBlock3Ptr->specialDolls.inventory[SPECIAL_DOLL_CAPACITY - 1], SPECIAL_DOLL_NECROZMA_ULTRA_SHINY);
}
