// GC Extension Mode: state machine and GBA second-screen UI.
//
// The field is suspended (faded out, overworld graphics released) while the
// mode runs in its own main callback. Every frame it drains the JoyBus queue
// once, updates the state machine and handles input, so connection, cancel,
// timeouts and UI keep working while the GameCube transfers data. Nothing in
// this mode writes party, bag, flags, vars or save data.
#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "gc_companion.h"
#include "gc_companion_link.h"
#include "gc_companion_pokemon.h"
#include "gc_companion_protocol.h"

#define BYE_WAIT_FRAMES 90
#define RESULT_FRAMES 60
#define DRAIN_FRAMES 15

enum
{
    WIN_TITLE,
    WIN_BODY,
    WIN_HELP,
    WIN_LABEL, // off-screen; used to render text labels for the GameCube
    WIN_COUNT,
};

enum GcError
{
    GC_ERROR_NONE,
    GC_ERROR_OUT_OF_MEMORY,
    GC_ERROR_HOST_ENDED,
    GC_ERROR_LINK_LOST,
};

struct GcCompanion
{
    u8 state;
    u8 error;
    u8 screen;
    u8 cursor;
    u8 confirmed;
    u8 phase;
    u8 phaseDone;
    u8 phaseTotal;
    u16 inputSeq;
    u16 timer;
    u8 returnStep;
    bool8 dirty;
    bool8 reconnecting;
    bool8 linkOpen;
    u32 exitMatch;
};

static EWRAM_DATA struct GcCompanion *sGc = NULL;

static void Task_GcCompanion_WaitFadeOut(u8 taskId);
static void CB2_GcCompanion_Init(void);
static void CB2_GcCompanion_Main(void);
static void GetUi(PartyUi *ui);
static void SetPhase(u8 phase, u8 done, u8 total);

static const struct GcPokemonUi sPokemonUi = {.getUi = GetUi, .setPhase = SetPhase};

static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sWindowTemplates[] =
{
    [WIN_TITLE] = {.bg = 0, .tilemapLeft = 1, .tilemapTop = 0,  .width = 28, .height = 2,  .paletteNum = 15, .baseBlock = 1},
    [WIN_BODY]  = {.bg = 0, .tilemapLeft = 1, .tilemapTop = 3,  .width = 28, .height = 12, .paletteNum = 15, .baseBlock = 57},
    [WIN_HELP]  = {.bg = 0, .tilemapLeft = 1, .tilemapTop = 17, .width = 28, .height = 2,  .paletteNum = 15, .baseBlock = 393},
    [WIN_LABEL] = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 22, .width = 16, .height = 2,  .paletteNum = 15, .baseBlock = 449},
    DUMMY_WIN_TEMPLATE,
};

static const u16 sTextPalette[16] = {RGB(2, 3, 7), RGB(31, 31, 31), RGB(9, 10, 13), RGB(31, 26, 8), RGB(31, 12, 10)};
static const u8 sColorNormal[3] = {0, 1, 2};
static const u8 sColorHighlight[3] = {0, 3, 2};
static const u8 sColorError[3] = {0, 4, 2};

static const u8 sText_Title[] = _("{ENG}GC EXTENSION MODE");
static const u8 sText_TitleConnected[] = _("{ENG}GC CONNECTED");
static const u8 sText_Waiting[] = _("{ENG}Waiting for GameCube...");
static const u8 sText_WaitingHint[] = _("{ENG}JoyBus, GC port 2 (Dolphin)");
static const u8 sText_Reconnecting[] = _("{ENG}Link lost. Waiting for GC...");
static const u8 sText_Handshake[] = _("{ENG}Handshake...");
static const u8 sText_SendingParty[] = _("{ENG}Sending party ");
static const u8 sText_SendingAssets[] = _("{ENG}Sending sprites ");
static const u8 sText_MenuParty[] = _("{ENG}PARTY");
static const u8 sText_Closing[] = _("{ENG}Closing session...");
static const u8 sText_ErrorMemory[] = _("{ENG}Not enough memory.");
static const u8 sText_ErrorHost[] = _("{ENG}The GameCube ended the session.");
static const u8 sText_ErrorLink[] = _("{ENG}Link lost.");
static const u8 sText_StateOk[] = _("{ENG}Party/bag/flags/vars unchanged.");
static const u8 sText_StateChanged[] = _("{ENG}WARNING: game state changed!");
static const u8 sText_Returning[] = _("{ENG}Returning to the field...");
static const u8 sText_HelpCancel[] = _("{ENG}B: Cancel");
static const u8 sText_HelpMenu[] = _("{ENG}A: Select   B: Exit");
static const u8 sText_HelpParty[] = _("{ENG}UP/DOWN: Move  A: Select  B: Back");
static const u8 sText_HelpError[] = _("{ENG}A: Reconnect   B: Exit");
static const u8 sText_Cursor[] = _("{ENG}>");
static const u8 sText_Selected[] = _("{ENG}+");
static const u8 sText_Slash[] = _("{ENG}/");
static const u8 sText_Lv[] = _("{ENG}Lv.");
static const u8 sText_Jpn[] = _("{JPN}");
static const u8 sText_Eng[] = _("{ENG}");
static const u8 sText_Space[] = _("{ENG} ");

