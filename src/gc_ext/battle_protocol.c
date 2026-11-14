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

static void mon_encode(const BattleMon *m, uint8_t *p, bool effects) {
    for (unsigned i = 0; i < BATTLE_MON_BYTES; ++i) p[i] = 0;
    p[0] = BATTLE_MON_VERSION;
    p[1] = m->type_count;
    p[2] = m->types[0];
    p[3] = m->types[1];
    for (unsigned i = 0; i < 4; ++i) {
        if (effects) p[4+i] = m->moves[i].effect;
        move_encode(&m->moves[i], p + 8 + 8*i);
    }
}

bool battle_effect_valid(uint8_t effect, uint8_t version) {
    if (effect == BME_HIT) return true;
    if (version < BATTLE_REQUEST_VERSION_DRAG) return false;
    if (effect <= BME_FORCE_OUT_ON_HIT) return true;
    if (effect == BME_PIVOT) return version >= BATTLE_REQUEST_VERSION_PIVOT;
    if (version < BATTLE_REQUEST_VERSION_CONDITIONS) return false;
    return (effect >= BME_STAGE_BASE && effect < BME_STAGE_BASE + BSTAT_COUNT * 12) ||
           (effect > BME_STATUS_BASE && effect <= BME_STATUS_BASE + BST_FROSTBITE) ||
           effect == BME_REFRESH || effect == BME_ELECTRIC_PARALYSIS;
}

const char *battle_effect_kind_name(uint8_t kind) {
    static const char *const names[] = {"none", "damage", "stat", "status", "cure", "heal", "drain",
                                        "recoil", "force_out", "pivot", "protect", "flinch", "confuse",
                                        "substitute", "charge", "recharge"};
    return kind < BEFF_COUNT ? names[kind] : "invalid";
}

static bool has_effect(const BattleMove *m, uint8_t kind) {
    for (unsigned i = 0; i < m->effect_count; ++i)
        if (m->effects[i].kind == kind) return true;
    return false;
}

