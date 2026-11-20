// Vendored from gba-gc-extension protocol/battle_protocol.c. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#include "gc_ext/battle_protocol.h"
#include <string.h>

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
                                        "substitute", "charge", "recharge", "modifier", "weather",
                                        "weather_accuracy", "weather_type", "weather_heal", "terrain"};
    return kind < BEFF_COUNT ? names[kind] : "invalid";
}

// ------------------------------------------------------------- V13 modifiers

void battle_modifier_put(const BattleModifier *mod, uint8_t flags, uint8_t *q) {
    q[0] = mod->stage;
    q[1] = mod->percent;
    q[2] = flags;
    for (unsigned i = 0; i < BATTLE_MAX_PREDICATES; ++i) {
        q[3 + 2 * i] = mod->pred[i].kind;
        q[4 + 2 * i] = mod->pred[i].arg;
    }
}

void battle_modifier_get(const uint8_t *q, BattleModifier *mod, uint8_t *flags) {
    mod->stage = q[0];
    mod->percent = q[1];
    *flags = q[2];
    for (unsigned i = 0; i < BATTLE_MAX_PREDICATES; ++i) mod->pred[i] = (BattlePredicate){q[3 + 2 * i], q[4 + 2 * i]};
}

static bool damage_stage(uint8_t stage) { return stage >= BMOD_BASE_POWER && stage <= BMOD_FINAL; }

static bool predicate_valid(BattlePredicate p, uint8_t stage, unsigned types, uint8_t source, bool v15, bool v16) {
    const uint8_t kind = p.kind & BPRED_KIND_MASK, subject = p.kind & ~BPRED_KIND_MASK;
    if (subject & ~BPRED_DEFENDER) return false;  // bit 6 is reserved
    // A speed record has one Pokemon, the holder, and no move.
    if (stage == BMOD_SPEED && subject) return false;
    if (!v15 && kind >= BPRED_V14_COUNT) return false;
    if (!v16 && kind >= BPRED_V15_COUNT) return false;
    switch (kind) {
    case BPRED_NONE: return !p.kind && !p.arg;
    case BPRED_HP_AT_MOST: return p.arg >= 2;
    case BPRED_STATUS: return p.arg && !(p.arg & ~0xfeu);  // 1 << BST_POISON .. 1 << BST_FROSTBITE
    // Properties of the move itself: an ability's or the field's filter; for a move the GBA resolves them.
    case BPRED_MOVE_TYPE: return source != BMSRC_MOVE && stage != BMOD_SPEED && !subject && p.arg < types;
    case BPRED_POWER_AT_MOST:
        return (source == BMSRC_ABILITY || source == BMSRC_ITEM) && damage_stage(stage) && !subject && p.arg &&
               stage != BMOD_BASE_POWER;
    case BPRED_WEATHER: return !subject && p.arg && !(p.arg & ~BWEATHER_ALL);
    case BPRED_HAS_TYPE: return p.arg < types;
    case BPRED_TERRAIN: return !subject && p.arg && !(p.arg & ~BTERRAIN_ALL);
    case BPRED_ON_TERRAIN: return p.arg && !(p.arg & ~BTERRAIN_ALL);
    case BPRED_EXPOSED: return !p.arg;
    default: return false;  // unknown predicates are refused, never ignored
    }
}

// The stages a record of each source may use.
static bool stage_allowed(uint8_t stage, uint8_t source, bool v16) {
    switch (source) {
    case BMSRC_MOVE: return damage_stage(stage);
    case BMSRC_ABILITY: return damage_stage(stage) || stage == BMOD_SPEED || stage == BMOD_ACCURACY;
    case BMSRC_FIELD: return stage == BMOD_WEATHER || stage == BMOD_DEFENSE || (v16 && stage == BMOD_POWER);
    // V17: the power of the holder's moves (where the game applies the attacker's item); the
    // other stages are added with the items that need them.
    case BMSRC_ITEM: return stage == BMOD_POWER;
    default: return false;
    }
}

// The stages the core chains in 4.12 fixed point (V14 and later): only they can truncate (V17).
static bool fixed_point_stage(uint8_t stage) {
    return stage == BMOD_POWER || stage == BMOD_ATTACK || stage == BMOD_DEFENSE || stage == BMOD_FINAL ||
           stage == BMOD_WEATHER;
}

// One meaning per record: packed predicates, no reserved bits, a percent.
bool battle_modifier_valid(const BattleModifier *mod, unsigned types, uint8_t source, uint8_t version) {
    const bool v15 = version >= BATTLE_REQUEST_VERSION_WEATHER, v16 = version >= BATTLE_REQUEST_VERSION_TERRAIN;
    const bool v17 = version >= BATTLE_REQUEST_VERSION_ITEMS;
    const uint8_t stage = v15 ? battle_modifier_stage(mod->stage) : mod->stage & BMOD_STAGE_MASK;
    const uint8_t category = (mod->stage & BMOD_CATEGORY_MASK) >> BMOD_CATEGORY_SHIFT;
    const bool ability = source == BMSRC_ABILITY;
    if (stage == BMOD_NONE || stage >= (v15 ? BMOD_COUNT : BMOD_V14_COUNT) || category > BCAT_SPECIAL) return false;
    if (source == BMSRC_FIELD && !v15) return false;
    if (source == BMSRC_ITEM && !v17) return false;
    if (!stage_allowed(stage, source, v16)) return false;
    const uint8_t allowed = BMOD_STAGE_MASK | BMOD_CATEGORY_MASK | (v15 ? BMOD_STAGE_HIGH : 0) |
                            (ability && stage != BMOD_SPEED ? BMODF_DEFENDER : 0) |
                            (v17 && fixed_point_stage(stage) ? BMOD_FLOORED : 0);
    if (mod->stage & ~allowed) return false;
    // A move's own category is known to the GBA; a speed record has no move.
    if (category && (source == BMSRC_MOVE || stage == BMOD_SPEED)) return false;
    if (!mod->percent) return false;
    if (!mod->pred[0].kind && mod->pred[1].kind) return false;
    for (unsigned i = 0; i < BATTLE_MAX_PREDICATES; ++i)
        if (!predicate_valid(mod->pred[i], stage, types, source, v15, v16)) return false;
    return true;
}

static bool has_effect(const BattleMove *m, uint8_t kind) {
    for (unsigned i = 0; i < m->effect_count; ++i)
        if (m->effects[i].kind == kind) return true;
    return false;
}

