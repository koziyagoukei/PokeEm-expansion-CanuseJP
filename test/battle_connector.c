#include "global.h"
#include "battle_connector.h"
#include "battle_frontier.h"
#include "frontier_tutor.h"
#include "battle_main.h"
#include "data.h"
#include "event_data.h"
#include "frontier_util.h"
#include "item_use.h"
#include "malloc.h"
#include "move_relearner.h"
#include "party_menu.h"
#include "load_save.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "shadow_pokemon.h"
#include "string_util.h"
#include "constants/characters.h"
#include "test/battle.h"
#include "constants/battle_connector.h"
#include "constants/battle_frontier_mons.h"
#include "constants/opponents.h"
#include "constants/region_map_sections.h"
#include "constants/songs.h"

static void GiveParty(u32 count)
{
    static const u16 species[PARTY_SIZE] = { SPECIES_MEWTWO, SPECIES_PIKACHU, SPECIES_EEVEE, SPECIES_ZIGZAGOON, SPECIES_RALTS, SPECIES_LOTAD };
    ZeroPlayerPartyMons();
    for (u32 i = 0; i < count; i++)
        CreateMon(&gParties[B_TRAINER_PLAYER][i], species[i], 50, i, OTID_STRUCT_PLAYER_ID);
    CalculatePlayerPartyCount();
}

static void StartWith(u32 slot)
{
    gSpecialVar_0x8004 = slot;
    gSpecialVar_0x8005 = PARTY_SIZE;
    EXPECT(BattleConnector_StartChallenge());
}

// A defeated Shadow Pokemon, as a room's battle leaves it
static void DefeatShadowMon(u16 species)
{
    BattleConnector_Test_NewBattle();
    ZeroEnemyPartyMons();
    BattleConnector_CreateShadowMon(&gParties[B_TRAINER_OPPONENT_A][0], species, 30);
    CalculateEnemyPartyCount();
}

TEST("Battle Connector: the player brings one Pokemon, of any species, and gets the party back")
{
    GiveParty(6);
    StartWith(0); // Mewtwo: no species rules
    EXPECT(BattleConnector_IsChallengeActive());
    EXPECT_EQ(CalculatePlayerPartyCount(), 1);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_MEWTWO);
    BattleConnector_RetireChallenge();
    EXPECT(!BattleConnector_IsChallengeActive());
    EXPECT_EQ(CalculatePlayerPartyCount(), 6);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][5], MON_DATA_SPECIES), SPECIES_LOTAD);
    EXPECT_EQ(BattleConnector_GetReturnState(), CONNECTOR_RETURN_RETIRED);
    EXPECT_EQ(BattleConnector_GetReturnState(), CONNECTOR_RETURN_NONE);

    GiveParty(3);
    StartWith(2);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_EEVEE);
    BattleConnector_RetireChallenge();
    gSpecialVar_0x8004 = 4; // an empty slot
    gSpecialVar_0x8005 = PARTY_SIZE;
    EXPECT(!BattleConnector_StartChallenge());
    EXPECT(!BattleConnector_IsChallengeActive());
}

TEST("Battle Connector: the player may bring a second Pokemon, but not the same one twice")
{
    GiveParty(6);
    gSpecialVar_0x8004 = 3;
    gSpecialVar_0x8005 = 1;
    EXPECT(BattleConnector_StartChallenge());
    EXPECT_EQ(CalculatePlayerPartyCount(), 2);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_SPECIES), SPECIES_ZIGZAGOON);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), SPECIES_PIKACHU);
    BattleConnector_RetireChallenge();
    EXPECT_EQ(CalculatePlayerPartyCount(), 6);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), SPECIES_PIKACHU);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][3], MON_DATA_SPECIES), SPECIES_ZIGZAGOON);

    gSpecialVar_0x8004 = 2;
    gSpecialVar_0x8005 = 2;
    EXPECT(!BattleConnector_StartChallenge());
    EXPECT(!BattleConnector_IsChallengeActive());
}

