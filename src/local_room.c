#include "global.h"
#include "decoration.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_player_avatar.h"
#include "fieldmap.h"
#include "fldeff_misc.h"
#include "local_room.h"
#include "metatile_behavior.h"
#include "palette.h"
#include "pokemon.h"
#include "script.h"
#include "secret_base.h"
#include "special_doll.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "util.h"
#include "constants/battle.h"
#include "constants/decorations.h"
#include "constants/event_objects.h"
#include "constants/maps.h"
#include "constants/map_groups.h"
#include "constants/metatile_labels.h"
#include "constants/pokemon.h"

static EWRAM_DATA bool8 sDecorating = FALSE;
static EWRAM_DATA u8 sRoamerPartySlots[LOCAL_ROOM_MAX_ROAMERS] = {};

static const u16 sRoamerFlags[LOCAL_ROOM_MAX_ROAMERS] =
{
    FLAG_TEMP_11, FLAG_TEMP_12, FLAG_TEMP_13,
};

static const u16 sFloorPalettes[LOCAL_ROOM_STYLE_COUNT * 16] =
    INCBIN_U16("data/tilesets/secondary/local_room/floor_palettes.bin");

static const u16 sWallPalettes[LOCAL_ROOM_STYLE_COUNT * 16] =
    INCBIN_U16("data/tilesets/secondary/local_room/wall_palettes.bin");

static const u32 sFloorTiles[LOCAL_ROOM_STYLE_COUNT * 32] =
    INCBIN_U32("data/tilesets/secondary/local_room/floors.bin");
static const u32 sWallTiles[LOCAL_ROOM_STYLE_COUNT * 64] =
    INCBIN_U32("data/tilesets/secondary/local_room/walls.bin");
static const u16 sBlankPalette[16] = {};
static const u16 sEntrancePalette[16] =
    INCGFX_U16("data/tilesets/secondary/brendans_mays_house/palettes/07.pal", ".gbapal");

STATIC_ASSERT(LOCAL_ROOM_DECOR_COUNT <= DECOR_MAX_SECRET_BASE, LocalRoomRearrangementCapacity);
STATIC_ASSERT(LOCAL_ROOM_DECOR_SPRITES + LOCAL_ROOM_MAX_ROAMERS + 1 <= OBJECT_EVENTS_COUNT, LocalRoomObjectCapacity);
STATIC_ASSERT(LOCAL_ROOM_WIDTH <= 16 && LOCAL_ROOM_HEIGHT <= 16, LocalRoomPackedPositions);
STATIC_ASSERT(sizeof(struct LocalRoomSave) == 44, LocalRoomSaveSize);

bool8 LocalRoom_IsCurrentMap(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_MINI_MINI_VILLA_LOCAL_ROOM)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_MINI_MINI_VILLA_LOCAL_ROOM);
}

void LocalRoom_UpdateChecksum(void)
{
    struct LocalRoomSave *room = &gSaveBlock3Ptr->localRoom;
    room->checksum = CalcCRC16((const u8 *)room, offsetof(struct LocalRoomSave, checksum));
}

static bool8 IsProtectedTile(s16 x, s16 y)
{
    return ((x == 7 || x == 8) && y == 12)
        || (x == 1 && (y == 3 || y == 4));
}

bool8 LocalRoom_IsPlacementAllowed(u8 decor, s16 x, s16 y)
{
    if (decor == DECOR_NONE || decor > NUM_DECORATIONS)
        return FALSE;
    return LocalRoom_IsPlacementAllowedForDecoration(&gDecorations[decor], x, y);
}

bool8 LocalRoom_IsPlacementAllowedForDecoration(const struct Decoration *decoration, s16 x, s16 y)
{
    u8 width, height;
    s16 dx, dy;
    u8 permission;

    if (!GetDecorationDimensionsForDecoration(decoration, &width, &height))
        return FALSE;

    permission = decoration->permission;
    if (x < 1 || x + width > LOCAL_ROOM_WIDTH - 1 || y >= LOCAL_ROOM_HEIGHT - 2)
        return FALSE;
    if (permission == DECORPERM_NA_WALL)
        return y == 2 && height == 1;
    if (y - height + 1 < (permission == DECORPERM_BEHIND_FLOOR ? 2 : 3))
        return FALSE;

    for (dy = 0; dy < height; dy++)
        for (dx = 0; dx < width; dx++)
            if (IsProtectedTile(x + dx, y - dy))
                return FALSE;

    return TRUE;
}

