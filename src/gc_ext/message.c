// Vendored from gba-gc-extension protocol/message.c. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#include "gc_ext/message.h"

unsigned caps_encode(const Caps *c, uint8_t *p) {
    msg_put16(p, c->version);
    msg_put16(p+2, c->peer_type);
    wire_put32(p+4, c->build_id);
    wire_put32(p+8, c->caps);
    msg_put16(p+12, c->max_payload);
    msg_put16(p+14, c->app_id);
    msg_put16(p+16, c->app_version);
    msg_put16(p+18, 0);
    for (unsigned i = 0; i < 4; ++i) p[20+i] = c->tag[i];
    return CAPS_BYTES;
}

bool caps_decode(const uint8_t *p, unsigned length, Caps *c) {
    if (length < CAPS_BYTES) return false;
    c->version = msg_get16(p);
    c->peer_type = msg_get16(p+2);
    c->build_id = wire_get32(p+4);
    c->caps = wire_get32(p+8);
    c->max_payload = msg_get16(p+12);
    c->app_id = msg_get16(p+14);
    c->app_version = msg_get16(p+16);
    for (unsigned i = 0; i < 4; ++i) c->tag[i] = p[20+i];
    return true;
}

unsigned asset_key_encode(const AssetKey *k, uint8_t *p) {
    msg_put16(p, k->type);
    msg_put16(p+2, 0);
    wire_put32(p+4, k->id);
    wire_put32(p+8, k->variant);
    wire_put32(p+12, k->extra);
    return ASSET_KEY_BYTES;
}

bool asset_key_decode(const uint8_t *p, unsigned length, AssetKey *k) {
    if (length < ASSET_KEY_BYTES) return false;
    k->type = msg_get16(p);
    k->id = wire_get32(p+4);
    k->variant = wire_get32(p+8);
    k->extra = wire_get32(p+12);
    return true;
}

bool asset_key_equal(const AssetKey *a, const AssetKey *b) {
    return a->type == b->type && a->id == b->id && a->variant == b->variant && a->extra == b->extra;
}

unsigned asset_info_encode(const AssetInfo *i, uint8_t *p) {
    msg_put16(p, i->status);
    msg_put16(p+2, 0);
    asset_key_encode(&i->key, p+4);
    msg_put16(p+20, i->width);
    msg_put16(p+22, i->height);
    p[24] = i->pixel_format;
    p[25] = i->palette_format;
    p[26] = i->frames;
    p[27] = 0;
    wire_put32(p+28, i->length);
    wire_put32(p+32, i->crc);
    return ASSET_INFO_BYTES;
}

bool asset_info_decode(const uint8_t *p, unsigned length, AssetInfo *i) {
    if (length < ASSET_INFO_BYTES) return false;
    i->status = msg_get16(p);
    asset_key_decode(p+4, ASSET_KEY_BYTES, &i->key);
    i->width = msg_get16(p+20);
    i->height = msg_get16(p+22);
    i->pixel_format = p[24];
    i->palette_format = p[25];
    i->frames = p[26];
    i->length = wire_get32(p+28);
    i->crc = wire_get32(p+32);
    return true;
}

uint32_t asset_palette_bytes(uint8_t palette_format) {
    return palette_format == PALETTE_BGR555_16 ? 32u : 0u;
}
