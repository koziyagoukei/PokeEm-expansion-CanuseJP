// Frontier content: Single and multi-format GC Battle tests (GC_MODE_BATTLE).
//
// Started from the debug menu. Builds a GcBattleRequestV1 from the player's
// lead Pokemon and the lead of a trainer's party, resolved from this game's
// own move/species/type data, and runs it through the generic GC Extension
// connector. The GameCube owns the temporary battle state; the GBA is the
// command input and second screen. The result is shown; with the "apply"
// variant, HP and PP of the lead are written back once, after the link is
// closed and the result was validated. Nothing else is changed.
#include "global.h"
#include "battle.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "battle_stat_change.h"
#include "config_changes.h"
#include "data.h"
#include "event_data.h"
#include "field_weather.h"
#include "item.h"
#include "main.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "random.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/battle_set_effect.h"
#include "constants/characters.h"
#include "constants/hold_effects.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "gc_extension.h"
#include "gc_ext/battle_protocol.h"
#include "frontier_gc_battle.h"

// The largest request this mode can build (six Pokemon in all, every block at
// its maximum), from the protocol's own formula, so the encoder never refuses one.
#define REQUEST_CAPACITY BATTLE_REQUEST_MAX_BYTES(PARTY_SIZE, NUMBER_OF_MON_TYPES)
STATIC_ASSERT(MAX_MON_MOVES == 4, GcRequestFourMoves);

enum GbaScreen { SCREEN_PREPARE, SCREEN_COMMAND, SCREEN_MOVES, SCREEN_TARGET, SCREEN_REPLACE, SCREEN_SWITCH, SCREEN_PIVOT, SCREEN_WAIT, SCREEN_ENDED };

struct FrontierGcBattle
{
    struct Pokemon opponent[PARTY_SIZE];
    u8 request[REQUEST_CAPACITY];
    u32 requestLength;
    u32 battleId;
    u32 leadPersonality;
    u16 trainerId;
    u8 environment;
    u8 leadSlot;
    u8 policy;
    u8 format, count, fieldCount;
    u8 roster; // zero for the legacy app, otherwise the roster app version
    bool8 endurance; // debug: V9 fixtures against weak foes for long battles
    u8 lab;          // debug: ability lab fixture (1..), 0 otherwise
    bool8 gen5Weather; // debug lab: the Gen 5 weather rules instead of this build's (V15)
    bool8 gen7Terrain; // debug lab: the Gen 7 terrain boost (x1.5) instead of this build's (V16)
    bool8 strictItems; // V17: refuse to start rather than normalise items (RULE_STRICT_ITEMS)
    bool8 gen3Items;   // debug lab: the Gen 3 type item boost (x1.1, Sea Incense x1.05) (V17)
    u8 itemState;      // V17 ITEMS_*: simulated, or why every item is off
    u16 itemReason;    // the item (ITEMS_OFF_ITEM) or ability (ITEMS_OFF_ABILITY) that normalised them
    u8 gimmickState;   // V18 FGC_GIMMICK_ITEMS_*: the audited Z-Crystals and Mega Stones (AcceptGimmickItems)
#if TESTING
    struct Pokemon *testMons;  // FrontierGcBattle_TestExport: the battle's Pokemon
    bool8 testStrictAll, testAllowGimmicks;
#endif
    u8 rotation, target;
    BattleMultiState multiState;
    u8 status[2][BATTLE_FIELD_SLOTS];
    s8 stages[2][BATTLE_FIELD_SLOTS][BSTAT_COUNT];
    u8 volatiles[2][BATTLE_FIELD_SLOTS]; // APP V9 BVF_* flags
    u8 reserveMask, struggleMask;
    BattleMultiAction multiAction;
    u8 screen;
    u8 cursor;
    u8 supportedMask;
    BattleState state;
    bool8 haveState;
    BattleAction pending;
    bool8 resultAccepted;
    bool8 applied;
    BattleResult result;
    u8 message[48];
};

static EWRAM_DATA struct FrontierGcBattle *sBattle = NULL;
static EWRAM_DATA u8 sPendingPolicy = 0;
static EWRAM_DATA u16 sPendingTrainer = 0;
static EWRAM_DATA u8 sPendingFormat = 0;
static EWRAM_DATA u8 sPendingRoster = 0;
static EWRAM_DATA bool8 sPendingEndurance = FALSE;
static EWRAM_DATA u8 sPendingLab = 0;
static EWRAM_DATA bool8 sPendingStrictItems = FALSE;
static EWRAM_DATA u8 sItemRefusal[64] = {0};  // a strict item refusal (outlives the battle data)
enum
{
    ITEMS_NOT_SENT = FGC_ITEMS_NOT_SENT,
    ITEMS_SIMULATED = FGC_ITEMS_SIMULATED,
    ITEMS_OFF_ITEM = FGC_ITEMS_OFF_ITEM,
    ITEMS_OFF_ABILITY = FGC_ITEMS_OFF_ABILITY,
};

static const u8 sText_StruggleRules[] = _("{ENG}Unsupported Struggle rules.");
static const u8 sText_NoMemory[] = _("{ENG}Not enough memory.");
static const u8 sText_RequestTooLarge[] = _("{ENG}GC battle request too large.");
static const u8 sText_NoLead[] = _("{ENG}No able Pokemon with a supported move.");
static const u8 sText_NoOpponent[] = _("{ENG}The trainer has no Pokemon.");
static const u8 sText_NoMoves[] = _("{ENG}Lead has no supported move.");
static const u8 sText_NoOpponentMoves[] = _("{ENG}Opponent has no supported move.");
static const u8 sText_Prepare[] = _("{ENG}GC Battle: waiting for the GameCube");
static const u8 sText_Fight[] = _("たたかう");
static const u8 sText_Cursor[] = _("{ENG}>");
static const u8 sText_Unsupported[] = _("{ENG}(unsupported)");
static const u8 sText_Wait[] = _("{ENG}GameCube is resolving the turn...");
static const u8 sText_Win[] = _("{ENG}WIN!");
static const u8 sText_Draw[] = _("{ENG}Draw.");
static const u8 sText_Lose[] = _("{ENG}LOSE...");
static const u8 sText_Cancelled[] = _("{ENG}Battle cancelled.");
static const u8 sText_Refused[] = _("{ENG}Battle not supported.");
static const u8 sText_HelpCommand[] = _("{ENG}A: Fight   B: Forfeit");
static const u8 sText_HelpMoves[] = _("{ENG}A: Use move   B: Back");
static const u8 sText_HelpEnded[] = _("{ENG}A: Return");
static const u8 sText_Vs[] = _("{ENG}VS ");
static const u8 sText_Lv[] = _("{ENG} Lv.");
static const u8 sText_Hp[] = _("{ENG}HP ");
static const u8 sText_Pp[] = _("{ENG}PP ");
static const u8 sText_Slash[] = _("{ENG}/");
static const u8 sText_Jpn[] = _("{JPN}");
static const u8 sText_Eng[] = _("{ENG}");
static const u8 sText_NotApplied[] = _("{ENG}Result shown only; party unchanged.");
static const u8 sText_NoResult[] = _("{ENG}No battle result; party unchanged.");
static const u8 sText_AppliedHp[] = _("{ENG}Applied to lead: HP ");
static const u8 sText_ApplyRefused[] = _("{ENG}Result not applied (lead changed).");

// ------------------------------------------------------------- request

static u8 TypeQuarters(enum Type attack, enum Type defend)
{
    return (gTypeEffectivenessTable[attack][defend] + 512) >> 10;
}

static u8 ResolveConditionEffect(enum Move move, u16 effect)
{
    // Export only single-stat primary effects. Multi-stat and conditional
    // effects remain unavailable until the GC supports their full behavior.
    if (effect == EFFECT_STAT_CHANGE && GetMoveAdditionalEffectCount(move) == 1)
    {
        static const enum Stat stats[] = {STAT_ATK, STAT_DEF, STAT_SPATK, STAT_SPDEF, STAT_SPEED, STAT_ACC, STAT_EVASION};
        const struct AdditionalEffect *change = gMovesInfo[move].additionalEffects;
        const bool32 down = change->moveEffect == STAT_CHANGE_EFFECT_MINUS;
        if (!down && change->moveEffect != STAT_CHANGE_EFFECT_PLUS)
            return BME_HIT;
        if (change->chance || change->onlyIfTargetRaisedStats || change->onChargeTurnOnly
         || change->preAttackEffect || change->onSide || change->pledgeCombo
         || (change->self && GetMoveTarget(move) != TARGET_USER))
            return BME_HIT;
        u8 resolved = BME_HIT;
        for (u32 stat = 0; stat < ARRAY_COUNT(stats); stat++)
        {
            s32 amount = GetStatStage(stats[stat], change);
            if (!amount)
                continue;
            if (resolved || amount > 6)
                return BME_HIT;
            resolved = BATTLE_STAGE_EFFECT(stat, down ? -amount : amount);
        }
        return resolved;
    }
    if (effect == EFFECT_REFRESH)
        return BME_REFRESH;
    if (effect != EFFECT_NON_VOLATILE_STATUS || gMovesInfo[move].powderMove)
        return BME_HIT;
    switch (GetMoveNonVolatileStatus(move))
    {
    case MOVE_EFFECT_POISON: return BATTLE_STATUS_EFFECT(BST_POISON);
    case MOVE_EFFECT_TOXIC: return BATTLE_STATUS_EFFECT(BST_TOXIC);
    case MOVE_EFFECT_BURN: return BATTLE_STATUS_EFFECT(BST_BURN);
    case MOVE_EFFECT_PARALYSIS:
        return move == MOVE_THUNDER_WAVE ? BME_ELECTRIC_PARALYSIS : BATTLE_STATUS_EFFECT(BST_PARALYSIS);
    case MOVE_EFFECT_SLEEP: return BATTLE_STATUS_EFFECT(BST_SLEEP);
    default: return BME_HIT;
    }
}

// ------------------------------------------------------------- V8 effects
//
// Resolves a move into the GC's effect descriptors from this build's move
// data (effect, argument, hit count, flags and additional effects) and
// battle config. Anything that cannot be expressed exactly is reported as
// unsupported, with the game's effect id kept for the host's debug log.

// Set when a list would need more than BATTLE_MAX_EFFECTS descriptors; the
// move is then refused instead of being sent with a truncated list.
static EWRAM_DATA bool8 sEffectOverflow = FALSE;

static bool8 PushEffect(BattleMove *out, BattleEffect e)
{
    if (out->effect_count >= BATTLE_MAX_EFFECTS)
    {
        sEffectOverflow = TRUE;
        return FALSE;
    }
    out->effects[out->effect_count++] = e;
    return TRUE;
}

// V10: when an additional effect of a hit runs, as the battle engine does it
// (Cmd_setadditionaleffects / CanApplyAdditionalEffect): a self effect once
// after the move (after the last target of a spread move), other effects
// on every hit of a multi-strike move, otherwise once per target.
static u8 EffectTiming(enum Move move, bool8 self, bool8 damaging)
{
    if (sBattle->roster < BATTLE_APP_VERSION_TIMING || !damaging)
        return 0;
    if (self)
        return BEFL_TIMING(BETIME_AFTER_MOVE);
    if (gMovesInfo[move].multiHit || GetMoveStrikeCount(move) > 1)
        return BEFL_TIMING(BETIME_PER_HIT);
    return BEFL_TIMING(BETIME_PER_TARGET);
}

static bool8 VolatilesEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_VOLATILES;
}

// Modifier records (requests V13+, gba-gc-extension docs/modifiers.md): one
// record type for moves and abilities, a stage, a percent and up to two
// predicates that must all hold. The GC runs them without code of its own
// for any move or ability.
#define MODIFIER(stage, percent, k0, a0, k1, a1) \
    ((BattleModifier){stage, percent, {{k0, a0}, {k1, a1}}})

static BattleEffect ModifierEffect(BattleModifier mod)
{
    BattleEffect e = {.kind = BEFF_MODIFIER};
    e.mod = mod;
    return e;
}

static BattleAbilityHook ModifierHook(BattleModifier mod)
{
    BattleAbilityHook h = {.kind = BABIL_MODIFIER};
    h.mod = mod;
    return h;
}

static bool8 PushEffect(BattleMove *out, BattleEffect e);
static u8 ExportStatusMask(u32 status);

// ---------------------------------------------------------------------------
// Game data -> modifier records (every battle). Each entry reproduces one
// modifier of this build's damage code (battle_util.c) exactly: its stage,
// its condition and its factor. Requests before V14 keep the V13 export
// (Hex only), so V13 requests are unchanged; the Ability Lab's own
// definitions are separate (sLabAbilities, debug fixtures only).

static const struct AbilityModifier
{
    u16 ability;
    BattleModifier mod;
} sAbilityModifiers[] = {
    // CalcMoveBasePowerAfterModifiers: power x1.5 while poisoned (physical) or burned (special).
    {ABILITY_TOXIC_BOOST, MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_POWER, BCAT_PHYSICAL), 150,
                                   BPRED_STATUS, (1u << BST_POISON) | (1u << BST_TOXIC), 0, 0)},
    {ABILITY_FLARE_BOOST, MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_POWER, BCAT_SPECIAL), 150,
                                   BPRED_STATUS, 1u << BST_BURN, 0, 0)},
    // CalcAttackStat: the attacking stat x0.5 at half HP or less (both categories).
    {ABILITY_DEFEATIST, MODIFIER(BMOD_ATTACK, 50, BPRED_HP_AT_MOST, 2, 0, 0)},
};

// The move's own modifier record, or FALSE if the effect cannot be one.
static bool8 ExportMoveModifier(enum Move move, enum BattleMoveEffects effect, BattleMove *out)
{
    switch (effect)
    {
    case EFFECT_DOUBLE_POWER_ON_ARG_STATUS:
    {
        // CalcMoveBasePower: Hex, Venoshock ... base power x2 while the target
        // has one of the move's statuses. The moves that also cure the status
        // (their substitute exception) stay unsupported.
        const u8 mask = ExportStatusMask(GetMoveEffectArg_Status(move));
        if (sBattle->roster < BATTLE_APP_VERSION_MODIFIERS || mask == 0
         || (GetMoveAdditionalEffectCount(move) && GetMoveAdditionalEffectById(move, 0)->moveEffect == MOVE_EFFECT_REMOVE_STATUS))
            return FALSE;
        return PushEffect(out, ModifierEffect(MODIFIER(BMOD_BASE_POWER, 200, BPRED_STATUS | BPRED_DEFENDER, mask, 0, 0)));
    }
    case EFFECT_BRINE:
        // CalcMoveBasePowerAfterModifiers: power x2 while the target is at half HP or less.
        if (sBattle->roster < BATTLE_APP_VERSION_FIXED_POINT)
            return FALSE;
        return PushEffect(out, ModifierEffect(MODIFIER(BMOD_POWER, 200, BPRED_HP_AT_MOST | BPRED_DEFENDER, 2, 0, 0)));
    default:
        return FALSE;
    }
}

static void ExportAbilityModifiers(enum Ability ability, BattleAbility *a);

// ---------------------------------------------------------------------------
// Request V15 weather (gba-gc-extension docs/weather.md). The GC keeps one
// weather state and runs the shared operations (set, suppression, countdown,
// residual, the weather step of the damage); everything this build decides
// becomes data here, read from its battle code and configs:
//   - the field rules: residual damage and its immune types, no freezing in
//     sun, the weather damage step and the weather stat boosts;
//   - the weather moves (EFFECT_WEATHER), the weather rules of other moves
//     (accuracy, charge, type, power, healing);
//   - the weather abilities (setters, suppression, speed, evasion, power,
//     residual). Everything else about weather stays unsupported.
// The Ability Lab can ask for the Gen 5 rules instead (gen5Weather) to show
// that the same GC follows the GBA's settings.

static bool8 WeatherEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_WEATHER;
}

// This build's weather kinds the GC models; primal weather, strong winds,
// fog and darkness have no GC kind (their moves and abilities stay unsupported).
static u8 ExportWeather(enum BattleWeather weather)
{
    switch (weather)
    {
    case BATTLE_WEATHER_SUN:          return BWEATHER_SUN;
    case BATTLE_WEATHER_RAIN:
    case BATTLE_WEATHER_RAIN_DOWNPOUR: return BWEATHER_RAIN;
    case BATTLE_WEATHER_SANDSTORM:    return BWEATHER_SANDSTORM;
    case BATTLE_WEATHER_HAIL:         return BWEATHER_HAIL;
    case BATTLE_WEATHER_SNOW:         return BWEATHER_SNOW;
    default:                          return BWEATHER_NONE;
    }
}

// TryChangeBattleWeather: 5 turns; ability weather before Gen 6 keeps the
// counter it finds (none: no end). A rock's 8 turns are the holder's item
// record (V17, ResolveItem); the GC applies it only to a limited count.
static u8 AbilityWeatherTurns(void)
{
    return sBattle->gen5Weather || GetConfig(B_ABILITY_WEATHER) < GEN_6 ? BWEATHER_TURNS_KEEP : 5;
}

static u8 SnowWarningWeather(void)
{
    return sBattle->gen5Weather || GetConfig(B_SNOW_WARNING) < GEN_9 ? BWEATHER_HAIL : BWEATHER_SNOW;
}

#define W_SUN BWEATHER_BIT(BWEATHER_SUN)
#define W_RAIN BWEATHER_BIT(BWEATHER_RAIN)
#define W_SAND BWEATHER_BIT(BWEATHER_SANDSTORM)
#define W_HAIL BWEATHER_BIT(BWEATHER_HAIL)
#define W_SNOW BWEATHER_BIT(BWEATHER_SNOW)

// TryFieldEffects FIELD_EFFECT_OVERWORLD_WEATHER: the map weathers this build
// brings into a battle (rain, sandstorm, drought, snow, fog).
static bool8 MapBringsWeather(void)
{
    u8 frontier;
    if (TryGetRandomFrontierLogicalWeather(&frontier))
        return TRUE;
    switch (GetCurrentWeather())
    {
    case WEATHER_RAIN:
    case WEATHER_RAIN_THUNDERSTORM:
    case WEATHER_DOWNPOUR:
    case WEATHER_SANDSTORM:
    case WEATHER_DROUGHT:
    case WEATHER_SNOW:
    case WEATHER_FOG_HORIZONTAL:
    case WEATHER_FOG_DIAGONAL:
        return TRUE;
    default:
        return FALSE;
    }
}

static void ResolveFieldRules(BattleFieldRules *f)
{
    u32 k, n = 0;
    memset(f, 0, sizeof(*f));
    for (k = 0; k < BWEATHER_KINDS; k++)
        memset(f->weather[k].immune_types, BABIL_NO_TYPE, sizeof(f->weather[k].immune_types));
    // HandleEndTurnWeatherDamage: max HP / 16, not underground (underwater is not
    // modelled); the immune abilities are hooks, Safety Goggles an item (not simulated).
    f->weather[BWEATHER_SANDSTORM - 1] = (BattleWeatherKindRules){16, BWKF_UNDERGROUND_SAFE,
                                                                   {TYPE_ROCK, TYPE_GROUND, TYPE_STEEL}};
    f->weather[BWEATHER_HAIL - 1] = (BattleWeatherKindRules){16, BWKF_UNDERGROUND_SAFE,
                                                              {TYPE_ICE, BABIL_NO_TYPE, BABIL_NO_TYPE}};
    // CanSetNonVolatileStatus: no freeze or frostbite in sun.
    f->weather[BWEATHER_SUN - 1].flags = BWKF_NO_FREEZE;
    // GetWeatherDamageModifier (the weather step, one factor).
    f->modifiers[n++] = MODIFIER(BMOD_WEATHER, 150, BPRED_WEATHER, W_SUN, BPRED_MOVE_TYPE, TYPE_FIRE);
    f->modifiers[n++] = MODIFIER(BMOD_WEATHER, 50, BPRED_WEATHER, W_SUN, BPRED_MOVE_TYPE, TYPE_WATER);
    f->modifiers[n++] = MODIFIER(BMOD_WEATHER, 150, BPRED_WEATHER, W_RAIN, BPRED_MOVE_TYPE, TYPE_WATER);
    f->modifiers[n++] = MODIFIER(BMOD_WEATHER, 50, BPRED_WEATHER, W_RAIN, BPRED_MOVE_TYPE, TYPE_FIRE);
    // CalcDefenseStat, after the items: sandstorm Rock Sp. Def, snow Ice Defense.
    if (GetConfig(B_SANDSTORM_SPDEF_BOOST) >= GEN_4)
        f->modifiers[n++] = MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_DEFENSE, BCAT_SPECIAL), 150, BPRED_WEATHER, W_SAND,
                                     BPRED_HAS_TYPE | BPRED_DEFENDER, TYPE_ROCK);
    f->modifiers[n++] = MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_DEFENSE, BCAT_PHYSICAL), 150, BPRED_WEATHER, W_SNOW,
                                 BPRED_HAS_TYPE | BPRED_DEFENDER, TYPE_ICE);
    f->modifier_count = n;
}

// ---------------------------------------------------------------------------
// Request V16 terrain (gba-gc-extension docs/terrain.md). As for the weather,
// the GC owns the state and the shared operations (set, the grounded rule,
// the terrain predicates, status and priority prevention, the heal and the
// countdown); this build's terrain rules are data:
//   - the terrain rules: the airborne type, what each terrain prevents, the
//     Grassy heal, and the type power records (CalcMoveBasePowerAfterModifiers);
//   - the terrain moves (EFFECT_TERRAIN), Earthquake's Grassy halving;
//   - the surge abilities, Levitate's ungrounding, Surge Surfer, Grass Pelt.
// The Ability Lab can ask for the Gen 7 boost (gen7Terrain).

static bool8 TerrainEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_TERRAIN;
}

static u8 ExportTerrain(enum BattleTerrain terrain)
{
    switch (terrain)
    {
    case B_TERRAIN_ELECTRIC: return BTERRAIN_ELECTRIC;
    case B_TERRAIN_GRASSY:   return BTERRAIN_GRASSY;
    case B_TERRAIN_MISTY:    return BTERRAIN_MISTY;
    case B_TERRAIN_PSYCHIC:  return BTERRAIN_PSYCHIC;
    default:                 return BTERRAIN_NONE;
    }
}

#define T_ELEC BTERRAIN_BIT(BTERRAIN_ELECTRIC)
#define T_GRASS BTERRAIN_BIT(BTERRAIN_GRASSY)
#define T_MIST BTERRAIN_BIT(BTERRAIN_MISTY)
#define T_PSY BTERRAIN_BIT(BTERRAIN_PSYCHIC)

static void ResolveTerrainRules(BattleTerrainRules *t)
{
    // B_TERRAIN_TYPE_BOOST: x1.3 from Gen 8, x1.5 before.
    const u8 boost = sBattle->gen7Terrain || GetConfig(B_TERRAIN_TYPE_BOOST) < GEN_8 ? 150 : 130;
    u32 n = 0;
    memset(t, 0, sizeof(*t));
    t->airborne_type = TYPE_FLYING;  // IsBattlerGrounded (Levitate is a hook)
    t->terrain[BTERRAIN_ELECTRIC - 1].flags = BTKF_NO_SLEEP;                   // CanSetNonVolatileStatus
    t->terrain[BTERRAIN_GRASSY - 1].heal_divisor = 16;                         // FIRST_EVENT_BLOCK_GRASSY_TERRAIN_HEAL
    t->terrain[BTERRAIN_MISTY - 1].flags = BTKF_NO_STATUS | BTKF_NO_CONFUSION; // and CanBeConfused
    t->terrain[BTERRAIN_PSYCHIC - 1].flags = BTKF_BLOCK_PRIORITY;              // CanPsychicTerrainProtectTarget
    // CalcMoveBasePowerAfterModifiers, after the move's own modifiers, in this order.
    t->modifiers[n++] = MODIFIER(BMOD_POWER, boost, BPRED_ON_TERRAIN, T_GRASS, BPRED_MOVE_TYPE, TYPE_GRASS);
    t->modifiers[n++] = MODIFIER(BMOD_POWER, 50, BPRED_ON_TERRAIN | BPRED_DEFENDER, T_MIST, BPRED_MOVE_TYPE, TYPE_DRAGON);
    t->modifiers[n++] = MODIFIER(BMOD_POWER, boost, BPRED_ON_TERRAIN, T_ELEC, BPRED_MOVE_TYPE, TYPE_ELECTRIC);
    t->modifiers[n++] = MODIFIER(BMOD_POWER, boost, BPRED_ON_TERRAIN, T_PSY, BPRED_MOVE_TYPE, TYPE_PSYCHIC);
    t->modifier_count = n;
}

// The weather rules of a move other than its effect: accuracy (Thunder,
// Hurricane, Blizzard ...), read from this build's move flags.
static void ExportWeatherAccuracy(enum Move move, BattleMove *out)
{
    if (MoveAlwaysHitsInRain(move))
        PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_ACCURACY, .arg0 = W_RAIN, .arg2 = 0});
    if (MoveAlwaysHitsInHailSnow(move))
        PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_ACCURACY, .arg0 = W_HAIL | W_SNOW, .arg2 = 0});
    if (MoveHas50AccuracyInSun(move))
        PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_ACCURACY, .arg0 = W_SUN, .arg2 = 50});
}

// CalcMoveBasePowerAfterModifiers EFFECT_SOLAR_BEAM: x0.5 in rain, hail,
// snow and (B_SANDSTORM_SOLAR_BEAM) sandstorm; fog has no GC kind.
static bool8 ExportSolarBeamPower(BattleMove *out)
{
    const u8 mask = W_RAIN | W_HAIL | W_SNOW | (GetConfig(B_SANDSTORM_SOLAR_BEAM) >= GEN_3 ? W_SAND : 0);
    return PushEffect(out, ModifierEffect(MODIFIER(BMOD_POWER, 50, BPRED_WEATHER, mask, 0, 0)));
}

// Weather Ball (GetDynamicMoveType, CalcMoveBasePower): the weather's type and x2 power.
static bool8 ExportWeatherBall(BattleMove *out)
{
    return PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_TYPE, .arg0 = W_SUN, .arg2 = TYPE_FIRE})
        && PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_TYPE, .arg0 = W_RAIN, .arg2 = TYPE_WATER})
        && PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_TYPE, .arg0 = W_SAND, .arg2 = TYPE_ROCK})
        && PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_TYPE, .arg0 = W_HAIL | W_SNOW, .arg2 = TYPE_ICE})
        && PushEffect(out, ModifierEffect(MODIFIER(BMOD_BASE_POWER, 200, BPRED_WEATHER, BWEATHER_ALL, 0, 0)));
}

// Cmd_recoverbasedonsunlight: 20/30 of max HP in sun (sandstorm for Shore Up),
// 1/4 in any other weather (not Shore Up), else 1/2.
static bool8 ExportWeatherHeal(enum BattleMoveEffects effect, BattleMove *out)
{
    const u8 boosted = effect == EFFECT_SHORE_UP ? W_SAND : W_SUN;
    if (!PushEffect(out, (BattleEffect){.kind = BEFF_HEAL, .who = BWHO_USER, .arg0 = 1, .arg2 = 2})
     || !PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_HEAL, .who = BWHO_USER, .arg0 = boosted, .arg1 = 20, .arg2 = 30}))
        return FALSE;
    if (effect != EFFECT_SHORE_UP)
        return PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER_HEAL, .who = BWHO_USER,
                                              .arg0 = BWEATHER_ALL & ~W_SUN, .arg1 = 1, .arg2 = 4});
    return TRUE;
}


// STATUS1_* bits of a move argument as a mask of 1 << BST_*.
static u8 ExportStatusMask(u32 status)
{
    u8 mask = 0;
    if (status & STATUS1_SLEEP)        mask |= 1u << BST_SLEEP;
    if (status & STATUS1_POISON)       mask |= 1u << BST_POISON;
    if (status & STATUS1_BURN)         mask |= 1u << BST_BURN;
    if (status & STATUS1_FREEZE)       mask |= 1u << BST_FREEZE;
    if (status & STATUS1_PARALYSIS)    mask |= 1u << BST_PARALYSIS;
    if (status & STATUS1_TOXIC_POISON) mask |= 1u << BST_TOXIC;
    if (status & STATUS1_FROSTBITE)    mask |= 1u << BST_FROSTBITE;
    return mask;
}

