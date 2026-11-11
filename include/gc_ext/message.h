// Vendored from gba-gc-extension protocol/message.h. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#ifndef GCEXT_MESSAGE_H
#define GCEXT_MESSAGE_H

// Generic message layer on top of the reliable frame codec (wire.h).
// Nothing in here knows about a particular game. All multi-byte fields are
// little-endian; every struct has an explicit encoded size and is converted
// field by field, never copied from memory.

#include <stdbool.h>
#include <stdint.h>
#include "gc_ext/wire.h"
#ifdef __cplusplus
extern "C" {
#endif

// Version of the message layer. The Phase 1 PoC set (HELLO/PING/BULK) is
// implicitly version 1. Version 2 added CAPS/APP/ASSET/BYE; version 3 adds
// MODE and RESULT. Peers accept any version >= MSG_PROTOCOL_MIN_VERSION and
// only use frames both sides advertise in their capability bits.
#define MSG_PROTOCOL_VERSION 3
#define MSG_PROTOCOL_MIN_VERSION 2

enum PeerType {
    PEER_UNKNOWN = 0,
    PEER_POC_CLIENT = 1,              // gba/main.cpp test client
    PEER_POKEEMERALD_EXPANSION = 2,   // pokeemerald-expansion GC Extension Mode
    PEER_GC_HOST = 0x100,             // gamecube/companion host
};

// Capability bits (Caps.caps).
#define CAP_APP     (1u << 0)  // APP frames
#define CAP_ASSETS  (1u << 1)  // ASSET_INFO / ASSET_CHUNK
#define CAP_BYE     (1u << 2)  // BYE frames
#define CAP_MODE    (1u << 3)  // MODE frame + mode request asset
#define CAP_RESULT  (1u << 4)  // RESULT frames
#define CAP_RLE     (1u << 5)  // host decodes ASSET_ENCODING_RLE asset data

// CAPS request (host -> client) and its ACK (client -> host) share this layout.
//   0 u16 msg protocol version   2 u16 peer type
//   4 u32 build id               8 u32 capability bits
//  12 u16 max frame payload     14 u16 application id (0 = none)
//  16 u16 application version   18 u16 reserved (0)
//  20 u8[4] tag (client: game code, e.g. "BPEE"; host: "GCXH")
#define CAPS_BYTES 24
typedef struct {
    uint16_t version, peer_type;
    uint32_t build_id, caps;
    uint16_t max_payload, app_id, app_version;
    uint8_t tag[4];
} Caps;

// APP request: u16 app id, u16 opcode, then opcode-specific arguments.
// APP ACK: u16 app id, u16 opcode, u16 status, u16 reserved, then data.
#define APP_REQUEST_HEADER 4
#define APP_REPLY_HEADER 8
#define APP_MAX_REPLY_DATA (WIRE_PAYLOAD - APP_REPLY_HEADER)
enum AppStatus { APP_OK = 0, APP_UNKNOWN_OP = 1, APP_BAD_ARGS = 2, APP_UNAVAILABLE = 3 };

// Asset identity. The client resolves the key to final, ready-to-use data;
// the host never receives ROM addresses.
//   0 u16 type   2 u16 reserved   4 u32 id   8 u32 variant   12 u32 extra
#define ASSET_KEY_BYTES 16
typedef struct {
    uint16_t type;
    uint32_t id, variant, extra;
} AssetKey;

enum PixelFormat {
    PIXEL_NONE = 0,
    PIXEL_4BPP_TILED = 1,    // 8x8 tiles, row-major, low nibble = left pixel
    PIXEL_4BPP_TILEMAP = 2,  // a GBA text BG: see TILEMAP_HEADER_BYTES
};
// PIXEL_4BPP_TILEMAP data (palette included, palette format NONE):
//   0 u16 map width (tiles)  2 u16 map height  4 u16 tile count  6 u8 palette banks
//   7 u8 flags (bit0: colour 0 is opaque, drawn with its bank's colour 0)
//   8 u16 map[w*h] (GBA screen entries: bits 0-9 tile, 10 hflip, 11 vflip, 12-15 bank)
//   .. tiles (tile count x 32 bytes), then banks x 16 BGR555 colours
#define TILEMAP_HEADER_BYTES 8
#define TILEMAP_FLAG_OPAQUE (1u << 0)
enum PaletteFormat { PALETTE_NONE = 0, PALETTE_BGR555_16 = 1 }; // 16 x u16 appended after the pixels

// ASSET_INFO request: AssetKey. ACK:
//   0 u16 status (AssetStatus)  2 u16 reserved
//   4 AssetKey (echo)          20 u16 width   22 u16 height
//  24 u8 pixel format          25 u8 palette format  26 u8 frames  27 u8 encoding
//  28 u32 data length as transferred              32 u32 CRC-32 of the transferred data
//  36 u32 decoded length (version 3; older clients send 36 bytes = not encoded)
// Clients only encode when the host advertises CAP_RLE.
#define ASSET_INFO_BYTES 40
#define ASSET_INFO_BYTES_V2 36
enum AssetEncoding { ASSET_ENCODING_RAW = 0, ASSET_ENCODING_RLE = 1 };
enum AssetStatus { ASSET_OK = 0, ASSET_NOT_FOUND = 1, ASSET_TOO_LARGE = 2 };
typedef struct {
    uint16_t status;
    AssetKey key;
    uint16_t width, height;
    uint8_t pixel_format, palette_format, frames, encoding;
    uint32_t length, crc, raw_length;
} AssetInfo;

// ASSET_CHUNK request: AssetKey, u32 offset, u32 length (<= WIRE_PAYLOAD).
// The ACK payload is exactly the requested bytes of the asset that the last
// ASSET_INFO opened. Chunks are idempotent, so retransmission is harmless.
#define ASSET_CHUNK_REQUEST_BYTES (ASSET_KEY_BYTES + 8)

// BYE request: u16 reason. Empty ACK. Ends the session cleanly.
enum ByeReason { BYE_HOST_EXIT = 1, BYE_CLIENT_EXIT = 2 };

// Modes. The client (GBA) decides which mode a session runs; the host asks
// with MODE after CAPS. Mode ids are a registry shared by all clients; what a
// mode does is defined by its own application protocol (APP id in the reply).
enum GcModeId {
    GC_MODE_NONE = 0,
    GC_MODE_PARTY_VIEW = 1,  // Phase 2 party viewer (pkmn_protocol.h)
    GC_MODE_BATTLE = 2,      // battle runtime (battle_protocol.h)
};

// MODE request: empty. ACK (24 bytes):
//   0 u16 mode id        2 u16 mode version   4 u16 mode app id   6 u16 mode app version
//   8 u32 request length (0 = none)          12 u32 CRC-32 of the request
//  16 u32 session nonce (changes for every mode start)               20 u32 flags (0)
// The request bytes are fetched as an asset with key
// {ASSET_MODE_REQUEST, mode id, 0, session nonce} through ASSET_INFO/ASSET_CHUNK.
#define MODE_INFO_BYTES 24
#define ASSET_MODE_REQUEST 0x0001
typedef struct {
    uint16_t mode_id, mode_version, app_id, app_version;
    uint32_t request_length, request_crc, nonce, flags;
} ModeInfo;

// RESULT request (host -> client): the outcome of a mode, versioned by the mode.
//   0 u16 mode id   2 u16 result version   4 u32 result id (unique per mode start)
//   8 ..            result payload (<= RESULT_MAX_PAYLOAD bytes)
// ACK: u16 ResultStatus, u16 0. A client accepts a given result id at most
// once; a repeated id is acknowledged as RESULT_DUPLICATE and not re-applied.
#define RESULT_HEADER_BYTES 8
#define RESULT_MAX_PAYLOAD (WIRE_PAYLOAD - RESULT_HEADER_BYTES)
enum ResultStatus { RESULT_ACCEPTED = 0, RESULT_DUPLICATE = 1, RESULT_REJECTED = 2, RESULT_UNSUPPORTED = 3 };

static inline uint16_t msg_get16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }
static inline void msg_put16(uint8_t *p, uint16_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }

