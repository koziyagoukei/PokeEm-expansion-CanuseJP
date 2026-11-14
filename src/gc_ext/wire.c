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

enum ParseResult wire_feed(Parser *p, uint32_t word, Frame *f) {
    if (p->count == 0 && word != WIRE_MAGIC) return INCOMPLETE;
    p->words[p->count++] = word;
    if (p->count == 2) {
        const unsigned len = word >> 16;
        const unsigned type = (word >> 8) & 255;
        if ((word & 255) != WIRE_VERSION || type < HELLO || type > WIRE_TYPE_LAST || len > WIRE_PAYLOAD) {
            p->count = 0;
            return BAD_HEADER;
        }
        p->target = 4 + (len+3)/4;
    }
    if (p->count < 4 || p->count < p->target) return INCOMPLETE;
    p->count = 0;
    const unsigned len = p->words[1] >> 16;
    if (frame_crc(p->words, len) != p->words[3]) return BAD_CRC;
    f->len = (uint16_t)len;
    f->type = (uint8_t)(p->words[1] >> 8);
    f->seq = p->words[2];
    for (unsigned i = 0; i < len; ++i)
        f->payload[i] = (uint8_t)(p->words[4+i/4] >> (8*(i%4)));
    return FRAME_OK;
}
