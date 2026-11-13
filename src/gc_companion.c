// GC Extension mode runner: state machine, GBA screen frame and field return.
//
// GcStartMode(handler) suspends the field (fade out, overworld graphics
// released) and runs its own main callback. Every frame it drains the JoyBus
// queue once, updates the state machine and handles input, so connection,
// cancel, timeouts and UI keep working while the GameCube transfers data.
// What the session is for is decided by the GcModeHandler; the runner itself
// never writes party, bag, flags, vars or save data. Only the handler's
// finish() may apply a verified result, once, after the link is closed.
#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "link.h"
#include "link_rfu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "random.h"
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
#include "gc_extension.h"

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
    GC_ERROR_MODE_REFUSED,
};

struct GcCompanion
{
    const struct GcModeHandler *handler;
    struct GcClientConfig config;
    const u8 *refusal;
    const u8 *finishMessage;
    u8 state;
    u8 error;
    u8 phase;
    u8 phaseDone;
    u8 phaseTotal;
    u16 timer;
    u8 returnStep;
    bool8 dirty;
    bool8 reconnecting;
    bool8 linkOpen;
    bool8 modeStarted;
    bool8 exitRequested;
    bool8 partyChanged;
    u32 exitMatch;
};

static EWRAM_DATA struct GcCompanion *sGc = NULL;
static EWRAM_DATA const struct GcModeHandler *sPendingHandler = NULL;

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
    // Never shown: only its RAM tile buffer is used to render text for the GameCube.
    [WIN_LABEL] = {.bg = 0, .tilemapLeft = 0, .tilemapTop = 22, .width = GC_TEXT_WINDOW_WIDTH, .height = GC_TEXT_WINDOW_HEIGHT, .paletteNum = 15, .baseBlock = 449},
    DUMMY_WIN_TEMPLATE,
};

static const u16 sTextPalette[16] = {RGB(2, 3, 7), RGB(31, 31, 31), RGB(9, 10, 13), RGB(31, 26, 8), RGB(31, 12, 10), RGB(16, 17, 20)};
static const u8 sColors[][3] =
{
    [GC_COLOR_NORMAL] = {0, 1, 2},
    [GC_COLOR_HIGHLIGHT] = {0, 3, 2},
    [GC_COLOR_ERROR] = {0, 4, 2},
    [GC_COLOR_DIM] = {0, 5, 2},
};

static const u8 sText_Title[] = _("{ENG}GC EXTENSION MODE");
static const u8 sText_TitleConnected[] = _("{ENG}GC CONNECTED");
static const u8 sText_Waiting[] = _("{ENG}Waiting for GameCube...");
static const u8 sText_WaitingHint[] = _("{ENG}JoyBus, GC port 2 (Dolphin)");
static const u8 sText_Reconnecting[] = _("{ENG}Link lost. Waiting for GC...");
static const u8 sText_Handshake[] = _("{ENG}Handshake...");
static const u8 sText_SendingParty[] = _("{ENG}Sending party ");
static const u8 sText_SendingAssets[] = _("{ENG}Sending data ");
static const u8 sText_Closing[] = _("{ENG}Closing session...");
static const u8 sText_ErrorMemory[] = _("{ENG}Not enough memory.");
static const u8 sText_ErrorHost[] = _("{ENG}The GameCube ended the session.");
static const u8 sText_ErrorLink[] = _("{ENG}Link lost.");
static const u8 sText_ErrorRefused[] = _("{ENG}This mode cannot start:");
static const u8 sText_Unavailable[] = _("{ENG}The link port is in use.");
static const u8 sText_StateOk[] = _("{ENG}Party/bag/flags/vars unchanged.");
static const u8 sText_StateChanged[] = _("{ENG}WARNING: game state changed!");
static const u8 sText_Returning[] = _("{ENG}Returning to the field...");
static const u8 sText_HelpCancel[] = _("{ENG}B: Cancel");
static const u8 sText_HelpError[] = _("{ENG}A: Reconnect   B: Exit");
static const u8 sText_HelpExit[] = _("{ENG}B: Exit");
static const u8 sText_Slash[] = _("{ENG}/");
static const u8 sText_Eng[] = _("{ENG}");

void GcStartMode(const struct GcModeHandler *handler)
{
    sPendingHandler = handler;
    LockPlayerFieldControls();
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_GcCompanion_WaitFadeOut, 10);
}

void GcCompanion_Start(void)
{
    GcStartMode(&gGcPartyViewMode);
}