bool battle_move_effects_valid(const BattleMove *m) {
    if (m->effect_count > BATTLE_MAX_EFFECTS || (m->move_flags & ~BMF_V9_MASK)) return false;
    const bool damaging = m->category != BCAT_STATUS;
    if (damaging != (m->power != 0) || damaging != has_effect(m, BEFF_DAMAGE)) return false;
    // A charging move is CHARGE, then DAMAGE, then its other effects.
    const unsigned first = has_effect(m, BEFF_CHARGE) ? 1 : 0;
    unsigned damage = 0;
    for (unsigned i = 0; i < m->effect_count; ++i) {
        const BattleEffect *e = &m->effects[i];
        if (e->who > BWHO_USER || e->chance > 100 || (e->flags & ~BEFL_TYPE_CHART)) return false;
        if (e->chance && !damaging) return false;  // chances are secondary effects of a hit
        switch (e->kind) {
        case BEFF_DAMAGE:
            if (damage++ || i != first || e->who != BWHO_TARGET || e->chance || e->flags ||
                e->arg0 < 1 || e->arg0 > e->arg2 || e->arg2 > BATTLE_MAX_HITS ||
                e->arg1 < BHITS_UNIFORM || e->arg1 > BHITS_2TO5_GEN1 ||
                (e->arg1 != BHITS_UNIFORM && (e->arg0 != 2 || e->arg2 != 5)))
                return false;
            break;
        case BEFF_STAT:
            if (e->arg0 >= BSTAT_COUNT || !e->arg1 || e->arg1 < -6 || e->arg1 > 6 || e->arg2 || e->flags) return false;
            break;
        case BEFF_STATUS:
            if (!e->arg0 || e->arg0 > BST_FROSTBITE || e->arg1 || e->arg2) return false;
            break;
        case BEFF_CURE:
            if (!e->arg0 || (e->arg0 & 1u) || e->arg1 || e->arg2 || e->flags) return false;
            break;
        case BEFF_HEAL:
            if (e->who != BWHO_USER || !e->arg0 || !e->arg2 || e->arg0 > e->arg2 || e->arg1 || e->flags || damaging)
                return false;
            break;
        case BEFF_DRAIN:
        case BEFF_RECOIL:
            if (e->who != BWHO_USER || !e->arg0 || e->arg0 > 100 || e->arg1 || e->arg2 || e->flags || !damaging ||
                e->chance)
                return false;
            break;
        case BEFF_FORCE_OUT:
        case BEFF_PIVOT:
            if (e->who != BWHO_TARGET || e->chance || e->arg0 || e->arg1 || e->arg2 || e->flags) return false;
            if (e->kind == BEFF_PIVOT && !damaging) return false;
            break;
        case BEFF_PROTECT:
            if (e->who != BWHO_USER || damaging || m->effect_count != 1 || (e->arg0 != 2 && e->arg0 != 3) ||
                e->arg1 || e->arg2 || e->flags || e->chance)
                return false;
            break;
        case BEFF_FLINCH:
            if (e->who != BWHO_TARGET || !damaging || e->arg0 || e->arg1 || e->arg2 || e->flags) return false;
            break;
        case BEFF_CONFUSE:
            if (e->arg0 || e->arg1 || e->arg2 || e->flags) return false;
            break;
        case BEFF_SUBSTITUTE:
            if (e->who != BWHO_USER || damaging || m->effect_count != 1 || e->chance || e->arg0 || e->arg1 ||
                e->arg2 || e->flags)
                return false;
            break;
        case BEFF_CHARGE:
            if (i || e->who != BWHO_USER || !damaging || e->chance || e->arg0 > BSEMI_UNDERGROUND || e->arg1 ||
                e->arg2 || e->flags || m->effect_count < 2 || m->effects[1].kind != BEFF_DAMAGE)
                return false;
            break;
        case BEFF_RECHARGE:
            if (e->who != BWHO_USER || !damaging || e->chance || e->arg0 || e->arg1 || e->arg2 || e->flags ||
                has_effect(m, BEFF_CHARGE))
                return false;
            break;
        default:
            return false;
        }
    }
    return m->effect_count != 0;
}

static unsigned move_effects_size(const BattleMove *m) {
    return BATTLE_MOVE_EFFECTS_HEADER + (unsigned)m->effect_count * BATTLE_EFFECT_BYTES;
}

static void move_effects_encode(const BattleMove *m, uint8_t *p) {
    p[0] = m->effect_count;
    p[1] = m->move_flags;
    msg_put16(p + 2, m->source_effect);
    for (unsigned i = 0; i < m->effect_count; ++i) {
        const BattleEffect *e = &m->effects[i];
        uint8_t *q = p + BATTLE_MOVE_EFFECTS_HEADER + i * BATTLE_EFFECT_BYTES;
        q[0] = e->kind;
        q[1] = e->who;
        q[2] = e->chance;
        q[3] = e->arg0;
        q[4] = (uint8_t)e->arg1;
        q[5] = e->arg2;
        q[6] = e->flags;
        q[7] = 0;
    }
}

// Returns the bytes consumed, or 0 if the block is malformed or too short.
static unsigned move_effects_decode(const uint8_t *p, uint32_t available, BattleMove *m) {
    if (available < BATTLE_MOVE_EFFECTS_HEADER || p[0] > BATTLE_MAX_EFFECTS) return 0;
    m->effect_count = p[0];
    m->move_flags = p[1];
    m->source_effect = msg_get16(p + 2);
    const unsigned size = move_effects_size(m);
    if (available < size) return 0;
    for (unsigned i = 0; i < m->effect_count; ++i) {
        const uint8_t *q = p + BATTLE_MOVE_EFFECTS_HEADER + i * BATTLE_EFFECT_BYTES;
        if (q[7]) return 0;
        m->effects[i] = (BattleEffect){q[0], q[1], q[2], q[3], (int8_t)q[4], q[5], q[6]};
    }
    return size;
}

