#include "global.h"
#include "frontier_opening.h"
#include "gpu_regs.h"
#include "m4a.h"
#include "main.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "task.h"
#include "title_screen.h"
#include "trig.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/species.h"

// The movie is cut to MUS_FRONTIER_OPENING (sound/songs/midi/mus_frontier_opening.mid):
// at 100 BPM a beat is exactly 36 frames and a bar 144.
#define BEAT 36
#define BAR (4 * BEAT)

#define HANDS_TOUCH   (2 * BAR - 2 * BEAT) // the glockenspiel triplet
#define HANDS_STING   (2 * BAR - BEAT)
#define NETWORK_ROLL  (2 * BAR - BEAT)
#define ATTACK_BREATH (BAR - BEAT / 4)     // the rolls stop a sixteenth early
#define ATTACK_HIT    BAR

#define OPENING_BITMAP_BYTES (DISPLAY_WIDTH * DISPLAY_HEIGHT)
// Mode 4's second page. The hands share the forest's clearing, so they wait
// there and the cut between them needs no blank frame.
#define DISPCNT_PAGE_1 0x0010
#define PAGE_1_VRAM (VRAM + 0xA000)

#define VIRUS_X 120
#define VIRUS_Y 52
#define VIRUS_HIGH_Y 46
#define TOUCH_X (142 << 8)
#define TOUCH_Y (95 << 8)

#define COLOR_VIRUS      RGB(12, 2, 14)
#define COLOR_SHADOW     RGB(6, 1, 9)
#define COLOR_SILHOUETTE RGB(1, 0, 3)
#define COLOR_INFECTED   RGB(18, 4, 24)
#define COLOR_POWER      RGB(31, 31, 24)

enum
{
    ACTOR_VIRUS,
    ACTOR_DARKRAI,
    ACTOR_ZYGARDE,
    ACTOR_RAYQUAZA,
    ACTOR_YOUNGSTER,
    ACTOR_LASS,
    ACTOR_COOL_M,
    ACTOR_COOL_F,
    ACTOR_COUNT,
};

enum
{
    TAG_MOTE = FRONTIER_OPENING_ACTOR_TAG + ACTOR_COUNT,
    TAG_GLOW,
    TAG_BEAD,
};
STATIC_ASSERT(TAG_BEAD < FRONTIER_OPENING_ACTOR_TAG + FRONTIER_OPENING_TAG_COUNT, OpeningTagCount);
STATIC_ASSERT(FRONTIER_OPENING_GUARDIAN_TAG == FRONTIER_OPENING_ACTOR_TAG + ACTOR_DARKRAI, OpeningGuardianTag);
STATIC_ASSERT(FRONTIER_OPENING_TRAINER_TAG == FRONTIER_OPENING_ACTOR_TAG + ACTOR_YOUNGSTER, OpeningTrainerTag);

#define FIREFLY_COUNT    12
#define SPORE_COUNT      8
#define MOTE_COUNT       18
#define TRAINER_COUNT    4
#define BEADS_PER_CHAIN  8
#define GUARDIAN_COUNT   3

static const u32 sForestPixels[] = INCBIN_U32("graphics/custom/frontier_opening/forest.pixels.bin");
static const u16 sForestPalette[] = INCBIN_U16("graphics/custom/frontier_opening/forest.palette.bin");
static const u32 sHandsPixels[] = INCBIN_U32("graphics/custom/frontier_opening/meeting_hands.pixels.bin");
static const u16 sHandsPalette[] = INCBIN_U16("graphics/custom/frontier_opening/meeting_hands.palette.bin");
static const u32 sNetworkPixels[] = INCBIN_U32("graphics/custom/frontier_opening/network.pixels.bin");
static const u16 sNetworkPalette[] = INCBIN_U16("graphics/custom/frontier_opening/network.palette.bin");
static const u32 sRidgePixels[] = INCBIN_U32("graphics/custom/frontier_opening/ridge.pixels.bin");
static const u16 sRidgePalette[] = INCBIN_U16("graphics/custom/frontier_opening/ridge.palette.bin");
STATIC_ASSERT(sizeof(sForestPixels) == OPENING_BITMAP_BYTES, OpeningForestBitmapSize);
STATIC_ASSERT(sizeof(sHandsPixels) == OPENING_BITMAP_BYTES, OpeningHandsBitmapSize);
STATIC_ASSERT(sizeof(sNetworkPixels) == OPENING_BITMAP_BYTES, OpeningNetworkBitmapSize);
STATIC_ASSERT(sizeof(sRidgePixels) == OPENING_BITMAP_BYTES, OpeningRidgeBitmapSize);

