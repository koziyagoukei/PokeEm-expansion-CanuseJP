// Vendored from gba-gc-extension protocol/battle_protocol.c. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#include "gc_ext/battle_protocol.h"

static void move_encode(const BattleMove *m, uint8_t *p) {
    msg_put16(p, m->power);
    p[2] = m->type;
    p[3] = m->category;
    p[4] = m->accuracy;
    p[5] = (uint8_t)m->priority;
    p[6] = m->target;
    p[7] = m->unsupported;
}

static void move_decode(const uint8_t *p, BattleMove *m) {
    m->power = msg_get16(p);
    m->type = p[2];
    m->category = p[3];
    m->accuracy = p[4];
    m->priority = (int8_t)p[5];
    m->target = p[6];
    m->unsupported = p[7];
}

static void mon_encode(const BattleMon *m, uint8_t *p) {
    for (unsigned i = 0; i < BATTLE_MON_BYTES; ++i) p[i] = 0;
    p[0] = BATTLE_MON_VERSION;
    p[1] = m->type_count;
    p[2] = m->types[0];
    p[3] = m->types[1];
    for (unsigned i = 0; i < 4; ++i) move_encode(&m->moves[i], p + 8 + 8*i);
}

static bool mon_decode(const uint8_t *p, BattleMon *m) {
    if (p[0] != BATTLE_MON_VERSION || p[1] < 1 || p[1] > 2) return false;
    m->type_count = p[1];
    m->types[0] = p[2];
    m->types[1] = p[3];
    m->front_y = m->back_y = 0;
    m->elevation = 0;
    for (unsigned i = 0; i < 4; ++i) move_decode(p + 8 + 8*i, &m->moves[i]);
    return true;
}

uint32_t battle_request_size(const BattleRequest *r) {
    return BATTLE_REQUEST_HEADER + (uint32_t)(r->player_count + r->opponent_count) * BATTLE_ENTRY_BYTES +
           (uint32_t)r->type_count * r->type_count;
}

uint32_t battle_request_encode(const BattleRequest *r, uint8_t *p, uint32_t capacity) {
    const uint32_t size = battle_request_size(r);
    if (size > capacity || r->player_count > BATTLE_MAX_PER_SIDE || r->opponent_count > BATTLE_MAX_PER_SIDE ||
        r->type_count > BATTLE_MAX_TYPES)
        return 0;
    p[0] = BATTLE_REQUEST_VERSION;
    p[1] = r->format;
    p[2] = r->battle_type;
    p[3] = r->player_count;
    p[4] = r->opponent_count;
    p[5] = r->active_per_side;
    msg_put16(p+6, r->rules);
    msg_put16(p+8, r->trainer_id);
    p[10] = r->trainer_class;
    p[11] = r->reward_mode;
    p[12] = r->result_policy;
    p[13] = r->type_count;
    msg_put16(p+14, BATTLE_ENTRY_BYTES);
    wire_put32(p+16, r->battle_id);
    wire_put32(p+20, r->environment);
    uint8_t *q = p + BATTLE_REQUEST_HEADER;
    for (unsigned side = 0; side < 2; ++side) {
        const BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count; ++i, q += BATTLE_ENTRY_BYTES) {
            pkmn_record_encode(&e[i].record, q);
            mon_encode(&e[i].mon, q + PKMN_RECORD_BYTES);
        }
    }
    for (unsigned i = 0; i < (unsigned)r->type_count * r->type_count; ++i) q[i] = r->type_chart[i];
    return size;
}

