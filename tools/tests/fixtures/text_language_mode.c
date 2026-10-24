/* Host harness: the Python test inserts the actual engine functions below. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef u8 bool8;
typedef u16 bool16;
typedef u32 bool32;
#define TRUE 1
#define FALSE 0
#define PACKED __attribute__((packed))
#define DEPRECATED(...)
#define STATIC_ASSERT(c, n) _Static_assert(c, #n);
#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define max(a, b) ((a) >= (b) ? (a) : (b))
#define CpuFill32(value, dst, size) memset(dst, value, size)
#include "text.h"
#include "constants/speaker_names.h"
#if TEST_LEGACY_MODE
#undef TEXT_JAPANESE_BY_DEFAULT
#define TEXT_JAPANESE_BY_DEFAULT FALSE
#endif
#define MAX_TEXT_PLACEHOLDER_DEPTH 8
#define CHAR_JP_VU 0xF1
#define JOY_HELD(...) 0
#define JOY_NEW(...) 0
#define BATTLE_TYPE_RECORDED 1
#define NAME_BOX_BASE_TILE_NUM 0
#define COPYWIN_GFX 0
#define PIXEL_FILL(c) (c)
#define SPRITE_NONE 0xFF
#define JPN EXT_CTRL_CODE_BEGIN, EXT_CTRL_CODE_JPN
#define ENG EXT_CTRL_CODE_BEGIN, EXT_CTRL_CODE_ENG
#define AUTO EXT_CTRL_CODE_BEGIN, EXT_CTRL_CODE_AUTO
#define CTRL EXT_CTRL_CODE_BEGIN
enum Language { LANGUAGE_JAPANESE = 1, LANGUAGE_ENGLISH = 2 };
struct Sprite { u8 nextX, nextY; };
static struct Sprite gSprites[1];
TextFlags gTextFlags;
bool8 gDisableTextPrinters;
struct TextGlyph gCurGlyph;
static u32 gBattleTypeFlags;
static int gMPlayInfo_BGM;
static const u8 *gSpeakerNamesTable[256];
static const u8 sKeypadIconTiles[4096];
static u8 sCurGlyphAdvance;
static const u8 *sStringPointers[8], *sPlaceholders[8];
static const u8 sEmpty[] = {EOS};
static bool32 sLoadedJapanese;
static struct { bool32 japanese; u8 width, advance, x; } sTrace[128];
static u32 sTraceCount, sClearedWidth;
static struct TextPrinter sAllocatedPrinter;
static u16 RenderText(struct TextPrinter *p);
static const u8 *GetExpandedPlaceholder(u32 id)
{
    return id < ARRAY_COUNT(sPlaceholders) && sPlaceholders[id] != NULL ? sPlaceholders[id] : sEmpty;
}
const u8 *DynamicPlaceholderTextUtil_GetPlaceholderPtr(u8 idx);
u8 GetExtCtrlCodeLength(u8 code);
void StripExtCtrlCodes(u8 *str);
static u32 OffsetCurrGlyph(u32 amount) { return 0; }
u32 CopyGlyphToVRAM(struct TextPrinter *p)
{
    assert(sTraceCount < ARRAY_COUNT(sTrace));
    sTrace[sTraceCount].japanese = sLoadedJapanese;
    sTrace[sTraceCount].width = gCurGlyph.width;
    sTrace[sTraceCount].advance = sCurGlyphAdvance ? sCurGlyphAdvance : gCurGlyph.width;
    sTrace[sTraceCount++].x = p->printerTemplate.currentX;
    return 0;
}
void ClearTextSpan(struct TextPrinter *p, u32 width)
{
    assert(width < 256);
    sClearedWidth += width;
}
void GenerateFontHalfRowLookupTable(union TextColor color) {}
static void CopyWindowToVram(u8 id, u8 mode) {}
static void FillWindowPixelBuffer(u8 id, u8 color) {}
static void ScrollWindow(u8 id, u8 direction, u8 distance, u8 color) {}
static void PlayBGM(u16 song) {}
static void PlaySE(u16 song) {}
static void m4aMPlayStop(void *info) {}
static void m4aMPlayContinue(void *info) {}
static void TrySpawnAndShowNamebox(const u8 *name, u16 tile) {}
static bool32 IsSEPlaying(void) { return FALSE; }
static u32 GetSpriteHeight(struct Sprite *sprite) { return 64; }
bool32 TextPrinterWait(struct TextPrinter *p) { return TRUE; }
bool32 TextPrinterWaitWithDownArrow(struct TextPrinter *p) { return TRUE; }
void TextPrinterInitDownArrowCounters(struct TextPrinter *p) {}
void TextPrinterClearDownArrow(struct TextPrinter *p) {}
bool32 IsPlayerTextSpeedInstant(void) { return TRUE; }
u32 GetPlayerTextScrollSpeed(void) { return 4; }
u32 GetPlayerTextSpeedModifier(void) { return 1; }
u32 GetGlyphWidth_Braille(u16 glyphId, bool32 japanese) { return 8; }
static u32 RenderFont(struct TextPrinter *p) { return RenderText(p); }
static struct TextPrinter *AllocateTextPrinter(void) { return &sAllocatedPrinter; }
static void BlitBitmapRectToWindow(u8 window, const void *src, u16 a, u16 b,
    u16 c, u16 d, u16 x, u16 y, u16 w, u16 h) {}

/* ENGINE_FUNCTIONS */