static u8 ExportMoveStatus(enum MoveEffect effect)
{
    switch (effect)
    {
    case MOVE_EFFECT_POISON:    return BST_POISON;
    case MOVE_EFFECT_TOXIC:     return BST_TOXIC;
    case MOVE_EFFECT_BURN:      return BST_BURN;
    case MOVE_EFFECT_PARALYSIS: return BST_PARALYSIS;
    case MOVE_EFFECT_SLEEP:     return BST_SLEEP;
    case MOVE_EFFECT_FREEZE:    return BST_FREEZE;
    case MOVE_EFFECT_FROSTBITE: return BST_FROSTBITE;
    default:                    return BST_NONE;
    }
}

static u8 ResolveAdditionalEffects(enum Move move, BattleMove *out, bool8 damaging)
{
    static const enum Stat stats[] = {STAT_ATK, STAT_DEF, STAT_SPATK, STAT_SPDEF, STAT_SPEED, STAT_ACC, STAT_EVASION};
    const bool8 selfTargeted = GetMoveTarget(move) == TARGET_USER;
    u32 i, stat;

    for (i = 0; i < GetMoveAdditionalEffectCount(move); i++)
    {
        const struct AdditionalEffect *e = GetMoveAdditionalEffectById(move, i);
        const u8 who = e->self && !selfTargeted ? BWHO_USER : BWHO_TARGET;
        const u8 chance = damaging ? e->chance : 0;
        u8 status;
        if (e->onlyIfTargetRaisedStats || e->onChargeTurnOnly || e->preAttackEffect || e->onSide || e->pledgeCombo
         || (!damaging && e->chance))
            return UNSUP_SECONDARY;
        switch (e->moveEffect)
        {
        case STAT_CHANGE_EFFECT_PLUS:
        case STAT_CHANGE_EFFECT_MINUS:
        case MOVE_EFFECT_STAT_PLUS:
        case MOVE_EFFECT_STAT_MINUS:
        {
            const bool8 down = e->moveEffect == STAT_CHANGE_EFFECT_MINUS || e->moveEffect == MOVE_EFFECT_STAT_MINUS;
            bool8 any = FALSE;
            for (stat = 0; stat < ARRAY_COUNT(stats); stat++)
            {
                s32 amount = GetStatStage(stats[stat], e);
                if (!amount)
                    continue;
                if (amount > 6 || !PushEffect(out, (BattleEffect){.kind = BEFF_STAT, .who = who, .chance = chance,
                                                                   .arg0 = stat, .arg1 = down ? -amount : amount,
                                                                   .flags = EffectTiming(move, who == BWHO_USER, damaging)}))
                    return UNSUP_OTHER;
                any = TRUE;
            }
            if (!any)
                return UNSUP_SECONDARY;
            break;
        }
        case MOVE_EFFECT_FLINCH:
            if (!VolatilesEnabled() || !damaging || e->self
             || !PushEffect(out, (BattleEffect){.kind = BEFF_FLINCH, .who = BWHO_TARGET, .chance = chance,
                                                .flags = EffectTiming(move, FALSE, damaging)}))
                return UNSUP_SECONDARY;
            break;
        case MOVE_EFFECT_CONFUSION:
            if (!VolatilesEnabled() || e->self
             || !PushEffect(out, (BattleEffect){.kind = BEFF_CONFUSE, .who = BWHO_TARGET, .chance = chance,
                                                .flags = EffectTiming(move, FALSE, damaging)}))
                return UNSUP_SECONDARY;
            break;
        case MOVE_EFFECT_RECHARGE:
            // Hyper Beam style: the user must recharge after a hit.
            if (!VolatilesEnabled() || !damaging || !e->self || e->chance
             || !PushEffect(out, (BattleEffect){.kind = BEFF_RECHARGE, .who = BWHO_USER}))
                return UNSUP_SECONDARY;
            break;
        case MOVE_EFFECT_ABSORB:
            if (!damaging || e->self || !e->argument.absorbPercentage
             || !PushEffect(out, (BattleEffect){.kind = BEFF_DRAIN, .who = BWHO_USER,
                                                .arg0 = e->argument.absorbPercentage}))
                return UNSUP_SECONDARY;
            break;
        default:
            status = ExportMoveStatus(e->moveEffect);
            if (status == BST_NONE)
                return UNSUP_SECONDARY;
            if (!PushEffect(out, (BattleEffect){.kind = BEFF_STATUS, .who = who, .chance = chance, .arg0 = status,
                                                .flags = EffectTiming(move, who == BWHO_USER, damaging)}))
                return UNSUP_OTHER;
            break;
        }
    }
    return 0;
}

// M4-B1a+: move behaviour this build keys to move flags the request has no
// input for, so such a move is never sent as a plain hit. Shadow moves are
// 2x against any Pokemon that is not a Shadow Pokemon, without STAB or PP
// use (IsShadowMove); Sacred Sword, Chip Away and Darkest Lariat ignore the
// target's Defense and evasion stages; Thousand Arrows hits Flying types and
// Levitate; from V11, where abilities take part, Sunsteel Strike, Moongeist
// Beam ... ignore the target's ability (moldBreakerActive); Z-Moves and Max
// Moves are gimmicks (RULE_NO_GIMMICKS).
#if TESTING
// Regression hashes (gc_gimmick_item_reference.c): the M4-B1a export, with
// neither the move flags below nor the gimmick items.
static EWRAM_DATA bool8 sTestLegacyExport = FALSE;
#define LEGACY_EXPORT() sTestLegacyExport
#else
#define LEGACY_EXPORT() FALSE
#endif

static u8 UnmodelledMoveFlags(enum Move move)
{
    if (LEGACY_EXPORT())
        return 0;
    if (gMovesInfo[move].shadowMove || gMovesInfo[move].ignoresTargetDefenseEvasionStages
     || gMovesInfo[move].ignoreTypeIfFlyingAndUngrounded || (move >= FIRST_Z_MOVE && move < FIRST_SHADOW_MOVE)
     || (sBattle->roster >= BATTLE_APP_VERSION_ABILITIES && gMovesInfo[move].ignoresTargetAbility))
        return UNSUP_OTHER;
    return 0;
}

static u8 ResolveEffects(enum Move move, BattleMove *out)
{
    const enum BattleMoveEffects effect = GetMoveEffect(move);
    const bool8 damaging = out->category != BCAT_STATUS;
    u8 unsupported = 0;

    out->effect = BME_HIT;
    out->source_effect = effect;
    out->move_flags = gMovesInfo[move].ignoresProtect ? BMF_IGNORES_PROTECT : 0;
    sEffectOverflow = FALSE;
    if (VolatilesEnabled())
    {
        // Substitute and semi-invulnerable reach, from this build's move flags.
        // Moves that damage underground targets also double against them here.
        if (gMovesInfo[move].ignoresSubstitute)
            out->move_flags |= BMF_IGNORES_SUBSTITUTE;
        if (MoveDamagesAirborneDoubleDamage(move))
            out->move_flags |= BMF_DOUBLE_AIRBORNE;
        else if (MoveDamagesAirborne(move))
            out->move_flags |= BMF_HITS_AIRBORNE;
        if (MoveDamagesUnderground(move))
            out->move_flags |= BMF_DOUBLE_UNDERGROUND;
    }
    // V11: contact, for the GC's contact abilities (Static, Rough Skin ...).
    if (sBattle->roster >= BATTLE_APP_VERSION_ABILITIES && MoveMakesContact(move))
        out->move_flags |= BMF_CONTACT;
    if (damaging)
    {
        BattleEffect damage = {.kind = BEFF_DAMAGE, .arg0 = 1, .arg2 = 1};
        if (out->power == 0)
            return UNSUP_EFFECT;
        if (gMovesInfo[move].multiHit)
        {
            damage.arg0 = 2;
            damage.arg2 = 5;
            damage.arg1 = GetConfig(B_MULTI_HIT_CHANCE) >= GEN_5 ? BHITS_2TO5_GEN5 : BHITS_2TO5_GEN1;
        }
        else if (GetMoveStrikeCount(move) > 1)
        {
            if (GetMoveStrikeCount(move) > BATTLE_MAX_HITS)
                return UNSUP_MULTI_HIT;
            damage.arg0 = damage.arg2 = GetMoveStrikeCount(move);
        }
        PushEffect(out, damage);
    }
    switch (effect)
    {
    case EFFECT_HIT:
    case EFFECT_STAT_CHANGE:
        break;
    case EFFECT_DOUBLE_POWER_ON_ARG_STATUS:  // Hex (V13+)
    case EFFECT_BRINE:                       // V14+
        if (!damaging || !ExportMoveModifier(move, effect, out))
            return UNSUP_EFFECT;
        break;
    case EFFECT_EARTHQUAKE:
        // A plain hit that also reaches (and doubles against) an underground
        // target. Its Grassy Terrain halving (CalcMoveBasePowerAfterModifiers:
        // the terrain, and the target not semi-invulnerable) is sent from V16;
        // before V16 there is no terrain.
        if (!VolatilesEnabled())
            return UNSUP_EFFECT;
        if (TerrainEnabled()
         && !PushEffect(out, ModifierEffect(MODIFIER(BMOD_POWER, 50, BPRED_TERRAIN, T_GRASS,
                                                     BPRED_EXPOSED | BPRED_DEFENDER, 0))))
            return UNSUP_OTHER;
        break;
    case EFFECT_TERRAIN:  // V16: Electric / Grassy / Misty / Psychic Terrain
    {
        const u8 kind = TerrainEnabled() ? ExportTerrain(GetMoveTerrainType(move)) : BTERRAIN_NONE;
        if (damaging || kind == BTERRAIN_NONE)
            return UNSUP_EFFECT;
        out->target = BTARGET_SELF;  // a field move (TARGET_FIELD)
        PushEffect(out, (BattleEffect){.kind = BEFF_TERRAIN, .who = BWHO_USER, .arg0 = kind, .arg2 = 5});
        break;
    }
    case EFFECT_RECOIL:
        PushEffect(out, (BattleEffect){.kind = BEFF_RECOIL, .who = BWHO_USER, .arg0 = GetMoveRecoil(move)});
        break;
    case EFFECT_HIT_ESCAPE:
        PushEffect(out, (BattleEffect){.kind = BEFF_PIVOT});
        break;
    case EFFECT_HIT_SWITCH_TARGET:
    case EFFECT_ROAR:
        PushEffect(out, (BattleEffect){.kind = BEFF_FORCE_OUT});
        break;
    case EFFECT_RESTORE_HP:  // the move script heals max HP / 2
        PushEffect(out, (BattleEffect){.kind = BEFF_HEAL, .who = BWHO_USER, .arg0 = 1, .arg2 = 2});
        break;
    // V15 weather (the rules above the request builder).
    case EFFECT_WEATHER:
    {
        const u8 kind = WeatherEnabled() ? ExportWeather(GetMoveWeatherType(move)) : BWEATHER_NONE;
        if (damaging || kind == BWEATHER_NONE)
            return UNSUP_EFFECT;
        out->target = BTARGET_SELF;  // a field move (TARGET_FIELD): no Pokemon is its target
        PushEffect(out, (BattleEffect){.kind = BEFF_WEATHER, .who = BWHO_USER, .arg0 = kind, .arg2 = 5});
        break;
    }
    case EFFECT_WEATHER_BALL:
        if (!WeatherEnabled() || !damaging || !ExportWeatherBall(out))
            return UNSUP_EFFECT;
        break;
    case EFFECT_MOONLIGHT:
    case EFFECT_MORNING_SUN:
    case EFFECT_SYNTHESIS:
    case EFFECT_SHORE_UP:
        if (!WeatherEnabled() || damaging || (effect != EFFECT_SHORE_UP && GetConfig(B_TIME_OF_DAY_HEALING_MOVES) == GEN_2)
         || !ExportWeatherHeal(effect, out))
            return UNSUP_EFFECT;
        break;
    case EFFECT_PROTECT:
        if (GetMoveProtectMethod(move) != PROTECT_NORMAL)
            return UNSUP_EFFECT;
        PushEffect(out, (BattleEffect){.kind = BEFF_PROTECT, .who = BWHO_USER,
                                       .arg0 = B_PROTECT_FAILURE_RATE >= GEN_5 ? 3 : 2});
        break;
    case EFFECT_NON_VOLATILE_STATUS:
    {
        const u8 status = ExportMoveStatus(GetMoveNonVolatileStatus(move));
        // Status moves ignore the type chart except Thunder Wave (as in the game's type calc).
        if (status == BST_NONE || gMovesInfo[move].powderMove)
            return UNSUP_EFFECT;
        PushEffect(out, (BattleEffect){.kind = BEFF_STATUS, .arg0 = status,
                                       .flags = move == MOVE_THUNDER_WAVE ? BEFL_TYPE_CHART : 0});
        break;
    }
    case EFFECT_REFRESH:
        PushEffect(out, (BattleEffect){.kind = BEFF_CURE,
            .arg0 = 1u << BST_POISON | 1u << BST_TOXIC | 1u << BST_BURN | 1u << BST_PARALYSIS});
        break;
    case EFFECT_CONFUSE:
        if (!VolatilesEnabled())
            return UNSUP_EFFECT;
        PushEffect(out, (BattleEffect){.kind = BEFF_CONFUSE, .who = BWHO_TARGET});
        break;
    case EFFECT_SWAGGER:  // the +2 Attack comes from the additional effects; confusion follows it
        if (!VolatilesEnabled())
            return UNSUP_EFFECT;
        break;
    case EFFECT_SUBSTITUTE:
        if (!VolatilesEnabled())
            return UNSUP_EFFECT;
        PushEffect(out, (BattleEffect){.kind = BEFF_SUBSTITUTE, .who = BWHO_USER});
        break;
    case EFFECT_SOLAR_BEAM:
    case EFFECT_TWO_TURNS_ATTACK:
    case EFFECT_SEMI_INVULNERABLE:
    {
        // Charge on the first turn, hit on the second. Before V15 GC battles
        // have no weather, so a weather shortcut (Solar Beam in sun) never
        // applies; V15 sends it (and Solar Beam's power in other weathers).
        u8 semi = BSEMI_NONE, skip = 0;
        if (!VolatilesEnabled() || !damaging)
            return UNSUP_EFFECT;
        if (effect == EFFECT_SEMI_INVULNERABLE)
        {
            switch (gMovesInfo[move].argument.twoTurnAttack.state)
            {
            case STATE_ON_AIR: semi = BSEMI_AIR; break;
            case STATE_UNDERGROUND: semi = BSEMI_UNDERGROUND; break;
            default: return UNSUP_EFFECT;  // underwater, Phantom Force, Sky Drop
            }
        }
        if (WeatherEnabled() && effect != EFFECT_SEMI_INVULNERABLE && GetTwoTurnMoveWeather(move))
        {
            skip = ExportWeather(GetTwoTurnMoveWeather(move));
            if (skip == BWEATHER_NONE)
                return UNSUP_EFFECT;
            skip = BWEATHER_BIT(skip);
        }
        // CHARGE leads the list, before DAMAGE.
        out->effects[1] = out->effects[0];
        out->effects[0] = (BattleEffect){.kind = BEFF_CHARGE, .who = BWHO_USER, .arg0 = semi, .arg2 = skip};
        out->effect_count = 2;
        if (WeatherEnabled() && effect == EFFECT_SOLAR_BEAM && !ExportSolarBeamPower(out))
            return UNSUP_OTHER;
        break;
    }
    default:
        return UNSUP_EFFECT;
    }
    unsupported = ResolveAdditionalEffects(move, out, damaging);
    if (unsupported)
        return unsupported;
    if (WeatherEnabled())
    {
        // Powder moves: Grass types, Overcoat and Safety Goggles block them (not modelled).
        if (gMovesInfo[move].powderMove)
            return UNSUP_EFFECT;
        ExportWeatherAccuracy(move, out);
    }
    if (effect == EFFECT_SWAGGER)
        PushEffect(out, (BattleEffect){.kind = BEFF_CONFUSE, .who = BWHO_TARGET});
    if (sEffectOverflow)
        return UNSUP_OTHER;
    if (effect == EFFECT_STAT_CHANGE && out->effect_count == 0)
        return UNSUP_EFFECT;
    if (IsExplosionMove(move) || gMovesInfo[move].alwaysCriticalHit || gMovesInfo[move].cantUseTwice
     || gMovesInfo[move].noAffectOnSameTypeTarget || gMovesInfo[move].accDecreaseIfUserNotSameType
     || MoveThawsUser(move) || out->accuracy > 100 || out->type >= NUMBER_OF_MON_TYPES
     || UnmodelledMoveFlags(move))
        return UNSUP_OTHER;
    if ((out->target & BTARGET_CLASS_MASK) == BTARGET_OTHER)
        return UNSUP_TARGET;
    return battle_move_effects_valid(out) ? 0 : UNSUP_EFFECT;
}

static u32 ExportCondition(u32 status)
{
    if (status & STATUS1_SLEEP) return BATTLE_CONDITION(BST_SLEEP, status & STATUS1_SLEEP);
    if (status & STATUS1_TOXIC_POISON) return BATTLE_CONDITION(BST_TOXIC, (status & STATUS1_TOXIC_COUNTER) >> 8);
    if (status & STATUS1_POISON) return BST_POISON;
    if (status & STATUS1_BURN) return BST_BURN;
    if (status & STATUS1_PARALYSIS) return BST_PARALYSIS;
    if (status & STATUS1_FREEZE) return BST_FREEZE;
    if (status & STATUS1_FROSTBITE) return BST_FROSTBITE;
    return BST_NONE;
}

static void ResolveMove(enum Move move, BattleMove *out)
{
    memset(out, 0, sizeof(*out));
    if (move == MOVE_NONE || move >= MOVES_COUNT_ALL)
    {
        out->unsupported = UNSUP_EMPTY;
        return;
    }
    out->power = GetMovePower(move);
    out->type = GetMoveType(move);
    switch (GetMoveCategory(move))
    {
    case DAMAGE_CATEGORY_PHYSICAL: out->category = BCAT_PHYSICAL; break;
    case DAMAGE_CATEGORY_SPECIAL:  out->category = BCAT_SPECIAL; break;
    default:                       out->category = BCAT_STATUS; break;
    }
    out->accuracy = GetMoveAccuracy(move);
    out->priority = GetMovePriority(move);
    switch (GetMoveTarget(move))
    {
    case TARGET_SELECTED:      out->target = BTARGET_SINGLE; break;
    case TARGET_BOTH:          out->target = BTARGET_ALL_FOES; break;
    case TARGET_FOES_AND_ALLY:
        out->target = sBattle->format == BATTLE_FORMAT_SINGLE ? BTARGET_ALL_FOES : BTARGET_ALL_ADJACENT;
        break;
    case TARGET_USER:          out->target = BTARGET_SELF; break;
    default:                   out->target = BTARGET_OTHER; break;
    }
    if (sBattle->roster >= BATTLE_APP_VERSION_EFFECTS)
    {
        if (sBattle->format == BATTLE_FORMAT_TRIPLE && move == MOVE_WING_ATTACK)
            out->target |= BTARGET_IGNORE_DISTANCE;
        out->unsupported = ResolveEffects(move, out);
        return;
    }
    const u16 effect = GetMoveEffect(move);
    if (sBattle->roster >= BATTLE_APP_VERSION_DRAG)
    {
        if (effect == EFFECT_ROAR)
            out->effect = BME_FORCE_OUT;
        else if (effect == EFFECT_HIT_SWITCH_TARGET)
            out->effect = BME_FORCE_OUT_ON_HIT;
        else if (sBattle->roster >= BATTLE_APP_VERSION_PIVOT && effect == EFFECT_HIT_ESCAPE)
            out->effect = BME_PIVOT;
    }
    if (sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS && out->effect == BME_HIT && out->category == BCAT_STATUS)
        out->effect = ResolveConditionEffect(move, effect);
    const bool8 conditionMove = out->effect >= BME_STAGE_BASE;
    // Move flags are resolved here so the host can remain game-independent.
    if (sBattle->format == BATTLE_FORMAT_TRIPLE && move == MOVE_WING_ATTACK)
        out->target |= BTARGET_IGNORE_DISTANCE;
    if (effect != EFFECT_HIT && out->effect == BME_HIT)
        out->unsupported |= UNSUP_EFFECT;
    if (GetMoveAdditionalEffectCount(move) != 0 && !(conditionMove && effect == EFFECT_STAT_CHANGE))
        out->unsupported |= UNSUP_SECONDARY;
    if (out->priority != 0 && out->effect == BME_HIT)
        out->unsupported |= UNSUP_PRIORITY;
    if ((out->target & BTARGET_CLASS_MASK) == BTARGET_OTHER ||
        (sBattle->format == BATTLE_FORMAT_SINGLE && out->target == BTARGET_SELF && !conditionMove))
        out->unsupported |= UNSUP_TARGET;
    if ((out->category == BCAT_STATUS || out->power == 0) && out->effect != BME_FORCE_OUT && !conditionMove)
        out->unsupported |= UNSUP_STATUS_MOVE;
    if (GetMoveStrikeCount(move) > 1 || gMovesInfo[move].multiHit)
        out->unsupported |= UNSUP_MULTI_HIT;
    if (IsExplosionMove(move) || gMovesInfo[move].alwaysCriticalHit || gMovesInfo[move].cantUseTwice
     || gMovesInfo[move].noAffectOnSameTypeTarget || gMovesInfo[move].accDecreaseIfUserNotSameType
     || (sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS && MoveThawsUser(move))
     || out->accuracy > 100 || out->type >= NUMBER_OF_MON_TYPES || UnmodelledMoveFlags(move))
        out->unsupported |= UNSUP_OTHER;
}

// Requests V11/V12: the Pokemon's ability as generic hooks, read from this
// build's ability code and configs (gba-gc-extension docs/abilities.md).
// Any ability with a battle effect the GC core does not run is sent as
// UNSUPPORTED (the GC shows it as not simulated), never as NONE. V11 carries
// one hook with a percent chance; abilities that need more are UNSUPPORTED there.
#define HOOK(kind, arg0, arg1, arg2, num, den, percent, flags) \
    ((BattleAbilityHook){kind, {{arg0, arg1, arg2, num, den, percent}}, flags})

static bool8 AddHook(BattleAbility *a, BattleAbilityHook hook)
{
    if (a->count >= BATTLE_MAX_ABILITY_HOOKS)
        return FALSE;
    a->hooks[a->count++] = hook;
    a->cls = BACLASS_HOOKS;
    return TRUE;
}

// The weather abilities (AbilityBattleEffects, GetBattlerTotalSpeedStat,
// GetTotalAccuracy, CalcMoveBasePowerAfterModifiers, CalcAttackStat,
// HandleEndTurnWeatherDamage). FALSE: not a weather ability this request has.
static bool8 ResolveWeatherAbility(enum Ability ability, BattleAbility *a)
{
    if (!WeatherEnabled())
        return FALSE;
    if (TerrainEnabled())
    {
        switch (ability)  // V16 terrain abilities (AbilityBattleEffects, GetBattlerTotalSpeedStat, CalcDefenseStat)
        {
        case ABILITY_ELECTRIC_SURGE: AddHook(a, HOOK(BABIL_ENTRY_TERRAIN, BTERRAIN_ELECTRIC, 0, 5, 0, 0, 0, 0)); return TRUE;
        case ABILITY_GRASSY_SURGE:   AddHook(a, HOOK(BABIL_ENTRY_TERRAIN, BTERRAIN_GRASSY, 0, 5, 0, 0, 0, 0)); return TRUE;
        case ABILITY_MISTY_SURGE:    AddHook(a, HOOK(BABIL_ENTRY_TERRAIN, BTERRAIN_MISTY, 0, 5, 0, 0, 0, 0)); return TRUE;
        case ABILITY_PSYCHIC_SURGE:  AddHook(a, HOOK(BABIL_ENTRY_TERRAIN, BTERRAIN_PSYCHIC, 0, 5, 0, 0, 0, 0)); return TRUE;
        case ABILITY_SURGE_SURFER:   // speed x2 on Electric Terrain (the terrain only, grounded or not)
            AddHook(a, ModifierHook(MODIFIER(BMOD_SPEED, 200, BPRED_TERRAIN, T_ELEC, 0, 0)));
            return TRUE;
        case ABILITY_GRASS_PELT:     // the holder's Defense x1.5 on Grassy Terrain
            AddHook(a, ModifierHook(MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_DEFENSE, BCAT_PHYSICAL) | BMODF_DEFENDER, 150,
                                             BPRED_TERRAIN, T_GRASS, 0, 0)));
            return TRUE;
        default:
            break;
        }
    }
    switch (ability)
    {
    case ABILITY_DRIZZLE:     AddHook(a, HOOK(BABIL_ENTRY_WEATHER, BWEATHER_RAIN, 0, AbilityWeatherTurns(), 0, 0, 0, 0)); break;
    case ABILITY_DROUGHT:     AddHook(a, HOOK(BABIL_ENTRY_WEATHER, BWEATHER_SUN, 0, AbilityWeatherTurns(), 0, 0, 0, 0)); break;
    case ABILITY_SAND_STREAM: AddHook(a, HOOK(BABIL_ENTRY_WEATHER, BWEATHER_SANDSTORM, 0, AbilityWeatherTurns(), 0, 0, 0, 0)); break;
    case ABILITY_SNOW_WARNING:
        AddHook(a, HOOK(BABIL_ENTRY_WEATHER, SnowWarningWeather(), 0, AbilityWeatherTurns(), 0, 0, 0, 0));
        break;
    case ABILITY_CLOUD_NINE:
    case ABILITY_AIR_LOCK:
        AddHook(a, HOOK(BABIL_WEATHER_SUPPRESS, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_SWIFT_SWIM:  AddHook(a, ModifierHook(MODIFIER(BMOD_SPEED, 200, BPRED_WEATHER, W_RAIN, 0, 0))); break;
    case ABILITY_CHLOROPHYLL: AddHook(a, ModifierHook(MODIFIER(BMOD_SPEED, 200, BPRED_WEATHER, W_SUN, 0, 0))); break;
    case ABILITY_SLUSH_RUSH:  AddHook(a, ModifierHook(MODIFIER(BMOD_SPEED, 200, BPRED_WEATHER, W_HAIL | W_SNOW, 0, 0))); break;
    case ABILITY_SAND_RUSH:
        AddHook(a, HOOK(BABIL_WEATHER_IMMUNE, W_SAND, 0, 0, 0, 0, 0, 0));
        AddHook(a, ModifierHook(MODIFIER(BMOD_SPEED, 200, BPRED_WEATHER, W_SAND, 0, 0)));
        break;
    case ABILITY_SAND_VEIL:   // accuracy x80/100 against the holder (target's ability step)
        AddHook(a, HOOK(BABIL_WEATHER_IMMUNE, W_SAND, 0, 0, 0, 0, 0, 0));
        AddHook(a, ModifierHook(MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_ACCURACY, 0) | BMODF_DEFENDER, 80,
                                         BPRED_WEATHER, W_SAND, 0, 0)));
        break;
    case ABILITY_SNOW_CLOAK:
        AddHook(a, HOOK(BABIL_WEATHER_IMMUNE, W_HAIL, 0, 0, 0, 0, 0, 0));
        AddHook(a, ModifierHook(MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_ACCURACY, 0) | BMODF_DEFENDER, 80,
                                         BPRED_WEATHER, W_HAIL | W_SNOW, 0, 0)));
        break;
    case ABILITY_SAND_FORCE:  // power x1.3 for Rock, Ground and Steel moves
        AddHook(a, HOOK(BABIL_WEATHER_IMMUNE, W_SAND, 0, 0, 0, 0, 0, 0));
        AddHook(a, ModifierHook(MODIFIER(BMOD_POWER, 130, BPRED_WEATHER, W_SAND, BPRED_MOVE_TYPE, TYPE_ROCK)));
        AddHook(a, ModifierHook(MODIFIER(BMOD_POWER, 130, BPRED_WEATHER, W_SAND, BPRED_MOVE_TYPE, TYPE_GROUND)));
        AddHook(a, ModifierHook(MODIFIER(BMOD_POWER, 130, BPRED_WEATHER, W_SAND, BPRED_MOVE_TYPE, TYPE_STEEL)));
        break;
    case ABILITY_OVERCOAT:    // its powder immunity: powder moves are unsupported in V15
        AddHook(a, HOOK(BABIL_WEATHER_IMMUNE, W_SAND | W_HAIL, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_RAIN_DISH:
        AddHook(a, HOOK(BABIL_WEATHER_RESIDUAL, W_RAIN, 1, 16, 0, 0, 0, 0));
        break;
    case ABILITY_ICE_BODY:    // instead of the hail damage; not underground
        AddHook(a, HOOK(BABIL_WEATHER_RESIDUAL, W_HAIL | W_SNOW, 1, 16, 0, 0, 0, BAHF_NOT_UNDERGROUND));
        break;
    case ABILITY_DRY_SKIN:    // Water absorbed (1/4); Fire power x1.25; 1/8 in rain and sun
        AddHook(a, HOOK(BABIL_TYPE_ABSORB, TYPE_WATER, 0, 0, 0, 0, 25, 0));
        AddHook(a, ModifierHook(MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_POWER, 0) | BMODF_DEFENDER, 125,
                                         BPRED_MOVE_TYPE, TYPE_FIRE, 0, 0)));
        AddHook(a, HOOK(BABIL_WEATHER_RESIDUAL, W_RAIN, 1, 8, 0, 0, 0, 0));
        AddHook(a, HOOK(BABIL_WEATHER_RESIDUAL, W_SUN, -1, 8, 0, 0, 0, 0));
        break;
    case ABILITY_SOLAR_POWER: // special attacking stat x1.5 in sun; 1/8 at the end of each sunny turn
        AddHook(a, ModifierHook(MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_ATTACK, BCAT_SPECIAL), 150,
                                         BPRED_WEATHER, W_SUN, 0, 0)));
        AddHook(a, HOOK(BABIL_WEATHER_RESIDUAL, W_SUN, -1, 8, 0, 0, 0, 0));
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

