#include "global.h"
#include "battle_connector.h"
#include "battle_main.h"
#include "data.h"
#include "event_data.h"
#include "item_use.h"
#include "load_save.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "shadow_pokemon.h"
#include "string_util.h"
#include "constants/characters.h"
#include "test/battle.h"
#include "constants/battle_connector.h"
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

TEST("Battle Connector: the player brings exactly one Pokemon, of any species, and gets the party back")
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