static const u32 sVirusTiles[] = INCGFX_U32("graphics/custom/pokevirus/front.png", ".4bpp");
static const u16 sVirusPalette[] = INCGFX_U16("graphics/custom/pokevirus/palette.pal", ".gbapal");
static const u32 sDarkraiTiles[] = INCGFX_U32("graphics/pokemon/darkrai/back.png", ".4bpp");
static const u16 sDarkraiPalette[] = INCGFX_U16("graphics/pokemon/darkrai/normal.pal", ".gbapal");
static const u32 sZygardeTiles[] = INCGFX_U32("graphics/pokemon/zygarde/back.png", ".4bpp");
static const u16 sZygardePalette[] = INCGFX_U16("graphics/pokemon/zygarde/normal.pal", ".gbapal");
static const u32 sRayquazaTiles[] = INCGFX_U32("graphics/pokemon/rayquaza/back.png", ".4bpp");
static const u16 sRayquazaPalette[] = INCGFX_U16("graphics/pokemon/rayquaza/normal.pal", ".gbapal");
static const u32 sYoungsterTiles[] = INCGFX_U32("graphics/trainers/front_pics/youngster.png", ".4bpp");
static const u16 sYoungsterPalette[] = INCGFX_U16("graphics/trainers/front_pics/youngster.png", ".gbapal");
static const u32 sLassTiles[] = INCGFX_U32("graphics/trainers/front_pics/lass.png", ".4bpp");
static const u16 sLassPalette[] = INCGFX_U16("graphics/trainers/front_pics/lass.png", ".gbapal");
static const u32 sCoolMTiles[] = INCGFX_U32("graphics/trainers/front_pics/cooltrainer_m.png", ".4bpp");
static const u16 sCoolMPalette[] = INCGFX_U16("graphics/trainers/front_pics/cooltrainer_m.png", ".gbapal");
static const u32 sCoolFTiles[] = INCGFX_U32("graphics/trainers/front_pics/cooltrainer_f.png", ".4bpp");
static const u16 sCoolFPalette[] = INCGFX_U16("graphics/trainers/front_pics/cooltrainer_f.png", ".gbapal");

static const u32 sMoteTiles[] = INCGFX_U32("graphics/custom/frontier_opening/firefly.png", ".4bpp");
static const u16 sFireflyPalette[] = INCGFX_U16("graphics/custom/frontier_opening/firefly.png", ".gbapal");
static const u32 sGlowTiles[] = INCGFX_U32("graphics/custom/frontier_opening/glow.png", ".4bpp");
static const u16 sGlowPalette[] = INCGFX_U16("graphics/custom/frontier_opening/glow.png", ".gbapal");
static const u32 sBeadTiles[] = INCGFX_U32("graphics/custom/frontier_opening/bead.png", ".4bpp");
static const u16 sBeadPalette[] = INCGFX_U16("graphics/custom/frontier_opening/bead.png", ".gbapal");
// firefly.png in the Pokevirus' spore colors, and in the guardians' gathering power.
static const u16 sSporePalette[16] = {0, RGB(30, 22, 31), RGB(22, 10, 26), RGB(13, 4, 18)};
static const u16 sPowerPalette[16] = {0, RGB(31, 31, 27), RGB(31, 27, 12), RGB(26, 17, 6)};

static const struct OamData sDoubleOam =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE,
    .shape = SPRITE_SHAPE(64x64), .size = SPRITE_SIZE(64x64),
};
// Shrunk trainers keep normal affine bounds, so the distant ones never wrap above the screen.
static const struct OamData sShrunkOam =
{
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = SPRITE_SHAPE(64x64), .size = SPRITE_SIZE(64x64),
};
static const struct OamData sPlainOam = {.shape = SPRITE_SHAPE(64x64), .size = SPRITE_SIZE(64x64)};
static const struct OamData sMoteOam = {.shape = SPRITE_SHAPE(8x8), .size = SPRITE_SIZE(8x8)};
static const struct OamData sGlowOam =
{
    .affineMode = ST_OAM_AFFINE_DOUBLE, .objMode = ST_OAM_OBJ_BLEND,
    .shape = SPRITE_SHAPE(32x32), .size = SPRITE_SIZE(32x32),
};

static const struct
{
    const u32 *tiles;
    const u16 *palette;
} sActors[ACTOR_COUNT] =
{
    [ACTOR_VIRUS] = {sVirusTiles, sVirusPalette},
    [ACTOR_DARKRAI] = {sDarkraiTiles, sDarkraiPalette},
    [ACTOR_ZYGARDE] = {sZygardeTiles, sZygardePalette},
    [ACTOR_RAYQUAZA] = {sRayquazaTiles, sRayquazaPalette},
    [ACTOR_YOUNGSTER] = {sYoungsterTiles, sYoungsterPalette},
    [ACTOR_LASS] = {sLassTiles, sLassPalette},
    [ACTOR_COOL_M] = {sCoolMTiles, sCoolMPalette},
    [ACTOR_COOL_F] = {sCoolFTiles, sCoolFPalette},
};

#define TEMPLATE(tag, oamData) \
    {.tileTag = tag, .paletteTag = tag, .oam = oamData, .anims = gDummySpriteAnimTable, \
     .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCallbackDummy}
static const struct SpriteTemplate sActorTemplates[ACTOR_COUNT] =
{
    [ACTOR_VIRUS] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_VIRUS, &sDoubleOam),
    [ACTOR_DARKRAI] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_DARKRAI, &sPlainOam),
    [ACTOR_ZYGARDE] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_ZYGARDE, &sPlainOam),
    [ACTOR_RAYQUAZA] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_RAYQUAZA, &sPlainOam),
    [ACTOR_YOUNGSTER] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_YOUNGSTER, &sShrunkOam),
    [ACTOR_LASS] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_LASS, &sShrunkOam),
    [ACTOR_COOL_M] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_COOL_M, &sShrunkOam),
    [ACTOR_COOL_F] = TEMPLATE(FRONTIER_OPENING_ACTOR_TAG + ACTOR_COOL_F, &sShrunkOam),
};
static const struct SpriteTemplate sMoteTemplate = TEMPLATE(TAG_MOTE, &sMoteOam);
static const struct SpriteTemplate sGlowTemplate = TEMPLATE(TAG_GLOW, &sGlowOam);
static const struct SpriteTemplate sBeadTemplate = TEMPLATE(TAG_BEAD, &sMoteOam);
#undef TEMPLATE