TEST("Battle Connector: a branch rolls three paths once, and the rooms follow the run's structure")
{
    u32 nextRoom;

    GiveParty(1);
    StartWith(0);
    for (u32 branch = 0; branch < BATTLE_CONNECTOR_BRANCHES; branch++)
    {
        u32 paths[CONNECTOR_PATH_COUNT];
        BattleConnector_PrepareBranch();
        for (u32 path = 0; path < CONNECTOR_PATH_COUNT; path++)
        {
            paths[path] = BattleConnector_Test_GetPathRoomType(path);
            EXPECT_GT(paths[path], CONNECTOR_ROOM_NONE);
            EXPECT_LE(paths[path], CONNECTOR_ROOM_RANDOM);
            if (branch == 0) // the later room types do not come yet
                EXPECT(paths[path] != CONNECTOR_ROOM_STRONG_SHADOW && paths[path] != CONNECTOR_ROOM_RARE_SHADOW
                    && paths[path] != CONNECTOR_ROOM_STRONG_TRAINER && paths[path] != CONNECTOR_ROOM_HEAL);
            gSpecialVar_0x8004 = path;
            BattleConnector_BufferPathHint();
            EXPECT(gStringVar1[0] != EOS);
        }
        BattleConnector_PrepareBranch(); // rolled once
        for (u32 path = 0; path < CONNECTOR_PATH_COUNT; path++)
            EXPECT_EQ(BattleConnector_Test_GetPathRoomType(path), paths[path]);

        gSpecialVar_0x8004 = CONNECTOR_PATH_CENTER;
        BattleConnector_EnterPath();
        if (paths[CONNECTOR_PATH_CENTER] != CONNECTOR_ROOM_RANDOM)
            EXPECT_EQ(BattleConnector_GetRoomType(), paths[CONNECTOR_PATH_CENTER]);
        EXPECT(BattleConnector_GetRoomType() != CONNECTOR_ROOM_RANDOM);

        nextRoom = BattleConnector_LeaveRoom();
        if (branch + 1 == BATTLE_CONNECTOR_MIDBOSS_AFTER)
        {
            EXPECT_EQ(nextRoom, CONNECTOR_NEXT_ROOM);
            EXPECT_EQ(BattleConnector_GetRoomType(), CONNECTOR_ROOM_MIDBOSS);
            nextRoom = BattleConnector_LeaveRoom();
        }
        if (branch + 1 < BATTLE_CONNECTOR_BRANCHES)
            EXPECT_EQ(nextRoom, CONNECTOR_NEXT_BRANCH);
    }
    EXPECT_EQ(nextRoom, CONNECTOR_NEXT_ROOM);
    EXPECT_EQ(BattleConnector_GetRoomType(), CONNECTOR_ROOM_BOSS);
    EXPECT_EQ(BattleConnector_LeaveRoom(), CONNECTOR_NEXT_RESULTS);
    EXPECT_EQ(BattleConnector_GetReturnState(), CONNECTOR_RETURN_CLEARED);
    BattleConnector_TakeNothingHome();
}

TEST("Battle Connector: a run leaves the Battle Pike and the other Frontier facilities untouched")
{
    u32 before = 0, after = 0;
    const u8 *frontier = (const u8 *)&gSaveBlock2Ptr->frontier;

    for (u32 i = 0; i < sizeof(gSaveBlock2Ptr->frontier); i++)
        before = before * 31 + frontier[i];
    GiveParty(2);
    StartWith(1);
    u32 next = CONNECTOR_NEXT_BRANCH;
    for (u32 room = 0; room < 16 && next != CONNECTOR_NEXT_RESULTS; room++)
    {
        if (next == CONNECTOR_NEXT_BRANCH)
        {
            BattleConnector_PrepareBranch();
            gSpecialVar_0x8004 = room % CONNECTOR_PATH_COUNT;
            BattleConnector_EnterPath();
        }
        DefeatShadowMon(SPECIES_ZUBAT);
        if (BattleConnector_BufferDefeatedShadowMon())
            BattleConnector_ProtectDefeatedMon();
        next = BattleConnector_LeaveRoom();
    }
    EXPECT_EQ(next, CONNECTOR_NEXT_RESULTS);
    gSpecialVar_0x8004 = 0;
    BattleConnector_TakeHomeCandidate();
    for (u32 i = 0; i < sizeof(gSaveBlock2Ptr->frontier); i++)
        after = after * 31 + frontier[i];
    EXPECT_EQ(before, after);
}

