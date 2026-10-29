#include "global.h"
#include "battle_anim.h"
#include "bg.h"
#include "contest.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "test/test.h"

TEST("Move backgrounds: every asset loads with padded screens and the correct map width")
{
    u32 bg = BG_NONE;
    u32 layout = 0;
    for (u32 candidate = 0; candidate < BG_COUNT; candidate++)
    {
        PARAMETRIZE_LABEL("background %d battle", candidate) { bg = candidate; layout = 0; }
        PARAMETRIZE_LABEL("background %d contest", candidate) { bg = candidate; layout = 1; }
        PARAMETRIZE_LABEL("background %d cutaway", candidate) { bg = candidate; layout = 2; }
    }
    bool32 contest = layout == 1;
    gMain.inBattle = layout == 0;
    gContestMoveAnimInCutaway = layout == 2;
    const struct BattleAnimBackground *asset = &gBattleAnimBackgroundTable[bg];
    u32 size = GetDecompressedDataSize(asset->tilemap);
    EXPECT_LE(size, 2 * BG_SCREEN_SIZE);
    u16 *expected = AllocZeroed(2 * BG_SCREEN_SIZE);
    DecompressDataWithHeaderWram(asset->tilemap, expected);
    u32 imageSize = GetDecompressedDataSize(asset->image);
    EXPECT_LE(imageSize, 26 * BG_SCREEN_SIZE - 2 * BG_CHAR_SIZE);
    for (u32 i = 0; i < size / sizeof(u16); i++)
        EXPECT_LT(expected[i] & 0x3FF, imageSize / TILE_SIZE_4BPP);
    if (contest)
        RelocateBattleBgPal(GetBattleBgPaletteNum(), expected, 0x100, TRUE);
    LoadMoveBg(bg);
    for (u32 i = 0; i < 16; i++)
        EXPECT_EQ(gPlttBufferUnfaded[BG_PLTT_ID(GetBattleBgPaletteNum()) + i], asset->palette[i]);
    EXPECT_EQ(GetAnimBgAttribute(3, BG_ANIM_SCREEN_SIZE), size > BG_SCREEN_SIZE);
    const volatile u16 *actual = (const void *)BG_SCREEN_ADDR(26);
    for (u32 i = 0; i < BG_SCREEN_SIZE; i++)
        EXPECT_EQ(actual[i], expected[i]);
    const u32 *image = asset->image;
    u8 *pixels = malloc_and_decompress(image, NULL);
    const volatile u8 *loaded = contest ? (const void *)BG_SCREEN_ADDR(4) : (const void *)BG_CHAR_ADDR(2);
    for (u32 i = 0; i < GetDecompressedDataSize(image); i++)
        EXPECT_EQ(loaded[i], pixels[i]);
    Free(pixels);
    Free(expected);
    gContestMoveAnimInCutaway = FALSE;
}