static void reset(void)
{
    memset(sPlaceholders, 0, sizeof(sPlaceholders));
    DynamicPlaceholderTextUtil_Reset();
    sTraceCount = sClearedWidth = sCurGlyphAdvance = 0;
    sLoadedJapanese = FALSE;
    memset(&gTextFlags, 0, sizeof(gTextFlags));
    memset(gSprites, SPRITE_NONE, sizeof(gSprites));
}

static struct TextPrinter render(const u8 *str, u8 font, u8 spacing, u8 type)
{
    struct TextPrinterTemplate template = {.currentChar = str, .fontId = font,
        .letterSpacing = spacing, .type = type};
    assert(AddTextPrinter(&template, 2, NULL));
    struct TextPrinter p = sAllocatedPrinter;
    assert(p.japanese == IsJapaneseTextMode(str));
    p.fontId = font;
    p.isInUse = TRUE;
    for (u32 i = 0; i < 4096; i++)
        if (RenderText(&p) == RENDER_FINISH)
            return p;
    assert(!"renderer failed to terminate");
    return p;
}

static void parity(const u8 *str, u8 font, u8 spacing, u8 type, u8 trailing)
{
    reset();
    struct TextPrinter p = render(str, font, spacing, type);
    s32 width = GetStringWidth(font, str, spacing);
    if (width != p.printerTemplate.currentX - trailing)
        fprintf(stderr, "font=%u spacing=%u width=%d renderer=%u trailing=%u\n",
            font, spacing, width, p.printerTemplate.currentX, trailing);
    assert(width == p.printerTemplate.currentX - trailing);
}

static void modes(void)
{
    const u8 kana[] = {1, 2, 0x51, 0x52, 0xF1, EOS};
    const u8 latin[] = {CHAR_A, CHAR_B, CHAR_0, EOS};
    const u8 jp[] = {JPN, CHAR_A, EOS}, en[] = {ENG, 1, EOS};
    const u8 autoJp[] = {AUTO, 1, EOS}, autoEn[] = {AUTO, CHAR_A, EOS};
    assert(IsJapaneseTextMode(kana));
    assert(IsJapaneseTextMode(latin) == TEXT_JAPANESE_BY_DEFAULT);
    assert(IsJapaneseTextMode(sEmpty) == TEXT_JAPANESE_BY_DEFAULT);
    assert(IsJapaneseTextMode(jp));
    assert(!IsJapaneseTextMode(en));
    assert(IsJapaneseTextMode(autoJp));
    assert(!IsJapaneseTextMode(autoEn));
    assert(IsJapaneseGlyph(1) && IsJapaneseGlyph(0x50) && IsJapaneseGlyph(0x51));
    assert(IsJapaneseGlyph(0xA0) && IsJapaneseGlyph(0xF1));
    assert(!IsJapaneseGlyph(CHAR_SPACE) && !IsJapaneseGlyph(CHAR_0));
    reset();
    render(latin, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER);
    assert(sTraceCount == 3 && sTrace[0].japanese == TEXT_JAPANESE_BY_DEFAULT);
    reset();
    render(en, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER);
    assert(sTraceCount == 1 && !sTrace[0].japanese);
}

