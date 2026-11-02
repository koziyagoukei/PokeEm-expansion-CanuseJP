#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "start_menu_ui.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "window.h"

struct StartMenuDisplay
{
    u16 tilemap[BG_SCREEN_SIZE / sizeof(u16)];
    u16 unfadedPalette[16];
    u16 fadedPalette[16];
    u16 dispcnt;
    u16 blend;
    u16 alpha;
    u16 brightness;
    u16 winIn;
    u16 winOut;
    u32 x;
    u32 y;
    u8 selected;
    u8 count;
    bool8 drawn;
    u8 anim;
    u8 animFrame;
    bool8 fadedOut;
};

enum
{
    MENU_ANIM_NONE,
    MENU_ANIM_OPEN,
    MENU_ANIM_CLOSE,
};

// The menu fades in over the field while rising into place, and sinks away on closing.
#define MENU_OPEN_FRAMES  8
#define MENU_CLOSE_FRAMES 6
#define MENU_RISE_PIXELS  16

static EWRAM_DATA struct StartMenuDisplay *sDisplay = NULL;
static const u32 sIcons[] = INCGFX_U32("graphics/interface/start_menu/icons.png", ".4bpp");
static const u16 sPalette[] = INCGFX_U16("graphics/interface/start_menu/palette.pal", ".gbapal");
static const u8 sText_Menu[] = _("メニュー");
static const u8 sText_PageSeparator[] = _("/");
static const u8 sLabelColors[] = {1, 2, 1};
static const u8 sMutedColors[] = {1, 5, 1};
static const u8 sDescriptionColors[] = {6, 2, 6};

// visibility runs from 0 (only the field) to 16 (only the menu).
static void SetMenuVisibility(u32 visibility)
{
    if (visibility >= 16)
    {
        ChangeBgY(0, 0, BG_COORD_SET);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON);
        return;
    }
    // Rows below the window are empty, so the field shows above the lowered menu.
    ChangeBgY(0, -(s32)((16 - visibility) * MENU_RISE_PIXELS / 16 * 256), BG_COORD_SET);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND
        | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(visibility, 16 - visibility));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    // The field's windows would otherwise switch the blend off where they apply.
    SetGpuReg(REG_OFFSET_WININ, sDisplay->winIn | WININ_WIN0_CLR | WININ_WIN1_CLR);
    SetGpuReg(REG_OFFSET_WINOUT, sDisplay->winOut | WINOUT_WIN01_CLR | WINOUT_WINOBJ_CLR);
    SetGpuReg(REG_OFFSET_DISPCNT, sDisplay->dispcnt | DISPCNT_BG0_ON);
}

static u32 GetMenuVisibility(void)
{
    switch (sDisplay->anim)
    {
    case MENU_ANIM_OPEN:
        return sDisplay->animFrame * 16 / MENU_OPEN_FRAMES;
    case MENU_ANIM_CLOSE:
        return 16 - sDisplay->animFrame * 16 / MENU_CLOSE_FRAMES;
    default:
        return 16;
    }
}

// Advances the open or close animation. Returns TRUE while it is still running.
bool32 StartMenuUi_Animate(void)
{
    u32 frames;

    if (sDisplay == NULL || sDisplay->anim == MENU_ANIM_NONE)
        return FALSE;
    frames = sDisplay->anim == MENU_ANIM_OPEN ? MENU_OPEN_FRAMES : MENU_CLOSE_FRAMES;
    if (++sDisplay->animFrame >= frames)
    {
        // A closed menu stays transparent until StartMenuUi_Close restores the field.
        SetMenuVisibility(sDisplay->anim == MENU_ANIM_OPEN ? 16 : 0);
        sDisplay->fadedOut = (sDisplay->anim == MENU_ANIM_CLOSE);
        sDisplay->anim = MENU_ANIM_NONE;
        return FALSE;
    }
    SetMenuVisibility(GetMenuVisibility());
    return TRUE;
}

void StartMenuUi_BeginClose(void)
{
    if (sDisplay == NULL)
        return;
    sDisplay->anim = MENU_ANIM_CLOSE;
    sDisplay->animFrame = 0;
    SetMenuVisibility(GetMenuVisibility());
}

