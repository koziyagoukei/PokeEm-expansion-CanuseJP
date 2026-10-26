#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "battle_setup.h"
#include "event_data.h"
#include "frontier_practice.h"
#include "frontier_story.h"
#include "frontier_util.h"
#include "money.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "string_util.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_trainers.h"
#include "constants/flags.h"
#include "constants/characters.h"
#include "constants/frontier_practice.h"
#include "constants/moves.h"
#include "constants/party_menu.h"
#include "constants/script_commands.h"
#include "constants/vars.h"

static void SetUpPractice(void)
{
    FrontierPractice_Reset();
    ZeroPlayerPartyMons();
    gBattleTypeFlags = 0;
    for (u32 slot = 0; slot < IN_BOX_COUNT; slot++)
        ZeroBoxMonAt(0, slot);
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 100, 123, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], MOVE_THUNDERBOLT, 0);
    CalculatePlayerPartyCount();
}

static void PlacePracticeMon(u32 slot)
{
    struct Pokemon mon;
    CreateMonWithIVs(&mon, SPECIES_SWAMPERT, 75, slot + 222, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(&mon, MOVE_WATERFALL, 0);
    u32 value = ITEM_SWAMPERTITE;
    SetMonData(&mon, MON_DATA_HELD_ITEM, &value);
    value = TYPE_WATER;
    SetMonData(&mon, MON_DATA_TERA_TYPE, &value);
    value = 10;
    SetMonData(&mon, MON_DATA_DYNAMAX_LEVEL, &value);
    SetBoxMonAt(0, slot, &mon.box);
}

static u32 FreeHeapBytes(void)
{
    const struct MemBlock *head = HeapHead();
    const struct MemBlock *block = head;
    u32 bytes = 0;
    do
    {
        if (!block->allocated)
            bytes += block->size;
        block = block->next;
    } while (block != head);
    return bytes;
}

TEST("Frontier practice: empty slots, Eggs, duplicates and seventh selections are rejected")
{
    SetUpPractice();
    EXPECT_EQ(FrontierPractice_AddBoxMon(TOTAL_BOXES_COUNT, 0), PRACTICE_INVALID_MON);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, IN_BOX_COUNT), PRACTICE_INVALID_MON);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_INVALID_MON);
    EXPECT(!FrontierPractice_CanSelectBoxMon(&gParties[B_TRAINER_PLAYER][0].box));
    PlacePracticeMon(0);
    u32 egg = TRUE;
    SetBoxMonDataAt(0, 0, MON_DATA_IS_EGG, &egg);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_INVALID_MON);
    egg = FALSE;
    SetBoxMonDataAt(0, 0, MON_DATA_IS_EGG, &egg);
    EXPECT(FrontierPractice_CanSelectBoxMon(GetBoxedMonPtr(0, 0)));
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_OK);
    EXPECT(!FrontierPractice_CanSelectBoxMon(GetBoxedMonPtr(0, 0)));
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_DUPLICATE);
    for (u32 slot = 1; slot < PARTY_SIZE; slot++)
    {
        PlacePracticeMon(slot);
        EXPECT_EQ(FrontierPractice_AddBoxMon(0, slot), PRACTICE_OK);
    }
    PlacePracticeMon(6);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 6), PRACTICE_TEAM_FULL);
    EXPECT_EQ(FrontierPractice_GetTeamCount(), 6);
    EXPECT(FrontierPractice_RemoveMon(2));
    EXPECT_EQ(FrontierPractice_GetTeamCount(), 5);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 6), PRACTICE_OK);
    EXPECT(!FrontierPractice_RemoveMon(6));
    FrontierPractice_Reset();
}

