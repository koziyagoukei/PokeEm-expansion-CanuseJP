#include "global.h"
#include "battle_anim.h"
#include "shadow_effect_gfx.h"
#include "sprite.h"
#include "constants/rgb.h"

#define SHADOW_PARTICLE_PALETTE_TAG 0xE724

// 8x8, 4bpp tiles: each hexadecimal digit is one pixel, index zero is transparent.
static const u32 sShadowTiles[][8] = {
    [SHADOW_PARTICLE_SHARD] = { 0x00010000, 0x00121000, 0x01232100, 0x12343210, 0x12343210, 0x01232100, 0x00121000, 0x00010000 },
    [SHADOW_PARTICLE_SPARK] = { 0x00011000, 0x01022010, 0x00233200, 0x12344321, 0x12344321, 0x00233200, 0x01022010, 0x00011000 },
    [SHADOW_PARTICLE_EMBER] = { 0x00010000, 0x00121000, 0x01231000, 0x01232100, 0x12343210, 0x12344310, 0x01233210, 0x00111100 },
    [SHADOW_PARTICLE_ARROW] = { 0x00011000, 0x00022000, 0x00032000, 0x10032001, 0x12032021, 0x01243210, 0x00133100, 0x00011000 },
};

#define SHADOW_FRAME(shape) { .data = (const u8 *)sShadowTiles[shape], .size = sizeof(sShadowTiles[shape]) }
static const struct SpriteFrameImage sShadowImages[] = {
    SHADOW_FRAME(SHADOW_PARTICLE_SHARD),
    SHADOW_FRAME(SHADOW_PARTICLE_SPARK),
    SHADOW_FRAME(SHADOW_PARTICLE_EMBER),
    SHADOW_FRAME(SHADOW_PARTICLE_ARROW),
};
#undef SHADOW_FRAME

static const union AnimCmd sAnimShard[] = { ANIMCMD_FRAME(SHADOW_PARTICLE_SHARD, 1), ANIMCMD_END };
static const union AnimCmd sAnimSpark[] = { ANIMCMD_FRAME(SHADOW_PARTICLE_SPARK, 1), ANIMCMD_END };
static const union AnimCmd sAnimEmber[] = { ANIMCMD_FRAME(SHADOW_PARTICLE_EMBER, 1), ANIMCMD_END };
static const union AnimCmd sAnimArrow[] = { ANIMCMD_FRAME(SHADOW_PARTICLE_ARROW, 1), ANIMCMD_END };
static const union AnimCmd *const sShadowAnims[] = { sAnimShard, sAnimSpark, sAnimEmber, sAnimArrow };

static const struct SpriteTemplate sShadowParticleTemplate = {
    .tileTag = TAG_NONE,
    .paletteTag = SHADOW_PARTICLE_PALETTE_TAG,
    .oam = &gOamData_AffineOff_ObjNormal_8x8,
    .anims = sShadowAnims,
    .images = sShadowImages,
};

bool32 LoadShadowParticlePalette(u16 accent)
{
    u16 colors[16] = { 0, RGB(3, 1, 6), RGB(11, 4, 18), accent, RGB(29, 24, 31) };
    struct SpritePalette palette = { colors, SHADOW_PARTICLE_PALETTE_TAG };
    return LoadSpritePalette(&palette) != 0xFF;
}

void FreeShadowParticlePalette(void)
{
    FreeSpritePaletteByTag(SHADOW_PARTICLE_PALETTE_TAG);
}

u32 CreateShadowParticle(enum ShadowParticleShape shape, s16 x, s16 y)
{
    u32 spriteId = CreateSpriteUnchecked(&sShadowParticleTemplate, x, y, 0);
    if (spriteId != MAX_SPRITES)
    {
        gSprites[spriteId].invisible = TRUE;
        StartSpriteAnim(&gSprites[spriteId], shape);
    }
    return spriteId;
}