// Each trainer is chained to the Pokevirus on one trumpet stab.
static const struct
{
    u8 actor;
    s16 x, y;
    u16 scale;
    u16 stab;
} sTrainers[TRAINER_COUNT] =
{
    {ACTOR_LASS, 80, 100, 0xA0, BEAT},
    {ACTOR_COOL_M, 160, 100, 0xA0, 2 * BEAT},
    {ACTOR_YOUNGSTER, 34, 114, 0xB4, 3 * BEAT},
    {ACTOR_COOL_F, 206, 114, 0xB4, 4 * BEAT},
};

// One guardian rises into the lineup on each downbeat of the fanfare.
static const struct
{
    u8 actor;
    s16 x, y;
    u16 arrival;
    s8 pan;
    u16 species;
} sGuardians[GUARDIAN_COUNT] =
{
    {ACTOR_DARKRAI, 44, 128, 0, -48, SPECIES_DARKRAI},
    {ACTOR_ZYGARDE, 196, 131, BAR, 48, SPECIES_ZYGARDE_50},
    {ACTOR_RAYQUAZA, 120, 128, 2 * BAR, 0, SPECIES_RAYQUAZA},
};

static const u16 sPhaseStarts[FRONTIER_OPENING_DONE + 1] =
{
    [FRONTIER_OPENING_FOREST] = 0,
    [FRONTIER_OPENING_HANDS] = 2 * BAR,
    [FRONTIER_OPENING_NETWORK] = 4 * BAR,
    [FRONTIER_OPENING_GUARDIANS] = 6 * BAR,
    [FRONTIER_OPENING_ATTACK] = 9 * BAR,
    [FRONTIER_OPENING_WHITE] = 10 * BAR + BEAT,
    [FRONTIER_OPENING_DONE] = 11 * BAR,
};

#define tPhase data[0]
#define tFrame data[1] // within the phase
#define tClock data[2] // since the forest began

static EWRAM_DATA u8 sTaskId = 0;
static EWRAM_DATA u8 sActorIds[ACTOR_COUNT] = {0};
static EWRAM_DATA u8 sMoteIds[MOTE_COUNT] = {0};
static EWRAM_DATA u8 sBeadIds[TRAINER_COUNT * BEADS_PER_CHAIN] = {0};
static EWRAM_DATA u8 sGlowIds[GUARDIAN_COUNT] = {0};
static EWRAM_DATA struct {s32 zoom, x, y;} sCamera = {0};
static EWRAM_DATA bool8 sShowPage1 = FALSE;

static void Task_FrontierOpening(u8 taskId);

static s16 Wave(s32 angle, s16 amplitude)
{
    return Sin(angle & 0xFF, amplitude);
}

static s16 WaveCos(s32 angle, s16 amplitude)
{
    return Cos(angle & 0xFF, amplitude);
}

// 0 before `from`, 256 after `from + length`.
static s32 Progress(s32 frame, s32 from, s32 length)
{
    if (frame <= from)
        return 0;
    if (frame >= from + length)
        return 256;
    return (frame - from) * 256 / length;
}

static s32 Ease(s32 t)
{
    return t * t * (768 - 2 * t) / 65536;
}

static s32 Lerp(s32 from, s32 to, s32 t)
{
    return from + (to - from) * t / 256;
}

// `level` at `at`, falling to 0 over `length` frames.
static s32 Decay(s32 frame, s32 at, s32 level, s32 length)
{
    if (frame < at || frame >= at + length)
        return 0;
    return level * (at + length - frame) / length;
}

static void VBlankCB_Opening(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    // The hands' page must show in the same VBlank as their palette.
    if (sShowPage1)
    {
        SetGpuReg(REG_OFFSET_DISPCNT, GetGpuReg(REG_OFFSET_DISPCNT) | DISPCNT_PAGE_1);
        sShowPage1 = FALSE;
    }
}

// Show texture point (x, y), in 1/256 pixels, at the screen's center, magnified
// 256 / zoom times. The point is clamped so the bitmap's edges never show.
static void Camera(s32 zoom, s32 x, s32 y)
{
    s32 halfWidth = DISPLAY_WIDTH / 2 * zoom, halfHeight = DISPLAY_HEIGHT / 2 * zoom;
    x = max(halfWidth, min(DISPLAY_WIDTH * 256 - halfWidth, x));
    y = max(halfHeight, min(DISPLAY_HEIGHT * 256 - halfHeight, y));
    sCamera.zoom = zoom;
    sCamera.x = x;
    sCamera.y = y;
    struct BgAffineSrcData src = {.texX = x, .texY = y, .scrX = DISPLAY_WIDTH / 2, .scrY = DISPLAY_HEIGHT / 2,
                                  .sx = zoom, .sy = zoom, .alpha = 0};
    struct BgAffineDstData dest;
    BgAffineSet(&src, &dest, 1);
    SetGpuReg(REG_OFFSET_BG2PA, dest.pa);
    SetGpuReg(REG_OFFSET_BG2PB, dest.pb);
    SetGpuReg(REG_OFFSET_BG2PC, dest.pc);
    SetGpuReg(REG_OFFSET_BG2PD, dest.pd);
    SetGpuReg(REG_OFFSET_BG2X_L, dest.dx);
    SetGpuReg(REG_OFFSET_BG2X_H, dest.dx >> 16);
    SetGpuReg(REG_OFFSET_BG2Y_L, dest.dy);
    SetGpuReg(REG_OFFSET_BG2Y_H, dest.dy >> 16);
}

static s16 ScreenX(s32 x)
{
    return DISPLAY_WIDTH / 2 + (x - sCamera.x) / sCamera.zoom;
}

static s16 ScreenY(s32 y)
{
    return DISPLAY_HEIGHT / 2 + (y - sCamera.y) / sCamera.zoom;
}