TEST("Battle Connector: its Shadow Pokemon have at least three perfect IVs and carry the facility's mark")
{
    struct Pokemon mon;

    for (u32 i = 0; i < 24; i++)
    {
        u32 perfect = 0;
        BattleConnector_CreateShadowMon(&mon, SPECIES_ZIGZAGOON, 30);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
            perfect += GetMonData(&mon, MON_DATA_HP_IV + stat) == MAX_PER_STAT_IVS;
        EXPECT_GE(perfect, BATTLE_CONNECTOR_PERFECT_IVS);
        EXPECT(IsShadowPokemon(&mon));
        EXPECT(GetMonData(&mon, MON_DATA_BATTLE_CONNECTOR_ORIGIN));
        EXPECT_EQ(GetMonData(&mon, MON_DATA_MET_LOCATION), MAPSEC_ARTISAN_CAVE);
        EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 30);
    }
    // An ordinary Pokemon does not carry it
    CreateMon(&mon, SPECIES_ZIGZAGOON, 30, 0, OTID_STRUCT_PLAYER_ID);
    EXPECT(!GetMonData(&mon, MON_DATA_BATTLE_CONNECTOR_ORIGIN));
}

TEST("Battle Connector: defeated Shadow Pokemon can be protected, join the party up to its size, and swap in")
{
    GiveParty(1);
    StartWith(0);
    for (u32 i = 0; i < BATTLE_CONNECTOR_PARTY_SIZE - 1; i++)
    {
        DefeatShadowMon(SPECIES_ZUBAT + i);
        EXPECT(BattleConnector_BufferDefeatedShadowMon());
        EXPECT_EQ(BattleConnector_ProtectDefeatedMon(), CONNECTOR_PROTECT_JOINED);
        EXPECT(!BattleConnector_BufferDefeatedShadowMon()); // each is offered once
    }
    EXPECT_EQ(CalculatePlayerPartyCount(), BATTLE_CONNECTOR_PARTY_SIZE);
    struct Pokemon *joined = &gParties[B_TRAINER_PLAYER][1];
    EXPECT(IsShadowPokemon(joined));
    EXPECT_EQ(GetMonData(joined, MON_DATA_HP), GetMonData(joined, MON_DATA_MAX_HP));
    EXPECT_EQ(GetMonData(joined, MON_DATA_OT_ID), GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_OT_ID));

    DefeatShadowMon(SPECIES_GEODUDE);
    EXPECT(BattleConnector_BufferDefeatedShadowMon());
    EXPECT_EQ(BattleConnector_ProtectDefeatedMon(), CONNECTOR_PROTECT_PARTY_FULL);
    EXPECT_EQ(BattleConnector_Test_GetCandidateCount(), BATTLE_CONNECTOR_PARTY_SIZE);
    gSpecialVar_0x8004 = 2;
    EXPECT(BattleConnector_SwapIntoParty());
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][2], MON_DATA_SPECIES), SPECIES_GEODUDE);

    // A non-Shadow opponent is not offered
    ZeroEnemyPartyMons();
    CreateMon(&gParties[B_TRAINER_OPPONENT_A][0], SPECIES_ZUBAT, 30, 0, OTID_STRUCT_PLAYER_ID);
    EXPECT(!BattleConnector_BufferDefeatedShadowMon());
    BattleConnector_FailChallenge();
}

TEST("Battle Connector: the protected list is capped")
{
    GiveParty(1);
    StartWith(0);
    for (u32 i = 0; i < BATTLE_CONNECTOR_MAX_CANDIDATES; i++)
    {
        DefeatShadowMon(SPECIES_ZUBAT);
        EXPECT(BattleConnector_BufferDefeatedShadowMon());
        EXPECT(BattleConnector_ProtectDefeatedMon() != CONNECTOR_PROTECT_LIST_FULL);
    }
    DefeatShadowMon(SPECIES_ZUBAT);
    EXPECT(BattleConnector_BufferDefeatedShadowMon());
    EXPECT_EQ(BattleConnector_ProtectDefeatedMon(), CONNECTOR_PROTECT_LIST_FULL);
    BattleConnector_FailChallenge();
}