// V10 timing: only for secondary effects of a hit; effects before or after
// all targets can only act on the user.
static bool effect_timing_valid(const BattleEffect *e, bool damaging) {
    const unsigned timing = BATTLE_EFFECT_TIMING(e);
    if (timing == BETIME_DEFAULT) return true;
    if (timing >= BETIME_COUNT || !damaging) return false;
    switch (e->kind) {
    case BEFF_STAT:
    case BEFF_STATUS:
    case BEFF_CURE:
    case BEFF_CONFUSE:
        return (timing != BETIME_AFTER_MOVE && timing != BETIME_PER_USE) || e->who == BWHO_USER;
    case BEFF_FLINCH:
        return timing == BETIME_PER_HIT || timing == BETIME_PER_TARGET || timing == BETIME_ON_DAMAGE;
    default:
        return false;
    }
}

bool battle_move_effects_valid(const BattleMove *m) {
    return battle_move_effects_valid_for(m, BATTLE_REQUEST_VERSION_LATEST);
}

static bool weather_mask_valid(uint8_t mask) { return mask && !(mask & ~BWEATHER_ALL); }

bool battle_move_effects_valid_for(const BattleMove *m, uint8_t version) {
    const bool v15 = version >= BATTLE_REQUEST_VERSION_WEATHER, v16 = version >= BATTLE_REQUEST_VERSION_TERRAIN;
    if (m->effect_count > BATTLE_MAX_EFFECTS || (m->move_flags & ~BMF_V11_MASK)) return false;
    const bool damaging = m->category != BCAT_STATUS;
    if (damaging != (m->power != 0) || damaging != has_effect(m, BEFF_DAMAGE)) return false;
    // A charging move is CHARGE, then DAMAGE, then its other effects.
    const unsigned first = has_effect(m, BEFF_CHARGE) ? 1 : 0;
    unsigned damage = 0;
    for (unsigned i = 0; i < m->effect_count; ++i) {
        const BattleEffect *e = &m->effects[i];
        if (e->kind == BEFF_MODIFIER) {  // V13 (the decoder refuses it in earlier requests)
            if (!damaging || e->flags || !battle_modifier_valid(&e->mod, 0, BMSRC_MOVE, version)) return false;
            continue;
        }
        if (e->kind > BEFF_V14_LAST && !v15) return false;
        if (e->kind > BEFF_V15_LAST && !v16) return false;
        if (e->who > BWHO_USER || e->chance > 100 || (e->flags & ~(BEFL_TYPE_CHART | BEFL_TIMING_MASK))) return false;
        if (e->chance && !damaging) return false;  // chances are secondary effects of a hit
        if (!effect_timing_valid(e, damaging)) return false;
        const uint8_t flags = e->flags & ~BEFL_TIMING_MASK;  // per-kind flags below
        switch (e->kind) {
        case BEFF_DAMAGE:
            if (damage++ || i != first || e->who != BWHO_TARGET || e->chance || flags ||
                e->arg0 < 1 || e->arg0 > e->arg2 || e->arg2 > BATTLE_MAX_HITS ||
                e->arg1 < BHITS_UNIFORM || e->arg1 > BHITS_2TO5_GEN1 ||
                (e->arg1 != BHITS_UNIFORM && (e->arg0 != 2 || e->arg2 != 5)))
                return false;
            break;
        case BEFF_STAT:
            if (e->arg0 >= BSTAT_COUNT || !e->arg1 || e->arg1 < -6 || e->arg1 > 6 || e->arg2 || flags) return false;
            break;
        case BEFF_STATUS:
            if (!e->arg0 || e->arg0 > BST_FROSTBITE || e->arg1 || e->arg2) return false;
            break;
        case BEFF_CURE:
            if (!e->arg0 || (e->arg0 & 1u) || e->arg1 || e->arg2 || flags) return false;
            break;
        case BEFF_HEAL:
            if (e->who != BWHO_USER || !e->arg0 || !e->arg2 || e->arg0 > e->arg2 || e->arg1 || flags || damaging)
                return false;
            break;
        case BEFF_DRAIN:
        case BEFF_RECOIL:
            if (e->who != BWHO_USER || !e->arg0 || e->arg0 > 100 || e->arg1 || e->arg2 || flags || !damaging ||
                e->chance)
                return false;
            break;
        case BEFF_FORCE_OUT:
        case BEFF_PIVOT:
            if (e->who != BWHO_TARGET || e->chance || e->arg0 || e->arg1 || e->arg2 || flags) return false;
            if (e->kind == BEFF_PIVOT && !damaging) return false;
            break;
        case BEFF_PROTECT:
            if (e->who != BWHO_USER || damaging || m->effect_count != 1 || (e->arg0 != 2 && e->arg0 != 3) ||
                e->arg1 || e->arg2 || flags || e->chance)
                return false;
            break;
        case BEFF_FLINCH:
            if (e->who != BWHO_TARGET || !damaging || e->arg0 || e->arg1 || e->arg2 || flags) return false;
            break;
        case BEFF_CONFUSE:
            if (e->arg0 || e->arg1 || e->arg2 || flags) return false;
            break;
        case BEFF_SUBSTITUTE:
            if (e->who != BWHO_USER || damaging || m->effect_count != 1 || e->chance || e->arg0 || e->arg1 ||
                e->arg2 || flags)
                return false;
            break;
        case BEFF_CHARGE:  // V15: arg2 = the weathers without a charging turn
            if (i || e->who != BWHO_USER || !damaging || e->chance || e->arg0 > BSEMI_UNDERGROUND || e->arg1 ||
                (e->arg2 && (!v15 || !weather_mask_valid(e->arg2))) || flags || m->effect_count < 2 ||
                m->effects[1].kind != BEFF_DAMAGE)
                return false;
            break;
        // V15 weather kinds: a field move, and passive rules of a move in a weather.
        case BEFF_WEATHER:
            if (damaging || m->effect_count != 1 || e->who != BWHO_USER || e->chance || !e->arg0 ||
                e->arg0 >= BWEATHER_COUNT || e->arg1 || (e->arg2 > BWEATHER_MAX_TURNS && e->arg2 != BWEATHER_TURNS_KEEP) ||
                flags)
                return false;
            break;
        case BEFF_TERRAIN:  // V16: a field move, like BEFF_WEATHER (no KEEP)
            if (damaging || m->effect_count != 1 || e->who != BWHO_USER || e->chance || !e->arg0 ||
                e->arg0 >= BTERRAIN_COUNT || e->arg1 || e->arg2 > BTERRAIN_MAX_TURNS || flags)
                return false;
            break;
        case BEFF_WEATHER_ACCURACY:
            if (e->who || e->chance || !weather_mask_valid(e->arg0) || e->arg1 || e->arg2 > 100 || e->flags) return false;
            break;
        case BEFF_WEATHER_TYPE:  // the type is checked against the request's type count by the core
            if (!damaging || e->who || e->chance || !weather_mask_valid(e->arg0) || e->arg1 || e->flags) return false;
            break;
        case BEFF_WEATHER_HEAL:
            if (damaging || !has_effect(m, BEFF_HEAL) || e->who != BWHO_USER || e->chance ||
                !weather_mask_valid(e->arg0) || e->arg1 < 1 || e->arg1 > e->arg2 || e->flags)
                return false;
            break;
        case BEFF_RECHARGE:
            if (e->who != BWHO_USER || !damaging || e->chance || e->arg0 || e->arg1 || e->arg2 || flags ||
                has_effect(m, BEFF_CHARGE))
                return false;
            break;
        default:
            return false;
        }
    }
    return m->effect_count != 0;
}

