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

// Version of the message layer (CAPS/APP/ASSET/BYE). The Phase 1 PoC set
// (HELLO/PING/BULK) is implicitly version 1 and has no CAPS message.
#define MSG_PROTOCOL_VERSION 2

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

enum PixelFormat { PIXEL_NONE = 0, PIXEL_4BPP_TILED = 1 };   // 8x8 tiles, row-major, low nibble = left pixel
enum PaletteFormat { PALETTE_NONE = 0, PALETTE_BGR555_16 = 1 }; // 16 x u16 appended after the pixels

// ASSET_INFO request: AssetKey. ACK:
//   0 u16 status (AssetStatus)  2 u16 reserved
//   4 AssetKey (echo)          20 u16 width   22 u16 height
//  24 u8 pixel format          25 u8 palette format  26 u8 frames  27 u8 reserved
//  28 u32 data length (pixels + palette)          32 u32 CRC-32 of the data
#define ASSET_INFO_BYTES 36
enum AssetStatus { ASSET_OK = 0, ASSET_NOT_FOUND = 1, ASSET_TOO_LARGE = 2 };
typedef struct {
    uint16_t status;
    AssetKey key;
    uint16_t width, height;
    uint8_t pixel_format, palette_format, frames;
    uint32_t length, crc;
} AssetInfo;

// ASSET_CHUNK request: AssetKey, u32 offset, u32 length (<= WIRE_PAYLOAD).
// The ACK payload is exactly the requested bytes of the asset that the last
// ASSET_INFO opened. Chunks are idempotent, so retransmission is harmless.
#define ASSET_CHUNK_REQUEST_BYTES (ASSET_KEY_BYTES + 8)

// BYE request: u16 reason. Empty ACK. Ends the session cleanly.
enum ByeReason { BYE_HOST_EXIT = 1, BYE_CLIENT_EXIT = 2 };

static inline uint16_t msg_get16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }
static inline void msg_put16(uint8_t *p, uint16_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); }

unsigned caps_encode(const Caps *caps, uint8_t *out);
bool caps_decode(const uint8_t *in, unsigned length, Caps *caps);
unsigned asset_key_encode(const AssetKey *key, uint8_t *out);
bool asset_key_decode(const uint8_t *in, unsigned length, AssetKey *key);
bool asset_key_equal(const AssetKey *a, const AssetKey *b);
unsigned asset_info_encode(const AssetInfo *info, uint8_t *out);
bool asset_info_decode(const uint8_t *in, unsigned length, AssetInfo *info);
uint32_t asset_palette_bytes(uint8_t palette_format);

#ifdef __cplusplus
}
#endif
#endif
