#include "global.h"
#include "event_data.h"
#include "frontier_tutor.h"
#include "item.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "test/test.h"
#include "constants/abilities.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/moves.h"

void Special_SetupFrontierHubState(void);

static const struct
{
    enum Species species;
    enum Item item;
    enum Ability ability;
    u8 nature;
    enum Type teraType;
    u8 trainedStat;
} sGifts[] =
{
    { SPECIES_SWAMPERT, ITEM_SWAMPERTITE, ABILITY_TORRENT, NATURE_ADAMANT, TYPE_WATER, STAT_ATK },
    { SPECIES_URSHIFU_RAPID_STRIKE, ITEM_FOCUS_SASH, ABILITY_UNSEEN_FIST, NATURE_JOLLY, TYPE_WATER, STAT_ATK },
    { SPECIES_BLAZIKEN, ITEM_BLAZIKENITE, ABILITY_SPEED_BOOST, NATURE_ADAMANT, TYPE_FIRE, STAT_ATK },
    { SPECIES_TALONFLAME, ITEM_HEAVY_DUTY_BOOTS, ABILITY_GALE_WINGS, NATURE_JOLLY, TYPE_FLYING, STAT_ATK },
    { SPECIES_SCEPTILE, ITEM_SCEPTILITE, ABILITY_UNBURDEN, NATURE_TIMID, TYPE_DRAGON, STAT_SPATK },
    { SPECIES_SINISTCHA, ITEM_LEFTOVERS, ABILITY_HOSPITALITY, NATURE_BOLD, TYPE_WATER, STAT_DEF },
    { SPECIES_GARDEVOIR, ITEM_GARDEVOIRITE, ABILITY_TRACE, NATURE_TIMID, TYPE_FAIRY, STAT_SPATK },
    { SPECIES_MIMIKYU, ITEM_MIMIKIUM_Z, ABILITY_DISGUISE, NATURE_ADAMANT, TYPE_GHOST, STAT_ATK },
    { SPECIES_RAYQUAZA, ITEM_LIFE_ORB, ABILITY_AIR_LOCK, NATURE_JOLLY, TYPE_NORMAL, STAT_ATK },
};

static void PrepareInitialParty(void)
{
    ClearBag();
    ZeroPlayerPartyMons();
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            ZeroBoxMonAt(box, slot);
    FlagClear(FLAG_RECEIVED_ULTRA_FRONTIE_HUB_INITIAL_PARTY);
}

static bool32 IsLearnable(enum Species species, enum Move move)
{
    const struct LevelUpMove *learnset = GetSpeciesLevelUpLearnset(species);
    const u16 *frontierMoves = GetFrontierFullLearnset(species);
    for (u32 i = 0; learnset[i].move != LEVEL_UP_MOVE_END; i++)
        if (learnset[i].move == move)
            return TRUE;
    if (frontierMoves != NULL)
        for (u32 i = 0; frontierMoves[i] != MOVE_UNAVAILABLE; i++)
            if (frontierMoves[i] == move)
                return TRUE;
    return CanLearnTeachableMove(species, move);
}

