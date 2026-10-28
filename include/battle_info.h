#ifndef GUARD_BATTLE_INFO_H
#define GUARD_BATTLE_INFO_H

#include "constants/battle.h"

enum BattleInfoEffect
{
    BINFO_REFLECT,
    BINFO_LIGHT_SCREEN,
    BINFO_AURORA_VEIL,
    BINFO_TAILWIND,
    BINFO_SAFEGUARD,
    BINFO_MIST,
    BINFO_LUCKY_CHANT,
    BINFO_RAINBOW,
    BINFO_SEA_OF_FIRE,
    BINFO_SWAMP,
    BINFO_DAMAGE_NON_TYPES,
    BINFO_WEATHER,
    BINFO_TERRAIN,
    BINFO_TRICK_ROOM,
    BINFO_WONDER_ROOM,
    BINFO_MAGIC_ROOM,
    BINFO_GRAVITY,
    BINFO_MUD_SPORT,
    BINFO_WATER_SPORT,
    BINFO_FAIRY_LOCK,
};

struct BattleInfoTurns
{
    u16 elapsed;
    u16 total;
    bool8 active;
    bool8 permanent;
};

void BattleInfo_Reset(void);
void BattleInfo_Free(void);
void BattleInfo_RecordDuration(enum BattleInfoEffect effect, enum BattleSide side, u32 duration);
void BattleInfo_SwapSides(void);
void BattleInfo_GetTurns(enum BattleInfoEffect effect, enum BattleSide side, struct BattleInfoTurns *turns);
bool32 BattleInfo_HandleInput(enum BattlerId battler, bool32 canOpen);
void BattleInfo_HideSprites(void);

#if TESTING
bool32 Test_BattleInfoOpen(enum BattlerId battler);
void Test_BattleInfoClose(void);
u32 Test_BattleInfoWindow(void);
void Test_BattleInfoPage(u32 page);
enum BattlerId Test_BattleInfoSelected(u32 column);
#endif

#endif