bool battle_volatile_rules_valid(const BattleVolatileRules *r) {
    return !(r->flags & ~0x7u) && r->confusion_hit_percent >= 1 && r->confusion_hit_percent <= 100 &&
           r->confusion_min >= 1 && r->confusion_min <= r->confusion_max && r->confusion_max <= 8 &&
           r->confusion_power && r->substitute_divisor >= 2 && r->substitute_divisor <= 8;
}

static void volatile_rules_encode(const BattleVolatileRules *r, uint8_t *p) {
    p[0] = r->flags;
    p[1] = r->confusion_hit_percent;
    p[2] = r->confusion_min;
    p[3] = r->confusion_max;
    p[4] = r->confusion_power;
    p[5] = r->substitute_divisor;
    p[6] = p[7] = 0;
}

static bool volatile_rules_decode(const uint8_t *p, BattleVolatileRules *r) {
    *r = (BattleVolatileRules){p[0], p[1], p[2], p[3], p[4], p[5]};
    return !p[6] && !p[7] && battle_volatile_rules_valid(r);
}

bool battle_condition_rules_valid(const BattleConditionRules *r, unsigned types) {
    return !(r->flags & ~0x3fu) &&
           r->poison_type < types && r->steel_type < types && r->fire_type < types &&
           r->electric_type < types && r->ice_type < types &&
           (r->paralysis_speed_divisor == 2 || r->paralysis_speed_divisor == 4) &&
           (r->paralysis_block_divisor == 4 || r->paralysis_block_divisor == 8) &&
           (r->burn_residual_divisor == 8 || r->burn_residual_divisor == 16) &&
           r->sleep_min >= 1 && r->sleep_min <= r->sleep_max && r->sleep_max <= 7 &&
           r->thaw_percent <= 100;
}

bool battle_condition_valid(uint32_t value, const BattleConditionRules *r) {
    const uint8_t kind = BATTLE_CONDITION_KIND(value), counter = BATTLE_CONDITION_COUNTER(value);
    if ((value >> 16) || kind > BST_FROSTBITE) return false;
    if (kind == BST_TOXIC) return counter <= 15;
    if (kind == BST_SLEEP) return counter && counter <= r->sleep_max;
    return !counter && (kind != BST_FROSTBITE || (r->flags & BCR_FROSTBITE));
}

static void conditions_encode(const BattleConditionRules *r, uint8_t *p) {
    p[0] = r->flags;
    p[1] = r->poison_type;
    p[2] = r->steel_type;
    p[3] = r->fire_type;
    p[4] = r->electric_type;
    p[5] = r->ice_type;
    p[6] = r->paralysis_speed_divisor;
    p[7] = r->paralysis_block_divisor;
    p[8] = r->burn_residual_divisor;
    p[9] = r->sleep_min;
    p[10] = r->sleep_max;
    p[11] = r->thaw_percent;
}

static void conditions_decode(const uint8_t *p, BattleConditionRules *r) {
    *r = (BattleConditionRules){
        .flags = p[0], .poison_type = p[1], .steel_type = p[2], .fire_type = p[3],
        .electric_type = p[4], .ice_type = p[5], .paralysis_speed_divisor = p[6],
        .paralysis_block_divisor = p[7], .burn_residual_divisor = p[8],
        .sleep_min = p[9], .sleep_max = p[10], .thaw_percent = p[11],
    };
}