// Fades use the blend unit instead of the palette fader, so a key can skip the
// movie at any moment. Light sprites are semi-transparent and add to the scene.
static void Blend(u16 targets, u16 effect, s32 level)
{
    SetGpuReg(REG_OFFSET_BLDCNT, targets | effect | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(10, 16));
    SetGpuReg(REG_OFFSET_BLDY, max(0, min(16, level)));
}

static void Tone(u16 effect, s32 level)
{
    Blend(BLDCNT_TGT1_BG2 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD, effect, level);
}

// Darkens the scenery only, leaving the sprites lit.
static void Shade(s32 level)
{
    Blend(BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BD, BLDCNT_EFFECT_DARKEN, level);
}

static struct Sprite *Actor(u32 actor)
{
    return sActorIds[actor] < MAX_SPRITES ? &gSprites[sActorIds[actor]] : NULL;
}

static struct Sprite *Fx(const u8 *ids, u32 i)
{
    return ids[i] < MAX_SPRITES ? &gSprites[ids[i]] : NULL;
}

// 0x100 is the natural size.
static void Scale(struct Sprite *sprite, s32 scale)
{
    s16 matrix = 0x10000 / max(scale, 0x10);
    SetOamMatrix(sprite->oam.matrixNum, matrix, 0, 0, matrix);
}

static void Tint(struct Sprite *sprite, s32 coeff, u16 color)
{
    BlendPalettes(1u << (16 + sprite->oam.paletteNum), max(0, min(16, coeff)), color);
}

// Twinkle between the lit and dimmed frames of firefly.png.
static void Twinkle(struct Sprite *sprite, bool32 dim)
{
    sprite->oam.tileNum = sprite->sheetTileStart + (dim ? 1 : 0);
}

static void ClearSceneSprites(void)
{
    for (u32 i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse)
        {
            FreeSpriteOamMatrix(&gSprites[i]);
            DestroySprite(&gSprites[i]);
        }
    }
    for (u32 i = 0; i < FRONTIER_OPENING_TAG_COUNT; i++)
    {
        FreeSpriteTilesByTag(FRONTIER_OPENING_ACTOR_TAG + i);
        FreeSpritePaletteByTag(FRONTIER_OPENING_ACTOR_TAG + i);
    }
    memset(sActorIds, MAX_SPRITES, sizeof(sActorIds));
    memset(sMoteIds, MAX_SPRITES, sizeof(sMoteIds));
    memset(sBeadIds, MAX_SPRITES, sizeof(sBeadIds));
    memset(sGlowIds, MAX_SPRITES, sizeof(sGlowIds));
    ClearSpriteCopyRequests();
}

static bool32 LoadSheet(const u32 *tiles, u16 size, const u16 *palette, u16 tag)
{
    const struct SpriteSheet sheet = {tiles, size, tag};
    const struct SpritePalette pal = {palette, tag};
    if (!CanAllocSpriteTiles(size / TILE_SIZE_4BPP))
        return FALSE;
    LoadSpriteSheet(&sheet);
    return GetSpriteTileStartByTag(tag) != TAG_NONE && LoadSpritePalette(&pal) != 0xFF;
}

static u8 CreateHiddenSprite(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    u8 id = CreateSprite(template, x, y, subpriority);
    if (id == MAX_SPRITES)
        return id;
    gSprites[id].invisible = TRUE;
    if (gSprites[id].oam.affineMode != ST_OAM_AFFINE_OFF)
    {
        // The movie sets every matrix itself.
        gSprites[id].affineAnimPaused = TRUE;
        gSprites[id].affineAnimBeginning = FALSE;
        Scale(&gSprites[id], 0x100);
    }
    return id;
}

static bool32 CreateActor(u32 actor, s16 x, s16 y, u8 subpriority)
{
    if (!LoadSheet(sActors[actor].tiles, 0x800, sActors[actor].palette, FRONTIER_OPENING_ACTOR_TAG + actor))
        return FALSE;
    sActorIds[actor] = CreateHiddenSprite(&sActorTemplates[actor], x, y, subpriority);
    return sActorIds[actor] != MAX_SPRITES;
}

// Effects are optional: a sprite that cannot be made is simply missing.
static void CreateEffects(const struct SpriteTemplate *template, const u32 *tiles, u16 size, const u16 *palette,
                          u8 *ids, u32 count, u8 subpriority)
{
    if (!LoadSheet(tiles, size, palette, template->tileTag))
        return;
    for (u32 i = 0; i < count; i++)
        ids[i] = CreateHiddenSprite(template, 0, DISPLAY_HEIGHT + 16, subpriority);
}

static bool32 CreateNetwork(void)
{
    if (!CreateActor(ACTOR_VIRUS, VIRUS_X, VIRUS_Y, 2))
        return FALSE;
    Scale(Actor(ACTOR_VIRUS), 0x140);
    for (u32 i = 0; i < TRAINER_COUNT; i++)
    {
        if (!CreateActor(sTrainers[i].actor, sTrainers[i].x, sTrainers[i].y, 3))
            return FALSE;
        Scale(Actor(sTrainers[i].actor), sTrainers[i].scale);
        Actor(sTrainers[i].actor)->invisible = FALSE;
    }
    Actor(ACTOR_VIRUS)->invisible = FALSE;
    CreateEffects(&sBeadTemplate, sBeadTiles, sizeof(sBeadTiles), sBeadPalette, sBeadIds, ARRAY_COUNT(sBeadIds), 5);
    CreateEffects(&sMoteTemplate, sMoteTiles, sizeof(sMoteTiles), sSporePalette, sMoteIds, SPORE_COUNT, 1);
    return TRUE;
}

