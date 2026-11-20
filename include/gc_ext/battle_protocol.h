// Vendored from gba-gc-extension protocol/battle_protocol.h. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#ifndef GCEXT_BATTLE_PROTOCOL_H
#define GCEXT_BATTLE_PROTOCOL_H

// Battle application protocol (GC_MODE_BATTLE). The GBA sends one request
// with everything the GameCube battle core needs, resolved from its own game
// data; the GameCube owns the temporary battle state, RNG and turn
// resolution and returns a versioned result. All fields are little-endian.
// Multi formats use separate STATE_MULTI and ACTION_MULTI opcodes.

#include <stdbool.h>
#include <stdint.h>
#include "gc_ext/pkmn_protocol.h"
#ifdef __cplusplus
extern "C" {
#endif

#define APP_GC_BATTLE 0x4247      // bytes 'G','B'
#define BATTLE_APP_VERSION 1
#define BATTLE_APP_VERSION_ROSTER 2
#define BATTLE_APP_VERSION_SWITCH 3
#define BATTLE_APP_VERSION_STRUGGLE 4
#define BATTLE_APP_VERSION_DRAG 5
#define BATTLE_APP_VERSION_PIVOT 6
#define BATTLE_APP_VERSION_CONDITIONS 7
#define BATTLE_APP_VERSION_EFFECTS 8
#define BATTLE_APP_VERSION_VOLATILES 9
#define BATTLE_APP_VERSION_TIMING 10  // V9 + effect application units
#define BATTLE_APP_VERSION_ABILITIES 11  // V10 + ability descriptors (same state/action layout)
#define BATTLE_APP_VERSION_ABILITY_HOOKS 12  // V11 + multi-hook ability blocks (same state/action layout)
#define BATTLE_APP_VERSION_MODIFIERS 13  // V12 + shared modifier records (same state/action layout)
#define BATTLE_APP_VERSION_FIXED_POINT 14  // V13 + Gen 5+ fixed-point damage arithmetic (same layout)
#define BATTLE_APP_VERSION_WEATHER 15  // V14 + weather (same state/action layout)
#define BATTLE_APP_VERSION_TERRAIN 16  // V15 + terrain (same state/action layout)
#define BATTLE_APP_VERSION_ITEMS 17  // V16 + held items (same state/action layout)
#define BATTLE_APP_VERSION_END_TURN_ITEMS 18  // V17 + end-of-turn item HP records (same layout)
#define BATTLE_MODE_VERSION 1

enum BattleFormat { BATTLE_FORMAT_SINGLE = 1, BATTLE_FORMAT_DOUBLE = 2, BATTLE_FORMAT_TRIPLE = 3, BATTLE_FORMAT_ROTATION = 4 };
enum BattleKind { BTYPE_TRAINER = 1 };  // prefixed: games define BATTLE_TYPE_* flags
enum BattleResultPolicy { RESULT_POLICY_DISPLAY_ONLY = 0, RESULT_POLICY_APPLY_HP_PP = 1 };

// Rules: mechanics the request explicitly normalises away for this battle.
#define RULE_IGNORE_ABILITIES  (1u << 0)
// Items: before V17 the request must say so (V16: refused otherwise). From V17
// the item blocks hold every Pokemon's item; with this rule none of them has
// any effect (the battle is normalised: all items off, shown as such).
#define RULE_IGNORE_ITEMS      (1u << 1)
#define RULE_IGNORE_STATUS     (1u << 2)  // existing status is carried but has no effect
#define RULE_NO_GIMMICKS       (1u << 3)  // no Mega, Z-Move, Dynamax, Tera
#define RULE_NO_CRITICAL_HITS  (1u << 4)
#define RULE_SUPPORTED_MOVES_ONLY (1u << 5)  // unsupported moves cannot be selected
// Request V12 rules.
#define RULE_STRICT_ABILITIES  (1u << 6)  // refuse to start if any ability is UNSUPPORTED (results must be exact)
#define RULE_FIXED_SEED        (1u << 7)  // the GC seeds its RNG with the battle id (reproducible test fixtures)
// Request V15 rule.
#define RULE_STRICT_MOVES      (1u << 8)  // refuse to start if a Pokemon knows a move the core cannot run
// Request V17 rule.
#define RULE_STRICT_ITEMS      (1u << 9)  // refuse to start rather than normalise items (results must be exact)
#define RULE_V11_MASK 0x3fu
#define RULE_V12_MASK 0xffu
#define RULE_V15_MASK 0x1ffu  // V16 adds no rule
#define RULE_V17_MASK 0x3ffu  // V18 adds no rule

#define BATTLE_MAX_PER_SIDE 6
#define BATTLE_MAX_TYPES 32

// Move data, 8 bytes, resolved by the GBA from its move table.
//   0 u16 power   2 u8 type   3 u8 category   4 u8 accuracy (0 = never misses)
//   5 s8 priority 6 u8 target  7 u8 unsupported reasons (0 = supported)
enum BattleCategory { BCAT_STATUS = 0, BCAT_PHYSICAL = 1, BCAT_SPECIAL = 2 };
// Bit 7 of target bypasses Triple distance; the low bits hold the class.
enum BattleTarget {
    BTARGET_OTHER = 0, BTARGET_SINGLE = 1, BTARGET_ALL_FOES = 2,
    BTARGET_ALL_ADJACENT = 3, BTARGET_SELF = 4,
};
#define BTARGET_CLASS_MASK 0x7fu
#define BTARGET_IGNORE_DISTANCE 0x80u
#define UNSUP_EFFECT      (1u << 0)  // not a plain damaging effect
#define UNSUP_SECONDARY   (1u << 1)  // has additional effects
#define UNSUP_PRIORITY    (1u << 2)
#define UNSUP_TARGET      (1u << 3)
#define UNSUP_STATUS_MOVE (1u << 4)
#define UNSUP_MULTI_HIT   (1u << 5)
#define UNSUP_OTHER       (1u << 6)  // fixed crits, recoil-like flags, explosion ...
#define UNSUP_EMPTY       (1u << 7)  // no move in this slot
enum GcResolvedMoveEffect { BME_HIT = 0, BME_FORCE_OUT = 1, BME_FORCE_OUT_ON_HIT = 2, BME_PIVOT = 3,
    BME_STAGE_BASE = 0x10, BME_STATUS_BASE = 0x70, BME_REFRESH = 0x78, BME_ELECTRIC_PARALYSIS = 0x79,
};
enum BattleStat { BSTAT_ATK, BSTAT_DEF, BSTAT_SPA, BSTAT_SPD, BSTAT_SPE, BSTAT_ACC, BSTAT_EVA, BSTAT_COUNT };
enum BattleStatus { BST_NONE, BST_POISON, BST_TOXIC, BST_BURN, BST_PARALYSIS, BST_SLEEP, BST_FREEZE, BST_FROSTBITE };
#define BATTLE_STAGE_EFFECT(stat, delta) (BME_STAGE_BASE + (stat) * 12 + (delta) + ((delta) < 0 ? 6 : 5))
#define BATTLE_STATUS_EFFECT(status) (BME_STATUS_BASE + (status))
#define BATTLE_CONDITION(kind, counter) ((uint32_t)(kind) | (uint32_t)(counter) << 8)
#define BATTLE_CONDITION_KIND(value) ((uint8_t)(value))
#define BATTLE_CONDITION_COUNTER(value) ((uint8_t)((value) >> 8))
#define BCR_ELECTRIC_IMMUNE (1u << 0)
#define BCR_POISON_TOXIC_ALWAYS_HITS (1u << 1)
#define BCR_FROSTBITE (1u << 2)
#define BCR_DYNAMIC_SPEED (1u << 3)
#define BCR_THAW_ON_FIRE_HIT (1u << 4)
#define BCR_SLEEP_RESET_ON_SWITCH (1u << 5)
#define BATTLE_CONDITION_RULE_BYTES 12
typedef struct {
    uint8_t flags, poison_type, steel_type, fire_type, electric_type, ice_type;
    uint8_t paralysis_speed_divisor, paralysis_block_divisor, burn_residual_divisor;
    uint8_t sleep_min, sleep_max, thaw_percent;
} BattleConditionRules;
bool battle_condition_rules_valid(const BattleConditionRules *rules, unsigned type_count);
bool battle_condition_valid(uint32_t condition, const BattleConditionRules *rules);
bool battle_effect_valid(uint8_t effect, uint8_t version);

// Request V8: move effect descriptors. Every move is a list of up to
// BATTLE_MAX_EFFECTS effects, executed by the GC in list order. The GBA
// resolves them from its own move data; the GC never sees game effect IDs
// except as a diagnostic (source effect).
//   per move: u8 count, u8 move flags, u16 source effect (diagnostic only),
//             then count x 8-byte descriptors:
//   0 u8 kind   1 u8 who (BWHO_*)   2 u8 chance (0 = certain, 1..100 = percent per target)
//   3 u8 arg0   4 s8 arg1   5 u8 arg2   6 u8 flags (BEFL_*)   7 u8 reserved (0)
enum BattleEffectKind {
    BEFF_NONE = 0,
    BEFF_DAMAGE = 1,     // arg0 = min hits, arg2 = max hits, arg1 = hit rule (BHITS_*)
    BEFF_STAT = 2,       // arg0 = stat (BSTAT_*), arg1 = stage delta (-6..6, not 0)
    BEFF_STATUS = 3,     // arg0 = status (BST_*); BEFL_TYPE_CHART: fails if the move's type has no effect
    BEFF_CURE = 4,       // arg0 = mask of cured statuses (1 << BST_*)
    BEFF_HEAL = 5,       // arg0 / arg2 of max HP (rounded down, min 1); fails at full HP
    BEFF_DRAIN = 6,      // arg0 = percent of the HP the target lost (min 1)
    BEFF_RECOIL = 7,     // arg0 = percent of the HP all targets lost (min 1), once per move
    BEFF_FORCE_OUT = 8,  // the target is replaced by a random legal reserve (after damage if any)
    BEFF_PIVOT = 9,      // after a landed hit the user may switch to a reserve mid-turn
    BEFF_PROTECT = 10,   // user protected this turn; arg0 = consecutive failure base (2 or 3)
    // Request V9 (volatile states and multi-turn actions).
    BEFF_FLINCH = 11,    // after a hit: the target loses its action if it has not acted this turn
    BEFF_CONFUSE = 12,   // confusion for a duration from the volatile rules
    BEFF_SUBSTITUTE = 13,// user pays max HP / rules divisor for a substitute (status move, alone)
    BEFF_CHARGE = 14,    // first effect: turn 1 charges (arg0 = BSEMI_*), turn 2 runs the rest;
                         // V15: arg2 = weather mask in which there is no charging turn (Solar Beam)
    BEFF_RECHARGE = 15,  // after a landed hit the user must recharge on its next turn
    // Request V13.
    BEFF_MODIFIER = 16,  // passive: a damage modifier of this move (BattleModifier, docs/rule-poc.md)
    // Request V15 (docs/weather.md). BWEATHER_* masks hold BWEATHER_BIT(kind).
    BEFF_WEATHER = 17,          // status move, alone: sets weather arg0 for arg2 turns (BWEATHER_TURNS_*)
    BEFF_WEATHER_ACCURACY = 18, // passive: in a weather of mask arg0 the accuracy is arg2 (0 = never misses)
    BEFF_WEATHER_TYPE = 19,     // passive, damaging: in a weather of mask arg0 the move's type is arg2
    BEFF_WEATHER_HEAL = 20,     // passive, with BEFF_HEAL: in a weather of mask arg0 it heals arg1 / arg2
    // Request V16 (docs/terrain.md).
    BEFF_TERRAIN = 21,          // status move, alone: sets terrain arg0 for arg2 turns (1..15, 0 = no end)
    BEFF_COUNT
};
#define BEFF_V15_LAST BEFF_WEATHER_HEAL
#define BEFF_V12_LAST BEFF_RECHARGE
#define BEFF_V14_LAST BEFF_MODIFIER
enum BattleSemiInvulnerable { BSEMI_NONE = 0, BSEMI_AIR = 1, BSEMI_UNDERGROUND = 2 };
// Request V15 weather kinds (docs/weather.md). The GBA maps its own weather
// to these; primal weather, fog and strong winds have no kind yet (their
// moves and abilities stay unsupported).
enum BattleWeatherKind {  // (games have their own enum BattleWeather)
    BWEATHER_NONE = 0, BWEATHER_SUN = 1, BWEATHER_RAIN = 2, BWEATHER_SANDSTORM = 3,
    BWEATHER_HAIL = 4, BWEATHER_SNOW = 5, BWEATHER_COUNT
};
#define BWEATHER_KINDS (BWEATHER_COUNT - 1)
#define BWEATHER_BIT(kind) ((uint8_t)(1u << ((kind) - 1)))
#define BWEATHER_ALL ((uint8_t)((1u << BWEATHER_KINDS) - 1))
// A setter's duration: 1..BWEATHER_MAX_TURNS turns, BWEATHER_TURNS_NO_END, or
// BWEATHER_TURNS_KEEP (the counter of the weather before stays, as this
// build's ability weather before Gen 6 does; no counter means no end).
#define BWEATHER_TURNS_NO_END 0u
#define BWEATHER_TURNS_KEEP 0xffu
#define BWEATHER_MAX_TURNS 15u
// Request V16 terrain kinds (docs/terrain.md).
enum BattleTerrainKind {
    BTERRAIN_NONE = 0, BTERRAIN_ELECTRIC = 1, BTERRAIN_GRASSY = 2, BTERRAIN_MISTY = 3, BTERRAIN_PSYCHIC = 4,
    BTERRAIN_COUNT
};
#define BTERRAIN_KINDS (BTERRAIN_COUNT - 1)
#define BTERRAIN_BIT(kind) ((uint8_t)(1u << ((kind) - 1)))
#define BTERRAIN_ALL ((uint8_t)((1u << BTERRAIN_KINDS) - 1))
#define BTERRAIN_MAX_TURNS 15u
enum BattleEffectWho { BWHO_TARGET = 0, BWHO_USER = 1 };
enum BattleHitRule { BHITS_UNIFORM = 0, BHITS_2TO5_GEN5 = 1, BHITS_2TO5_GEN1 = 2 };
#define BATTLE_MAX_EFFECTS 6
#define BATTLE_MAX_HITS 10
#define BATTLE_EFFECT_BYTES 8
#define BATTLE_MOVE_EFFECTS_HEADER 4
#define BEFL_TYPE_CHART (1u << 0)
// Request V10: when a secondary effect runs (flags bits 1-3). 0 keeps the
// V8/V9 behaviour: once per target after its hits.
enum BattleEffectTiming {
    BETIME_DEFAULT = 0,     // = BETIME_PER_TARGET
    BETIME_PER_HIT = 1,     // after every hit that struck the Pokemon itself (multi-strike secondaries)
    BETIME_PER_TARGET = 2,  // once per target the move landed on, after its hits
    BETIME_ON_DAMAGE = 3,   // once per target whose own HP the move reduced (not only its substitute)
    BETIME_AFTER_MOVE = 4,  // once after all targets, if the move landed on any (user only)
    BETIME_PER_USE = 5,     // once per use, when the move first lands, before later targets (user only)
    BETIME_COUNT
};
#define BEFL_TIMING_SHIFT 1
#define BEFL_TIMING_MASK (7u << BEFL_TIMING_SHIFT)
#define BEFL_TIMING(t) ((uint8_t)((t) << BEFL_TIMING_SHIFT))
#define BATTLE_EFFECT_TIMING(e) (((e)->flags & BEFL_TIMING_MASK) >> BEFL_TIMING_SHIFT)
#define BMF_IGNORES_PROTECT (1u << 0)
// V9 move flags.
#define BMF_IGNORES_SUBSTITUTE  (1u << 1)  // e.g. sound moves (per the game's move flags)
#define BMF_HITS_AIRBORNE       (1u << 2)  // can hit a target in the air (Fly)
#define BMF_DOUBLE_AIRBORNE     (1u << 3)  // and deals double damage to it
#define BMF_HITS_UNDERGROUND    (1u << 4)  // can hit a target underground (Dig)
#define BMF_DOUBLE_UNDERGROUND  (1u << 5)  // and deals double damage to it
// V11 move flag.
#define BMF_CONTACT             (1u << 6)  // makes contact (the game's move data); Struggle counts
#define BMF_V8_MASK BMF_IGNORES_PROTECT
#define BMF_V9_MASK 0x3fu
#define BMF_V11_MASK 0x7fu
// Request V13: a modifier record, shared by moves (BEFF_MODIFIER in an
// effect list) and abilities (BABIL_MODIFIER in a hook list). The record is
// the kind byte followed by the 7-byte modifier body:
//   1 u8 stage: bits 0-2 BMOD_*, bits 3-4 move category (0 any, BCAT_PHYSICAL,
//        BCAT_SPECIAL), bits 5-6 zero, bit 7 BMODF_DEFENDER (abilities only:
//        applies while the holder is the defending Pokemon, else the attacker)
//   2 u8 percent (1..255): the value is multiplied by percent / 100
//   3 u8 flags: BAHF_BREAKABLE in a hook list, 0 in an effect list
//   4 u8 predicate 0 kind (BPRED_* | BPRED_DEFENDER)  5 u8 predicate 0 argument
//   6 u8 predicate 1 kind                              7 u8 predicate 1 argument
// The modifier applies when both predicates hold (an empty one holds). It
// never draws a random number. Where each stage applies, how it rounds and
// its minimum are fixed by the core, not by the record (docs/modifiers.md):
// V13 floors after each modifier; V14 (BATTLE_REQUEST_VERSION_FIXED_POINT)
// chains the stage's modifiers in 4.12 fixed point and rounds once, as the
// Gen 5+ damage formula does. BASE_POWER is integer in both.
// Request V15 adds three stages, written with bit 5 as the stage's bit 3
// (always 0 before V15): WEATHER (the weather step of the damage formula,
// first matching field record only), and the integer SPEED and ACCURACY.
// Request V17 gives bit 6 a meaning (BMOD_FLOORED, any source, fixed-point
// stages only): the percent becomes its 4.12 factor by truncation instead of
// to the nearest value (4096 * 110 / 100 = 4505, not 4506), as the games'
// floored constants do. Before V17 bit 6 stays reserved (a record with it set
// is refused).
enum BattleModifierStage {
    BMOD_NONE = 0,
    BMOD_BASE_POWER = 1,  // the move's base power (e.g. Hex), before the power modifiers; floor, min 1
    BMOD_POWER = 2,       // power modifiers (Technician, Brine); min 1
    BMOD_ATTACK = 3,      // attacking stat after stages; min 1
    BMOD_DEFENSE = 4,     // defending stat after stages; min 1
    BMOD_FINAL = 5,       // damage after type and burn (the damage minimum of 1 follows)
    BMOD_V14_COUNT,
    // Request V15.
    BMOD_WEATHER = 6,     // damage after the spread factor, before the random factor (field records)
    BMOD_SPEED = 7,       // the holder's speed after stages, before paralysis; floor, min 1 (abilities)
    BMOD_ACCURACY = 8,    // accuracy after stages and the V12 accuracy hook; floor (abilities)
    BMOD_COUNT
};
#define BMOD_STAGE_MASK 0x07u
#define BMOD_STAGE_HIGH 0x20u  // V15: bit 3 of the stage
#define BMOD_CATEGORY_SHIFT 3
#define BMOD_CATEGORY_MASK (3u << BMOD_CATEGORY_SHIFT)
#define BMODF_DEFENDER 0x80u
#define BMOD_FLOORED 0x40u     // V17: the percent's 4.12 factor is truncated (see above)
#define BATTLE_MODIFIER_STAGE(stage, category) \
    ((uint8_t)(((stage) & BMOD_STAGE_MASK) | ((stage) & 8u ? BMOD_STAGE_HIGH : 0u) | (category) << BMOD_CATEGORY_SHIFT))
static inline uint8_t battle_modifier_stage(uint8_t stage_byte) {
    return (uint8_t)((stage_byte & BMOD_STAGE_MASK) | ((stage_byte & BMOD_STAGE_HIGH) >> 2));
}
enum BattlePredicateKind {
    BPRED_NONE = 0,
    BPRED_HP_AT_MOST = 1,     // subject's HP <= max HP / arg (arg >= 2)
    BPRED_STATUS = 2,         // subject's major status is in arg (mask of 1 << BST_*)
    BPRED_MOVE_TYPE = 3,      // the move's type is arg (never for typeless Struggle); abilities and field
    BPRED_POWER_AT_MOST = 4,  // the move's power after BMOD_BASE_POWER <= arg; abilities and items, not at BASE_POWER
    BPRED_V14_COUNT,
    // Request V15.
    BPRED_WEATHER = 5,        // the weather in effect is in arg (mask of BWEATHER_BIT); none while suppressed
    BPRED_HAS_TYPE = 6,       // subject has type arg
    BPRED_V15_COUNT,
    // Request V16.
    BPRED_TERRAIN = 7,        // the terrain in effect is in arg (mask of BTERRAIN_BIT); no subject
    BPRED_ON_TERRAIN = 8,     // subject is affected by a terrain of arg: grounded and not semi-invulnerable
    BPRED_EXPOSED = 9,        // subject is not semi-invulnerable (arg 0)
    BPRED_COUNT
};
#define BPRED_KIND_MASK 0x3fu
#define BPRED_DEFENDER 0x80u  // HP / STATUS: the subject is the defending Pokemon (else the attacker)
#define BATTLE_MAX_PREDICATES 2
#define BATTLE_MODIFIER_BODY_BYTES 7
typedef struct { uint8_t kind, arg; } BattlePredicate;
// The modifier without its flags byte (record bytes 1, 2 and 4..7). The
// flags belong to the effect or hook slot that holds it.
typedef struct {
    uint8_t stage, percent;
    BattlePredicate pred[BATTLE_MAX_PREDICATES];
} BattleModifier;
// Where a record is: in a move's effect list, an ability's hook list (BMODF_DEFENDER
// and the move predicates allowed), the V15 field rules (weather and field stat records)
// or a V17 item block (the holder's item; V17: the power of the holder's moves).
enum BattleModifierSource { BMSRC_MOVE = 0, BMSRC_ABILITY = 1, BMSRC_FIELD = 2, BMSRC_ITEM = 3 };
bool battle_modifier_valid(const BattleModifier *modifier, unsigned type_count, uint8_t source, uint8_t version);
// The 7-byte record body (after the kind byte) and the slot's flags.
void battle_modifier_put(const BattleModifier *modifier, uint8_t flags, uint8_t *body);
void battle_modifier_get(const uint8_t *body, BattleModifier *modifier, uint8_t *flags);

// Effects and hooks are 8 bytes in memory, as on the wire: the kind, the
// fields of that kind, then the flags. A modifier record shares the storage
// of the legacy fields, so code reads `mod` only for BEFF_MODIFIER /
// BABIL_MODIFIER and the legacy fields only for the other kinds; `kind` and
// `flags` are valid for every kind. The legacy field order is unchanged, so
// positional initializers keep their meaning.
typedef struct {
    uint8_t kind;
    union {
        struct {
            uint8_t who, chance, arg0;
            int8_t arg1;
            uint8_t arg2;
        };
        BattleModifier mod;  // BEFF_MODIFIER
    };
    uint8_t flags;  // BEFL_* (0 for BEFF_MODIFIER)
} BattleEffect;

typedef struct {
    uint16_t power;
    uint8_t type, category, accuracy;
    int8_t priority;
    uint8_t target, unsupported, effect;
    // V8 effect list (legacy effect bytes are translated by the core).
    uint8_t effect_count, move_flags;
    uint16_t source_effect;
    BattleEffect effects[BATTLE_MAX_EFFECTS];
} BattleMove;
// Structural check of a V8 effect list against its move (shared by both sides),
// for a request version (kinds and fields of a later version are refused);
// battle_move_effects_valid checks against the latest version.
bool battle_move_effects_valid_for(const BattleMove *move, uint8_t version);
bool battle_move_effects_valid(const BattleMove *move);
const char *battle_effect_kind_name(uint8_t kind);

// BattleMonV1, 40 bytes, appended to each PokemonRecordV1 in the request.
//   0 u8 version (1)  1 u8 type count  2 u8 types[2]
//   4..7 resolved effect for each move (request V5); reserved in earlier requests
//   8 BattleMove[4]
#define BATTLE_MON_VERSION 1
#define BATTLE_MON_BYTES 40
// Abilities (docs/abilities.md), resolved by the GBA from its game data and
// configuration. The GC never interprets game ability ids.
//
// Request V11: one 8-byte descriptor per entry:
//   0 u8 kind (BABIL_*)  1 u8 arg0  2 s8 arg1  3 u8 arg2  4 u8 chance (percent, 0 = certain)
//   5 u8 percent         6 u16 source ability id (= the record's ability id)
// Request V12: a variable-length block per entry, an ability being a list of hooks:
//   0 u8 class (BACLASS_*)  1 u8 hook count (0 unless HOOKS, then 1..BATTLE_MAX_ABILITY_HOOKS)
//   2 u16 source ability id, then count x 8-byte hooks:
//   0 u8 kind (BABIL_ATTACK_MULT..)  1 u8 arg0  2 s8 arg1  3 u8 arg2
//   4 u8 chance numerator  5 u8 chance denominator (both 0 = certain)  6 u8 percent  7 u8 flags (BAHF_*)
// Both decode into BattleAbility; a V11 descriptor becomes one hook with chance/100.
enum BattleAbilityClass { BACLASS_NONE = 0, BACLASS_UNSUPPORTED = 1, BACLASS_HOOKS = 2 };
enum BattleAbilityKind {
    BABIL_NONE = 0,             // no battle effect in this model
    BABIL_UNSUPPORTED = 1,      // has a battle effect the core does not run: shown, inert
    BABIL_ATTACK_MULT = 2,      // arg0 = category (BCAT_PHYSICAL/SPECIAL); attacking stat x percent
    BABIL_PINCH = 3,            // arg0 = type, arg2 = HP divisor; attacking stat x percent at HP <= max/arg2
    BABIL_LOW_POWER = 4,        // arg0 = max base power; power x percent
    BABIL_RESIST = 5,           // arg0 = type, arg2 = second type or BABIL_NO_TYPE; attacker's stat x percent
    BABIL_TYPE_IMMUNE = 6,      // arg0 = type: damaging and type-chart status moves of it have no effect
    BABIL_TYPE_ABSORB = 7,      // arg0 = type: moves of it heal percent of max HP instead
    BABIL_STATUS_IMMUNE = 8,    // arg0 = mask of 1 << BST_*
    BABIL_VOLATILE_IMMUNE = 9,  // arg0 = mask of BABIL_VOL_*
    BABIL_STAT_DROP_IMMUNE = 10,// arg0 = mask of 1 << BSTAT_*; arg2 bit 0: ignores target evasion
    BABIL_ACCURACY = 11,        // accuracy x percent
    BABIL_NO_GUARD = 12,        // moves by and against the holder always hit
    BABIL_CONTACT_STATUS = 13,  // arg0 = status, chance: on a contact hit, the attacker
    BABIL_CONTACT_DAMAGE = 14,  // arg2 = divisor of the attacker's max HP, on a contact hit
    BABIL_ENTRY_STAT = 15,      // arg0 = stat, arg1 = delta: every foe on the field, on entry
    BABIL_END_TURN_STAT = 16,   // arg0 = stat, arg1 = delta: the holder, at the end of a turn
    BABIL_STURDY = 17,          // survives a knock-out hit from full HP with 1 HP
    BABIL_NO_RECOIL = 18,       // no recoil from the holder's moves (not Struggle)
    BABIL_SHIELD_DUST = 19,     // secondary effects of others' moves do not affect the holder
    BABIL_SECONDARY_CHANCE = 20,// secondary effect chances of the holder's moves x percent
    BABIL_INFILTRATOR = 21,     // the holder's moves pass substitutes
    // Request V12 hook kinds.
    BABIL_DEFENSE_MULT = 22,    // arg0 = category; the holder's defending stat x percent (BAHF_REQUIRES_STATUS)
    BABIL_IGNORE_BURN = 23,     // burn does not halve the holder's physical damage
    BABIL_STAB_MULT = 24,       // same-type bonus = percent (instead of 150)
    BABIL_KO_STAT = 25,         // arg0 = stat, arg1 = delta per Pokemon the holder's move knocked out
    BABIL_END_TURN_CURE = 26,   // arg0 = mask of 1 << BST_*; chance at the end of a turn, before residuals
    BABIL_ABSORB_STATE = 27,    // arg0 = type: moves of it are absorbed and set the holder's ability state
    BABIL_TYPE_PASS = 28,       // arg0 = attacking type, arg2 = defending type: that immunity becomes neutral
    BABIL_STATUS_MIRROR = 29,   // arg0 = mask of 1 << BST_*: a status another battler gives the holder goes back
    // Request V13 hook kind.
    BABIL_MODIFIER = 30,        // a damage modifier (BattleModifier in `mod`; the other fields are 0)
    // Request V15 hook kinds (docs/weather.md).
    BABIL_ENTRY_WEATHER = 31,   // on entry: sets weather arg0 for arg2 turns (BWEATHER_TURNS_*)
    BABIL_WEATHER_SUPPRESS = 32,// while the holder is on the field no weather has an effect (Cloud Nine)
    BABIL_WEATHER_IMMUNE = 33,  // arg0 = weather mask: no residual damage from those weathers
    BABIL_WEATHER_RESIDUAL = 34,// arg0 = weather mask: instead of the weather's residual, the holder
                                // heals (arg1 = 1) or loses (arg1 = -1) max HP / arg2 (BAHF_NOT_UNDERGROUND)
    // Request V16 hook kinds (docs/terrain.md).
    BABIL_ENTRY_TERRAIN = 35,   // on entry: sets terrain arg0 for arg2 turns (1..15, 0 = no end)
    BABIL_UNGROUNDED = 36,      // the holder is not grounded (Levitate)
    BABIL_COUNT
};
#define BABIL_V15_LAST BABIL_WEATHER_RESIDUAL
#define BABIL_V11_LAST BABIL_INFILTRATOR
#define BABIL_V12_LAST BABIL_STATUS_MIRROR
#define BABIL_V14_LAST BABIL_MODIFIER
#define BABIL_NO_TYPE 0xffu
#define BABIL_VOL_FLINCH (1u << 0)
#define BABIL_VOL_CONFUSION (1u << 1)
#define BABIL_VOL_INTIMIDATE (1u << 2)
#define BABIL_IGNORE_EVASION (1u << 0)
// ENTRY_STAT arg2: which foes (V11 always 0).
enum BattleAbilityRange { BRANGE_ALL_FOES = 0, BRANGE_ADJACENT_FOES = 1 };
// Hook flags (V12).
#define BAHF_REQUIRES_STATUS (1u << 0)  // only while the holder has a major status
#define BAHF_REQUIRES_STATE  (1u << 1)  // only while the holder's ability state is set (ABSORB_STATE)
#define BAHF_TYPE_FILTER     (1u << 2)  // ATTACK_MULT: only moves of type arg2
#define BAHF_NOT_FROZEN      (1u << 3)  // ABSORB_STATE: not while the holder is frozen
#define BAHF_TOXIC_TO_POISON (1u << 4)  // STATUS_MIRROR: bad poison goes back as poison
#define BAHF_NOT_UNDERGROUND (1u << 5)  // V15 WEATHER_RESIDUAL: not while the holder is underground
#define BAHF_BREAKABLE       (1u << 7)  // the game marks the ability breakable (Mold Breaker); carried, no effect yet
#define BATTLE_ABILITY_BYTES 8          // V11 descriptor
#define BATTLE_ABILITY_HEADER_BYTES 4   // V12 block header
#define BATTLE_ABILITY_HOOK_BYTES 8     // V12 hook
#define BATTLE_MAX_ABILITY_HOOKS 4
typedef struct {
    uint8_t kind;
    union {
        struct {
            uint8_t arg0;
            int8_t arg1;
            uint8_t arg2, chance, chance_den, percent;
        };
        BattleModifier mod;  // BABIL_MODIFIER
    };
    uint8_t flags;  // BAHF_* (only BAHF_BREAKABLE for BABIL_MODIFIER)
} BattleAbilityHook;
typedef struct {
    uint8_t cls, count;  // BACLASS_*; hooks used
    uint16_t source;     // the game's ability id (diagnostic and names only)
    BattleAbilityHook hooks[BATTLE_MAX_ABILITY_HOOKS];
} BattleAbility;
bool battle_ability_hook_valid(const BattleAbilityHook *hook, unsigned type_count, uint8_t version);
bool battle_ability_valid(const BattleAbility *ability, unsigned type_count, uint8_t version);
const char *battle_ability_kind_name(uint8_t kind);

// Request V17 held items (docs/items.md), resolved by the GBA from its item
// data and configuration. The GC never interprets game item ids. One block
// per entry, after the ability blocks:
//   0 u8 class (BICLASS_*)  1 u8 record count (0 unless RECORDS, then 1..BATTLE_MAX_ITEM_RECORDS)
//   2 u16 source item id (= the record's held item; 0 = no item, class NONE), then count x 8-byte records:
//   0 u8 kind (BITEM_*), then for
//     BITEM_MODIFIER:      the 7-byte modifier body (flags byte 0; BMSRC_ITEM)
//     BITEM_WEATHER_TURNS: 1 u8 weather mask (BWEATHER_BIT)  2 u8 0  3 u8 turns (1..BWEATHER_MAX_TURNS)  4..7 0
//     BITEM_TERRAIN_TURNS: 1 u8 terrain mask (BTERRAIN_BIT)  2 u8 0  3 u8 turns (1..BTERRAIN_MAX_TURNS)  4..7 0
//     BITEM_HP (V18):      1 u8 timing (BITIME_*)  2 u8 operation (BIHP_*)  3 u8 max HP divisor (>= 1)
//                          4 u8 predicate kind (BPRED_NONE or BPRED_HAS_TYPE, the holder)  5 u8 its argument
//                          6 u8 0  7 u8 flags 0
//       The holder's HP changes by max HP / divisor (at least 1): a heal up to
//       its max HP (none at full HP), or damage (it may faint). At one timing
//       the first BITEM_HP record whose predicate holds is the item's effect
//       there; the later ones are its alternatives (Black Sludge: heal if
//       Poison, else damage). A heal at full HP is that effect doing nothing.
// A definition is the item's and never depends on its holder (no species or
// ability of the holder is resolved into it), so it stays right if the item
// later changes hands. NONE: no item, or an item with no battle effect in
// this model. UNSUPPORTED: an item with an effect the core cannot run; the
// core refuses such a battle unless RULE_IGNORE_ITEMS normalises every item.
enum BattleItemClass { BICLASS_NONE = 0, BICLASS_UNSUPPORTED = 1, BICLASS_RECORDS = 2 };
enum BattleItemKind {
    BITEM_NONE = 0,
    BITEM_MODIFIER = 1,       // a modifier of the holder's moves (BattleModifier in `mod`)
    BITEM_WEATHER_TURNS = 2,  // a weather of mask arg0 the holder sets for a limited time lasts arg2 turns
    BITEM_TERRAIN_TURNS = 3,  // a terrain of mask arg0 the holder sets for a limited time lasts arg2 turns
    BITEM_V17_COUNT,
    // Request V18 (docs/items.md).
    BITEM_HP = 4,             // a change of the holder's HP at a timing (`hp`)
    BITEM_COUNT
};
// When a BITEM_HP record acts. Each timing is one fixed point of the core's
// turn sequence (docs/items.md, "Order"); later stages add timings.
enum BattleItemTiming {
    BITIME_NONE = 0,
    BITIME_END_TURN = 1,  // end of turn, the holder's first block: after its terrain heal and Shed Skin
    BITIME_COUNT
};
enum BattleItemHpOperation { BIHP_NONE = 0, BIHP_HEAL = 1, BIHP_DAMAGE = 2, BIHP_COUNT };
#define BATTLE_ITEM_HEADER_BYTES 4
#define BATTLE_ITEM_RECORD_BYTES 8
#define BATTLE_MAX_ITEM_RECORDS 4
typedef struct {
    uint8_t kind;
    union {
        struct { uint8_t arg0, arg1, arg2; };  // WEATHER_TURNS / TERRAIN_TURNS (arg1 0)
        BattleModifier mod;                    // BITEM_MODIFIER
        struct {
            uint8_t timing, op, divisor;       // BITIME_*, BIHP_*, max HP / divisor
            BattlePredicate pred;              // on the holder
            uint8_t reserved;                  // 0
        } hp;                                  // BITEM_HP (V18)
    };
    uint8_t flags;  // 0 (reserved)
} BattleItemRecord;
typedef struct {
    uint8_t cls, count;  // BICLASS_*; records used
    uint16_t source;     // the game's item id (diagnostic and names only)
    BattleItemRecord records[BATTLE_MAX_ITEM_RECORDS];
} BattleItem;
// `held_item` is the entry record's item: the block must name the same one.
bool battle_item_valid(const BattleItem *item, uint16_t held_item, unsigned type_count, uint8_t version);
const char *battle_item_kind_name(uint8_t kind);

typedef struct {
    uint8_t type_count, types[2];
    // Source compatibility for legacy producers; not encoded or used by GC.
    int8_t front_y, back_y;
    uint8_t elevation;
    BattleMove moves[4];
    BattleAbility ability;  // V11 suffix block (not in the 40-byte record)
    BattleItem item;        // V17 suffix block (zero before V17)
} BattleMon;

typedef struct {
    PokemonRecord record;
    BattleMon mon;
} BattleEntry;
#define BATTLE_ENTRY_BYTES (PKMN_RECORD_BYTES + BATTLE_MON_BYTES)

// GcBattleRequestV1 (the GC_MODE_BATTLE mode request asset).
//   0 u8 request version (1)   1 u8 format   2 u8 battle type   3 u8 player count
//   4 u8 opponent count        5 u8 active per side             6 u16 rules
//   8 u16 trainer id          10 u8 trainer class              11 u8 reward mode (0 none)
//  12 u8 result policy        13 u8 type count N               14 u16 entry length (140)
//  16 u32 battle id           20 u8 battle environment (game id)  21..23 reserved
//  24 entries: player entries, then opponent entries (PokemonRecordV1 + BattleMonV1)
//  .. type chart: N*N u8, [attack type][defend type], multiplier in quarters (0,2,4,8)
#define BATTLE_REQUEST_VERSION 1
#define BATTLE_REQUEST_VERSION_ROSTER 2
#define BATTLE_REQUEST_VERSION_SWITCH 3
#define BATTLE_REQUEST_VERSION_STRUGGLE 4
#define BATTLE_REQUEST_VERSION_DRAG 5
#define BATTLE_REQUEST_VERSION_PIVOT 6
#define BATTLE_REQUEST_VERSION_CONDITIONS 7
#define BATTLE_REQUEST_VERSION_EFFECTS 8
#define BATTLE_REQUEST_VERSION_VOLATILES 9
#define BATTLE_REQUEST_VERSION_TIMING 10  // effect descriptors may carry BEFL_TIMING
#define BATTLE_REQUEST_VERSION_ABILITIES 11  // ability block after the effect lists; BMF_CONTACT
#define BATTLE_REQUEST_VERSION_ABILITY_HOOKS 12  // multi-hook ability blocks; strict / fixed-seed rules
#define BATTLE_REQUEST_VERSION_MODIFIERS 13  // BEFF_MODIFIER / BABIL_MODIFIER records (same layout as V12)
#define BATTLE_REQUEST_VERSION_FIXED_POINT 14  // same layout as V13; 4.12 damage arithmetic (docs/modifiers.md)
#define BATTLE_REQUEST_VERSION_WEATHER 15  // V14 + field rules after the volatile rules; weather (docs/weather.md)
#define BATTLE_REQUEST_VERSION_TERRAIN 16  // V15 + terrain rules after the field rules (docs/terrain.md)
#define BATTLE_REQUEST_VERSION_ITEMS 17  // V16 + item blocks after the ability blocks (docs/items.md)
#define BATTLE_REQUEST_VERSION_END_TURN_ITEMS 18  // V17 + BITEM_HP records (same layout, docs/items.md)
#define BATTLE_REQUEST_VERSION_LATEST BATTLE_REQUEST_VERSION_END_TURN_ITEMS
#define BATTLE_REQUEST_HEADER 24
enum StruggleRecoil { BSTRUGGLE_MAX_HP_FLOOR = 1, BSTRUGGLE_MAX_HP_NEAREST = 2 };
#define BATTLE_MOVE_STRUGGLE 0xffffu  // logical presentation identifier
// V9 volatile rules, 8 bytes after the V8 effect lists.
//   0 u8 flags (BVR_*)   1 u8 confusion self-hit percent   2 u8 confusion min turns
//   3 u8 confusion max turns (the counter; the last tick ends confusion)
//   4 u8 confusion self-hit power   5 u8 substitute HP divisor   6..7 reserved (0)
#define BATTLE_VOLATILE_RULE_BYTES 8
#define BVR_SUBSTITUTE_MULTI_HIT_CONTINUES (1u << 0)  // hits after a broken substitute strike the Pokemon
#define BVR_RECHARGE_SKIP_ON_KO (1u << 1)             // no recharge after knocking the target out
#define BVR_CONFUSION_BURN (1u << 2)                  // burn halves the confusion self-hit
typedef struct {
    uint8_t flags, confusion_hit_percent, confusion_min, confusion_max, confusion_power, substitute_divisor;
} BattleVolatileRules;
bool battle_volatile_rules_valid(const BattleVolatileRules *rules);

// V15 field rules, BATTLE_FIELD_RULE_BYTES after the volatile rules (docs/weather.md):
//   0 u8 flags (BFR_*)   1 u8 battle-start weather (BWEATHER_*, 0 none)
//   2 u8 its turns (1..BWEATHER_MAX_TURNS, 0 = no end)   3 u8 field record count N
//   4 per weather kind BWEATHER_SUN..BWEATHER_SNOW, 5 bytes each:
//        u8 residual divisor (0 = no residual damage)  u8 flags (BWKF_*)
//        u8 immune types[3] (BABIL_NO_TYPE = none; immune to the residual damage)
//  29..31 reserved (0)
//  32 BATTLE_MAX_FIELD_MODIFIERS x 7-byte modifier record bodies (stage, percent,
//     flags = 0, two predicates); the first N are used, the rest are zero.
// Field records hold the rules that belong to no move or ability: the weather
// step of the damage (BMOD_WEATHER) and the weather stat boosts (BMOD_DEFENSE).
#define BATTLE_MAX_FIELD_MODIFIERS 8
#define BATTLE_FIELD_KIND_BYTES 5
#define BATTLE_FIELD_RULE_BYTES (32 + BATTLE_MAX_FIELD_MODIFIERS * BATTLE_MODIFIER_BODY_BYTES)
#define BFR_START_LOCKED (1u << 0)        // the battle-start weather cannot be replaced (overworld weather, Gen 9)
#define BWKF_UNDERGROUND_SAFE (1u << 0)   // an underground Pokemon takes no residual damage
#define BWKF_NO_FREEZE (1u << 1)          // no Pokemon can be frozen (or get frostbite) in this weather
#define BWKF_MASK 0x3u
#define BATTLE_WEATHER_IMMUNE_TYPES 3
typedef struct {
    uint8_t residual_divisor, flags;
    uint8_t immune_types[BATTLE_WEATHER_IMMUNE_TYPES];
} BattleWeatherKindRules;
typedef struct {
    uint8_t flags, start_weather, start_turns, modifier_count;
    BattleWeatherKindRules weather[BWEATHER_KINDS];  // index kind - 1
    BattleModifier modifiers[BATTLE_MAX_FIELD_MODIFIERS];
} BattleFieldRules;
bool battle_field_rules_valid(const BattleFieldRules *rules, unsigned type_count);

// V16 terrain rules, BATTLE_TERRAIN_RULE_BYTES after the V15 field rules (docs/terrain.md):
//   0 u8 flags (0)   1 u8 battle-start terrain (BTERRAIN_*, 0 none)   2 u8 its turns (0 = no end)
//   3 u8 airborne type: a Pokemon of it is not grounded (BABIL_NO_TYPE = none)
//   4 u8 terrain record count N   5..7 reserved (0)
//   8 per terrain kind BTERRAIN_ELECTRIC..BTERRAIN_PSYCHIC, 2 bytes each:
//        u8 end-of-turn heal divisor for an affected Pokemon (0 = none)   u8 flags (BTKF_*)
//  16 BATTLE_MAX_TERRAIN_MODIFIERS x 7-byte modifier record bodies (flags 0); the first N are used.
// The terrain records are field records after the V15 ones; V16 field records
// may also use the POWER stage (in the formula after the move's own records).
#define BATTLE_MAX_TERRAIN_MODIFIERS 8
#define BATTLE_TERRAIN_RULE_BYTES (16 + BATTLE_MAX_TERRAIN_MODIFIERS * BATTLE_MODIFIER_BODY_BYTES)
#define BTKF_NO_SLEEP (1u << 0)        // an affected Pokemon cannot fall asleep
#define BTKF_NO_STATUS (1u << 1)       // an affected Pokemon gets no major status
#define BTKF_NO_CONFUSION (1u << 2)    // an affected Pokemon cannot become confused
#define BTKF_BLOCK_PRIORITY (1u << 3)  // a foe's move with priority > 0 fails against an affected Pokemon
#define BTKF_MASK 0xfu
typedef struct { uint8_t heal_divisor, flags; } BattleTerrainKindRules;
typedef struct {
    uint8_t flags, start_terrain, start_turns, airborne_type, modifier_count;
    BattleTerrainKindRules terrain[BTERRAIN_KINDS];  // index kind - 1
    BattleModifier modifiers[BATTLE_MAX_TERRAIN_MODIFIERS];
} BattleTerrainRules;
bool battle_terrain_rules_valid(const BattleTerrainRules *rules, unsigned type_count);

// The largest request (V9 and later: every move with the most effects, every
// ability with the most hooks, the V15 field rules) for `entries` Pokemon in
// all and `types` types (the V16 terrain rules and V17 item blocks with the
// most records included). Producers size their buffer with it, so a valid
// request of any version always fits.
#define BATTLE_REQUEST_MAX_BYTES(entries, types)     (BATTLE_REQUEST_HEADER + (entries) * BATTLE_ENTRY_BYTES + (types) * (types) + BATTLE_CONDITION_RULE_BYTES +      (entries) * 4 * (BATTLE_MOVE_EFFECTS_HEADER + BATTLE_MAX_EFFECTS * BATTLE_EFFECT_BYTES) +      (entries) * (BATTLE_ABILITY_HEADER_BYTES + BATTLE_MAX_ABILITY_HOOKS * BATTLE_ABILITY_HOOK_BYTES) +      (entries) * (BATTLE_ITEM_HEADER_BYTES + BATTLE_MAX_ITEM_RECORDS * BATTLE_ITEM_RECORD_BYTES) +      BATTLE_VOLATILE_RULE_BYTES + BATTLE_FIELD_RULE_BYTES + BATTLE_TERRAIN_RULE_BYTES)

typedef struct {
    uint8_t format, battle_type, player_count, opponent_count, active_per_side;
    uint16_t rules, trainer_id;
    uint8_t trainer_class, reward_mode, result_policy, type_count;
    uint32_t battle_id;
    uint8_t environment;
    uint8_t version; // zero in callers constructing a legacy V1 request
    uint8_t struggle_power, struggle_accuracy, struggle_recoil; // V4 header bytes 21..23
    BattleEntry player[BATTLE_MAX_PER_SIDE], opponent[BATTLE_MAX_PER_SIDE];
    uint8_t type_chart[BATTLE_MAX_TYPES * BATTLE_MAX_TYPES];
    BattleConditionRules conditions; // V7 suffix after the type chart
    BattleVolatileRules volatiles;   // V9 suffix after the effect lists
    BattleFieldRules field;          // V15 suffix after the volatile rules
    BattleTerrainRules terrain;      // V16 suffix after the field rules
} BattleRequest;

uint32_t battle_request_size(const BattleRequest *request);
uint32_t battle_request_encode(const BattleRequest *request, uint8_t *out, uint32_t capacity);
bool battle_request_decode(const uint8_t *in, uint32_t length, BattleRequest *request);

// Legacy UI asset IDs (0x02xx) for older client/host pairs. The native GC
// presentation does not request these. Keep IDs reserved for compatibility.
enum {
    ASSET_BATTLE_BG = 0x0201,         // id = environment; PIXEL_4BPP_TILEMAP 30x20 (240x160), opaque
    ASSET_BATTLE_TEXTBOX = 0x0202,    // id 0; PIXEL_4BPP_TILEMAP 30x6, screen rows 14-19
    ASSET_BATTLE_HEALTHBOX = 0x0203,  // id = side (0 player, 1 opponent), variant = level, extra = personality;
                                      // frame with nickname/gender/Lv printed by the game; 128x64 / 128x32
    ASSET_BATTLE_HPBAR = 0x0204,      // id 0; tiles H,P, green 0-8, yellow 0-8, red 0-8 (29 x 8x8) + bar palette
    ASSET_BATTLE_DIGITS = 0x0205,     // id 0; healthbox-font glyphs 0-9 and '/' in 8x16 cells
    ASSET_BATTLE_MESSAGE = 0x0206,    // id = BMSG | side << 8, variant = move, extra = personality / trainer;
                                      // a 208x32 message window rendered with the game's battle text
};
enum BattleMessageId {
    BMSG_TRAINER_INTRO = 1, BMSG_TRAINER_SENT_OUT = 2, BMSG_GO_PKMN = 3, BMSG_USED_MOVE = 4,
    BMSG_MISSED = 5, BMSG_SUPER_EFFECTIVE = 6, BMSG_NOT_VERY_EFFECTIVE = 7, BMSG_NO_EFFECT = 8,
    BMSG_FAINTED = 9, BMSG_WIN = 10, BMSG_LOSE = 11, BMSG_FORFEIT = 12, BMSG_WHAT_WILL_DO = 13,
};
#define BMSG_KEY(msg, side) ((uint32_t)(msg) | (uint32_t)(side) << 8)
#define HPBAR_TILE_H 0
#define HPBAR_TILE_GREEN 2
#define HPBAR_TILE_YELLOW 11
#define HPBAR_TILE_RED 20
#define HPBAR_TILES 29

// APP opcodes of APP_GC_BATTLE.
enum BattleOp {
    BATTLE_OP_STATE = 1,   // host -> GBA: BattleState (args); empty reply
    BATTLE_OP_ACTION = 2,  // host polls the GBA: reply BattleAction
    BATTLE_OP_STATE_MULTI = 3,
    BATTLE_OP_ACTION_MULTI = 4,
    BATTLE_OP_STATE_ROSTER = 5,
    BATTLE_OP_ACTION_ROSTER = 6,
};

// BattleState, 16 bytes.
//   0 u8 phase   1 u8 turn   2 u8 outcome (when ENDED)  3 u8 player slot
//   4 u16 player hp   6 u16 player max hp   8 u16 opponent hp   10 u16 opponent max hp
//  12 u8 pp[4] of the active player battler
enum BattlePhase { BPHASE_WAIT = 1, BPHASE_CHOOSE = 2, BPHASE_ENDED = 3, BPHASE_REPLACE = 4, BPHASE_PIVOT = 5 };
#define BATTLE_STATE_BYTES 16
typedef struct {
    uint8_t phase, turn, outcome, player_slot;
    uint16_t player_hp, player_max_hp, opponent_hp, opponent_max_hp;
    uint8_t pp[4];
} BattleState;

// BattleAction, 4 bytes: u8 turn, u8 action, u8 move slot, u8 flags.
// The turn echoes the CHOOSE state it answers, so stale input is ignored.
enum BattleActionKind { BACTION_NONE = 0, BACTION_MOVE = 1, BACTION_FORFEIT = 2, BACTION_SHIFT = 3, BACTION_REPLACE = 4, BACTION_SWITCH = 5, BACTION_STRUGGLE = 6, BACTION_PIVOT = 7 };
#define BACTION_FLAG_EXIT (1u << 0)
#define BATTLE_ACTION_BYTES 4
typedef struct {
    uint8_t turn, action, move_slot, flags;
} BattleAction;

// Multi-format commands use a per-selection token. The GBA may repeat the
// reply until it receives the next state. A delayed reply cannot answer a
// different battler's prompt in the same turn.
#define BATTLE_FIELD_SLOTS 3
#define BATTLE_SLOT_NONE 0xffu
#define BATTLE_TARGET_AUTO 0x80u  // the remaining bits are a legal foe mask
#define BATTLE_MULTI_SHIFT (1u << 0)
enum BattleRotation { BROT_KEEP = 0, BROT_LEFT = 1, BROT_RIGHT = 2 };
enum BattlePositionSlot { BPOS_LEFT = 0, BPOS_CENTER = 1, BPOS_RIGHT = 2 };

typedef struct {
    uint8_t party_slot;
    uint16_t hp, max_hp;
} BattleFieldMon;
typedef struct {
    uint8_t phase, turn, outcome, actor, flags;
    uint16_t token;
    BattleFieldMon field[2][BATTLE_FIELD_SLOTS];
    uint8_t pp[BATTLE_FIELD_SLOTS][4], targets[BATTLE_FIELD_SLOTS][4];
} BattleMultiState;
#define BATTLE_MULTI_STATE_BYTES (7 + 2 * BATTLE_FIELD_SLOTS * 5 + 8 * BATTLE_FIELD_SLOTS)
typedef struct {
    uint16_t token;
    uint8_t actor, action, move_slot, target, rotation, flags;
} BattleMultiAction;
#define BATTLE_MULTI_ACTION_BYTES 8

unsigned battle_multi_state_encode(const BattleMultiState *state, uint8_t *out);
bool battle_multi_state_decode(const uint8_t *in, unsigned length, BattleMultiState *state);
unsigned battle_multi_action_encode(const BattleMultiAction *action, uint8_t *out);
bool battle_multi_action_decode(const uint8_t *in, unsigned length, BattleMultiAction *action);

// Roster app V2 extends the multi state with a reserve mask and the
// player's roster in request order. Empty field/roster entries use 0xff.
typedef struct {
    BattleMultiState field_state;
    uint8_t reserves, struggle_mask;
    BattleFieldMon player[BATTLE_MAX_PER_SIDE];
    uint8_t status[2][BATTLE_FIELD_SLOTS];
    int8_t stages[2][BATTLE_FIELD_SLOTS][BSTAT_COUNT];
    uint8_t volatiles[2][BATTLE_FIELD_SLOTS];  // APP V9 (BVF_*)
} BattleRosterState;
#define BATTLE_ROSTER_STATE_BYTES (BATTLE_MULTI_STATE_BYTES + 1 + 5 * BATTLE_MAX_PER_SIDE)

unsigned battle_roster_state_encode(const BattleRosterState *state, uint8_t *out);
bool battle_roster_state_decode(const uint8_t *in, unsigned length, BattleRosterState *state);
// Same layout as multi action. REPLACE uses target as a roster index.
bool battle_roster_action_decode(const uint8_t *in, unsigned length, BattleMultiAction *action);
// V3 permits SWITCH and supplies a reserve mask during CHOOSE.
bool battle_switch_action_decode(const uint8_t *in, unsigned length, BattleMultiAction *action);

// V4 appends the mask of field positions allowed to use Struggle.
#define BATTLE_STRUGGLE_STATE_BYTES (BATTLE_ROSTER_STATE_BYTES + 1)
unsigned battle_struggle_state_encode(const BattleRosterState *state, uint8_t *out);
bool battle_struggle_state_decode(const uint8_t *in, unsigned length, BattleRosterState *state);
bool battle_struggle_action_decode(const uint8_t *in, unsigned length, BattleMultiAction *action);

// V6 uses the V4/V5 state size, and adds a mid-turn pivot phase.
bool battle_pivot_state_decode(const uint8_t *in, unsigned length, BattleRosterState *state);
bool battle_pivot_action_decode(const uint8_t *in, unsigned length, BattleMultiAction *action);

#define BATTLE_CONDITION_STATE_BYTES (BATTLE_STRUGGLE_STATE_BYTES + 2 * BATTLE_FIELD_SLOTS * (1 + BSTAT_COUNT))
unsigned battle_condition_state_encode(const BattleRosterState *state, uint8_t *out);
bool battle_condition_state_decode(const uint8_t *in, unsigned length, BattleRosterState *state);

// APP V9 appends one volatile byte per field position (side, then slot).
// A LOCKED position has a forced action this turn: the host never prompts it.
#define BVF_CONFUSED          (1u << 0)
#define BVF_SUBSTITUTE        (1u << 1)
#define BVF_CHARGING          (1u << 2)
#define BVF_SEMI_INVULNERABLE (1u << 3)
#define BVF_RECHARGING        (1u << 4)
#define BVF_LOCKED            (1u << 5)
#define BVF_MASK 0x3fu
#define BATTLE_VOLATILE_STATE_BYTES (BATTLE_CONDITION_STATE_BYTES + 2 * BATTLE_FIELD_SLOTS)
unsigned battle_volatile_state_encode(const BattleRosterState *state, uint8_t *out);
bool battle_volatile_state_decode(const uint8_t *in, unsigned length, BattleRosterState *state);

// GcBattleResultV1 (RESULT payload, result version 1).
//   0 u8 outcome   1 u8 turns   2 u8 player count P   3 u8 opponent count O
//   4 u32 battle id
//   8 P x 12 bytes: u8 party slot, u8 flags (bit0 fainted), u16 hp, u32 status, u8 pp[4]
//   .. O x 2 bytes: u16 hp
enum BattleOutcome { OUTCOME_NONE = 0, OUTCOME_WIN = 1, OUTCOME_LOSE = 2, OUTCOME_DRAW = 3,
                     OUTCOME_CANCELLED = 4, OUTCOME_UNSUPPORTED = 5 };
#define BATTLE_RESULT_VERSION 1
#define BATTLE_RESULT_FAINTED (1u << 0)
typedef struct {
    uint8_t outcome, turns, player_count, opponent_count;
    uint32_t battle_id;
    struct { uint8_t slot, flags; uint16_t hp; uint32_t status; uint8_t pp[4]; } player[BATTLE_MAX_PER_SIDE];
    uint16_t opponent_hp[BATTLE_MAX_PER_SIDE];
} BattleResult;

unsigned battle_state_encode(const BattleState *state, uint8_t *out);
bool battle_state_decode(const uint8_t *in, unsigned length, BattleState *state);
unsigned battle_action_encode(const BattleAction *action, uint8_t *out);
bool battle_action_decode(const uint8_t *in, unsigned length, BattleAction *action);
unsigned battle_result_encode(const BattleResult *result, uint8_t *out);
bool battle_result_decode(const uint8_t *in, unsigned length, BattleResult *result);

#ifdef __cplusplus
}
#endif
#endif