bool8 LocalRoom_IsPlainFloor(s16 mapX, s16 mapY)
{
    return LocalRoom_IsCurrentMap()
        && MapGridGetCollisionAt(mapX, mapY) == 0
        && MetatileBehavior_IsNormal(MapGridGetMetatileBehaviorAt(mapX, mapY));
}

bool8 LocalRoom_HasAccessPathAfterPlacement(u8 decor, s16 x, s16 y)
{
    if (decor == DECOR_NONE || decor > NUM_DECORATIONS)
        return FALSE;
    return LocalRoom_HasAccessPathForDecoration(&gDecorations[decor], x, y);
}

bool8 LocalRoom_HasAccessPathForDecoration(const struct Decoration *decoration, s16 x, s16 y)
{
    static const s8 dx[] = {0, 1, 0, -1};
    static const s8 dy[] = {-1, 0, 1, 0};
    u16 blocked[LOCAL_ROOM_HEIGHT] = {};
    u16 visited[LOCAL_ROOM_HEIGHT] = {};
    u8 queue[LOCAL_ROOM_WIDTH * LOCAL_ROOM_HEIGHT];
    u16 head = 0, tail = 0;
    s16 tx, ty;
    u8 width, height, i;

    if (!LocalRoom_IsCurrentMap() || !LocalRoom_IsPlacementAllowedForDecoration(decoration, x, y))
        return FALSE;
    GetDecorationDimensionsForDecoration(decoration, &width, &height);

    for (ty = 0; ty < LOCAL_ROOM_HEIGHT; ty++)
    {
        for (tx = 0; tx < LOCAL_ROOM_WIDTH; tx++)
        {
            u8 objectId = GetObjectEventIdByPosition(tx + MAP_OFFSET, ty + MAP_OFFSET, 0);
            if (MapGridGetCollisionAt(tx + MAP_OFFSET, ty + MAP_OFFSET) != 0
             || (objectId != OBJECT_EVENTS_COUNT && objectId != gPlayerAvatar.objectEventId))
                blocked[ty] |= 1 << tx;
        }
    }

    // Match the collision applied by ShowDecorationOnMap and furniture objects.
    for (ty = 0; ty < height; ty++)
    {
        for (tx = 0; tx < width; tx++)
        {
            if (decoration->permission == DECORPERM_SPRITE)
            {
                if (ty == 0)
                    blocked[y] |= 1 << (x + tx);
            }
            else if (decoration->permission != DECORPERM_NA_WALL)
            {
                u16 tile = NUM_TILES_IN_PRIMARY + decoration->tiles[(height - 1 - ty) * width + tx];
                u8 behavior = GetAttributeByMetatileIdAndMapLayout(tile, METATILE_ATTRIBUTE_BEHAVIOR, FALSE);
                u8 layer = GetAttributeByMetatileIdAndMapLayout(tile, METATILE_ATTRIBUTE_LAYER_TYPE, FALSE);
                if (MetatileBehavior_IsSecretBaseImpassable(behavior)
                 || (decoration->permission != DECORPERM_PASS_FLOOR && layer != METATILE_LAYER_TYPE_NORMAL))
                    blocked[y - ty] |= 1 << (x + tx);
            }
        }
    }

    for (tx = 7; tx <= 8; tx++)
    {
        if (!(blocked[12] & (1 << tx)))
        {
            visited[12] |= 1 << tx;
            queue[tail++] = tx << 4 | 12;
        }
    }
    while (head < tail)
    {
        u8 position = queue[head++];
        tx = position >> 4;
        ty = position & 15;
        if (tx == 1 && ty == 4)
            return TRUE;
        for (i = 0; i < ARRAY_COUNT(dx); i++)
        {
            s16 nextX = tx + dx[i], nextY = ty + dy[i];
            if (nextX < 0 || nextX >= LOCAL_ROOM_WIDTH || nextY < 0 || nextY >= LOCAL_ROOM_HEIGHT)
                continue;
            if ((blocked[nextY] | visited[nextY]) & (1 << nextX))
                continue;
            visited[nextY] |= 1 << nextX;
            queue[tail++] = nextX << 4 | nextY;
        }
    }
    return FALSE;
}