static void metrics(void)
{
    const u8 kana[] = {1, 2, 0x51, 0x52, 0xF1, EOS};
    const u8 jp[] = {JPN, 1, 2, 0x51, 0x52, 0xF1, EOS};
    const u8 en[] = {ENG, CHAR_A, CHAR_B, CHAR_0, CHAR_SPACE, CHAR_PLUS, CHAR_e_ACUTE, EOS};
    const u8 fonts[] = {FONT_NORMAL, FONT_SMALL, FONT_NARROW, FONT_NARROWER,
        FONT_SHORT, FONT_SMALL_NARROW, FONT_SMALL_NARROWER, FONT_SHORT_NARROW, FONT_SHORT_NARROWER};
    for (u32 i = 0; i < ARRAY_COUNT(fonts); i++)
    {
        assert(GetStringWidth(fonts[i], kana, 0) == GetStringWidth(fonts[i], jp, 0));
        for (u32 type = WINDOW_TEXT_PRINTER; type <= SPRITE_TEXT_PRINTER; type++)
        {
            parity(kana, fonts[i], 0, type, 0);
            parity(en, fonts[i], 3, type, 0);
        }
    }
    const u8 lines[] = {JPN, 1, 2, CHAR_NEWLINE, 1, 2, 3, EOS};
    assert(GetStringWidth(FONT_NORMAL, lines, 0) == 21);
}

static void switches(void)
{
    const u8 mixed[] = {JPN, 1, ENG, CHAR_A, CHAR_PLUS, AUTO, 0x51, 0x52, ENG, CHAR_B, EOS};
    const u8 spaced[] = {JPN, 1, 2, ENG, EOS};
    const u8 automatic[] = {ENG, CHAR_A, AUTO, 1, 2, EOS};
    parity(mixed, FONT_NORMAL, 2, WINDOW_TEXT_PRINTER, 0);
    assert(sTraceCount == 6);
    assert(sTrace[0].japanese && !sTrace[1].japanese && !sTrace[2].japanese);
    assert(sTrace[3].japanese && sTrace[4].japanese && !sTrace[5].japanese);
    parity(spaced, FONT_NORMAL, 2, WINDOW_TEXT_PRINTER, 2);
    parity(automatic, FONT_NORMAL, 2, WINDOW_TEXT_PRINTER, 2);
}

static void controls(void)
{
    const u8 str[] = {CTRL, EXT_CTRL_CODE_COLOR, EXT_CTRL_CODE_ENG,
        CTRL, EXT_CTRL_CODE_PLAY_SE, EOS, EXT_CTRL_CODE_JPN,
        CTRL, EXT_CTRL_CODE_TEXT_COLORS, EXT_CTRL_CODE_AUTO, CHAR_DYNAMIC, EOS,
        JPN, 1, AUTO, CHAR_A, EOS};
    u8 expanded[128];
    reset();
    StringExpandPlaceholders(expanded, str);
    assert(memcmp(str, expanded, sizeof(str)) == 0);
    DynamicPlaceholderTextUtil_ExpandPlaceholders(expanded, str);
    assert(memcmp(str, expanded, sizeof(str)) == 0);
    parity(str, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER, 0);
    assert(sTraceCount == 2 && sTrace[0].japanese && !sTrace[1].japanese);
    assert(GetExtCtrlCodeLength(EXT_CTRL_CODE_AUTO) == 1);
    assert(GetExtCtrlCodeLength(EXT_CTRL_CODE_SPEAKER) == 2);
    assert(GetExtCtrlCodeLength(EXT_CTRL_CODE_PLAY_SE) == 3);
    const u8 speaker[] = {CTRL, EXT_CTRL_CODE_SPEAKER, EXT_CTRL_CODE_ENG, 1, EOS};
    assert(IsJapaneseTextMode(speaker));
}

