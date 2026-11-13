#ifndef GUARD_GC_COMPANION_PROTOCOL_H
#define GUARD_GC_COMPANION_PROTOCOL_H

// Generic message layer of GC Extension Mode (HELLO, PING, CAPS, APP,
// ASSET_INFO/ASSET_CHUNK, BYE). Game-specific behaviour is plugged in through
// the asset resolver and the application handler.

#include "gc_ext/message.h"

// Resolves an asset key to final data (pixels followed by palette). The data
// must stay valid until the next resolve call.
typedef bool (*GcAssetResolver)(const AssetKey *key, AssetInfo *info, const u8 **data);
// Handles one APP opcode. Writes up to APP_MAX_REPLY_DATA bytes to out.
typedef u16 (*GcAppHandler)(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength);
// Handles one RESULT payload (already de-duplicated by result id). Returns ResultStatus.
typedef u16 (*GcResultHandler)(u16 modeId, u16 resultVersion, const u8 *data, u16 length);

struct GcClientConfig
{
    u16 peerType;
    u32 buildId;
    u8 tag[4];
    u16 appId;
    u16 appVersion;
    GcAssetResolver resolveAsset;
    GcAppHandler handleApp;
    // Mode-specific APP id and handler (0 / NULL = none) and RESULT handler.
    u16 modeAppId;
    GcAppHandler handleModeApp;
    GcResultHandler handleResult;
};

// Event bits returned by GcProtocol_TakeEvents.
#define GC_PROTO_EVENT_HELLO (1 << 0)
#define GC_PROTO_EVENT_CAPS  (1 << 1)
#define GC_PROTO_EVENT_BYE   (1 << 2)
#define GC_PROTO_EVENT_ASSET (1 << 3)
#define GC_PROTO_EVENT_MODE   (1 << 4)
#define GC_PROTO_EVENT_RESULT (1 << 5)
#define GC_PROTO_EVENT_VERSION (1 << 6)  // the host's release does not match ours

struct GcProtocolStats
{
    u32 assetsOpened;
    u32 assetBytesSent;
    u32 appRequests;
};

void GcProtocol_Init(const struct GcClientConfig *config);
// Mode announced by MODE; the request bytes are served as asset
// {ASSET_MODE_REQUEST, mode id, 0, nonce} and must stay valid.
void GcProtocol_SetMode(const ModeInfo *info, const u8 *request);
bool GcProtocol_HandleFrame(const Frame *request, Frame *reply);
u32 GcProtocol_TakeEvents(void);
const Caps *GcProtocol_GetHostCaps(void);
// Release number this build sends in CAPS (GcExtension_GetRelease, 0 = development).
u16 GcProtocol_GetRelease(void);
// TRUE once CAPS showed a host whose release differs from ours.
bool8 GcProtocol_IsVersionMismatch(void);
const struct GcProtocolStats *GcProtocol_GetStats(void);
void GcProtocol_ResetSession(void);
void GcProtocol_Free(void);

#endif // GUARD_GC_COMPANION_PROTOCOL_H