static bool32 CreateGuardians(void)
{
    if (!CreateActor(ACTOR_VIRUS, VIRUS_X, VIRUS_HIGH_Y, 3))
        return FALSE;
    Scale(Actor(ACTOR_VIRUS), 0x180);
    Actor(ACTOR_VIRUS)->invisible = FALSE;
    for (u32 i = 0; i < GUARDIAN_COUNT; i++)
        if (!CreateActor(sGuardians[i].actor, sGuardians[i].x, DISPLAY_HEIGHT + 32, 2))
            return FALSE;
    CreateEffects(&sMoteTemplate, sMoteTiles, sizeof(sMoteTiles), sSporePalette, sMoteIds, MOTE_COUNT, 0);
    CreateEffects(&sGlowTemplate, sGlowTiles, sizeof(sGlowTiles), sGlowPalette, sGlowIds, GUARDIAN_COUNT, 0);
    return TRUE;
}

static bool32 LoadScene(enum FrontierOpeningPhase phase)
{
    SetVBlankCallback(NULL);
    sShowPage1 = FALSE;
    // Every scene opens from full white or black. Forced blank shows white, so
    // the network, which opens from black, loads under the darkened screen.
    bool32 fromBlack = phase == FRONTIER_OPENING_NETWORK;
    Tone(fromBlack ? BLDCNT_EFFECT_DARKEN : BLDCNT_EFFECT_LIGHTEN, 16);
    if (!fromBlack)
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_FORCED_BLANK);
    ClearSceneSprites();
    ResetSpriteData();
    FreeAllSpritePalettes();
    // Bitmap modes reserve OBJ tiles 0..511 for their framebuffer pages.
    gReservedSpriteTileCount = 512;
    AllocSpriteTiles(0);
    ResetPaletteFade();
    bool32 success = TRUE;
    switch (phase)
    {
    case FRONTIER_OPENING_FOREST:
        CpuCopy32(sForestPixels, (void *)VRAM, OPENING_BITMAP_BYTES);
        CpuCopy32(sHandsPixels, (void *)PAGE_1_VRAM, OPENING_BITMAP_BYTES);
        LoadPalette(sForestPalette, BG_PLTT_ID(0), PLTT_SIZE_8BPP);
        CreateEffects(&sMoteTemplate, sMoteTiles, sizeof(sMoteTiles), sFireflyPalette, sMoteIds, FIREFLY_COUNT, 0);
        break;
    case FRONTIER_OPENING_NETWORK:
        CpuCopy32(sNetworkPixels, (void *)VRAM, OPENING_BITMAP_BYTES);
        LoadPalette(sNetworkPalette, BG_PLTT_ID(0), PLTT_SIZE_8BPP);
        success = CreateNetwork();
        break;
    default:
        CpuCopy32(sRidgePixels, (void *)VRAM, OPENING_BITMAP_BYTES);
        LoadPalette(sRidgePalette, BG_PLTT_ID(0), PLTT_SIZE_8BPP);
        success = CreateGuardians();
        break;
    }
    Camera(0x100, DISPLAY_WIDTH / 2 << 8, DISPLAY_HEIGHT / 2 << 8);
    BuildOamBuffer();
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_4 | DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetVBlankCallback(VBlankCB_Opening);
    return success;
}

static void ShowHands(void)
{
    sShowPage1 = TRUE;
    LoadPalette(sHandsPalette, BG_PLTT_ID(0), PLTT_SIZE_8BPP);
    CreateEffects(&sGlowTemplate, sGlowTiles, sizeof(sGlowTiles), sGlowPalette, sGlowIds, 1, 1);
}

static void UpdateFireflies(s32 clock)
{
    for (u32 i = 0; i < FIREFLY_COUNT; i++)
    {
        struct Sprite *sprite = Fx(sMoteIds, i);
        if (sprite == NULL)
            continue;
        s32 t = (clock + i * 47) % 128;
        sprite->x = 12 + (i * 83) % 216 + Wave(t * 3 + i * 40, 6);
        sprite->y = 78 + (i * 37) % 64 - t / 4;
        sprite->invisible = t >= 112;
        // Each light swells in, blinks now and then, and dims before it goes out.
        Twinkle(sprite, t < 12 || t >= 96 || (t / 6 + i) % 5 == 0);
    }
}

static void UpdateForest(s32 f)
{
    s32 t = Ease(Progress(f, 0, 2 * BAR));
    Camera(Lerp(0xF4, 0xE8, t), Lerp(120 << 8, 124 << 8, t), Lerp(80 << 8, 82 << 8, t));
    Tone(BLDCNT_EFFECT_LIGHTEN, 16 - f * 16 / BEAT);
    UpdateFireflies(f);
}