unsigned caps_encode(const Caps *caps, uint8_t *out);
bool caps_decode(const uint8_t *in, unsigned length, Caps *caps);
unsigned asset_key_encode(const AssetKey *key, uint8_t *out);
bool asset_key_decode(const uint8_t *in, unsigned length, AssetKey *key);
bool asset_key_equal(const AssetKey *a, const AssetKey *b);
unsigned mode_info_encode(const ModeInfo *info, uint8_t *out);
bool mode_info_decode(const uint8_t *in, unsigned length, ModeInfo *info);
unsigned asset_info_encode(const AssetInfo *info, uint8_t *out);
bool asset_info_decode(const uint8_t *in, unsigned length, AssetInfo *info);
uint32_t asset_palette_bytes(uint8_t palette_format);

// Byte RLE: control c < 128 -> c+1 literal bytes follow; c >= 128 -> the next
// byte repeats c-125 times (3..130). Returns the encoded/decoded length, or 0
// if it does not fit (encode) / the stream is malformed (decode).
uint32_t rle_encode(const uint8_t *in, uint32_t length, uint8_t *out, uint32_t capacity);
uint32_t rle_decode(const uint8_t *in, uint32_t length, uint8_t *out, uint32_t capacity);
#define RLE_WORST_CASE(n) ((n) + (n) / 128 + 2)

#ifdef __cplusplus
}
#endif
#endif