static bool mon_decode(const uint8_t *p, BattleMon *m, uint8_t version) {
    if (p[0] != BATTLE_MON_VERSION || p[1] < 1 || p[1] > 2) return false;
    m->type_count = p[1];
    m->types[0] = p[2];
    m->types[1] = p[3];
    m->front_y = m->back_y = 0;
    m->elevation = 0;
    for (unsigned i = 0; i < 4; ++i) {
        move_decode(p + 8 + 8*i, &m->moves[i]);
        m->moves[i].effect = version >= BATTLE_REQUEST_VERSION_DRAG && version < BATTLE_REQUEST_VERSION_EFFECTS ?
                             p[4+i] : BME_HIT;
        m->moves[i].effect_count = 0;
        if (version >= BATTLE_REQUEST_VERSION_EFFECTS && p[4+i]) return false;  // reserved in V8
        if (!battle_effect_valid(m->moves[i].effect, version)) return false;
    }
    return true;
}

static uint32_t effects_size(const BattleRequest *r) {
    if (r->version < BATTLE_REQUEST_VERSION_EFFECTS) return 0;
    uint32_t size = 0;
    for (unsigned side = 0; side < 2; ++side) {
        const BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count && i < BATTLE_MAX_PER_SIDE; ++i)
            for (unsigned m = 0; m < 4; ++m) size += move_effects_size(&e[i].mon.moves[m]);
    }
    return size;
}

static uint32_t fixed_size(const BattleRequest *r) {
    return BATTLE_REQUEST_HEADER + (uint32_t)(r->player_count + r->opponent_count) * BATTLE_ENTRY_BYTES +
           (uint32_t)r->type_count * r->type_count +
           (r->version >= BATTLE_REQUEST_VERSION_CONDITIONS ? BATTLE_CONDITION_RULE_BYTES : 0);
}

uint32_t battle_request_size(const BattleRequest *r) {
    return fixed_size(r) + effects_size(r) +
           (r->version >= BATTLE_REQUEST_VERSION_VOLATILES ? BATTLE_VOLATILE_RULE_BYTES : 0);
}

uint32_t battle_request_encode(const BattleRequest *r, uint8_t *p, uint32_t capacity) {
    const uint32_t size = battle_request_size(r);
    const uint8_t version = r->version ? r->version : BATTLE_REQUEST_VERSION;
    if ((version < BATTLE_REQUEST_VERSION || version > BATTLE_REQUEST_VERSION_VOLATILES) ||
        size > capacity || r->player_count > BATTLE_MAX_PER_SIDE || r->opponent_count > BATTLE_MAX_PER_SIDE ||
        r->type_count > BATTLE_MAX_TYPES)
        return 0;
    p[0] = version;
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
    if (version >= BATTLE_REQUEST_VERSION_STRUGGLE) {
        p[21] = r->struggle_power;
        p[22] = r->struggle_accuracy;
        p[23] = r->struggle_recoil;
    }
    uint8_t *q = p + BATTLE_REQUEST_HEADER;
    for (unsigned side = 0; side < 2; ++side) {
        const BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count; ++i, q += BATTLE_ENTRY_BYTES) {
            pkmn_record_encode(&e[i].record, q);
            mon_encode(&e[i].mon, q + PKMN_RECORD_BYTES,
                       version >= BATTLE_REQUEST_VERSION_DRAG && version < BATTLE_REQUEST_VERSION_EFFECTS);
        }
    }
    for (unsigned i = 0; i < (unsigned)r->type_count * r->type_count; ++i) q[i] = r->type_chart[i];
    q += (unsigned)r->type_count * r->type_count;
    if (version >= BATTLE_REQUEST_VERSION_CONDITIONS) {
        conditions_encode(&r->conditions, q);
        q += BATTLE_CONDITION_RULE_BYTES;
    }
    if (version >= BATTLE_REQUEST_VERSION_EFFECTS)
        for (unsigned side = 0; side < 2; ++side) {
            const BattleEntry *e = side ? r->opponent : r->player;
            const unsigned count = side ? r->opponent_count : r->player_count;
            for (unsigned i = 0; i < count; ++i)
                for (unsigned m = 0; m < 4; ++m) {
                    const BattleMove *move = &e[i].mon.moves[m];
                    if (move->effect_count > BATTLE_MAX_EFFECTS) return 0;
                    move_effects_encode(move, q);
                    q += move_effects_size(move);
                }
        }
    if (version >= BATTLE_REQUEST_VERSION_VOLATILES) volatile_rules_encode(&r->volatiles, q);
    return size;
}

