#ifndef GUARD_GC_EXTENSION_H
#define GUARD_GC_EXTENSION_H

// Generic GC Extension API.
//
// Game content (an NPC, a facility, a debug entry) starts a GameCube mode with
// GcStartMode(handler). The connector does everything that is not specific to
// the mode: JoyBus link, handshake and capabilities, party/record/asset export,
// transfer progress, cancel/timeout/BYE handling, the GBA screen frame and the
// return to the field. A mode supplies its request bytes, its own APP opcodes,
// what the GBA shows while it is active, and a single place (finish) where a
// verified result may be applied to persistent state.

#include "gc_ext/pkmn_protocol.h"

struct Pokemon;

// Export slots: 0..5 are the player party; content may register extra
// Pokemon (e.g. an opponent) that the host can read as records/assets.
#define GC_EXTRA_SLOT_BASE 0x80
#define GC_MAX_EXTRA_MONS 6
// Off-screen text window used to render text for the GameCube.
#define GC_TEXT_WINDOW_WIDTH 28
#define GC_TEXT_WINDOW_HEIGHT 4

enum GcColor { GC_COLOR_NORMAL, GC_COLOR_HIGHLIGHT, GC_COLOR_ERROR, GC_COLOR_DIM };

struct GcModeHandler
{
    u16 modeId;      // GC_MODE_* registry in gc_ext/message.h
    u16 modeVersion;
    u16 appId;       // APP id of this mode's opcodes (0 = none)
    u16 appVersion;
    // Called once after the GBA screen is ready and before the link opens.
    // Return FALSE to refuse the mode; reason is shown on the GBA.
    bool8 (*init)(const u8 **reason);
    // Request bytes the host downloads after MODE (may return NULL / 0).
    const u8 *(*getRequest)(u32 *length);
    u16 (*handleApp)(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength);
    // While the session is active: draw the body/help windows (use
    // GcExtension_PrintLine) and handle input. Call GcExtension_Redraw() when
    // the screen must change; call GcExtension_RequestExit() to leave.
    void (*draw)(u8 bodyWindow, u8 helpWindow);
    void (*input)(void);
    // A RESULT frame; called at most once per result id. Returns ResultStatus.
    u16 (*onResult)(u16 resultVersion, const u8 *data, u16 length);
    // After the link is closed and the unchanged-state check has run. The only
    // place a mode may change persistent state. Return a message to show
    // (or NULL) and set *changedParty if the party was modified.
    const u8 *(*finish)(bool8 *changedParty);
    void (*free)(void);
    // Optional: party viewer UI state reported through PKMN_UI_STATE.
    void (*getPartyUi)(PartyUi *ui);
    // Optional: serves asset types the connector does not know (e.g. the
    // game's own UI graphics for this mode). Data must stay valid until the
    // next call. Same contract as the connector's own resolver.
    bool (*resolveAsset)(const AssetKey *key, AssetInfo *info, const u8 **data);
};

// Starts a mode from a script special (declare the special with waitstate=1).
void GcStartMode(const struct GcModeHandler *handler);
// TRUE when no wireless/link session is using the serial port.
bool8 GcExtension_IsAvailable(void);
void GcExtension_Redraw(void);
void GcExtension_RequestExit(void);
bool8 GcExtension_IsExitRequested(void);
void GcExtension_PrintLine(u8 windowId, const u8 *str, u8 x, u8 line, u8 color);
void GcExtension_SetExtraMons(struct Pokemon *mons, u8 count);
struct Pokemon *GcExtension_GetExportMon(u32 slot);
bool8 GcExtension_BuildRecord(u32 slot, PokemonRecord *record);
// Renders text with the game's font into the off-screen window and returns its
// tiles (stride GC_TEXT_WINDOW_WIDTH tiles); colors = {background, foreground, shadow}.
const u8 *GcExtension_RenderTextTiles(u8 fontId, const u8 colors[3], u8 fill, const u8 *str);
// Same, packed as a widthTiles x heightTiles 4bpp asset with the given 16-colour palette.
bool8 GcExtension_RenderTextAsset(u8 fontId, const u8 colors[3], u8 fill, const u8 *str, u8 widthTiles, u8 heightTiles,
                                  const u16 *palette, AssetInfo *info, const u8 **data);

// Built-in generic modes: Phase 2 party viewer and the link check (both read-only).
extern const struct GcModeHandler gGcPartyViewMode;
extern const struct GcModeHandler gGcLinkCheckMode;
void GcCompanion_Start(void);

#endif // GUARD_GC_EXTENSION_H
