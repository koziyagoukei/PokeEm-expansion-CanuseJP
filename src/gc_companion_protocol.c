// Generic message layer for GC Extension Mode. Knows frames and messages,
// not Pokemon: game data arrives through GcClientConfig callbacks.
#include "global.h"
#include "gc_companion_link.h"
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
};

static EWRAM_DATA struct GcProtocolState sProto = {0};

void GcProtocol_Init(const struct GcClientConfig *config)
{
    memset(&sProto, 0, sizeof(sProto));
    sProto.config = config;
}

void GcProtocol_ResetSession(void)
{
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
    if (!caps_decode(request->payload, request->len, &caps) || caps.version < MSG_PROTOCOL_VERSION)
        return false;
    sProto.hostCaps = caps;
    sProto.haveCaps = TRUE;
    caps.version = MSG_PROTOCOL_VERSION;
    caps.peer_type = config->peerType;
    caps.build_id = config->buildId;
    caps.caps = CAP_APP | CAP_ASSETS | CAP_BYE;
    caps.max_payload = WIRE_PAYLOAD;
    caps.app_id = config->appId;
    caps.app_version = config->appVersion;
    memcpy(caps.tag, config->tag, 4);
    reply->len = caps_encode(&caps, reply->payload);
    sProto.events |= GC_PROTO_EVENT_CAPS;
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
    if (appId != config->appId || config->handleApp == NULL)
        status = APP_UNKNOWN_OP;
    else
        status = config->handleApp(opcode, request->payload + APP_REQUEST_HEADER,
                                   request->len - APP_REQUEST_HEADER,
                                   reply->payload + APP_REPLY_HEADER, &length);
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
    if (sProto.config->resolveAsset == NULL || !sProto.config->resolveAsset(&info.key, &info, &data))
    {
        if (info.status == ASSET_OK)
            info.status = ASSET_NOT_FOUND;
    }
    else
    {
        info.status = ASSET_OK;
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
    default:
        return false; // Phase 1 benchmark/bulk frames are not served by the game.
    }
}
