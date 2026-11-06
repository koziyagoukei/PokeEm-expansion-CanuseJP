#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "move.h"
#include "shadow_effect_gfx.h"
#include "sprite.h"
#include "task.h"
#include "trig.h"
#include "constants/rgb.h"

#define SHADOW_PARTICLE_COUNT 12

enum ShadowMotion { RUSH, BURST, WAVE, SPIRAL, FALL, RING, MIST, SINK, SHATTER, DRAW, WEATHER };

struct ShadowAnimStyle
{
    u8 motion;
    u8 shape;
    u8 spread;
    u8 duration;
    u16 accent;
};

#define SHADOW_STYLE(move, motion, shape, spread, duration, color) \
    [(move) - FIRST_SHADOW_MOVE] = { motion, SHADOW_PARTICLE_##shape, spread, duration, color }
static const struct ShadowAnimStyle sShadowMoveStyles[] = {
    SHADOW_STYLE(MOVE_SHADOW_BLITZ, RUSH,    SHARD, 14, 26, RGB(23, 11, 29)),
    SHADOW_STYLE(MOVE_SHADOW_RUSH,  RUSH,    EMBER, 25, 33, RGB(26,  7, 24)),
    SHADOW_STYLE(MOVE_SHADOW_BREAK, BURST,   SHARD, 39, 40, RGB(24,  5, 28)),
    SHADOW_STYLE(MOVE_SHADOW_END,   DRAW,    SPARK, 44, 44, RGB(30,  3, 15)),
    SHADOW_STYLE(MOVE_SHADOW_WAVE,  WAVE,    SHARD, 24, 31, RGB(18, 10, 30)),
    SHADOW_STYLE(MOVE_SHADOW_RAVE,  WAVE,    SPARK, 38, 39, RGB(27,  7, 21)),
    SHADOW_STYLE(MOVE_SHADOW_STORM, SPIRAL,  EMBER, 36, 42, RGB(18, 14, 29)),
    SHADOW_STYLE(MOVE_SHADOW_FIRE,  SPIRAL,  EMBER, 22, 36, RGB(30,  9, 12)),
    SHADOW_STYLE(MOVE_SHADOW_BOLT,  FALL,    ARROW, 27, 29, RGB(29, 21,  6)),
    SHADOW_STYLE(MOVE_SHADOW_CHILL, FALL,    SHARD, 34, 38, RGB( 9, 24, 31)),
    SHADOW_STYLE(MOVE_SHADOW_BLAST, RUSH,    SPARK, 38, 32, RGB(19, 17, 31)),
    SHADOW_STYLE(MOVE_SHADOW_SKY,   WEATHER, EMBER, 44, 46, RGB(19,  9, 24)),
    SHADOW_STYLE(MOVE_SHADOW_HOLD,  RING,    SHARD, 24, 37, RGB(18,  8, 27)),
    SHADOW_STYLE(MOVE_SHADOW_MIST,  MIST,    EMBER, 42, 40, RGB(18, 13, 26)),
    SHADOW_STYLE(MOVE_SHADOW_PANIC, RING,    SPARK, 34, 36, RGB(26,  9, 28)),
    SHADOW_STYLE(MOVE_SHADOW_DOWN,  SINK,    ARROW, 24, 30, RGB(16,  8, 22)),
    SHADOW_STYLE(MOVE_SHADOW_SHED,  SHATTER, SHARD, 43, 38, RGB(21, 14, 30)),
    SHADOW_STYLE(MOVE_SHADOW_HALF,  DRAW,    EMBER, 36, 42, RGB(22,  5, 26)),
};
#undef SHADOW_STYLE

static const struct ShadowAnimStyle sDynaphobiaStyle = { BURST, SHADOW_PARTICLE_SPARK, 57, 56, RGB(30, 8, 22) };
static const struct ShadowAnimStyle sDarknessStyle = { WEATHER, SHADOW_PARTICLE_ARROW, 32, 34, RGB(19, 10, 28) };

static const struct ShadowAnimStyle *GetShadowAnimStyle(enum Move move)
{
    if (move >= FIRST_SHADOW_MOVE && move <= LAST_SHADOW_MOVE)
        return &sShadowMoveStyles[move - FIRST_SHADOW_MOVE];
    if (move == MOVE_DYNAPHOBIA)
        return &sDynaphobiaStyle;
    return &sDarknessStyle;
}

#define tFrame data[0]
#define tMove data[1]
#define tSprite(i) data[2 + (i)]
STATIC_ASSERT(SHADOW_PARTICLE_COUNT + 2 <= ARRAY_COUNT(((struct Task *)0)->data), ShadowParticlesFitTask);

static void UpdateShadowParticles(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    enum Move move = task->tMove;
    const struct ShadowAnimStyle *style = GetShadowAnimStyle(move);
    s32 frame = task->tFrame++;
    s32 ax = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_X_2);
    s32 ay = GetBattlerSpriteCoord(gBattleAnimAttacker, BATTLER_COORD_Y_PIC_OFFSET);
    bool32 spread = !IsContest() && (GetMoveTarget(move) == TARGET_BOTH || move == MOVE_SHADOW_HALF);

    if (frame == style->duration)
    {
        for (u32 i = 0; i < SHADOW_PARTICLE_COUNT; i++)
        {
            if (task->tSprite(i) != MAX_SPRITES)
                DestroySprite(&gSprites[task->tSprite(i)]);
        }
        FreeShadowParticlePalette();
        DestroyAnimVisualTask(taskId);
        return;
    }

    for (u32 i = 0; i < SHADOW_PARTICLE_COUNT; i++)
    {
        if (task->tSprite(i) == MAX_SPRITES)
            continue;

        struct Sprite *sprite = &gSprites[task->tSprite(i)];
        s32 delay = (i % 6) * 2;
        s32 elapsed = frame - delay;
        sprite->invisible = elapsed < 0;
        if (sprite->invisible)
            continue;

        s32 progress = elapsed * 256 / (style->duration - delay);
        s32 radius = style->spread;
        s32 angle = i * 256 / SHADOW_PARTICLE_COUNT;
        enum BattlerId target = gBattleAnimTarget;
        if (move == MOVE_SHADOW_HALF && (i & 1))
            target = gBattleAnimAttacker;
        if (spread && (i & 2) && IsBattlerSpriteVisible(GetPartnerBattler(target)))
            target = GetPartnerBattler(target);
        s32 tx = GetBattlerSpriteCoord(target, BATTLER_COORD_X_2);
        s32 ty = GetBattlerSpriteCoord(target, BATTLER_COORD_Y_PIC_OFFSET);
        s32 direction = ax > tx ? -1 : 1;
        s32 x = tx, y = ty;

        switch (style->motion)
        {
        case RUSH:
        case WAVE:
            if (progress < 164)
            {
                x = ax + (tx - ax) * progress / 164;
                y = ay + (ty - ay) * progress / 164;
                y += Sin((i * 21 + (style->motion == WAVE ? progress * 2 : 0)) & 255, radius / 2);
            }
            else
            {
                radius = radius * (progress - 164) / 92;
                x += Cos(angle, radius);
                y += Sin(angle, radius);
            }
            break;
        case BURST:
        case SHATTER:
            if (style->motion == SHATTER)
            {
                x = 120;
                y = 62;
                radius *= 2;
            }
            radius = radius * progress / 256;
            x += Cos(angle, radius);
            y += Sin(angle, radius / 2);
            break;
        case SPIRAL:
            angle = (angle + progress * 2) & 255;
            x += Cos(angle, radius * (256 - progress / 2) / 256);
            y += Sin(angle, radius / 3) + radius / 2 - progress * radius / 256;
            break;
        case FALL:
            x += ((s32)(i % 6) - 3) * radius / 4;
            y += (progress - 198) * radius / 92;
            break;
        case RING:
            angle = (angle + progress) & 255;
            radius = radius * (320 - progress) / 256;
            x += Cos(angle, radius);
            y += Sin(angle, radius / 2);
            break;
        case MIST:
            x += direction * (progress - 128) * radius / 128;
            y += Sin((angle + progress) & 255, radius / 3);
            break;
        case SINK:
            x += Cos(angle, radius);
            y += (progress - 128) * radius / 128;
            break;
        case DRAW:
            if (move == MOVE_SHADOW_END && progress > 144)
            {
                x = ax;
                y = ay;
            }
            radius = radius * (256 - progress) / 256;
            x += Cos(angle, radius);
            y += Sin(angle, radius);
            break;
        case WEATHER:
            x = 10 + i * 19 - progress / 24;
            y = -12 + progress / 2 - (s32)(i % 3) * 12;
            break;
        }

        sprite->x = x;
        sprite->y = y;
    }
}

void AnimTask_ShadowParticles(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    enum Move move = gAnimMoveIndex;
    const struct ShadowAnimStyle *style = GetShadowAnimStyle(move);
    if (!LoadShadowParticlePalette(style->accent))
    {
        DestroyAnimVisualTask(taskId);
        return;
    }

    task->tFrame = 0;
    task->tMove = move;
    for (u32 i = 0; i < SHADOW_PARTICLE_COUNT; i++)
    {
        task->tSprite(i) = CreateShadowParticle(style->shape, 0, 0);
    }
    task->func = UpdateShadowParticles;
}
