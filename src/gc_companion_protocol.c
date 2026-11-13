// Generic message layer for GC Extension Mode. Knows frames and messages,
// not Pokemon: game data arrives through GcClientConfig callbacks.
#include "global.h"
#include "gc_companion_link.h"
#include "malloc.h"
#include "gc_companion_protocol.h"

struct GcProtocolState
{
    const struct GcClientConfig *config;
    Caps hostCaps;
    bool8 haveCaps;
    bool8 assetOpen;
    AssetKey openKey;
    const u8 *openData;
    u32 openLength;
    u32 events;
    struct GcProtocolStats stats;
    ModeInfo mode;
    const u8 *modeRequest;
    bool8 haveResult;
    u32 lastResultId;
    u8 *encoded;  // RLE copy of the open asset (freed on the next asset)
    bool8 versionMismatch;
};

static EWRAM_DATA struct GcProtocolState sProto = {0};

// Content that ships a numbered release pairs it with a GameCube host by
// defining this symbol (see gba-gc-extension docs/releases.md). Development
// builds keep 0 and accept any host.
const u16 gGcExtensionRelease __attribute__((weak)) = GCX_RELEASE_DEV;

void GcProtocol_Init(const struct GcClientConfig *config)
{
    TRY_FREE_AND_SET_NULL(sProto.encoded);
    memset(&sProto, 0, sizeof(sProto));
    sProto.config = config;
}

void GcProtocol_SetMode(const ModeInfo *info, const u8 *request)
{
    sProto.mode = *info;
    sProto.modeRequest = request;
    if (request == NULL)
        sProto.mode.request_length = 0;
    sProto.mode.request_crc = sProto.mode.request_length ? ~wire_crc(UINT32_MAX, request, sProto.mode.request_length) : 0;
}

void GcProtocol_Free(void)
{
    TRY_FREE_AND_SET_NULL(sProto.encoded);
}

void GcProtocol_ResetSession(void)
{
    sProto.versionMismatch = FALSE;
    sProto.haveCaps = FALSE;
    sProto.assetOpen = FALSE;
}

u32 GcProtocol_TakeEvents(void)
{
    u32 events = sProto.events;
    sProto.events = 0;
    return events;
}

const Caps *GcProtocol_GetHostCaps(void)
{
    return sProto.haveCaps ? &sProto.hostCaps : NULL;
}

u16 GcProtocol_GetRelease(void)
{
    return gGcExtensionRelease;
}

bool8 GcProtocol_IsVersionMismatch(void)
{
    return sProto.versionMismatch;
}

const struct GcProtocolStats *GcProtocol_GetStats(void)
{
    return &sProto.stats;
}

static bool HandlePing(const Frame *request, Frame *reply)
{
    const struct GcLinkStats *stats = GcLink_GetStats();
    if (request->len != 4)
        return false;
    memcpy(reply->payload, request->payload, 4);
    wire_put32(reply->payload + 4, stats->resets);
    wire_put32(reply->payload + 8, stats->overflows);
    wire_put32(reply->payload + 12, stats->crcErrors);
    wire_put32(reply->payload + 16, stats->duplicates);
    reply->len = 20;
    return true;
}

static bool HandleCaps(const Frame *request, Frame *reply)
{
    const struct GcClientConfig *config = sProto.config;
    Caps caps;
    if (!caps_decode(request->payload, request->len, &caps) || caps.version < MSG_PROTOCOL_MIN_VERSION)
        return false;
    sProto.hostCaps = caps;
    sProto.haveCaps = TRUE;
    caps.version = MSG_PROTOCOL_VERSION;
    caps.peer_type = config->peerType;
    caps.build_id = config->buildId;
    caps.caps = CAP_APP | CAP_ASSETS | CAP_BYE | CAP_MODE | CAP_RESULT;
    caps.max_payload = WIRE_PAYLOAD;
    caps.app_id = config->appId;
    caps.app_version = config->appVersion;
    caps.release = gGcExtensionRelease;
    memcpy(caps.tag, config->tag, 4);
    reply->len = caps_encode(&caps, reply->payload);
    sProto.events |= GC_PROTO_EVENT_CAPS;
    sProto.versionMismatch = sProto.hostCaps.release != gGcExtensionRelease;
    if (sProto.versionMismatch)
        sProto.events |= GC_PROTO_EVENT_VERSION;
    return true;
}

static bool HandleApp(const Frame *request, Frame *reply)
{
    const struct GcClientConfig *config = sProto.config;
    u16 appId, opcode, status, length = 0;
    if (!sProto.haveCaps || request->len < APP_REQUEST_HEADER)
        return false;
    appId = msg_get16(request->payload);
    opcode = msg_get16(request->payload + 2);
    if (appId == config->appId && config->handleApp != NULL)
        status = config->handleApp(opcode, request->payload + APP_REQUEST_HEADER,
                                   request->len - APP_REQUEST_HEADER,
                                   reply->payload + APP_REPLY_HEADER, &length);
    else if (appId != 0 && appId == config->modeAppId && config->handleModeApp != NULL)
        status = config->handleModeApp(opcode, request->payload + APP_REQUEST_HEADER,
                                       request->len - APP_REQUEST_HEADER,
                                       reply->payload + APP_REPLY_HEADER, &length);
    else
        status = APP_UNKNOWN_OP;
    if (length > APP_MAX_REPLY_DATA)
        return false;
    msg_put16(reply->payload, appId);
    msg_put16(reply->payload + 2, opcode);
    msg_put16(reply->payload + 4, status);
    msg_put16(reply->payload + 6, 0);
    reply->len = APP_REPLY_HEADER + length;
    sProto.stats.appRequests++;
    return true;
}

