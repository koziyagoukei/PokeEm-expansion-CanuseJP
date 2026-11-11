// Vendored from gba-gc-extension protocol/wire.h. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#ifndef WIRE_H
#define WIRE_H

#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define WIRE_MAGIC UINT32_C(0x31434247)
#define WIRE_VERSION 1
#define WIRE_PAYLOAD 224
#define WIRE_WORDS ((16 + WIRE_PAYLOAD) / 4)

// Frame types. 1-7 are the Phase 1 PoC set; 8-14 are the generic message layer
// (protocol/message.h). The frame format itself is unchanged (WIRE_VERSION 1).
enum { HELLO = 1, ACK, NACK, PING, BULK_BEGIN, BULK_DATA, BULK_END,
       CAPS, APP, ASSET_INFO, ASSET_CHUNK, BYE, MODE, RESULT, WIRE_TYPE_LAST = RESULT };
typedef struct {
    uint32_t seq;
    uint16_t len;
    uint8_t type;
    uint8_t payload[WIRE_PAYLOAD];
} Frame;

typedef struct {
    uint32_t words[WIRE_WORDS];
    unsigned count;
    unsigned target;
} Parser;

enum ParseResult { INCOMPLETE, FRAME_OK, BAD_HEADER, BAD_CRC };

uint32_t wire_get32(const uint8_t *bytes);
void wire_put32(uint8_t *bytes, uint32_t value);
uint32_t wire_crc(uint32_t state, const uint8_t *data, unsigned length);
unsigned wire_encode(const Frame *frame, uint32_t words[WIRE_WORDS]);
enum ParseResult wire_feed(Parser *parser, uint32_t word, Frame *out);
#ifdef __cplusplus
}
#endif
#endif