TEST("Frontier initial party: all sixteen combinations contain four partners and Rayquaza")
{
    u32 choices = 0;
    for (u32 bits = 0; bits < 16; bits++)
        PARAMETRIZE { choices = bits; }

    PrepareInitialParty();
    SET_RNG(RNG_NONE, choices);
    Special_SetupFrontierHubState();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT(FlagGet(FLAG_RECEIVED_ULTRA_FRONTIE_HUB_INITIAL_PARTY));
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 5);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][5], MON_DATA_SPECIES), SPECIES_NONE);

    for (u32 slot = 0; slot < 5; slot++)
    {
        u32 expected = slot == 4 ? 8 : slot * 2 + ((choices >> slot) & 1);
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][slot];
        enum Species species = GetMonData(mon, MON_DATA_SPECIES);
        EXPECT_EQ(species, sGifts[expected].species);
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 100);
        EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), sGifts[expected].item);
        EXPECT_EQ(GetMonAbility(mon), sGifts[expected].ability);
        EXPECT_EQ(GetNature(mon), sGifts[expected].nature);
        EXPECT_EQ(GetMonData(mon, MON_DATA_TERA_TYPE), sGifts[expected].teraType);
        EXPECT_EQ(GetMonData(mon, MON_DATA_DYNAMAX_LEVEL), 10);
        EXPECT_EQ(GetMonData(mon, MON_DATA_GIGANTAMAX_FACTOR), species == SPECIES_URSHIFU_RAPID_STRIKE);
        EXPECT_EQ(GetMonData(mon, MON_DATA_FRIENDSHIP), MAX_FRIENDSHIP);
        EXPECT_EQ(GetMonData(mon, MON_DATA_STATUS), 0);
        EXPECT_EQ(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
        EXPECT_EQ(GetMonData(mon, MON_DATA_PP_BONUSES), 0xFF);

        for (u32 stat = 0; stat < NUM_STATS; stat++)
        {
            u32 ev = stat == sGifts[expected].trainedStat ? 252 : 0;
            if (species == SPECIES_SINISTCHA)
            {
                if (stat == STAT_HP) ev = 252;
                if (stat == STAT_SPDEF) ev = 4;
            }
            else
            {
                if (stat == STAT_SPEED) ev = 252;
                if (stat == STAT_HP) ev = 4;
            }
            EXPECT_EQ(GetMonData(mon, MON_DATA_HP_IV + stat), 31);
            EXPECT_EQ(GetMonData(mon, MON_DATA_HP_EV + stat), ev);
        }
        for (u32 moveSlot = 0; moveSlot < MAX_MON_MOVES; moveSlot++)
        {
            enum Move move = GetMonData(mon, MON_DATA_MOVE1 + moveSlot);
            EXPECT_NE(move, MOVE_NONE);
            EXPECT(IsLearnable(species, move));
            EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + moveSlot), CalculatePPWithBonus(move, 0xFF, moveSlot));
        }
    }
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][4], MON_DATA_MOVE1), MOVE_DRAGON_ASCENT);
    if (choices & 8)
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][3], MON_DATA_MOVE1), MOVE_PLAY_ROUGH);
}

TEST("Frontier initial party: revisiting preserves an already received party")
{
    struct Pokemon before[PARTY_SIZE];
    PrepareInitialParty();
    SET_RNG(RNG_NONE, 0);
    Special_SetupFrontierHubState();
    memcpy(before, gParties[B_TRAINER_PLAYER], sizeof(before));
    SET_RNG(RNG_NONE, 15);
    Special_SetupFrontierHubState();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 5);
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
    EXPECT_EQ(CountStorageNonEggMons(), 0);
}

TEST("Frontier initial party: insufficient storage gives no partial gift and retry gives exactly five")
{
    struct Pokemon before[PARTY_SIZE];
    PrepareInitialParty();
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
        CreateRandomMon(&gParties[B_TRAINER_PLAYER][slot], SPECIES_MAGIKARP, 5);
    CalculatePlayerPartyCount();
    memcpy(before, gParties[B_TRAINER_PLAYER], sizeof(before));
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
        for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
            if (box != 0 || slot >= 4)
                SetBoxMonAt(box, slot, &before[0].box);

    Special_SetupFrontierHubState();
    EXPECT_EQ(gSpecialVar_Result, FALSE);
    EXPECT(!FlagGet(FLAG_RECEIVED_ULTRA_FRONTIE_HUB_INITIAL_PARTY));
    for (u32 slot = 0; slot < 4; slot++)
        EXPECT_EQ(GetBoxMonDataAt(0, slot, MON_DATA_SPECIES), SPECIES_NONE);
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);

    ZeroBoxMonAt(0, 4);
    SET_RNG(RNG_NONE, 0);
    Special_SetupFrontierHubState();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
    EXPECT(FlagGet(FLAG_RECEIVED_ULTRA_FRONTIE_HUB_INITIAL_PARTY));
    for (u32 slot = 0; slot < 5; slot++)
        EXPECT_EQ(GetBoxMonDataAt(0, slot, MON_DATA_SPECIES), sGifts[slot == 4 ? 8 : slot * 2].species);
    EXPECT_EQ(memcmp(before, gParties[B_TRAINER_PLAYER], sizeof(before)), 0);
}