void GcCompanion_Start(void)
{
    LockPlayerFieldControls();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_GcCompanion_WaitFadeOut, 10);
}

static void Task_GcCompanion_WaitFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        DestroyTask(taskId);
        gMain.state = 0;
        SetMainCallback2(CB2_GcCompanion_Init);
    }
}

static void VBlankCB_GcCompanion(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void SetState(u8 state)
{
    sGc->state = state;
    sGc->timer = 0;
    sGc->dirty = TRUE;
}

static void CB2_GcCompanion_Init(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, 0);
        SetGpuReg(REG_OFFSET_WINOUT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        ResetPaletteFade();
        FreeAllSpritePalettes();
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
        InitWindows(sWindowTemplates);
        DeactivateAllTextPrinters();
        LoadPalette(sTextPalette, BG_PLTT_ID(0), sizeof(sTextPalette));
        LoadPalette(sTextPalette, BG_PLTT_ID(15), sizeof(sTextPalette));
        gMain.state++;
        break;
    case 2:
        sGc = AllocZeroedUnchecked(sizeof(*sGc));
        if (sGc == NULL)
        {
            FreeAllWindowBuffers();
            SetMainCallback2(CB2_ReturnToFieldContinueScript);
            return;
        }
        sGc->confirmed = PKMN_NO_SLOT;
        if (!GcPokemon_Init(WIN_LABEL, &sPokemonUi))
        {
            sGc->error = GC_ERROR_OUT_OF_MEMORY;
            SetState(GC_STATE_ERROR);
        }
        else
        {
            GcProtocol_Init(GcPokemon_GetClientConfig());
            sGc->linkOpen = GcLink_Open();
            if (sGc->linkOpen)
            {
                SetState(GC_STATE_CONNECTING);
            }
            else
            {
                sGc->error = GC_ERROR_OUT_OF_MEMORY;
                SetState(GC_STATE_ERROR);
            }
        }
        gMain.state++;
        break;
    case 3:
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_GcCompanion);
        SetMainCallback2(CB2_GcCompanion_Main);
        break;
    }
}

static void GetUi(PartyUi *ui)
{
    ui->count = gPartiesCount[B_TRAINER_PLAYER];
    ui->cursor = sGc->cursor;
    ui->confirmed = sGc->confirmed;
    ui->screen = sGc->screen;
    ui->flags = sGc->state == GC_STATE_DISCONNECTING ? UI_EXIT_REQUESTED : 0;
    ui->input_seq = sGc->inputSeq;
}

static void SetPhase(u8 phase, u8 done, u8 total)
{
    sGc->phaseDone = done;
    sGc->phaseTotal = total;
    if (phase != sGc->phase)
        sGc->phase = phase;
    if (sGc->state == GC_STATE_DISCONNECTING || sGc->state == GC_STATE_RETURN_FIELD)
        return;
    switch (phase)
    {
    case PHASE_PARTY:
        SetState(GC_STATE_SEND_PARTY);
        break;
    case PHASE_ASSETS:
        SetState(GC_STATE_SEND_ASSETS);
        break;
    case PHASE_READY:
        if (sGc->state != GC_STATE_ACTIVE)
            SetState(GC_STATE_ACTIVE);
        sGc->reconnecting = FALSE;
        break;
    }
    sGc->dirty = TRUE;
}

// ---------------------------------------------------------------- drawing

static void PrintLine(u8 windowId, const u8 *str, u8 x, u8 line, const u8 *color)
{
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, x, line * 16, color, TEXT_SKIP_DRAW, str);
}