TEST("Battle Connector: the results give exactly one protected Pokemon and throw the rest away")
{
    GiveParty(6);
    StartWith(3);
    DefeatShadowMon(SPECIES_ZUBAT);
    BattleConnector_BufferDefeatedShadowMon();
    BattleConnector_ProtectDefeatedMon();
    DefeatShadowMon(SPECIES_GEODUDE);
    BattleConnector_BufferDefeatedShadowMon();
    BattleConnector_ProtectDefeatedMon();
    // The candidates in use carry what happened in the facility back to the list
    AddShadowHeartGauge(&gParties[B_TRAINER_PLAYER][2], 2);
    EXPECT_EQ(BattleConnector_GetCandidateCount(), 2);

    BattleConnector_ShowCandidates();
    EXPECT_EQ(CalculatePlayerPartyCount(), 2);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_SPECIES), SPECIES_GEODUDE);
    gSpecialVar_0x8004 = 1;
    BattleConnector_BufferCandidateDetails();
    EXPECT(gStringVar4[0] != EOS);
    EXPECT_EQ(BattleConnector_TakeHomeCandidate(), MON_GIVEN_TO_PC); // a full party: to the PC

    EXPECT(!BattleConnector_IsChallengeActive());
    EXPECT_EQ(BattleConnector_GetCandidateCount(), 0);
    EXPECT_EQ(CalculatePlayerPartyCount(), 6);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][3], MON_DATA_SPECIES), SPECIES_ZIGZAGOON);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT(!IsShadowPokemon(&gParties[B_TRAINER_PLAYER][i]));
    // The kept one, in the PC, with its facility progress
    bool32 found = FALSE;
    for (u32 box = 0; box < TOTAL_BOXES_COUNT && !found; box++)
    {
        for (u32 pos = 0; pos < IN_BOX_COUNT && !found; pos++)
        {
            struct BoxPokemon *boxMon = GetBoxedMonPtr(box, pos);
            if (GetBoxMonData(boxMon, MON_DATA_SPECIES) == SPECIES_GEODUDE && IsShadowBoxPokemon(boxMon))
            {
                found = TRUE;
                EXPECT(GetBoxMonData(boxMon, MON_DATA_BATTLE_CONNECTOR_ORIGIN));
                ZeroBoxMonData(boxMon);
            }
        }
    }
    EXPECT(found);
}

TEST("Battle Connector: a lost run throws away every protected Pokemon and gives the party back")
{
    GiveParty(4);
    StartWith(1);
    DefeatShadowMon(SPECIES_ZUBAT);
    BattleConnector_BufferDefeatedShadowMon();
    EXPECT_EQ(BattleConnector_ProtectDefeatedMon(), CONNECTOR_PROTECT_JOINED);
    BattleConnector_FailChallenge();
    EXPECT(!BattleConnector_IsChallengeActive());
    EXPECT_EQ(BattleConnector_GetCandidateCount(), 0);
    EXPECT_EQ(CalculatePlayerPartyCount(), 4);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT(!IsShadowPokemon(&gParties[B_TRAINER_PLAYER][i]));
    EXPECT_EQ(BattleConnector_GetReturnState(), CONNECTOR_RETURN_FAILED);
    // Nothing of it is left to take home
    gSpecialVar_0x8004 = 0;
    EXPECT_EQ(BattleConnector_TakeHomeCandidate(), MON_CANT_GIVE);
}