bool battle_request_decode(const uint8_t *p, uint32_t length, BattleRequest *r) {
    if (length < BATTLE_REQUEST_HEADER || p[0] != BATTLE_REQUEST_VERSION || msg_get16(p+14) != BATTLE_ENTRY_BYTES)
        return false;
    r->format = p[1];
    r->battle_type = p[2];
    r->player_count = p[3];
    r->opponent_count = p[4];
    r->active_per_side = p[5];
    r->rules = msg_get16(p+6);
    r->trainer_id = msg_get16(p+8);
    r->trainer_class = p[10];
    r->reward_mode = p[11];
    r->result_policy = p[12];
    r->type_count = p[13];
    r->battle_id = wire_get32(p+16);
    r->environment = p[20];
    if (!r->player_count || !r->opponent_count || r->player_count > BATTLE_MAX_PER_SIDE ||
        r->opponent_count > BATTLE_MAX_PER_SIDE || !r->type_count || r->type_count > BATTLE_MAX_TYPES ||
        length != battle_request_size(r))
        return false;
    const uint8_t *q = p + BATTLE_REQUEST_HEADER;
    for (unsigned side = 0; side < 2; ++side) {
        BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count; ++i, q += BATTLE_ENTRY_BYTES)
            if (!pkmn_record_decode(q, PKMN_RECORD_BYTES, &e[i].record) ||
                !mon_decode(q + PKMN_RECORD_BYTES, &e[i].mon))
                return false;
    }
    for (unsigned i = 0; i < (unsigned)r->type_count * r->type_count; ++i) r->type_chart[i] = q[i];
    return true;
}

unsigned battle_state_encode(const BattleState *s, uint8_t *p) {
    p[0] = s->phase;
    p[1] = s->turn;
    p[2] = s->outcome;
    p[3] = s->player_slot;
    msg_put16(p+4, s->player_hp);
    msg_put16(p+6, s->player_max_hp);
    msg_put16(p+8, s->opponent_hp);
    msg_put16(p+10, s->opponent_max_hp);
    for (unsigned i = 0; i < 4; ++i) p[12+i] = s->pp[i];
    return BATTLE_STATE_BYTES;
}

bool battle_state_decode(const uint8_t *p, unsigned length, BattleState *s) {
    if (length < BATTLE_STATE_BYTES || p[0] < BPHASE_WAIT || p[0] > BPHASE_ENDED) return false;
    s->phase = p[0];
    s->turn = p[1];
    s->outcome = p[2];
    s->player_slot = p[3];
    s->player_hp = msg_get16(p+4);
    s->player_max_hp = msg_get16(p+6);
    s->opponent_hp = msg_get16(p+8);
    s->opponent_max_hp = msg_get16(p+10);
    for (unsigned i = 0; i < 4; ++i) s->pp[i] = p[12+i];
    return true;
}

unsigned battle_action_encode(const BattleAction *a, uint8_t *p) {
    p[0] = a->turn;
    p[1] = a->action;
    p[2] = a->move_slot;
    p[3] = a->flags;
    return BATTLE_ACTION_BYTES;
}

bool battle_action_decode(const uint8_t *p, unsigned length, BattleAction *a) {
    if (length < BATTLE_ACTION_BYTES || p[1] > BACTION_FORFEIT || p[2] > 3) return false;
    a->turn = p[0];
    a->action = p[1];
    a->move_slot = p[2];
    a->flags = p[3];
    return true;
}

unsigned battle_multi_state_encode(const BattleMultiState *s, uint8_t *p) {
    p[0] = s->phase;
    p[1] = s->turn;
    p[2] = s->outcome;
    p[3] = s->actor;
    p[4] = s->flags;
    msg_put16(p+5, s->token);
    uint8_t *q = p + 7;
    for (unsigned side = 0; side < 2; ++side)
        for (unsigned i = 0; i < BATTLE_FIELD_SLOTS; ++i, q += 5) {
            const BattleFieldMon *m = &s->field[side][i];
            q[0] = m->party_slot;
            msg_put16(q+1, m->hp);
            msg_put16(q+3, m->max_hp);
        }
    for (unsigned i = 0; i < BATTLE_FIELD_SLOTS; ++i, q += 8)
        for (unsigned m = 0; m < 4; ++m) {
            q[m] = s->pp[i][m];
            q[4+m] = s->targets[i][m];
        }
    return BATTLE_MULTI_STATE_BYTES;
}

