#include "global.h"
#include "decoration.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "local_room.h"
#include "special_doll.h"
#include "sprite.h"
#include "task.h"
#include "util.h"
#include "constants/decorations.h"
#include "constants/event_objects.h"

const u8 gText_SpecialDolls[] = _("{JPN}とくしゅドール");
static const u8 sDescription[] = _("{JPN}とくべつな すがたの\nドール。べっそうに\n3たいまで かざれる。");
static const u8 sShinyDescription[] = _("{JPN}いろちがいの すがたの\nドール。べっそうに\n3たいまで かざれる。");

static const union AnimCmd sStillAnim[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};
static const union AnimCmd *const sStillAnims[] = {sStillAnim};

#include "data/special_doll_graphics.h"

// Only the front-facing frame is loaded, not the entire follower animation sheet.
#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    static const struct SpriteFrameImage sImages_##symbol[] = {{.data = sPic_##symbol, .size = frameSize * frameSize / 2}}; \
    static const u16 sGraphics_##symbol[] = {SPECIAL_DOLL_GFX_FIRST + SPECIAL_DOLL_##_id - 1}; \
    static const u16 sShinyGraphics_##symbol[] = {SPECIAL_DOLL_GFX_FIRST + SPECIAL_DOLL_##_id##_SHINY - 1};
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL

static const struct Decoration sDecorations[SPECIAL_DOLL_COUNT] =
{
#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    [SPECIAL_DOLL_##_id] = { \
        .id = SPECIAL_DOLL_##_id, .name = _(label), \
        .permission = DECORPERM_SPRITE, .shape = DECORSHAPE_1x1, .category = DECORCAT_DOLL, \
        .description = sDescription, .tiles = sGraphics_##symbol, \
    },
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL
#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    [SPECIAL_DOLL_##_id##_SHINY] = { \
        .id = SPECIAL_DOLL_##_id##_SHINY, .name = _("{ENG}{STAR}" label), \
        .permission = DECORPERM_SPRITE, .shape = DECORSHAPE_1x1, .category = DECORCAT_DOLL, \
        .description = sShinyDescription, .tiles = sShinyGraphics_##symbol, \
    },
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL
};

static const struct ObjectEventGraphicsInfo sGraphicsInfo[SPECIAL_DOLL_COUNT - 1] =
{
#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    [SPECIAL_DOLL_##_id - 1] = { \
        .tileTag = TAG_NONE, .paletteTag = SPECIAL_DOLL_PAL_FIRST + SPECIAL_DOLL_##_id - 1, \
        .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE, .size = frameSize * frameSize / 2, \
        .width = frameSize, .height = frameSize, .paletteSlot = PALSLOT_NPC_1, \
        .inanimate = TRUE, .oam = &gObjectEventBaseOam_##frameSize##x##frameSize, \
        .anims = sStillAnims, .images = sImages_##symbol, .affineAnims = gDummySpriteAffineAnimTable, \
    },
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL
#define SPECIAL_DOLL(_id, symbol, label, path, frameSize) \
    [SPECIAL_DOLL_##_id##_SHINY - 1] = { \
        .tileTag = TAG_NONE, .paletteTag = SPECIAL_DOLL_PAL_FIRST + SPECIAL_DOLL_##_id##_SHINY - 1, \
        .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE, .size = frameSize * frameSize / 2, \
        .width = frameSize, .height = frameSize, .paletteSlot = PALSLOT_NPC_1, \
        .inanimate = TRUE, .oam = &gObjectEventBaseOam_##frameSize##x##frameSize, \
        .anims = sStillAnims, .images = sImages_##symbol, .affineAnims = gDummySpriteAffineAnimTable, \
    },
#include "constants/special_doll_list.h"
#undef SPECIAL_DOLL
};

// Extending the base list must not renumber the already-saved shiny IDs.
STATIC_ASSERT(SPECIAL_DOLL_NORMAL_COUNT == SPECIAL_DOLL_SHINY_OFFSET + 1, SpecialDollStableShinyIds);
STATIC_ASSERT(SPECIAL_DOLL_COUNT <= 256, SpecialDollSavedIds);
STATIC_ASSERT(SPECIAL_DOLL_CAPACITY < 255, SpecialDollInventoryCapacity);
STATIC_ASSERT(SPECIAL_DOLL_GFX_FIRST > OBJ_EVENT_GFX_VAR_F, SpecialDollGraphicsRange);
STATIC_ASSERT(SPECIAL_DOLL_GFX_FIRST + SPECIAL_DOLL_COUNT < OBJ_EVENT_MON_FEMALE, SpecialDollGraphicsFlags);
STATIC_ASSERT(offsetof(struct SaveBlock3, specialDolls) == 72, SpecialDollAppendOnlySave);
STATIC_ASSERT(sizeof(struct SpecialDollSave) == 80, SpecialDollSaveSize);