// Darkrai's hand meets Pecharunt's: a light on the glockenspiel triplet, then
// the sting snuffs it out and the shadow falls.
static void UpdateHands(s32 f)
{
    s32 t = Ease(Progress(f, 0, HANDS_STING));
    s32 shake = Wave(f * 96, Decay(f, HANDS_STING, 3, 14));
    Camera(Lerp(0xE8, 0xC0, t), Lerp(124 << 8, 141 << 8, t) + shake * 256, Lerp(82 << 8, 93 << 8, t));
    if (f < HANDS_STING)
    {
        s32 light = max(6 - f / 2, Decay(f, HANDS_TOUCH, 3, 10));
        light = max(light, max(Decay(f, HANDS_TOUCH + 12, 3, 10), Decay(f, HANDS_TOUCH + 24, 4, 12)));
        Tone(BLDCNT_EFFECT_LIGHTEN, light);
    }
    else
    {
        Tone(BLDCNT_EFFECT_DARKEN, Progress(f, HANDS_STING + 4, 28) * 16 / 256);
        BlendPalettes(PALETTES_BG, 4 + Progress(f, HANDS_STING, 24) * 6 / 256, COLOR_SHADOW);
    }
    UpdateFireflies(f + 2 * BAR);

    struct Sprite *glow = Fx(sGlowIds, 0);
    if (glow != NULL)
    {
        s32 scale;
        if (f < HANDS_STING)
        {
            scale = Lerp(0x40, 0x120, Progress(f, HANDS_TOUCH - 4, 40));
            scale += Decay(f, HANDS_TOUCH, 0x30, 10) + Decay(f, HANDS_TOUCH + 12, 0x40, 10)
                   + Decay(f, HANDS_TOUCH + 24, 0x60, 12);
        }
        else
        {
            scale = Lerp(0x120, 0, Progress(f, HANDS_STING, 10));
        }
        glow->invisible = f < HANDS_TOUCH - 4 || scale < 0x18;
        glow->x = ScreenX(TOUCH_X);
        glow->y = ScreenY(TOUCH_Y);
        Scale(glow, scale);
    }
}

static s32 Heartbeat(s32 f)
{
    s32 pulse = 0;
    for (s32 beat = 0; beat < 2 * BAR; beat += 2 * BEAT)
        pulse = max(pulse, max(Decay(f, beat, 6, 16), Decay(f, beat + BEAT / 2, 3, 12)));
    return pulse;
}

// The Pokevirus and the infected look out at the player; a chain lashes out
// to each trainer on a trumpet stab.
static void UpdateNetwork(s32 f)
{
    s32 pulse = Heartbeat(f), jolt = 0;
    for (u32 i = 0; i < TRAINER_COUNT; i++)
        jolt += Wave(f * 80, Decay(f, sTrainers[i].stab, 2, 8));
    s32 shake = jolt + Wave(f * 72, f >= NETWORK_ROLL ? 2 : 0);
    s32 t = Ease(Progress(f, 0, 2 * BAR));
    Camera(Lerp(0xF8, 0xE8, t) - pulse, (120 << 8) + shake * 256, Lerp(80 << 8, 76 << 8, t));
    if (f < BEAT / 2)
        Tone(BLDCNT_EFFECT_DARKEN, 16 - f * 16 / (BEAT / 2));
    else
        Tone(BLDCNT_EFFECT_DARKEN, Progress(f, 2 * BAR - BEAT / 2, BEAT / 2) * 16 / 256);
    BlendPalettes(PALETTES_BG, pulse, COLOR_VIRUS);

    struct Sprite *virus = Actor(ACTOR_VIRUS);
    if (virus != NULL)
    {
        virus->y2 = Wave(f * 2, 2);
        Scale(virus, 0x140 + pulse * 6);
    }

    for (u32 i = 0; i < TRAINER_COUNT; i++)
    {
        struct Sprite *trainer = Actor(sTrainers[i].actor);
        if (trainer == NULL)
            continue;
        s32 since = f - sTrainers[i].stab;
        trainer->x2 = Wave(f * 64 + i * 50, f >= NETWORK_ROLL ? 1 : 0);
        if (since < 0)
            Tint(trainer, 14, COLOR_SILHOUETTE);
        else if (since < 4)
            Tint(trainer, 16, RGB_WHITE);
        else
            Tint(trainer, max(7, 13 - (since - 4) / 2) + pulse / 2, COLOR_INFECTED);

        s32 grow = Progress(f, sTrainers[i].stab - 8, 8);
        s32 toX = sTrainers[i].x, toY = sTrainers[i].y - 10;
        for (u32 j = 0; j < BEADS_PER_CHAIN; j++)
        {
            struct Sprite *bead = Fx(sBeadIds, i * BEADS_PER_CHAIN + j);
            if (bead == NULL)
                continue;
            s32 along = (j + 1) * 256 / (BEADS_PER_CHAIN + 1);
            bead->invisible = grow < along;
            bead->x = VIRUS_X + (toX - VIRUS_X) * along / 256;
            bead->y = VIRUS_Y + 8 + (toY - VIRUS_Y - 8) * along / 256 + Wave(along / 2, 10)
                    + Wave(f * 4 + j * 24 + i * 64, 1);
        }
    }

    for (u32 i = 0; i < SPORE_COUNT; i++)
    {
        struct Sprite *spore = Fx(sMoteIds, i);
        if (spore == NULL)
            continue;
        s32 age = (f + i * 29) % 96, angle = i * 32 + 16, radius = 12 + age * 3 / 4;
        spore->x = VIRUS_X + WaveCos(angle, radius);
        spore->y = VIRUS_Y + Wave(angle, radius) / 2 + age / 3;
        spore->invisible = age >= 88;
        Twinkle(spore, age < 8 || age >= 72 || (age / 5 + i) % 4 == 0);
    }
}

static void PlaceGuardian(u32 i, s32 f, s32 lift, s32 power)
{
    struct Sprite *sprite = Actor(sGuardians[i].actor);
    if (sprite == NULL)
        return;
    s32 rise = 256 - Progress(f, sGuardians[i].arrival, 14);
    sprite->invisible = f < sGuardians[i].arrival;
    sprite->x = sGuardians[i].x;
    sprite->y = sGuardians[i].y + 72 * rise * rise / 65536;
    sprite->y2 = Wave(f * 3 + i * 85, 1) - lift;
    if (power > 0)
        Tint(sprite, power, COLOR_POWER);
    else
        Tint(sprite, Decay(f, sGuardians[i].arrival, 12, 16), RGB_WHITE);
}

