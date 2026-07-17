#include "global.h"
#include "pokemon.h"
#include "contest.h"
#include "link.h"
#include "config/link_compat.h"

STATIC_ASSERT(sizeof(struct BoxPokemon) == LINK_SIZEOF_BOX_POKEMON, BoxPokemonLinkLayoutChanged);
STATIC_ASSERT(sizeof(struct Pokemon) == LINK_SIZEOF_POKEMON, PokemonLinkLayoutChanged);
STATIC_ASSERT(sizeof(struct BattlePokemon) == LINK_SIZEOF_BATTLE_POKEMON, BattlePokemonLinkLayoutChanged);
STATIC_ASSERT(sizeof(struct ContestPokemon) == LINK_SIZEOF_CONTEST_POKEMON, ContestPokemonLinkLayoutChanged);
STATIC_ASSERT(sizeof(struct LinkPlayer) == LINK_SIZEOF_LINK_PLAYER, LinkPlayerLinkLayoutChanged);
STATIC_ASSERT(sizeof(struct LinkPlayerBlock) == LINK_SIZEOF_LINK_PLAYER_BLOCK, LinkPlayerBlockLinkLayoutChanged);
