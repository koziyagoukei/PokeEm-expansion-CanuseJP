#include "global.h"
#include "bg.h"
#include "event_object_movement.h"
#include "frontier_intro.h"
#include "gpu_regs.h"
#include "main.h"
#include "main_menu.h"
#include "menu.h"
#include "naming_screen.h"
#include "overworld.h"
#include "palette.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trig.h"
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "data/frontier_intro.h"

enum
{
    INTRO_VOID_FADE_IN,
    INTRO_VOID_WAIT,
    INTRO_GENDER_TEXT,
    INTRO_GENDER_INPUT,
    INTRO_NAME_TEXT,
    INTRO_NAME_FADE,
    INTRO_CONFIRM_TEXT,
    INTRO_CONFIRM_INPUT,
    INTRO_ACCEPT_TEXT,
    INTRO_SCENE_SETUP,
    INTRO_LOOK_AROUND,
    INTRO_ARCEUS_DESCEND,
    INTRO_ARCEUS_COLOR,
    INTRO_ARCEUS_CRY,
    INTRO_SPEECH,
    INTRO_QUESTION_TEXT,
    INTRO_QUESTION_INPUT,
    INTRO_FAREWELL,
    INTRO_DEPART_SPIN,
    INTRO_DEPART,
};

#define tState data[0]
#define tSpeech data[1]
#define tArceusSprite data[2]
#define tPlayerSprite data[3]
#define tTimer data[4]
#define tSpinDelay data[5]

#define sFloatTimer data[7]

#define ARCEUS_X 120
#define ARCEUS_Y 36
#define ARCEUS_DESCENT 24
#define ARCEUS_DESCENT_FRAMES (ARCEUS_DESCENT * 2)
#define ARCEUS_PALE_COEFF 10
#define PLAYER_X 120
#define PLAYER_Y 92
#define DEPART_LIFT 6

static const struct BgTemplate sBgTemplate = {
    .bg = 0,
    .charBaseIndex = 2,
    .mapBaseIndex = 30,
};

static const struct WindowTemplate sWindows[] = {
    {.bg = 0, .tilemapLeft = 2, .tilemapTop = 15, .width = 26, .height = 4, .paletteNum = 15, .baseBlock = 1},
    {.bg = 0, .tilemapLeft = 11, .tilemapTop = 6, .width = 8, .height = 4, .paletteNum = 15, .baseBlock = 0x80},
    DUMMY_WIN_TEMPLATE,
};

static const struct MenuAction sGenderChoices[] = {
    {gText_Boy, {NULL}},
    {gText_Girl, {NULL}},
};

static const u8 *const sSpeech[] = {
    sFrontierIntroWelcome,
    sFrontierIntroRequest,
};

static const u8 *const sFarewell[] = {
    sFrontierIntroThanks,
    sFrontierIntroSend,
    sFrontierIntroDepart,
};

// The player looks around the empty space before Arceus answers.
static const struct {
    u8 frame;
    u8 anim;
} sLookAround[] = {
    {40, ANIM_STD_FACE_WEST},
    {70, ANIM_STD_FACE_EAST},
    {100, ANIM_STD_FACE_SOUTH},
    {135, ANIM_STD_FACE_NORTH},
};

static const u8 sSpinAnims[] = {
    ANIM_STD_FACE_SOUTH,
    ANIM_STD_FACE_WEST,
    ANIM_STD_FACE_NORTH,
    ANIM_STD_FACE_EAST,
};

static void Task_FrontierIntro(u8 taskId);
static void CB2_FrontierIntroAfterNaming(void);