TEST("Frontier practice: six battle copies preserve boxed data and restore the exact original party")
{
    struct Pokemon original[PARTY_SIZE];
    struct BoxPokemon boxes[PARTY_SIZE];
    SetUpPractice();
    u32 value = 1;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &value);
    value = STATUS1_POISON;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS, &value);
    value = 0;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_PP1, &value);
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        PlacePracticeMon(slot);
        boxes[slot] = *GetBoxedMonPtr(0, slot);
        EXPECT_EQ(FrontierPractice_AddBoxMon(0, slot), PRACTICE_OK);
    }
    SetMoney(&gSaveBlock1Ptr->money, 123456);
    FlagSet(FLAG_EXPANSION_TERA_ORB_CHARGED);
    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_EASY);
    EXPECT(FrontierPractice_SetOptions(FRONTIER_AI_LEVEL_STRONGEST, FALSE, TRUE));
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_OK);
    EXPECT(FrontierPractice_IsBattle());
    EXPECT_EQ(FrontierPractice_GetAiLevel(), FRONTIER_AI_LEVEL_STRONGEST);
    EXPECT_EQ(VarGet(VAR_FRONTIER_AI_LEVEL), FRONTIER_AI_LEVEL_EASY);
    EXPECT_EQ(gPartiesCount[B_TRAINER_OPPONENT_A], 6);
    EXPECT(gBattleTypeFlags & BATTLE_TYPE_FRONTIER);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 50);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_STATUS), 0);
    for (u32 slot = 0; slot < PARTY_SIZE; slot++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][slot];
        EXPECT_EQ(GetMonData(mon, MON_DATA_LEVEL), 50);
        EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1), MOVE_WATERFALL);
        EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), ITEM_SWAMPERTITE);
        EXPECT_EQ(GetMonData(mon, MON_DATA_TERA_TYPE), TYPE_WATER);
        EXPECT_EQ(GetMonData(mon, MON_DATA_DYNAMAX_LEVEL), 10);
        EXPECT_EQ(GetMonData(mon, MON_DATA_HP), GetMonData(mon, MON_DATA_MAX_HP));
        EXPECT_EQ(memcmp(&boxes[slot], GetBoxedMonPtr(0, slot), sizeof(boxes[slot])), 0);
    }
    value = ITEM_NONE;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HELD_ITEM, &value);
    value = 0;
    SetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_HP, &value);
    SetMoney(&gSaveBlock1Ptr->money, 0);
    FlagClear(FLAG_EXPANSION_TERA_ORB_CHARGED);
    FrontierPractice_RestoreBattle();
    EXPECT(!FrontierPractice_IsBattle());
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], 1);
    EXPECT_EQ(GetMoney(&gSaveBlock1Ptr->money), 123456);
    EXPECT(FlagGet(FLAG_EXPANSION_TERA_ORB_CHARGED));
    EXPECT_EQ(FrontierPractice_GetAiLevel(), FRONTIER_AI_LEVEL_EASY);
    EXPECT_EQ(FrontierPractice_GetTeamCount(), 6);
    FrontierPractice_Reset();
}

TEST("Frontier practice: doubles validate both teams and original levels remain unchanged")
{
    SetUpPractice();
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_NO_TEAM);
    PlacePracticeMon(0);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_OK);
    EXPECT(FrontierPractice_SetOptions(FRONTIER_AI_LEVEL_HARD, TRUE, FALSE));
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_NOT_ENOUGH_ENEMY);
    PlacePracticeMon(1);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 1), PRACTICE_OK);
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_NOT_ENOUGH_PLAYER);
    CreateRandomMon(&gParties[B_TRAINER_PLAYER][1], SPECIES_RAYQUAZA, 80);
    CalculatePlayerPartyCount();
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_OK);
    EXPECT(gBattleTypeFlags & BATTLE_TYPE_DOUBLE);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 100);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][1], MON_DATA_LEVEL), 80);
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_OPPONENT_A][0], MON_DATA_LEVEL), 75);
    EXPECT(!FrontierPractice_SetOptions(FRONTIER_AI_LEVEL_NORMAL, FALSE, TRUE));
    FrontierPractice_Reset();
}

TEST("Frontier practice: replaced box slots require reselection instead of silently changing opponents")
{
    SetUpPractice();
    PlacePracticeMon(0);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_OK);
    ZeroBoxMonAt(0, 0);
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_TEAM_CHANGED);
    EXPECT(!FrontierPractice_IsBattle());
    EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][0], MON_DATA_LEVEL), 100);
    EXPECT(!FrontierPractice_SetOptions(FRONTIER_AI_LEVEL_EASY + 1, FALSE, TRUE));
    EXPECT(!FrontierPractice_SetOptions(FRONTIER_AI_LEVEL_NORMAL, 2, TRUE));
    FrontierPractice_Reset();
}

TEST("Frontier practice: difficulty is local and simulated wins cannot advance story")
{
    u32 ai = 0;
    for (u32 level = 0; level <= FRONTIER_AI_LEVEL_EASY; level++)
        PARAMETRIZE { ai = level; }
    SetUpPractice();
    PlacePracticeMon(0);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_OK);
    VarSet(VAR_FRONTIER_AI_LEVEL, FRONTIER_AI_LEVEL_HARD);
    EXPECT(FrontierPractice_SetOptions(ai, FALSE, TRUE));
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_OK);
    EXPECT_EQ(FrontierPractice_GetAiLevel(), ai);
    gBattleTypeFlags |= BATTLE_TYPE_LINK;
    EXPECT_EQ(FrontierPractice_GetAiLevel(), FRONTIER_AI_LEVEL_HARD);
    gBattleTypeFlags &= ~BATTLE_TYPE_LINK;
    VarSet(VAR_FRONTIER_INTRO_STATE, FRONTIER_INTRO_COMPLETE);
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_TOWER);
    u32 firstFlag = FLAG_STORY_NAMED_STATES_START + NAMED_STORY_BRAIN_FIRST * 2;
    FlagClear(firstFlag);
    FlagClear(firstFlag + 1);
    TRAINER_BATTLE_PARAM.opponentA = TRAINER_FRONTIER_BRAIN;
    gBattleOutcome = B_OUTCOME_WON;
    FrontierStory_OnBattleFinished();
    EXPECT(!FlagGet(firstFlag));
    EXPECT(!FlagGet(firstFlag + 1));
    FrontierPractice_Reset();
    EXPECT_EQ(FrontierPractice_GetAiLevel(), FRONTIER_AI_LEVEL_HARD);
}