TEST("Battle Connector: a boss learns its purification move when purified and keeps the facility's mark")
{
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    enum Move move;

    ZeroPlayerPartyMons();
    BattleConnector_CreateShadowMon(mon, SPECIES_MEWTWO, 70);
    CalculatePlayerPartyCount();
    move = GetShadowPurifyMove(mon);
    EXPECT(move == MOVE_RAGE_FIST || move == MOVE_FIERY_WRATH);
    EXPECT(!IsBattleConnectorPurified(mon));
    // It knows its own moves (they come back when purified)
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        EXPECT(GetMonData(mon, MON_DATA_MOVE1 + i) != MOVE_NONE);

    // Room for it: purification teaches it
    for (u32 i = 1; i < MAX_MON_MOVES; i++)
    {
        const u16 none = MOVE_NONE;
        SetMonData(mon, MON_DATA_MOVE1 + i, &none);
    }
    AddShadowHeartGauge(mon, SHADOW_GAUGE_BARS);
    EXPECT(PurifyShadowPokemon(mon));
    EXPECT(IsBattleConnectorPurified(mon));
    bool32 learned = FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        learned |= GetMonData(mon, MON_DATA_MOVE1 + i) == move;
    EXPECT(learned);

    // Saved and loaded, it is still purified, still the facility's
    SavePlayerParty();
    ZeroPlayerPartyMons();
    LoadPlayerParty();
    EXPECT(IsBattleConnectorPurified(&gParties[B_TRAINER_PLAYER][0]));

    // A Mewtwo marked elsewhere gets no purification move
    CreateMon(mon, SPECIES_MEWTWO, 70, 0, OTID_STRUCT_PLAYER_ID);
    MarkShadowPokemon(mon);
    EXPECT_EQ(GetShadowPurifyMove(mon), MOVE_NONE);
}

TEST("Battle Connector: every boss has purification moves, and a protected Pokemon's Shadow state survives saving")
{
    static const u16 bosses[] = {
        SPECIES_MEWTWO, SPECIES_MEW, SPECIES_LUGIA, SPECIES_HO_OH, SPECIES_CELEBI,
        SPECIES_KYOGRE, SPECIES_GROUDON, SPECIES_RAYQUAZA, SPECIES_JIRACHI, SPECIES_DEOXYS,
    };
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];

    ZeroPlayerPartyMons();
    for (u32 i = 0; i < ARRAY_COUNT(bosses); i++)
    {
        BattleConnector_CreateShadowMon(mon, bosses[i], 70);
        EXPECT(GetShadowPurifyMove(mon) != MOVE_NONE);
    }
    CalculatePlayerPartyCount();
    AddShadowHeartGauge(mon, 2);
    u32 gauge = GetShadowGauge(mon);
    SavePlayerParty();
    ZeroPlayerPartyMons();
    LoadPlayerParty();
    EXPECT(IsShadowPokemon(&gParties[B_TRAINER_PLAYER][0]));
    EXPECT_EQ(GetShadowGauge(&gParties[B_TRAINER_PLAYER][0]), gauge);
    EXPECT(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_BATTLE_CONNECTOR_ORIGIN));
}

TEST("Battle Connector: rooms take Battle Frontier sets of generations 1-3 by tier, and the mid-boss its own sets")
{
    static const u8 rooms[][2] = {
        { CONNECTOR_ROOM_SHADOW,         CONNECTOR_TIER_NORMAL },
        { CONNECTOR_ROOM_STRONG_SHADOW,  CONNECTOR_TIER_STRONG },
        { CONNECTOR_ROOM_RARE_SHADOW,    CONNECTOR_TIER_RARE },
        { CONNECTOR_ROOM_DARK_TRAINER,   CONNECTOR_TIER_NORMAL },
        { CONNECTOR_ROOM_STRONG_TRAINER, CONNECTOR_TIER_STRONG },
    };
    u32 room = 0, tries = 0;
    for (u32 i = 0; i < ARRAY_COUNT(rooms); i++)
        PARAMETRIZE { room = i; }

    GiveParty(1);
    StartWith(0);
    for (tries = 0; tries < 20; tries++)
    {
        u16 set, species;
        BattleConnector_Test_SetUpRoom(rooms[room][0]);
        set = BattleConnector_Test_GetRoomSet();
        species = BattleConnector_Test_GetRoomSpecies();
        EXPECT_LT(set, NUM_FRONTIER_MONS);
        EXPECT_EQ(gBattleFrontierMons[set].species, species);
        EXPECT(BattleConnector_IsRoomSpecies(species));
        EXPECT_LE(SpeciesToNationalPokedexNum(species), BATTLE_CONNECTOR_MAX_DEX_NUM);
        EXPECT_EQ(BattleConnector_GetSpeciesTier(species), rooms[room][1]);
    }
    for (tries = 0; tries < 20; tries++)
    {
        BattleConnector_Test_SetUpRoom(CONNECTOR_ROOM_MIDBOSS);
        EXPECT_LT(BattleConnector_Test_GetRoomSet(), NUM_FRONTIER_MONS);
        EXPECT_EQ(gBattleFrontierMons[BattleConnector_Test_GetRoomSet()].species, BattleConnector_Test_GetRoomSpecies());
        EXPECT(gSpeciesInfo[BattleConnector_Test_GetRoomSpecies()].isSubLegendary);
    }
    BattleConnector_RetireChallenge();
}

