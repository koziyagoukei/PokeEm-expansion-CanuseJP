#include "global.h"
#include "bg.h"
#include "gpu_regs.h"
#include "malloc.h"
#include "menu.h"
#include "palette.h"
#include "start_menu_ui.h"
#include "string_util.h"
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
    u32 x;
    u32 y;
    u8 selected;
    u8 count;
    bool8 drawn;
};

static EWRAM_DATA struct StartMenuDisplay *sDisplay = NULL;
static const u32 sIcons[] = INCGFX_U32("graphics/interface/start_menu/icons.png", ".4bpp");
static const u16 sPalette[] = INCGFX_U16("graphics/interface/start_menu/palette.pal", ".gbapal");
static const u8 sText_Menu[] = _("メニュー");
static const u8 sText_PageSeparator[] = _("/");
static const u8 sLabelColors[] = {1, 2, 1};
static const u8 sMutedColors[] = {1, 5, 1};
static const u8 sDescriptionColors[] = {6, 2, 6};

bool32 StartMenuUi_Open(void)
{
    if (sDisplay != NULL)
        return TRUE;
    sDisplay = AllocZeroed(sizeof(*sDisplay));
    if (sDisplay == NULL)
        return FALSE;
    CpuCopy16(GetBgTilemapBuffer(0), sDisplay->tilemap, sizeof(sDisplay->tilemap));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(15)], sDisplay->unfadedPalette, PLTT_SIZE_4BPP);
    CpuCopy16(&gPlttBufferFaded[BG_PLTT_ID(15)], sDisplay->fadedPalette, PLTT_SIZE_4BPP);
    sDisplay->dispcnt = GetGpuReg(REG_OFFSET_DISPCNT);
    sDisplay->blend = GetGpuReg(REG_OFFSET_BLDCNT);
    sDisplay->alpha = GetGpuReg(REG_OFFSET_BLDALPHA);
    sDisplay->brightness = GetGpuReg(REG_OFFSET_BLDY);
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
        FillWindowPixelRect(window, PIXEL_FILL(selected ? 14 : 8), x + (32 - width) / 2, y + row, width, 1);
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
        FillWindowPixelRect(window, PIXEL_FILL(8), 8, 18, 224, 1);
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
    AddTextPrinterParameterized4(window, FONT_SMALL, 12, 128, 0, 1, sDescriptionColors, TEXT_SKIP_DRAW, entries[selected].description);
    sDisplay->selected = selected;
    sDisplay->count = count;
    sDisplay->drawn = TRUE;
    if (full)
        LoadPalette(sPalette, BG_PLTT_ID(15), sizeof(sPalette));
    PutWindowTilemap(window);
    CopyWindowToVram(window, COPYWIN_FULL);
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON);
}

void StartMenuUi_Close(void)
{
    if (sDisplay == NULL)
        return;
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
    SetGpuReg(REG_OFFSET_DISPCNT, sDisplay->dispcnt);
    Free(sDisplay);
    sDisplay = NULL;
}