void LocalRoom_EnsureSaveData(void)
{
    struct LocalRoomSave *room = &gSaveBlock3Ptr->localRoom;
    u8 i, sprites = 0;

    if (room->magic != LOCAL_ROOM_SAVE_MAGIC
     || room->version != LOCAL_ROOM_SAVE_VERSION
     || room->checksum != CalcCRC16((const u8 *)room, offsetof(struct LocalRoomSave, checksum)))
    {
        memset(room, 0, sizeof(*room));
        room->magic = LOCAL_ROOM_SAVE_MAGIC;
        room->version = LOCAL_ROOM_SAVE_VERSION;
    }

    if (room->floorStyle >= LOCAL_ROOM_STYLE_COUNT)
        room->floorStyle = 0;
    if (room->wallStyle >= LOCAL_ROOM_STYLE_COUNT)
        room->wallStyle = 0;
    if (room->roamerCount > LOCAL_ROOM_MAX_ROAMERS)
        room->roamerCount = 0;

    for (i = 0; i < LOCAL_ROOM_DECOR_COUNT; i++)
    {
        u8 decor = room->decorations[i];
        if (decor == DECOR_NONE)
            room->decorationPositions[i] = 0;
        else if (!LocalRoom_IsPlacementAllowed(decor, room->decorationPositions[i] >> 4, room->decorationPositions[i] & 15)
              || (gDecorations[decor].permission == DECORPERM_SPRITE && ++sprites > LOCAL_ROOM_DECOR_SPRITES))
        {
            room->decorations[i] = DECOR_NONE;
            room->decorationPositions[i] = 0;
        }
    }
    LocalRoom_UpdateChecksum();
    SpecialDoll_EnsureSaveData();
}

static void HideRoamers(void)
{
    u8 i;
    for (i = 0; i < LOCAL_ROOM_MAX_ROAMERS; i++)
    {
        FlagSet(sRoamerFlags[i]);
        RemoveObjectEventByLocalIdAndMap(LOCAL_ROOM_DECOR_SPRITES + 1 + i,
            gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
        sRoamerPartySlots[i] = PARTY_SIZE;
    }
}

static void ReserveRoomSpritePalettes(void)
{
    // Field/weather code can overwrite the fixed player/NPC palette banks.
    if (gReservedSpritePaletteCount < SPECIAL_DOLL_FIRST_PALETTE_SLOT)
        gReservedSpritePaletteCount = SPECIAL_DOLL_FIRST_PALETTE_SLOT;
}

void LocalRoom_PrepareMap(void)
{
    u8 i;
    if (!LocalRoom_IsCurrentMap())
        return;
    LocalRoom_EnsureSaveData();
    FlagSet(FLAG_TEMP_HIDE_FOLLOWER);
    for (i = 0; i < LOCAL_ROOM_DECOR_SPRITES; i++)
        FlagSet(FLAG_DECORATION_1 + i);
    HideRoamers();
}

void LocalRoom_LoadDecorations(void)
{
    u8 i;
    s16 x, y;
    struct LocalRoomSave *room = &gSaveBlock3Ptr->localRoom;
    if (!LocalRoom_IsCurrentMap())
        return;
    LocalRoom_EnsureSaveData();
    for (y = 0; y < LOCAL_ROOM_HEIGHT; y++)
        for (x = 0; x < LOCAL_ROOM_WIDTH; x++)
            MapGridSetMetatileEntryAt(x + MAP_OFFSET, y + MAP_OFFSET,
                gMapHeader.mapLayout->map[y * LOCAL_ROOM_WIDTH + x]);
    for (i = 0; i < LOCAL_ROOM_DECOR_COUNT; i++)
    {
        u8 decor = room->decorations[i];
        if (decor != DECOR_NONE && gDecorations[decor].permission != DECORPERM_SPRITE)
            ShowDecorationOnMap((room->decorationPositions[i] >> 4) + MAP_OFFSET,
                (room->decorationPositions[i] & 15) + MAP_OFFSET, decor);
    }
}

void LocalRoom_ApplyStyle(void)
{
    const u32 *floorTiles, *wallTiles;
    if (!LocalRoom_IsCurrentMap())
        return;
    LocalRoom_EnsureSaveData();
    LoadPalette(&sFloorPalettes[gSaveBlock3Ptr->localRoom.floorStyle * 16], BG_PLTT_ID(7), PLTT_SIZE_4BPP);
    LoadPalette(&sWallPalettes[gSaveBlock3Ptr->localRoom.wallStyle * 16], BG_PLTT_ID(8), PLTT_SIZE_4BPP);
    LoadPalette(sBlankPalette, BG_PLTT_ID(9), PLTT_SIZE_4BPP);
    LoadPalette(sEntrancePalette, BG_PLTT_ID(10), PLTT_SIZE_4BPP);
    floorTiles = &sFloorTiles[gSaveBlock3Ptr->localRoom.floorStyle * 32];
    wallTiles = &sWallTiles[gSaveBlock3Ptr->localRoom.wallStyle * 64];
    CpuCopy16(floorTiles, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x224)), 64);
    CpuCopy16(floorTiles + 16, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x234)), 64);
    CpuCopy16(wallTiles, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x240)), 64);
    CpuCopy16(wallTiles + 16, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x250)), 64);
    CpuCopy16(wallTiles, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x204)), 64);
    CpuCopy16(wallTiles + 16, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x214)), 64);
    CpuCopy16(wallTiles + 32, (void *)(BG_VRAM + TILE_OFFSET_4BPP(0x260)), 128);
}