bool battle_multi_state_decode(const uint8_t *p, unsigned n, BattleMultiState *s) {
    if (n != BATTLE_MULTI_STATE_BYTES || p[0] < BPHASE_WAIT || p[0] > BPHASE_ENDED ||
        p[2] > OUTCOME_UNSUPPORTED || p[3] >= BATTLE_FIELD_SLOTS ||
        (p[4] & ~BATTLE_MULTI_SHIFT) || (p[0] == BPHASE_CHOOSE && !msg_get16(p+5)))
        return false;
    s->phase = p[0];
    s->turn = p[1];
    s->outcome = p[2];
    s->actor = p[3];
    s->flags = p[4];
    s->token = msg_get16(p+5);
    const uint8_t *q = p + 7;
    for (unsigned side = 0; side < 2; ++side)
        for (unsigned i = 0; i < BATTLE_FIELD_SLOTS; ++i, q += 5) {
            BattleFieldMon *m = &s->field[side][i];
            m->party_slot = q[0];
            m->hp = msg_get16(q+1);
            m->max_hp = msg_get16(q+3);
            if (m->hp > m->max_hp) return false;
        }
    for (unsigned i = 0; i < BATTLE_FIELD_SLOTS; ++i, q += 8)
        for (unsigned m = 0; m < 4; ++m) {
            if (q[4+m] & ~(BATTLE_TARGET_AUTO | 7u)) return false;
            s->pp[i][m] = q[m];
            s->targets[i][m] = q[4+m];
        }
    return true;
}

unsigned battle_multi_action_encode(const BattleMultiAction *a, uint8_t *p) {
    msg_put16(p, a->token);
    p[2] = a->actor;
    p[3] = a->action;
    p[4] = a->move_slot;
    p[5] = a->target;
    p[6] = a->rotation;
    p[7] = a->flags;
    return BATTLE_MULTI_ACTION_BYTES;
}

bool battle_multi_action_decode(const uint8_t *p, unsigned n, BattleMultiAction *a) {
    if (n != BATTLE_MULTI_ACTION_BYTES || p[2] >= BATTLE_FIELD_SLOTS || p[3] > BACTION_SHIFT ||
        p[4] >= 4 || p[5] >= BATTLE_FIELD_SLOTS || p[6] > BROT_RIGHT || (p[7] & ~BACTION_FLAG_EXIT))
        return false;
    a->token = msg_get16(p);
    a->actor = p[2];
    a->action = p[3];
    a->move_slot = p[4];
    a->target = p[5];
    a->rotation = p[6];
    a->flags = p[7];
    return true;
}

unsigned battle_result_encode(const BattleResult *r, uint8_t *p) {
    p[0] = r->outcome;
    p[1] = r->turns;
    p[2] = r->player_count;
    p[3] = r->opponent_count;
    wire_put32(p+4, r->battle_id);
    uint8_t *q = p + 8;
    for (unsigned i = 0; i < r->player_count; ++i, q += 12) {
        q[0] = r->player[i].slot;
        q[1] = r->player[i].flags;
        msg_put16(q+2, r->player[i].hp);
        wire_put32(q+4, r->player[i].status);
        for (unsigned m = 0; m < 4; ++m) q[8+m] = r->player[i].pp[m];
    }
    for (unsigned i = 0; i < r->opponent_count; ++i, q += 2) msg_put16(q, r->opponent_hp[i]);
    return (unsigned)(q - p);
}

bool battle_result_decode(const uint8_t *p, unsigned length, BattleResult *r) {
    if (length < 8) return false;
    r->outcome = p[0];
    r->turns = p[1];
    r->player_count = p[2];
    r->opponent_count = p[3];
    r->battle_id = wire_get32(p+4);
    if (r->outcome < OUTCOME_WIN || r->outcome > OUTCOME_UNSUPPORTED || r->player_count > BATTLE_MAX_PER_SIDE ||
        r->opponent_count > BATTLE_MAX_PER_SIDE || length != 8u + 12u * r->player_count + 2u * r->opponent_count)
        return false;
    const uint8_t *q = p + 8;
    for (unsigned i = 0; i < r->player_count; ++i, q += 12) {
        r->player[i].slot = q[0];
        r->player[i].flags = q[1];
        r->player[i].hp = msg_get16(q+2);
        r->player[i].status = wire_get32(q+4);
        for (unsigned m = 0; m < 4; ++m) r->player[i].pp[m] = q[8+m];
    }
    for (unsigned i = 0; i < r->opponent_count; ++i, q += 2) r->opponent_hp[i] = msg_get16(q);
    return true;
}