bool32 StartMenuUi_Open(bool32 animate)
{
    if (sDisplay != NULL)
        return TRUE;
    sDisplay = AllocZeroed(sizeof(*sDisplay));
    if (sDisplay == NULL)
        return FALSE;
    sDisplay->anim = animate ? MENU_ANIM_OPEN : MENU_ANIM_NONE;
    CpuCopy16(GetBgTilemapBuffer(0), sDisplay->tilemap, sizeof(sDisplay->tilemap));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(15)], sDisplay->unfadedPalette, PLTT_SIZE_4BPP);
    CpuCopy16(&gPlttBufferFaded[BG_PLTT_ID(15)], sDisplay->fadedPalette, PLTT_SIZE_4BPP);
    sDisplay->dispcnt = GetGpuReg(REG_OFFSET_DISPCNT);
    sDisplay->blend = GetGpuReg(REG_OFFSET_BLDCNT);
    sDisplay->alpha = GetGpuReg(REG_OFFSET_BLDALPHA);
    sDisplay->brightness = GetGpuReg(REG_OFFSET_BLDY);
    sDisplay->winIn = GetGpuReg(REG_OFFSET_WININ);
    sDisplay->winOut = GetGpuReg(REG_OFFSET_WINOUT);
    sDisplay->x = GetBgX(0);
    sDisplay->y = GetBgY(0);
    if (AddFullscreenStartMenuWindow() == WINDOW_NONE)
    {
        Free(sDisplay);
        sDisplay = NULL;
        return FALSE;
    }
    return TRUE;
}

static void DrawIconCircle(u8 window, u32 x, u32 y, bool32 selected)
{
    static const u8 widths[] = {8, 14, 18, 20, 24, 26, 28, 28, 30, 30, 30, 32, 32, 32, 32, 32};
    for (u32 row = 0; row < 32; row++)
    {
        u32 width = widths[row < 16 ? row : 31 - row];
        FillWindowPixelRect(window, PIXEL_FILL(selected ? 13 : 8), x + (32 - width) / 2, y + row, width, 1);
        if (width > 4 && row > 1 && row < 30)
            FillWindowPixelRect(window, PIXEL_FILL(selected ? 3 : 2), x + (32 - width) / 2 + 1, y + row, width - 2, 1);
    }
}