TEST("Battle Connector: a Shadow Pokemon made from a set keeps the set but no Mega Stone, with three perfect IVs")
{
    const struct TrainerMon *fmon = &gBattleFrontierMons[FRONTIER_MON_VENUSAUR_1];
    struct Pokemon mon;
    u32 perfect = 0;

    BattleConnector_CreateShadowMonFromSet(&mon, FRONTIER_MON_VENUSAUR_1, 40);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPECIES), SPECIES_VENUSAUR);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_LEVEL), 40);
    EXPECT_EQ(GetNature(&mon), fmon->nature);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_SPATK_EV), fmon->ev[3]);
    EXPECT_EQ(fmon->heldItem, ITEM_VENUSAURITE);
    EXPECT_EQ(GetMonData(&mon, MON_DATA_HELD_ITEM), ITEM_NONE);
    for (u32 stat = 0; stat < NUM_STATS; stat++)
        perfect += (GetMonData(&mon, MON_DATA_HP_IV + stat) == MAX_PER_STAT_IVS);
    EXPECT_GE(perfect, BATTLE_CONNECTOR_PERFECT_IVS);
    EXPECT(IsShadowPokemon(&mon));
    EXPECT(GetMonData(&mon, MON_DATA_BATTLE_CONNECTOR_ORIGIN));
    // Its own moves are the set's (the Shadow moves are laid over them in battle)
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        EXPECT_EQ(GetMonData(&mon, MON_DATA_MOVE1 + i), fmon->moves[i]);
}

TEST("Battle Connector: a Shadow Pokemon made from a set with a nickname goes by its species name")
{
    struct Pokemon mon;
    u8 name[POKEMON_NAME_BUFFER_SIZE];

    EXPECT(gBattleFrontierMons[FRONTIER_MON_SALAMENCE_1].nickname != NULL);
    BattleConnector_CreateShadowMonFromSet(&mon, FRONTIER_MON_SALAMENCE_1, 50);
    GetMonData(&mon, MON_DATA_NICKNAME, name);
    EXPECT_EQ(StringCompare(name, GetSpeciesName(SPECIES_SALAMENCE)), 0);
}

static bool32 LearnsetHasMove(const u16 *learnset, u16 move)
{
    for (u32 i = 0; learnset != NULL && learnset[i] != MOVE_UNAVAILABLE; i++)
    {
        if (learnset[i] == move)
            return TRUE;
    }
    return FALSE;
}

static bool32 SpeciesCanLearnMove(u16 species, u16 move)
{
    const struct LevelUpMove *levelUp = GetSpeciesLevelUpLearnset(species);

    for (u32 i = 0; levelUp != NULL && levelUp[i].move != LEVEL_UP_MOVE_END; i++)
    {
        if (levelUp[i].move == move)
            return TRUE;
    }
    return CanLearnTeachableMove(species, move) || LearnsetHasMove(GetSpeciesEggMoves(species), move)
        || LearnsetHasMove(GetFrontierFullLearnset(species), move) || LearnsetHasMove(GetFrontierEventLearnset(species), move)
        || LearnsetHasMove(GetFrontierZaDlcLearnset(species), move)
        || LearnsetHasMove(GetFrontierChampionsLearnset(species), move);
}