const char *battle_ability_kind_name(uint8_t kind) {
    static const char *const names[] = {"none", "unsupported", "attack_mult", "pinch", "low_power", "resist",
                                        "type_immune", "type_absorb", "status_immune", "volatile_immune",
                                        "stat_drop_immune", "accuracy", "no_guard", "contact_status",
                                        "contact_damage", "entry_stat", "end_turn_stat", "sturdy", "no_recoil",
                                        "shield_dust", "secondary_chance", "infiltrator", "defense_mult",
                                        "ignore_burn", "stab_mult", "ko_stat", "end_turn_cure", "absorb_state",
                                        "type_pass", "status_mirror", "modifier", "entry_weather",
                                        "weather_suppress", "weather_immune", "weather_residual", "entry_terrain",
                                        "ungrounded"};
    return kind < BABIL_COUNT ? names[kind] : "invalid";
}

static bool category_ok(uint8_t category, bool any) {
    return category == BCAT_PHYSICAL || category == BCAT_SPECIAL || (any && category == BCAT_STATUS);
}

// Arguments a kind does not use must be zero, so a hook has one meaning.
bool battle_ability_hook_valid(const BattleAbilityHook *h, unsigned types, uint8_t version) {
    const bool v12 = version >= BATTLE_REQUEST_VERSION_ABILITY_HOOKS;
    const bool v13 = version >= BATTLE_REQUEST_VERSION_MODIFIERS;
    const bool v15 = version >= BATTLE_REQUEST_VERSION_WEATHER;
    const bool v16 = version >= BATTLE_REQUEST_VERSION_TERRAIN;
    const bool type_ok = h->arg0 < types;
    const uint8_t statuses = 0xfeu;  // 1 << BST_POISON .. 1 << BST_FROSTBITE
    const uint8_t mirrored = 1u << BST_POISON | 1u << BST_TOXIC | 1u << BST_PARALYSIS | 1u << BST_BURN;
    bool ok, chance = false, percent = false;
    uint8_t flags = 0;  // flags the kind accepts
    if (h->kind < BABIL_ATTACK_MULT ||
        h->kind > (v16 ? BABIL_COUNT - 1 : v15 ? BABIL_V15_LAST : v13 ? BABIL_V14_LAST : v12 ? BABIL_V12_LAST :
                   BABIL_V11_LAST))
        return false;
    switch (h->kind) {
    case BABIL_MODIFIER:  // everything but the flags is in the modifier record
        return !(h->flags & ~BAHF_BREAKABLE) && battle_modifier_valid(&h->mod, types, BMSRC_ABILITY, version);
    // V15 weather hooks.
    case BABIL_ENTRY_WEATHER:
        ok = h->arg0 && h->arg0 < BWEATHER_COUNT && !h->arg1 &&
             (h->arg2 <= BWEATHER_MAX_TURNS || h->arg2 == BWEATHER_TURNS_KEEP);
        break;
    case BABIL_WEATHER_SUPPRESS:
    case BABIL_UNGROUNDED:  // V16
        ok = !h->arg0 && !h->arg1 && !h->arg2;
        break;
    case BABIL_ENTRY_TERRAIN:  // V16
        ok = h->arg0 && h->arg0 < BTERRAIN_COUNT && !h->arg1 && h->arg2 <= BTERRAIN_MAX_TURNS;
        break;
    case BABIL_WEATHER_IMMUNE:
        ok = h->arg0 && !(h->arg0 & ~BWEATHER_ALL) && !h->arg1 && !h->arg2;
        break;
    case BABIL_WEATHER_RESIDUAL:
        flags = BAHF_NOT_UNDERGROUND;
        ok = h->arg0 && !(h->arg0 & ~BWEATHER_ALL) && (h->arg1 == 1 || h->arg1 == -1) && h->arg2 >= 2 &&
             h->arg2 <= 16;
        break;
    case BABIL_NO_GUARD: case BABIL_STURDY: case BABIL_NO_RECOIL: case BABIL_SHIELD_DUST:
    case BABIL_INFILTRATOR: case BABIL_IGNORE_BURN:
        ok = !h->arg0 && !h->arg1 && !h->arg2;
        break;
    case BABIL_ATTACK_MULT:
        flags = BAHF_REQUIRES_STATUS | BAHF_REQUIRES_STATE | BAHF_TYPE_FILTER;
        ok = category_ok(h->arg0, v12) && !h->arg1 &&
             ((h->flags & BAHF_TYPE_FILTER) ? h->arg2 < types : !h->arg2);
        percent = true;
        break;
    case BABIL_DEFENSE_MULT:
        flags = BAHF_REQUIRES_STATUS;
        ok = category_ok(h->arg0, false) && !h->arg1 && !h->arg2;
        percent = true;
        break;
    case BABIL_PINCH:
        ok = type_ok && !h->arg1 && h->arg2 >= 2 && h->arg2 <= 8;
        percent = true;
        break;
    case BABIL_LOW_POWER:
        ok = h->arg0 && !h->arg1 && !h->arg2;
        percent = true;
        break;
    case BABIL_RESIST:
        ok = type_ok && !h->arg1 && (h->arg2 == BABIL_NO_TYPE || (h->arg2 < types && h->arg2 != h->arg0));
        percent = true;
        break;
    case BABIL_TYPE_IMMUNE:
        ok = type_ok && !h->arg1 && !h->arg2;
        break;
    case BABIL_TYPE_ABSORB:
        ok = type_ok && !h->arg1 && !h->arg2 && h->percent <= 100;
        percent = true;
        break;
    case BABIL_ABSORB_STATE:
        flags = BAHF_NOT_FROZEN;
        ok = type_ok && !h->arg1 && !h->arg2;
        break;
    case BABIL_TYPE_PASS:
        ok = type_ok && !h->arg1 && h->arg2 < types;
        break;
    case BABIL_STATUS_IMMUNE:
        ok = h->arg0 && !(h->arg0 & ~statuses) && !h->arg1 && !h->arg2;
        break;
    case BABIL_STATUS_MIRROR:
        flags = BAHF_TOXIC_TO_POISON;
        ok = h->arg0 && !(h->arg0 & ~mirrored) && !h->arg1 && !h->arg2;
        break;
    case BABIL_END_TURN_CURE:
        ok = h->arg0 && !(h->arg0 & ~statuses) && !h->arg1 && !h->arg2;
        chance = true;
        break;
    case BABIL_VOLATILE_IMMUNE:
        ok = h->arg0 && !(h->arg0 & ~0x7u) && !h->arg1 && !h->arg2;
        break;
    case BABIL_STAT_DROP_IMMUNE:
        ok = !(h->arg0 & ~0x7fu) && (h->arg0 || h->arg2) && !h->arg1 && !(h->arg2 & ~BABIL_IGNORE_EVASION);
        break;
    case BABIL_ACCURACY:
    case BABIL_SECONDARY_CHANCE:
    case BABIL_STAB_MULT:
        ok = !h->arg0 && !h->arg1 && !h->arg2;
        percent = true;
        break;
    case BABIL_CONTACT_STATUS:
        ok = h->arg0 >= BST_POISON && h->arg0 <= BST_FROSTBITE && h->arg0 != BST_TOXIC && !h->arg1 && !h->arg2;
        chance = true;
        break;
    case BABIL_CONTACT_DAMAGE:
        ok = !h->arg0 && !h->arg1 && h->arg2 >= 2 && h->arg2 <= 16;
        break;
    case BABIL_ENTRY_STAT:
        ok = h->arg0 < BSTAT_COUNT && h->arg1 && h->arg1 >= -6 && h->arg1 <= 6 &&
             (h->arg2 == BRANGE_ALL_FOES || (v12 && h->arg2 == BRANGE_ADJACENT_FOES));
        break;
    case BABIL_END_TURN_STAT:
    case BABIL_KO_STAT:
        ok = h->arg0 < BSTAT_COUNT && h->arg1 && h->arg1 >= -6 && h->arg1 <= 6 && !h->arg2;
        break;
    default:
        return false;
    }
    if (v12) flags |= BAHF_BREAKABLE;
    if (h->flags & ~flags) return false;
    if (chance ? (!h->chance || !h->chance_den || h->chance > h->chance_den) : (h->chance || h->chance_den))
        return false;
    if (percent ? !h->percent : h->percent != 0) return false;
    return ok;
}

