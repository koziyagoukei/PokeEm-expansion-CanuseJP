// Vendored from gba-gc-extension protocol/pkmn_protocol.c. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#include "gc_ext/pkmn_protocol.h"

unsigned pkmn_record_encode(const PokemonRecord *r, uint8_t *p) {
    for (unsigned i = 0; i < PKMN_RECORD_BYTES; ++i) p[i] = 0;
    p[0] = PKMN_RECORD_VERSION;
    p[1] = r->slot;
    msg_put16(p+2, PKMN_RECORD_BYTES);
    wire_put32(p+4, r->personality);
    wire_put32(p+8, r->ot_id);
    msg_put16(p+12, r->species);
    msg_put16(p+14, r->national_dex);
    p[16] = r->form;
    p[17] = r->level;
    p[18] = r->nature;
    p[19] = r->tera_type;
    p[20] = r->gender;
    p[21] = r->flags;
    p[22] = r->ability_slot;
    p[23] = r->friendship;
    const uint16_t stats[] = {r->hp, r->max_hp, r->attack, r->defense, r->speed, r->sp_attack, r->sp_defense};
    for (unsigned i = 0; i < 7; ++i) msg_put16(p+24+2*i, stats[i]);
    msg_put16(p+38, r->ability);
    msg_put16(p+40, r->held_item);
    wire_put32(p+44, r->status);
    for (unsigned i = 0; i < 4; ++i) {
        msg_put16(p+48+2*i, r->moves[i]);
        p[56+i] = r->pp[i];
        p[60+i] = r->max_pp[i];
    }
    p[64] = r->language;
    p[65] = r->encoding;
    p[66] = r->nickname_length > PKMN_NICKNAME_BYTES ? PKMN_NICKNAME_BYTES : r->nickname_length;
    for (unsigned i = 0; i < PKMN_NICKNAME_BYTES; ++i) p[68+i] = i < p[66] ? r->nickname[i] : 0xff;
    asset_key_encode(&r->sprite, p+84);
    return PKMN_RECORD_BYTES;
}

bool pkmn_record_decode(const uint8_t *p, unsigned length, PokemonRecord *r) {
    if (length < PKMN_RECORD_BYTES || p[0] != PKMN_RECORD_VERSION || msg_get16(p+2) != PKMN_RECORD_BYTES ||
        p[66] > PKMN_NICKNAME_BYTES)
        return false;
    r->version = p[0];
    r->slot = p[1];
    r->length = msg_get16(p+2);
    r->personality = wire_get32(p+4);
    r->ot_id = wire_get32(p+8);
    r->species = msg_get16(p+12);
    r->national_dex = msg_get16(p+14);
    r->form = p[16];
    r->level = p[17];
    r->nature = p[18];
    r->tera_type = p[19];
    r->gender = p[20];
    r->flags = p[21];
    r->ability_slot = p[22];
    r->friendship = p[23];
    uint16_t *stats[] = {&r->hp, &r->max_hp, &r->attack, &r->defense, &r->speed, &r->sp_attack, &r->sp_defense};
    for (unsigned i = 0; i < 7; ++i) *stats[i] = msg_get16(p+24+2*i);
    r->ability = msg_get16(p+38);
    r->held_item = msg_get16(p+40);
    r->status = wire_get32(p+44);
    for (unsigned i = 0; i < 4; ++i) {
        r->moves[i] = msg_get16(p+48+2*i);
        r->pp[i] = p[56+i];
        r->max_pp[i] = p[60+i];
    }
    r->language = p[64];
    r->encoding = p[65];
    r->nickname_length = p[66];
    for (unsigned i = 0; i < PKMN_NICKNAME_BYTES; ++i) r->nickname[i] = p[68+i];
    asset_key_decode(p+84, ASSET_KEY_BYTES, &r->sprite);
    return true;
}

unsigned party_ui_encode(const PartyUi *u, bool party_info, uint8_t *p) {
    if (party_info) {
        p[0] = u->count;
        p[1] = u->record_version;
        msg_put16(p+2, u->record_length);
        p[4] = u->cursor;
        p[5] = u->confirmed;
        p[6] = u->screen;
        p[7] = u->flags;
        return PARTY_INFO_BYTES;
    }
    p[0] = u->cursor;
    p[1] = u->confirmed;
    p[2] = u->screen;
    p[3] = u->flags;
    msg_put16(p+4, u->input_seq);
    msg_put16(p+6, 0);
    return UI_STATE_BYTES;
}

bool party_ui_decode(const uint8_t *p, unsigned length, bool party_info, PartyUi *u) {
    if (party_info) {
        if (length < PARTY_INFO_BYTES) return false;
        u->count = p[0];
        u->record_version = p[1];
        u->record_length = msg_get16(p+2);
        u->cursor = p[4];
        u->confirmed = p[5];
        u->screen = p[6];
        u->flags = p[7];
        return true;
    }
    if (length < UI_STATE_BYTES) return false;
    u->cursor = p[0];
    u->confirmed = p[1];
    u->screen = p[2];
    u->flags = p[3];
    u->input_seq = msg_get16(p+4);
    return true;
}

AssetKey pkmn_nickname_label(const PokemonRecord *r) {
    AssetKey key = {ASSET_TEXT_LABEL, LABEL_ID(LABEL_NICKNAME, r->slot), 0, r->personality};
    key.variant = ~wire_crc(UINT32_MAX, r->nickname, PKMN_NICKNAME_BYTES);
    return key;
}