bool battle_request_decode(const uint8_t *p, uint32_t length, BattleRequest *r) {
    if (length < BATTLE_REQUEST_HEADER ||
        (p[0] < BATTLE_REQUEST_VERSION || p[0] > BATTLE_REQUEST_VERSION_VOLATILES) ||
        msg_get16(p+14) != BATTLE_ENTRY_BYTES)
        return false;
    r->version = p[0];
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
    const bool struggle = r->version >= BATTLE_REQUEST_VERSION_STRUGGLE;
    r->struggle_power = struggle ? p[21] : 0;
    r->struggle_accuracy = struggle ? p[22] : 0;
    r->struggle_recoil = struggle ? p[23] : 0;
    if (struggle && (!r->struggle_power || r->struggle_accuracy > 100 ||
        (r->struggle_recoil != BSTRUGGLE_MAX_HP_FLOOR && r->struggle_recoil != BSTRUGGLE_MAX_HP_NEAREST)))
        return false;
    if (!r->player_count || !r->opponent_count || r->player_count > BATTLE_MAX_PER_SIDE ||
        r->opponent_count > BATTLE_MAX_PER_SIDE || !r->type_count || r->type_count > BATTLE_MAX_TYPES ||
        (r->version < BATTLE_REQUEST_VERSION_EFFECTS ? length != fixed_size(r) : length < fixed_size(r)))
        return false;
    const uint8_t *q = p + BATTLE_REQUEST_HEADER;
    for (unsigned side = 0; side < 2; ++side) {
        BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count; ++i, q += BATTLE_ENTRY_BYTES)
            if (!pkmn_record_decode(q, PKMN_RECORD_BYTES, &e[i].record) ||
                !mon_decode(q + PKMN_RECORD_BYTES, &e[i].mon, r->version))
                return false;
    }
    for (unsigned i = 0; i < (unsigned)r->type_count * r->type_count; ++i) r->type_chart[i] = q[i];
    q += (unsigned)r->type_count * r->type_count;
    r->conditions = (BattleConditionRules){0};
    if (r->version >= BATTLE_REQUEST_VERSION_CONDITIONS) {
        conditions_decode(q, &r->conditions);
        q += BATTLE_CONDITION_RULE_BYTES;
        if (!battle_condition_rules_valid(&r->conditions, r->type_count)) return false;
    }
    r->volatiles = (BattleVolatileRules){0};
    if (r->version < BATTLE_REQUEST_VERSION_EFFECTS) return true;
    const uint8_t *end = p + length;
    if (r->version >= BATTLE_REQUEST_VERSION_VOLATILES) {
        if (length < fixed_size(r) + BATTLE_VOLATILE_RULE_BYTES) return false;
        end -= BATTLE_VOLATILE_RULE_BYTES;
        if (!volatile_rules_decode(end, &r->volatiles)) return false;
    }
    for (unsigned side = 0; side < 2; ++side) {
        BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count; ++i)
            for (unsigned m = 0; m < 4; ++m) {
                BattleMove *move = &e[i].mon.moves[m];
                const unsigned used = move_effects_decode(q, (uint32_t)(end - q), move);
                if (!used) return false;
                // Empty slots carry no effects; everything else must be well formed.
                if (move->unsupported != UNSUP_EMPTY && !move->unsupported && !battle_move_effects_valid(move))
                    return false;
                if (move->unsupported == UNSUP_EMPTY && move->effect_count) return false;
                q += used;
            }
    }
    return q == end;
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