static void AppendNumber(u8 *dest, s32 value)
{
    u8 digits[8];
    ConvertIntToDecimalStringN(digits, value, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(dest, sText_Eng);
    StringAppend(dest, digits);
}

static void DrawProgress(const u8 *label)
{
    u8 text[64];
    StringCopy(text, label);
    AppendNumber(text, sGc->phaseDone);
    StringAppend(text, sText_Slash);
    AppendNumber(text, sGc->phaseTotal);
    PrintLine(WIN_BODY, text, 0, 0, sColorNormal);
}

static void DrawPartyList(void)
{
    u8 text[64];
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    u32 slot;

    for (slot = 0; slot < gPartiesCount[B_TRAINER_PLAYER] && slot < PARTY_SIZE; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        const u8 *color = slot == sGc->cursor ? sColorHighlight : sColorNormal;
        if (slot == sGc->cursor)
            PrintLine(WIN_BODY, sText_Cursor, 0, slot, color);
        if (slot == sGc->confirmed)
            PrintLine(WIN_BODY, sText_Selected, 8, slot, color);
        GetMonData(mon, MON_DATA_NICKNAME, name);
        StringCopy(text, sText_Jpn);
        StringAppend(text, name);
        PrintLine(WIN_BODY, text, 18, slot, color);
        StringCopy(text, sText_Lv);
        AppendNumber(text, GetMonData(mon, MON_DATA_LEVEL));
        PrintLine(WIN_BODY, text, 120, slot, color);
        StringCopy(text, sText_Eng);
        AppendNumber(text, GetMonData(mon, MON_DATA_HP));
        StringAppend(text, sText_Slash);
        AppendNumber(text, GetMonData(mon, MON_DATA_MAX_HP));
        PrintLine(WIN_BODY, text, 168, slot, color);
    }
}

static void Redraw(void)
{
    const u8 *title = sText_Title, *help = NULL;

    FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_BODY, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));

    switch (sGc->state)
    {
    case GC_STATE_CONNECTING:
        PrintLine(WIN_BODY, sGc->reconnecting ? sText_Reconnecting : sText_Waiting, 0, 0, sColorNormal);
        PrintLine(WIN_BODY, sText_WaitingHint, 0, 1, sColorNormal);
        help = sText_HelpCancel;
        break;
    case GC_STATE_HANDSHAKE:
        title = sText_TitleConnected;
        PrintLine(WIN_BODY, sText_Handshake, 0, 0, sColorNormal);
        help = sText_HelpCancel;
        break;
    case GC_STATE_SEND_PARTY:
        title = sText_TitleConnected;
        DrawProgress(sText_SendingParty);
        help = sText_HelpCancel;
        break;
    case GC_STATE_SEND_ASSETS:
        title = sText_TitleConnected;
        DrawProgress(sText_SendingAssets);
        help = sText_HelpCancel;
        break;
    case GC_STATE_ACTIVE:
        title = sText_TitleConnected;
        if (sGc->screen == SCREEN_MENU)
        {
            PrintLine(WIN_BODY, sText_Cursor, 0, 0, sColorHighlight);
            PrintLine(WIN_BODY, sText_MenuParty, 12, 0, sColorHighlight);
            help = sText_HelpMenu;
        }
        else
        {
            DrawPartyList();
            help = sText_HelpParty;
        }
        break;
    case GC_STATE_DISCONNECTING:
        title = sText_TitleConnected;
        PrintLine(WIN_BODY, sText_Closing, 0, 0, sColorNormal);
        break;
    case GC_STATE_ERROR:
        PrintLine(WIN_BODY, sGc->error == GC_ERROR_OUT_OF_MEMORY ? sText_ErrorMemory
                          : sGc->error == GC_ERROR_HOST_ENDED ? sText_ErrorHost : sText_ErrorLink,
                  0, 0, sColorError);
        help = sGc->error == GC_ERROR_OUT_OF_MEMORY ? sText_HelpCancel : sText_HelpError;
        break;
    case GC_STATE_RETURN_FIELD:
        if (sGc->returnStep > 0)
            PrintLine(WIN_BODY, sGc->exitMatch == STATE_DIGEST_ALL_MATCH ? sText_StateOk : sText_StateChanged,
                      0, 0, sGc->exitMatch == STATE_DIGEST_ALL_MATCH ? sColorNormal : sColorError);
        PrintLine(WIN_BODY, sText_Returning, 0, 1, sColorNormal);
        break;
    }
    PrintLine(WIN_TITLE, title, 0, 0, sColorNormal);
    if (help != NULL)
        PrintLine(WIN_HELP, help, 0, 0, sColorNormal);
    PutWindowTilemap(WIN_TITLE);
    PutWindowTilemap(WIN_BODY);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_TITLE, COPYWIN_FULL);
    CopyWindowToVram(WIN_BODY, COPYWIN_FULL);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
    sGc->dirty = FALSE;
}

// ---------------------------------------------------------------- logic

static bool8 IsSessionState(u8 state)
{
    return state >= GC_STATE_HANDSHAKE && state <= GC_STATE_ACTIVE;
}