static void placeholders(void)
{
    const u8 jp[] = {JPN, 1, 2, EOS}, en[] = {ENG, CHAR_A, EOS};
    const u8 str[] = {PLACEHOLDER_BEGIN, 1, PLACEHOLDER_BEGIN, 2, JPN, 0x51, EOS};
    const u8 dynamic[] = {CHAR_DYNAMIC, 0, CHAR_DYNAMIC, 255, JPN, 0x51, EOS};
    const u8 nested[] = {PLACEHOLDER_BEGIN, 3, EOS};
    u8 expanded[128];
    reset();
    sPlaceholders[1] = jp;
    sPlaceholders[2] = en;
    sPlaceholders[3] = jp;
    StringExpandPlaceholders(expanded, str);
    assert(GetStringWidth(FONT_NORMAL, str, 2) == GetStringWidth(FONT_NORMAL, expanded, 2));
    struct TextPrinter p = render(expanded, FONT_NORMAL, 2, WINDOW_TEXT_PRINTER);
    assert(GetStringWidth(FONT_NORMAL, str, 2) == p.printerTemplate.currentX - 2);
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, nested);
    DynamicPlaceholderTextUtil_ExpandPlaceholders(expanded, dynamic);
    u8 full[128];
    StringExpandPlaceholders(full, expanded);
    assert(GetStringWidth(FONT_NORMAL, dynamic, 0) == GetStringWidth(FONT_NORMAL, full, 0));
    assert(IsJapaneseTextMode(dynamic));
    assert(DynamicPlaceholderTextUtil_GetPlaceholderPtr(255) == NULL);
    const u8 recursive[] = {CHAR_DYNAMIC, 0, EOS};
    DynamicPlaceholderTextUtil_SetPlaceholderPtr(0, recursive);
    assert(GetStringWidth(FONT_NORMAL, recursive, 0) == 0);
    assert(IsJapaneseTextMode(recursive) == TEXT_JAPANESE_BY_DEFAULT);
}

static void names(void)
{
    u8 name[32] = {1, 2, EOS}, original[32];
    reset();
    ConvertInternationalString(name, LANGUAGE_JAPANESE);
    assert(name[0] == CTRL && name[1] == EXT_CTRL_CODE_JPN);
    assert(name[4] == CTRL);
    assert(name[5] == (TEXT_JAPANESE_BY_DEFAULT ? EXT_CTRL_CODE_JPN : EXT_CTRL_CODE_ENG));
    assert(name[6] == EOS);
    memcpy(original, name, sizeof(name));
    ConvertInternationalContestantName(name);
    assert(memcmp(original, name, sizeof(name)) == 0);
    ConvertInternationalString(name, LANGUAGE_JAPANESE);
    assert(memcmp(original, name, sizeof(name)) == 0);
    const u8 str[] = {PLACEHOLDER_BEGIN, 1, CHAR_0, CHAR_1, 0x51, EOS};
    sPlaceholders[1] = name;
    u8 expanded[128];
    StringExpandPlaceholders(expanded, str);
    render(expanded, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER);
    assert(sTraceCount == 5 && sTrace[0].japanese);
    assert(!sTrace[2].japanese && !sTrace[3].japanese);
    assert(sTrace[4].japanese == TEXT_JAPANESE_BY_DEFAULT);
    StripExtCtrlCodes(name);
    assert(name[0] == 1 && name[1] == 2 && name[2] == EOS);
}

static void symbols(void)
{
    const u8 str[] = {JPN, 1, CHAR_EXTRA_SYMBOL, 0xD0,
        CTRL, EXT_CTRL_CODE_ESCAPE, 0xF0, 0x51, EOS};
    parity(str, FONT_NORMAL, 1, WINDOW_TEXT_PRINTER, 1);
    assert(sTraceCount == 4 && sTrace[0].japanese && sTrace[3].japanese);
    assert(!sTrace[1].japanese && !sTrace[2].japanese);
    assert(sTrace[1].advance == GetGlyphWidth(0x1D0, FALSE, FONT_NORMAL));
    assert(GetGlyphWidth(0x1D0, TRUE, FONT_NORMAL) == GetGlyphWidth(0x1D0, FALSE, FONT_NORMAL));
    const u8 icons[] = {JPN, 1, CHAR_KEYPAD_ICON, CHAR_L_BUTTON, 2, EOS};
    parity(icons, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER, 0);
    parity(icons, FONT_NORMAL, 0, SPRITE_TEXT_PRINTER, 0);
    for (u32 index = 0xD0; index <= 0xFE; index++)
    {
        u8 emoji[] = {JPN, CHAR_EXTRA_SYMBOL, index, 1, EOS}, expanded[32];
        StringExpandPlaceholders(expanded, emoji);
        assert(memcmp(expanded, emoji, sizeof(emoji)) == 0);
        DynamicPlaceholderTextUtil_ExpandPlaceholders(expanded, emoji);
        assert(memcmp(expanded, emoji, sizeof(emoji)) == 0);
        parity(emoji, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER, 0);
    }
}

