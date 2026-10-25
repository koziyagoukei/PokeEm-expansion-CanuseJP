#include "global.h"
#include "battle_transition_pokevirus.h"
#include "gpu_regs.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "constants/rgb.h"

#define CHAIN_HEIGHT 64
#define GROW_FRAMES 56
#define HOLD_FRAMES 20

struct ReleaseChain
{
    s16 x;
    u8 length;
    u8 delay;
    bool8 fromTop;
};

static const struct ReleaseChain sChains[POKEVIRUS_CHAIN_COUNT] =
{
    { 56, 116,  0, TRUE},
    {120, 100,  6, TRUE},
    {184, 124, 12, TRUE},
    { 24, 120,  3, FALSE},
    { 88, 104,  9, FALSE},
    {152, 116,  0, FALSE},
    {216, 112,  6, FALSE},
};

static const u32 sChainTiles[] = INCGFX_U32("graphics/custom/pokevirus/release_transition/chain.png", ".4bpp");
static const u16 sChainPalette[] = INCGFX_U16("graphics/custom/pokevirus/release_transition/chain.png", ".gbapal");
STATIC_ASSERT(sizeof(sChainTiles) == 32 * CHAIN_HEIGHT / 2, PokevirusChainTileSize)

static const struct SpriteSheet sChainSheet = {sChainTiles, sizeof(sChainTiles), POKEVIRUS_CHAIN_TAG};
static const struct SpritePalette sChainPal = {sChainPalette, POKEVIRUS_CHAIN_TAG};
static const struct OamData sChainOam =
{
    .shape = SPRITE_SHAPE(32x64),
    .size = SPRITE_SIZE(32x64),
    .priority = 0,
};
static const struct SpriteTemplate sChainTemplate =
{
    .tileTag = POKEVIRUS_CHAIN_TAG,
    .paletteTag = POKEVIRUS_CHAIN_TAG,
    .oam = &sChainOam,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

EWRAM_DATA static u8 sChainSpriteIds[POKEVIRUS_CHAIN_COUNT * POKEVIRUS_CHAIN_SEGMENTS] = {0};

static void DestroyChains(void)
{
    for (u32 i = 0; i < ARRAY_COUNT(sChainSpriteIds); i++)
    {
        if (sChainSpriteIds[i] != MAX_SPRITES)
            DestroySprite(&gSprites[sChainSpriteIds[i]]);
        sChainSpriteIds[i] = MAX_SPRITES;
    }
    FreeSpriteTilesByTag(POKEVIRUS_CHAIN_TAG);
    FreeSpritePaletteByTag(POKEVIRUS_CHAIN_TAG);
}

static bool32 CreateChains(void)
{
    memset(sChainSpriteIds, MAX_SPRITES, sizeof(sChainSpriteIds));
    if (!CanAllocSpriteTiles(sizeof(sChainTiles) / TILE_SIZE_4BPP))
        return FALSE;
    LoadSpriteSheet(&sChainSheet);
    if (GetSpriteTileStartByTag(POKEVIRUS_CHAIN_TAG) == TAG_NONE)
        return FALSE;
    if (LoadSpritePalette(&sChainPal) == 0xFF)
        return FALSE;

    for (u32 i = 0; i < ARRAY_COUNT(sChainSpriteIds); i++)
    {
        u8 id = CreateSpriteUnchecked(&sChainTemplate, sChains[i / POKEVIRUS_CHAIN_SEGMENTS].x, 0, 0);
        if (id == MAX_SPRITES)
            return FALSE;
        sChainSpriteIds[i] = id;
        gSprites[id].invisible = TRUE;
    }
    return TRUE;
}

static void UpdateChains(u32 frame)
{
    for (u32 i = 0; i < ARRAY_COUNT(sChains); i++)
    {
        const struct ReleaseChain *chain = &sChains[i];
        s32 length = frame > chain->delay ? min((frame - chain->delay) * 3, chain->length) : 0;
        for (u32 j = 0; j < POKEVIRUS_CHAIN_SEGMENTS; j++)
        {
            struct Sprite *sprite = &gSprites[sChainSpriteIds[i * POKEVIRUS_CHAIN_SEGMENTS + j]];
            s32 y = chain->fromTop ? length - CHAIN_HEIGHT / 2 - j * CHAIN_HEIGHT
                                  : DISPLAY_HEIGHT - length + CHAIN_HEIGHT / 2 + j * CHAIN_HEIGHT;
            // Fully off-screen pieces must be hidden, not wrapped by the GBA OBJ Y coordinate.
            sprite->invisible = y + CHAIN_HEIGHT / 2 <= 0 || y - CHAIN_HEIGHT / 2 >= DISPLAY_HEIGHT;
            sprite->y = y;
            sprite->vFlip = !chain->fromTop;
        }
    }
}

static void StartFinalFade(void)
{
    // Preserve the current field tint so the fade does not flash back to the unfaded map.
    CpuCopy32(gPlttBufferFaded, gPlttBufferUnfaded, PLTT_SIZE);
    BeginNormalPaletteFade(PALETTES_ALL, -1, 0, 16, RGB_BLACK);
}

void Task_PokevirusRelease(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    switch (task->data[0])
    {
    case 0:
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON | DISPCNT_BG0_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        if (!CreateChains())
        {
            DestroyChains();
            StartFinalFade();
            task->data[0] = 2;
            break;
        }
        task->data[0] = 1;
        break;
    case 1:
    {
        u32 frame = ++task->data[1];
        u32 chainPalette = IndexOfSpritePaletteTag(POKEVIRUS_CHAIN_TAG);
        UpdateChains(frame);
        BlendPalettes(PALETTES_ALL & ~(1u << (16 + chainPalette)), min(frame / 6, 8), RGB(5, 0, 7));
        if (frame >= GROW_FRAMES + HOLD_FRAMES)
        {
            StartFinalFade();
            task->data[0] = 2;
        }
        break;
    }
    case 2:
        if (!gPaletteFade.active)
        {
            DestroyChains();
            DestroyTask(taskId);
        }
        break;
    }
}
