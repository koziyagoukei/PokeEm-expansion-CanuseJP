#include "global.h"
#include "decoration.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "local_room.h"
#include "overworld.h"
#include "secret_base.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "test/test.h"
#include "constants/decorations.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/metatile_labels.h"

static void SaveDecoration(u8 slot, u8 decor, u8 position)
{
    gSaveBlock3Ptr->localRoom.decorations[slot] = decor;
    gSaveBlock3Ptr->localRoom.decorationPositions[slot] = position;
    LocalRoom_UpdateChecksum();
}

static void EnterLocalRoomMap(void)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_MINI_MINI_VILLA_LOCAL_ROOM);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_MINI_MINI_VILLA_LOCAL_ROOM);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(
        gSaveBlock1Ptr->location.mapGroup, gSaveBlock1Ptr->location.mapNum);
    InitMap();
}

TEST("Local room: append-only extension preserves its pre-existing prefix")
{
    EXPECT_EQ(offsetof(struct SaveBlock3, localRoom), 28);
    EXPECT_EQ(sizeof(struct LocalRoomSave), 44);
    EXPECT_EQ(sizeof(struct SaveBlock3), 72);
}

TEST("Local room: new save starts empty and roaming disabled")
{
    u32 i;
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.magic, LOCAL_ROOM_SAVE_MAGIC);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.version, LOCAL_ROOM_SAVE_VERSION);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.roamerCount, 0);
    for (i = 0; i < LOCAL_ROOM_DECOR_COUNT; i++)
    {
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[i], DECOR_NONE);
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorationPositions[i], 0);
    }
}

TEST("Local room: old-save unused extension bytes are not interpreted as furniture")
{
    u32 i;
    memset(&gSaveBlock3Ptr->localRoom, 0xFF, sizeof(struct LocalRoomSave));
    LocalRoom_EnsureSaveData();
    for (i = 0; i < LOCAL_ROOM_DECOR_COUNT; i++)
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[i], DECOR_NONE);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.roamerCount, 0);
}

TEST("Local room: save data survives a serialized round trip")
{
    struct SaveBlock3 saved;
    LocalRoom_EnsureSaveData();
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    SaveDecoration(15, DECOR_PIKA_POSTER, 0xB2);
    gSaveBlock3Ptr->localRoom.floorStyle = 2;
    gSaveBlock3Ptr->localRoom.wallStyle = 1;
    gSaveBlock3Ptr->localRoom.roamerCount = 3;
    LocalRoom_UpdateChecksum();
    memcpy(&saved, gSaveBlock3Ptr, sizeof(saved));
    memset(gSaveBlock3Ptr, 0, sizeof(saved));
    memcpy(gSaveBlock3Ptr, &saved, sizeof(saved));
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_SMALL_DESK);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorationPositions[0], 0x36);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[15], DECOR_PIKA_POSTER);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.floorStyle, 2);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.wallStyle, 1);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.roamerCount, 3);
}

TEST("Local room: unsupported save version initializes only the local room")
{
    LocalRoom_EnsureSaveData();
    gSaveBlock1Ptr->playerRoomDecorations[0] = DECOR_PIKACHU_DOLL;
    gSaveBlock1Ptr->secretBases[0].decorations[0] = DECOR_POKEMON_DESK;
    gSaveBlock3Ptr->localRoom.version++;
    LocalRoom_UpdateChecksum();
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.version, LOCAL_ROOM_SAVE_VERSION);
    EXPECT_EQ(gSaveBlock1Ptr->playerRoomDecorations[0], DECOR_PIKACHU_DOLL);
    EXPECT_EQ(gSaveBlock1Ptr->secretBases[0].decorations[0], DECOR_POKEMON_DESK);
}

TEST("Local room: corrupted extension resets without touching secret bases")
{
    LocalRoom_EnsureSaveData();
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    gSaveBlock1Ptr->secretBases[0].decorations[0] = DECOR_POKEMON_DESK;
    gSaveBlock3Ptr->localRoom.decorations[0] ^= 0x80;
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_NONE);
    EXPECT_EQ(gSaveBlock1Ptr->secretBases[0].decorations[0], DECOR_POKEMON_DESK);
}

TEST("Local room: invalid style and roamer settings are bounded")
{
    LocalRoom_EnsureSaveData();
    gSaveBlock3Ptr->localRoom.floorStyle = 255;
    gSaveBlock3Ptr->localRoom.wallStyle = 255;
    gSaveBlock3Ptr->localRoom.roamerCount = 255;
    LocalRoom_UpdateChecksum();
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.floorStyle, 0);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.wallStyle, 0);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.roamerCount, 0);
}