static void PlaceVirusAbove(s32 f, s32 recoil, s32 tremble)
{
    struct Sprite *virus = Actor(ACTOR_VIRUS);
    if (virus == NULL)
        return;
    virus->y2 = Wave(f * 2, 2);
    virus->x2 = Wave(f * 100, recoil / 16 + tremble);
    Scale(virus, 0x180 - recoil);
    Tint(virus, recoil / 6, RGB_WHITE);
}

static void UpdateGuardians(s32 f)
{
    s32 shake = 0, flash = Decay(f, 0, 16, 20), recoil = 0, cleared = 0;
    for (u32 i = 0; i < GUARDIAN_COUNT; i++)
    {
        s32 arrival = sGuardians[i].arrival;
        shake += Wave(f * 88, Decay(f, arrival, 3, 14));
        flash = max(flash, Decay(f, arrival, 12, 16));
        recoil += Decay(f, arrival, 0x30, 16);
        cleared += Progress(f, arrival, 24);
        if (f == arrival + 12)
            PlayCry_NormalNoDucking(sGuardians[i].species, sGuardians[i].pan, CRY_VOLUME * 3 / 4, CRY_PRIORITY_NORMAL);
        PlaceGuardian(i, f, 0, 0);
    }
    Camera(0xF0, (120 << 8) + shake * 256, 80 << 8);
    Tone(BLDCNT_EFFECT_LIGHTEN, flash);
    // The Pokevirus' purple leaves the sky a little with each guardian.
    BlendPalettes(PALETTES_BG, 7 - cleared * 2 / 256, COLOR_VIRUS);
    PlaceVirusAbove(f, recoil, 0);

    for (u32 i = 0; i < MOTE_COUNT; i++)
    {
        struct Sprite *spore = Fx(sMoteIds, i);
        if (spore == NULL)
            continue;
        s32 t = (f + i * 31) % 120;
        spore->x = VIRUS_X + WaveCos(i * 32 + 8, 16 + t / 2);
        spore->y = VIRUS_HIGH_Y + 14 + t;
        spore->invisible = i >= SPORE_COUNT || t >= 100;
        Twinkle(spore, t < 8 || t >= 84 || (t / 5 + i) % 4 == 0);
    }
}

// The guardians gather power on the rise, loose it on the breath before the
// hit, and the light swallows the screen.
static void UpdateAttack(s32 f)
{
    s32 gather = Progress(f, 0, ATTACK_BREATH);
    s32 shake = Wave(f * 90, f < ATTACK_BREATH ? f / 48 : 0) + Wave(f * 70, Decay(f, ATTACK_HIT, 5, 30));
    Camera(Lerp(0xF0, 0xE0, Ease(Progress(f, 0, ATTACK_HIT))), (120 << 8) + shake * 256, 80 << 8);
    // The world dims around the lit guardians, then the light takes everything.
    if (f < ATTACK_BREATH)
        Shade(gather * 7 / 256);
    else if (f < ATTACK_HIT)
        Shade(9);
    else
        Tone(BLDCNT_EFFECT_LIGHTEN, 10 + Decay(f, ATTACK_HIT, 4, 4) + Progress(f, ATTACK_HIT, BEAT) * 6 / 256);
    BlendPalettes(PALETTES_BG, 1, COLOR_VIRUS);

    s32 lift = Progress(f, ATTACK_BREATH, 6) * 3 / 256;
    s32 power = f < ATTACK_BREATH ? gather * 5 / 256 : 8;
    for (u32 i = 0; i < GUARDIAN_COUNT; i++)
        PlaceGuardian(i, f + 3 * BAR, lift, power);
    PlaceVirusAbove(f, 0, f < ATTACK_HIT / 2 ? 1 : 2);

    for (u32 i = 0; i < MOTE_COUNT; i++)
    {
        struct Sprite *mote = Fx(sMoteIds, i);
        if (mote == NULL)
            continue;
        u32 guardian = i % GUARDIAN_COUNT, k = i / GUARDIAN_COUNT;
        s32 t = (f * 2 + k * 21) % 64, radius = 52 - t * 3 / 4, angle = k * 43 + f * 4 + guardian * 85;
        mote->x = sGuardians[guardian].x + WaveCos(angle, radius);
        mote->y = sGuardians[guardian].y - 12 + Wave(angle, radius) / 2;
        mote->invisible = f >= ATTACK_BREATH || f < (s32)k * 8;
        Twinkle(mote, t < 6 || (t / 4 + i) % 3 == 0);
    }

    for (u32 i = 0; i < GUARDIAN_COUNT; i++)
    {
        struct Sprite *beam = Fx(sGlowIds, i);
        if (beam == NULL)
            continue;
        s32 fromX = sGuardians[i].x, fromY = sGuardians[i].y - 28;
        if (f < ATTACK_BREATH)
        {
            beam->x = fromX;
            beam->y = fromY;
            Scale(beam, Lerp(0x20, 0xA0, Progress(f, ATTACK_BREATH - 9, 9)));
            beam->invisible = f < ATTACK_BREATH - 9;
        }
        else if (f < ATTACK_HIT)
        {
            s32 p = Progress(f, ATTACK_BREATH, ATTACK_HIT - ATTACK_BREATH);
            beam->x = Lerp(fromX, VIRUS_X, p);
            beam->y = Lerp(fromY, VIRUS_HIGH_Y, p);
            Scale(beam, Lerp(0xA0, 0x100, p));
            beam->invisible = FALSE;
        }
        else
        {
            // One burst stays on the Pokevirus until the screen is nearly white.
            beam->x = VIRUS_X;
            beam->y = VIRUS_HIGH_Y;
            Scale(beam, Lerp(0x100, 0x200, Progress(f, ATTACK_HIT, 24)));
            beam->invisible = i != 0 || f >= ATTACK_HIT + 24;
        }
    }
}