void StartMenuUi_Draw(const struct StartMenuUiEntry *entries, u32 count, u32 selected, const u8 *status)
{
    u8 window = GetStartMenuWindowId();
    u32 first = selected / START_MENU_GRID_SIZE * START_MENU_GRID_SIZE;
    u8 page[12];
    u8 *end;
    bool32 full;
    if (sDisplay == NULL || window == WINDOW_NONE || selected >= count)
        return;

    full = !sDisplay->drawn || sDisplay->count != count
        || sDisplay->selected / START_MENU_GRID_SIZE != selected / START_MENU_GRID_SIZE;
    if (full)
    {
        FillWindowPixelBuffer(window, PIXEL_FILL(1));
        for (u32 y = 22; y < 122; y += 8)
            for (u32 x = (y & 8) ? 4 : 8; x < 240; x += 8)
                FillWindowPixelRect(window, PIXEL_FILL(7), x, y, 1, 1);
        FillWindowPixelRect(window, PIXEL_FILL(8), 0, 18, 240, 2);
        FillWindowPixelRect(window, PIXEL_FILL(13), 4, 4, 2, 9);
        AddTextPrinterParameterized4(window, FONT_SMALL, 10, 3, 0, 0, sLabelColors, TEXT_SKIP_DRAW, sText_Menu);
        if (status != NULL)
        {
            u32 width = GetStringWidth(FONT_SMALL, status, 0);
            AddTextPrinterParameterized4(window, FONT_SMALL, width < 158 ? 230 - width : 72, 3,
                                        0, 0, sMutedColors, TEXT_SKIP_DRAW, status);
        }
        else if (count > START_MENU_GRID_SIZE)
        {
            end = ConvertIntToDecimalStringN(page, first / START_MENU_GRID_SIZE + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
            end = StringCopy(end, sText_PageSeparator);
            ConvertIntToDecimalStringN(end, (count + START_MENU_GRID_SIZE - 1) / START_MENU_GRID_SIZE, STR_CONV_MODE_LEFT_ALIGN, 1);
            AddTextPrinterParameterized4(window, FONT_SMALL, 208, 3, 0, 0, sMutedColors, TEXT_SKIP_DRAW, page);
        }
    }

    for (u32 i = first; i < min(first + START_MENU_GRID_SIZE, count); i++)
    {
        u32 cell = i - first;
        u32 center = 26 + cell % START_MENU_GRID_COLUMNS * 47;
        u32 top = 24 + cell / START_MENU_GRID_COLUMNS * 49;
        if (!full && i != selected && i != sDisplay->selected)
            continue;
        FillWindowPixelRect(window, PIXEL_FILL(1), center - 16, top, 32, 32);
        DrawIconCircle(window, center - 16, top, i == selected);
        BlitBitmapToWindow(window, (const u8 *)sIcons + entries[i].icon * 24 * 24 / 2, center - 12, top + 4, 24, 24);
        if (full)
        {
            u32 width = GetStringWidth(FONT_SMALL, entries[i].label, 0);
            AddTextPrinterParameterized4(window, FONT_SMALL, center - min(width, 46) / 2, top + 33,
                                        0, 0, sLabelColors, TEXT_SKIP_DRAW, entries[i].label);
        }
    }
    FillWindowPixelRect(window, PIXEL_FILL(8), 0, 123, 240, 1);
    FillWindowPixelRect(window, PIXEL_FILL(6), 0, 124, 240, 36);
    FillWindowPixelRect(window, PIXEL_FILL(15), 0, 124, 3, 36);
    FillWindowPixelRect(window, PIXEL_FILL(8), 237, 124, 3, 36);
    FillWindowPixelRect(window, PIXEL_FILL(13), 0, 124, 3, 12);
    AddTextPrinterParameterized4(window, FONT_SMALL, 12, 128, 0, 1, sDescriptionColors, TEXT_SKIP_DRAW, entries[selected].description);
    sDisplay->selected = selected;
    sDisplay->count = count;
    sDisplay->drawn = TRUE;
    if (full)
        LoadPalette(sPalette, BG_PLTT_ID(15), sizeof(sPalette));
    PutWindowTilemap(window);
    CopyWindowToVram(window, COPYWIN_FULL);
    ChangeBgX(0, 0, BG_COORD_SET);
    SetMenuVisibility(GetMenuVisibility());
}

#define tDispcnt data[0]

// Shows BG0 again once the field tilemap and window tiles have reached VRAM.
static void Task_ShowFieldBg0AfterCopy(u8 taskId)
{
    if (IsDma3ManagerBusyWithBgCopy())
        return;
    SetGpuReg(REG_OFFSET_DISPCNT, gTasks[taskId].tDispcnt);
    DestroyTask(taskId);
}

void StartMenuUi_Close(void)
{
    bool32 fadedOut;

    if (sDisplay == NULL)
        return;
    fadedOut = sDisplay->fadedOut;
    RemoveStartMenuWindow();
    CpuCopy16(sDisplay->tilemap, GetBgTilemapBuffer(0), sizeof(sDisplay->tilemap));
    // The large canvas shares character memory with normal dialog windows.
    // Their RAM buffers are retained, so restore those tiles without resetting BGs.
    LoadMessageBoxAndBorderGfx();
    for (u32 i = 0; i < WINDOWS_MAX; i++)
        if (gWindows[i].window.bg == 0 && gWindows[i].tileData != NULL)
            CopyWindowToVram(i, COPYWIN_GFX);
    CopyBgTilemapBufferToVram(0);
    CpuCopy16(sDisplay->unfadedPalette, &gPlttBufferUnfaded[BG_PLTT_ID(15)], PLTT_SIZE_4BPP);
    CpuCopy16(sDisplay->fadedPalette, &gPlttBufferFaded[BG_PLTT_ID(15)], PLTT_SIZE_4BPP);
    if (gPaletteFade.y != 0)
        BlendPalettes(1 << 15, gPaletteFade.y, gPaletteFade.blendColor);
    ChangeBgX(0, sDisplay->x, BG_COORD_SET);
    ChangeBgY(0, sDisplay->y, BG_COORD_SET);
    SetGpuReg(REG_OFFSET_BLDCNT, sDisplay->blend);
    SetGpuReg(REG_OFFSET_BLDALPHA, sDisplay->alpha);
    SetGpuReg(REG_OFFSET_BLDY, sDisplay->brightness);
    SetGpuReg(REG_OFFSET_WININ, sDisplay->winIn);
    SetGpuReg(REG_OFFSET_WINOUT, sDisplay->winOut);
    if (fadedOut)
    {
        // The registers take effect before the field tilemap is copied back, which
        // would show the menu for a frame. It has faded out, so leave BG0 off until then.
        SetGpuReg(REG_OFFSET_DISPCNT, sDisplay->dispcnt & ~DISPCNT_BG0_ON);
        gTasks[CreateTask(Task_ShowFieldBg0AfterCopy, 0)].tDispcnt = sDisplay->dispcnt;
    }
    else
    {
        SetGpuReg(REG_OFFSET_DISPCNT, sDisplay->dispcnt);
    }
    Free(sDisplay);
    sDisplay = NULL;
}