TEST("Local room: invalid decoration and out-of-map position cannot index graphics")
{
    LocalRoom_EnsureSaveData();
    SaveDecoration(0, 255, 0x36);
    SaveDecoration(1, DECOR_SMALL_DESK, 0xFF);
    SaveDecoration(2, DECOR_SMALL_DESK, 0x00);
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_NONE);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[1], DECOR_NONE);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[2], DECOR_NONE);
}

TEST("Local room: twelve sprite decorations leave room for player and three roamers")
{
    u32 i;
    LocalRoom_EnsureSaveData();
    for (i = 0; i < LOCAL_ROOM_DECOR_COUNT; i++)
        SaveDecoration(i, DECOR_PIKACHU_DOLL, 0x36);
    LocalRoom_EnsureSaveData();
    for (i = 0; i < LOCAL_ROOM_DECOR_SPRITES; i++)
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[i], DECOR_PIKACHU_DOLL);
    for (; i < LOCAL_ROOM_DECOR_COUNT; i++)
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[i], DECOR_NONE);
}

TEST("Local room: ordinary secret base, player room and local room use independent arrays")
{
    LocalRoom_EnsureSaveData();
    gSaveBlock1Ptr->secretBases[0].secretBaseId = 10;
    gSaveBlock1Ptr->secretBases[0].decorations[0] = DECOR_POKEMON_DESK;
    gSaveBlock1Ptr->secretBases[0].decorationPositions[0] = 0x33;
    gSaveBlock1Ptr->playerRoomDecorations[0] = DECOR_PICHU_DOLL;
    gSaveBlock1Ptr->playerRoomDecorationPositions[0] = 0x44;
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock1Ptr->secretBases[0].decorations[0], DECOR_POKEMON_DESK);
    EXPECT_EQ(gSaveBlock1Ptr->secretBases[0].decorationPositions[0], 0x33);
    EXPECT_EQ(gSaveBlock1Ptr->playerRoomDecorations[0], DECOR_PICHU_DOLL);
    EXPECT_EQ(gSaveBlock1Ptr->playerRoomDecorationPositions[0], 0x44);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_SMALL_DESK);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorationPositions[0], 0x36);
}

TEST("Local room: removing ordinary secret bases leaves local and player room furniture intact")
{
    LocalRoom_EnsureSaveData();
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    gSaveBlock1Ptr->secretBases[0].decorations[0] = DECOR_POKEMON_DESK;
    gSaveBlock1Ptr->playerRoomDecorations[0] = DECOR_PICHU_DOLL;
    ClearSecretBases();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_SMALL_DESK);
    EXPECT_EQ(gSaveBlock1Ptr->playerRoomDecorations[0], DECOR_PICHU_DOLL);
}

TEST("Local room: clearing local furniture leaves ordinary secret bases and player room intact")
{
    LocalRoom_EnsureSaveData();
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    gSaveBlock1Ptr->secretBases[0].decorations[0] = DECOR_POKEMON_DESK;
    gSaveBlock1Ptr->playerRoomDecorations[0] = DECOR_PICHU_DOLL;
    gSaveBlock3Ptr->localRoom.floorStyle = 2;
    LocalRoom_UpdateChecksum();
    LocalRoom_ClearDecorationData();
    LocalRoom_EnsureSaveData();
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_NONE);
    EXPECT_EQ(gSaveBlock3Ptr->localRoom.floorStyle, 2);
    EXPECT_EQ(gSaveBlock1Ptr->secretBases[0].decorations[0], DECOR_POKEMON_DESK);
    EXPECT_EQ(gSaveBlock1Ptr->playerRoomDecorations[0], DECOR_PICHU_DOLL);
}

TEST("Local room: entrance and PC access cannot be covered by furniture")
{
    EXPECT(LocalRoom_IsPlacementAllowed(DECOR_SMALL_DESK, 7, 10));
    EXPECT(LocalRoom_IsPlacementAllowed(DECOR_HEAVY_DESK, 6, 10));
    EXPECT(!LocalRoom_IsPlacementAllowed(DECOR_PIKACHU_DOLL, 1, 4));
    EXPECT(!LocalRoom_IsPlacementAllowed(DECOR_SMALL_DESK, 1, 3));
    EXPECT(LocalRoom_IsPlacementAllowed(DECOR_SMALL_DESK, 3, 6));
}

