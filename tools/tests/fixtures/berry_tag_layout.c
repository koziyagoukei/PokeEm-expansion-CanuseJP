/* Host-only window I/O. The test inserts the real berry screen functions. */
enum { WIN_BERRY_NAME, WIN_SIZE_FIRM, WIN_DESC, WIN_BERRY_TAG };
struct WindowTemplate
{
    u8 bg, tilemapLeft, tilemapTop, width, height, paletteNum;
    u16 baseBlock;
};
#define DUMMY_WIN_TEMPLATE {0}
struct BerryInfo
{
    const u8 *name, *description1, *description2;
    u16 size;
    u8 firmness;
};
static struct { u16 tilemapBuffers[3][1024]; u16 berryId; } sTag;
static __typeof__(sTag) *sBerryTag = &sTag;
static const struct BerryInfo *sInfo;
static u8 sPixels[160][240];
static u16 sOwners[160][240];
static u16 sDrawId;
u8 gStringVar1[256], gStringVar2[256], gStringVar3[256], gStringVar4[1000];
enum StringConvertMode { STR_CONV_MODE_LEFT_ALIGN, STR_CONV_MODE_RIGHT_ALIGN, STR_CONV_MODE_LEADING_ZEROS };
static const s32 sPowersOfTen[] = {1, 10, 100, 1000};
static const u8 sDigits[] = {CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9};
static u32 WindowWidthPx(u8 id);
static const struct BerryInfo *GetBerryInfo(u16 id) { return sInfo; }
static u16 *GetBgTilemapBuffer(u8 bg) { static u16 data[1024]; return data; }
static void PutWindowTilemap(u8 id) {}
static void ScheduleBgCopyTilemapToVram(u8 bg) {}
static void AddTextPrinterParameterized4(u8 id, u8 font, u8 x, u8 y, u8 spacing,
    u8 lineSpacing, const u8 *colors, s8 speed, const u8 *text);
u16 AddTextPrinterParameterized(u8 id, u8 font, const u8 *text,
    u8 x, u8 y, u8 speed, TextPrinterCallback callback)
{
    static const u8 colors[] = {0, 2, 3};
    AddTextPrinterParameterized4(id, font, x, y, 0, 0, colors, speed, text);
    return 0;
}

/* SCREEN_FUNCTIONS */

static u32 WindowWidthPx(u8 id) { return sWindowTemplates[id].width * 8; }
static void host_pixel(u8 id, u32 x, u32 y, u8 color, u8 background)
{
    const struct WindowTemplate *w = &sWindowTemplates[id];
    if (color == background)
        return;
    if (x >= w->width * 8 || y >= w->height * 8)
        fprintf(stderr, "berry=%u window=%u pixel=(%u,%u)\n", sTag.berryId, id, x, y);
    assert(x < w->width * 8 && y < w->height * 8);
    x += w->tilemapLeft * 8;
    y += w->tilemapTop * 8;
    assert(x < 240 && y < 160);
    assert(sOwners[y][x] == 0 || sOwners[y][x] == sDrawId);
    sOwners[y][x] = sDrawId;
    sPixels[y][x] = color;
}
static void host_fill(u8 id, u8 color)
{
    const struct WindowTemplate *w = &sWindowTemplates[id];
    for (u32 y = w->tilemapTop * 8; y < (w->tilemapTop + w->height) * 8; y++)
        memset(&sPixels[y][w->tilemapLeft * 8], color, w->width * 8);
}
static void AddTextPrinterParameterized4(u8 id, u8 font, u8 x, u8 y, u8 spacing,
    u8 lineSpacing, const u8 *colors, s8 speed, const u8 *text)
{
    struct TextPrinter p = {0};
    p.printerTemplate = (struct TextPrinterTemplate){.currentChar = text,
        .type = WINDOW_TEXT_PRINTER, .windowId = id, .fontId = font,
        .x = x, .y = y, .currentX = x, .currentY = y,
        .letterSpacing = spacing, .lineSpacing = lineSpacing,
        .color = {.background = colors[0], .foreground = colors[1], .shadow = colors[2]}};
    assert(x + GetStringWidth(font, text, spacing) <= WindowWidthPx(id));
    p.japanese = IsJapaneseTextMode(text);
    p.fontId = font;
    p.isInUse = TRUE;
    GenerateFontHalfRowLookupTable(p.printerTemplate.color);
    sDrawId++;
    for (u32 i = 0; i < 4096; i++)
        if (RenderText(&p) == RENDER_FINISH)
            return;
    assert(!"renderer failed to terminate");
}

int main(int argc, char **argv)
{
    assert(argc == 2);
    reset();
    sPlaceholders[PLACEHOLDER_ID_STRING_VAR_1] = gStringVar1;
    sPlaceholders[PLACEHOLDER_ID_STRING_VAR_2] = gStringVar2;
    for (u32 i = 0; i < ARRAY_COUNT(sBerryCases); i++)
    {
        memset(sPixels, 255, sizeof(sPixels));
        memset(sOwners, 0, sizeof(sOwners));
        sDrawId = 0;
        sTag.berryId = i;
        sInfo = &sBerryCases[i];
        AddBerryTagTextToBg0();
        PrintAllBerryData();
        char path[1024];
        snprintf(path, sizeof(path), "%s/%02u.bin", argv[1], i);
        FILE *out = fopen(path, "wb");
        assert(out != NULL);
        assert(fwrite(sPixels, 1, sizeof(sPixels), out) == sizeof(sPixels));
        assert(fclose(out) == 0);
    }
    return 0;
}
