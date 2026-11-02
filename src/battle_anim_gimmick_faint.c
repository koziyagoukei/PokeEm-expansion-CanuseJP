#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_terastal.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "constants/rgb.h"

// Dedicated to this effect; do not share the activation animation's resources.
#define TAG_TERA_FAINT_CRYSTAL 0xF1A0
#define TERA_FAINT_CRYSTALS 12

static void SpriteCB_TeraFaintCrystal(struct Sprite *sprite);
static void Task_TeraFaintCrystals(u8 taskId);

static const u32 sTeraFaintCrystalTiles[] = INCGFX_U32("graphics/battle_anims/sprites/tera_faint_crystal.png", ".4bpp");

static const struct SpriteSheet sTeraFaintCrystalSheet =
{
    .data = sTeraFaintCrystalTiles,
    .size = sizeof(sTeraFaintCrystalTiles),
    .tag = TAG_TERA_FAINT_CRYSTAL,
};

static const union AnimCmd sCrystalIdle[] =
{
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_END,
};

static const union AnimCmd sCrystalDissolve[] =
{
    ANIMCMD_FRAME(4, 4),
    ANIMCMD_FRAME(8, 4),
    ANIMCMD_FRAME(12, 5),
    ANIMCMD_FRAME(16, 5),
    ANIMCMD_FRAME(20, 5),
    ANIMCMD_FRAME(24, 5),
    ANIMCMD_FRAME(28, 6),
    ANIMCMD_END,
};

static const union AnimCmd *const sCrystalAnims[] =
{
    sCrystalIdle,
    sCrystalDissolve,
};

static const struct SpriteTemplate sTeraFaintCrystalTemplate =
{
    .tileTag = TAG_TERA_FAINT_CRYSTAL,
    .paletteTag = TAG_TERA_FAINT_CRYSTAL,
    .oam = &gOamData_AffineOff_ObjNormal_16x16,
    .anims = sCrystalAnims,
    .callback = SpriteCB_TeraFaintCrystal,
};

static void LoadTeraFaintPalette(enum Type type, u32 slot)
{
    u16 palette[16] = {0};
    u16 color = GetTeraTypeRGB(type);
    u32 r = color & 31;
    u32 g = (color >> 5) & 31;
    u32 b = (color >> 10) & 31;

    palette[1] = RGB(r / 3, g / 3, b / 3);
    palette[2] = RGB(r * 2 / 3, g * 2 / 3, b * 2 / 3);
    palette[3] = color;
    palette[4] = RGB((r + 62) / 3, (g + 62) / 3, (b + 62) / 3);
    palette[5] = RGB_WHITE;
    palette[6] = RGB((r + 31) / 2, (g + 31) / 2, (b + 31) / 2);
    palette[7] = color;
    if (type == TYPE_STELLAR)
    {
        palette[2] = RGB(29, 9, 20);
        palette[3] = RGB(10, 25, 29);
        palette[4] = RGB(29, 26, 12);
        palette[6] = RGB(20, 14, 30);
        palette[7] = RGB(12, 29, 17);
    }
    LoadPalette(palette, OBJ_PLTT_ID(slot), sizeof(palette));
}

void AnimTask_TeraFaintCrystals(u8 taskId)
{
    u32 palette;

    if (!CanAllocSpriteTiles(sizeof(sTeraFaintCrystalTiles) / TILE_SIZE_4BPP))
    {
        DestroyAnimVisualTask(taskId);
        return;
    }
    palette = AllocSpritePalette(TAG_TERA_FAINT_CRYSTAL);
    if (palette == 0xFF)
    {
        DestroyAnimVisualTask(taskId);
        return;
    }
    if (LoadSpriteSheet(&sTeraFaintCrystalSheet) == 0xFFFF)
    {
        FreeSpritePaletteByTag(TAG_TERA_FAINT_CRYSTAL);
        DestroyAnimVisualTask(taskId);
        return;
    }
    LoadTeraFaintPalette(GetBattlerTeraType(gBattleAnimAttacker), palette);
    gTasks[taskId].data[3] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2);
    gTasks[taskId].data[4] = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    gTasks[taskId].func = Task_TeraFaintCrystals;
}

static void Task_TeraFaintCrystals(u8 taskId)
{
    static const s8 offsets[TERA_FAINT_CRYSTALS][2] =
    {
        {-22, -12}, {0, -28}, {22, -12},
        {-28, 6}, {0, -6}, {28, 6},
        {-18, -24}, {18, -24}, {0, 16},
        {-22, 18}, {22, 18}, {4, -18},
    };
    struct Task *task = &gTasks[taskId];

    if (task->data[0]++ % 6 == 0 && task->data[2] < TERA_FAINT_CRYSTALS)
    {
        for (u32 i = 0; i < 3; i++)
        {
            u32 n = task->data[2]++;
            u32 spriteId = CreateSprite(&sTeraFaintCrystalTemplate,
                task->data[3] + offsets[n][0], task->data[4] + offsets[n][1], 0);
            if (spriteId != MAX_SPRITES)
            {
                struct Sprite *sprite = &gSprites[spriteId];
                sprite->data[1] = offsets[n][0] < 0 ? -32 : 32;
                sprite->data[3] = taskId;
                sprite->data[4] = 18 + (n % 3) * 4;
                task->data[1]++;
            }
        }
    }
    if (task->data[2] == TERA_FAINT_CRYSTALS && task->data[1] == 0)
    {
        FreeSpriteTilesByTag(TAG_TERA_FAINT_CRYSTAL);
        FreeSpritePaletteByTag(TAG_TERA_FAINT_CRYSTAL);
        DestroyAnimVisualTask(taskId);
    }
}

static void SpriteCB_TeraFaintCrystal(struct Sprite *sprite)
{
    u32 frame = sprite->data[0]++;

    sprite->data[5] += sprite->data[1];
    sprite->x2 = sprite->data[5] >> 8;
    if (frame < sprite->data[4])
    {
        sprite->y2 = -(s32)(frame / 8);
    }
    else
    {
        if (frame == sprite->data[4])
            StartSpriteAnim(sprite, 1);
        sprite->data[2] += 48;
        sprite->y2 = sprite->data[2] >> 8;
        if (sprite->animEnded)
        {
            gTasks[sprite->data[3]].data[1]--;
            DestroySprite(sprite);
        }
    }
}
