// Vendored from gba-gc-extension protocol/battle_protocol.h. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#ifndef GCEXT_BATTLE_PROTOCOL_H
#define GCEXT_BATTLE_PROTOCOL_H

// Battle application protocol (GC_MODE_BATTLE). The GBA sends one request
// with everything the GameCube battle core needs, resolved from its own game
// data; the GameCube owns the temporary battle state, RNG and turn
// resolution and returns a versioned result. All fields are little-endian.
// Only SINGLE is implemented by the current battle core; the format field
// already reserves DOUBLE, TRIPLE and ROTATION.

#include <stdbool.h>
#include <stdint.h>
#include "gc_ext/pkmn_protocol.h"
#ifdef __cplusplus
extern "C" {
#endif

#define APP_GC_BATTLE 0x4247      // bytes 'G','B'
#define BATTLE_APP_VERSION 1
#define BATTLE_MODE_VERSION 1

enum BattleFormat { BATTLE_FORMAT_SINGLE = 1, BATTLE_FORMAT_DOUBLE = 2, BATTLE_FORMAT_TRIPLE = 3, BATTLE_FORMAT_ROTATION = 4 };
enum BattleKind { BTYPE_TRAINER = 1 };  // prefixed: games define BATTLE_TYPE_* flags
enum BattleResultPolicy { RESULT_POLICY_DISPLAY_ONLY = 0, RESULT_POLICY_APPLY_HP_PP = 1 };

// Rules: mechanics the request explicitly normalises away for this battle.
#define RULE_IGNORE_ABILITIES  (1u << 0)
#define RULE_IGNORE_ITEMS      (1u << 1)
#define RULE_IGNORE_STATUS     (1u << 2)  // existing status is carried but has no effect
#define RULE_NO_GIMMICKS       (1u << 3)  // no Mega, Z-Move, Dynamax, Tera
#define RULE_NO_CRITICAL_HITS  (1u << 4)
#define RULE_SUPPORTED_MOVES_ONLY (1u << 5)  // unsupported moves cannot be selected

#define BATTLE_MAX_PER_SIDE 6
#define BATTLE_MAX_TYPES 32

// Move data, 8 bytes, resolved by the GBA from its move table.
//   0 u16 power   2 u8 type   3 u8 category   4 u8 accuracy (0 = never misses)
//   5 s8 priority 6 u8 target  7 u8 unsupported reasons (0 = supported)
enum BattleCategory { BCAT_STATUS = 0, BCAT_PHYSICAL = 1, BCAT_SPECIAL = 2 };
// ALL_FOES = spread moves (both foes / foes and ally). In SINGLE there is one
// foe and no ally, so they hit that foe at full power; other formats will need
// spread damage and ally hits before accepting them.
enum BattleTarget { BTARGET_OTHER = 0, BTARGET_SINGLE = 1, BTARGET_ALL_FOES = 2 };
#define UNSUP_EFFECT      (1u << 0)  // not a plain damaging effect
#define UNSUP_SECONDARY   (1u << 1)  // has additional effects
#define UNSUP_PRIORITY    (1u << 2)
#define UNSUP_TARGET      (1u << 3)
#define UNSUP_STATUS_MOVE (1u << 4)
#define UNSUP_MULTI_HIT   (1u << 5)
#define UNSUP_OTHER       (1u << 6)  // fixed crits, recoil-like flags, explosion ...
#define UNSUP_EMPTY       (1u << 7)  // no move in this slot
typedef struct {
    uint16_t power;
    uint8_t type, category, accuracy;
    int8_t priority;
    uint8_t target, unsupported;
} BattleMove;

// BattleMonV1, 40 bytes, appended to each PokemonRecordV1 in the request.
//   0 u8 version (1)  1 u8 type count  2 u8 types[2]
//   4 s8 front pic y offset  5 s8 back pic y offset  6 u8 elevation (opponent)  7 u8 reserved
//   8 BattleMove[4]
// The picture offsets place sprites exactly where the game would.
#define BATTLE_MON_VERSION 1
#define BATTLE_MON_BYTES 40
typedef struct {
    uint8_t type_count, types[2];
    int8_t front_y, back_y;
    uint8_t elevation;
    BattleMove moves[4];
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
#define BATTLE_REQUEST_HEADER 24
typedef struct {
    uint8_t format, battle_type, player_count, opponent_count, active_per_side;
    uint16_t rules, trainer_id;
    uint8_t trainer_class, reward_mode, result_policy, type_count;
    uint32_t battle_id;
    uint8_t environment;
    BattleEntry player[BATTLE_MAX_PER_SIDE], opponent[BATTLE_MAX_PER_SIDE];
    uint8_t type_chart[BATTLE_MAX_TYPES * BATTLE_MAX_TYPES];
} BattleRequest;

uint32_t battle_request_size(const BattleRequest *request);
uint32_t battle_request_encode(const BattleRequest *request, uint8_t *out, uint32_t capacity);
bool battle_request_decode(const uint8_t *in, uint32_t length, BattleRequest *request);

// Battle presentation assets served by the client from its own battle
// graphics, so the host draws the game's real UI (types 0x02xx).
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
};

// BattleState, 16 bytes.
//   0 u8 phase   1 u8 turn   2 u8 outcome (when ENDED)  3 u8 player slot
//   4 u16 player hp   6 u16 player max hp   8 u16 opponent hp   10 u16 opponent max hp
//  12 u8 pp[4] of the active player battler
enum BattlePhase { BPHASE_WAIT = 1, BPHASE_CHOOSE = 2, BPHASE_ENDED = 3 };
#define BATTLE_STATE_BYTES 16
typedef struct {
    uint8_t phase, turn, outcome, player_slot;
    uint16_t player_hp, player_max_hp, opponent_hp, opponent_max_hp;
    uint8_t pp[4];
} BattleState;

// BattleAction, 4 bytes: u8 turn, u8 action, u8 move slot, u8 flags.
// The turn echoes the CHOOSE state it answers, so stale input is ignored.
enum BattleActionKind { BACTION_NONE = 0, BACTION_MOVE = 1, BACTION_FORFEIT = 2 };
#define BACTION_FLAG_EXIT (1u << 0)
#define BATTLE_ACTION_BYTES 4
typedef struct {
    uint8_t turn, action, move_slot, flags;
} BattleAction;

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