static bool HandleAssetInfo(const Frame *request, Frame *reply)
{
    AssetInfo info = {0};
    const u8 *data = NULL;
    if (!sProto.haveCaps || !asset_key_decode(request->payload, request->len, &info.key))
        return false;
    sProto.assetOpen = FALSE;
    if (info.key.type == ASSET_MODE_REQUEST)
    {
        // The mode request is served by the connector itself.
        if (sProto.modeRequest != NULL && sProto.mode.request_length != 0 && info.key.id == sProto.mode.mode_id
         && info.key.variant == 0 && info.key.extra == sProto.mode.nonce)
        {
            data = sProto.modeRequest;
            info.length = sProto.mode.request_length;
            info.pixel_format = PIXEL_NONE;
            info.palette_format = PALETTE_NONE;
        }
    }
    else if (sProto.config->resolveAsset != NULL && !sProto.config->resolveAsset(&info.key, &info, &data))
    {
        data = NULL;
    }
    if (data == NULL)
    {
        if (info.status == ASSET_OK)
            info.status = ASSET_NOT_FOUND;
    }
    else
    {
        info.status = ASSET_OK;
        info.encoding = ASSET_ENCODING_RAW;
        info.raw_length = info.length;
        TRY_FREE_AND_SET_NULL(sProto.encoded);
        if ((sProto.hostCaps.caps & CAP_RLE) && info.length > 64)
        {
            sProto.encoded = AllocUnchecked(RLE_WORST_CASE(info.length));
            if (sProto.encoded != NULL)
            {
                u32 packed = rle_encode(data, info.length, sProto.encoded, RLE_WORST_CASE(info.length));
                if (packed != 0 && packed < info.length)
                {
                    info.encoding = ASSET_ENCODING_RLE;
                    info.length = packed;
                    data = sProto.encoded;
                }
            }
        }
        info.crc = ~wire_crc(UINT32_MAX, data, info.length);
        sProto.assetOpen = TRUE;
        sProto.openKey = info.key;
        sProto.openData = data;
        sProto.openLength = info.length;
        sProto.stats.assetsOpened++;
        sProto.events |= GC_PROTO_EVENT_ASSET;
    }
    asset_key_decode(request->payload, request->len, &info.key); // echo the requested key
    reply->len = asset_info_encode(&info, reply->payload);
    return true;
}

static bool HandleAssetChunk(const Frame *request, Frame *reply)
{
    AssetKey key;
    u32 offset, length;
    if (!sProto.assetOpen || request->len != ASSET_CHUNK_REQUEST_BYTES
     || !asset_key_decode(request->payload, request->len, &key) || !asset_key_equal(&key, &sProto.openKey))
        return false;
    offset = wire_get32(request->payload + ASSET_KEY_BYTES);
    length = wire_get32(request->payload + ASSET_KEY_BYTES + 4);
    if (length == 0 || length > WIRE_PAYLOAD || offset > sProto.openLength || length > sProto.openLength - offset)
        return false;
    memcpy(reply->payload, sProto.openData + offset, length);
    reply->len = length;
    sProto.stats.assetBytesSent += length;
    return true;
}

static bool HandleResult(const Frame *request, Frame *reply)
{
    u16 status, modeId, version;
    u32 resultId;
    if (!sProto.haveCaps || request->len < RESULT_HEADER_BYTES)
        return false;
    modeId = msg_get16(request->payload);
    version = msg_get16(request->payload + 2);
    resultId = wire_get32(request->payload + 4);
    if (sProto.haveResult && resultId == sProto.lastResultId)
        status = RESULT_DUPLICATE; // never applied twice, even across reconnects
    else if (modeId != sProto.mode.mode_id || sProto.config->handleResult == NULL)
        status = RESULT_UNSUPPORTED;
    else
        status = sProto.config->handleResult(modeId, version, request->payload + RESULT_HEADER_BYTES,
                                             request->len - RESULT_HEADER_BYTES);
    if (status == RESULT_ACCEPTED)
    {
        sProto.haveResult = TRUE;
        sProto.lastResultId = resultId;
        sProto.events |= GC_PROTO_EVENT_RESULT;
    }
    msg_put16(reply->payload, status);
    msg_put16(reply->payload + 2, 0);
    reply->len = 4;
    return true;
}

bool GcProtocol_HandleFrame(const Frame *request, Frame *reply)
{
    switch (request->type)
    {
    case HELLO:
        if (request->len != 0 || request->seq != 1)
            return false;
        GcProtocol_ResetSession();
        sProto.events |= GC_PROTO_EVENT_HELLO;
        return true;
    case PING:
        return HandlePing(request, reply);
    case CAPS:
        return HandleCaps(request, reply);
    case APP:
        return HandleApp(request, reply);
    case ASSET_INFO:
        return HandleAssetInfo(request, reply);
    case ASSET_CHUNK:
        return HandleAssetChunk(request, reply);
    case BYE:
        sProto.events |= GC_PROTO_EVENT_BYE;
        return true;
    case MODE:
        if (!sProto.haveCaps || request->len != 0 || sProto.mode.mode_id == GC_MODE_NONE)
            return false;
        if (sProto.versionMismatch && !gcx_release_compatible(gGcExtensionRelease, sProto.hostCaps.release))
        {
            // Refuse to run anything with a host of another release: announce no mode.
            ModeInfo none = {0};
            reply->len = mode_info_encode(&none, reply->payload);
            return true;
        }
        reply->len = mode_info_encode(&sProto.mode, reply->payload);
        sProto.events |= GC_PROTO_EVENT_MODE;
        return true;
    case RESULT:
        return HandleResult(request, reply);
    default:
        return false; // Phase 1 benchmark/bulk frames are not served by the game.
    }
}
