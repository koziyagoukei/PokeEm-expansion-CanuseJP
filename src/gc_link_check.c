// Built-in GC Extension mode: link check (GC_MODE_LINK_CHECK).
// Shows that the GameCube answered and which protocol, release and build
// each side runs. Read-only: it never changes game state.
#include "global.h"
#include "main.h"
#include "sound.h"
#include "string_util.h"
#include "constants/songs.h"
#include "gc_extension.h"
#include "gc_companion_pokemon.h"
#include "gc_companion_protocol.h"

static const u8 sText_Connected[] = _("{ENG}Connected to the GameCube.");
static const u8 sText_Gc[] = _("{ENG}GC  ");
static const u8 sText_Gba[] = _("{ENG}GBA ");
static const u8 sText_Release[] = _("{ENG} release ");
static const u8 sText_Protocol[] = _("{ENG}Protocol v");
static const u8 sText_Build[] = _("{ENG}  build ");
static const u8 sText_Ok[] = _("{ENG}Versions match.");
static const u8 sText_Dev[] = _("{ENG}Development build: not checked.");
static const u8 sText_Help[] = _("{ENG}B: Exit");

static void AppendHex(u8 *dest, u32 value)
{
    u8 digits[10];
    ConvertIntToHexStringN(digits, value, STR_CONV_MODE_LEADING_ZEROS, 8);
    StringAppend(dest, digits);
}

static void AppendDec(u8 *dest, u32 value)
{
    u8 digits[8];
    ConvertIntToDecimalStringN(digits, value, STR_CONV_MODE_LEFT_ALIGN, 5);
    StringAppend(dest, digits);
}

static void Draw(u8 body, u8 help)
{
    const Caps *host = GcProtocol_GetHostCaps();
    u8 text[64];

    GcExtension_PrintLine(body, sText_Connected, 0, 0, GC_COLOR_HIGHLIGHT);
    if (host == NULL)
        return;
    StringCopy(text, sText_Gc);
    StringAppend(text, sText_Release);
    AppendDec(text, host->release);
    StringAppend(text, sText_Build);
    AppendHex(text, host->build_id);
    GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_NORMAL);
    StringCopy(text, sText_Gba);
    StringAppend(text, sText_Release);
    AppendDec(text, GcProtocol_GetRelease());
    StringAppend(text, sText_Build);
    AppendHex(text, GcPokemon_GetBuildId());
    GcExtension_PrintLine(body, text, 0, 2, GC_COLOR_NORMAL);
    StringCopy(text, sText_Protocol);
    AppendDec(text, host->version);
    GcExtension_PrintLine(body, text, 0, 3, GC_COLOR_DIM);
    GcExtension_PrintLine(body, GcProtocol_GetRelease() == GCX_RELEASE_DEV ? sText_Dev : sText_Ok, 0, 4,
                          GC_COLOR_NORMAL);
    GcExtension_PrintLine(help, sText_Help, 0, 0, GC_COLOR_NORMAL);
}

static void Input(void)
{
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        GcExtension_RequestExit();
    }
}

const struct GcModeHandler gGcLinkCheckMode =
{
    .modeId = GC_MODE_LINK_CHECK,
    .modeVersion = 1,
    .draw = Draw,
    .input = Input,
};