TEST("Battle Connector: every room and mid-boss species learns one of two moves it cannot learn otherwise when purified")
{
    struct Pokemon mon;
    u32 checked = 0;

    for (u32 set = 0; set < NUM_FRONTIER_MONS; set++)
    {
        u16 species = gBattleFrontierMons[set].species;
        bool32 midboss = gSpeciesInfo[species].isSubLegendary && GET_BASE_SPECIES_ID(species) == species
            && SpeciesToNationalPokedexNum(species) <= BATTLE_CONNECTOR_MAX_DEX_NUM;
        if ((!BattleConnector_IsRoomSpecies(species) && !midboss) || species == SPECIES_DITTO || species == SPECIES_SMEARGLE)
            continue;
        EXPECT_EQ(GetSpeciesPurifyMoveCount(species), 2);
        BattleConnector_CreateShadowMonFromSet(&mon, set, 50);
        u16 move = GetShadowPurifyMove(&mon);
        EXPECT(move != MOVE_NONE);
        EXPECT(!SpeciesCanLearnMove(species, move));
        checked++;
    }
    EXPECT_GT(checked, 100);
}

TEST("Battle Connector: during a run nothing may move Pokemon or held items in or out of the party")
{
    GiveParty(2);
    StartWith(0);
    // Fusions and the camp shortcut check this; held items are refused separately
    EXPECT(InBattleFacilityChallenge());
    // Talking to the reception again must not replace the party kept for the end of the run
    gSpecialVar_0x8004 = 0;
    gSpecialVar_0x8005 = PARTY_SIZE;
    EXPECT(!BattleConnector_StartChallenge());
    BattleConnector_RetireChallenge();
    EXPECT(!InBattleFacilityChallenge());
    EXPECT_EQ(CalculatePlayerPartyCount(), 2);
}

TEST("Battle Connector: a run needs room for the Pokemon taken home")
{
    GiveParty(6);
    for (u32 box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (u32 pos = 0; pos < IN_BOX_COUNT; pos++)
            CreateBoxMon(GetBoxedMonPtr(box, pos), SPECIES_ZIGZAGOON, 5, 0, OTID_STRUCT_PLAYER_ID);
    }
    EXPECT(!BattleConnector_HasRoomToTakeHome());
    gSpecialVar_0x8004 = 0;
    gSpecialVar_0x8005 = PARTY_SIZE;
    EXPECT(!BattleConnector_StartChallenge());

    // One free place is enough
    ZeroBoxMonAt(0, 0);
    EXPECT(BattleConnector_HasRoomToTakeHome());
    StartWith(0);
    BattleConnector_RetireChallenge();
    ResetPokemonStorageSystem();
}

TEST("Battle Connector: a boss has random IVs besides its perfect ones")
{
    struct Pokemon mon;
    bool32 otherIV = FALSE;

    for (u32 i = 0; i < 8; i++)
    {
        BattleConnector_CreateShadowMon(&mon, SPECIES_MEWTWO, 70);
        for (u32 stat = 0; stat < NUM_STATS; stat++)
        {
            u32 iv = GetMonData(&mon, MON_DATA_HP_IV + stat);
            otherIV |= (iv != 0 && iv != MAX_PER_STAT_IVS);
        }
    }
    EXPECT(otherIV);
}