static void HandleLinkEvents(u32 link, u32 proto)
{
    if (link & (GC_LINK_EVENT_RESET | GC_LINK_EVENT_TIMEOUT))
    {
        GcProtocol_ResetSession();
        if (IsSessionState(sGc->state))
        {
            sGc->reconnecting = TRUE;
            SetState(GC_STATE_CONNECTING);
        }
        else if (sGc->state == GC_STATE_DISCONNECTING)
        {
            SetState(GC_STATE_RETURN_FIELD); // host is gone; nothing left to close
        }
    }
    if ((link & GC_LINK_EVENT_CONNECTED) && sGc->state == GC_STATE_CONNECTING)
        SetState(GC_STATE_HANDSHAKE);
    if (proto & GC_PROTO_EVENT_BYE)
    {
        if (sGc->state == GC_STATE_DISCONNECTING)
        {
            SetState(GC_STATE_RETURN_FIELD);
        }
        else if (sGc->state != GC_STATE_RETURN_FIELD)
        {
            sGc->error = GC_ERROR_HOST_ENDED;
            SetState(GC_STATE_ERROR);
        }
    }
}

static void MoveCursor(s8 delta)
{
    u8 count = gPartiesCount[B_TRAINER_PLAYER];
    if (count == 0)
        return;
    sGc->cursor = (sGc->cursor + count + delta) % count;
    sGc->inputSeq++;
    sGc->dirty = TRUE;
    PlaySE(SE_SELECT);
}

static void HandleInput(void)
{
    switch (sGc->state)
    {
    case GC_STATE_CONNECTING:
    case GC_STATE_HANDSHAKE:
    case GC_STATE_SEND_PARTY:
    case GC_STATE_SEND_ASSETS:
        if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            SetState(GC_STATE_RETURN_FIELD);
        }
        break;
    case GC_STATE_ACTIVE:
        if (sGc->screen == SCREEN_MENU)
        {
            if (JOY_NEW(A_BUTTON))
            {
                PlaySE(SE_SELECT);
                sGc->screen = SCREEN_PARTY;
                sGc->inputSeq++;
                sGc->dirty = TRUE;
            }
            else if (JOY_NEW(B_BUTTON))
            {
                PlaySE(SE_SELECT);
                SetState(GC_STATE_DISCONNECTING);
            }
        }
        else if (JOY_REPEAT(DPAD_UP))
        {
            MoveCursor(-1);
        }
        else if (JOY_REPEAT(DPAD_DOWN))
        {
            MoveCursor(1);
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            sGc->confirmed = sGc->cursor;
            sGc->inputSeq++;
            sGc->dirty = TRUE;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sGc->screen = SCREEN_MENU;
            sGc->inputSeq++;
            sGc->dirty = TRUE;
        }
        break;
    case GC_STATE_DISCONNECTING:
        if (++sGc->timer >= BYE_WAIT_FRAMES || JOY_NEW(B_BUTTON))
            SetState(GC_STATE_RETURN_FIELD);
        break;
    case GC_STATE_ERROR:
        if (JOY_NEW(A_BUTTON) && sGc->linkOpen)
        {
            PlaySE(SE_SELECT);
            sGc->reconnecting = TRUE;
            SetState(GcLink_IsConnected() ? GC_STATE_HANDSHAKE : GC_STATE_CONNECTING);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            SetState(GC_STATE_RETURN_FIELD);
        }
        break;
    }
}

static void FreeMode(void)
{
    GcPokemon_Free();
    FreeAllWindowBuffers();
    TRY_FREE_AND_SET_NULL(sGc);
}

// Closes the link, verifies that the game state is unchanged and fades back
// to the field. The script that started the mode continues afterwards.
static void UpdateReturnToField(void)
{
    struct GcStateDigest digest;

    switch (sGc->returnStep)
    {
    case 0:
        // Let the GameCube read the last reply (e.g. the BYE ACK) before
        // JoyBus is switched off; give up after DRAIN_FRAMES.
        // The last word sits in JOY_TRANS once the queue is empty: wait 3+ frames.
        if (sGc->linkOpen && (GcLink_HasPendingOutput() || sGc->timer < 3) && ++sGc->timer < DRAIN_FRAMES)
            break;
        if (sGc->linkOpen)
            GcLink_Close();
        sGc->linkOpen = FALSE;
        GcPokemon_ComputeDigest(&digest);
        sGc->exitMatch = GcPokemon_CompareWithEntry(&digest);
        sGc->returnStep++;
        sGc->timer = 0;
        sGc->dirty = TRUE;
        break;
    case 1:
        if (++sGc->timer >= RESULT_FRAMES)
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sGc->returnStep++;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            FreeMode();
            SetMainCallback2(CB2_ReturnToFieldContinueScript);
        }
        break;
    }
}

static void CB2_GcCompanion_Main(void)
{
    u32 linkEvents = 0, protoEvents;

    if (sGc->linkOpen)
        linkEvents = GcLink_Poll(GcProtocol_HandleFrame);
    protoEvents = GcProtocol_TakeEvents();
    HandleLinkEvents(linkEvents, protoEvents);
    if (!gPaletteFade.active)
        HandleInput();
    if (sGc->state == GC_STATE_RETURN_FIELD)
        UpdateReturnToField();
    if (sGc != NULL && sGc->dirty)
        Redraw();

    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}