bool battle_ability_valid(const BattleAbility *a, unsigned types, uint8_t version) {
    if (a->cls == BACLASS_NONE || a->cls == BACLASS_UNSUPPORTED) return a->count == 0;
    if (a->cls != BACLASS_HOOKS || !a->count || a->count > BATTLE_MAX_ABILITY_HOOKS) return false;
    if (version < BATTLE_REQUEST_VERSION_ABILITY_HOOKS && a->count != 1) return false;
    for (unsigned i = 0; i < a->count; ++i)
        if (!battle_ability_hook_valid(&a->hooks[i], types, version)) return false;
    return true;
}

// ------------------------------------------------------- V17 / V18 held items

const char *battle_item_kind_name(uint8_t kind) {
    switch (kind) {
    case BITEM_NONE: return "none";
    case BITEM_MODIFIER: return "modifier";
    case BITEM_WEATHER_TURNS: return "weather turns";
    case BITEM_TERRAIN_TURNS: return "terrain turns";
    case BITEM_HP: return "hp";
    default: return "unknown";
    }
}

static bool item_record_valid(const BattleItemRecord *x, unsigned types, uint8_t version) {
    if (x->flags) return false;
    switch (x->kind) {
    case BITEM_MODIFIER: return battle_modifier_valid(&x->mod, types, BMSRC_ITEM, version);
    case BITEM_WEATHER_TURNS:
        return x->arg0 && !(x->arg0 & ~BWEATHER_ALL) && !x->arg1 && x->arg2 && x->arg2 <= BWEATHER_MAX_TURNS;
    case BITEM_TERRAIN_TURNS:
        return x->arg0 && !(x->arg0 & ~BTERRAIN_ALL) && !x->arg1 && x->arg2 && x->arg2 <= BTERRAIN_MAX_TURNS;
    case BITEM_HP:  // V18: one timing, a heal or damage, a holder predicate
        if (version < BATTLE_REQUEST_VERSION_END_TURN_ITEMS || x->hp.timing != BITIME_END_TURN ||
            (x->hp.op != BIHP_HEAL && x->hp.op != BIHP_DAMAGE) || !x->hp.divisor || x->hp.reserved)
            return false;
        switch (x->hp.pred.kind) {
        case BPRED_NONE: return !x->hp.pred.arg;
        case BPRED_HAS_TYPE: return x->hp.pred.arg < types;
        default: return false;
        }
    default: return false;  // unknown kinds are refused, never ignored
    }
}

// One meaning per block: the record's item, a class that fits it, and no two
// duration records for the same weather or terrain.
bool battle_item_valid(const BattleItem *it, uint16_t held_item, unsigned types, uint8_t version) {
    if (version < BATTLE_REQUEST_VERSION_ITEMS) return false;
    if (it->source != held_item) return false;
    switch (it->cls) {
    case BICLASS_NONE: return it->count == 0;
    case BICLASS_UNSUPPORTED: return it->count == 0 && held_item;
    case BICLASS_RECORDS: break;
    default: return false;
    }
    if (!held_item || !it->count || it->count > BATTLE_MAX_ITEM_RECORDS) return false;
    uint8_t weather = 0, terrain = 0;
    for (unsigned i = 0; i < it->count; ++i) {
        const BattleItemRecord *x = &it->records[i];
        if (!item_record_valid(x, types, version)) return false;
        uint8_t *seen = x->kind == BITEM_WEATHER_TURNS ? &weather : x->kind == BITEM_TERRAIN_TURNS ? &terrain : NULL;
        if (seen) {
            if (*seen & x->arg0) return false;
            *seen |= x->arg0;
        }
    }
    return true;
}

static unsigned item_size(const BattleItem *it) {
    return BATTLE_ITEM_HEADER_BYTES + (unsigned)(it->cls == BICLASS_RECORDS ? it->count : 0) * BATTLE_ITEM_RECORD_BYTES;
}