TEST("Battle Connector: the Move Reminder teaches a purified Pokemon its purification move again")
{
    struct Pokemon mon;
    u16 *moves = Alloc(sizeof(u16) * MAX_RELEARNER_MOVES);
    u32 count;
    bool32 listed;
    enum Move move;

    BattleConnector_CreateShadowMon(&mon, SPECIES_MEWTWO, 70);
    move = GetShadowPurifyMove(&mon);
    // Still a Shadow Pokemon: not yet
    count = Test_GetRelearnerLevelUpMoves(&mon.box, moves);
    listed = FALSE;
    for (u32 i = 0; i < count; i++)
        listed |= (moves[i] == move);
    EXPECT(!listed);

    // Purified without learning it (the scene's prompt declined): it can be relearned
    AddShadowHeartGauge(&mon, SHADOW_GAUGE_BARS);
    EXPECT(PurifyShadowPokemonInScene(&mon));
    EXPECT(!MonKnowsMove(&mon, move));
    count = Test_GetRelearnerLevelUpMoves(&mon.box, moves);
    listed = FALSE;
    for (u32 i = 0; i < count; i++)
        listed |= (moves[i] == move);
    EXPECT(listed);

    // A Pokemon not from the facility gets nothing of the kind
    CreateMon(&mon, SPECIES_MEWTWO, 70, 0, OTID_STRUCT_PLAYER_ID);
    MarkShadowPokemon(&mon);
    AddShadowHeartGauge(&mon, SHADOW_GAUGE_BARS);
    PurifyShadowPokemon(&mon);
    count = Test_GetRelearnerLevelUpMoves(&mon.box, moves);
    listed = FALSE;
    for (u32 i = 0; i < count; i++)
        listed |= (moves[i] == move);
    EXPECT(!listed);
    Free(moves);
}

TEST("Battle Connector: Colosseum's battle music follows the room")
{
    BattleConnector_Test_SetBattleActive(TRUE, CONNECTOR_ROOM_SHADOW);
    EXPECT_EQ(GetBattleBGM(), MUS_VS_COLOSSEUM);
    BattleConnector_Test_SetBattleActive(TRUE, CONNECTOR_ROOM_DARK_TRAINER);
    EXPECT_EQ(GetBattleBGM(), MUS_VS_COLOSSEUM);
    BattleConnector_Test_SetBattleActive(TRUE, CONNECTOR_ROOM_STRONG_SHADOW);
    EXPECT_EQ(GetBattleBGM(), MUS_VS_COLOSSEUM_2);
    BattleConnector_Test_SetBattleActive(TRUE, CONNECTOR_ROOM_MIDBOSS);
    EXPECT_EQ(GetBattleBGM(), MUS_VS_COLOSSEUM_2);
    BattleConnector_Test_SetBattleActive(TRUE, CONNECTOR_ROOM_BOSS);
    EXPECT_EQ(GetBattleBGM(), MUS_VS_COLOSSEUM_ADMIN);
    BattleConnector_Test_SetBattleActive(FALSE, CONNECTOR_ROOM_NONE);
    EXPECT(GetBattleBGM() != MUS_VS_COLOSSEUM_ADMIN);
}

WILD_BATTLE_TEST("Battle Connector: in its battles balls and running are off and no EXP is gained")
{
    bool32 connector;
    PARAMETRIZE { connector = FALSE; }
    PARAMETRIZE { connector = TRUE; }
    GIVEN {
        BattleConnector_Test_SetBattleActive(connector, CONNECTOR_ROOM_SHADOW);
        PLAYER(SPECIES_WOBBUFFET) { Level(20); }
        OPPONENT(SPECIES_CATERPIE) { Level(10); HP(1); }
    } WHEN {
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        if (connector)
            NOT EXPERIENCE_BAR(player);
        else
            EXPERIENCE_BAR(player);
    } THEN {
        BattleConnector_Test_SetBattleActive(FALSE, CONNECTOR_ROOM_NONE);
    }
}

WILD_BATTLE_TEST("Battle Connector: its battles forbid balls and running")
{
    GIVEN {
        BattleConnector_Test_SetBattleActive(TRUE, CONNECTOR_ROOM_SHADOW);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CATERPIE);
    } WHEN {
        TURN { MOVE(player, MOVE_CELEBRATE); }
    } THEN {
        EXPECT(!CanThrowBall());
        EXPECT_EQ(IsRunningFromBattleImpossible(B_BATTLER_0), BATTLE_RUN_FORBIDDEN);
        BattleConnector_Test_SetBattleActive(FALSE, CONNECTOR_ROOM_NONE);
        EXPECT(CanThrowBall());
    }
}
