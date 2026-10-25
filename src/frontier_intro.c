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
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "data/frontier_intro.h"

enum
{
    INTRO_GENDER_TEXT,
    INTRO_GENDER_INPUT,
    INTRO_NAME_TEXT,
    INTRO_NAME_FADE,
    INTRO_CONFIRM_TEXT,
    INTRO_CONFIRM_INPUT,
    INTRO_SCENE_FADE_OUT,
    INTRO_SCENE_FADE_IN,
    INTRO_SPEECH,
    INTRO_DEPART,
};

#define tState data[0]
#define tSpeech data[1]
#define tArceusSprite data[2]
#define tPlayerSprite data[3]

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
    sFrontierIntroSend,
    sFrontierIntroDepart,
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
    gTasks[taskId].tState = afterNaming ? INTRO_CONFIRM_TEXT : INTRO_GENDER_TEXT;
    gTasks[taskId].tArceusSprite = SPRITE_NONE;
    gTasks[taskId].tPlayerSprite = SPRITE_NONE;
    PrintIntroText(afterNaming ? sFrontierIntroConfirm : sFrontierIntroGender, FALSE);
    ShowBg(0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
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

static void CreateIntroScene(u8 taskId)
{
    u8 spriteId;
    u16 playerGfx = gSaveBlock2Ptr->playerGender == MALE ? PLAYER_AVATAR_GFX_MALE_NORMAL : PLAYER_AVATAR_GFX_FEMALE_NORMAL;

    // These are ordinary overworld graphics, not battle pictures or object events.
    spriteId = CreateObjectGraphicsSprite(OBJ_EVENT_GFX_SPECIES(ARCEUS), SpriteCallbackDummy, 120, 42, 1);
    if (spriteId < MAX_SPRITES)
    {
        StartSpriteAnim(&gSprites[spriteId], ANIM_STD_FACE_SOUTH);
        gTasks[taskId].tArceusSprite = spriteId;
    }
    spriteId = CreateObjectGraphicsSprite(playerGfx, SpriteCallbackDummy, 120, 88, 0);
    if (spriteId < MAX_SPRITES)
    {
        StartSpriteAnim(&gSprites[spriteId], ANIM_STD_FACE_NORTH);
        gTasks[taskId].tPlayerSprite = spriteId;
    }
    FillWindowPixelBuffer(0, PIXEL_FILL(1));
    ClearWindowTilemap(0);
    CopyWindowToVram(0, COPYWIN_FULL);
    PlayBGM(MUS_ROUTE122);
}

static void Task_FrontierIntro(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s8 input;

    if (gPaletteFade.active)
        return;

    switch (tState)
    {
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
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_WHITE);
            tState = INTRO_SCENE_FADE_OUT;
        }
        else if (input == 1 || input == MENU_B_PRESSED)
        {
            PlaySE(SE_SELECT);
            PrintIntroText(sFrontierIntroGender, FALSE);
            tState = INTRO_GENDER_TEXT;
        }
        break;
    case INTRO_SCENE_FADE_OUT:
        CreateIntroScene(taskId);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_WHITE);
        tState = INTRO_SCENE_FADE_IN;
        break;
    case INTRO_SCENE_FADE_IN:
        PrintIntroText(sSpeech[0], TRUE);
        tSpeech = 0;
        tState = INTRO_SPEECH;
        break;
    case INTRO_SPEECH:
        if (!RunTextPrintersAndIsPrinter0Active() && JOY_NEW(A_BUTTON | B_BUTTON))
        {
            if (++tSpeech < ARRAY_COUNT(sSpeech))
                PrintIntroText(sSpeech[tSpeech], TRUE);
            else
            {
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_WHITE);
                FadeOutBGM(4);
                tState = INTRO_DEPART;
            }
        }
        break;
    case INTRO_DEPART:
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