const struct Decoration *SpecialDoll_GetDecoration(u16 doll)
{
    if (doll == SPECIAL_DOLL_NONE || doll >= SPECIAL_DOLL_COUNT)
        return NULL;
    return &sDecorations[doll];
}

const struct ObjectEventGraphicsInfo *SpecialDoll_GetGraphicsInfo(u16 graphicsId)
{
    if (graphicsId < SPECIAL_DOLL_GFX_FIRST || graphicsId >= SPECIAL_DOLL_GFX_FIRST + SPECIAL_DOLL_COUNT - 1)
        return NULL;
    return &sGraphicsInfo[graphicsId - SPECIAL_DOLL_GFX_FIRST];
}

bool8 SpecialDoll_IsShiny(u16 doll)
{
    return doll >= SPECIAL_DOLL_NORMAL_COUNT && doll < SPECIAL_DOLL_COUNT;
}

void SpecialDoll_UpdateChecksum(void)
{
    struct SpecialDollSave *save = &gSaveBlock3Ptr->specialDolls;
    save->checksum = CalcCRC16((const u8 *)save, offsetof(struct SpecialDollSave, checksum));
}

u8 LocalRoom_GetSpriteDecorationCount(void)
{
    u32 i;
    u8 count = 0;
    if (gSaveBlock3Ptr->localRoom.magic != LOCAL_ROOM_SAVE_MAGIC)
        return 0;
    for (i = 0; i < LOCAL_ROOM_DECOR_COUNT; i++)
    {
        u8 decor = gSaveBlock3Ptr->localRoom.decorations[i];
        if (decor != DECOR_NONE && decor <= NUM_DECORATIONS && gDecorations[decor].permission == DECORPERM_SPRITE)
            count++;
    }
    return count;
}

void SpecialDoll_EnsureSaveData(void)
{
    struct SpecialDollSave *save = &gSaveBlock3Ptr->specialDolls;
    u8 owned[SPECIAL_DOLL_COUNT] = {};
    u32 i, j;
    u8 sprites = LocalRoom_GetSpriteDecorationCount();

    if (save->magic != SPECIAL_DOLL_SAVE_MAGIC || save->version != SPECIAL_DOLL_SAVE_VERSION
     || save->checksum != CalcCRC16((const u8 *)save, offsetof(struct SpecialDollSave, checksum)))
    {
        memset(save, 0, sizeof(*save));
        save->magic = SPECIAL_DOLL_SAVE_MAGIC;
        save->version = SPECIAL_DOLL_SAVE_VERSION;
    }
    for (i = 0; i < SPECIAL_DOLL_CAPACITY; i++)
    {
        if (save->inventory[i] >= SPECIAL_DOLL_COUNT)
            save->inventory[i] = SPECIAL_DOLL_NONE;
        owned[save->inventory[i]]++;
    }
    for (i = 0; i < SPECIAL_DOLL_PLACEMENT_COUNT; i++)
    {
        u8 doll = save->decorations[i];
        u8 position = save->positions[i];
        const struct Decoration *decoration = SpecialDoll_GetDecoration(doll);
        bool8 valid = decoration != NULL && owned[doll] != 0 && sprites < LOCAL_ROOM_DECOR_SPRITES;
        if (valid)
            valid = LocalRoom_IsPlacementAllowedForDecoration(decoration, position >> 4, position & 15);
        for (j = 0; valid && j < i; j++)
            if (save->decorations[j] != SPECIAL_DOLL_NONE && save->positions[j] == position)
                valid = FALSE;
        if (!valid)
        {
            save->decorations[i] = SPECIAL_DOLL_NONE;
            save->positions[i] = 0;
        }
        else
        {
            owned[doll]--;
            sprites++;
        }
    }
    SpecialDoll_UpdateChecksum();
}

