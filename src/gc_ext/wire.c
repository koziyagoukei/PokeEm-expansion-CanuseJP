// Vendored from gba-gc-extension protocol/wire.c. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#include "gc_ext/wire.h"

uint32_t wire_get32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1]<<8 | (uint32_t)p[2]<<16 | (uint32_t)p[3]<<24;
}

void wire_put32(uint8_t *p, uint32_t w) {
    for (unsigned i = 0; i < 4; ++i) p[i] = (uint8_t)(w >> (8*i));
}

uint32_t wire_crc(uint32_t state, const uint8_t *data, unsigned length) {
    for (unsigned i = 0; i < length; ++i) {
        state ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            state = (state >> 1) ^ (UINT32_C(0xedb88320) & (0u - (state & 1u)));
    }
    return state;
}

static uint32_t frame_crc(const uint32_t *words, unsigned bytes) {
    uint32_t state = UINT32_MAX;
    for (unsigned i = 0; i < 3; ++i) {
        uint8_t part[4];
        wire_put32(part, words[i]);
        state = wire_crc(state, part, 4);
    }
    for (unsigned i = 0; i < bytes; ++i) {
        const uint8_t byte = (uint8_t)(words[4+i/4] >> (8*(i%4)));
        state = wire_crc(state, &byte, 1);
    }
    return ~state;
}

unsigned wire_encode(const Frame *f, uint32_t w[WIRE_WORDS]) {
    if (f->len > WIRE_PAYLOAD || f->type < HELLO || f->type > WIRE_TYPE_LAST) return 0;
    w[0] = WIRE_MAGIC;
    w[1] = (uint32_t)f->len<<16 | (uint32_t)f->type<<8 | WIRE_VERSION;
    w[2] = f->seq;
    for (unsigned i = 0; i < (f->len+3u)/4; ++i) w[4+i] = 0;
    for (unsigned i = 0; i < f->len; ++i) w[4+i/4] |= (uint32_t)f->payload[i] << (8*(i%4));
    w[3] = frame_crc(w, f->len);
    return 4 + (f->len+3u)/4;
}

// Checks the header word of the buffered candidate and sets its length.
static bool header_ok(Parser *p) {
    const uint32_t word = p->words[1];
    const unsigned len = word >> 16;
    const unsigned type = (word >> 8) & 255;
    if ((word & 255) != WIRE_VERSION || type < HELLO || type > WIRE_TYPE_LAST || len > WIRE_PAYLOAD) return false;
    p->target = 4 + (len+3)/4;
    return true;
}

// A candidate failed: keep the buffered words from the next magic on (the
// start of a retransmission that arrived after a lost word), else nothing.
static void resync(Parser *p) {
    unsigned i = 1;
    while (i < p->count && p->words[i] != WIRE_MAGIC) ++i;
    const unsigned keep = p->count - i;
    for (unsigned k = 0; k < keep; ++k) p->words[k] = p->words[i + k];
    p->count = keep;
    if (keep) ++p->resyncs;
}

// Frames are found by their magic word and confirmed only by a valid header,
// length and CRC, so a magic value inside a payload cannot pass as a frame.
// After a failure the parser re-checks any later magic it already holds, so
// a frame that lost a word does not swallow the start of the next one.
// BAD_HEADER / BAD_CRC with count > 0 means a new candidate is in progress.
enum ParseResult wire_feed(Parser *p, uint32_t word, Frame *f) {
    if (p->count == 0 && word != WIRE_MAGIC) return INCOMPLETE;
    p->words[p->count++] = word;
    enum ParseResult failure = INCOMPLETE;
    for (;;) {
        if (p->count >= 2 && !header_ok(p)) {
            failure = BAD_HEADER;
            resync(p);
            continue;
        }
        if (p->count < 4 || p->count < p->target) return failure;
        const unsigned len = p->words[1] >> 16;
        if (frame_crc(p->words, len) != p->words[3]) {
            failure = BAD_CRC;
            resync(p);
            continue;
        }
        p->count = 0;
        f->len = (uint16_t)len;
        f->type = (uint8_t)(p->words[1] >> 8);
        f->seq = p->words[2];
        for (unsigned i = 0; i < len; ++i)
            f->payload[i] = (uint8_t)(p->words[4+i/4] >> (8*(i%4)));
        return FRAME_OK;
    }
}