static bool8 IsRoamerSpaceClear(s16 x, s16 y)
{
    s16 dx, dy;
    for (dy = -1; dy <= 1; dy++)
    {
        for (dx = -1; dx <= 1; dx++)
        {
            s16 mapX = x + dx + MAP_OFFSET, mapY = y + dy + MAP_OFFSET;
            if (IsProtectedTile(x + dx, y + dy)
             || MapGridGetCollisionAt(mapX, mapY) != 0
             || !MetatileBehavior_IsNormal(MapGridGetMetatileBehaviorAt(mapX, mapY))
             || GetObjectEventIdByPosition(mapX, mapY, 0) != OBJECT_EVENTS_COUNT)
                return FALSE;
        }
    }
    return TRUE;
}

static void RefreshRoamers(void)
{
    u8 slot, count = 0;
    HideRoamers();
    if (sDecorating)
        return;

    for (slot = 0; slot < PARTY_SIZE && count < gSaveBlock3Ptr->localRoom.roamerCount; slot++)
    {
        s16 x, y;
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        enum Species species = GetMonData(mon, MON_DATA_SPECIES);
        bool32 shiny = IsMonShiny(mon);
        bool32 female = GetMonGender(mon) == MON_FEMALE;
        u8 localId = LOCAL_ROOM_DECOR_SPRITES + 1 + count;
        bool8 spawned = FALSE;

        if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG)
         || SpeciesToGraphicsInfo(species, shiny, female) == NULL)
            continue;

        for (y = 6; y <= 10 && !spawned; y += 3)
        {
            for (x = 3; x <= 12 && !spawned; x += 3)
            {
                u8 objectId;
                if (!IsRoamerSpaceClear(x, y))
                    continue;
                VarSet(VAR_OBJ_GFX_ID_C + count, GetGraphicsIdForMon(species, shiny, female));
                FlagClear(sRoamerFlags[count]);
                // Set the template before spawning; saved template coords may be from a previous visit.
                gSaveBlock1Ptr->objectEventTemplates[localId - 1].x = x;
                gSaveBlock1Ptr->objectEventTemplates[localId - 1].y = y;
                TrySpawnObjectEvent(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
                if (!TryGetObjectEventIdByLocalIdAndMap(localId, gSaveBlock1Ptr->location.mapNum,
                    gSaveBlock1Ptr->location.mapGroup, &objectId))
                {
                    sRoamerPartySlots[count++] = slot;
                    spawned = TRUE;
                }
                else
                {
                    FlagSet(sRoamerFlags[count]);
                }
            }
        }
    }
}

void LocalRoom_InitObjects(void)
{
    if (!LocalRoom_IsCurrentMap())
        return;
    ReserveRoomSpritePalettes();
    HideSecretBaseDecorationSprites();
    gSpecialVar_0x8004 = 0;
    InitSecretBaseDecorationSprites();
    SpecialDoll_InitObjects();
    RefreshRoamers();
}

void LocalRoom_Resume(void)
{
    if (!LocalRoom_IsCurrentMap())
        return;
    ReserveRoomSpritePalettes();
    FlagSet(FLAG_TEMP_HIDE_FOLLOWER);
    LocalRoom_ApplyStyle();
}