TEST("Frontier practice: a cold boot initializes all facility metadata and restores the previous context")
{
    u8 name[PLAYER_NAME_LENGTH + 1];
    u32 level50 = TRUE;
    PARAMETRIZE { level50 = TRUE; }
    PARAMETRIZE { level50 = FALSE; }
    SetUpPractice();
    PlacePracticeMon(0);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_OK);
    gFacilityTrainers = NULL;
    gFacilityTrainerMons = NULL;
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_TENT;
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_FACTORY);
    EXPECT(FrontierPractice_SetOptions(FRONTIER_AI_LEVEL_NORMAL, FALSE, level50));
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_OK);
    EXPECT(gFacilityTrainers == gBattleFrontierTrainers);
    EXPECT(gFacilityTrainerMons == gBattleFrontierMons);
    EXPECT_EQ((u32)gSaveBlock2Ptr->frontier.lvlMode, level50 ? FRONTIER_LVL_50 : FRONTIER_LVL_OPEN);
    EXPECT_NE(GetFrontierTrainerFrontSpriteId(FRONTIER_TRAINER_BRADY), TRAINER_PIC_HIKER);
    EXPECT_EQ(GetFrontierTrainerFacilityClass(FRONTIER_TRAINER_BRADY), FACILITY_CLASS_YOUNGSTER);
    GetFrontierTrainerName(name, FRONTIER_TRAINER_BRADY);
    EXPECT_EQ(memcmp(name, gBattleFrontierTrainers[FRONTIER_TRAINER_BRADY].trainerName, PLAYER_NAME_LENGTH), 0);
    FrontierPractice_RestoreBattle();
    EXPECT(gFacilityTrainers == NULL);
    EXPECT(gFacilityTrainerMons == NULL);
    EXPECT_EQ((u32)gSaveBlock2Ptr->frontier.lvlMode, FRONTIER_LVL_TENT);
    EXPECT_EQ(VarGet(VAR_FRONTIER_FACILITY), FRONTIER_FACILITY_FACTORY);
    FrontierPractice_Reset();
}

TEST("Frontier practice: storage selection waits for the asynchronous UI result")
{
    extern const u8 FrontierPractice_ChooseBox[];
    EXPECT_EQ(FrontierPractice_ChooseBox[0], SCR_OP_CALLNATIVE);
    EXPECT_EQ(FrontierPractice_ChooseBox[5], SELECT_PC_MON_FRONTIER_PRACTICE);
    EXPECT_EQ(FrontierPractice_ChooseBox[6], SCR_OP_WAITSTATE);
}

TEST("Frontier practice: both team pages use prompt scrolling instead of invisible extra lines")
{
    SetUpPractice();
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        PlacePracticeMon(i);
        EXPECT_EQ(FrontierPractice_AddBoxMon(0, i), PRACTICE_OK);
    }
    for (u32 page = 0; page < 2; page++)
    {
        u32 newlines = 0, scrolls = 0;
        gSpecialVar_0x8004 = page;
        Special_FrontierPracticeBufferTeam();
        for (const u8 *p = gStringVar4; *p != EOS; p++)
        {
            newlines += *p == CHAR_NEWLINE;
            scrolls += *p == CHAR_PROMPT_SCROLL;
        }
        EXPECT_EQ(newlines, 1);
        EXPECT_EQ(scrolls, 2);
    }
    FrontierPractice_Reset();
}

TEST("Frontier practice: original party backup does not occupy the heap reset by battle entry")
{
    struct Pokemon original[PARTY_SIZE];
    SetUpPractice();
    PlacePracticeMon(0);
    EXPECT_EQ(FrontierPractice_AddBoxMon(0, 0), PRACTICE_OK);
    memcpy(original, gParties[B_TRAINER_PLAYER], sizeof(original));
    u32 freeBefore = FreeHeapBytes();
    EXPECT_EQ(FrontierPractice_PrepareBattle(), PRACTICE_OK);
    EXPECT_EQ(FreeHeapBytes(), freeBefore);
    void *scratch = Alloc(4096);
    EXPECT(scratch != NULL);
    memset(scratch, 0xA5, 4096);
    FrontierPractice_RestoreBattle();
    EXPECT_EQ(memcmp(original, gParties[B_TRAINER_PLAYER], sizeof(original)), 0);
    memset(scratch, 0x5A, 4096);
    Free(scratch);
    EXPECT_EQ(FreeHeapBytes(), freeBefore);
    FrontierPractice_Reset();
}