bool8 GcExtension_IsAvailable(void)
{
    return gWirelessCommType == 0 && !gReceivedRemoteLinkPlayers;
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

void GcExtension_Redraw(void)
{
    if (sGc != NULL)
        sGc->dirty = TRUE;
}

void GcExtension_RequestExit(void)
{
    if (sGc != NULL && sGc->state == GC_STATE_ACTIVE)
        SetState(GC_STATE_DISCONNECTING);
}

bool8 GcExtension_IsExitRequested(void)
{
    return sGc != NULL && sGc->state == GC_STATE_DISCONNECTING;
}

void GcExtension_PrintLine(u8 windowId, const u8 *str, u8 x, u8 line, u8 color)
{
    AddTextPrinterParameterized3(windowId, FONT_NORMAL, x, line * 16, sColors[color], TEXT_SKIP_DRAW, str);
}

void GcExtension_SetExtraMons(struct Pokemon *mons, u8 count)
{
    GcPokemon_SetExtraMons(mons, count);
}

struct Pokemon *GcExtension_GetExportMon(u32 slot)
{
    return GcPokemon_GetMon(slot);
}

bool8 GcExtension_BuildRecord(u32 slot, PokemonRecord *record)
{
    return GcPokemon_BuildRecord(slot, record);
}

const u8 *GcExtension_RenderTextTiles(u8 fontId, const u8 colors[3], u8 fill, const u8 *str)
{
    return GcPokemon_RenderTextTiles(fontId, colors, fill, str);
}

bool8 GcExtension_RenderTextAsset(u8 fontId, const u8 colors[3], u8 fill, const u8 *str, u8 widthTiles, u8 heightTiles,
                                  const u16 *palette, AssetInfo *info, const u8 **data)
{
    return GcPokemon_RenderTextAsset(fontId, colors, fill, str, widthTiles, heightTiles, palette, info, data);
}

static u16 HandleModeResult(u16 modeId, u16 resultVersion, const u8 *data, u16 length)
{
    if (sGc->handler->onResult == NULL || modeId != sGc->handler->modeId)
        return RESULT_UNSUPPORTED;
    return sGc->handler->onResult(resultVersion, data, length);
}

// Opens the link and announces the mode; returns FALSE (with sGc->error set) on failure.
static bool8 StartSession(void)
{
    const struct GcModeHandler *h = sGc->handler;
    ModeInfo mode = {0};
    const u8 *request = NULL;
    u32 length = 0;

    if (!GcExtension_IsAvailable())
    {
        sGc->error = GC_ERROR_MODE_REFUSED;
        sGc->refusal = sText_Unavailable;
        return FALSE;
    }
    if (!GcPokemon_Init(WIN_LABEL, &sPokemonUi))
    {
        sGc->error = GC_ERROR_OUT_OF_MEMORY;
        return FALSE;
    }
    if (h->init != NULL && !h->init(&sGc->refusal))
    {
        sGc->error = GC_ERROR_MODE_REFUSED;
        return FALSE;
    }
    sGc->modeStarted = TRUE;
    GcPokemon_SetModeResolver(h->resolveAsset);
    sGc->config = *GcPokemon_GetClientConfig();
    sGc->config.modeAppId = h->appId;
    sGc->config.handleModeApp = h->handleApp;
    sGc->config.handleResult = HandleModeResult;
    GcProtocol_Init(&sGc->config);
    if (h->getRequest != NULL)
        request = h->getRequest(&length);
    mode.mode_id = h->modeId;
    mode.mode_version = h->modeVersion;
    mode.app_id = h->appId;
    mode.app_version = h->appVersion;
    mode.request_length = length;
    mode.nonce = Random32();
    GcProtocol_SetMode(&mode, request);
    sGc->linkOpen = GcLink_Open();
    if (!sGc->linkOpen)
    {
        sGc->error = GC_ERROR_OUT_OF_MEMORY;
        return FALSE;
    }
    return TRUE;
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
        sGc->handler = sPendingHandler != NULL ? sPendingHandler : &gGcPartyViewMode;
        SetState(StartSession() ? GC_STATE_CONNECTING : GC_STATE_ERROR);
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
    memset(ui, 0, sizeof(*ui));
    ui->confirmed = PKMN_NO_SLOT;
    if (sGc->handler->getPartyUi != NULL)
        sGc->handler->getPartyUi(ui);
    ui->count = gPartiesCount[B_TRAINER_PLAYER];
    ui->flags = sGc->state == GC_STATE_DISCONNECTING ? UI_EXIT_REQUESTED : 0;
}

static void SetPhase(u8 phase, u8 done, u8 total)
{
    sGc->phaseDone = done;
    sGc->phaseTotal = total;
    sGc->phase = phase;
    if (sGc->state == GC_STATE_DISCONNECTING || sGc->state == GC_STATE_RETURN_FIELD || sGc->state == GC_STATE_ERROR)
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
    GcExtension_PrintLine(WIN_BODY, text, 0, 0, GC_COLOR_NORMAL);
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
        GcExtension_PrintLine(WIN_BODY, sGc->reconnecting ? sText_Reconnecting : sText_Waiting, 0, 0, GC_COLOR_NORMAL);
        GcExtension_PrintLine(WIN_BODY, sText_WaitingHint, 0, 1, GC_COLOR_DIM);
        help = sText_HelpCancel;
        break;
    case GC_STATE_HANDSHAKE:
        title = sText_TitleConnected;
        GcExtension_PrintLine(WIN_BODY, sText_Handshake, 0, 0, GC_COLOR_NORMAL);
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
        if (sGc->handler->draw != NULL)
            sGc->handler->draw(WIN_BODY, WIN_HELP);
        break;
    case GC_STATE_DISCONNECTING:
        title = sText_TitleConnected;
        GcExtension_PrintLine(WIN_BODY, sText_Closing, 0, 0, GC_COLOR_NORMAL);
        break;
    case GC_STATE_ERROR:
        switch (sGc->error)
        {
        case GC_ERROR_OUT_OF_MEMORY:
            GcExtension_PrintLine(WIN_BODY, sText_ErrorMemory, 0, 0, GC_COLOR_ERROR);
            help = sText_HelpExit;
            break;
        case GC_ERROR_MODE_REFUSED:
            GcExtension_PrintLine(WIN_BODY, sText_ErrorRefused, 0, 0, GC_COLOR_ERROR);
            if (sGc->refusal != NULL)
                GcExtension_PrintLine(WIN_BODY, sGc->refusal, 0, 1, GC_COLOR_NORMAL);
            help = sText_HelpExit;
            break;
        default:
            GcExtension_PrintLine(WIN_BODY, sGc->error == GC_ERROR_HOST_ENDED ? sText_ErrorHost : sText_ErrorLink,
                                  0, 0, GC_COLOR_ERROR);
            help = sText_HelpError;
            break;
        }
        break;
    case GC_STATE_RETURN_FIELD:
        if (sGc->returnStep > 0)
        {
            GcExtension_PrintLine(WIN_BODY, sGc->exitMatch == STATE_DIGEST_ALL_MATCH ? sText_StateOk : sText_StateChanged,
                                  0, 0, sGc->exitMatch == STATE_DIGEST_ALL_MATCH ? GC_COLOR_NORMAL : GC_COLOR_ERROR);
            if (sGc->finishMessage != NULL)
                GcExtension_PrintLine(WIN_BODY, sGc->finishMessage, 0, 1, GC_COLOR_HIGHLIGHT);
        }
        GcExtension_PrintLine(WIN_BODY, sText_Returning, 0, 2, GC_COLOR_NORMAL);
        break;
    }
    GcExtension_PrintLine(WIN_TITLE, title, 0, 0, GC_COLOR_NORMAL);
    if (help != NULL)
        GcExtension_PrintLine(WIN_HELP, help, 0, 0, GC_COLOR_NORMAL);
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
        if (sGc->handler->input != NULL)
            sGc->handler->input();
        break;
    case GC_STATE_DISCONNECTING:
        if (++sGc->timer >= BYE_WAIT_FRAMES || JOY_NEW(B_BUTTON))
            SetState(GC_STATE_RETURN_FIELD);
        break;
    case GC_STATE_ERROR:
        if (JOY_NEW(A_BUTTON) && sGc->linkOpen && sGc->error != GC_ERROR_MODE_REFUSED)
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
    if (sGc->modeStarted && sGc->handler->free != NULL)
        sGc->handler->free();
    GcPokemon_Free();
    GcProtocol_Free();
    FreeAllWindowBuffers();
    TRY_FREE_AND_SET_NULL(sGc);
    sPendingHandler = NULL;
}

// Closes the link, verifies that the session left the game state unchanged,
// lets the mode apply a verified result once, and fades back to the field.
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
        if (sGc->modeStarted)
        {
            GcPokemon_ComputeDigest(&digest);
            sGc->exitMatch = GcPokemon_CompareWithEntry(&digest);
            if (sGc->handler->finish != NULL)
                sGc->finishMessage = sGc->handler->finish(&sGc->partyChanged);
        }
        else
        {
            sGc->exitMatch = STATE_DIGEST_ALL_MATCH;
        }
        sGc->returnStep++;
        sGc->timer = 0;
        sGc->dirty = TRUE;
        break;
    case 1:
        if (++sGc->timer >= RESULT_FRAMES + (sGc->finishMessage != NULL ? RESULT_FRAMES : 0))
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