static void UpdateWhite(void)
{
    Tone(BLDCNT_EFFECT_LIGHTEN, 16);
    // White palettes too, so the title's first blank frames stay white.
    BlendPalettes(PALETTES_ALL, 16, RGB_WHITE);
    for (u32 i = 0; i < GUARDIAN_COUNT; i++)
        if (Fx(sGlowIds, i) != NULL)
            Fx(sGlowIds, i)->invisible = TRUE;
}

static void UpdateScene(enum FrontierOpeningPhase phase, s32 f)
{
    switch (phase)
    {
    case FRONTIER_OPENING_FOREST:
        UpdateForest(f);
        break;
    case FRONTIER_OPENING_HANDS:
        UpdateHands(f);
        break;
    case FRONTIER_OPENING_NETWORK:
        UpdateNetwork(f);
        break;
    case FRONTIER_OPENING_GUARDIANS:
        UpdateGuardians(f);
        break;
    case FRONTIER_OPENING_ATTACK:
        UpdateAttack(f);
        break;
    default:
        UpdateWhite();
        break;
    }
}

static void Abort(struct Task *task)
{
    // Allocation failure must still lead to the title, never a stuck intro.
    task->tPhase = FRONTIER_OPENING_WHITE;
    task->tClock = sPhaseStarts[FRONTIER_OPENING_WHITE];
}

// FALSE when the rest of this frame's steps must wait: a scene was loaded or the movie ended.
static bool32 BeginPhase(struct Task *task, enum FrontierOpeningPhase phase)
{
    task->tPhase = phase;
    switch (phase)
    {
    case FRONTIER_OPENING_HANDS:
        ShowHands();
        return TRUE;
    case FRONTIER_OPENING_NETWORK:
    case FRONTIER_OPENING_GUARDIANS:
        if (!LoadScene(phase))
            Abort(task);
        return FALSE;
    case FRONTIER_OPENING_ATTACK:
        if (IndexOfSpritePaletteTag(TAG_MOTE) != 0xFF)
            LoadPalette(sPowerPalette, OBJ_PLTT_ID(IndexOfSpritePaletteTag(TAG_MOTE)), PLTT_SIZE_4BPP);
        return TRUE;
    case FRONTIER_OPENING_WHITE:
        return TRUE;
    default:
        FrontierOpening_Stop();
        SetMainCallback2(CB2_InitTitleScreen);
        return FALSE;
    }
}

static bool32 Step(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    bool32 more = TRUE;
    task->tClock++;
    if (task->tClock >= sPhaseStarts[task->tPhase + 1])
    {
        more = BeginPhase(task, task->tPhase + 1);
        if (!FrontierOpening_IsActive())
            return FALSE;
    }
    task->tFrame = task->tClock - sPhaseStarts[task->tPhase];
    UpdateScene(task->tPhase, task->tFrame);
    return more;
}

// Frames since the opening song started, while it plays.
static u32 MusicClock(void)
{
    if (gMPlayInfo_BGM.songHeader != gSongTable[MUS_FRONTIER_OPENING].header
     || !(gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_TRACK)
     || (gMPlayInfo_BGM.status & MUSICPLAYER_STATUS_PAUSE))
        return 0;
    // 100 BPM at 24 ticks per beat: two ticks every three frames.
    return gMPlayInfo_BGM.clock * 3 / 2;
}

static void Task_FrontierOpening(u8 taskId)
{
    // The song keeps time in the VBlank interrupt; catch up after a slow frame
    // so no cut falls behind its beat.
    u32 music = MusicClock(), clock = gTasks[taskId].tClock, steps = 1;
    if (music > clock + 1)
        steps = min(music - clock, 4);
    while (steps-- != 0 && Step(taskId))
        ;
}

void Task_FrontierOpeningStart(u8 taskId)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
    // A previously reset wave task may have reused this task's slot.
    if (gScanlineEffect.waveTaskId == taskId)
        gScanlineEffect.waveTaskId = TASK_NONE;
    ScanlineEffect_Stop();
    ScanlineEffect_Clear();
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    sTaskId = taskId;
    memset(gTasks[taskId].data, 0, sizeof(gTasks[taskId].data));
    gTasks[taskId].func = Task_FrontierOpening;
    PlayBGM(MUS_FRONTIER_OPENING);
    if (!LoadScene(FRONTIER_OPENING_FOREST))
        Abort(&gTasks[taskId]);
    gTasks[taskId].tFrame = gTasks[taskId].tClock - sPhaseStarts[gTasks[taskId].tPhase];
    UpdateScene(gTasks[taskId].tPhase, gTasks[taskId].tFrame);
}

bool32 FrontierOpening_IsActive(void)
{
    return sTaskId != TASK_NONE && sTaskId < NUM_TASKS && gTasks[sTaskId].isActive
        && gTasks[sTaskId].func == Task_FrontierOpening;
}

enum FrontierOpeningPhase FrontierOpening_GetPhase(void)
{
    return FrontierOpening_IsActive() ? gTasks[sTaskId].tPhase : FRONTIER_OPENING_DONE;
}

u16 FrontierOpening_GetFrame(void)
{
    return FrontierOpening_IsActive() ? gTasks[sTaskId].tFrame : 0;
}

void FrontierOpening_Stop(void)
{
    if (!FrontierOpening_IsActive())
        return;
    ClearSceneSprites();
    DestroyTask(sTaskId);
    sTaskId = TASK_NONE;
    gReservedSpriteTileCount = 0;
}