static bool decode_multi_state(const uint8_t *p, unsigned n, BattleMultiState *s, uint8_t max_phase) {
    if (n != BATTLE_MULTI_STATE_BYTES || p[0] < BPHASE_WAIT || p[0] > max_phase ||
        p[2] > OUTCOME_UNSUPPORTED || p[3] >= BATTLE_FIELD_SLOTS ||
        (p[4] & ~BATTLE_MULTI_SHIFT) ||
        ((p[0] == BPHASE_CHOOSE || p[0] >= BPHASE_REPLACE) && !msg_get16(p+5)))
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

bool battle_multi_state_decode(const uint8_t *p, unsigned n, BattleMultiState *s) {
    return decode_multi_state(p, n, s, BPHASE_ENDED);
}

unsigned battle_roster_state_encode(const BattleRosterState *s, uint8_t *p) {
    uint8_t *q = p + battle_multi_state_encode(&s->field_state, p);
    *q++ = s->reserves;
    for (unsigned i = 0; i < BATTLE_MAX_PER_SIDE; ++i, q += 5) {
        q[0] = s->player[i].party_slot;
        msg_put16(q+1, s->player[i].hp);
        msg_put16(q+3, s->player[i].max_hp);
    }
    return BATTLE_ROSTER_STATE_BYTES;
}

static bool decode_roster_state(const uint8_t *p, unsigned n, BattleRosterState *s, uint8_t max_phase) {
    if (n != BATTLE_ROSTER_STATE_BYTES ||
        !decode_multi_state(p, BATTLE_MULTI_STATE_BYTES, &s->field_state, max_phase))
        return false;
    const uint8_t *q = p + BATTLE_MULTI_STATE_BYTES;
    s->struggle_mask = 0;
    s->reserves = *q++;
    if (s->reserves >= (1u << BATTLE_MAX_PER_SIDE) ||
        (s->field_state.phase >= BPHASE_REPLACE && !s->reserves))
        return false;
    for (unsigned i = 0; i < BATTLE_MAX_PER_SIDE; ++i, q += 5) {
        s->player[i] = (BattleFieldMon){q[0], msg_get16(q+1), msg_get16(q+3)};
        if (s->player[i].hp > s->player[i].max_hp ||
            ((s->reserves & (1u << i)) && (!s->player[i].hp || q[0] == BATTLE_SLOT_NONE)))
            return false;
    }
    return true;
}

bool battle_roster_state_decode(const uint8_t *p, unsigned n, BattleRosterState *s) {
    return decode_roster_state(p, n, s, BPHASE_REPLACE);
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

static bool decode_multi_action(const uint8_t *p, unsigned n, BattleMultiAction *a, uint8_t max_action) {
    if (n != BATTLE_MULTI_ACTION_BYTES || p[2] >= BATTLE_FIELD_SLOTS ||
        p[3] > max_action ||
        p[4] >= 4 || p[6] > BROT_RIGHT || (p[7] & ~BACTION_FLAG_EXIT))
        return false;
    if (p[3] == BACTION_REPLACE || p[3] == BACTION_SWITCH || p[3] == BACTION_PIVOT) {
        if (p[5] >= BATTLE_MAX_PER_SIDE || p[4] || p[6]) return false;
    } else if (p[3] == BACTION_STRUGGLE) {
        if (p[4] || p[5]) return false;
    } else if (p[5] >= BATTLE_FIELD_SLOTS) {
        return false;
    }
    a->token = msg_get16(p);
    a->actor = p[2];
    a->action = p[3];
    a->move_slot = p[4];
    a->target = p[5];
    a->rotation = p[6];
    a->flags = p[7];
    return true;
}

bool battle_multi_action_decode(const uint8_t *p, unsigned n, BattleMultiAction *a) {
    return decode_multi_action(p, n, a, BACTION_SHIFT);
}

bool battle_roster_action_decode(const uint8_t *p, unsigned n, BattleMultiAction *a) {
    return decode_multi_action(p, n, a, BACTION_REPLACE);
}

bool battle_switch_action_decode(const uint8_t *p, unsigned n, BattleMultiAction *a) {
    return decode_multi_action(p, n, a, BACTION_SWITCH);
}

unsigned battle_struggle_state_encode(const BattleRosterState *s, uint8_t *p) {
    p[battle_roster_state_encode(s, p)] = s->struggle_mask;
    return BATTLE_STRUGGLE_STATE_BYTES;
}

static bool decode_struggle_state(const uint8_t *p, unsigned n, BattleRosterState *s, uint8_t max_phase) {
    if (n != BATTLE_STRUGGLE_STATE_BYTES || p[BATTLE_ROSTER_STATE_BYTES] >= (1u << BATTLE_FIELD_SLOTS) ||
        !decode_roster_state(p, BATTLE_ROSTER_STATE_BYTES, s, max_phase))
        return false;
    s->struggle_mask = p[BATTLE_ROSTER_STATE_BYTES];
    return true;
}

bool battle_struggle_state_decode(const uint8_t *p, unsigned n, BattleRosterState *s) {
    return decode_struggle_state(p, n, s, BPHASE_REPLACE);
}

bool battle_pivot_state_decode(const uint8_t *p, unsigned n, BattleRosterState *s) {
    return decode_struggle_state(p, n, s, BPHASE_PIVOT);
}

bool battle_pivot_action_decode(const uint8_t *p, unsigned n, BattleMultiAction *a) {
    return decode_multi_action(p, n, a, BACTION_PIVOT);
}

bool battle_struggle_action_decode(const uint8_t *p, unsigned n, BattleMultiAction *a) {
    return decode_multi_action(p, n, a, BACTION_STRUGGLE);
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

unsigned battle_condition_state_encode(const BattleRosterState *s, uint8_t *p) {
    p += battle_struggle_state_encode(s, p);
    for (unsigned side = 0; side < 2; ++side)
        for (unsigned slot = 0; slot < BATTLE_FIELD_SLOTS; ++slot) {
            *p++ = s->status[side][slot];
            for (unsigned stat = 0; stat < BSTAT_COUNT; ++stat)
                *p++ = (uint8_t)s->stages[side][slot][stat];
        }
    return BATTLE_CONDITION_STATE_BYTES;
}

unsigned battle_volatile_state_encode(const BattleRosterState *s, uint8_t *p) {
    p += battle_condition_state_encode(s, p);
    for (unsigned side = 0; side < 2; ++side)
        for (unsigned slot = 0; slot < BATTLE_FIELD_SLOTS; ++slot) *p++ = s->volatiles[side][slot];
    return BATTLE_VOLATILE_STATE_BYTES;
}

bool battle_volatile_state_decode(const uint8_t *p, unsigned length, BattleRosterState *s) {
    if (length != BATTLE_VOLATILE_STATE_BYTES || !battle_condition_state_decode(p, BATTLE_CONDITION_STATE_BYTES, s))
        return false;
    p += BATTLE_CONDITION_STATE_BYTES;
    for (unsigned side = 0; side < 2; ++side)
        for (unsigned slot = 0; slot < BATTLE_FIELD_SLOTS; ++slot) {
            if (*p & ~BVF_MASK) return false;
            s->volatiles[side][slot] = *p++;
        }
    return true;
}

bool battle_condition_state_decode(const uint8_t *p, unsigned length, BattleRosterState *s) {
    if (length != BATTLE_CONDITION_STATE_BYTES ||
        !battle_pivot_state_decode(p, BATTLE_STRUGGLE_STATE_BYTES, s)) return false;
    p += BATTLE_STRUGGLE_STATE_BYTES;
    for (unsigned side = 0; side < 2; ++side)
        for (unsigned slot = 0; slot < BATTLE_FIELD_SLOTS; ++slot) {
            if (*p > BST_FROSTBITE) return false;
            s->status[side][slot] = *p++;
            for (unsigned stat = 0; stat < BSTAT_COUNT; ++stat) {
                const int8_t stage = (int8_t)*p++;
                if (stage < -6 || stage > 6) return false;
                s->stages[side][slot][stat] = stage;
            }
        }
    return true;
}
