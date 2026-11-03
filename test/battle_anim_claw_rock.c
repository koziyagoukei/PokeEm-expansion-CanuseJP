#include "global.h"
#include "decompress.h"
#include "graphics.h"
#include "malloc.h"
#include "menu.h"
#include "test/test.h"

TEST("Claw and rock assets: replacement sheets preserve their frame storage contracts")
{
    const u32 *tiles = gBattleAnimSpriteGfx_ClawSlash;
    u32 size = 0xA00;
    u32 frames = 5;
    PARAMETRIZE { tiles = gBattleAnimSpriteGfx_ClawSlash; size = 0xA00; frames = 5; }
    PARAMETRIZE { tiles = gBattleAnimSpriteGfx_NewRocks; size = 0xC00; frames = 6; }
    PARAMETRIZE { tiles = gBattleAnimSpriteGfx_RocksSmall; size = 0x140; frames = 0; }
    EXPECT_EQ(GetDecompressedDataSize(tiles), size);
    u8 *pixels = malloc_and_decompress(tiles, NULL);
    for (u32 frame = 0; frame < frames; frame++)
    {
        u32 opaque = 0;
        for (u32 i = 0; i < 0x200; i++)
        {
            u8 pair = pixels[frame * 0x200 + i];
            opaque += (pair & 15) != 0;
            opaque += (pair >> 4) != 0;
        }
        EXPECT_GT(opaque, 0);
        EXPECT_LT(opaque, 32 * 32);
    }
    if (frames == 0)
    {
        // Small-rock animations retain their original empty leading/trailing cells.
        for (u32 i = 0; i < 0x80; i++) EXPECT_EQ(pixels[i], 0);
        for (u32 i = 0x100; i < 0x140; i++) EXPECT_EQ(pixels[i], 0);
    }
    Free(pixels);
}