void LocalRoom_ReturnToField(void)
{
    // Resume runs after ResetSpriteData, before object sprites have been rebuilt.
    if (!LocalRoom_IsCurrentMap())
        return;
    if (VarGet(VAR_TEMP_0) != 0)
        RefreshRoamers();
}

void LocalRoom_BeginDecorating(void)
{
    ReserveRoomSpritePalettes();
    sDecorating = TRUE;
    HideRoamers();
}

void LocalRoom_EndDecorating(void)
{
    sDecorating = FALSE;
    if (LocalRoom_IsCurrentMap())
        RefreshRoamers();
}

void LocalRoom_ShowDecorationMenu(void)
{
    if (LocalRoom_IsCurrentMap())
        CreateTask(DoLocalRoomDecorationMenu, 0);
}

static void RedrawRoom(void)
{
    HideRoamers();
    HideSecretBaseDecorationSprites();
    LocalRoom_LoadDecorations();
    LocalRoom_InitObjects();
    LocalRoom_ApplyStyle();
    DrawWholeMapView();
}

void LocalRoom_SetFloorStyle(void)
{
    if (LocalRoom_IsCurrentMap() && gSpecialVar_Result < LOCAL_ROOM_STYLE_COUNT)
    {
        gSaveBlock3Ptr->localRoom.floorStyle = gSpecialVar_Result;
        LocalRoom_UpdateChecksum();
        LocalRoom_ApplyStyle();
    }
}

void LocalRoom_SetWallStyle(void)
{
    if (LocalRoom_IsCurrentMap() && gSpecialVar_Result < LOCAL_ROOM_STYLE_COUNT)
    {
        gSaveBlock3Ptr->localRoom.wallStyle = gSpecialVar_Result;
        LocalRoom_UpdateChecksum();
        LocalRoom_ApplyStyle();
    }
}

void LocalRoom_SetRoamerCount(void)
{
    if (LocalRoom_IsCurrentMap() && gSpecialVar_Result <= LOCAL_ROOM_MAX_ROAMERS)
    {
        gSaveBlock3Ptr->localRoom.roamerCount = gSpecialVar_Result;
        LocalRoom_UpdateChecksum();
        RefreshRoamers();
    }
}

void LocalRoom_ClearDecorations(void)
{
    if (LocalRoom_IsCurrentMap())
    {
        LocalRoom_ClearDecorationData();
        RedrawRoom();
    }
}

void LocalRoom_ClearDecorationData(void)
{
    memset(gSaveBlock3Ptr->localRoom.decorations, 0, LOCAL_ROOM_DECOR_COUNT);
    memset(gSaveBlock3Ptr->localRoom.decorationPositions, 0, LOCAL_ROOM_DECOR_COUNT);
    LocalRoom_UpdateChecksum();
    SpecialDoll_ClearPlacements();
}

void LocalRoom_BufferMonMessage(void)
{
    u16 localId = gSpecialVar_LastTalked;
    u8 index;
    if (localId <= LOCAL_ROOM_DECOR_SPRITES || localId > LOCAL_ROOM_DECOR_SPRITES + LOCAL_ROOM_MAX_ROAMERS)
        return;
    index = sRoamerPartySlots[localId - LOCAL_ROOM_DECOR_SPRITES - 1];
    if (index < PARTY_SIZE)
        GetMonData(&gParties[B_TRAINER_PLAYER][index], MON_DATA_NICKNAME, gStringVar1);
}

void LocalRoom_PerStepCallback(u8 taskId)
{
    s16 x, y;
    s16 *data = gTasks[taskId].data;
    if (!LocalRoom_IsCurrentMap())
        return;
    PlayerGetDestCoords(&x, &y);
    if (!data[1] || (data[2] == x && data[3] == y))
    {
        data[1] = 1;
    }
    else
    {
        u8 behavior = MapGridGetMetatileBehaviorAt(x, y);
        if (MetatileBehavior_IsSecretBaseBalloon(behavior))
            PopSecretBaseBalloon(MapGridGetMetatileIdAt(x, y), x, y);
        else if (MetatileBehavior_IsSecretBaseBreakableDoor(behavior))
            ShatterSecretBaseBreakableDoor(x, y);
    }
    data[2] = x;
    data[3] = y;
}