static void minimum(void)
{
    const u8 small[] = {JPN, CTRL, EXT_CTRL_CODE_MIN_LETTER_SPACING, 6, 1, 2, EOS};
    const u8 large[] = {JPN, CTRL, EXT_CTRL_CODE_MIN_LETTER_SPACING, 10, 1, 2, EOS};
    parity(small, FONT_NORMAL, 2, WINDOW_TEXT_PRINTER, 0);
    assert(sClearedWidth == 0 && GetStringWidth(FONT_NORMAL, small, 2) == 14);
    parity(large, FONT_NORMAL, 2, WINDOW_TEXT_PRINTER, 0);
    assert(sClearedWidth == 6 && GetStringWidth(FONT_NORMAL, large, 2) == 20);
}

static void numeric(void)
{
    const u8 fonts[] = {FONT_SMALL, FONT_NORMAL, FONT_SHORT, FONT_NARROW, FONT_NARROWER,
        FONT_SMALL_NARROW, FONT_SMALL_NARROWER, FONT_SHORT_NARROW, FONT_SHORT_NARROWER};
    for (u32 f = 0; f < ARRAY_COUNT(fonts); f++)
    {
        for (u32 digit = CHAR_0; digit <= CHAR_9; digit++)
        {
            const u8 text[] = {JPN, 1, digit, 2, EOS};
            assert(GetGlyphWidth(digit, TRUE, fonts[f]) == GetGlyphWidth(digit, FALSE, fonts[f]));
            for (u32 type = WINDOW_TEXT_PRINTER; type <= SPRITE_TEXT_PRINTER; type++)
            {
                parity(text, fonts[f], 0, type, 0);
                assert(sTraceCount == 3 && sTrace[0].japanese && !sTrace[1].japanese && sTrace[2].japanese);
                assert(sTrace[1].advance == GetGlyphWidth(digit, FALSE, fonts[f]));
                parity(text, fonts[f], 2, type, 2);
            }
        }
    }
    const u8 level[] = {ENG, CHAR_EXTRA_SYMBOL, CHAR_LV_2, CHAR_1, CHAR_0, CHAR_0, EOS};
    const u8 pp[] = {ENG, CHAR_6, CHAR_4, CHAR_SLASH, CHAR_6, CHAR_4, EOS};
    const u8 power[] = {ENG, CHAR_1, CHAR_0, CHAR_0, EOS};
    const u8 weight[] = {ENG, CHAR_9, CHAR_9, CHAR_9, CHAR_PERIOD, CHAR_9, CHAR_SPACE, CHAR_k, CHAR_g, CHAR_PERIOD, EOS};
    const u8 period[] = {JPN, CHAR_PERIOD, EOS};
    parity(level, FONT_SMALL, 0, SPRITE_TEXT_PRINTER, 0);
    assert(GetStringWidth(FONT_SMALL, level, 0) <= 24);
    assert(GetStringWidth(FONT_NORMAL, pp, 0) <= 44);
    assert(GetStringWidth(FONT_NORMAL, power, 0) <= 19);
    parity(weight, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER, 0);
    for (u32 i = 0; i < sTraceCount; i++)
        assert(!sTrace[i].japanese);
    parity(period, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER, 0);
    assert(sTraceCount == 1 && sTrace[0].japanese);
}

int main(int argc, char **argv)
{
    assert(argc == 2);
    if (!strcmp(argv[1], "modes")) modes();
    else if (!strcmp(argv[1], "metrics")) metrics();
    else if (!strcmp(argv[1], "switches")) switches();
    else if (!strcmp(argv[1], "controls")) controls();
    else if (!strcmp(argv[1], "placeholders")) placeholders();
    else if (!strcmp(argv[1], "names")) names();
    else if (!strcmp(argv[1], "symbols")) symbols();
    else if (!strcmp(argv[1], "minimum")) minimum();
    else if (!strcmp(argv[1], "numeric")) numeric();
    else assert(!"unknown test case");
    return 0;
}
