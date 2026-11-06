#ifndef GUARD_SHADOW_EFFECT_GFX_H
#define GUARD_SHADOW_EFFECT_GFX_H

enum ShadowParticleShape
{
    SHADOW_PARTICLE_SHARD,
    SHADOW_PARTICLE_SPARK,
    SHADOW_PARTICLE_EMBER,
    SHADOW_PARTICLE_ARROW,
};

bool32 LoadShadowParticlePalette(u16 accent);
void FreeShadowParticlePalette(void);
u32 CreateShadowParticle(enum ShadowParticleShape shape, s16 x, s16 y);

#endif