static BattleAbility ResolveAbility(enum Ability ability)
{
    BattleAbility a = {.cls = BACLASS_UNSUPPORTED, .source = ability};
    const bool8 v12 = sBattle->roster >= BATTLE_APP_VERSION_ABILITY_HOOKS;
    // Contact statuses: 30% from Gen 4, 1/3 before (V12 only: V11 chances are percentages).
    const bool8 percentChance = GetConfig(B_ABILITY_TRIGGER_CHANCE) >= GEN_4;
    const u8 contactNum = percentChance ? 30 : 1, contactDen = percentChance ? 100 : 3;
    const u8 intimidate = GetConfig(B_UPDATED_INTIMIDATE) >= GEN_8 ? BABIL_VOL_INTIMIDATE : 0;
    const u8 breakable = v12 && ability < ABILITIES_COUNT && gAbilitiesInfo[ability].breakable ? BAHF_BREAKABLE : 0;

    if (ResolveWeatherAbility(ability, &a))  // V15
    {
        for (u32 i = 0; i < a.count; i++)
            a.hooks[i].flags |= breakable;
        return a;
    }
    switch (ability)
    {
    // No battle effect in a GC trainer battle (escape, after-battle, items not simulated).
    case ABILITY_NONE:
    case ABILITY_RUN_AWAY:
    case ABILITY_PICKUP:
    case ABILITY_HONEY_GATHER:
    case ABILITY_BALL_FETCH:
        a.cls = BACLASS_NONE;
        return a;
    case ABILITY_HUGE_POWER:
    case ABILITY_PURE_POWER:
        AddHook(&a, HOOK(BABIL_ATTACK_MULT, BCAT_PHYSICAL, 0, 0, 0, 0, 200, 0));
        break;
    case ABILITY_BLAZE:    AddHook(&a, HOOK(BABIL_PINCH, TYPE_FIRE, 0, 3, 0, 0, 150, 0)); break;
    case ABILITY_TORRENT:  AddHook(&a, HOOK(BABIL_PINCH, TYPE_WATER, 0, 3, 0, 0, 150, 0)); break;
    case ABILITY_OVERGROW: AddHook(&a, HOOK(BABIL_PINCH, TYPE_GRASS, 0, 3, 0, 0, 150, 0)); break;
    case ABILITY_SWARM:    AddHook(&a, HOOK(BABIL_PINCH, TYPE_BUG, 0, 3, 0, 0, 150, 0)); break;
    case ABILITY_TECHNICIAN:
        AddHook(&a, HOOK(BABIL_LOW_POWER, 60, 0, 0, 0, 0, 150, 0));
        break;
    case ABILITY_THICK_FAT:
        AddHook(&a, HOOK(BABIL_RESIST, TYPE_FIRE, 0, TYPE_ICE, 0, 0, 50, 0));
        break;
    case ABILITY_LEVITATE:
        AddHook(&a, HOOK(BABIL_TYPE_IMMUNE, TYPE_GROUND, 0, 0, 0, 0, 0, 0));
        if (TerrainEnabled())  // V16: not grounded (IsBattlerUngroundedByAbilityItemOrEffect)
            AddHook(&a, HOOK(BABIL_UNGROUNDED, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_VOLT_ABSORB:
        AddHook(&a, HOOK(BABIL_TYPE_ABSORB, TYPE_ELECTRIC, 0, 0, 0, 0, 25, 0));
        break;
    case ABILITY_WATER_ABSORB:
        AddHook(&a, HOOK(BABIL_TYPE_ABSORB, TYPE_WATER, 0, 0, 0, 0, 25, 0));
        break;
    case ABILITY_LIMBER:
        AddHook(&a, HOOK(BABIL_STATUS_IMMUNE, 1u << BST_PARALYSIS, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_INSOMNIA:
    case ABILITY_VITAL_SPIRIT:
        AddHook(&a, HOOK(BABIL_STATUS_IMMUNE, 1u << BST_SLEEP, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_WATER_VEIL:
        AddHook(&a, HOOK(BABIL_STATUS_IMMUNE, 1u << BST_BURN, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_IMMUNITY:
        AddHook(&a, HOOK(BABIL_STATUS_IMMUNE, (1u << BST_POISON) | (1u << BST_TOXIC), 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_INNER_FOCUS:
        AddHook(&a, HOOK(BABIL_VOLATILE_IMMUNE, BABIL_VOL_FLINCH | intimidate, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_OWN_TEMPO:
        AddHook(&a, HOOK(BABIL_VOLATILE_IMMUNE, BABIL_VOL_CONFUSION | intimidate, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_CLEAR_BODY:
    case ABILITY_WHITE_SMOKE:
    case ABILITY_FULL_METAL_BODY:
        AddHook(&a, HOOK(BABIL_STAT_DROP_IMMUNE, 0x7f, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_HYPER_CUTTER:
        AddHook(&a, HOOK(BABIL_STAT_DROP_IMMUNE, 1u << BSTAT_ATK, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_BIG_PECKS:
        AddHook(&a, HOOK(BABIL_STAT_DROP_IMMUNE, 1u << BSTAT_DEF, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_ILLUMINATE:
        if (B_ILLUMINATE_EFFECT < GEN_9)
        {
            a.cls = BACLASS_NONE;  // wild encounter rate only
            return a;
        }
        // fallthrough
    case ABILITY_KEEN_EYE:
        AddHook(&a, HOOK(BABIL_STAT_DROP_IMMUNE, 1u << BSTAT_ACC, 0, BABIL_IGNORE_EVASION, 0, 0, 0, 0));
        break;
    case ABILITY_COMPOUND_EYES:
        AddHook(&a, HOOK(BABIL_ACCURACY, 0, 0, 0, 0, 0, 130, 0));
        break;
    case ABILITY_NO_GUARD:
        AddHook(&a, HOOK(BABIL_NO_GUARD, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_STATIC:
    case ABILITY_FLAME_BODY:
    case ABILITY_POISON_POINT:
        if (percentChance || v12)
            AddHook(&a, HOOK(BABIL_CONTACT_STATUS,
                             ability == ABILITY_STATIC ? BST_PARALYSIS : ability == ABILITY_FLAME_BODY ? BST_BURN : BST_POISON,
                             0, 0, contactNum, contactDen, 0, 0));
        break;
    case ABILITY_ROUGH_SKIN:
    case ABILITY_IRON_BARBS:
        AddHook(&a, HOOK(BABIL_CONTACT_DAMAGE, 0, 0, B_ROUGH_SKIN_DMG >= GEN_4 ? 8 : 16, 0, 0, 0, 0));
        break;
    case ABILITY_INTIMIDATE:
        // V12: adjacent foes (Gen 5/6 Triple); V11 has no range (every foe).
        AddHook(&a, HOOK(BABIL_ENTRY_STAT, BSTAT_ATK, -1, v12 ? BRANGE_ADJACENT_FOES : BRANGE_ALL_FOES, 0, 0, 0, 0));
        break;
    case ABILITY_SPEED_BOOST:
        AddHook(&a, HOOK(BABIL_END_TURN_STAT, BSTAT_SPE, 1, 0, 0, 0, 0, 0));
        break;
    case ABILITY_STURDY:
        if (GetConfig(B_STURDY) >= GEN_5)  // the older one only stops OHKO moves, which are refused
            AddHook(&a, HOOK(BABIL_STURDY, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_ROCK_HEAD:
        AddHook(&a, HOOK(BABIL_NO_RECOIL, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_SHIELD_DUST:
        AddHook(&a, HOOK(BABIL_SHIELD_DUST, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_SERENE_GRACE:
        AddHook(&a, HOOK(BABIL_SECONDARY_CHANCE, 0, 0, 0, 0, 0, 200, 0));
        break;
    case ABILITY_INFILTRATOR:
        if (GetConfig(B_INFILTRATOR_SUBSTITUTE) >= GEN_6)
            AddHook(&a, HOOK(BABIL_INFILTRATOR, 0, 0, 0, 0, 0, 0, 0));
        break;
    // Milestone 4-A2 (V12 only).
    case ABILITY_GUTS:
        if (!v12)
            break;
        AddHook(&a, HOOK(BABIL_ATTACK_MULT, BCAT_PHYSICAL, 0, 0, 0, 0, 150, BAHF_REQUIRES_STATUS));
        AddHook(&a, HOOK(BABIL_IGNORE_BURN, 0, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_MARVEL_SCALE:
        if (v12)
            AddHook(&a, HOOK(BABIL_DEFENSE_MULT, BCAT_PHYSICAL, 0, 0, 0, 0, 150, BAHF_REQUIRES_STATUS));
        break;
    case ABILITY_ADAPTABILITY:
        if (v12)
            AddHook(&a, HOOK(BABIL_STAB_MULT, 0, 0, 0, 0, 0, 200, 0));
        break;
    case ABILITY_MOXIE:
        if (v12)
            AddHook(&a, HOOK(BABIL_KO_STAT, BSTAT_ATK, 1, 0, 0, 0, 0, 0));
        break;
    case ABILITY_SHED_SKIN:
        // 30% only under the Gen 4 setting, 1/3 otherwise (as in this build's end-of-turn code).
        if (v12)
            AddHook(&a, HOOK(BABIL_END_TURN_CURE, 0xfe, 0, 0,
                             GetConfig(B_ABILITY_TRIGGER_CHANCE) == GEN_4 ? 30 : 1,
                             GetConfig(B_ABILITY_TRIGGER_CHANCE) == GEN_4 ? 100 : 3, 0, 0));
        break;
    case ABILITY_FLASH_FIRE:
        if (!v12)
            break;
        AddHook(&a, HOOK(BABIL_ABSORB_STATE, TYPE_FIRE, 0, 0, 0, 0, 0, B_FLASH_FIRE_FROZEN >= GEN_5 ? 0 : BAHF_NOT_FROZEN));
        AddHook(&a, HOOK(BABIL_ATTACK_MULT, 0, 0, TYPE_FIRE, 0, 0, 150, BAHF_REQUIRES_STATE | BAHF_TYPE_FILTER));
        break;
    case ABILITY_SCRAPPY:
        if (!v12)
            break;
        AddHook(&a, HOOK(BABIL_TYPE_PASS, TYPE_NORMAL, 0, TYPE_GHOST, 0, 0, 0, 0));
        AddHook(&a, HOOK(BABIL_TYPE_PASS, TYPE_FIGHTING, 0, TYPE_GHOST, 0, 0, 0, 0));
        if (intimidate)
            AddHook(&a, HOOK(BABIL_VOLATILE_IMMUNE, intimidate, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_OBLIVIOUS:
        // Its other effects (infatuation, Taunt, Captivate) have no move or ability here that
        // the GC runs; before Gen 8 it therefore does nothing in a GC battle.
        if (!v12)
            break;
        if (!intimidate)
        {
            a.cls = BACLASS_NONE;
            return a;
        }
        AddHook(&a, HOOK(BABIL_VOLATILE_IMMUNE, intimidate, 0, 0, 0, 0, 0, 0));
        break;
    case ABILITY_SYNCHRONIZE:
        if (v12)
            AddHook(&a, HOOK(BABIL_STATUS_MIRROR, (1u << BST_POISON) | (1u << BST_TOXIC) | (1u << BST_PARALYSIS) | (1u << BST_BURN),
                             0, 0, 0, 0, 0, GetConfig(B_SYNCHRONIZE_TOXIC) >= GEN_5 ? 0 : BAHF_TOXIC_TO_POISON));
        break;
    default:
        ExportAbilityModifiers(ability, &a);  // V14: abilities that are modifier records only
        break;
    }
    for (u32 i = 0; i < a.count; i++)
        a.hooks[i].flags |= breakable;
    return a;
}

static void ExportAbilityModifiers(enum Ability ability, BattleAbility *a)
{
    if (sBattle->roster < BATTLE_APP_VERSION_FIXED_POINT)
        return;
    for (u32 i = 0; i < ARRAY_COUNT(sAbilityModifiers); i++)
    {
        if (sAbilityModifiers[i].ability == ability)
            AddHook(a, ModifierHook(sAbilityModifiers[i].mod));
    }
}

// ---------------------------------------------------------------------------
// Request V17 held items (gba-gc-extension docs/items.md): each Pokemon's item
// as the GC's item block, read from this build's item data and battle code
// (CalcMoveBasePowerAfterModifiers, TryChangeWeather, TryChangeBattleTerrain).
// An item is NONE only if it has no effect in a GC trainer battle, a block of
// records only if the GC runs all of its effect, and UNSUPPORTED otherwise
// (the default). A definition is the item's: nothing of its holder goes in.
static bool8 ItemsEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_ITEMS;
}

// The holder's species changes form in battle because of this item (Zacian's
// Rusted Sword at the battle start, primal reversion, Mega Evolution): the GC
// keeps a Pokemon's species, so the item is not simulated for this holder.
// Item-hold and item-use forms are applied outside battles.
static bool8 ItemChangesFormInBattle(u16 species, u16 item)
{
    const struct FormChange *f = GetSpeciesFormChanges(species);
    for (; f != NULL && f->method != FORM_CHANGE_TERMINATOR; f++)
    {
        if (f->method != FORM_CHANGE_ITEM_HOLD && f->method != FORM_CHANGE_ITEM_USE && f->param1 == item)
            return TRUE;
    }
    return FALSE;
}

// ---------------------------------------------------------------------------
// M4-B1a+ (V18): audited Z-Crystals and Mega Stones (gba-gc-extension
// docs/items.md 11). Their battle effect is a gimmick (a Z-Move, Mega
// Evolution) that a request with RULE_NO_GIMMICKS never runs, so they are
// held with no effect: class NONE with the item's id kept (Knock Off, Trick,
// Poltergeist and Acrobatics see a held item; the GC runs none of them).
// Each item below was checked in this build: its hold effect, no parameter,
// and every form change that names it is Mega Evolution with its own stone.
// Not listed, so UNSUPPORTED as before: Ultranecrozium Z (Ultra Burst), the
// Primal orbs (an automatic form change), and any item added later.
// ResolveItem leaves a candidate UNSUPPORTED; AcceptGimmickItems makes it
// NONE once the battle's rules are final, for the whole battle or not at all.
static const u16 sAuditedGimmickItems[] = {
    // The 18 type Z-Crystals.
    ITEM_NORMALIUM_Z, ITEM_FIRIUM_Z, ITEM_WATERIUM_Z, ITEM_ELECTRIUM_Z, ITEM_GRASSIUM_Z, ITEM_ICIUM_Z,
    ITEM_FIGHTINIUM_Z, ITEM_POISONIUM_Z, ITEM_GROUNDIUM_Z, ITEM_FLYINIUM_Z, ITEM_PSYCHIUM_Z, ITEM_BUGINIUM_Z,
    ITEM_ROCKIUM_Z, ITEM_GHOSTIUM_Z, ITEM_DRAGONIUM_Z, ITEM_DARKINIUM_Z, ITEM_STEELIUM_Z, ITEM_FAIRIUM_Z,
    // The signature Z-Crystals but Ultranecrozium Z.
    ITEM_PIKANIUM_Z, ITEM_EEVIUM_Z, ITEM_SNORLIUM_Z, ITEM_MEWNIUM_Z, ITEM_DECIDIUM_Z, ITEM_INCINIUM_Z,
    ITEM_PRIMARIUM_Z, ITEM_LYCANIUM_Z, ITEM_MIMIKIUM_Z, ITEM_KOMMONIUM_Z, ITEM_TAPUNIUM_Z, ITEM_SOLGANIUM_Z,
    ITEM_LUNALIUM_Z, ITEM_MARSHADIUM_Z, ITEM_ALORAICHIUM_Z, ITEM_PIKASHUNIUM_Z,
    // The 92 Mega Stones of this build.
    ITEM_VENUSAURITE, ITEM_CHARIZARDITE_X, ITEM_CHARIZARDITE_Y, ITEM_BLASTOISINITE, ITEM_BEEDRILLITE,
    ITEM_PIDGEOTITE, ITEM_ALAKAZITE, ITEM_SLOWBRONITE, ITEM_GENGARITE, ITEM_KANGASKHANITE,
    ITEM_PINSIRITE, ITEM_GYARADOSITE, ITEM_AERODACTYLITE, ITEM_MEWTWONITE_X, ITEM_MEWTWONITE_Y,
    ITEM_AMPHAROSITE, ITEM_STEELIXITE, ITEM_SCIZORITE, ITEM_HERACRONITE, ITEM_HOUNDOOMINITE,
    ITEM_TYRANITARITE, ITEM_SCEPTILITE, ITEM_BLAZIKENITE, ITEM_SWAMPERTITE, ITEM_GARDEVOIRITE,
    ITEM_SABLENITE, ITEM_MAWILITE, ITEM_AGGRONITE, ITEM_MEDICHAMITE, ITEM_MANECTITE,
    ITEM_SHARPEDONITE, ITEM_CAMERUPTITE, ITEM_ALTARIANITE, ITEM_BANETTITE, ITEM_ABSOLITE,
    ITEM_GLALITITE, ITEM_SALAMENCITE, ITEM_METAGROSSITE, ITEM_LATIASITE, ITEM_LATIOSITE,
    ITEM_LOPUNNITE, ITEM_GARCHOMPITE, ITEM_LUCARIONITE, ITEM_ABOMASITE, ITEM_GALLADITE,
    ITEM_AUDINITE, ITEM_DIANCITE, ITEM_CLEFABLITE, ITEM_VICTREEBELITE, ITEM_STARMINITE,
    ITEM_DRAGONINITE, ITEM_MEGANIUMITE, ITEM_FERALIGITE, ITEM_SKARMORITE, ITEM_FROSLASSITE,
    ITEM_EMBOARITE, ITEM_EXCADRITE, ITEM_SCOLIPITE, ITEM_SCRAFTINITE, ITEM_EELEKTROSSITE,
    ITEM_CHANDELURITE, ITEM_CHESNAUGHTITE, ITEM_DELPHOXITE, ITEM_GRENINJITE, ITEM_PYROARITE,
    ITEM_FLOETTITE, ITEM_MALAMARITE, ITEM_BARBARACITE, ITEM_DRAGALGITE, ITEM_HAWLUCHANITE,
    ITEM_ZYGARDITE, ITEM_DRAMPANITE, ITEM_FALINKSITE, ITEM_HEATRANITE, ITEM_DARKRANITE,
    ITEM_ZERAORITE, ITEM_RAICHUNITE_X, ITEM_RAICHUNITE_Y, ITEM_CHIMECHITE, ITEM_ABSOLITE_Z,
    ITEM_STARAPTITE, ITEM_GARCHOMPITE_Z, ITEM_LUCARIONITE_Z, ITEM_GOLURKITE, ITEM_MEOWSTICITE,
    ITEM_CRABOMINITE, ITEM_GOLISOPITE, ITEM_MAGEARNITE, ITEM_SCOVILLAINITE, ITEM_BAXCALIBRITE,
    ITEM_TATSUGIRINITE, ITEM_GLIMMORANITE,
};

enum { GIMMICK_ITEM_NONE, GIMMICK_ITEM_Z_CRYSTAL, GIMMICK_ITEM_MEGA_STONE };

// The kind of audited gimmick item `item` is for this holder, or NONE: not
// for a Mega, Primal, Ultra Burst, Gigantamax or Tera form (a gimmick form),
// not a Z-Crystal for Arceus (its type Z-Crystals are Multitype form items),
// and not if any form change of the holder's species names the item, but
// Mega Evolution with a Mega Stone.
static u8 GimmickItemCandidate(u16 item, u16 species)
{
    const struct FormChange *f;
    const struct SpeciesInfo *info;
    enum HoldEffect hold;
    u32 i;
    u8 kind;

    if (item == ITEM_NONE || item >= ITEMS_COUNT || species == SPECIES_NONE || species >= NUM_SPECIES
     || !IsSpeciesEnabled(species))
        return GIMMICK_ITEM_NONE;
    for (i = 0; i < ARRAY_COUNT(sAuditedGimmickItems) && sAuditedGimmickItems[i] != item; i++)
        ;
    if (i == ARRAY_COUNT(sAuditedGimmickItems) || GetItemHoldEffectParam(item) != 0)
        return GIMMICK_ITEM_NONE;
    hold = GetItemHoldEffect(item);
    kind = hold == HOLD_EFFECT_Z_CRYSTAL ? GIMMICK_ITEM_Z_CRYSTAL
         : hold == HOLD_EFFECT_MEGA_STONE ? GIMMICK_ITEM_MEGA_STONE : GIMMICK_ITEM_NONE;
    info = &gSpeciesInfo[species];
    if (kind == GIMMICK_ITEM_NONE || info->isMegaEvolution || info->isPrimalReversion || info->isUltraBurst
     || info->isGigantamax || info->isTeraForm
     || (kind == GIMMICK_ITEM_Z_CRYSTAL && GET_BASE_SPECIES_ID(species) == SPECIES_ARCEUS))
        return GIMMICK_ITEM_NONE;
    for (f = GetSpeciesFormChanges(species); f != NULL && f->method != FORM_CHANGE_TERMINATOR; f++)
    {
        if (f->param1 == item && !(kind == GIMMICK_ITEM_MEGA_STONE && f->method == FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM))
            return GIMMICK_ITEM_NONE;
    }
    return kind;
}

// A form change this build makes by itself in a battle without gimmicks: at
// the battle start or on entry, by HP, weather, status, a move, a hit, the
// turn end or Battle Bond, or Primal Reversion with its orb held. The GC
// keeps every Pokemon's species, so a battle with such a Pokemon (either
// side, the bench too) keeps every gimmick item UNSUPPORTED. Outside battle,
// faint and end-of-battle reverts and the gimmicks themselves do not count.
static bool8 ChangesFormByItself(u16 species, u16 item)
{
    const struct FormChange *f;
    for (f = GetSpeciesFormChanges(species); f != NULL && f->method != FORM_CHANGE_TERMINATOR; f++)
    {
        switch (f->method)
        {
        case FORM_CHANGE_BEGIN_BATTLE:
        case FORM_CHANGE_BATTLE_SWITCH_OUT:
        case FORM_CHANGE_BATTLE_SWITCH_IN:
        case FORM_CHANGE_BATTLE_HP_PERCENT_TURN_END:
        case FORM_CHANGE_BATTLE_HP_PERCENT_SEND_OUT:
        case FORM_CHANGE_BATTLE_HP_PERCENT_DURING_MOVE:
        case FORM_CHANGE_BATTLE_WEATHER:
        case FORM_CHANGE_BATTLE_TURN_END:
        case FORM_CHANGE_STATUS:
        case FORM_CHANGE_BATTLE_HIT_BY_MOVE_CATEGORY:
        case FORM_CHANGE_BATTLE_HIT_BY_CONFUSION_SELF_DMG:
        case FORM_CHANGE_BATTLE_BEFORE_MOVE:
        case FORM_CHANGE_BATTLE_AFTER_MOVE:
        case FORM_CHANGE_BATTLE_BOND:
        case FORM_CHANGE_BATTLE_BEFORE_MOVE_CATEGORY:
            return TRUE;
        case FORM_CHANGE_BATTLE_PRIMAL_REVERSION:
            if (f->param1 == item)
                return TRUE;
            break;
        default:
            break;
        }
    }
    return FALSE;
}

// Moves that read, take, swap, remove or use held items, and the gimmick
// moves (Z-Moves, Max Moves). All are unsupported moves (the GC never runs
// them), but in this build they would see a held Z-Crystal or Mega Stone
// (Knock Off, Trick, Poltergeist, Acrobatics ...): a battle where any
// Pokemon knows one keeps gimmick items UNSUPPORTED.
static bool8 MoveUsesHeldItems(u16 move)
{
    u32 i;
    if (move >= FIRST_Z_MOVE && move < FIRST_SHADOW_MOVE)
        return TRUE;
    switch (GetMoveEffect(move))
    {
    case EFFECT_KNOCK_OFF:
    case EFFECT_STEAL_ITEM:
    case EFFECT_TRICK:
    case EFFECT_BESTOW:
    case EFFECT_FLING:
    case EFFECT_ACROBATICS:
    case EFFECT_POLTERGEIST:
    case EFFECT_CORROSIVE_GAS:
    case EFFECT_EMBARGO:
    case EFFECT_MAGIC_ROOM:
    case EFFECT_RECYCLE:
    case EFFECT_NATURAL_GIFT:
    case EFFECT_CHANGE_TYPE_ON_ITEM:
    case EFFECT_BELCH:
    case EFFECT_STUFF_CHEEKS:
    case EFFECT_TEATIME:
        return TRUE;
    default:
        break;
    }
    for (i = 0; i < GetMoveAdditionalEffectCount(move); i++)
    {
        const enum MoveEffect effect = GetMoveAdditionalEffectById(move, i)->moveEffect;
        if (effect == MOVE_EFFECT_BUG_BITE || effect == MOVE_EFFECT_INCINERATE)
            return TRUE;
    }
    return FALSE;
}

// Once the battle's rules are final, before CheckItems: every candidate of
// the battle becomes NONE (held, no effect) only if the request is V18,
// forbids gimmicks, no Pokemon changes form by itself and no Pokemon knows
// a move that uses held items (supported or not); otherwise every one stays UNSUPPORTED, so CheckItems
// turns every item off (strict: refuses), as for any unsupported item. Item
// abilities (Klutz, Frisk ...) are CheckItems' as before: they turn every
// item off, accepted gimmick items with them.
static u8 AcceptGimmickItems(BattleRequest *r)
{
    bool8 any = FALSE;
    u32 side, i, m;
    if (LEGACY_EXPORT())
        return FGC_GIMMICK_ITEMS_NONE;
    for (side = 0; side < 2; side++)
    {
        BattleEntry *e = side ? r->opponent : r->player;
        const u32 count = side ? r->opponent_count : r->player_count;
        for (i = 0; i < count; i++)
        {
            if (e[i].mon.item.cls == BICLASS_UNSUPPORTED
             && GimmickItemCandidate(e[i].record.held_item, SanitizeSpeciesId(e[i].record.species)))
                any = TRUE;
        }
    }
    if (!any)
        return FGC_GIMMICK_ITEMS_NONE;
    if (r->version < BATTLE_REQUEST_VERSION_END_TURN_ITEMS)
        return FGC_GIMMICK_ITEMS_OFF_VERSION;
    if (!(r->rules & RULE_NO_GIMMICKS))
        return FGC_GIMMICK_ITEMS_OFF_RULES;
    for (side = 0; side < 2; side++)
    {
        const BattleEntry *e = side ? r->opponent : r->player;
        const u32 count = side ? r->opponent_count : r->player_count;
        for (i = 0; i < count; i++)
        {
            if (ChangesFormByItself(SanitizeSpeciesId(e[i].record.species), e[i].record.held_item))
                return FGC_GIMMICK_ITEMS_OFF_FORM;
            for (m = 0; m < MAX_MON_MOVES; m++)
            {
                if (e[i].record.moves[m] != MOVE_NONE && MoveUsesHeldItems(e[i].record.moves[m]))
                    return FGC_GIMMICK_ITEMS_OFF_MOVE;
            }
        }
    }
    for (side = 0; side < 2; side++)
    {
        BattleEntry *e = side ? r->opponent : r->player;
        const u32 count = side ? r->opponent_count : r->player_count;
        for (i = 0; i < count; i++)
        {
            if (e[i].mon.item.cls == BICLASS_UNSUPPORTED
             && GimmickItemCandidate(e[i].record.held_item, SanitizeSpeciesId(e[i].record.species)))
            {
                e[i].mon.item.cls = BICLASS_NONE;  // the id stays (source = held_item)
                e[i].mon.item.count = 0;
            }
        }
    }
    return FGC_GIMMICK_ITEMS_ACCEPTED;
}

#define ITEM_FIELD_TURNS 8  // TryChangeWeather / TryChangeBattleTerrain with the rock or Terrain Extender

// Request V18: end-of-turn HP records (FIRST_EVENT_BLOCK_HEAL_ITEMS). The
// divisors are this build's code, not item data: TryLeftovers heals
// GetNonDynamaxMaxHP / 16, TryBlackSludgeDamage hurts by / 8 (both at least 1:
// SetHealAmount, SetPassiveDamageAmount). GC battles have no Dynamax
// (RULE_NO_GIMMICKS), so that max HP is the max HP.
static bool8 EndTurnItemsEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_END_TURN_ITEMS;
}

#define ITEM_LEFTOVERS_DIVISOR 16
#define ITEM_BLACK_SLUDGE_DIVISOR 8

static BattleItemRecord ItemHp(u8 op, u8 divisor, u8 pred, u8 arg)
{
    BattleItemRecord x = {.kind = BITEM_HP};
    x.hp.timing = BITIME_END_TURN;
    x.hp.op = op;
    x.hp.divisor = divisor;
    x.hp.pred = (BattlePredicate){pred, arg};
    return x;
}

static void AddItemRecord(BattleItem *it, BattleItemRecord record)
{
    it->records[it->count++] = record;
    it->cls = BICLASS_RECORDS;
}

static BattleItemRecord ItemModifier(BattleModifier mod)
{
    BattleItemRecord x = {.kind = BITEM_MODIFIER};
    x.mod = mod;
    return x;
}

// Request V19 consumable items (gba-gc-extension docs/items.md section 12),
// from this build's ItemBattleEffects: the HP berries (ItemHealHp) and the
// status berries (TryCure*), each used up when it acts. Every other berry
// (stat, pinch, type-resist, confusion-heal, Leppa ...) stays UNSUPPORTED.
static bool8 ConsumableItemsEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_CONSUMABLE_ITEMS;
}

// HasEnoughHpToEatBerry(battler, ability, 2, item): HP at most max HP / 2.
#define ITEM_BERRY_HP_FRACTION 2

// Request V20 after-move items (gba-gc-extension docs/items.md section 14):
// Life Orb's final damage factor and its HP loss after the holder's move.
static bool8 MoveItemsEnabled(void)
{
    return sBattle->roster >= BATTLE_APP_VERSION_MOVE_ITEMS;
}

// TryLifeOrb: GetNonDynamaxMaxHP / 10 (SetPassiveDamageAmount: at least 1).
#define ITEM_LIFE_ORB_DIVISOR 10

static void ResolveConsumable(u16 item, BattleItem *it)
{
    const u32 param = GetItemHoldEffectParam(item);
    BattleItemRecord x = {.flags = BIREC_CONSUMED};
    switch (GetItemHoldEffect(item))
    {
    // ItemHealHp: a fixed amount (Oran Berry), or max HP * percent / 100
    // (Sitrus Berry), which is max HP / (100 / percent) when that divides 100.
    case HOLD_EFFECT_RESTORE_HP:
    case HOLD_EFFECT_RESTORE_PCT_HP:
        if (param == 0 || param > 255
         || (GetItemHoldEffect(item) == HOLD_EFFECT_RESTORE_PCT_HP && (param > 100 || 100 % param != 0)))
            return;
        x.kind = BITEM_HP;
        x.hp.timing = BITIME_HP_THRESHOLD;
        x.hp.op = GetItemHoldEffect(item) == HOLD_EFFECT_RESTORE_HP ? BIHP_HEAL_FIXED : BIHP_HEAL;
        x.hp.divisor = GetItemHoldEffect(item) == HOLD_EFFECT_RESTORE_HP ? param : 100 / param;
        x.hp.pred = (BattlePredicate){BPRED_HP_AT_MOST, ITEM_BERRY_HP_FRACTION};
        break;
    // TryCureParalysis / TryCurePoison (and bad poison) / TryCureBurn /
    // TryCureFreezeOrFrostbite / TryCureSleep / TryCureConfusion /
    // TryCureAnyStatus (every status, then confusion).
    case HOLD_EFFECT_CURE_PAR:       x.cure.status = 1u << BST_PARALYSIS; break;
    case HOLD_EFFECT_CURE_PSN:       x.cure.status = 1u << BST_POISON | 1u << BST_TOXIC; break;
    case HOLD_EFFECT_CURE_BRN:       x.cure.status = 1u << BST_BURN; break;
    case HOLD_EFFECT_CURE_FRZ:       x.cure.status = 1u << BST_FREEZE | 1u << BST_FROSTBITE; break;
    case HOLD_EFFECT_CURE_SLP:       x.cure.status = 1u << BST_SLEEP; break;
    case HOLD_EFFECT_CURE_CONFUSION: x.cure.volatiles = BIVOL_CONFUSION; break;
    case HOLD_EFFECT_CURE_STATUS:
        x.cure.status = 1u << BST_POISON | 1u << BST_TOXIC | 1u << BST_BURN | 1u << BST_PARALYSIS
                      | 1u << BST_SLEEP | 1u << BST_FREEZE | 1u << BST_FROSTBITE;
        x.cure.volatiles = BIVOL_CONFUSION;
        break;
    default:
        return;
    }
    if (x.kind != BITEM_HP)
    {
        x.kind = BITEM_CURE;
        x.cure.timing = BITIME_STATUS;
    }
    AddItemRecord(it, x);
}

static BattleItem ResolveItem(u16 item, u16 species)
{
    BattleItem it = {.cls = BICLASS_UNSUPPORTED, .source = item};
    enum HoldEffect hold;
    u32 param, i;
    u8 mask = 0;

    if (item == ITEM_NONE)
    {
        it.cls = BICLASS_NONE;
        return it;
    }
    // Berries are consumed: from V19 the HP and status berries of ResolveConsumable
    // (M4-B1b); every other berry stays UNSUPPORTED. The e-Reader Enigma Berry comes
    // from the save.
    if (item >= ITEMS_COUNT || item == ITEM_ENIGMA_BERRY_E_READER || ItemChangesFormInBattle(species, item))
        return it;
    if (GetItemPocket(item) == POCKET_BERRIES)
    {
        if (ConsumableItemsEnabled())
            ResolveConsumable(item, &it);
        return it;
    }
    hold = GetItemHoldEffect(item);
    param = GetItemHoldEffectParam(item);
    if (param > 100)
        param = 100;  // CalcMoveBasePowerAfterModifiers caps the attacker's holdEffectParam
    switch (hold)
    {
    // No effect in a GC trainer battle: prize money, experience, friendship,
    // evolution, wild encounters and fleeing do not exist there.
    case HOLD_EFFECT_NONE:
    case HOLD_EFFECT_DOUBLE_PRIZE:
    case HOLD_EFFECT_EXP_SHARE:
    case HOLD_EFFECT_LUCKY_EGG:
    case HOLD_EFFECT_FRIENDSHIP_UP:
    case HOLD_EFFECT_PREVENT_EVOLVE:
    case HOLD_EFFECT_REPEL:
    case HOLD_EFFECT_CAN_ALWAYS_RUN:
        it.cls = BICLASS_NONE;
        break;
    // Physical / special power x(1 + PercentToUQ4_12_Floored(param)): the truncated factor.
    case HOLD_EFFECT_MUSCLE_BAND:
    case HOLD_EFFECT_WISE_GLASSES:
        AddItemRecord(&it, ItemModifier(MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_POWER, hold == HOLD_EFFECT_MUSCLE_BAND
                                                                       ? BCAT_PHYSICAL : BCAT_SPECIAL) | BMOD_FLOORED,
                                                 100 + param, 0, 0, 0, 0)));
        break;
    // Moves of the item's type x(1 + PercentToUQ4_12(param)): the nearest factor.
    case HOLD_EFFECT_TYPE_POWER:
        if (GetItemSecondaryId(item) >= NUMBER_OF_MON_TYPES)
            break;
        if (sBattle->gen3Items)  // Lab: the Gen 3 values (I_TYPE_BOOST_POWER < GEN_4)
            param = item == ITEM_SEA_INCENSE ? 5 : 10;
        AddItemRecord(&it, ItemModifier(MODIFIER(BMOD_POWER, 100 + param, BPRED_MOVE_TYPE, GetItemSecondaryId(item), 0, 0)));
        break;
    // The weathers whose rock this is (gBattleWeatherInfo), set for a limited time: 8 turns.
    case HOLD_EFFECT_DAMP_ROCK:
    case HOLD_EFFECT_HEAT_ROCK:
    case HOLD_EFFECT_SMOOTH_ROCK:
    case HOLD_EFFECT_ICY_ROCK:
        if (!WeatherEnabled())
            break;
        for (i = 0; i < BATTLE_WEATHER_COUNT; i++)
        {
            if (gBattleWeatherInfo[i].rock == hold && ExportWeather(i) != BWEATHER_NONE)
                mask |= BWEATHER_BIT(ExportWeather(i));
        }
        if (mask)
            AddItemRecord(&it, (BattleItemRecord){.kind = BITEM_WEATHER_TURNS, .arg0 = mask, .arg2 = ITEM_FIELD_TURNS});
        break;
    case HOLD_EFFECT_TERRAIN_EXTENDER:
        if (TerrainEnabled())
            AddItemRecord(&it, (BattleItemRecord){.kind = BITEM_TERRAIN_TURNS, .arg0 = BTERRAIN_ALL,
                                                  .arg2 = ITEM_FIELD_TURNS});
        break;
    // V18, ItemBattleEffects at the end of the turn: TryLeftovers heals below
    // max HP unless Heal Block (a move the GC does not run) is in effect.
    case HOLD_EFFECT_LEFTOVERS:
        if (EndTurnItemsEnabled())
            AddItemRecord(&it, ItemHp(BIHP_HEAL, ITEM_LEFTOVERS_DIVISOR, BPRED_NONE, 0));
        break;
    // A Poison-type holder (IS_BATTLER_OF_TYPE, its types now) heals as with
    // Leftovers; any other is hurt (TryBlackSludgeDamage; Magic Guard: CheckItems).
    case HOLD_EFFECT_BLACK_SLUDGE:
        if (EndTurnItemsEnabled())
        {
            AddItemRecord(&it, ItemHp(BIHP_HEAL, ITEM_LEFTOVERS_DIVISOR, BPRED_HAS_TYPE, TYPE_POISON));
            AddItemRecord(&it, ItemHp(BIHP_DAMAGE, ITEM_BLACK_SLUDGE_DIVISOR, BPRED_NONE, 0));
        }
        break;
    // V19, GetAdjustedDamage: after Sturdy, a hit that would take all the HP
    // of a holder at full HP (IsBattlerAtMaxHp) leaves 1 HP; the sash is used
    // up (BattleScript_HangedOnMsg). Focus Band (a chance) is not this.
    case HOLD_EFFECT_FOCUS_SASH:
        if (ConsumableItemsEnabled())
        {
            BattleItemRecord x = {.kind = BITEM_ENDURE, .flags = BIREC_CONSUMED};
            x.endure.timing = BITIME_LETHAL_HIT;
            x.endure.condition = BIEND_FULL_HP;
            AddItemRecord(&it, x);
        }
        break;
    // V20: GetAttackerItemsModifier gives UQ_4_12_FLOORED(1.3) in the final
    // chain (GetOtherModifiers); TryLifeOrb at MOVEEND_LIFE_ORB_SHELL_BELL takes
    // max HP / 10 after a move that landed. The item is kept. Magic Guard and
    // Sheer Force change what it does: HpItemAbility.
    case HOLD_EFFECT_LIFE_ORB:
        if (MoveItemsEnabled())
        {
            BattleItemRecord x = {.kind = BITEM_HP};
            AddItemRecord(&it, ItemModifier(MODIFIER(BMOD_FINAL | BMOD_FLOORED, 130, 0, 0, 0, 0)));
            x.hp.timing = BITIME_AFTER_MOVE;
            x.hp.op = BIHP_DAMAGE;
            x.hp.divisor = ITEM_LIFE_ORB_DIVISOR;
            AddItemRecord(&it, x);
        }
        break;
    default:
        break;
    }
    return it;
}

// Abilities whose battle effect involves held items: Klutz turns its holder's
// item off, Pickpocket and Magician take items, Frisk reveals them, the rest
// act on consumed, removed or form items. The GC runs none of them, so with
// any held item in the battle every item is normalised away (strict: refused),
// never just the holder's.
static bool8 ItemAbility(u16 ability)
{
    switch (ability)
    {
    case ABILITY_KLUTZ:
    case ABILITY_PICKPOCKET:
    case ABILITY_MAGICIAN:
    case ABILITY_STICKY_HOLD:
    case ABILITY_FRISK:
    case ABILITY_UNBURDEN:
    case ABILITY_SYMBIOSIS:
    case ABILITY_HARVEST:
    case ABILITY_GLUTTONY:
    case ABILITY_CHEEK_POUCH:
    case ABILITY_RIPEN:
    case ABILITY_UNNERVE:
    case ABILITY_AS_ONE_ICE_RIDER:
    case ABILITY_AS_ONE_SHADOW_RIDER:
    case ABILITY_MULTITYPE:
    case ABILITY_RKS_SYSTEM:
    case ABILITY_PICKUP:
    case ABILITY_BALL_FETCH:
        return TRUE;
    default:
        return FALSE;
    }
}

// V18: abilities of an HP record's own holder that change what the record
// does, read from the records (not the item id). Magic Guard stops the damage
// (TryBlackSludgeDamage); Color Change, Protean, Libero and Imposter change
// the holder's types (the type predicate); Power Construct and Tera Shift
// change its max HP (the amount); Trace, Receiver, Power of Alchemy and
// Wandering Spirit can give the holder Magic Guard or a type changer. The GC
// runs none of them, so such a holder turns every item off (strict: refused).
// V19: Cud Chew eats its used-up berry again the next turn (Cmd_removeitem:
// cudChewTimer). The GC does not, so a holder of a used-up berry with it
// turns every item off (strict: refused).
static bool8 ConsumableItemAbility(u16 ability, u16 item, const BattleItem *it)
{
    u32 i;
    if (ability != ABILITY_CUD_CHEW || GetItemPocket(item) != POCKET_BERRIES)
        return FALSE;
    for (i = 0; i < it->count; i++)
    {
        if (it->records[i].flags & BIREC_CONSUMED)
            return TRUE;
    }
    return FALSE;
}

// V20: Sheer Force skips the after-move stage for the moves it affects
// (MOVEEND_SHEER_FORCE jumps past MOVEEND_LIFE_ORB_SHELL_BELL), which depends
// on each move's effects; Magic Guard stops the loss (TryLifeOrb) but not the
// damage factor. The GC runs neither, so an after-move record's holder with
// one of them turns every item off (strict: refused).
static bool8 HpItemAbility(u16 ability, const BattleItem *it)
{
    bool8 hp = FALSE, damage = FALSE, typed = FALSE, afterMove = FALSE;
    u32 i;
    for (i = 0; i < it->count; i++)
    {
        if (it->records[i].kind != BITEM_HP)
            continue;
        hp = TRUE;
        damage |= it->records[i].hp.op == BIHP_DAMAGE;
        typed |= it->records[i].hp.pred.kind == BPRED_HAS_TYPE;
        afterMove |= it->records[i].hp.timing == BITIME_AFTER_MOVE;
    }
    if (!hp)
        return FALSE;
    if (afterMove && ability == ABILITY_SHEER_FORCE)
        return TRUE;
    switch (ability)
    {
    case ABILITY_POWER_CONSTRUCT:
    case ABILITY_TERA_SHIFT:
        return TRUE;
    case ABILITY_MAGIC_GUARD:
        return damage;
    case ABILITY_COLOR_CHANGE:
    case ABILITY_PROTEAN:
    case ABILITY_LIBERO:
    case ABILITY_IMPOSTER:
        return typed;
    case ABILITY_TRACE:
    case ABILITY_RECEIVER:
    case ABILITY_POWER_OF_ALCHEMY:
    case ABILITY_WANDERING_SPIRIT:
        return damage || typed;
    default:
        return FALSE;
    }
}

// The whole battle's items: simulated only if every item is simulated (or has
// no effect) and no Pokemon has an item ability; otherwise all are off, never
// a part (RULE_IGNORE_ITEMS). Moves that use items (Knock Off, Thief, Trick,
// Fling, Bug Bite, Incinerate, Acrobatics, Natural Gift, Recycle, Embargo,
// Magic Room ...) are unsupported moves: they are never run.
static u8 CheckItems(const BattleRequest *r, u16 *why)
{
    bool8 held = FALSE;
    u16 ability = ABILITY_NONE;
    u32 side, i;
    for (side = 0; side < 2; side++)
    {
        const BattleEntry *e = side ? r->opponent : r->player;
        const u32 count = side ? r->opponent_count : r->player_count;
        for (i = 0; i < count; i++)
        {
            if (e[i].mon.item.cls == BICLASS_UNSUPPORTED)
            {
                *why = e[i].record.held_item;
                return ITEMS_OFF_ITEM;
            }
            if (e[i].record.held_item)
                held = TRUE;
            if (ItemAbility(e[i].record.ability) || HpItemAbility(e[i].record.ability, &e[i].mon.item)
             || ConsumableItemAbility(e[i].record.ability, e[i].record.held_item, &e[i].mon.item))
                ability = e[i].record.ability;
        }
    }
    if (held && ability != ABILITY_NONE)
    {
        *why = ability;
        return ITEMS_OFF_ABILITY;
    }
    return ITEMS_SIMULATED;
}

static u8 FillEntry(u32 slot, BattleEntry *entry)
{
    struct Pokemon *mon = GcExtension_GetExportMon(slot);
    enum Species species;
    u32 i;
    u8 mask = 0;

    GcExtension_BuildRecord(slot, &entry->record);
    if (sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS)
        entry->record.status = ExportCondition(entry->record.status);
    species = SanitizeSpeciesId(GetMonData(mon, MON_DATA_SPECIES));
    entry->mon.types[0] = gSpeciesInfo[species].types[0];
    entry->mon.types[1] = gSpeciesInfo[species].types[1];
    entry->mon.type_count = entry->mon.types[0] == entry->mon.types[1] ? 1 : 2;
    if (sBattle->roster >= BATTLE_APP_VERSION_ABILITIES)
        entry->mon.ability = ResolveAbility(entry->record.ability);
    if (ItemsEnabled())
        entry->mon.item = ResolveItem(entry->record.held_item, species);
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        ResolveMove(entry->record.moves[i], &entry->mon.moves[i]);
        if (entry->mon.moves[i].unsupported == 0 && entry->record.pp[i] > 0)
            mask |= 1u << i;
    }
    return mask;
}

// Milestone 4-A2 ability lab (debug menu > Utilities > GC Ability Lab):
// reproducible V12 fixtures. Each one fixes the species and ability slot,
// level, moves, HP and status of every Pokemon, and the GC seeds its RNG
// with the battle id (RULE_FIXED_SEED), so a run with the same inputs
// gives the same battle. gba-gc-extension docs/abilities.md lists what each
// fixture shows and the inputs to use.
struct LabMon
{
    u16 species;
    u8 abilityNum, level, hpPercent;
    u32 status;
    u16 moves[MAX_MON_MOVES];
    u8 custom;  // sLabAbilities index: replaces the resolved ability (V13 PoC)
    u16 item;   // V17: the held item (ITEM_NONE before V17 fixtures)
};

struct LabFixture
{
    u8 format, count;
    bool8 strict;
    struct LabMon mons[2][3];
    u8 version;  // 0 = V12
    u8 seedOf;   // 0 = own; else use that fixture's battle id (same GC seed)
    u8 startWeather, startTurns;  // V15: the battle-start weather (BWEATHER_*, 0 none; turns 0 = no end)
    bool8 gen5Weather;            // V15: send the Gen 5 weather rules instead of this build's
    u8 startTerrain, startTerrainTurns;  // V16: the battle-start terrain (BTERRAIN_*, 0 none)
    bool8 gen7Terrain;            // V16: send the Gen 7 terrain boost (x1.5) instead of this build's
    bool8 strictItems;            // V17: refuse to start rather than normalise items
    bool8 gen3Items;              // V17: send the Gen 3 type item boost (x1.1) instead of this build's
};

// ---------------------------------------------------------------------------
// Ability Lab only (debug fixtures): abilities defined here and nowhere in
// the game data, as modifier records the GC runs without code of its own.
// Only BuildLabEntries reads this table (a LabMon with `custom`); normal
// battles take their records from the game data section above. The holder
// keeps its game ability id.
enum { LAB_CUSTOM_NONE, LAB_CUSTOM_LOW_HP_STATUS, LAB_CUSTOM_LOW_HP };
static const BattleModifier sLabAbilities[] = {
    // Physical power x2 while at half HP or less AND with a major status.
    [LAB_CUSTOM_LOW_HP_STATUS] = MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_POWER, BCAT_PHYSICAL), 200,
                                          BPRED_HP_AT_MOST, 2, BPRED_STATUS, 0xfe),
    // The same ability redefined: x1.5 at half HP or less, status not needed.
    [LAB_CUSTOM_LOW_HP] = MODIFIER(BATTLE_MODIFIER_STAGE(BMOD_POWER, BCAT_PHYSICAL), 150,
                                   BPRED_HP_AT_MOST, 2, 0, 0),
};

#define LM(species, num, level, hp, status, ...) {SPECIES_##species, num, level, hp, status, {__VA_ARGS__}, 0}
#define LMC(custom, species, num, level, hp, status, ...) \
    {SPECIES_##species, num, level, hp, status, {__VA_ARGS__}, LAB_CUSTOM_##custom}
#define LMI(item, species, num, level, hp, status, ...) \
    {SPECIES_##species, num, level, hp, status, {__VA_ARGS__}, 0, ITEM_##item}
static const struct LabFixture sLabFixtures[] = {
    [1] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Sturdy: survives at 1 HP from full, then faints
        {LM(BLASTOISE, 0, 50, 100, 0, MOVE_WATER_GUN)},
        {LM(GEODUDE, 1, 10, 100, 0, MOVE_TACKLE)}}},
    [2] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Sturdy on the first hit of Double Kick, the second knocks out
        {LM(HITMONLEE, 0, 50, 100, 0, MOVE_DOUBLE_KICK)},
        {LM(GEODUDE, 1, 10, 100, 0, MOVE_TACKLE)}}},
    [3] = {BATTLE_FORMAT_DOUBLE, 3, FALSE, {  // Rough Skin per contact hit; knocks out an attacker; none for Water Gun
        {LM(HITMONLEE, 0, 30, 100, 0, MOVE_DOUBLE_KICK), LM(MACHOP, 0, 30, 5, 0, MOVE_TACKLE),
         LM(BLASTOISE, 0, 30, 100, 0, MOVE_WATER_GUN)},
        {LM(SHARPEDO, 0, 30, 100, 0, MOVE_LEER), LM(SHARPEDO, 0, 30, 100, 0, MOVE_LEER),
         LM(SHARPEDO, 0, 30, 100, 0, MOVE_LEER)}}},
    [4] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Static per contact hit; an Electric attacker is immune
        {LM(MACHOP, 1, 30, 100, 0, MOVE_DOUBLE_KICK), LM(RAICHU, 0, 30, 100, 0, MOVE_TACKLE)},
        {LM(PIKACHU, 0, 35, 100, 0, MOVE_GROWL), LM(PIKACHU, 0, 35, 100, 0, MOVE_GROWL)}}},
    [5] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Flame Body; a Fire attacker is immune
        {LM(HITMONLEE, 0, 30, 100, 0, MOVE_DOUBLE_KICK), LM(PONYTA, 0, 30, 100, 0, MOVE_TACKLE)},
        {LM(MAGMAR, 0, 35, 100, 0, MOVE_GROWL), LM(MAGMAR, 0, 35, 100, 0, MOVE_GROWL)}}},
    [6] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Poison Point; a Poison attacker is immune
        {LM(HITMONLEE, 0, 30, 100, 0, MOVE_DOUBLE_KICK), LM(NIDORAN_M, 0, 30, 100, 0, MOVE_TACKLE)},
        {LM(NIDORAN_F, 0, 35, 100, 0, MOVE_GROWL), LM(NIDORAN_F, 0, 35, 100, 0, MOVE_GROWL)}}},
    [7] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Volt Absorb heals; Water Absorb at full HP; Surf hits everyone else
        {LM(PIKACHU, 0, 30, 100, 0, MOVE_THUNDERBOLT), LM(BLASTOISE, 0, 30, 100, 0, MOVE_SURF)},
        {LM(LANTURN, 0, 30, 50, 0, MOVE_GROWL), LM(LANTURN, 2, 30, 100, 0, MOVE_GROWL)}}},
    [8] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Limber: cured on entry, blocks Thunder Wave, Body Slam's chance
        {LM(PIKACHU, 0, 30, 100, 0, MOVE_THUNDER_WAVE, MOVE_BODY_SLAM)},
        {LM(HITMONLEE, 0, 30, 100, STATUS1_PARALYSIS, MOVE_GROWL)}}},
    [9] = {BATTLE_FORMAT_TRIPLE, 3, FALSE, {  // Insomnia, Immunity, Water Veil; then Hypnosis on Immunity works
        {LM(PIKACHU, 0, 40, 100, 0, MOVE_HYPNOSIS), LM(NIDORAN_M, 0, 40, 100, 0, MOVE_TOXIC, MOVE_HYPNOSIS),
         LM(PONYTA, 0, 40, 100, 0, MOVE_WILL_O_WISP)},
        {LM(HOOTHOOT, 0, 30, 100, 0, MOVE_GROWL), LM(SNORLAX, 0, 30, 100, 0, MOVE_GROWL),
         LM(GOLDEEN, 1, 30, 100, 0, MOVE_GROWL)}}},
    [10] = {BATTLE_FORMAT_TRIPLE, 3, FALSE, {  // Intimidate from the left: adjacent foes only (Inner Focus blocks)
        {LM(GYARADOS, 0, 30, 100, 0, MOVE_GROWL), LM(MACHOP, 1, 30, 100, 0, MOVE_GROWL),
         LM(MACHOP, 1, 30, 100, 0, MOVE_GROWL)},
        {LM(GOLBAT, 0, 30, 100, 0, MOVE_GROWL), LM(MACHOP, 0, 30, 100, 0, MOVE_GROWL),
         LM(METAGROSS, 0, 30, 100, 0, MOVE_GROWL)}}},
    [11] = {BATTLE_FORMAT_TRIPLE, 3, FALSE, {  // Own Tempo, Oblivious, Scrappy (and a Normal move without it)
        {LM(KANGASKHAN, 1, 30, 100, 0, MOVE_TACKLE), LM(GYARADOS, 0, 30, 100, 0, MOVE_CONFUSE_RAY),
         LM(PIKACHU, 0, 30, 100, 0, MOVE_TACKLE)},
        {LM(SPINDA, 0, 30, 100, 0, MOVE_GROWL), LM(GASTLY, 0, 30, 100, 0, MOVE_GROWL),
         LM(SLOWPOKE, 0, 30, 100, 0, MOVE_GROWL)}}},
    [12] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Serene Grace doubles Body Slam's chance; Shield Dust stops it
        {LM(TOGEKISS, 1, 30, 100, 0, MOVE_BODY_SLAM), LM(SNORLAX, 0, 30, 100, 0, MOVE_BODY_SLAM)},
        {LM(DUSTOX, 0, 40, 100, 0, MOVE_GROWL), LM(MACHOP, 1, 40, 100, 0, MOVE_GROWL)}}},
    [13] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Rock Head: no Double-Edge recoil (the partner takes it)
        {LM(GEODUDE, 0, 30, 100, 0, MOVE_DOUBLE_EDGE), LM(MACHOP, 0, 30, 100, 0, MOVE_DOUBLE_EDGE)},
        {LM(SNORLAX, 0, 30, 100, 0, MOVE_GROWL), LM(SNORLAX, 0, 30, 100, 0, MOVE_GROWL)}}},
    [14] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Infiltrator through a substitute; No Guard reaches a flying foe
        {LM(GOLBAT, 2, 30, 100, 0, MOVE_BITE), LM(MACHOP, 1, 30, 100, 0, MOVE_ZAP_CANNON)},
        {LM(CROBAT, 0, 40, 100, 0, MOVE_SUBSTITUTE), LM(PIDGEOT, 0, 40, 100, 0, MOVE_FLY)}}},
    [15] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Guts (burned), Marvel Scale (paralysed), Adaptability
        {LM(MACHOP, 0, 30, 100, STATUS1_BURN, MOVE_TACKLE), LM(PORYGON_Z, 0, 30, 100, 0, MOVE_TACKLE)},
        {LM(MILOTIC, 0, 30, 100, STATUS1_PARALYSIS, MOVE_GROWL), LM(SNORLAX, 0, 30, 100, 0, MOVE_GROWL)}}},
    [16] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Flash Fire absorbs Ember and boosts its own Fire moves
        {LM(VULPIX, 0, 30, 100, 0, MOVE_EMBER)},
        {LM(MAGMAR, 0, 30, 100, 0, MOVE_EMBER)}}},
    [17] = {BATTLE_FORMAT_DOUBLE, 3, FALSE, {  // Moxie per knock-out, not after the last one
        {LM(GYARADOS, 2, 60, 100, 0, MOVE_TACKLE), LM(MACHOP, 1, 30, 100, 0, MOVE_GROWL),
         LM(MACHOP, 1, 30, 100, 0, MOVE_GROWL)},
        {LM(MACHOP, 1, 5, 100, 0, MOVE_GROWL), LM(MACHOP, 1, 5, 100, 0, MOVE_GROWL),
         LM(MACHOP, 1, 5, 100, 0, MOVE_GROWL)}}},
    [18] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Shed Skin: 1/3 at the end of each turn, before the burn damage
        {LM(DRATINI, 0, 30, 100, STATUS1_BURN, MOVE_GROWL)},
        {LM(SNORLAX, 0, 30, 100, 0, MOVE_GROWL)}}},
    [19] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // Synchronize: paralysis back to Machop; Pikachu is immune
        {LM(ABRA, 0, 30, 100, 0, MOVE_GROWL), LM(RALTS, 0, 30, 100, 0, MOVE_GROWL)},
        {LM(MACHOP, 1, 30, 100, 0, MOVE_THUNDER_WAVE), LM(PIKACHU, 0, 30, 100, 0, MOVE_THUNDER_WAVE)}}},
    [20] = {BATTLE_FORMAT_DOUBLE, 2, TRUE, {  // strict rules: Early Bird is not simulated, the GC refuses
        {LM(MACHOP, 1, 30, 100, 0, MOVE_TACKLE), LM(MACHOP, 1, 30, 100, 0, MOVE_TACKLE)},
        {LM(KANGASKHAN, 0, 30, 100, 0, MOVE_GROWL), LM(MACHOP, 1, 30, 100, 0, MOVE_GROWL)}}},
    // V13 PoC (docs/rule-poc.md): move and ability modifier records.
    [21] = {BATTLE_FORMAT_SINGLE, 2, FALSE, {  // Hex x2 against a poisoned target, x1 against the next one
        {LM(MISDREAVUS, 0, 30, 100, 0, MOVE_HEX), LM(MISDREAVUS, 0, 30, 100, 0, MOVE_HEX)},
        {LM(MACHOP, 1, 36, 100, STATUS1_POISON, MOVE_LEER), LM(MACHOP, 1, 36, 100, 0, MOVE_LEER)}},
        BATTLE_APP_VERSION_MODIFIERS, 0},
    [22] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // custom ability: HP <= 1/2 AND status
        {LMC(LOW_HP_STATUS, RATTATA, 0, 30, 60, STATUS1_POISON, MOVE_TACKLE),
         LMC(LOW_HP_STATUS, RATTATA, 0, 30, 40, 0, MOVE_TACKLE)},
        {LM(SNORLAX, 0, 30, 100, 0, MOVE_RECOVER), LM(SNORLAX, 0, 30, 100, 0, MOVE_RECOVER)}},
        BATTLE_APP_VERSION_MODIFIERS, 0},
    [23] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // L22 with the ability redefined (HP only, x1.5), same seed
        {LMC(LOW_HP, RATTATA, 0, 30, 60, STATUS1_POISON, MOVE_TACKLE),
         LMC(LOW_HP, RATTATA, 0, 30, 40, 0, MOVE_TACKLE)},
        {LM(SNORLAX, 0, 30, 100, 0, MOVE_RECOVER), LM(SNORLAX, 0, 30, 100, 0, MOVE_RECOVER)}},
        BATTLE_APP_VERSION_MODIFIERS, 22},
    [24] = {BATTLE_FORMAT_TRIPLE, 3, FALSE, {  // Triple: Hex and the custom ability together
        {LM(MISDREAVUS, 0, 30, 100, 0, MOVE_HEX), LMC(LOW_HP_STATUS, RATTATA, 0, 30, 45, STATUS1_POISON, MOVE_TACKLE),
         LM(MISDREAVUS, 0, 30, 100, 0, MOVE_HEX)},
        {LM(MACHOP, 1, 30, 100, STATUS1_BURN, MOVE_LEER), LM(SNORLAX, 0, 30, 100, 0, MOVE_RECOVER),
         LM(MACHOP, 1, 30, 100, 0, MOVE_LEER)}}, BATTLE_APP_VERSION_MODIFIERS, 0},
    [25] = {BATTLE_FORMAT_ROTATION, 3, FALSE, {  // Rotation: the front uses Hex / the custom ability
        {LM(MISDREAVUS, 0, 30, 100, 0, MOVE_HEX), LMC(LOW_HP_STATUS, RATTATA, 0, 30, 45, STATUS1_POISON, MOVE_TACKLE),
         LM(MISDREAVUS, 0, 30, 100, 0, MOVE_HEX)},
        {LM(MACHOP, 1, 30, 100, STATUS1_POISON, MOVE_LEER), LM(MACHOP, 1, 30, 100, 0, MOVE_LEER),
         LM(MACHOP, 1, 30, 100, 0, MOVE_LEER)}}, BATTLE_APP_VERSION_MODIFIERS, 0},
    // V14 (docs/modifiers.md): fixed-point arithmetic and the game-data records.
    [26] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Guts (poisoned) and Thick Fat: one attack chain, x0.75
        {LM(MACHAMP, 0, 36, 100, STATUS1_POISON, MOVE_ICE_PUNCH)},
        {LM(SNORLAX, 1, 36, 100, 0, MOVE_RECOVER)}}, BATTLE_APP_VERSION_FIXED_POINT, 0},
    [27] = {BATTLE_FORMAT_DOUBLE, 2, FALSE, {  // spread Rock Slide; Brine at half HP with Flare Boost
        {LM(GOLEM, 0, 34, 100, 0, MOVE_ROCK_SLIDE), LM(DRIFBLIM, 2, 34, 100, STATUS1_BURN, MOVE_BRINE)},
        {LM(SNORLAX, 0, 34, 100, 0, MOVE_RECOVER), LM(SNORLAX, 0, 34, 45, 0, MOVE_RECOVER)}},
        BATTLE_APP_VERSION_FIXED_POINT, 0},
    [28] = {BATTLE_FORMAT_TRIPLE, 3, FALSE, {  // Defeatist and Thick Fat (x0.25), Toxic Boost, Technician
        {LM(ARCHEOPS, 0, 34, 45, 0, MOVE_FIRE_PUNCH), LM(ZANGOOSE, 2, 34, 100, STATUS1_POISON, MOVE_ICE_PUNCH),
         LM(SCIZOR, 1, 34, 100, 0, MOVE_DOUBLE_KICK)},
        {LM(SNORLAX, 1, 34, 100, 0, MOVE_RECOVER), LM(SNORLAX, 0, 34, 100, 0, MOVE_RECOVER),
         LM(SNORLAX, 0, 34, 100, 0, MOVE_RECOVER)}}, BATTLE_APP_VERSION_FIXED_POINT, 0},
    [29] = {BATTLE_FORMAT_ROTATION, 3, FALSE, {  // Earthquake on Dig (final x2), Guts and Thick Fat
        {LM(GOLEM, 0, 34, 100, 0, MOVE_EARTHQUAKE), LM(MACHAMP, 0, 34, 100, STATUS1_POISON, MOVE_ICE_PUNCH),
         LM(ZANGOOSE, 2, 34, 100, STATUS1_POISON, MOVE_ICE_PUNCH)},
        {LM(DUGTRIO, 0, 34, 100, 0, MOVE_DIG), LM(SNORLAX, 1, 34, 100, 0, MOVE_RECOVER),
         LM(SNORLAX, 0, 34, 100, 0, MOVE_RECOVER)}}, BATTLE_APP_VERSION_FIXED_POINT, 0},
    // V15 (docs/weather.md): weather, strict abilities and moves.
    [30] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // Drizzle (slower) over Drought; Swift Swim; Hurricane, Solar Beam
        {LM(PELIPPER, 1, 40, 100, 0, MOVE_SURF, MOVE_HURRICANE), LM(KINGDRA, 0, 40, 100, 0, MOVE_SURF, MOVE_RAIN_DANCE)},
        {LM(NINETALES, 2, 40, 100, 0, MOVE_FLAMETHROWER, MOVE_SOLAR_BEAM, MOVE_SUNNY_DAY),
         LM(VENUSAUR, 2, 40, 100, 0, MOVE_SOLAR_BEAM, MOVE_SYNTHESIS)}},
        BATTLE_APP_VERSION_WEATHER, 0},
    [31] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // Sand Stream, Sand Force, Sand Veil; Cloud Nine; Weather Ball
        {LM(TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE, MOVE_WEATHER_BALL), LM(EXCADRILL, 1, 40, 100, 0, MOVE_ROCK_SLIDE),
         LM(GARCHOMP, 0, 40, 100, 0, MOVE_EARTHQUAKE)},
        {LM(GOLDUCK, 1, 40, 100, 0, MOVE_SURF, MOVE_RAIN_DANCE), LM(PELIPPER, 1, 40, 100, 0, MOVE_WEATHER_BALL),
         LM(LUDICOLO, 1, 40, 100, 0, MOVE_SURF)}},
        BATTLE_APP_VERSION_WEATHER, 0},
    [32] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // Snow Warning, Ice Body, Snow Cloak, Blizzard; Hail; Overcoat
        {LM(ABOMASNOW, 0, 40, 100, 0, MOVE_BLIZZARD), LM(GLALIE, 1, 40, 60, 0, MOVE_ICE_BEAM, MOVE_HAIL),
         LM(MAMOSWINE, 1, 40, 100, 0, MOVE_BODY_SLAM)},
        {LM(CLOYSTER, 2, 40, 100, 0, MOVE_ICE_BEAM), LM(CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER, MOVE_SUNNY_DAY),
         LM(SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_WEATHER, 0},
    [33] = {BATTLE_FORMAT_ROTATION, 3, TRUE, {  // sun: Chlorophyll, Solar Power, Dry Skin, Moonlight, no-charge Solar Beam
        {LM(NINETALES, 2, 40, 100, 0, MOVE_WEATHER_BALL, MOVE_MOONLIGHT), LM(VENUSAUR, 2, 40, 100, 0, MOVE_SOLAR_BEAM),
         LM(CHARIZARD, 2, 40, 100, 0, MOVE_FLAMETHROWER)},
        {LM(TOXICROAK, 1, 40, 100, 0, MOVE_POISON_JAB), LM(LUDICOLO, 1, 40, 100, 0, MOVE_RAIN_DANCE, MOVE_SURF),
         LM(TORKOAL, 1, 40, 100, 0, MOVE_FLAMETHROWER)}},
        BATTLE_APP_VERSION_WEATHER, 0},
    [34] = {BATTLE_FORMAT_SINGLE, 1, TRUE, {  // strict rules: primal weather (Desolate Land) is not simulated
        {LM(PELIPPER, 1, 40, 100, 0, MOVE_SURF)},
        {LM(GROUDON_PRIMAL, 0, 40, 100, 0, MOVE_EARTHQUAKE)}},
        BATTLE_APP_VERSION_WEATHER, 0},
    [35] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // L32 with the Gen 5 weather rules: Snow Warning makes hail, no end
        {LM(ABOMASNOW, 0, 40, 100, 0, MOVE_BLIZZARD), LM(GLALIE, 1, 40, 60, 0, MOVE_ICE_BEAM, MOVE_HAIL),
         LM(MAMOSWINE, 1, 40, 100, 0, MOVE_BODY_SLAM)},
        {LM(CLOYSTER, 2, 40, 100, 0, MOVE_ICE_BEAM), LM(CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER, MOVE_SUNNY_DAY),
         LM(SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_WEATHER, 32, BWEATHER_NONE, 0, TRUE},
    [36] = {BATTLE_FORMAT_DOUBLE, 2, TRUE, {  // battle-start rain (no end) from the rules; Swift Swim; Thunder
        {LM(KINGDRA, 0, 40, 100, 0, MOVE_SURF, MOVE_THUNDER), LM(LUDICOLO, 0, 40, 100, 0, MOVE_SURF)},
        {LM(TORKOAL, 1, 40, 100, 0, MOVE_FLAMETHROWER), LM(SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_WEATHER, 0, BWEATHER_RAIN, 0},
    // V16 (docs/terrain.md): terrain, strict abilities and moves.
    [37] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // Electric: no sleep for grounded Koko; boost; Psychic Terrain replaces it
        {LM(TAPU_KOKO, 0, 40, 100, 0, MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK),
         LM(RAICHU_ALOLA, 0, 40, 100, 0, MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK)},
        {LM(BRONZONG, 0, 40, 100, 0, MOVE_HYPNOSIS, MOVE_PSYCHIC), LM(TAPU_LELE, 0, 40, 100, 0, MOVE_PSYCHIC, MOVE_HYPNOSIS)}},
        BATTLE_APP_VERSION_TERRAIN, 0},
    [38] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // Grassy: heal, Earthquake / Bulldoze x0.5, Grass Pelt, Levitate / Flying
        {LM(RILLABOOM, 2, 40, 60, 0, MOVE_EARTHQUAKE, MOVE_ENERGY_BALL), LM(GOGOAT, 2, 40, 70, 0, MOVE_BULLDOZE),
         LM(TAPU_BULU, 0, 40, 100, 0, MOVE_ENERGY_BALL)},
        {LM(FLYGON, 0, 40, 80, 0, MOVE_DRAGON_PULSE), LM(PIDGEOT, 0, 40, 80, 0, MOVE_QUICK_ATTACK),
         LM(SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_TERRAIN, 0},
    [39] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // Misty: no status or confusion when grounded; Dragon x0.5 on grounded
        {LM(TAPU_FINI, 0, 40, 100, 0, MOVE_SURF, MOVE_CONFUSE_RAY), LM(DRAGONITE, 0, 40, 100, 0, MOVE_DRAGON_CLAW),
         LM(GARCHOMP, 0, 40, 100, 0, MOVE_DRAGON_CLAW)},
        {LM(JOLTEON, 0, 40, 100, 0, MOVE_THUNDER_WAVE), LM(CROBAT, 0, 40, 100, 0, MOVE_TOXIC, MOVE_CONFUSE_RAY),
         LM(DRAGONITE, 0, 40, 100, 0, MOVE_DRAGON_CLAW)}},
        BATTLE_APP_VERSION_TERRAIN, 0},
    [40] = {BATTLE_FORMAT_ROTATION, 3, TRUE, {  // Psychic: foes' priority moves fail on the grounded front
        {LM(TAPU_LELE, 0, 40, 100, 0, MOVE_PSYCHIC), LM(INDEEDEE_M, 0, 40, 100, 0, MOVE_PSYCHIC),
         LM(PIDGEOT, 0, 40, 100, 0, MOVE_QUICK_ATTACK)},
        {LM(RATICATE, 1, 40, 100, 0, MOVE_QUICK_ATTACK), LM(AZUMARILL, 0, 40, 100, 0, MOVE_AQUA_JET),
         LM(PIDGEOT, 2, 40, 100, 0, MOVE_QUICK_ATTACK)}},
        BATTLE_APP_VERSION_TERRAIN, 0},
    [41] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // L38 with the Gen 7 terrain boost (x1.5), same seed
        {LM(RILLABOOM, 2, 40, 60, 0, MOVE_EARTHQUAKE, MOVE_ENERGY_BALL), LM(GOGOAT, 2, 40, 70, 0, MOVE_BULLDOZE),
         LM(TAPU_BULU, 0, 40, 100, 0, MOVE_ENERGY_BALL)},
        {LM(FLYGON, 0, 40, 80, 0, MOVE_DRAGON_PULSE), LM(PIDGEOT, 0, 40, 80, 0, MOVE_QUICK_ATTACK),
         LM(SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_TERRAIN, 38, BWEATHER_NONE, 0, FALSE, BTERRAIN_NONE, 0, TRUE},
    [42] = {BATTLE_FORMAT_SINGLE, 1, TRUE, {  // strict rules: Rising Voltage (terrain-dependent power) is not simulated
        {LM(TAPU_KOKO, 0, 40, 100, 0, MOVE_RISING_VOLTAGE)},
        {LM(SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_TERRAIN, 0},
    // V17 (docs/items.md): held items, strict abilities and moves.
    [43] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // Muscle Band (truncated x1.1), Charcoal, Drizzle with Damp Rock (8)
        {LMI(MUSCLE_BAND, MACHAMP, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(DAMP_ROCK, PELIPPER, 1, 40, 100, 0, MOVE_SURF, MOVE_RAIN_DANCE)},
        {LMI(CHARCOAL, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER), LMI(AMULET_COIN, SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 0},
    [44] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // Terrain Extender on Grassy Surge, Miracle Seed, Wise Glasses, type items
        {LMI(TERRAIN_EXTENDER, RILLABOOM, 2, 40, 100, 0, MOVE_ENERGY_BALL, MOVE_EARTHQUAKE),
         LMI(MIRACLE_SEED, TAPU_BULU, 0, 40, 100, 0, MOVE_ENERGY_BALL), LMI(WISE_GLASSES, GOGOAT, 2, 40, 100, 0, MOVE_ENERGY_BALL)},
        {LMI(MAGNET, TAPU_KOKO, 0, 40, 100, 0, MOVE_THUNDERBOLT), LMI(DRAGON_FANG, FLYGON, 0, 40, 100, 0, MOVE_DRAGON_PULSE),
         LMI(SILK_SCARF, SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 0},
    [45] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // Drought with Heat Rock, Mystic Water, Sea Incense, Damp Rock Rain Dance
        {LMI(HEAT_ROCK, NINETALES, 2, 40, 100, 0, MOVE_FLAMETHROWER, MOVE_SUNNY_DAY),
         LMI(MYSTIC_WATER, KINGDRA, 0, 40, 100, 0, MOVE_SURF), LMI(WISE_GLASSES, VENUSAUR, 2, 40, 100, 0, MOVE_SOLAR_BEAM)},
        {LMI(SEA_INCENSE, GOLDUCK, 1, 40, 100, 0, MOVE_SURF), LMI(DAMP_ROCK, PELIPPER, 0, 40, 100, 0, MOVE_HURRICANE, MOVE_RAIN_DANCE),
         LMI(SILK_SCARF, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 0},
    [46] = {BATTLE_FORMAT_ROTATION, 3, TRUE, {  // Sand Stream with Smooth Rock, Snow Warning with Icy Rock, type items
        {LMI(SMOOTH_ROCK, TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE), LMI(SOFT_SAND, GARCHOMP, 0, 40, 100, 0, MOVE_EARTHQUAKE),
         LMI(MUSCLE_BAND, EXCADRILL, 1, 40, 100, 0, MOVE_ROCK_SLIDE)},
        {LMI(ICY_ROCK, ABOMASNOW, 0, 40, 100, 0, MOVE_BLIZZARD), LMI(NEVER_MELT_ICE, GLALIE, 1, 40, 100, 0, MOVE_ICE_BEAM),
         LMI(HARD_STONE, MAMOSWINE, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 0},
    [47] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L43 with Leftovers (not simulated): every item off, same seed
        {LMI(MUSCLE_BAND, MACHAMP, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(DAMP_ROCK, PELIPPER, 1, 40, 100, 0, MOVE_SURF, MOVE_RAIN_DANCE)},
        {LMI(CHARCOAL, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER), LMI(LEFTOVERS, SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 43},
    [48] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L47 under strict item rules: refused on the GBA
        {LMI(MUSCLE_BAND, MACHAMP, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(DAMP_ROCK, PELIPPER, 1, 40, 100, 0, MOVE_SURF, MOVE_RAIN_DANCE)},
        {LMI(CHARCOAL, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER), LMI(LEFTOVERS, SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 43, BWEATHER_NONE, 0, FALSE, BTERRAIN_NONE, 0, FALSE, TRUE},
    [49] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L43 with the Gen 3 type item boost (x1.1), same seed
        {LMI(MUSCLE_BAND, MACHAMP, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(DAMP_ROCK, PELIPPER, 1, 40, 100, 0, MOVE_SURF, MOVE_RAIN_DANCE)},
        {LMI(CHARCOAL, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER), LMI(AMULET_COIN, SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_ITEMS, 43, BWEATHER_NONE, 0, FALSE, BTERRAIN_NONE, 0, FALSE, FALSE, TRUE},
    [50] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Klutz (not simulated) holding Muscle Band: every item off
        {LMI(MUSCLE_BAND, LOPUNNY, 1, 40, 100, 0, MOVE_BODY_SLAM)},
        {LMI(CHARCOAL, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER)}},
        BATTLE_APP_VERSION_ITEMS, 0},
    // V18 (docs/items.md): end-of-turn Leftovers and Black Sludge.
    [51] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // sandstorm then Leftovers; Black Sludge heals Poison, hurts others
        {LMI(LEFTOVERS, SNORLAX, 0, 40, 70, 0, MOVE_BODY_SLAM), LMI(BLACK_SLUDGE, TENTACRUEL, 0, 40, 50, 0, MOVE_SLUDGE_BOMB, MOVE_SURF)},
        {LMI(BLACK_SLUDGE, TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE), LMI(BLACK_SLUDGE, TOXICROAK, 1, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [52] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // Grassy Surge: terrain heal then the item, Pokemon by Pokemon; a Black Sludge faint
        {LMI(LEFTOVERS, RILLABOOM, 2, 40, 60, 0, MOVE_SWORDS_DANCE), LMI(BLACK_SLUDGE, AERODACTYL, 0, 40, 10, 0, MOVE_SWORDS_DANCE),
         LMI(BLACK_SLUDGE, VENUSAUR, 0, 40, 50, 0, MOVE_SWORDS_DANCE)},
        {LMI(LEFTOVERS, TOXICROAK, 1, 40, 70, STATUS1_BURN, MOVE_BULK_UP),
         LMI(BLACK_SLUDGE, NIDOKING, 0, 40, 80, 0, MOVE_CALM_MIND), LMI(LEFTOVERS, CROBAT, 0, 40, 60, 0, MOVE_BULK_UP)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [53] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // six holders in speed order, with sandstorm, poison and burn
        {LMI(LEFTOVERS, SNORLAX, 1, 40, 50, STATUS1_POISON, MOVE_BODY_SLAM), LMI(LEFTOVERS, PELIPPER, 0, 40, 60, STATUS1_BURN, MOVE_SURF),
         LMI(BLACK_SLUDGE, NIDOKING, 0, 40, 70, 0, MOVE_SLUDGE_BOMB)},
        {LMI(LEFTOVERS, TYRANITAR, 0, 40, 80, 0, MOVE_ROCK_SLIDE), LMI(BLACK_SLUDGE, CROBAT, 0, 40, 50, 0, MOVE_WING_ATTACK),
         LMI(BLACK_SLUDGE, LUDICOLO, 0, 40, 100, 0, MOVE_SURF)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [54] = {BATTLE_FORMAT_ROTATION, 3, TRUE, {  // only the front Pokemon; Shed Skin, Rain Dish (Drizzle) and Leftovers
        {LMI(AMULET_COIN, SNORLAX, 1, 40, 100, 0, MOVE_BODY_SLAM), LMI(BLACK_SLUDGE, TENTACRUEL, 0, 40, 60, 0, MOVE_SURF),
         LMI(LEFTOVERS, DRAGONAIR, 0, 40, 60, STATUS1_POISON, MOVE_BODY_SLAM)},
        {LMI(LEFTOVERS, LUDICOLO, 1, 40, 70, 0, MOVE_SURF), LMI(BLACK_SLUDGE, PELIPPER, 1, 40, 100, 0, MOVE_SURF),
         LMI(BLACK_SLUDGE, TOXICROAK, 1, 40, 50, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [55] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L51 with Life Orb (not simulated): every item off, same seed
        {LMI(LEFTOVERS, SNORLAX, 0, 40, 70, 0, MOVE_BODY_SLAM), LMI(BLACK_SLUDGE, TENTACRUEL, 0, 40, 50, 0, MOVE_SLUDGE_BOMB, MOVE_SURF)},
        {LMI(LIFE_ORB, TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE), LMI(BLACK_SLUDGE, TOXICROAK, 1, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 51},
    [56] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L55 under strict item rules: refused on the GBA
        {LMI(LEFTOVERS, SNORLAX, 0, 40, 70, 0, MOVE_BODY_SLAM), LMI(BLACK_SLUDGE, TENTACRUEL, 0, 40, 50, 0, MOVE_SLUDGE_BOMB, MOVE_SURF)},
        {LMI(LIFE_ORB, TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE), LMI(BLACK_SLUDGE, TOXICROAK, 1, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 51, BWEATHER_NONE, 0, FALSE, BTERRAIN_NONE, 0, FALSE, TRUE},
    [57] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Magic Guard (not simulated) holding Black Sludge: every item off
        {LMI(BLACK_SLUDGE, CLEFABLE, 1, 40, 100, 0, MOVE_BODY_SLAM)},
        {LMI(LEFTOVERS, SNORLAX, 1, 40, 70, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    // M4-B1a+ (docs/items.md 11): audited Z-Crystals and Mega Stones held with
    // no effect (no gimmicks); Leftovers and Black Sludge beside them as in L51-L54.
    [58] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // Snorlium Z, Venusaurite; sandstorm, Leftovers, Black Sludge
        {LMI(SNORLIUM_Z, SNORLAX, 0, 40, 70, 0, MOVE_BODY_SLAM), LMI(VENUSAURITE, VENUSAUR, 0, 40, 60, 0, MOVE_SLUDGE_BOMB)},
        {LMI(LEFTOVERS, TYRANITAR, 0, 40, 70, 0, MOVE_ROCK_SLIDE), LMI(BLACK_SLUDGE, TOXICROAK, 1, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [59] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // Mega Stones of both sides and Z-Crystals beside Leftovers / Black Sludge
        {LMI(CHARIZARDITE_Y, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER), LMI(LEFTOVERS, SNORLAX, 0, 40, 60, 0, MOVE_BODY_SLAM),
         LMI(AMPHAROSITE, AMPHAROS, 0, 40, 100, 0, MOVE_THUNDERBOLT)},
        {LMI(GYARADOSITE, GYARADOS, 0, 40, 100, 0, MOVE_SURF), LMI(BLACK_SLUDGE, VENUSAUR, 0, 40, 60, 0, MOVE_SLUDGE_BOMB),
         LMI(STEELIUM_Z, SCIZOR, 1, 40, 100, 0, MOVE_WING_ATTACK)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [60] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // six holders: three Z-Crystals, two Mega Stones, Black Sludge; sandstorm
        {LMI(NORMALIUM_Z, SNORLAX, 0, 40, 60, 0, MOVE_BODY_SLAM), LMI(DARKINIUM_Z, TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE),
         LMI(AMPHAROSITE, AMPHAROS, 0, 40, 100, 0, MOVE_THUNDERBOLT)},
        {LMI(SWAMPERTITE, SWAMPERT, 0, 40, 100, 0, MOVE_SURF), LMI(FLYINIUM_Z, CROBAT, 0, 40, 100, 0, MOVE_WING_ATTACK),
         LMI(BLACK_SLUDGE, NIDOKING, 0, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [61] = {BATTLE_FORMAT_ROTATION, 3, TRUE, {  // Rotation: Firium Z, Venusaurite, Leftovers / Blastoisinite, Waterium Z, Black Sludge
        {LMI(FIRIUM_Z, CHARIZARD, 0, 40, 100, 0, MOVE_FLAMETHROWER), LMI(VENUSAURITE, VENUSAUR, 0, 40, 100, 0, MOVE_SLUDGE_BOMB),
         LMI(LEFTOVERS, SNORLAX, 0, 40, 60, 0, MOVE_BODY_SLAM)},
        {LMI(BLASTOISINITE, BLASTOISE, 0, 40, 100, 0, MOVE_SURF), LMI(WATERIUM_Z, GYARADOS, 0, 40, 100, 0, MOVE_SURF),
         LMI(BLACK_SLUDGE, TENTACRUEL, 0, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    [62] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L58 with Life Orb (not simulated): every item off, same seed
        {LMI(SNORLIUM_Z, SNORLAX, 0, 40, 70, 0, MOVE_BODY_SLAM), LMI(VENUSAURITE, VENUSAUR, 0, 40, 60, 0, MOVE_SLUDGE_BOMB)},
        {LMI(LIFE_ORB, TYRANITAR, 0, 40, 70, 0, MOVE_ROCK_SLIDE), LMI(BLACK_SLUDGE, TOXICROAK, 1, 40, 60, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 58},
    [63] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Ultranecrozium Z under strict item rules: refused on the GBA
        {LMI(ULTRANECROZIUM_Z, NECROZMA_DUSK_MANE, 0, 40, 100, 0, MOVE_IRON_HEAD)},
        {LMI(LEFTOVERS, SNORLAX, 0, 40, 70, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0, BWEATHER_NONE, 0, FALSE, BTERRAIN_NONE, 0, FALSE, TRUE},
    [64] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // the Red Orb (Primal Reversion by itself) beside a Z-Crystal: every item off
        {LMI(RED_ORB, GROUDON, 0, 40, 100, 0, MOVE_EARTHQUAKE)},
        {LMI(SNORLIUM_Z, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_END_TURN_ITEMS, 0},
    // M4-B1b, V19 (docs/items.md 12): consumable items, used up when they act.
    [65] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // Sitrus after a hit, Lum against Thunder Wave, Focus Sash (sand)
        {LMI(LUM_BERRY, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(SITRUS_BERRY, RAICHU, 0, 40, 60, 0, MOVE_THUNDERBOLT)},
        {LMI(SITRUS_BERRY, JOLTEON, 0, 40, 100, 0, MOVE_THUNDER_WAVE), LMI(FOCUS_SASH, TYRANITAR, 0, 8, 100, 0, MOVE_ROCK_SLIDE)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0},
    [66] = {BATTLE_FORMAT_DOUBLE, 3, TRUE, {  // a spread hit, then Focus Sash; Chesto against Hypnosis, Pecha against Toxic
        {LMI(ORAN_BERRY, TYRANITAR, 0, 40, 70, 0, MOVE_ROCK_SLIDE), LMI(CHESTO_BERRY, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM),
         LMI(PECHA_BERRY, MACHAMP, 0, 40, 100, 0, MOVE_CROSS_CHOP)},
        {LMI(SITRUS_BERRY, HYPNO, 2, 40, 60, 0, MOVE_HYPNOSIS), LMI(FOCUS_SASH, ARBOK, 0, 15, 100, 0, MOVE_TOXIC),
         LMI(LUM_BERRY, NIDOKING, 0, 40, 100, 0, MOVE_SLUDGE_BOMB)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0},
    [67] = {BATTLE_FORMAT_TRIPLE, 3, TRUE, {  // status berries against paralysis, burn and confusion
        {LMI(RAWST_BERRY, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(CHERI_BERRY, RAICHU, 0, 40, 100, 0, MOVE_THUNDERBOLT),
         LMI(PERSIM_BERRY, MACHAMP, 0, 40, 100, 0, MOVE_CROSS_CHOP)},
        {LMI(SITRUS_BERRY, JOLTEON, 0, 40, 60, 0, MOVE_THUNDER_WAVE), LMI(ORAN_BERRY, RAPIDASH, 1, 40, 70, 0, MOVE_WILL_O_WISP),
         LMI(LUM_BERRY, HYPNO, 2, 40, 100, 0, MOVE_CONFUSE_RAY)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0},
    [68] = {BATTLE_FORMAT_ROTATION, 3, TRUE, {  // only the front: Sitrus, Lum, Chesto, Persim across rotations
        {LMI(SITRUS_BERRY, SNORLAX, 0, 40, 55, 0, MOVE_BODY_SLAM), LMI(LUM_BERRY, RAICHU, 0, 40, 100, 0, MOVE_THUNDERBOLT),
         LMI(FOCUS_SASH, MACHAMP, 0, 15, 100, 0, MOVE_CROSS_CHOP)},
        {LMI(ORAN_BERRY, TYRANITAR, 0, 40, 60, 0, MOVE_ROCK_SLIDE), LMI(PERSIM_BERRY, JOLTEON, 0, 40, 100, 0, MOVE_THUNDER_WAVE),
         LMI(CHESTO_BERRY, HYPNO, 2, 40, 100, 0, MOVE_HYPNOSIS)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0},
    [69] = {BATTLE_FORMAT_SINGLE, 2, TRUE, {  // L65 with Life Orb (not simulated): every item off, same seed
        {LMI(LUM_BERRY, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM), LMI(SITRUS_BERRY, RAICHU, 0, 40, 60, 0, MOVE_THUNDERBOLT)},
        {LMI(SITRUS_BERRY, JOLTEON, 0, 40, 100, 0, MOVE_THUNDER_WAVE), LMI(LIFE_ORB, TYRANITAR, 0, 8, 100, 0, MOVE_ROCK_SLIDE)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 65},
    [70] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Cud Chew (not simulated) holding Sitrus Berry: every item off
        {LMI(SITRUS_BERRY, FARIGIRAF, 0, 40, 60, 0, MOVE_TACKLE)},
        {LMI(LUM_BERRY, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0},
    [71] = {BATTLE_FORMAT_SINGLE, 1, FALSE, {  // Figy Berry (not simulated) under strict item rules: refused on the GBA
        {LMI(FIGY_BERRY, SNORLAX, 0, 40, 100, 0, MOVE_BODY_SLAM)},
        {LMI(SITRUS_BERRY, TYRANITAR, 0, 40, 100, 0, MOVE_ROCK_SLIDE)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0, BWEATHER_NONE, 0, FALSE, BTERRAIN_NONE, 0, FALSE, TRUE},
    // ACT-002 (M4-B1b repair, docs/battle-reference): a turn lost to recharging
    // ends with the move end's item checks.
    [72] = {BATTLE_FORMAT_SINGLE, 1, TRUE, {  // Solar Power leaves Sitrus due; it acts after the recharge, before the foe
        {LMI(SITRUS_BERRY, CHARIZARD, 2, 40, 60, 0, MOVE_HYPER_BEAM)},
        {LMI(NONE, SNORLAX, 0, 40, 100, 0, MOVE_SUNNY_DAY)}},
        BATTLE_APP_VERSION_CONSUMABLE_ITEMS, 0, BWEATHER_SUN, 0},
};
#undef LM
#undef LMC
#undef LMI
#define LAB_FIXTURE_COUNT (ARRAY_COUNT(sLabFixtures) - 1)
#define LAB_BATTLE_ID(fixture) (0x4C414200u | (fixture))  // "LAB" + fixture: the GC's seed

static bool8 BuildLabEntries(BattleRequest *request, const u8 **reason)
{
    const struct LabFixture *fixture = &sLabFixtures[sBattle->lab];
    u32 side, i, m;
    GcExtension_SetExtraMons(sBattle->opponent, 2 * sBattle->count);
    for (side = 0; side < 2; side++)
    {
        BattleEntry *entries = side ? request->opponent : request->player;
        for (i = 0; i < sBattle->count; i++)
        {
            const struct LabMon *spec = &fixture->mons[side][i];
            const u32 index = side * sBattle->count + i;
            struct Pokemon *mon = &sBattle->opponent[index];
            u8 abilityNum = spec->abilityNum;
            u32 status = spec->status;
            u16 hp;
            CreateMonWithIVs(mon, spec->species, spec->level, 0x4C410000 + index, OTID_STRUCT_PLAYER_ID, 15);
            SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
            for (m = 0; m < MAX_MON_MOVES; m++)
            {
                SetMonMoveSlot(mon, spec->moves[m], m);
                if (spec->moves[m] == MOVE_NONE)
                {
                    u8 pp = 0;
                    SetMonData(mon, MON_DATA_PP1 + m, &pp);
                }
            }
            hp = GetMonData(mon, MON_DATA_MAX_HP) * spec->hpPercent / 100;
            if (hp == 0)
                hp = 1;
            SetMonData(mon, MON_DATA_HP, &hp);
            SetMonData(mon, MON_DATA_STATUS, &status);
            SetMonData(mon, MON_DATA_HELD_ITEM, &spec->item);
            if (FillEntry(GC_EXTRA_SLOT_BASE + index, &entries[i]) == 0)
            {
                *reason = side ? sText_NoOpponentMoves : sText_NoMoves;
                return FALSE;
            }
            if (spec->custom)
            {
                BattleAbility *a = &entries[i].mon.ability;
                *a = (BattleAbility){.cls = BACLASS_HOOKS, .source = entries[i].record.ability};
                AddHook(a, ModifierHook(sLabAbilities[spec->custom]));
            }
        }
    }
    return TRUE;
}

// Test rule: the player's battler is the first party member that can battle
// and knows at least one move the GC battle core supports.
static s32 FindLead(BattleEntry *scratch)
{
    u32 i;
    for (i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG)
         && GetMonData(mon, MON_DATA_HP) != 0 && FillEntry(i, scratch) != 0)
            return i;
    }
    return -1;
}

// Mode-owned fixtures use the same export path as a normal party. They
// never enter gPlayerParty or the save. Slot IDs remain stable as they move.
static void SetConditionFixture(struct Pokemon *mon, u32 side, u32 index)
{
    static const enum Move moves[][3] = {
        {MOVE_SWORDS_DANCE, MOVE_HARDEN, MOVE_NASTY_PLOT},
        {MOVE_GROWL, MOVE_THUNDER_WAVE, MOVE_AMNESIA},
    };
    if (side)
    {
        if (sBattle->format == BATTLE_FORMAT_DOUBLE && index < 2)
        {
            u16 hp = 1;
            u32 status = index ? STATUS1_POISON : STATUS1_BURN;
            SetMonData(mon, MON_DATA_HP, &hp);
            SetMonData(mon, MON_DATA_STATUS, &status);
        }
        else if (sBattle->format == BATTLE_FORMAT_TRIPLE)
        {
            u32 status = index == 2 ? (B_USE_FROSTBITE ? STATUS1_FROSTBITE : STATUS1_FREEZE) : STATUS1_BURN;
            SetMonData(mon, MON_DATA_STATUS, &status);
        }
        return;
    }

    const enum Move setup = moves[sBattle->format == BATTLE_FORMAT_DOUBLE][index];
    const enum Move attack = GetMonData(mon, MON_DATA_MOVE1);
    SetMonMoveSlot(mon, setup, 0);
    SetMonMoveSlot(mon, attack, 1);
    SetMonMoveSlot(mon, sBattle->format == BATTLE_FORMAT_TRIPLE ? MOVE_POISON_GAS : MOVE_TOXIC, 2);
    SetMonMoveSlot(mon, MOVE_REFRESH, 3);
    if (sBattle->format == BATTLE_FORMAT_SINGLE || sBattle->format == BATTLE_FORMAT_TRIPLE
     || sBattle->format == BATTLE_FORMAT_ROTATION)
    {
        static const u32 initial[] = {STATUS1_BURN, STATUS1_POISON, STATUS1_PARALYSIS};
        u32 status = initial[index];
        SetMonData(mon, MON_DATA_STATUS, &status);
    }
}

// Milestone 3 move effect fixtures: each format carries representative
// stat, status, heal, drain, recoil, secondary, priority, Protect,
// multi-hit and switch moves. Temporary Pokemon only; display-only results.
static void SetEffectFixture(struct Pokemon *mon, u32 side, u32 index)
{
    static const enum Move sets[4][2][3][MAX_MON_MOVES] = {
        [0] = { // Single
            {{MOVE_SWORDS_DANCE, MOVE_HYPNOSIS, MOVE_RECOVER, MOVE_QUICK_ATTACK}},
            {{MOVE_SCREECH, MOVE_PROTECT, MOVE_FURY_SWIPES, MOVE_GIGA_DRAIN}},
        },
        [1] = { // Double (third member in reserve)
            {{MOVE_ICY_WIND, MOVE_PROTECT, MOVE_U_TURN, MOVE_AGILITY},
             {MOVE_THUNDERBOLT, MOVE_TOXIC, MOVE_GROWL, MOVE_DRAGON_TAIL},
             {MOVE_GIGA_DRAIN, MOVE_DOUBLE_EDGE, MOVE_WILL_O_WISP, MOVE_RECOVER}},
            {{MOVE_TACKLE, MOVE_PROTECT, MOVE_FLAMETHROWER, MOVE_SCREECH},
             {MOVE_ICE_BEAM, MOVE_FURY_SWIPES, MOVE_GROWL, MOVE_TACKLE},
             {MOVE_TACKLE, MOVE_THUNDER_WAVE, MOVE_NONE, MOVE_NONE}},
        },
        [2] = { // Triple
            {{MOVE_SCREECH, MOVE_WILL_O_WISP, MOVE_HEAT_WAVE, MOVE_PROTECT},
             {MOVE_SURF, MOVE_ICE_BEAM, MOVE_RECOVER, MOVE_QUICK_ATTACK},
             {MOVE_THUNDERBOLT, MOVE_DOUBLE_KICK, MOVE_AGILITY, MOVE_GIGA_DRAIN}},
            {{MOVE_TACKLE, MOVE_GROWL, MOVE_PROTECT, MOVE_NONE},
             {MOVE_TACKLE, MOVE_FLAMETHROWER, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_THUNDER_WAVE, MOVE_NONE, MOVE_NONE}},
        },
        [3] = { // Rotation
            {{MOVE_SWORDS_DANCE, MOVE_QUICK_ATTACK, MOVE_PROTECT, MOVE_DOUBLE_EDGE},
             {MOVE_AGILITY, MOVE_THUNDERBOLT, MOVE_TOXIC, MOVE_RECOVER},
             {MOVE_GIGA_DRAIN, MOVE_FURY_SWIPES, MOVE_HYPNOSIS, MOVE_U_TURN}},
            {{MOVE_TACKLE, MOVE_GROWL, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_EMBER, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_SCREECH, MOVE_NONE, MOVE_NONE}},
        },
    };
    const enum Move *moves = sets[sBattle->format - BATTLE_FORMAT_SINGLE][side][index];
    u32 m;
    for (m = 0; m < MAX_MON_MOVES; m++)
    {
        SetMonMoveSlot(mon, moves[m], m);
        if (moves[m] == MOVE_NONE)
        {
            u8 pp = 0;
            SetMonData(mon, MON_DATA_PP1 + m, &pp);
        }
    }
}

// Milestone 3.5 volatile fixtures: flinch, confusion (Confuse Ray,
// Supersonic, Swagger), Substitute, Solar Beam, Fly, Dig, Hyper Beam,
// Giga Impact and the moves that reach semi-invulnerable targets.
// Temporary Pokemon only; display-only results.
static void SetVolatileFixture(struct Pokemon *mon, u32 side, u32 index)
{
    static const enum Move sets[4][2][3][MAX_MON_MOVES] = {
        [0] = { // Single
            {{MOVE_AIR_SLASH, MOVE_CONFUSE_RAY, MOVE_SUBSTITUTE, MOVE_HYPER_BEAM}},
            {{MOVE_BITE, MOVE_SOLAR_BEAM, MOVE_SUPERSONIC, MOVE_DIG}},
        },
        [1] = { // Double (third member in reserve)
            {{MOVE_FLY, MOVE_ROCK_SLIDE, MOVE_PROTECT, MOVE_SWAGGER},
             {MOVE_EARTHQUAKE, MOVE_SUBSTITUTE, MOVE_GIGA_IMPACT, MOVE_CONFUSE_RAY},
             {MOVE_SOLAR_BEAM, MOVE_U_TURN, MOVE_DOUBLE_KICK, MOVE_RECOVER}},
            {{MOVE_GUST, MOVE_BITE, MOVE_TACKLE, MOVE_NONE},
             {MOVE_SUPERSONIC, MOVE_TACKLE, MOVE_SUBSTITUTE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_DIG, MOVE_NONE, MOVE_NONE}},
        },
        [2] = { // Triple
            {{MOVE_TWISTER, MOVE_SUBSTITUTE, MOVE_HYPER_BEAM, MOVE_PROTECT},
             {MOVE_DIG, MOVE_SURF, MOVE_CONFUSE_RAY, MOVE_EARTHQUAKE},
             {MOVE_SOLAR_BEAM, MOVE_DOUBLE_KICK, MOVE_SWAGGER, MOVE_ROCK_SLIDE}},
            {{MOVE_TACKLE, MOVE_BITE, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_FLY, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_SUPERSONIC, MOVE_NONE, MOVE_NONE}},
        },
        [3] = { // Rotation
            {{MOVE_SOLAR_BEAM, MOVE_SUBSTITUTE, MOVE_AIR_SLASH, MOVE_PROTECT},
             {MOVE_DIG, MOVE_HYPER_BEAM, MOVE_CONFUSE_RAY, MOVE_EARTHQUAKE},
             {MOVE_FLY, MOVE_SWAGGER, MOVE_DOUBLE_KICK, MOVE_RECOVER}},
            {{MOVE_TACKLE, MOVE_BITE, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_GUST, MOVE_NONE, MOVE_NONE},
             {MOVE_TACKLE, MOVE_SUPERSONIC, MOVE_NONE, MOVE_NONE}},
        },
    };
    // Endurance (debug): the player only uses non-damaging volatile moves,
    // so the level-5 foes' charge, flinch, confusion and Substitute play out
    // over a long battle that ends by forfeit.
    static const enum Move endurance[3][MAX_MON_MOVES] = {
        {MOVE_PROTECT, MOVE_SUBSTITUTE, MOVE_CONFUSE_RAY, MOVE_SUPERSONIC},
        {MOVE_SUBSTITUTE, MOVE_CONFUSE_RAY, MOVE_PROTECT, MOVE_SUPERSONIC},
        {MOVE_RECOVER, MOVE_PROTECT, MOVE_SUBSTITUTE, MOVE_CONFUSE_RAY},
    };
    const enum Move *moves = sBattle->endurance && side == 0 ? endurance[index]
                           : sets[sBattle->format - BATTLE_FORMAT_SINGLE][side][index];
    u32 m;
    for (m = 0; m < MAX_MON_MOVES; m++)
    {
        SetMonMoveSlot(mon, moves[m], m);
        if (moves[m] == MOVE_NONE)
        {
            u8 pp = 0;
            SetMonData(mon, MON_DATA_PP1 + m, &pp);
        }
    }
    // V10: Clanging Scales (hits both foes, then the user's Defense -1 once).
    if (sBattle->roster >= BATTLE_APP_VERSION_TIMING && !sBattle->endurance && side == 0 && index == 0
     && sBattle->format == BATTLE_FORMAT_DOUBLE)
        SetMonMoveSlot(mon, MOVE_CLANGING_SCALES, 3);
}

// Milestone 4-A: the V9/V10 volatile move sets on Pokemon whose abilities
// exercise every hook kind (ability slot in the species data). One holder
// per side of an UNSUPPORTED ability (Guts) shows the not-simulated notice.
static const struct AbilityFixture
{
    enum Species species;
    u8 abilityNum;
} sAbilityFixtures[4][2][3] = {
    [0] = { // Single: Intimidate vs Sturdy
        {{SPECIES_GYARADOS, 0}},
        {{SPECIES_GEODUDE, 1}},
    },
    [1] = { // Double (third in reserve): Serene Grace, No Guard, Speed Boost vs Levitate, Rough Skin, Own Tempo
        {{SPECIES_TOGEKISS, 1}, {SPECIES_MACHOP, 1}, {SPECIES_NINJASK, 0}},
        {{SPECIES_BRONZONG, 0}, {SPECIES_CARVANHA, 0}, {SPECIES_SPINDA, 0}},
    },
    [2] = { // Triple: Technician, Huge Power, Compound Eyes vs Limber, Water Absorb, Shield Dust
        {{SPECIES_SCIZOR, 1}, {SPECIES_AZUMARILL, 1}, {SPECIES_BUTTERFREE, 0}},
        {{SPECIES_HITMONLEE, 0}, {SPECIES_LANTURN, 2}, {SPECIES_DUSTOX, 0}},
    },
    [3] = { // Rotation: Clear Body, Immunity, Infiltrator vs Static, Intimidate, Guts (not simulated)
        {{SPECIES_METAGROSS, 0}, {SPECIES_SNORLAX, 0}, {SPECIES_GOLBAT, 2}},
        {{SPECIES_PIKACHU, 0}, {SPECIES_GYARADOS, 0}, {SPECIES_TAILLOW, 0}},
    },
};

static bool8 BuildFixtureEntries(BattleRequest *request, const u8 **reason)
{
    static const struct BattleFixture
    {
        enum Species species;
        enum Move move1, move2;
    } player[] = {
        {SPECIES_TAILLOW, MOVE_WING_ATTACK, MOVE_TACKLE},
        {SPECIES_MARSHTOMP, MOVE_WATER_GUN, MOVE_SURF},
        {SPECIES_GROVYLE, MOVE_STRENGTH, MOVE_ABSORB},
    }, foe[] = {
        {SPECIES_MAKUHITA, MOVE_TACKLE, MOVE_POUND},
        {SPECIES_GEODUDE, MOVE_ROCK_THROW, MOVE_TACKLE},
        {SPECIES_NUMEL, MOVE_TACKLE, MOVE_EMBER},
    };
    u32 side, i;
    // V9 fixtures face an equal-level team so charge, recharge and volatile
    // states have turns to play out.
    const u8 foeLevel = sBattle->trainerId == TRAINER_STEVEN ? 45
                      : sBattle->endurance ? 5
                      : sBattle->roster >= BATTLE_APP_VERSION_VOLATILES ? 23 : 13;
    GcExtension_SetExtraMons(sBattle->opponent, 2 * sBattle->count);
    for (side = 0; side < 2; side++)
    {
        BattleEntry *entries = side ? request->opponent : request->player;
        for (i = 0; i < sBattle->count; i++)
        {
            u32 index = side * sBattle->count + i;
            const struct BattleFixture *fixture = side ? &foe[i] : &player[i];
            struct Pokemon *mon = &sBattle->opponent[index];
            const bool8 abilities = sBattle->roster >= BATTLE_APP_VERSION_ABILITIES;
            const struct AbilityFixture *withAbility = &sAbilityFixtures[sBattle->format - BATTLE_FORMAT_SINGLE][side][i];
            CreateMonWithIVs(mon, abilities ? withAbility->species : fixture->species, side ? foeLevel : 23,
                             0x48210000 + index, OTID_STRUCT_PLAYER_ID, 15);
            if (abilities)
            {
                u8 abilityNum = withAbility->abilityNum;
                SetMonData(mon, MON_DATA_ABILITY_NUM, &abilityNum);
            }
            SetMonMoveSlot(mon, fixture->move1, 0);
            SetMonMoveSlot(mon, fixture->move2, 1);
            SetMonMoveSlot(mon, MOVE_NONE, 2);
            SetMonMoveSlot(mon, MOVE_NONE, 3);
            if (sBattle->roster == BATTLE_APP_VERSION_DRAG)
            {
                if (side == 0 && i == 0)
                    SetMonMoveSlot(mon, MOVE_DRAGON_TAIL, 1);
                else if (side == 0 && i == 1)
                {
                    SetMonMoveSlot(mon, MOVE_ROAR, 0);
                    SetMonMoveSlot(mon, MOVE_WATER_GUN, 1);
                }
                else if (side == 1 && i == 1)
                    SetMonMoveSlot(mon, MOVE_TACKLE, 0);
            }
            if (sBattle->roster == BATTLE_APP_VERSION_PIVOT && side == 0 && i == 0)
            {
                SetMonMoveSlot(mon, MOVE_U_TURN, 0);
                SetMonMoveSlot(mon, MOVE_WING_ATTACK, 1);
            }
            if (sBattle->roster && sBattle->roster < BATTLE_APP_VERSION_DRAG && side == 0 && i == 0)
            {
                u16 hp = 1;
                SetMonData(mon, MON_DATA_HP, &hp);
            }
            if (sBattle->roster == BATTLE_APP_VERSION_CONDITIONS)
                SetConditionFixture(mon, side, i);
            else if (sBattle->roster == BATTLE_APP_VERSION_EFFECTS)
                SetEffectFixture(mon, side, i);
            else if (sBattle->roster >= BATTLE_APP_VERSION_VOLATILES)
                SetVolatileFixture(mon, side, i);
            const bool8 dry = sBattle->roster == BATTLE_APP_VERSION_STRUGGLE && side == 0 && i == 0;
            if (dry)
            {
                u8 pp = 0;
                for (u32 m = 0; m < MAX_MON_MOVES; m++)
                    SetMonData(mon, MON_DATA_PP1 + m, &pp);
            }
            if (FillEntry(GC_EXTRA_SLOT_BASE + index, &entries[i]) == 0 && !dry)
            {
                *reason = side ? sText_NoOpponentMoves : sText_NoMoves;
                return FALSE;
            }
        }
    }
    return TRUE;
}

static bool8 BuildSingleEntries(BattleRequest *request, const struct Trainer *trainer, const u8 **reason)
{
    s32 lead = FindLead(&request->player[0]);
    if (lead < 0)
    {
        *reason = sText_NoLead;
        return FALSE;
    }
    CreateNPCTrainerPartyFromTrainer(sBattle->opponent, trainer);
    if (GetMonData(&sBattle->opponent[0], MON_DATA_SPECIES) == SPECIES_NONE)
    {
        *reason = sText_NoOpponent;
        return FALSE;
    }
    GcExtension_SetExtraMons(sBattle->opponent, 1);
    sBattle->leadSlot = lead;
    sBattle->leadPersonality = GetMonData(&gParties[B_TRAINER_PLAYER][lead], MON_DATA_PERSONALITY);
    sBattle->supportedMask = FillEntry(lead, &request->player[0]);
    if (FillEntry(GC_EXTRA_SLOT_BASE, &request->opponent[0]) == 0)
    {
        *reason = sText_NoOpponentMoves;
        return FALSE;
    }
    return TRUE;
}

static bool32 ResolveConditionRules(BattleConditionRules *rules)
{
    // Older type/status rules and Champions' weighted sleep and freeze
    // guarantees require a separate ruleset. Sleep Clause is also separate.
    if (GetConfig(B_STATUS_TYPE_IMMUNITY) < GEN_3 || GetConfig(B_SLEEP_TURNS) < GEN_3
     || GetConfig(B_SLEEP_TURNS) >= GEN_CHAMPIONS || GetConfig(B_FREEZE_TURNS) >= GEN_CHAMPIONS
     || B_SLEEP_CLAUSE || FlagGet(B_FLAG_SLEEP_CLAUSE))
        return FALSE;
    *rules = (BattleConditionRules){
        .flags = (GetConfig(B_PARALYZE_ELECTRIC) >= GEN_6 ? BCR_ELECTRIC_IMMUNE : 0)
               | (GetConfig(B_TOXIC_NEVER_MISS) >= GEN_6 ? BCR_POISON_TOXIC_ALWAYS_HITS : 0)
               | (B_USE_FROSTBITE ? BCR_FROSTBITE : 0)
               | (GetConfig(B_RECALC_TURN_AFTER_ACTIONS) >= GEN_8 ? BCR_DYNAMIC_SPEED : 0)
               | (GetConfig(B_HIT_THAW) >= GEN_3 ? BCR_THAW_ON_FIRE_HIT : 0),
        .poison_type = TYPE_POISON,
        .steel_type = TYPE_STEEL,
        .fire_type = TYPE_FIRE,
        .electric_type = TYPE_ELECTRIC,
        .ice_type = TYPE_ICE,
        .paralysis_speed_divisor = GetConfig(B_PARALYSIS_SPEED) >= GEN_7 ? 2 : 4,
        .paralysis_block_divisor = GetConfig(B_PARALYSIS_CHANCE) >= GEN_CHAMPIONS ? 8 : 4,
        .burn_residual_divisor = GetConfig(B_BURN_DAMAGE) >= GEN_7 || GetConfig(B_BURN_DAMAGE) == GEN_1 ? 16 : 8,
        .sleep_min = 2,
        .sleep_max = GetConfig(B_SLEEP_TURNS) >= GEN_5 ? 4 : 5,
        .thaw_percent = 20,
    };
    return TRUE;
}

// Volatile rules of this build, as read from its battle code and config:
// confusion lasts 2..B_CONFUSION_TURNS actions, hits itself 1/3 (Gen 7+)
// or 1/2 with a typeless 40-power physical hit that burn halves;
// Substitute costs max HP / 4; a multi-hit move keeps hitting after it
// breaks a substitute; only the Gen 1 rule skips the recharge after a KO.
static bool32 ResolveVolatileRules(BattleVolatileRules *rules)
{
    *rules = (BattleVolatileRules){
        .flags = BVR_SUBSTITUTE_MULTI_HIT_CONTINUES | BVR_CONFUSION_BURN
               | (B_SKIP_RECHARGE == GEN_1 ? BVR_RECHARGE_SKIP_ON_KO : 0),
        .confusion_hit_percent = GetConfig(B_CONFUSION_SELF_DMG_CHANCE) >= GEN_7 ? 33 : 50,
        .confusion_min = 2,
        .confusion_max = B_CONFUSION_TURNS,
        .confusion_power = 40,
        .substitute_divisor = 4,
    };
    return battle_volatile_rules_valid(rules);
}

static bool8 IsLegacySingle(void)
{
    return sBattle->format == BATTLE_FORMAT_SINGLE && !sBattle->roster;
}

#if TESTING
static bool8 BuildTestEntries(BattleRequest *request, const u8 **reason);
#endif

static bool8 BuildRequest(const u8 **reason)
{
    BattleRequest *request = AllocZeroedUnchecked(sizeof(*request));
    const struct Trainer *trainer = GetTrainerStructFromId(sBattle->trainerId);
    u32 a, d;

    if (request == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
#if TESTING
    if (sBattle->testMons ? !BuildTestEntries(request, reason) :
#else
    if (
#endif
        !(IsLegacySingle() ? BuildSingleEntries(request, trainer, reason)
          : sBattle->lab ? BuildLabEntries(request, reason)
                         : BuildFixtureEntries(request, reason)))
    {
        Free(request);
        return FALSE;
    }
    sBattle->battleId = !sBattle->lab ? Random32() | 1
                      : LAB_BATTLE_ID(sLabFixtures[sBattle->lab].seedOf ? sLabFixtures[sBattle->lab].seedOf : sBattle->lab);
    request->format = sBattle->format;
    request->battle_type = BTYPE_TRAINER;
    request->player_count = sBattle->count;
    request->opponent_count = sBattle->count;
    request->active_per_side = sBattle->fieldCount;
    request->version = sBattle->roster ? sBattle->roster : BATTLE_REQUEST_VERSION;
    if (request->version >= BATTLE_REQUEST_VERSION_STRUGGLE)
    {
        const u32 recoil = GetConfig(B_STRUGGLE_RECOIL);
        const u32 power = GetMovePower(MOVE_STRUGGLE);
        if (recoil < GEN_4 || GetMoveType(MOVE_STRUGGLE) != TYPE_MYSTERY || power == 0 || power > 255)
        {
            *reason = sText_StruggleRules;
            Free(request);
            return FALSE;
        }
        request->struggle_power = power;
        request->struggle_accuracy = GetMoveAccuracy(MOVE_STRUGGLE);
        request->struggle_recoil = recoil >= GEN_5 ? BSTRUGGLE_MAX_HP_NEAREST : BSTRUGGLE_MAX_HP_FLOOR;
    }
    request->rules = RULE_IGNORE_ABILITIES | RULE_IGNORE_ITEMS | RULE_IGNORE_STATUS | RULE_NO_GIMMICKS
                   | RULE_NO_CRITICAL_HITS | RULE_SUPPORTED_MOVES_ONLY;
    if (request->version >= BATTLE_REQUEST_VERSION_ABILITIES)
        request->rules &= ~RULE_IGNORE_ABILITIES;
#if TESTING
    if (sBattle->testAllowGimmicks)
        request->rules &= ~RULE_NO_GIMMICKS;
    if (sBattle->testStrictAll)
        request->rules |= RULE_STRICT_ABILITIES | (request->version >= BATTLE_REQUEST_VERSION_WEATHER ? RULE_STRICT_MOVES : 0);
#endif
    // Ability lab: the battle id seeds the GC (repeatable runs); one fixture checks strict rules.
    // V15 fixtures are strict for moves too: every move must be simulated.
    if (sBattle->lab)
        request->rules |= RULE_FIXED_SEED | (sLabFixtures[sBattle->lab].strict ? RULE_STRICT_ABILITIES : 0)
                        | (sLabFixtures[sBattle->lab].strict && request->version >= BATTLE_REQUEST_VERSION_WEATHER
                           ? RULE_STRICT_MOVES : 0);
    if (request->version >= BATTLE_REQUEST_VERSION_CONDITIONS)
    {
        if (!ResolveConditionRules(&request->conditions))
        {
            static const u8 unsupported[] = _("{ENG}Unsupported GC condition rules.");
            *reason = unsupported;
            Free(request);
            return FALSE;
        }
        request->rules &= ~RULE_IGNORE_STATUS;
    }
    if (request->version >= BATTLE_REQUEST_VERSION_VOLATILES && !ResolveVolatileRules(&request->volatiles))
    {
        static const u8 unsupported[] = _("{ENG}Unsupported GC volatile rules.");
        *reason = unsupported;
        Free(request);
        return FALSE;
    }
    if (request->version >= BATTLE_REQUEST_VERSION_WEATHER)
    {
        ResolveFieldRules(&request->field);
        if (sBattle->lab)
        {
            request->field.start_weather = sLabFixtures[sBattle->lab].startWeather;
            request->field.start_turns = sLabFixtures[sBattle->lab].startTurns;
        }
        if (request->version >= BATTLE_REQUEST_VERSION_TERRAIN)
        {
            ResolveTerrainRules(&request->terrain);
            if (sBattle->lab)
            {
                request->terrain.start_terrain = sLabFixtures[sBattle->lab].startTerrain;
                request->terrain.start_turns = sLabFixtures[sBattle->lab].startTerrainTurns;
            }
        }
        else if (MapBringsWeather())
        {
            // The overworld weather this build would bring into the battle is not exported yet.
            static const u8 unsupported[] = _("{ENG}Map weather is not supported in GC battles.");
            *reason = unsupported;
            Free(request);
            return FALSE;
        }
    }
    // V17: the items take part only if all of them can (else none does, said on
    // both screens); the strict rule refuses the battle instead.
    if (request->version >= BATTLE_REQUEST_VERSION_ITEMS)
    {
        if (sBattle->strictItems)
            request->rules |= RULE_STRICT_ITEMS;
        // M4-B1a+: the rules are final here; audited Z-Crystals and Mega
        // Stones are held with no effect only if the whole battle allows it.
        sBattle->gimmickState = AcceptGimmickItems(request);
        sBattle->itemState = CheckItems(request, &sBattle->itemReason);
        if (sBattle->itemState == ITEMS_SIMULATED)
        {
            request->rules &= ~RULE_IGNORE_ITEMS;
        }
        else if (sBattle->strictItems)
        {
            static const u8 notSimulated[] = _("{ENG}Strict items: not simulated: ");
            static const u8 itemAbility[] = _("{ENG}Strict items: item ability: ");
            StringCopy(sItemRefusal, sBattle->itemState == ITEMS_OFF_ITEM ? notSimulated : itemAbility);
            StringAppend(sItemRefusal, sText_Jpn);
            StringAppend(sItemRefusal, sBattle->itemState == ITEMS_OFF_ITEM ? GetItemName(sBattle->itemReason)
                                                                           : gAbilitiesInfo[sBattle->itemReason].name);
            *reason = sItemRefusal;
            Free(request);
            return FALSE;
        }
    }
    request->trainer_id = sBattle->trainerId;
    request->trainer_class = trainer->trainerClass;
    request->reward_mode = 0;
    request->result_policy = sBattle->policy;
    request->type_count = NUMBER_OF_MON_TYPES;
    request->battle_id = sBattle->battleId;
    request->environment = sBattle->environment;
    for (a = 0; a < NUMBER_OF_MON_TYPES; a++)
        for (d = 0; d < NUMBER_OF_MON_TYPES; d++)
            request->type_chart[a * NUMBER_OF_MON_TYPES + d] = TypeQuarters(a, d);
    sBattle->requestLength = battle_request_encode(request, sBattle->request, sizeof(sBattle->request));
    Free(request);
    if (sBattle->requestLength == 0)
    {
        // Cannot happen with REQUEST_CAPACITY; the battle is refused rather than sent cut.
        *reason = sText_RequestTooLarge;
        return FALSE;
    }
    return TRUE;
}

// ------------------------------------------------------------- handler

static void FreeBattle(void)
{
    FrontierGcBattle_FreeGfx();
    GcExtension_SetExtraMons(NULL, 0);
    TRY_FREE_AND_SET_NULL(sBattle);
}

static bool8 Init(const u8 **reason)
{
    sBattle = AllocZeroedUnchecked(sizeof(*sBattle));
    if (sBattle == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    sBattle->format = sPendingFormat;
    sBattle->fieldCount = sPendingFormat == BATTLE_FORMAT_SINGLE ? 1 : sPendingFormat == BATTLE_FORMAT_DOUBLE ? 2 : 3;
    sBattle->roster = sPendingRoster;
    sBattle->endurance = sPendingEndurance;
    sBattle->lab = sPendingLab;
    sBattle->gen5Weather = sBattle->lab && sLabFixtures[sBattle->lab].gen5Weather;
    sBattle->gen7Terrain = sBattle->lab && sLabFixtures[sBattle->lab].gen7Terrain;
    sBattle->strictItems = sBattle->lab ? sLabFixtures[sBattle->lab].strictItems : sPendingStrictItems;
    sBattle->gen3Items = sBattle->lab && sLabFixtures[sBattle->lab].gen3Items;
    sPendingStrictItems = FALSE;  // one battle only
    sBattle->count = sBattle->roster && sBattle->format == BATTLE_FORMAT_DOUBLE ? 3 : sBattle->fieldCount;
    if (sBattle->lab)
        sBattle->count = sLabFixtures[sBattle->lab].count;
    sBattle->policy = IsLegacySingle() ? sPendingPolicy : RESULT_POLICY_DISPLAY_ONLY;
    sBattle->trainerId = sPendingTrainer;
    sBattle->environment = BattleSetup_GetEnvironmentId();
    sBattle->screen = SCREEN_PREPARE;
    if (BuildRequest(reason))
        return TRUE;
    FreeBattle();
    return FALSE;
}

static const u8 *GetRequest(u32 *length)
{
    *length = sBattle->requestLength;
    return sBattle->request;
}

static u16 HandleMultiApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength)
{
    BattleMultiState state;
    BattleRosterState received;
    BattleMultiAction action = sBattle->multiAction;
    u32 side, pos;
    u8 mask = 0, struggle = 0;
    const bool8 roster = sBattle->roster != 0;

    switch (opcode)
    {
    case BATTLE_OP_STATE_ROSTER:
    case BATTLE_OP_STATE_MULTI:
        if (roster)
        {
            if (opcode != BATTLE_OP_STATE_ROSTER)
                return APP_BAD_ARGS;
            if (sBattle->roster >= BATTLE_APP_VERSION_VOLATILES)
            {
                if (!battle_volatile_state_decode(args, argLength, &received))
                    return APP_BAD_ARGS;
            }
            else if (sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS)
            {
                if (!battle_condition_state_decode(args, argLength, &received))
                    return APP_BAD_ARGS;
            }
            else if (sBattle->roster == BATTLE_APP_VERSION_PIVOT)
            {
                if (!battle_pivot_state_decode(args, argLength, &received))
                    return APP_BAD_ARGS;
            }
            else if (sBattle->roster >= BATTLE_APP_VERSION_STRUGGLE)
            {
                if (!battle_struggle_state_decode(args, argLength, &received))
                    return APP_BAD_ARGS;
            }
            else if (!battle_roster_state_decode(args, argLength, &received))
            {
                return APP_BAD_ARGS;
            }
            state = received.field_state;
            mask = received.reserves;
            struggle = received.struggle_mask;
            if (struggle >= (1u << sBattle->fieldCount))
                return APP_BAD_ARGS;
            if ((state.phase >= BPHASE_REPLACE && !mask) || mask >= (1u << sBattle->count)
             || (state.phase >= BPHASE_REPLACE && (state.actor >= sBattle->fieldCount
                 || ((state.phase == BPHASE_REPLACE) != (state.field[0][state.actor].party_slot == BATTLE_SLOT_NONE)))))
                return APP_BAD_ARGS;
            for (pos = 0; pos < BATTLE_MAX_PER_SIDE; pos++)
                if (received.player[pos].party_slot != (pos < sBattle->count ? GC_EXTRA_SLOT_BASE + pos : BATTLE_SLOT_NONE))
                    return APP_BAD_ARGS;
        }
        else if (opcode != BATTLE_OP_STATE_MULTI || !battle_multi_state_decode(args, argLength, &state))
            return APP_BAD_ARGS;
        for (side = 0; side < 2; side++)
            for (pos = 0; pos < BATTLE_FIELD_SLOTS; pos++)
            {
                u8 slot = state.field[side][pos].party_slot;
                if (slot == BATTLE_SLOT_NONE && (roster || pos >= sBattle->fieldCount || state.outcome == OUTCOME_UNSUPPORTED))
                    continue;
                if (slot < GC_EXTRA_SLOT_BASE + side * sBattle->count
                 || slot >= GC_EXTRA_SLOT_BASE + (side + 1) * sBattle->count)
                    return APP_BAD_ARGS;
            }
        if (sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS)
        {
            memcpy(sBattle->status, received.status, sizeof(sBattle->status));
            memcpy(sBattle->stages, received.stages, sizeof(sBattle->stages));
            memcpy(sBattle->volatiles, received.volatiles, sizeof(sBattle->volatiles));
        }
        sBattle->multiState = state;
        sBattle->reserveMask = mask;
        sBattle->struggleMask = struggle;
        sBattle->haveState = TRUE;
        sBattle->multiAction = (BattleMultiAction){.token = state.token, .actor = state.actor};
        sBattle->screen = state.phase == BPHASE_CHOOSE ? SCREEN_COMMAND
                        : state.phase == BPHASE_ENDED ? SCREEN_ENDED
                        : state.phase == BPHASE_REPLACE ? SCREEN_REPLACE
                        : state.phase == BPHASE_PIVOT ? SCREEN_PIVOT : SCREEN_WAIT;
        sBattle->cursor = 0;
        if (state.phase >= BPHASE_REPLACE)
            while (!(mask & (1u << sBattle->cursor)))
                sBattle->cursor++;
        sBattle->rotation = BROT_KEEP;
        if (sBattle->format == BATTLE_FORMAT_ROTATION && state.phase == BPHASE_CHOOSE && !state.field[0][BPOS_CENTER].hp)
            sBattle->rotation = state.field[0][BPOS_LEFT].hp ? BROT_LEFT : BROT_RIGHT;
        GcExtension_Redraw();
        return APP_OK;
    case BATTLE_OP_ACTION_ROSTER:
    case BATTLE_OP_ACTION_MULTI:
        if (argLength != 0 || (opcode == BATTLE_OP_ACTION_ROSTER) != roster)
            return APP_BAD_ARGS;
        if (GcExtension_IsExitRequested())
            action.flags |= BACTION_FLAG_EXIT;
        *outLength = battle_multi_action_encode(&action, out);
        return APP_OK;
    }
    return APP_UNKNOWN_OP;
}

static u16 HandleApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength)
{
    BattleState state;
    BattleAction action = {0};

    if (!IsLegacySingle())
        return HandleMultiApp(opcode, args, argLength, out, outLength);
    switch (opcode)
    {
    case BATTLE_OP_STATE:
        if (!battle_state_decode(args, argLength, &state))
            return APP_BAD_ARGS;
        sBattle->state = state;
        sBattle->haveState = TRUE;
        memset(&sBattle->pending, 0, sizeof(sBattle->pending));
        if (state.phase == BPHASE_CHOOSE)
        {
            sBattle->screen = SCREEN_COMMAND;
            sBattle->cursor = 0;
        }
        else
        {
            sBattle->screen = state.phase == BPHASE_ENDED ? SCREEN_ENDED : SCREEN_WAIT;
        }
        GcExtension_Redraw();
        return APP_OK;
    case BATTLE_OP_ACTION:
        if (sBattle->pending.action != BACTION_NONE)
            action = sBattle->pending;
        else
            action.turn = sBattle->state.turn;
        if (GcExtension_IsExitRequested())
            action.flags |= BACTION_FLAG_EXIT;
        *outLength = battle_action_encode(&action, out);
        return APP_OK;
    }
    return APP_UNKNOWN_OP;
}

static u16 OnResult(u16 version, const u8 *data, u16 length)
{
    BattleResult result;
    u32 i, m;

    if (version != BATTLE_RESULT_VERSION || !battle_result_decode(data, length, &result))
        return RESULT_REJECTED;
    if (result.battle_id != sBattle->battleId || result.player_count != sBattle->count
     || result.opponent_count != sBattle->count)
        return RESULT_REJECTED;
    for (i = 0; i < result.player_count; i++)
    {
        u8 slot = IsLegacySingle() ? sBattle->leadSlot : GC_EXTRA_SLOT_BASE + i;
        struct Pokemon *mon = GcExtension_GetExportMon(slot);
        if (result.player[i].slot != slot || result.player[i].hp > GetMonData(mon, MON_DATA_MAX_HP))
            return RESULT_REJECTED;
        for (m = 0; m < MAX_MON_MOVES; m++)
        {
            u16 move = GetMonData(mon, MON_DATA_MOVE1 + m);
            u8 maxPp = move != MOVE_NONE ? CalculatePPWithBonus(move, GetMonData(mon, MON_DATA_PP_BONUSES), m) : 0;
            if (result.player[i].pp[m] > maxPp)
                return RESULT_REJECTED;
        }
    }
    sBattle->result = result;
    sBattle->resultAccepted = TRUE;
    return RESULT_ACCEPTED;
}

static void AppendNumber(u8 *dest, s32 value)
{
    u8 digits[8];
    ConvertIntToDecimalStringN(digits, value, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(dest, sText_Eng);
    StringAppend(dest, digits);
}

// Runs once, after the link is closed. Only the "apply" policy writes HP/PP,
// only for a validated result, only to the same lead Pokemon, only once.
static const u8 *Finish(bool8 *changedParty)
{
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u32 i, before;

    if (!sBattle->resultAccepted)
        return sText_NoResult;
    if (sBattle->policy != RESULT_POLICY_APPLY_HP_PP || sBattle->applied
     || sBattle->result.outcome == OUTCOME_CANCELLED || sBattle->result.outcome == OUTCOME_UNSUPPORTED)
        return sText_NotApplied;
    if (GetMonData(lead, MON_DATA_PERSONALITY) != sBattle->leadPersonality)
        return sText_ApplyRefused;
    before = GetMonData(lead, MON_DATA_HP);
    SetMonData(lead, MON_DATA_HP, &sBattle->result.player[0].hp);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonData(lead, MON_DATA_PP1 + i, &sBattle->result.player[0].pp[i]);
    sBattle->applied = TRUE;
    *changedParty = TRUE;
    StringCopy(sBattle->message, sText_AppliedHp);
    AppendNumber(sBattle->message, before);
    StringAppend(sBattle->message, sText_Slash);
    AppendNumber(sBattle->message, sBattle->result.player[0].hp);
    return sBattle->message;
}

void FrontierGcBattle_GetScene(struct FrontierGcBattleScene *scene)
{
    scene->player = sBattle != NULL ? &gParties[B_TRAINER_PLAYER][sBattle->leadSlot] : NULL;
    scene->opponent = sBattle != NULL ? &sBattle->opponent[0] : NULL;
    scene->trainerId = sBattle != NULL ? sBattle->trainerId : 0;
    scene->environment = sBattle != NULL ? sBattle->environment : 0;
}

// ------------------------------------------------------------- GBA screen

static void AppendMonLine(u8 *text, struct Pokemon *mon)
{
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringAppend(text, sText_Jpn);
    StringAppend(text, name);
    StringAppend(text, sText_Lv);
    AppendNumber(text, GetMonData(mon, MON_DATA_LEVEL));
}

static void DrawHp(u8 body, u8 line, u16 hp, u16 max)
{
    u8 text[32];
    StringCopy(text, sText_Hp);
    AppendNumber(text, hp);
    StringAppend(text, sText_Slash);
    AppendNumber(text, max);
    GcExtension_PrintLine(body, text, 136, line, GC_COLOR_NORMAL);
}

// V17: before the battle, whether held items take part; when every item is
// off, why (the item that is not simulated, or the item ability).
static void DrawItemState(u8 body, u8 row)
{
    static const u8 simulated[] = _("{ENG}Held items: simulated");
    static const u8 off[] = _("{ENG}ALL HELD ITEM EFFECTS ARE OFF");
    static const u8 notSimulated[] = _("{ENG}Not simulated: ");
    static const u8 itemAbility[] = _("{ENG}Item ability: ");
    u8 text[64];
    if (sBattle->itemState == ITEMS_NOT_SENT)
        return;
    if (sBattle->itemState == ITEMS_SIMULATED)
    {
        GcExtension_PrintLine(body, simulated, 0, row, GC_COLOR_DIM);
        return;
    }
    GcExtension_PrintLine(body, off, 0, row, GC_COLOR_ERROR);
    StringCopy(text, sBattle->itemState == ITEMS_OFF_ITEM ? notSimulated : itemAbility);
    StringAppend(text, sText_Jpn);
    StringAppend(text, sBattle->itemState == ITEMS_OFF_ITEM ? GetItemName(sBattle->itemReason)
                                                           : gAbilitiesInfo[sBattle->itemReason].name);
    GcExtension_PrintLine(body, text, 0, row + 1, GC_COLOR_ERROR);
}

// V17, during the battle: the last body row of every screen that is not a list
// says that every item is off (the first command prompt comes before any action).
static void DrawItemNotice(u8 body)
{
    static const u8 off[] = _("{ENG}ALL ITEMS OFF: ");
    u8 text[64];
    if (sBattle->itemState != ITEMS_OFF_ITEM && sBattle->itemState != ITEMS_OFF_ABILITY)
        return;
    StringCopy(text, off);
    StringAppend(text, sText_Jpn);
    StringAppend(text, sBattle->itemState == ITEMS_OFF_ITEM ? GetItemName(sBattle->itemReason)
                                                           : gAbilitiesInfo[sBattle->itemReason].name);
    GcExtension_PrintLine(body, text, 0, 5, GC_COLOR_ERROR);
}

static void DrawSingle(u8 body, u8 help)
{
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u8 text[64];
    u32 i;

    if (!sBattle->haveState)
    {
        GcExtension_PrintLine(body, sText_Prepare, 0, 0, GC_COLOR_NORMAL);
        DrawItemState(body, 2);
        return;
    }
    StringCopy(text, sText_Vs);
    AppendMonLine(text, &sBattle->opponent[0]);
    GcExtension_PrintLine(body, text, 0, 0, GC_COLOR_DIM);
    DrawHp(body, 0, sBattle->state.opponent_hp, sBattle->state.opponent_max_hp);
    text[0] = EOS;
    AppendMonLine(text, lead);
    GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_NORMAL);
    DrawHp(body, 1, sBattle->state.player_hp, sBattle->state.player_max_hp);

    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        GcExtension_PrintLine(body, sText_Cursor, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(body, sText_Fight, 12, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_HelpCommand, 0, 0, GC_COLOR_NORMAL);
        break;
    case SCREEN_MOVES:
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            u16 move = GetMonData(lead, MON_DATA_MOVE1 + i);
            bool8 usable = (sBattle->supportedMask & (1u << i)) && sBattle->state.pp[i] > 0;
            u8 color = !usable ? GC_COLOR_DIM : i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
            if (move == MOVE_NONE)
                continue;
            if (i == sBattle->cursor)
                GcExtension_PrintLine(body, sText_Cursor, 0, 2 + i, color);
            StringCopy(text, sText_Jpn);
            StringAppend(text, GetMoveName(move));
            GcExtension_PrintLine(body, text, 12, 2 + i, color);
            if (!(sBattle->supportedMask & (1u << i)))
            {
                GcExtension_PrintLine(body, sText_Unsupported, 120, 2 + i, GC_COLOR_DIM);
            }
            else
            {
                StringCopy(text, sText_Pp);
                AppendNumber(text, sBattle->state.pp[i]);
                StringAppend(text, sText_Slash);
                AppendNumber(text, CalculatePPWithBonus(move, GetMonData(lead, MON_DATA_PP_BONUSES), i));
                GcExtension_PrintLine(body, text, 150, 2 + i, color);
            }
        }
        GcExtension_PrintLine(help, sText_HelpMoves, 0, 0, GC_COLOR_NORMAL);
        break;
    case SCREEN_TARGET:
    case SCREEN_REPLACE:
    case SCREEN_SWITCH:
    case SCREEN_PIVOT:
    case SCREEN_WAIT:
        GcExtension_PrintLine(body, sText_Wait, 0, 3, GC_COLOR_DIM);
        break;
    case SCREEN_ENDED:
        switch (sBattle->state.outcome)
        {
        case OUTCOME_WIN:  GcExtension_PrintLine(body, sText_Win, 0, 3, GC_COLOR_HIGHLIGHT); break;
        case OUTCOME_LOSE: GcExtension_PrintLine(body, sText_Lose, 0, 3, GC_COLOR_ERROR); break;
        case OUTCOME_CANCELLED: GcExtension_PrintLine(body, sText_Cancelled, 0, 3, GC_COLOR_NORMAL); break;
        default: GcExtension_PrintLine(body, sText_Refused, 0, 3, GC_COLOR_ERROR); break;
        }
        GcExtension_PrintLine(help, sText_HelpEnded, 0, 0, GC_COLOR_NORMAL);
        break;
    }
}

static bool8 MoveUsable(u8 slot)
{
    return (sBattle->supportedMask & (1u << slot)) && sBattle->state.pp[slot] > 0;
}

static void InputSingle(void)
{
    u32 i;
    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->screen = SCREEN_MOVES;
            for (sBattle->cursor = 0; sBattle->cursor < MAX_MON_MOVES && !MoveUsable(sBattle->cursor); sBattle->cursor++)
                ;
            if (sBattle->cursor == MAX_MON_MOVES)
                sBattle->cursor = 0;
            GcExtension_Redraw();
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->pending.turn = sBattle->state.turn;
            sBattle->pending.action = BACTION_FORFEIT;
            sBattle->screen = SCREEN_WAIT;
            GcExtension_Redraw();
        }
        break;
    case SCREEN_MOVES:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
        {
            s32 step = JOY_REPEAT(DPAD_UP) ? MAX_MON_MOVES - 1 : 1;
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                sBattle->cursor = (sBattle->cursor + step) % MAX_MON_MOVES;
                if (GetMonData(&gParties[B_TRAINER_PLAYER][sBattle->leadSlot], MON_DATA_MOVE1 + sBattle->cursor) != MOVE_NONE)
                    break;
            }
            PlaySE(SE_SELECT);
            GcExtension_Redraw();
        }
        else if (JOY_NEW(A_BUTTON) && MoveUsable(sBattle->cursor))
        {
            PlaySE(SE_SELECT);
            sBattle->pending.turn = sBattle->state.turn;
            sBattle->pending.action = BACTION_MOVE;
            sBattle->pending.move_slot = sBattle->cursor;
            sBattle->screen = SCREEN_WAIT;
            GcExtension_Redraw();
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_FAILURE);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->screen = SCREEN_COMMAND;
            GcExtension_Redraw();
        }
        break;
    case SCREEN_ENDED:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            GcExtension_RequestExit();
        }
        break;
    }
}

static const u8 sText_PositionHp[] = _("{ENG} HP ");
static const u8 sText_Left[] = _("{ENG}L");
static const u8 sText_Center[] = _("{ENG}C");
static const u8 sText_Right[] = _("{ENG}R");
static const u8 sText_Front[] = _("{ENG}FRONT");
static const u8 sText_Target[] = _("{ENG}Choose enemy position");
static const u8 sText_TargetHelp[] = _("{ENG}D-Pad: Target  A: OK  B: Back");
static const u8 sText_ShiftHelp[] = _("{ENG}A: Fight  SELECT: Shift  B: Quit");
static const u8 sText_RotateHelp[] = _("{ENG}L/R: Rotate  A: Fight  B: Quit");
static const u8 sText_Empty[] = _("{ENG}--");

static u8 SelectedPosition(void)
{
    if (sBattle->format != BATTLE_FORMAT_ROTATION)
        return sBattle->multiState.actor;
    if (sBattle->rotation == BROT_LEFT)
        return BPOS_LEFT;
    if (sBattle->rotation == BROT_RIGHT)
        return BPOS_RIGHT;
    return BPOS_CENTER;
}

static const u8 *PositionName(u8 pos)
{
    if (pos == BPOS_LEFT)
        return sText_Left;
    if (pos == BPOS_RIGHT || sBattle->format == BATTLE_FORMAT_DOUBLE)
        return sText_Right;
    return sBattle->format == BATTLE_FORMAT_ROTATION ? sText_Front : sText_Center;
}

static u8 FirstChoice(u8 mask)
{
    u8 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (mask & (1 << i))
            return i;
    return 0;
}

static u8 ChoiceAfter(u8 mask, u8 current, u8 count, bool8 backward)
{
    u8 i;
    for (i = 0; i < count; i++)
    {
        current = (current + (backward ? count - 1 : 1)) % count;
        if (mask & (1u << current))
            return current;
    }
    return current;
}

static u8 AvailableMoves(u8 pos)
{
    u8 mask = 0, i;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (sBattle->multiState.targets[pos][i])
            mask |= 1u << i;
    return mask;
}

static const u8 *ConditionLabel(u8 kind)
{
    static const u8 *const labels[] = {
        COMPOUND_STRING("{ENG}"),
        COMPOUND_STRING("{ENG} PSN"),
        COMPOUND_STRING("{ENG} TOX"),
        COMPOUND_STRING("{ENG} BRN"),
        COMPOUND_STRING("{ENG} PAR"),
        COMPOUND_STRING("{ENG} SLP"),
        COMPOUND_STRING("{ENG} FRZ"),
        COMPOUND_STRING("{ENG} FRB"),
    };
    return labels[kind];
}

// Short volatile marks for the field rows (APP V9): c confused, s substitute,
// ! charging or out of reach, r recharging.
static void AppendVolatileMarks(u8 *text, u8 flags)
{
    static const u8 confused[] = _("{ENG}c");
    static const u8 substitute[] = _("{ENG}s");
    static const u8 charging[] = _("{ENG}!");
    static const u8 recharging[] = _("{ENG}r");
    if (flags & BVF_CONFUSED)
        StringAppend(text, confused);
    if (flags & BVF_SUBSTITUTE)
        StringAppend(text, substitute);
    if (flags & BVF_CHARGING)
        StringAppend(text, charging);
    if (flags & BVF_RECHARGING)
        StringAppend(text, recharging);
}

static void DrawConditionSummary(u8 body, u8 pos)
{
    static const u8 *const stats[] = {
        COMPOUND_STRING("{ENG} A"), COMPOUND_STRING("{ENG} D"), COMPOUND_STRING("{ENG} SA"),
        COMPOUND_STRING("{ENG} SD"), COMPOUND_STRING("{ENG} S"), COMPOUND_STRING("{ENG} AC"),
        COMPOUND_STRING("{ENG} EV"),
    };
    static const u8 plus[] = _("{ENG}+");
    static const u8 minus[] = _("{ENG}-");
    u8 text[96];
    StringCopy(text, ConditionLabel(sBattle->status[0][pos]));
    if (sBattle->volatiles[0][pos] & BVF_CONFUSED)
        StringAppend(text, COMPOUND_STRING("{ENG} CNF"));
    if (sBattle->volatiles[0][pos] & BVF_SUBSTITUTE)
        StringAppend(text, COMPOUND_STRING("{ENG} SUB"));
    for (u32 i = 0; i < BSTAT_COUNT; i++)
    {
        s8 stage = sBattle->stages[0][pos][i];
        if (!stage)
            continue;
        StringAppend(text, stats[i]);
        StringAppend(text, stage > 0 ? plus : minus);
        AppendNumber(text, stage > 0 ? stage : -stage);
    }
    GcExtension_PrintLine(body, text, 0, 2, GC_COLOR_NORMAL);
}

static void DrawPositions(u8 body, u8 side, u8 row, u8 selected, u8 legal)
{
    const BattleMultiState *state = &sBattle->multiState;
    u8 slot, text[32];
    for (slot = 0; slot < sBattle->fieldCount; slot++)
    {
        const BattleFieldMon *mon = &state->field[side][slot];
        u8 color = !(legal & (1 << slot)) || !mon->hp ? GC_COLOR_DIM
                 : slot == selected ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
        static const u8 shortFront[] = _("{ENG}F");
        static const u8 colon[] = _("{ENG}:");
        const bool8 conditions = sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS;
        const u8 *name = PositionName(slot);
        StringCopy(text, conditions && name == sText_Front ? shortFront : name);
        StringAppend(text, conditions ? colon : sText_PositionHp);
        if (mon->hp)
        {
            AppendNumber(text, mon->hp);
            if (conditions)
                StringAppend(text, ConditionLabel(sBattle->status[side][slot]));
            AppendVolatileMarks(text, sBattle->volatiles[side][slot]);
        }
        else
            StringAppend(text, sText_Empty);
        if (slot == selected)
            GcExtension_PrintLine(body, sText_Cursor, slot * 74, row, color);
        GcExtension_PrintLine(body, text, slot * 74 + 9, row, color);
    }
}

static void DrawMulti(u8 body, u8 help)
{
    const BattleMultiState *state = &sBattle->multiState;
    const bool8 rotation = sBattle->format == BATTLE_FORMAT_ROTATION;
    const u8 pos = SelectedPosition();
    const BattleFieldMon *mon = &state->field[0][pos];
    u8 text[64], i;

    if (!sBattle->haveState)
    {
        GcExtension_PrintLine(body, sText_Prepare, 0, 0, GC_COLOR_NORMAL);
        DrawItemState(body, 2);
        return;
    }
    DrawPositions(body, 1, 0, sBattle->screen == SCREEN_TARGET ? sBattle->target : BATTLE_SLOT_NONE,
                  sBattle->screen == SCREEN_TARGET ? state->targets[pos][sBattle->cursor] : 7);
    if (sBattle->screen == SCREEN_REPLACE || sBattle->screen == SCREEN_SWITCH || sBattle->screen == SCREEN_PIVOT)
    {
        static const u8 choose[] = _("{ENG}Choose a reserve for ");
        static const u8 switchOut[] = _("{ENG}Switch out position ");
        static const u8 replaceHelp[] = _("{ENG}A: Send out   B: Forfeit");
        static const u8 switchHelp[] = _("{ENG}A: Switch   B: Back");
        const bool8 switching = sBattle->screen == SCREEN_SWITCH;
        StringCopy(text, switching ? switchOut : choose);
        StringAppend(text, PositionName(state->actor));
        GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_HIGHLIGHT);
        u8 row = 2;
        for (i = 0; i < sBattle->count; i++)
        {
            if (!(sBattle->reserveMask & (1u << i)))
                continue;
            u8 color = i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
            if (i == sBattle->cursor)
                GcExtension_PrintLine(body, sText_Cursor, 0, row, color);
            text[0] = EOS;
            AppendMonLine(text, GcExtension_GetExportMon(GC_EXTRA_SLOT_BASE + i));
            GcExtension_PrintLine(body, text, 12, row++, color);
        }
        GcExtension_PrintLine(help, switching ? switchHelp : replaceHelp, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    if (sBattle->screen == SCREEN_ENDED)
    {
        const u8 *result = state->outcome == OUTCOME_WIN ? sText_Win : state->outcome == OUTCOME_LOSE ? sText_Lose
                         : state->outcome == OUTCOME_DRAW ? sText_Draw
                         : state->outcome == OUTCOME_CANCELLED ? sText_Cancelled : sText_Refused;
        GcExtension_PrintLine(body, result, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_HelpEnded, 0, 0, GC_COLOR_NORMAL);
        DrawItemNotice(body);
        return;
    }
    if (sBattle->screen == SCREEN_WAIT)
    {
        GcExtension_PrintLine(body, sText_Wait, 0, 3, GC_COLOR_DIM);
        DrawItemNotice(body);
        return;
    }

    StringCopy(text, PositionName(pos));
    StringAppend(text, sText_Cursor);
    AppendMonLine(text, GcExtension_GetExportMon(mon->party_slot));
    GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_HIGHLIGHT);
    DrawHp(body, 1, mon->hp, mon->max_hp);

    if (sBattle->screen == SCREEN_COMMAND)
    {
        if (sBattle->roster >= BATTLE_APP_VERSION_CONDITIONS)
            DrawConditionSummary(body, pos);
        DrawPositions(body, 0, 3, pos, 7);
        static const u8 switchHint[] = _("{ENG}START: Party");
        static const u8 struggleText[] = _("{ENG}Struggle");
        GcExtension_PrintLine(body, sBattle->struggleMask & (1u << pos) ? struggleText : sText_Fight,
                              0, 4, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, rotation ? sText_RotateHelp
                            : state->flags & BATTLE_MULTI_SHIFT ? sText_ShiftHelp : sText_HelpCommand,
                              0, 0, GC_COLOR_NORMAL);
        if (sBattle->reserveMask)
            GcExtension_PrintLine(body, switchHint, 88, 4, GC_COLOR_NORMAL);
        DrawItemNotice(body);
        return;
    }
    if (sBattle->screen == SCREEN_TARGET)
    {
        GcExtension_PrintLine(body, sText_Target, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_TargetHelp, 0, 0, GC_COLOR_NORMAL);
        DrawItemNotice(body);
        return;
    }

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u16 move = GetMonData(GcExtension_GetExportMon(mon->party_slot), MON_DATA_MOVE1 + i);
        u8 color = !state->targets[pos][i] ? GC_COLOR_DIM : i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
        if (move == MOVE_NONE)
            continue;
        if (i == sBattle->cursor)
            GcExtension_PrintLine(body, sText_Cursor, 0, 2 + i, color);
        StringCopy(text, sText_Jpn);
        StringAppend(text, GetMoveName(move));
        GcExtension_PrintLine(body, text, 12, 2 + i, color);
        StringCopy(text, sText_Pp);
        AppendNumber(text, state->pp[pos][i]);
        GcExtension_PrintLine(body, text, 150, 2 + i, color);
    }
    GcExtension_PrintLine(help, sText_HelpMoves, 0, 0, GC_COLOR_NORMAL);
}

static void QueueMultiAction(u8 kind, u8 target)
{
    sBattle->multiAction.action = kind;
    sBattle->multiAction.move_slot = kind == BACTION_MOVE ? sBattle->cursor : 0;
    sBattle->multiAction.target = target;
    sBattle->multiAction.rotation = kind == BACTION_MOVE || kind == BACTION_STRUGGLE ? sBattle->rotation : BROT_KEEP;
    sBattle->screen = SCREEN_WAIT;
}

static void InputMulti(void)
{
    const BattleMultiState *state = &sBattle->multiState;
    const u8 pos = SelectedPosition();
    const u8 mask = AvailableMoves(pos);
    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        if (sBattle->format == BATTLE_FORMAT_ROTATION && (JOY_NEW(L_BUTTON) || JOY_NEW(R_BUTTON)))
        {
            const bool8 left = JOY_NEW(L_BUTTON) != 0;
            const u8 next = left ? BROT_LEFT : BROT_RIGHT;
            const u8 rotation = sBattle->rotation == next ? BROT_KEEP : next;
            const u8 slot = rotation == BROT_KEEP ? BPOS_CENTER : left ? BPOS_LEFT : BPOS_RIGHT;
            if (!AvailableMoves(slot) && !(sBattle->struggleMask & (1u << slot)))
            {
                PlaySE(SE_FAILURE);
                return;
            }
            sBattle->rotation = rotation;
        }
        else if (JOY_NEW(START_BUTTON) && sBattle->reserveMask)
        {
            sBattle->cursor = ChoiceAfter(sBattle->reserveMask, sBattle->count - 1, sBattle->count, FALSE);
            sBattle->screen = SCREEN_SWITCH;
        }
        else if (JOY_NEW(SELECT_BUTTON) && (state->flags & BATTLE_MULTI_SHIFT))
            QueueMultiAction(BACTION_SHIFT, 0);
        else if (JOY_NEW(B_BUTTON))
            QueueMultiAction(BACTION_FORFEIT, 0);
        else if (JOY_NEW(A_BUTTON) && (sBattle->struggleMask & (1u << pos)))
            QueueMultiAction(BACTION_STRUGGLE, 0);
        else if (JOY_NEW(A_BUTTON) && mask)
        {
            sBattle->cursor = FirstChoice(mask);
            sBattle->screen = SCREEN_MOVES;
        }
        else
            return;
        break;
    case SCREEN_MOVES:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
            sBattle->cursor = ChoiceAfter(mask, sBattle->cursor, MAX_MON_MOVES, JOY_REPEAT(DPAD_UP));
        else if (JOY_NEW(B_BUTTON))
            sBattle->screen = SCREEN_COMMAND;
        else if (JOY_NEW(A_BUTTON) && (mask & (1 << sBattle->cursor)))
        {
            u8 targets = state->targets[pos][sBattle->cursor];
            sBattle->target = FirstChoice(targets & 7);
            if ((targets & BATTLE_TARGET_AUTO) || sBattle->format == BATTLE_FORMAT_ROTATION)
                QueueMultiAction(BACTION_MOVE, sBattle->target);
            else
                sBattle->screen = SCREEN_TARGET;
        }
        else
            return;
        break;
    case SCREEN_TARGET:
        if (JOY_REPEAT(DPAD_LEFT) || JOY_REPEAT(DPAD_RIGHT))
            sBattle->target = ChoiceAfter(state->targets[pos][sBattle->cursor] & 7,
                                         sBattle->target, sBattle->fieldCount, JOY_REPEAT(DPAD_LEFT));
        else if (JOY_NEW(B_BUTTON))
            sBattle->screen = SCREEN_MOVES;
        else if (JOY_NEW(A_BUTTON))
            QueueMultiAction(BACTION_MOVE, sBattle->target);
        else
            return;
        break;
    case SCREEN_REPLACE:
    case SCREEN_SWITCH:
    case SCREEN_PIVOT:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
            sBattle->cursor = ChoiceAfter(sBattle->reserveMask, sBattle->cursor, sBattle->count, JOY_REPEAT(DPAD_UP));
        else if (JOY_NEW(A_BUTTON))
            QueueMultiAction(sBattle->screen == SCREEN_SWITCH ? BACTION_SWITCH :
                             sBattle->screen == SCREEN_PIVOT ? BACTION_PIVOT : BACTION_REPLACE, sBattle->cursor);
        else if (JOY_NEW(B_BUTTON))
        {
            if (sBattle->screen == SCREEN_SWITCH)
                sBattle->screen = SCREEN_COMMAND;
            else
                QueueMultiAction(BACTION_FORFEIT, 0);
        }
        else
            return;
        break;
    case SCREEN_ENDED:
        if (!JOY_NEW(A_BUTTON))
            return;
        GcExtension_RequestExit();
        break;
    default:
        return;
    }
    PlaySE(SE_SELECT);
    GcExtension_Redraw();
}

static void Draw(u8 body, u8 help)
{
    if (IsLegacySingle())
        DrawSingle(body, help);
    else
        DrawMulti(body, help);
}

static void Input(void)
{
    if (IsLegacySingle())
        InputSingle();
    else
        InputMulti();
}

static const struct GcModeHandler sFrontierGcBattleMode =
{
    .modeId = GC_MODE_BATTLE,
    .modeVersion = BATTLE_MODE_VERSION,
    .appId = APP_GC_BATTLE,
    .appVersion = BATTLE_APP_VERSION,
    .init = Init,
    .getRequest = GetRequest,
    .handleApp = HandleApp,
    .draw = Draw,
    .input = Input,
    .onResult = OnResult,
    .finish = Finish,
    .free = FreeBattle,
    .resolveAsset = FrontierGcBattle_ResolveAsset,
};

// Script special (waitstate): VAR_0x8004 = result policy (0 show, 1 apply
// HP/PP), VAR_0x8005 = trainer id. VAR_0x8006 is the format (0 for
// Single). Bits 8 through 12 select the lifecycle fixtures, bit 13 selects V7,
// bit 14 the V8 move effect fixtures and bit 15 the volatile fixtures as V12
// with abilities (with bit 11: as V11; bit 10: V10; bit 9: V9; with bit 8:
// level-5 foes for endurance, debug menu only).
static EWRAM_DATA struct GcModeHandler sFrontierGcRosterMode = {0};

void FrontierGcBattle_Start(void)
{
    u8 format = gSpecialVar_0x8006 & 0xff;
    sPendingPolicy = gSpecialVar_0x8004 == RESULT_POLICY_APPLY_HP_PP ? RESULT_POLICY_APPLY_HP_PP : RESULT_POLICY_DISPLAY_ONLY;
    sPendingTrainer = gSpecialVar_0x8005;
    sPendingFormat = format >= BATTLE_FORMAT_DOUBLE && format <= BATTLE_FORMAT_ROTATION ? format : BATTLE_FORMAT_SINGLE;
    sPendingRoster = 0;
    sPendingEndurance = FALSE;
    sPendingLab = 0;
    if (gSpecialVar_0x8006 & 0x8000)
    {
        // V12 (multi-hook abilities); bits 11, 10, 9 keep V11, V10, V9 for regression runs.
        sPendingRoster = (gSpecialVar_0x8006 & 0x200) ? BATTLE_APP_VERSION_VOLATILES
                       : (gSpecialVar_0x8006 & 0x400) ? BATTLE_APP_VERSION_TIMING
                       : (gSpecialVar_0x8006 & 0x800) ? BATTLE_APP_VERSION_ABILITIES
                       : BATTLE_APP_VERSION_ABILITY_HOOKS;
        sPendingEndurance = (gSpecialVar_0x8006 & 0x100) != 0;
    }
    else if (gSpecialVar_0x8006 & 0x4000)
        sPendingRoster = BATTLE_APP_VERSION_EFFECTS;
    else if (gSpecialVar_0x8006 & 0x2000)
        sPendingRoster = BATTLE_APP_VERSION_CONDITIONS;
    else if (format == BATTLE_FORMAT_DOUBLE)
    {
        if (gSpecialVar_0x8006 & 0x1000)
            sPendingRoster = BATTLE_APP_VERSION_PIVOT;
        else if (gSpecialVar_0x8006 & 0x800)
            sPendingRoster = BATTLE_APP_VERSION_DRAG;
        else if (gSpecialVar_0x8006 & 0x400)
            sPendingRoster = BATTLE_APP_VERSION_STRUGGLE;
        else if (gSpecialVar_0x8006 & 0x200)
            sPendingRoster = BATTLE_APP_VERSION_SWITCH;
        else if (gSpecialVar_0x8006 & 0x100)
            sPendingRoster = BATTLE_APP_VERSION_ROSTER;
    }
    if (sPendingRoster)
    {
        sFrontierGcRosterMode = sFrontierGcBattleMode;
        sFrontierGcRosterMode.appVersion = sPendingRoster;
        GcStartMode(&sFrontierGcRosterMode);
    }
    else
    {
        GcStartMode(&sFrontierGcBattleMode);
    }
}

// V17 strict items for the next battle started (formal use: results must be
// exact): an item the GC cannot run refuses the battle instead of turning
// every item off. Cleared once a battle starts.
void FrontierGcBattle_SetStrictItems(bool8 strict)
{
    sPendingStrictItems = strict;
}

// Script special (waitstate): VAR_0x8004 = ability lab fixture (debug menu).
void FrontierGcBattle_StartLab(void)
{
    const u16 fixture = gSpecialVar_0x8004;
    if (fixture == 0 || fixture > LAB_FIXTURE_COUNT)
    {
        ScriptContext_Enable();
        return;
    }
    sPendingPolicy = RESULT_POLICY_DISPLAY_ONLY;
    sPendingTrainer = TRAINER_CALVIN_1;
    sPendingFormat = sLabFixtures[fixture].format;
    sPendingRoster = sLabFixtures[fixture].version ? sLabFixtures[fixture].version : BATTLE_APP_VERSION_ABILITY_HOOKS;
    sPendingEndurance = FALSE;
    sPendingLab = fixture;
    sFrontierGcRosterMode = sFrontierGcBattleMode;
    sFrontierGcRosterMode.appVersion = sPendingRoster;
    GcStartMode(&sFrontierGcRosterMode);
}

#if TESTING
// ------------------------------------------------------------- test hooks
// test/battle/gc_gimmick_item_reference.c: the exporter's judgement through
// the same code a battle runs (ResolveMove, ResolveItem, BuildRequest), with
// the Pokemon a test gives instead of the debug fixtures.

u8 FrontierGcBattle_TestMoveSupport(u8 roster, u8 format, u16 move)
{
    struct FrontierGcBattle *saved = sBattle;
    BattleMove out;
    sBattle = AllocZeroed(sizeof(*sBattle));
    sBattle->roster = roster;
    sBattle->format = format;
    ResolveMove(move, &out);
    Free(sBattle);
    sBattle = saved;
    return out.unsupported;
}

u8 FrontierGcBattle_TestGimmickItem(u16 item, u16 species)
{
    return GimmickItemCandidate(item, species);
}

static bool8 BuildTestEntries(BattleRequest *request, const u8 **reason)
{
    u32 side, i;
    GcExtension_SetExtraMons(sBattle->opponent, 2 * sBattle->count);
    for (side = 0; side < 2; side++)
    {
        for (i = 0; i < sBattle->count; i++)
        {
            const u32 index = side * sBattle->count + i;
            sBattle->opponent[index] = sBattle->testMons[index];
            if (FillEntry(GC_EXTRA_SLOT_BASE + index, side ? &request->opponent[i] : &request->player[i]) == 0)
            {
                *reason = side ? sText_NoOpponentMoves : sText_NoMoves;
                return FALSE;
            }
        }
    }
    return TRUE;
}

void FrontierGcBattle_TestExport(const struct FrontierGcExportTest *test, struct FrontierGcExportResult *result)
{
    const u8 *reason = NULL;
    memset(result, 0, sizeof(*result));
    sPendingPolicy = RESULT_POLICY_DISPLAY_ONLY;
    sPendingTrainer = TRAINER_CALVIN_1;
    sPendingFormat = test->format;
    sPendingRoster = test->roster;
    sPendingEndurance = FALSE;
    sPendingLab = 0;
    sPendingStrictItems = test->strictItems;
    sBattle = AllocZeroed(sizeof(*sBattle));
    sBattle->format = test->format;
    sBattle->fieldCount = test->format == BATTLE_FORMAT_SINGLE ? 1 : test->format == BATTLE_FORMAT_DOUBLE ? 2 : 3;
    sBattle->roster = test->roster;
    sBattle->strictItems = test->strictItems;
    sPendingStrictItems = FALSE;
    sBattle->count = test->count;
    sBattle->policy = RESULT_POLICY_DISPLAY_ONLY;
    sBattle->trainerId = TRAINER_CALVIN_1;
    sBattle->testMons = test->mons;
    sBattle->testStrictAll = test->strictAll;
    sBattle->testAllowGimmicks = test->allowGimmicks;
    result->built = BuildRequest(&reason);
    result->refusal = reason;
    result->itemState = sBattle->itemState;
    result->itemReason = sBattle->itemReason;
    result->gimmickState = sBattle->gimmickState;
    if (result->built)
    {
        result->request = AllocZeroed(sizeof(*result->request));
        if (!battle_request_decode(sBattle->request, sBattle->requestLength, result->request))
            TRY_FREE_AND_SET_NULL(result->request);
    }
    GcExtension_SetExtraMons(NULL, 0);
    TRY_FREE_AND_SET_NULL(sBattle);
}

// A Lab fixture (lab 1..) or a debug fixture (lab 0: roster, format,
// endurance and trainer as FrontierGcBattle_Start sets them), built as Init
// builds it for the GC.
void FrontierGcBattle_TestExportFixture(u8 lab, u8 roster, u8 format, bool8 endurance, u16 trainer,
                                        struct FrontierGcExportResult *result)
{
    const u8 *reason = NULL;
    memset(result, 0, sizeof(*result));
    sPendingPolicy = RESULT_POLICY_DISPLAY_ONLY;
    sPendingTrainer = lab ? TRAINER_CALVIN_1 : trainer;
    sPendingFormat = lab ? sLabFixtures[lab].format : format;
    sPendingRoster = lab ? (sLabFixtures[lab].version ? sLabFixtures[lab].version : BATTLE_APP_VERSION_ABILITY_HOOKS) : roster;
    sPendingEndurance = endurance;
    sPendingLab = lab;
    result->built = Init(&reason);
    result->refusal = reason;
    if (result->built)
    {
        result->itemState = sBattle->itemState;
        result->itemReason = sBattle->itemReason;
        result->gimmickState = sBattle->gimmickState;
        result->request = AllocZeroed(sizeof(*result->request));
        if (!battle_request_decode(sBattle->request, sBattle->requestLength, result->request))
            TRY_FREE_AND_SET_NULL(result->request);
        FreeBattle();
    }
}

u8 FrontierGcBattle_TestLabCount(void)
{
    return LAB_FIXTURE_COUNT;
}

// test/battle/gc_ability_hooks_export.c: the ability block a request of `roster` gives `ability`.
void FrontierGcBattle_TestResolveAbility(u8 roster, u16 ability, BattleAbility *out)
{
    struct FrontierGcBattle *saved = sBattle;
    sBattle = AllocZeroed(sizeof(*sBattle));
    sBattle->roster = roster;
    *out = ResolveAbility(ability);
    Free(sBattle);
    sBattle = saved;
}

void FrontierGcBattle_TestSetLegacyExport(bool8 legacy)
{
    sTestLegacyExport = legacy;
}
#endif
