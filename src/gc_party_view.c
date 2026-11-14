// Built-in GC Extension mode: party viewer (GC_MODE_PARTY_VIEW).
// The GBA shows the party list; the GameCube mirrors the selected slot.
// Read-only: it never changes game state.
#include "global.h"
#include "main.h"
#include "malloc.h"
#include "pokemon.h"
#include "sound.h"
#include "string_util.h"
#include "window.h"
#include "constants/songs.h"
#include "gc_extension.h"

struct GcPartyView
{
    u8 screen;
    u8 cursor;
    u8 confirmed;
    u16 inputSeq;
};

static EWRAM_DATA struct GcPartyView *sView = NULL;

static const u8 sText_MenuParty[] = _("{ENG}PARTY");
static const u8 sText_HelpMenu[] = _("{ENG}A: Select   B: Exit");
static const u8 sText_HelpParty[] = _("{ENG}UP/DOWN: Move  A: Select  B: Back");
static const u8 sText_Cursor[] = _("{ENG}>");
static const u8 sText_Selected[] = _("{ENG}+");
static const u8 sText_Slash[] = _("{ENG}/");
static const u8 sText_Lv[] = _("{ENG}Lv.");
static const u8 sText_Jpn[] = _("{JPN}");
static const u8 sText_Eng[] = _("{ENG}");
static const u8 sText_NoMemory[] = _("{ENG}Not enough memory.");

static bool8 Init(const u8 **reason)
{
    sView = AllocZeroedUnchecked(sizeof(*sView));
    if (sView == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    sView->confirmed = PKMN_NO_SLOT;
    return TRUE;
}

static void FreeView(void)
{
    TRY_FREE_AND_SET_NULL(sView);
}

static void GetPartyUi(PartyUi *ui)
{
    ui->cursor = sView->cursor;
    ui->confirmed = sView->confirmed;
    ui->screen = sView->screen;
    ui->input_seq = sView->inputSeq;
}

static void AppendNumber(u8 *dest, s32 value)
{
    u8 digits[8];
    ConvertIntToDecimalStringN(digits, value, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(dest, sText_Eng);
    StringAppend(dest, digits);
}

static void Draw(u8 body, u8 help)
{
    u8 text[64];
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    u32 slot;

    if (sView->screen == SCREEN_MENU)
    {
        GcExtension_PrintLine(body, sText_Cursor, 0, 0, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(body, sText_MenuParty, 12, 0, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_HelpMenu, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    for (slot = 0; slot < gPartiesCount[B_TRAINER_PLAYER] && slot < PARTY_SIZE; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        u8 color = slot == sView->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
        if (slot == sView->cursor)
            GcExtension_PrintLine(body, sText_Cursor, 0, slot, color);
        if (slot == sView->confirmed)
            GcExtension_PrintLine(body, sText_Selected, 8, slot, color);
        GetMonData(mon, MON_DATA_NICKNAME, name);
        StringCopy(text, sText_Jpn);
        StringAppend(text, name);
        GcExtension_PrintLine(body, text, 18, slot, color);
        StringCopy(text, sText_Lv);
        AppendNumber(text, GetMonData(mon, MON_DATA_LEVEL));
        GcExtension_PrintLine(body, text, 120, slot, color);
        StringCopy(text, sText_Eng);
        AppendNumber(text, GetMonData(mon, MON_DATA_HP));
        StringAppend(text, sText_Slash);
        AppendNumber(text, GetMonData(mon, MON_DATA_MAX_HP));
        GcExtension_PrintLine(body, text, 168, slot, color);
    }
    GcExtension_PrintLine(help, sText_HelpParty, 0, 0, GC_COLOR_NORMAL);
}

static void Changed(void)
{
    sView->inputSeq++;
    GcExtension_Redraw();
    PlaySE(SE_SELECT);
}

static void Input(void)
{
    u8 count = gPartiesCount[B_TRAINER_PLAYER];

    if (sView->screen == SCREEN_MENU)
    {
        if (JOY_NEW(A_BUTTON))
        {
            sView->screen = SCREEN_PARTY;
            Changed();
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            GcExtension_RequestExit();
        }
    }
    else if (JOY_REPEAT(DPAD_UP) && count != 0)
    {
        sView->cursor = (sView->cursor + count - 1) % count;
        Changed();
    }
    else if (JOY_REPEAT(DPAD_DOWN) && count != 0)
    {
        sView->cursor = (sView->cursor + 1) % count;
        Changed();
    }
    else if (JOY_NEW(A_BUTTON))
    {
        sView->confirmed = sView->cursor;
        Changed();
    }
    else if (JOY_NEW(B_BUTTON))
    {
        sView->screen = SCREEN_MENU;
        Changed();
    }
}

const struct GcModeHandler gGcPartyViewMode =
{
    .modeId = GC_MODE_PARTY_VIEW,
    .modeVersion = 1,
    .init = Init,
    .draw = Draw,
    .input = Input,
    .free = FreeView,
    .getPartyUi = GetPartyUi,
};
