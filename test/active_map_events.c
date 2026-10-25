#include "global.h"
#include "event_data.h"
#include "heal_location.h"
#include "script.h"
#include "test/test.h"
#include "constants/heal_locations.h"
#include "constants/maps.h"

extern const u8 CableClub_EventScript_HideOrShowMysteryGiftMan[];

TEST("Active maps: Frontier whiteout uses its retained Pokemon Center nurse")
{
    struct WarpData previous = gSaveBlock1Ptr->lastHealLocation;
    struct WarpData warp;
    const struct HealLocation *heal = GetHealLocation(HEAL_LOCATION_BATTLE_FRONTIER_OUTSIDE_EAST);
    u16 npc = GetHealNpcLocalId(HEAL_LOCATION_BATTLE_FRONTIER_OUTSIDE_EAST);

    EXPECT_NE(npc, 0);
    gSaveBlock1Ptr->lastHealLocation = (struct WarpData){heal->mapGroup, heal->mapNum, WARP_ID_NONE, heal->x, heal->y};
    SetWhiteoutRespawnWarpAndHealerNPC(&warp);
    gSaveBlock1Ptr->lastHealLocation = previous;
    EXPECT_EQ(warp.mapGroup, MAP_GROUP(MAP_BATTLE_FRONTIER_POKEMON_CENTER_1F));
    EXPECT_EQ(warp.mapNum, MAP_NUM(MAP_BATTLE_FRONTIER_POKEMON_CENTER_1F));
    EXPECT_EQ(gSpecialVar_LastTalked, npc);
    EXPECT_EQ(gSpecialVar_0x800B, npc);
}

TEST("Active maps: disabled Mystery Gift hides the courier even with a pending ticket")
{
    ASSUME(FREE_MYSTERY_GIFT || FREE_MYSTERY_EVENT_BUFFERS);
    VarSet(VAR_DISTRIBUTE_EON_TICKET, 1);
    FlagClear(FLAG_HIDE_POKEMON_CENTER_2F_MYSTERY_GIFT_MAN);
    RunScriptImmediately(CableClub_EventScript_HideOrShowMysteryGiftMan);
    EXPECT(FlagGet(FLAG_HIDE_POKEMON_CENTER_2F_MYSTERY_GIFT_MAN));
    VarSet(VAR_DISTRIBUTE_EON_TICKET, 0);
}