TEST("Local room: posters use the north wall and large furniture stays in bounds")
{
    EXPECT(LocalRoom_IsPlacementAllowed(DECOR_PIKA_POSTER, 11, 2));
    EXPECT(!LocalRoom_IsPlacementAllowed(DECOR_PIKA_POSTER, 11, 3));
    EXPECT(!LocalRoom_IsPlacementAllowed(DECOR_LONG_POSTER, 14, 2));
    EXPECT(!LocalRoom_IsPlacementAllowed(DECOR_HEAVY_DESK, 14, 9));
    EXPECT(!LocalRoom_IsPlacementAllowed(DECOR_NONE, 3, 6));
    EXPECT(!LocalRoom_IsPlacementAllowed(255, 3, 6));
}

TEST("Local room: reloading renders saved furniture once and cleanup removes stale metatiles")
{
    u16 original;
    LocalRoom_EnsureSaveData();
    EnterLocalRoomMap();
    original = MapGridGetMetatileIdAt(3 + MAP_OFFSET, 6 + MAP_OFFSET);
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    LocalRoom_LoadDecorations();
    EXPECT(MapGridGetMetatileIdAt(3 + MAP_OFFSET, 6 + MAP_OFFSET) != original);
    LocalRoom_LoadDecorations();
    EXPECT(MapGridGetMetatileIdAt(3 + MAP_OFFSET, 6 + MAP_OFFSET) != original);
    LocalRoom_ClearDecorationData();
    LocalRoom_LoadDecorations();
    EXPECT_EQ(MapGridGetMetatileIdAt(3 + MAP_OFFSET, 6 + MAP_OFFSET), original);
}

TEST("Local room: map is indoor and not an ordinary secret base")
{
    EnterLocalRoomMap();
    EXPECT(LocalRoom_IsCurrentMap());
    EXPECT(!CurMapIsSecretBase());
    EXPECT_EQ(gMapHeader.mapType, MAP_TYPE_INDOOR);
}

TEST("Local room: every existing decoration has an in-bounds footprint")
{
    u32 decor;
    for (decor = 1; decor <= NUM_DECORATIONS; decor++)
    {
        s16 x, y;
        bool8 allowed = FALSE;
        for (y = 2; y < 12 && !allowed; y++)
            for (x = 1; x < 15 && !allowed; x++)
                allowed = LocalRoom_IsPlacementAllowed(decor, x, y);
        EXPECT(allowed);
    }
}

TEST("Local room: decoration color arguments do not overwrite Japanese control codes")
{
    u8 prefix[9];
    bool8 disabled;
    PARAMETRIZE { disabled = FALSE; }
    PARAMETRIZE { disabled = TRUE; }

    StringCopy(prefix, gText_Color161Shadow161);
    EXPECT_EQ(StringLength(prefix), 8);
    prefix[2] = disabled ? 4 : 2;
    prefix[5] = disabled ? 5 : 3;
    EXPECT_EQ(prefix[0], EXT_CTRL_CODE_BEGIN);
    EXPECT_EQ(prefix[1], EXT_CTRL_CODE_COLOR);
    EXPECT_EQ(prefix[3], EXT_CTRL_CODE_BEGIN);
    EXPECT_EQ(prefix[4], EXT_CTRL_CODE_SHADOW);
    EXPECT_EQ(prefix[6], EXT_CTRL_CODE_BEGIN);
    EXPECT_EQ(prefix[7], EXT_CTRL_CODE_JPN);
    EXPECT_EQ(prefix[8], EOS);
}

static void PreparePlacementTest(void)
{
    u32 i;
    EnterLocalRoomMap();
    memset(gObjectEvents, 0, sizeof(gObjectEvents));
    for (i = 0; i < LOCAL_ROOM_DECOR_SPRITES; i++)
        FlagSet(FLAG_DECORATION_1 + i);
}

TEST("Local room: actual decoration menu accepts all floor furniture on empty floor")
{
    u32 decor;
    PreparePlacementTest();
    for (decor = 1; decor <= NUM_DECORATIONS; decor++)
    {
        if (gDecorations[decor].permission == DECORPERM_SPRITE || gDecorations[decor].permission == DECORPERM_NA_WALL)
            continue;
        if (!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, decor, 3, 9))
            Test_ExitWithResult(TEST_RESULT_FAIL, __LINE__, "Decoration %d cannot be placed on empty floor", decor);
    }
}

TEST("Local room: actual decoration menu accepts dolls and cushions directly on floor")
{
    u32 decor;
    PreparePlacementTest();
    for (decor = 1; decor <= NUM_DECORATIONS; decor++)
    {
        if (gDecorations[decor].permission != DECORPERM_SPRITE)
            continue;
        if (!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, decor, 3, 9))
            Test_ExitWithResult(TEST_RESULT_FAIL, __LINE__, "Sprite decoration %d cannot be placed on empty floor", decor);
    }
}