static void item_encode(const BattleItem *it, uint8_t *p) {
    const unsigned count = it->cls == BICLASS_RECORDS ? it->count : 0;
    p[0] = it->cls;
    p[1] = (uint8_t)count;
    msg_put16(p + 2, it->source);
    for (unsigned i = 0; i < count && i < BATTLE_MAX_ITEM_RECORDS; ++i) {
        const BattleItemRecord *x = &it->records[i];
        uint8_t *q = p + BATTLE_ITEM_HEADER_BYTES + i * BATTLE_ITEM_RECORD_BYTES;
        for (unsigned k = 0; k < BATTLE_ITEM_RECORD_BYTES; ++k) q[k] = 0;
        q[0] = x->kind;
        if (x->kind == BITEM_MODIFIER) {
            battle_modifier_put(&x->mod, x->flags, q + 1);
            continue;
        }
        if (x->kind == BITEM_HP) {  // V18 (the encoder validates the version first)
            q[1] = x->hp.timing;
            q[2] = x->hp.op;
            q[3] = x->hp.divisor;
            q[4] = x->hp.pred.kind;
            q[5] = x->hp.pred.arg;
            q[6] = x->hp.reserved;
            q[7] = x->flags;
            continue;
        }
        q[1] = x->arg0;
        q[2] = x->arg1;
        q[3] = x->arg2;
        q[7] = x->flags;
    }
}

// Returns the bytes consumed, or 0 if the block is too short, has too many
// records or carries anything in its unused bytes.
static unsigned item_decode(const uint8_t *p, uint32_t available, BattleItem *it) {
    if (available < BATTLE_ITEM_HEADER_BYTES || p[1] > BATTLE_MAX_ITEM_RECORDS) return 0;
    *it = (BattleItem){.cls = p[0], .count = p[1], .source = msg_get16(p + 2)};
    const unsigned size = BATTLE_ITEM_HEADER_BYTES + (unsigned)it->count * BATTLE_ITEM_RECORD_BYTES;
    if (available < size) return 0;
    for (unsigned i = 0; i < it->count; ++i) {
        const uint8_t *q = p + BATTLE_ITEM_HEADER_BYTES + i * BATTLE_ITEM_RECORD_BYTES;
        BattleItemRecord *x = &it->records[i];
        *x = (BattleItemRecord){.kind = q[0]};
        if (q[0] == BITEM_MODIFIER) {
            battle_modifier_get(q + 1, &x->mod, &x->flags);
            continue;
        }
        if (q[0] == BITEM_HP) {  // its version and fields are checked by battle_item_valid
            x->hp.timing = q[1];
            x->hp.op = q[2];
            x->hp.divisor = q[3];
            x->hp.pred = (BattlePredicate){q[4], q[5]};
            x->hp.reserved = q[6];
            x->flags = q[7];
            continue;
        }
        x->arg0 = q[1];
        x->arg1 = q[2];
        x->arg2 = q[3];
        x->flags = q[7];
        if (q[4] || q[5] || q[6]) return 0;
    }
    return size;
}

