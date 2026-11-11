// Vendored from gba-gc-extension protocol/pkmn_protocol.h. Do not edit here;
// change the extension repository and run tools/sync-pokeemerald.ps1.
#ifndef GCEXT_PKMN_PROTOCOL_H
#define GCEXT_PKMN_PROTOCOL_H

// Pokemon application protocol, carried inside generic APP frames
// (message.h). It defines wire records only; it never mirrors a game struct.
// All fields are little-endian with fixed offsets.

#include <stdbool.h>
#include <stdint.h>
#include "gc_ext/message.h"
#ifdef __cplusplus
extern "C" {
#endif

#define APP_POKEMON 0x4B50          // bytes 'P','K'
#define PKMN_APP_VERSION 1

enum PkmnOp {
    PKMN_PARTY_INFO = 1,  // -> PartyInfo
    PKMN_RECORD = 2,      // u8 slot -> PokemonRecordV1
    PKMN_UI_STATE = 3,    // -> UiState (polled; also the keepalive)
    PKMN_SET_PHASE = 4,   // u8 phase, u8 done, u8 total: host progress shown on the GBA
    PKMN_STATE_DIGEST = 5 // -> StateDigest (save-safety check)
};

// Asset types served by the Pokemon client.
enum {
    ASSET_PKMN_FRONT = 0x0101,  // id = species, variant = PKMN_SPRITE_* bits, extra = personality if it changes pixels
    ASSET_TEXT_LABEL = 0x0102,  // id = LABEL_ID(kind, value), variant = CRC of the string bytes for nicknames
};
#define PKMN_SPRITE_SHINY  (1u << 0)
#define PKMN_SPRITE_FEMALE (1u << 1)  // only set if the species has female-specific graphics
#define PKMN_SPRITE_EGG    (1u << 2)

// Text labels are rendered by the game's own font on the GBA and sent as
// 4bpp images, so the host needs no game charset or font.
enum LabelKind { LABEL_SPECIES = 1, LABEL_MOVE = 2, LABEL_ABILITY = 3, LABEL_ITEM = 4, LABEL_NICKNAME = 5 };
#define LABEL_ID(kind, value) ((uint32_t)(kind) << 24 | ((uint32_t)(value) & 0xffffffu))
#define LABEL_KIND(id) ((id) >> 24)
#define LABEL_VALUE(id) ((id) & 0xffffffu)

enum PkmnPhase { PHASE_CONNECTING = 0, PHASE_PARTY = 1, PHASE_ASSETS = 2, PHASE_READY = 3 };
enum PkmnScreen { SCREEN_MENU = 0, SCREEN_PARTY = 1 };
#define UI_EXIT_REQUESTED (1u << 0)
#define PKMN_NO_SLOT 0xff

// PARTY_INFO reply (8 bytes):
//   0 u8 party count  1 u8 record version  2 u16 record length
//   4 u8 cursor slot  5 u8 confirmed slot (0xff none)  6 u8 screen  7 u8 ui flags
#define PARTY_INFO_BYTES 8
// UI_STATE reply (8 bytes):
//   0 u8 cursor slot  1 u8 confirmed slot  2 u8 screen  3 u8 ui flags
//   4 u16 input sequence (increments on every accepted GBA input)  6 u16 reserved
#define UI_STATE_BYTES 8
typedef struct {
    uint8_t count, record_version;
    uint16_t record_length;
    uint8_t cursor, confirmed, screen, flags;
    uint16_t input_seq;
} PartyUi;

// STATE_DIGEST reply (24 bytes): current CRC-32 of the player party (and
// count), bag (decrypted item/quantity pairs), event flags, vars and the raw
// SaveBlock1. The game re-keys encrypted fields and moves SaveBlock1 on map
// loads, so the raw SaveBlock1 CRC is only comparable within one mode visit.
//   0 u32 party  4 u32 bag  8 u32 flags  12 u32 vars  16 u32 save block 1
//  20 u8 match bits (bit n set = field n identical to the mode-entry snapshot)
//  21..23 reserved
#define STATE_DIGEST_BYTES 24
#define STATE_DIGEST_FIELDS 5
#define STATE_DIGEST_ALL_MATCH ((1u << STATE_DIGEST_FIELDS) - 1)

// PokemonRecordV1, 100 bytes.
//  off size field
//    0  1   record version (1)          1  1   party slot
//    2  2   record length (100)
//    4  4   personality                 8  4   OT id
//   12  2   species (game species id; forms are species ids)
//   14  2   national dex number
//   16  1   form index                 17  1   level
//   18  1   nature                     19  1   tera type
//   20  1   gender (0 male, 1 female, 2 genderless)
//   21  1   flags (bit0 shiny, bit1 egg)
//   22  1   ability slot               23  1   friendship
//   24  2   current HP                 26  2   max HP
//   28  2   attack  30 2 defense  32 2 speed  34 2 sp. attack  36 2 sp. defense
//   38  2   ability id                 40  2   held item id
//   42  2   reserved (0)
//   44  4   status condition (game STATUS1 bits)
//   48  8   move[4] (u16 each)
//   56  4   pp[4]                      60  4   max pp[4]
//   64  1   language                   65  1   name encoding (1 = pokeemerald charmap)
//   66  1   nickname length (bytes)    67  1   reserved (0)
//   68 16   nickname, raw game encoding, 0xff padded
//   84 16   front sprite AssetKey (resolved by the client)
#define PKMN_RECORD_VERSION 1
#define PKMN_RECORD_BYTES 100
#define PKMN_NICKNAME_BYTES 16
#define PKMN_ENCODING_POKEEMERALD 1
#define PKMN_FLAG_SHINY (1u << 0)
#define PKMN_FLAG_EGG   (1u << 1)
typedef struct {
    uint8_t version, slot;
    uint16_t length;
    uint32_t personality, ot_id;
    uint16_t species, national_dex;
    uint8_t form, level, nature, tera_type, gender, flags, ability_slot, friendship;
    uint16_t hp, max_hp, attack, defense, speed, sp_attack, sp_defense;
    uint16_t ability, held_item;
    uint32_t status;
    uint16_t moves[4];
    uint8_t pp[4], max_pp[4];
    uint8_t language, encoding, nickname_length;
    uint8_t nickname[PKMN_NICKNAME_BYTES];
    AssetKey sprite;
} PokemonRecord;

unsigned pkmn_record_encode(const PokemonRecord *record, uint8_t *out);
bool pkmn_record_decode(const uint8_t *in, unsigned length, PokemonRecord *record);
unsigned party_ui_encode(const PartyUi *ui, bool party_info, uint8_t *out);
bool party_ui_decode(const uint8_t *in, unsigned length, bool party_info, PartyUi *ui);
// Key of the nickname label for a record (variant = CRC-32 of the raw name).
AssetKey pkmn_nickname_label(const PokemonRecord *record);

#ifdef __cplusplus
}
#endif
#endif