TEST("Local room: furniture can use the formerly reserved row and central columns")
{
    PreparePlacementTest();
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_SMALL_DESK, 3, 4));
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_HEAVY_DESK, 4, 6));
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_SURF_MAT, 6, 9));
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PIKACHU_DOLL, 7, 9));
}

TEST("Local room: placement cannot close the last walkable route to the PC")
{
    s16 x;
    PreparePlacementTest();
    for (x = 2; x < 15; x++)
        MapGridSetMetatileImpassabilityAt(x + MAP_OFFSET, 5 + MAP_OFFSET, TRUE);
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_SMALL_DESK, 1, 5));
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PIKACHU_DOLL, 1, 5));
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_C_LOW_NOTE_MAT, 1, 5));
}

TEST("Local room: ordinary secret-base and player-room dolls still require a desk or mat")
{
    PreparePlacementTest();
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_SECRET_BASE, DECOR_PIKACHU_DOLL, 3, 9));
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_PLAYER_ROOM, DECOR_PIKACHU_DOLL, 3, 9));
    ShowDecorationOnMap(3 + MAP_OFFSET, 9 + MAP_OFFSET, DECOR_SURF_MAT);
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_SECRET_BASE, DECOR_PIKACHU_DOLL, 4, 8));
    EXPECT(Test_CanPlaceDecorationAt(DECOR_CONTEXT_PLAYER_ROOM, DECOR_PIKACHU_DOLL, 4, 8));
}

TEST("Local room: walls, furniture overlaps and sprite slot limits still reject dolls")
{
    u32 i;
    PreparePlacementTest();
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PIKACHU_DOLL, 3, 2));
    gObjectEvents[1].active = TRUE;
    gObjectEvents[1].currentCoords.x = 3 + MAP_OFFSET;
    gObjectEvents[1].currentCoords.y = 9 + MAP_OFFSET;
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PIKACHU_DOLL, 3, 9));
    gObjectEvents[1].active = FALSE;
    for (i = 0; i < LOCAL_ROOM_DECOR_SPRITES; i++)
        FlagClear(FLAG_DECORATION_1 + i);
    EXPECT(!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, DECOR_PIKACHU_DOLL, 3, 9));
}

TEST("Local room: all three floor styles preserve furniture and placement permissions")
{
    u32 style, decor;
    u16 deskTile;
    PreparePlacementTest();
    SaveDecoration(0, DECOR_SMALL_DESK, 0x36);
    SaveDecoration(1, DECOR_PIKACHU_DOLL, 0x56);
    LocalRoom_LoadDecorations();
    deskTile = MapGridGetMetatileIdAt(3 + MAP_OFFSET, 6 + MAP_OFFSET);
    for (style = 0; style < LOCAL_ROOM_STYLE_COUNT; style++)
    {
        gSpecialVar_Result = style;
        LocalRoom_SetFloorStyle();
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.floorStyle, style);
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[0], DECOR_SMALL_DESK);
        EXPECT_EQ(gSaveBlock3Ptr->localRoom.decorations[1], DECOR_PIKACHU_DOLL);
        EXPECT_EQ(MapGridGetMetatileIdAt(3 + MAP_OFFSET, 6 + MAP_OFFSET), deskTile);
        for (decor = 1; decor <= NUM_DECORATIONS; decor++)
        {
            s16 y = gDecorations[decor].permission == DECORPERM_NA_WALL ? 2 : 9;
            if (!Test_CanPlaceDecorationAt(DECOR_CONTEXT_LOCAL_ROOM, decor, 9, y))
                Test_ExitWithResult(TEST_RESULT_FAIL, __LINE__, "Floor style %d rejects decoration %d", style, decor);
        }
    }
}

TEST("Local room: decoration names have no printable prefix and fit list buffers")
{
    u32 decor;
    u8 name[24];
    for (decor = 1; decor <= NUM_DECORATIONS; decor++)
    {
        EXPECT_LE(StringLength(gText_Color161Shadow161) + StringLength(gDecorations[decor].name) + 1, sizeof(name));
        StringCopy(name, gText_Color161Shadow161);
        name[2] = 2;
        name[5] = 3;
        StringAppend(name, gDecorations[decor].name);
        EXPECT_EQ(name[8], gDecorations[decor].name[0]);
        EXPECT_EQ(GetStringWidth(FONT_NARROW, name, 0), GetStringWidth(FONT_NARROW, gDecorations[decor].name, 0));
    }
}