bool8 SpecialDoll_Add(u16 doll)
{
    u32 i;
    if (SpecialDoll_GetDecoration(doll) == NULL)
        return FALSE;
    SpecialDoll_EnsureSaveData();
    for (i = 0; i < SPECIAL_DOLL_CAPACITY; i++)
    {
        if (gSaveBlock3Ptr->specialDolls.inventory[i] == SPECIAL_DOLL_NONE)
        {
            gSaveBlock3Ptr->specialDolls.inventory[i] = doll;
            SpecialDoll_UpdateChecksum();
            return TRUE;
        }
    }
    return FALSE;
}

u8 SpecialDoll_GetOwnedCount(void)
{
    u32 i;
    u8 count = 0;
    for (i = 0; i < SPECIAL_DOLL_CAPACITY; i++)
        if (SpecialDoll_GetDecoration(gSaveBlock3Ptr->specialDolls.inventory[i]) != NULL)
            count++;
    return count;
}

void SpecialDoll_CondenseInventory(void)
{
    u32 src, dest = 0;
    u8 *inventory = gSaveBlock3Ptr->specialDolls.inventory;
    for (src = 0; src < SPECIAL_DOLL_CAPACITY; src++)
        if (inventory[src] != SPECIAL_DOLL_NONE)
            inventory[dest++] = inventory[src];
    memset(inventory + dest, 0, SPECIAL_DOLL_CAPACITY - dest);
    SpecialDoll_UpdateChecksum();
}

u8 SpecialDoll_GetPlacedCount(void)
{
    u32 i;
    u8 count = 0;
    for (i = 0; i < SPECIAL_DOLL_PLACEMENT_COUNT; i++)
        if (SpecialDoll_GetDecoration(gSaveBlock3Ptr->specialDolls.decorations[i]) != NULL)
            count++;
    return count;
}

void SpecialDoll_InitObjects(void)
{
    u32 i, slot;
    if (!LocalRoom_IsCurrentMap())
        return;
    SpecialDoll_EnsureSaveData();
    for (i = 0; i < SPECIAL_DOLL_PLACEMENT_COUNT; i++)
    {
        const struct Decoration *decoration = SpecialDoll_GetDecoration(gSaveBlock3Ptr->specialDolls.decorations[i]);
        if (decoration == NULL)
            continue;
        for (slot = 0; slot < LOCAL_ROOM_DECOR_SPRITES; slot++)
        {
            u32 templateId;
            if (!FlagGet(FLAG_DECORATION_1 + slot))
                continue;
            for (templateId = 0; templateId < gMapHeader.events->objectEventCount; templateId++)
            {
                const struct ObjectEventTemplate *template = &gMapHeader.events->objectEvents[templateId];
                u8 objectId;
                s16 x = gSaveBlock3Ptr->specialDolls.positions[i] >> 4;
                s16 y = gSaveBlock3Ptr->specialDolls.positions[i] & 15;
                if (template->flagId != FLAG_DECORATION_1 + slot)
                    continue;
                if (LoadObjectEventPalette(SPECIAL_DOLL_PAL_FIRST + decoration->id - 1) == 0xFF)
                    break;
                VarSet(VAR_OBJ_GFX_ID_0 + template->graphicsId - OBJ_EVENT_GFX_VAR_0, decoration->tiles[0]);
                gSaveBlock1Ptr->objectEventTemplates[templateId].x = x;
                gSaveBlock1Ptr->objectEventTemplates[templateId].y = y;
                FlagClear(template->flagId);
                objectId = TrySpawnObjectEvent(template->localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
                if (objectId == OBJECT_EVENTS_COUNT)
                    FlagSet(template->flagId);
                break;
            }
            break;
        }
    }
}

void SpecialDoll_ClearPlacements(void)
{
    memset(gSaveBlock3Ptr->specialDolls.decorations, 0, sizeof(gSaveBlock3Ptr->specialDolls.decorations));
    memset(gSaveBlock3Ptr->specialDolls.positions, 0, sizeof(gSaveBlock3Ptr->specialDolls.positions));
    SpecialDoll_UpdateChecksum();
}

void SpecialDoll_ShowDecorationMenu(void)
{
    if (LocalRoom_IsCurrentMap())
        CreateTask(DoSpecialDollDecorationMenu, 0);
}