static uint32_t items_size(const BattleRequest *r) {
    if (r->version < BATTLE_REQUEST_VERSION_ITEMS) return 0;
    uint32_t size = 0;
    for (unsigned side = 0; side < 2; ++side) {
        const BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count && i < BATTLE_MAX_PER_SIDE; ++i) size += item_size(&e[i].mon.item);
    }
    return size;
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
        if (e->kind == BEFF_MODIFIER) {  // V13 (the encoder refuses it in earlier versions)
            battle_modifier_put(&e->mod, e->flags, q + 1);
            continue;
        }
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
static unsigned move_effects_decode(const uint8_t *p, uint32_t available, BattleMove *m, uint8_t version) {
    if (available < BATTLE_MOVE_EFFECTS_HEADER || p[0] > BATTLE_MAX_EFFECTS) return 0;
    m->effect_count = p[0];
    m->move_flags = p[1];
    m->source_effect = msg_get16(p + 2);
    const unsigned size = move_effects_size(m);
    if (available < size) return 0;
    for (unsigned i = 0; i < m->effect_count; ++i) {
        const uint8_t *q = p + BATTLE_MOVE_EFFECTS_HEADER + i * BATTLE_EFFECT_BYTES;
        BattleEffect *e = &m->effects[i];
        memset(e, 0, sizeof *e);  // the whole union, so a later read of either form is defined
        e->kind = q[0];
        if (version >= BATTLE_REQUEST_VERSION_MODIFIERS && q[0] == BEFF_MODIFIER) {
            battle_modifier_get(q + 1, &e->mod, &e->flags);
            continue;
        }
        if (q[7]) return 0;
        // Before V13 the kind is not a modifier: a supported move with it was always
        // refused (its legacy bytes never validated); an unsupported one keeps the bytes.
        if (q[0] == BEFF_MODIFIER && !m->unsupported) return 0;
        e->who = q[1];
        e->chance = q[2];
        e->arg0 = q[3];
        e->arg1 = (int8_t)q[4];
        e->arg2 = q[5];
        e->flags = q[6];
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

// ------------------------------------------------------------- V15 field rules

bool battle_field_rules_valid(const BattleFieldRules *r, unsigned types) {
    if ((r->flags & ~BFR_START_LOCKED) || r->modifier_count > BATTLE_MAX_FIELD_MODIFIERS) return false;
    // A start weather lasts 1..15 turns or has no end; a locked one needs a weather.
    if (r->start_weather >= BWEATHER_COUNT || r->start_turns > BWEATHER_MAX_TURNS ||
        (!r->start_weather && (r->start_turns || r->flags)))
        return false;
    for (unsigned k = 0; k < BWEATHER_KINDS; ++k) {
        const BattleWeatherKindRules *w = &r->weather[k];
        if (w->residual_divisor == 1 || (w->flags & ~BWKF_MASK)) return false;
        for (unsigned t = 0; t < BATTLE_WEATHER_IMMUNE_TYPES; ++t)
            if (w->immune_types[t] != BABIL_NO_TYPE && w->immune_types[t] >= types) return false;
    }
    for (unsigned i = 0; i < BATTLE_MAX_FIELD_MODIFIERS; ++i) {
        const BattleModifier *m = &r->modifiers[i];
        if (i < r->modifier_count) {
            if (!battle_modifier_valid(m, types, BMSRC_FIELD, BATTLE_REQUEST_VERSION_WEATHER)) return false;
        } else if (m->stage || m->percent || m->pred[0].kind || m->pred[0].arg || m->pred[1].kind || m->pred[1].arg) {
            return false;  // unused records are zero
        }
    }
    return true;
}

static void field_rules_encode(const BattleFieldRules *r, uint8_t *p) {
    for (unsigned i = 0; i < BATTLE_FIELD_RULE_BYTES; ++i) p[i] = 0;
    p[0] = r->flags;
    p[1] = r->start_weather;
    p[2] = r->start_turns;
    p[3] = r->modifier_count;
    for (unsigned k = 0; k < BWEATHER_KINDS; ++k) {
        uint8_t *q = p + 4 + k * BATTLE_FIELD_KIND_BYTES;
        q[0] = r->weather[k].residual_divisor;
        q[1] = r->weather[k].flags;
        for (unsigned t = 0; t < BATTLE_WEATHER_IMMUNE_TYPES; ++t) q[2 + t] = r->weather[k].immune_types[t];
    }
    for (unsigned i = 0; i < BATTLE_MAX_FIELD_MODIFIERS; ++i)
        battle_modifier_put(&r->modifiers[i], 0, p + 32 + i * BATTLE_MODIFIER_BODY_BYTES);
}

static bool field_rules_decode(const uint8_t *p, BattleFieldRules *r, unsigned types) {
    *r = (BattleFieldRules){.flags = p[0], .start_weather = p[1], .start_turns = p[2], .modifier_count = p[3]};
    for (unsigned k = 0; k < BWEATHER_KINDS; ++k) {
        const uint8_t *q = p + 4 + k * BATTLE_FIELD_KIND_BYTES;
        r->weather[k].residual_divisor = q[0];
        r->weather[k].flags = q[1];
        for (unsigned t = 0; t < BATTLE_WEATHER_IMMUNE_TYPES; ++t) r->weather[k].immune_types[t] = q[2 + t];
    }
    if (p[29] || p[30] || p[31]) return false;
    for (unsigned i = 0; i < BATTLE_MAX_FIELD_MODIFIERS; ++i) {
        uint8_t flags;
        battle_modifier_get(p + 32 + i * BATTLE_MODIFIER_BODY_BYTES, &r->modifiers[i], &flags);
        if (flags) return false;
    }
    return battle_field_rules_valid(r, types);
}

// ------------------------------------------------------------- V16 terrain rules

bool battle_terrain_rules_valid(const BattleTerrainRules *r, unsigned types) {
    if (r->flags || r->modifier_count > BATTLE_MAX_TERRAIN_MODIFIERS) return false;
    if (r->start_terrain >= BTERRAIN_COUNT || r->start_turns > BTERRAIN_MAX_TURNS ||
        (!r->start_terrain && r->start_turns))
        return false;
    if (r->airborne_type != BABIL_NO_TYPE && r->airborne_type >= types) return false;
    for (unsigned k = 0; k < BTERRAIN_KINDS; ++k)
        if (r->terrain[k].heal_divisor == 1 || (r->terrain[k].flags & ~BTKF_MASK)) return false;
    for (unsigned i = 0; i < BATTLE_MAX_TERRAIN_MODIFIERS; ++i) {
        const BattleModifier *m = &r->modifiers[i];
        if (i < r->modifier_count) {
            if (!battle_modifier_valid(m, types, BMSRC_FIELD, BATTLE_REQUEST_VERSION_TERRAIN)) return false;
        } else if (m->stage || m->percent || m->pred[0].kind || m->pred[0].arg || m->pred[1].kind || m->pred[1].arg) {
            return false;  // unused records are zero
        }
    }
    return true;
}

static void terrain_rules_encode(const BattleTerrainRules *r, uint8_t *p) {
    for (unsigned i = 0; i < BATTLE_TERRAIN_RULE_BYTES; ++i) p[i] = 0;
    p[0] = r->flags;
    p[1] = r->start_terrain;
    p[2] = r->start_turns;
    p[3] = r->airborne_type;
    p[4] = r->modifier_count;
    for (unsigned k = 0; k < BTERRAIN_KINDS; ++k) {
        p[8 + 2 * k] = r->terrain[k].heal_divisor;
        p[9 + 2 * k] = r->terrain[k].flags;
    }
    for (unsigned i = 0; i < BATTLE_MAX_TERRAIN_MODIFIERS; ++i)
        battle_modifier_put(&r->modifiers[i], 0, p + 16 + i * BATTLE_MODIFIER_BODY_BYTES);
}

static bool terrain_rules_decode(const uint8_t *p, BattleTerrainRules *r, unsigned types) {
    *r = (BattleTerrainRules){.flags = p[0], .start_terrain = p[1], .start_turns = p[2], .airborne_type = p[3],
                              .modifier_count = p[4]};
    if (p[5] || p[6] || p[7]) return false;
    for (unsigned k = 0; k < BTERRAIN_KINDS; ++k)
        r->terrain[k] = (BattleTerrainKindRules){p[8 + 2 * k], p[9 + 2 * k]};
    for (unsigned i = 0; i < BATTLE_MAX_TERRAIN_MODIFIERS; ++i) {
        uint8_t flags;
        battle_modifier_get(p + 16 + i * BATTLE_MODIFIER_BODY_BYTES, &r->modifiers[i], &flags);
        if (flags) return false;
    }
    return battle_terrain_rules_valid(r, types);
}

static unsigned ability_size(const BattleAbility *a, uint8_t version) {
    if (version < BATTLE_REQUEST_VERSION_ABILITY_HOOKS) return BATTLE_ABILITY_BYTES;
    return BATTLE_ABILITY_HEADER_BYTES + (unsigned)(a->cls == BACLASS_HOOKS ? a->count : 0) * BATTLE_ABILITY_HOOK_BYTES;
}

// V11 carries one hook with a percent chance and no flags; anything else
// cannot be written as V11.
static bool ability_encode_v11(const BattleAbility *a, uint8_t *p) {
    for (unsigned i = 0; i < BATTLE_ABILITY_BYTES; ++i) p[i] = 0;
    msg_put16(p + 6, a->source);
    if (a->cls != BACLASS_HOOKS) {
        p[0] = a->cls == BACLASS_UNSUPPORTED ? BABIL_UNSUPPORTED : BABIL_NONE;
        return a->count == 0;
    }
    const BattleAbilityHook *h = &a->hooks[0];
    if (a->count != 1 || h->flags || (h->chance_den && h->chance_den != 100) || h->kind > BABIL_V11_LAST)
        return false;
    p[0] = h->kind;
    p[1] = h->arg0;
    p[2] = (uint8_t)h->arg1;
    p[3] = h->arg2;
    p[4] = h->chance;
    p[5] = h->percent;
    return true;
}

static void ability_decode_v11(const uint8_t *p, BattleAbility *a) {
    *a = (BattleAbility){.source = msg_get16(p + 6)};
    if (p[0] == BABIL_NONE || p[0] == BABIL_UNSUPPORTED) {
        a->cls = p[0] == BABIL_UNSUPPORTED ? BACLASS_UNSUPPORTED : BACLASS_NONE;
        // Unused bytes must be zero (checked through an impossible hook).
        if (p[1] || p[2] || p[3] || p[4] || p[5]) a->count = 1;
        return;
    }
    a->cls = BACLASS_HOOKS;
    a->count = 1;
    a->hooks[0] = (BattleAbilityHook){p[0], {{p[1], (int8_t)p[2], p[3], p[4], (uint8_t)(p[4] ? 100 : 0), p[5]}}, 0};
}

static void ability_encode_v12(const BattleAbility *a, uint8_t *p) {
    p[0] = a->cls;
    p[1] = a->count;
    msg_put16(p + 2, a->source);
    for (unsigned i = 0; i < a->count && i < BATTLE_MAX_ABILITY_HOOKS; ++i) {
        const BattleAbilityHook *h = &a->hooks[i];
        uint8_t *q = p + BATTLE_ABILITY_HEADER_BYTES + i * BATTLE_ABILITY_HOOK_BYTES;
        q[0] = h->kind;
        if (h->kind == BABIL_MODIFIER) {  // V13 (the encoder refuses it in earlier versions)
            battle_modifier_put(&h->mod, h->flags, q + 1);
            continue;
        }
        q[1] = h->arg0;
        q[2] = (uint8_t)h->arg1;
        q[3] = h->arg2;
        q[4] = h->chance;
        q[5] = h->chance_den;
        q[6] = h->percent;
        q[7] = h->flags;
    }
}

// Returns the bytes consumed, or 0 if the block is too short or has too many hooks.
static unsigned ability_decode_v12(const uint8_t *p, uint32_t available, BattleAbility *a, uint8_t version) {
    if (available < BATTLE_ABILITY_HEADER_BYTES || p[1] > BATTLE_MAX_ABILITY_HOOKS) return 0;
    *a = (BattleAbility){.cls = p[0], .count = p[1], .source = msg_get16(p + 2)};
    const unsigned size = BATTLE_ABILITY_HEADER_BYTES + (unsigned)a->count * BATTLE_ABILITY_HOOK_BYTES;
    if (available < size) return 0;
    for (unsigned i = 0; i < a->count; ++i) {
        const uint8_t *q = p + BATTLE_ABILITY_HEADER_BYTES + i * BATTLE_ABILITY_HOOK_BYTES;
        if (version >= BATTLE_REQUEST_VERSION_MODIFIERS && q[0] == BABIL_MODIFIER) {
            a->hooks[i] = (BattleAbilityHook){.kind = BABIL_MODIFIER};
            battle_modifier_get(q + 1, &a->hooks[i].mod, &a->hooks[i].flags);
            continue;
        }
        a->hooks[i] = (BattleAbilityHook){q[0], {{q[1], (int8_t)q[2], q[3], q[4], q[5], q[6]}}, q[7]};
    }
    return size;
}

static uint32_t abilities_size(const BattleRequest *r) {
    if (r->version < BATTLE_REQUEST_VERSION_ABILITIES) return 0;
    uint32_t size = 0;
    for (unsigned side = 0; side < 2; ++side) {
        const BattleEntry *e = side ? r->opponent : r->player;
        const unsigned count = side ? r->opponent_count : r->player_count;
        for (unsigned i = 0; i < count && i < BATTLE_MAX_PER_SIDE; ++i) size += ability_size(&e[i].mon.ability, r->version);
    }
    return size;
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
    return fixed_size(r) + effects_size(r) + abilities_size(r) + items_size(r) +
           (r->version >= BATTLE_REQUEST_VERSION_VOLATILES ? BATTLE_VOLATILE_RULE_BYTES : 0) +
           (r->version >= BATTLE_REQUEST_VERSION_WEATHER ? BATTLE_FIELD_RULE_BYTES : 0) +
           (r->version >= BATTLE_REQUEST_VERSION_TERRAIN ? BATTLE_TERRAIN_RULE_BYTES : 0);
}

static uint16_t rules_mask(uint8_t version) {
    return version >= BATTLE_REQUEST_VERSION_ITEMS ? RULE_V17_MASK :
           version >= BATTLE_REQUEST_VERSION_WEATHER ? RULE_V15_MASK :
           version >= BATTLE_REQUEST_VERSION_ABILITY_HOOKS ? RULE_V12_MASK : RULE_V11_MASK;
}

uint32_t battle_request_encode(const BattleRequest *r, uint8_t *p, uint32_t capacity) {
    const uint32_t size = battle_request_size(r);
    const uint8_t version = r->version ? r->version : BATTLE_REQUEST_VERSION;
    if ((version < BATTLE_REQUEST_VERSION || version > BATTLE_REQUEST_VERSION_LATEST) ||
        (r->rules & ~rules_mask(version)) ||
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
                    // V13 records and V15 kinds cannot be written into an earlier request.
                    for (unsigned k = 0; k < move->effect_count; ++k) {
                        const BattleEffect *x = &move->effects[k];
                        if (version < BATTLE_REQUEST_VERSION_MODIFIERS && x->kind == BEFF_MODIFIER) return 0;
                        if (version < BATTLE_REQUEST_VERSION_WEATHER &&
                            (x->kind > BEFF_V14_LAST || (x->kind == BEFF_CHARGE && x->arg2)))
                            return 0;
                        if (version < BATTLE_REQUEST_VERSION_TERRAIN && x->kind > BEFF_V15_LAST) return 0;
                    }
                    move_effects_encode(move, q);
                    q += move_effects_size(move);
                }
        }
    if (version >= BATTLE_REQUEST_VERSION_ABILITIES)
        for (unsigned side = 0; side < 2; ++side) {
            const BattleEntry *e = side ? r->opponent : r->player;
            const unsigned count = side ? r->opponent_count : r->player_count;
            for (unsigned i = 0; i < count; ++i) {
                const BattleAbility *a = &e[i].mon.ability;
                for (unsigned k = 0; k < a->count && k < BATTLE_MAX_ABILITY_HOOKS; ++k) {
                    if (version < BATTLE_REQUEST_VERSION_MODIFIERS && a->hooks[k].kind == BABIL_MODIFIER) return 0;
                    if (version < BATTLE_REQUEST_VERSION_WEATHER && a->hooks[k].kind > BABIL_V14_LAST) return 0;
                    if (version < BATTLE_REQUEST_VERSION_TERRAIN && a->hooks[k].kind > BABIL_V15_LAST) return 0;
                }
                if (version >= BATTLE_REQUEST_VERSION_ABILITY_HOOKS) ability_encode_v12(a, q);
                else if (!ability_encode_v11(a, q)) return 0;
                q += ability_size(a, version);
            }
        }
    // V17: the item blocks follow the ability blocks; a block that does not
    // describe its record's item is not written.
    if (version >= BATTLE_REQUEST_VERSION_ITEMS)
        for (unsigned side = 0; side < 2; ++side) {
            const BattleEntry *e = side ? r->opponent : r->player;
            const unsigned count = side ? r->opponent_count : r->player_count;
            for (unsigned i = 0; i < count; ++i) {
                const BattleItem *it = &e[i].mon.item;
                if (!battle_item_valid(it, e[i].record.held_item, r->type_count, version)) return 0;
                item_encode(it, q);
                q += item_size(it);
            }
        }
    if (version >= BATTLE_REQUEST_VERSION_VOLATILES) {
        volatile_rules_encode(&r->volatiles, q);
        q += BATTLE_VOLATILE_RULE_BYTES;
    }
    if (version >= BATTLE_REQUEST_VERSION_WEATHER) {
        field_rules_encode(&r->field, q);
        q += BATTLE_FIELD_RULE_BYTES;
    }
    if (version >= BATTLE_REQUEST_VERSION_TERRAIN) terrain_rules_encode(&r->terrain, q);
    return size;
}

bool battle_request_decode(const uint8_t *p, uint32_t length, BattleRequest *r) {
    if (length < BATTLE_REQUEST_HEADER ||
        (p[0] < BATTLE_REQUEST_VERSION || p[0] > BATTLE_REQUEST_VERSION_LATEST) ||
        msg_get16(p+14) != BATTLE_ENTRY_BYTES)
        return false;
    r->version = p[0];
    r->format = p[1];
    r->battle_type = p[2];
    r->player_count = p[3];
    r->opponent_count = p[4];
    r->active_per_side = p[5];
    r->rules = msg_get16(p+6);
    // Rule bits of a later version make the request malformed (audit P-03 rule).
    if (r->rules & ~rules_mask(r->version)) return false;
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
        for (unsigned i = 0; i < BATTLE_MAX_PER_SIDE; ++i) {
            e[i].mon.ability = (BattleAbility){0};
            e[i].mon.item = (BattleItem){0};
        }
    }
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
    r->field = (BattleFieldRules){0};
    r->terrain = (BattleTerrainRules){0};
    if (r->version < BATTLE_REQUEST_VERSION_EFFECTS) return true;
    const uint8_t *end = p + length;
    if (r->version >= BATTLE_REQUEST_VERSION_TERRAIN) {  // V16: the terrain rules close the request
        if (length < fixed_size(r) + BATTLE_VOLATILE_RULE_BYTES + BATTLE_FIELD_RULE_BYTES + BATTLE_TERRAIN_RULE_BYTES)
            return false;
        end -= BATTLE_TERRAIN_RULE_BYTES;
        if (!terrain_rules_decode(end, &r->terrain, r->type_count)) return false;
    }
    if (r->version >= BATTLE_REQUEST_VERSION_WEATHER) {  // V15: the field rules close the request
        if ((uint32_t)(end - p) < fixed_size(r) + BATTLE_VOLATILE_RULE_BYTES + BATTLE_FIELD_RULE_BYTES) return false;
        end -= BATTLE_FIELD_RULE_BYTES;
        if (!field_rules_decode(end, &r->field, r->type_count)) return false;
    }
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
                const unsigned used = move_effects_decode(q, (uint32_t)(end - q), move, r->version);
                if (!used) return false;
                // Empty slots carry no effects; everything else must be well formed.
                if (move->unsupported != UNSUP_EMPTY && !move->unsupported &&
                    !battle_move_effects_valid_for(move, r->version))
                    return false;
                if (move->unsupported == UNSUP_EMPTY && move->effect_count) return false;
                // Fields of a later version are a malformed request, not an unsupported
                // move (audit P-03): V9 kinds and move flags, V10 timing.
                if (r->version < BATTLE_REQUEST_VERSION_VOLATILES && (move->move_flags & ~BMF_V8_MASK)) return false;
                if (r->version < BATTLE_REQUEST_VERSION_ABILITIES && (move->move_flags & ~BMF_V9_MASK)) return false;
                for (unsigned k = 0; k < move->effect_count; ++k) {
                    if (r->version < BATTLE_REQUEST_VERSION_TIMING && (move->effects[k].flags & BEFL_TIMING_MASK))
                        return false;
                    if (r->version < BATTLE_REQUEST_VERSION_VOLATILES && move->effects[k].kind > BEFF_PROTECT)
                        return false;
                }
                q += used;
            }
    }
    // V11/V12: the ability blocks follow the effect lists, before the volatile rules.
    if (r->version >= BATTLE_REQUEST_VERSION_ABILITIES)
        for (unsigned side = 0; side < 2; ++side) {
            BattleEntry *e = side ? r->opponent : r->player;
            const unsigned count = side ? r->opponent_count : r->player_count;
            for (unsigned i = 0; i < count; ++i) {
                BattleAbility *a = &e[i].mon.ability;
                unsigned used = BATTLE_ABILITY_BYTES;
                if (r->version >= BATTLE_REQUEST_VERSION_ABILITY_HOOKS)
                    used = ability_decode_v12(q, (uint32_t)(end - q), a, r->version);
                else if ((uint32_t)(end - q) >= BATTLE_ABILITY_BYTES)
                    ability_decode_v11(q, a);
                else
                    used = 0;
                if (!used || !battle_ability_valid(a, r->type_count, r->version) || a->source != e[i].record.ability)
                    return false;
                q += used;
            }
        }
    // V17: the item blocks follow the ability blocks.
    if (r->version >= BATTLE_REQUEST_VERSION_ITEMS)
        for (unsigned side = 0; side < 2; ++side) {
            BattleEntry *e = side ? r->opponent : r->player;
            const unsigned count = side ? r->opponent_count : r->player_count;
            for (unsigned i = 0; i < count; ++i) {
                BattleItem *it = &e[i].mon.item;
                const unsigned used = item_decode(q, (uint32_t)(end - q), it);
                if (!used || !battle_item_valid(it, e[i].record.held_item, r->type_count, r->version)) return false;
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
    // Fields of later APP versions start empty; their decoders fill them in.
    s->struggle_mask = 0;
    memset(s->status, 0, sizeof(s->status));
    memset(s->stages, 0, sizeof(s->stages));
    memset(s->volatiles, 0, sizeof(s->volatiles));
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
