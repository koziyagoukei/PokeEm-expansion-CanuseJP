#ifndef GUARD_CONFIG_SHADOW_POKEMON_H
#define GUARD_CONFIG_SHADOW_POKEMON_H

// Shadow Pokemon (docs/shadow_pokemon.md). The heart gauge follows Pokemon XD, Hyper Mode Pokemon Colosseum.

// Full heart gauge and aggression (1 very high .. 5 very low) of a species with no row in src/data/shadow_pokemon.h
// (or a row that leaves them 0).
#define SHADOW_DEFAULT_HEART_GAUGE 3000
#define SHADOW_DEFAULT_AGGRESSION  3

// A species with no Shadow moves of its own gets them from its base stats (src/shadow_pokemon.c, GetDefaultShadowMoves):
// from STRONG_BST a harder hit and a support move, from RARE_BST the strongest ones. The same lines as the Battle
// Connector's strong and rare rooms.
#define SHADOW_DEFAULT_STRONG_BST 480
#define SHADOW_DEFAULT_RARE_BST   535

// A Shadow Pokemon gains no EXP. This % of it is held and given at purification (Pokemon XD: 80).
#define SHADOW_HELD_EXP_PERCENT 80

// A Shadow Pokemon's own moves (after its Shadow moves) open one by one as its heart opens: the first with 1 bar of
// the heart gauge cleared, the second with 3 (Pokemon XD). The others stay sealed until purification.
#define SHADOW_OWN_MOVE_1_STAGE 1
#define SHADOW_OWN_MOVE_2_STAGE 3

// Purification (Pokemon XD's Purify Chamber): a Shadow Pokemon with an empty heart gauge is purified when it is talked
// to in the camp with this many other Pokemon in the party that are not Shadow Pokemon (nor Eggs).
#define SHADOW_PURIFY_PARTNERS 4

// Each PARTY_BATH_INTERVAL (5 seconds) in the party bath lowers the heart gauge of each Shadow Pokemon in the party by
// this much. (Walking all the time lowers it by about 7 every 5 seconds.) A meal at the dining room lowers it like a
// scent (SHADOW_ACTION_SCENT).
#define SHADOW_BATH_GAUGE_REDUCTION 10

// Hyper Mode (Colosseum): % chance of going into it when a Shadow move is chosen, by nature. Colosseum names the
// natures that go into it more and less readily but not the numbers, so these are this game's own.
#define SHADOW_HYPER_MODE_CHANCE_HIGH   30
#define SHADOW_HYPER_MODE_CHANCE_NORMAL 20
#define SHADOW_HYPER_MODE_CHANCE_LOW    10

// Hyper Mode raises the critical hit stage of Shadow moves by this much (Colosseum gives no number).
#define SHADOW_HYPER_MODE_CRIT_STAGES 2

#endif