static void VBlankCB_FrontierIntro(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_RunFrontierIntro(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void PrintIntroText(const u8 *text, bool32 withFrame)
{
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    if (withFrame)
        DrawDialogFrameWithCustomTileAndPalette(0, FALSE, 0xF3, 15);
    StringExpandPlaceholders(gStringVar4, text);
    PutWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
    AddTextPrinterForMessage(TRUE);
}

static void InitIntroScreen(bool32 afterNaming)
{
    u8 taskId;

    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ScanlineEffect_Stop();
    ResetBgsAndClearDma3BusyFlags(0);
    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    InitBgsFromTemplates(0, &sBgTemplate, 1);
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    ResetTempTileDataBuffers();
    InitWindows(sWindows);
    DeactivateAllTextPrinters();
    LoadMessageBoxGfx(0, 0xF3, BG_PLTT_ID(15));
    LoadUserWindowBorderGfx(0, 0xE0, BG_PLTT_ID(14));
    LoadPalette(sFrontierIntroWhite, 0, sizeof(sFrontierIntroWhite));
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_MOSAIC, 0);
    taskId = CreateTask(Task_FrontierIntro, 0);
    gTasks[taskId].tArceusSprite = SPRITE_NONE;
    gTasks[taskId].tPlayerSprite = SPRITE_NONE;
    if (afterNaming)
    {
        gTasks[taskId].tState = INTRO_CONFIRM_TEXT;
        PrintIntroText(sFrontierIntroConfirm, FALSE);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
    }
    else
    {
        // A slow fade from the menu's black into the empty white space.
        gTasks[taskId].tState = INTRO_VOID_FADE_IN;
        BeginNormalPaletteFade(PALETTES_ALL, 4, 16, 0, RGB_BLACK);
    }
    ShowBg(0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetVBlankCallback(VBlankCB_FrontierIntro);
    SetMainCallback2(CB2_RunFrontierIntro);
}

void CB2_FrontierIntro(void)
{
    InitIntroScreen(FALSE);
}

static void CB2_FrontierIntroAfterNaming(void)
{
    InitIntroScreen(TRUE);
}

static void SpriteCB_ArceusFloat(struct Sprite *sprite)
{
    sprite->y2 = Sin(sprite->sFloatTimer, 3);
    sprite->sFloatTimer = (sprite->sFloatTimer + 2) & 0xFF;
}

static u32 GetSpritePaletteMask(u8 spriteId)
{
    return 1u << (16 + gSprites[spriteId].oam.paletteNum);
}

static void CreateIntroPlayer(u8 taskId)
{
    u8 spriteId;
    u16 playerGfx = gSaveBlock2Ptr->playerGender == MALE ? PLAYER_AVATAR_GFX_MALE_NORMAL : PLAYER_AVATAR_GFX_FEMALE_NORMAL;

    // These are ordinary overworld graphics, not battle pictures or object events.
    spriteId = CreateObjectGraphicsSprite(playerGfx, SpriteCallbackDummy, PLAYER_X, PLAYER_Y, 0);
    if (spriteId < MAX_SPRITES)
    {
        StartSpriteAnim(&gSprites[spriteId], ANIM_STD_FACE_SOUTH);
        gTasks[taskId].tPlayerSprite = spriteId;
    }
}

static void CreateIntroArceus(u8 taskId)
{
    u8 spriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_SPECIES(ARCEUS), SpriteCB_ArceusFloat, ARCEUS_X, ARCEUS_Y - ARCEUS_DESCENT, 1);

    if (spriteId >= MAX_SPRITES)
        return;
    StartSpriteAnim(&gSprites[spriteId], ANIM_STD_FACE_SOUTH);
    // Arceus materializes out of the white as a pale shape, then takes on its colors.
    gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    BlendPalettes(GetSpritePaletteMask(spriteId), ARCEUS_PALE_COEFF, RGB_WHITE);
    gTasks[taskId].tArceusSprite = spriteId;
}

static void Task_FrontierIntro(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s8 input;
    u32 i;

    // The departure keeps spinning while the screen fades to white.
    if (gPaletteFade.active && tState != INTRO_DEPART_SPIN)
        return;

    switch (tState)
    {
    case INTRO_VOID_FADE_IN:
        tTimer = 0;
        tState = INTRO_VOID_WAIT;
        break;
    case INTRO_VOID_WAIT:
        if (++tTimer >= 45)
        {
            FadeInNewBGM(MUS_ROUTE122, 6);
            PrintIntroText(sFrontierIntroGender, FALSE);
            tState = INTRO_GENDER_TEXT;
        }
        break;
    case INTRO_GENDER_TEXT:
        if (!RunTextPrintersAndIsPrinter0Active())
        {
            FillWindowPixelBuffer(1, PIXEL_FILL(1));
            PrintMenuTable(1, ARRAY_COUNT(sGenderChoices), sGenderChoices);
            InitMenuInUpperLeftCornerNormal(1, ARRAY_COUNT(sGenderChoices), gSaveBlock2Ptr->playerGender == FEMALE);
            PutWindowTilemap(1);
            CopyWindowToVram(1, COPYWIN_FULL);
            tState = INTRO_GENDER_INPUT;
        }
        break;
    case INTRO_GENDER_INPUT:
        input = Menu_ProcessInputNoWrap();
        if (input == MALE || input == FEMALE)
        {
            PlaySE(SE_SELECT);
            gSaveBlock2Ptr->playerGender = input;
            ClearWindowTilemap(1);
            CopyWindowToVram(1, COPYWIN_FULL);
            PrintIntroText(sFrontierIntroName, FALSE);
            tState = INTRO_NAME_TEXT;
        }
        break;
    case INTRO_NAME_TEXT:
        if (!RunTextPrintersAndIsPrinter0Active() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = INTRO_NAME_FADE;
        }
        break;
    case INTRO_NAME_FADE:
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        NewGameBirchSpeech_SetDefaultPlayerName(0);
        DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_FrontierIntroAfterNaming);
        break;
    case INTRO_CONFIRM_TEXT:
        if (!RunTextPrintersAndIsPrinter0Active())
        {
            CreateYesNoMenuParameterized(23, 8, 0xE0, 0xC0, 14, 15);
            tState = INTRO_CONFIRM_INPUT;
        }
        break;
    case INTRO_CONFIRM_INPUT:
        input = Menu_ProcessInputNoWrapClearOnChoose();
        if (input == 0)
        {
            PlaySE(SE_SELECT);
            PrintIntroText(sFrontierIntroAccept, FALSE);
            tState = INTRO_ACCEPT_TEXT;
        }
        else if (input == 1 || input == MENU_B_PRESSED)
        {
            PlaySE(SE_SELECT);
            PrintIntroText(sFrontierIntroGenderRetry, FALSE);
            tState = INTRO_GENDER_TEXT;
        }
        break;
    case INTRO_ACCEPT_TEXT:
        if (!RunTextPrintersAndIsPrinter0Active() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 2, 0, 16, RGB_WHITE);
            FadeOutBGM(4);
            tState = INTRO_SCENE_SETUP;
        }
        break;
    case INTRO_SCENE_SETUP:
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        ClearWindowTilemap(0);
        CopyWindowToVram(0, COPYWIN_FULL);
        CreateIntroPlayer(taskId);
        BeginNormalPaletteFade(PALETTES_ALL, 3, 16, 0, RGB_WHITE);
        tTimer = 0;
        tState = INTRO_LOOK_AROUND;
        break;
    case INTRO_LOOK_AROUND:
        tTimer++;
        for (i = 0; i < ARRAY_COUNT(sLookAround); i++)
        {
            if (tTimer == sLookAround[i].frame && tPlayerSprite != SPRITE_NONE)
                StartSpriteAnim(&gSprites[tPlayerSprite], sLookAround[i].anim);
        }
        if (tTimer == 120)
            PlaySE(SE_M_MORNING_SUN);
        if (tTimer >= 150)
        {
            CreateIntroArceus(taskId);
            tTimer = 0;
            tState = INTRO_ARCEUS_DESCEND;
        }
        break;
    case INTRO_ARCEUS_DESCEND:
        tTimer++;
        if (tArceusSprite != SPRITE_NONE)
            gSprites[tArceusSprite].y = ARCEUS_Y - ARCEUS_DESCENT + tTimer / 2;
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(tTimer * 16 / ARCEUS_DESCENT_FRAMES, 16 - tTimer * 16 / ARCEUS_DESCENT_FRAMES));
        if (tTimer >= ARCEUS_DESCENT_FRAMES)
        {
            if (tArceusSprite != SPRITE_NONE)
            {
                gSprites[tArceusSprite].oam.objMode = ST_OAM_OBJ_NORMAL;
                BeginNormalPaletteFade(GetSpritePaletteMask(tArceusSprite), 2, ARCEUS_PALE_COEFF, 0, RGB_WHITE);
            }
            SetGpuReg(REG_OFFSET_BLDCNT, 0);
            SetGpuReg(REG_OFFSET_BLDALPHA, 0);
            tState = INTRO_ARCEUS_COLOR;
        }
        break;
    case INTRO_ARCEUS_COLOR:
        PlayCry_Normal(SPECIES_ARCEUS, 0);
        tState = INTRO_ARCEUS_CRY;
        break;
    case INTRO_ARCEUS_CRY:
        if (IsCryFinished())
        {
            FadeInNewBGM(MUS_SOOTOPOLIS, 4);
            PrintIntroText(sSpeech[0], TRUE);
            tSpeech = 0;
            tState = INTRO_SPEECH;
        }
        break;
    case INTRO_SPEECH:
        if (!RunTextPrintersAndIsPrinter0Active() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            if (++tSpeech < ARRAY_COUNT(sSpeech))
                PrintIntroText(sSpeech[tSpeech], TRUE);
            else
            {
                PrintIntroText(sFrontierIntroQuestion, TRUE);
                tState = INTRO_QUESTION_TEXT;
            }
        }
        break;
    case INTRO_QUESTION_TEXT:
        if (!RunTextPrintersAndIsPrinter0Active())
        {
            CreateYesNoMenuParameterized(23, 8, 0xE0, 0xC0, 14, 15);
            tState = INTRO_QUESTION_INPUT;
        }
        break;
    case INTRO_QUESTION_INPUT:
        input = Menu_ProcessInputNoWrapClearOnChoose();
        if (input == 0)
        {
            PlaySE(SE_SELECT);
            PrintIntroText(sFarewell[0], TRUE);
            tSpeech = 0;
            tState = INTRO_FAREWELL;
        }
        else if (input == 1 || input == MENU_B_PRESSED)
        {
            // Arceus does not take no for an answer.
            PlaySE(SE_SELECT);
            PrintIntroText(sFrontierIntroRefuse, TRUE);
            tState = INTRO_QUESTION_TEXT;
        }
        break;
    case INTRO_FAREWELL:
        if (!RunTextPrintersAndIsPrinter0Active() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            if (++tSpeech < ARRAY_COUNT(sFarewell))
                PrintIntroText(sFarewell[tSpeech], TRUE);
            else
            {
                ClearDialogWindowAndFrame(0, TRUE);
                PlaySE(SE_WARP_OUT);
                FadeOutBGM(4);
                tTimer = 0;
                tSpinDelay = 4;
                tState = INTRO_DEPART_SPIN;
            }
        }
        break;
    case INTRO_DEPART_SPIN:
        // Mirrors the Teleport spin, so the arrival on the field picks it up.
        // Arceus floats right above, so the player only lifts slightly.
        tTimer++;
        if (tPlayerSprite != SPRITE_NONE)
        {
            if (tTimer % tSpinDelay == 0)
                StartSpriteAnim(&gSprites[tPlayerSprite], sSpinAnims[(tTimer / tSpinDelay) % ARRAY_COUNT(sSpinAnims)]);
            if (tTimer == 16)
                tSpinDelay = 2;
            if (tTimer > 16 && tTimer % 2 == 0 && gSprites[tPlayerSprite].y2 > -DEPART_LIFT)
                gSprites[tPlayerSprite].y2--;
        }
        if (tTimer == 24)
            BeginNormalPaletteFade(PALETTES_ALL, 1, 0, 16, RGB_WHITE);
        if (tTimer > 24 && !gPaletteFade.active)
        {
            tTimer = 0;
            tState = INTRO_DEPART;
        }
        break;
    case INTRO_DEPART:
        // A short hold in the white lets the music finish fading.
        if (++tTimer < 30)
            break;
        if (tArceusSprite != SPRITE_NONE)
            DestroySpriteAndFreeResources(&gSprites[tArceusSprite]);
        if (tPlayerSprite != SPRITE_NONE)
            DestroySpriteAndFreeResources(&gSprites[tPlayerSprite]);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_NewGame);
        break;
    }
}
