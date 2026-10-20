#include "global.h"
#include "battle.h"
#include "battle_pyramid.h"
#include "battle_pyramid_bag.h"
#include "battle_setup.h"
#include "event_data.h"
#include "fieldmap.h"
#include "frontier_util.h"
#include "item.h"
#include "load_save.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokemon.h"
#include "script.h"
#include "task.h"
#include "test/test.h"
#include "trainer_see.h"
#include "constants/battle_frontier.h"
#include "constants/frontier_util.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/map_groups.h"
#include "constants/maps.h"

static const enum Species sSpecies[PARTY_SIZE] = {
    SPECIES_PIKACHU, SPECIES_BULBASAUR, SPECIES_CHARMANDER,
    SPECIES_SQUIRTLE, SPECIES_EEVEE, SPECIES_SNORLAX,
};
static const u8 sLevels[PARTY_SIZE] = {100, 51, 73, 49, 50, 25};
static const u8 sOrder[FRONTIER_PARTY_SIZE] = {6, 3, 5};

static void SetUpPyramidParty(u32 levelMode)
{
    VarSet(VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_PYRAMID);
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES);
    gSaveBlock2Ptr->frontier.lvlMode = levelMode;
    ZeroPlayerPartyMons();
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        CreateMonWithIVs(mon, sSpecies[i], sLevels[i], 0x12345678 + i, OTID_STRUCT_PRESET(1234), 31);
        for (u32 m = 0; m < MAX_MON_MOVES; m++)
            SetMonMoveSlot(mon, MOVE_TACKLE, m);
    }
    CalculatePlayerPartyCount();
    SavePlayerParty();
    memset(gSaveBlock2Ptr->frontier.selectedPartyMons, 0, sizeof(gSaveBlock2Ptr->frontier.selectedPartyMons));
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
        gSaveBlock2Ptr->frontier.selectedPartyMons[i] = sOrder[i];
}

static void SelectPyramidParty(void)
{
    gSpecialVar_0x8004 = FRONTIER_UTIL_FUNC_SET_PARTY_ORDER;
    gSpecialVar_0x8005 = FRONTIER_PARTY_SIZE;
    CallFrontierUtilFunc();
}

static void EnterPyramidMap(bool32 summit, u32 status)
{
    u32 group = summit ? MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP) : MAP_GROUP(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR);
    u32 num = summit ? MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_TOP) : MAP_NUM(MAP_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(group, num);
    gSaveBlock1Ptr->pos.x = gSaveBlock1Ptr->pos.y = 0;
    gBackupMapLayout.width = 32;
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = summit ? FRONTIER_STAGES_PER_CHALLENGE : 0;
    gSaveBlock2Ptr->frontier.challengeStatus = status;
    gBattleOutcome = 0;
    ResetTasks();
    RunOnResumeMapScript();
    ResetTasks();
}

TEST("Pyramid: normal floor and summit reloads preserve all six originals and arbitrary selected slots")
{
    u32 mode = FRONTIER_LVL_50;
    bool32 summit = FALSE;
    for (u32 m = 0; m < FRONTIER_LVL_MODE_COUNT; m++)
        for (u32 s = 0; s < 2; s++)
            PARAMETRIZE { mode = m; summit = s; }
    struct Pokemon originals[PARTY_SIZE];
    bool32 flash = gFlashMemoryPresent;
    // Run the real resume script without writing any emulator save sectors.
    gFlashMemoryPresent = FALSE;
    SetUpPyramidParty(mode);
    memcpy(originals, gParties[B_TRAINER_PLAYER], sizeof(originals));
    SelectPyramidParty();
    EnterPyramidMap(summit, 0);
    EXPECT_EQ(*GetSavedPlayerPartyCount(), PARTY_SIZE);
    for (u32 i = 0; i < PARTY_SIZE; i++)
        EXPECT_EQ(memcmp(GetSavedPlayerPartyMon(i), &originals[i], sizeof(struct Pokemon)), 0);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], FRONTIER_PARTY_SIZE);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        EXPECT_EQ(gSaveBlock2Ptr->frontier.selectedPartyMons[i], sOrder[i]);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), sSpecies[sOrder[i] - 1]);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_LEVEL), mode == FRONTIER_LVL_50 ? min(50, sLevels[sOrder[i] - 1]) : sLevels[sOrder[i] - 1]);
    }
    gFlashMemoryPresent = flash;
}

TEST("Pyramid: defeated trainer flags are independent for every local ID")
{
    for (u32 defeated = 0; defeated < MAX_PYRAMID_TRAINERS; defeated++)
    {
        gSaveBlock2Ptr->frontier.pyramidTrainerFlags = 1u << defeated;
        for (u32 trainer = 0; trainer < MAX_PYRAMID_TRAINERS; trainer++)
        {
            gObjectEvents[0].localId = trainer + 1;
            EXPECT_EQ(GetBattlePyramidTrainerFlag(0), trainer == defeated);
        }
    }
    gObjectEvents[0].localId = LOCALID_NONE;
    EXPECT(!GetBattlePyramidTrainerFlag(0));
    gObjectEvents[0].localId = MAX_PYRAMID_TRAINERS + 1;
    EXPECT(!GetBattlePyramidTrainerFlag(0));
    EXPECT(!GetBattlePyramidTrainerFlag(OBJECT_EVENTS_COUNT));
}

TEST("Pyramid: paired trainer encounters persist wandering movement for both correct templates")
{
    memset(gSaveBlock1Ptr->objectEventTemplates, 0, sizeof(gSaveBlock1Ptr->objectEventTemplates));
    memset(gSaveBlock2Ptr->frontier.trainerIds, 0xFF, sizeof(gSaveBlock2Ptr->frontier.trainerIds));
    gSaveBlock2Ptr->frontier.trainerIds[1] = 10;
    gSaveBlock2Ptr->frontier.trainerIds[3] = 20;
    gSaveBlock2Ptr->frontier.pyramidTrainerFlags = 0;
    gSelectedObjectEvent = 1;
    gObjectEvents[1].localId = 2;
    gObjectEvents[2].localId = 4;
    gSpecialVar_LastTalked = 2;
    gApproachingTrainers[1].objectEventId = 2;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_PYRAMID | BATTLE_TYPE_DOUBLE | BATTLE_TYPE_TWO_OPPONENTS;
    TRAINER_BATTLE_PARAM.opponentA = 10;
    TRAINER_BATTLE_PARAM.opponentB = 20;
    MarkApproachingPyramidTrainersAsBattled();
    EXPECT_EQ(gSaveBlock2Ptr->frontier.pyramidTrainerFlags, (1u << 1) | (1u << 3));
    EXPECT_EQ(gSaveBlock1Ptr->objectEventTemplates[1].movementType, MOVEMENT_TYPE_WANDER_AROUND);
    EXPECT_EQ(gSaveBlock1Ptr->objectEventTemplates[3].movementType, MOVEMENT_TYPE_WANDER_AROUND);
    EXPECT_EQ(gSaveBlock1Ptr->objectEventTemplates[0].movementType, 0);
    EXPECT_EQ(gSaveBlock1Ptr->objectEventTemplates[2].movementType, 0);
}

TEST("Pyramid: resuming a paused full party does not apply selected slots twice")
{
    u32 mode = FRONTIER_LVL_50;
    PARAMETRIZE { mode = FRONTIER_LVL_50; }
    PARAMETRIZE { mode = FRONTIER_LVL_OPEN; }
    bool32 flash = gFlashMemoryPresent;
    gFlashMemoryPresent = FALSE;
    SetUpPyramidParty(mode);
    EnterPyramidMap(FALSE, CHALLENGE_STATUS_PAUSED);
    EXPECT_EQ(*GetSavedPlayerPartyCount(), PARTY_SIZE);
    EXPECT_EQ(gPartiesCount[B_TRAINER_PLAYER], FRONTIER_PARTY_SIZE);
    for (u32 i = 0; i < FRONTIER_PARTY_SIZE; i++)
    {
        EXPECT_EQ(gSaveBlock2Ptr->frontier.selectedPartyMons[i], sOrder[i]);
        EXPECT_EQ(GetMonData(&gParties[B_TRAINER_PLAYER][i], MON_DATA_SPECIES), sSpecies[sOrder[i] - 1]);
    }
    gFlashMemoryPresent = flash;
}

TEST("Pyramid: floor transitions retain medicine, PP and held-item changes in the selected party")
{
    bool32 flash = gFlashMemoryPresent;
    gFlashMemoryPresent = FALSE;
    SetUpPyramidParty(FRONTIER_LVL_50);
    SelectPyramidParty();
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];
    u32 hp = GetMonData(mon, MON_DATA_MAX_HP) - 17;
    u32 pp = 3, item = ITEM_ELECTRIUM_Z;
    SetMonData(mon, MON_DATA_HP, &hp);
    SetMonData(mon, MON_DATA_PP1, &pp);
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    EnterPyramidMap(FALSE, 0);
    EXPECT_EQ(GetMonData(mon, MON_DATA_HP), hp);
    EXPECT_EQ(GetMonData(mon, MON_DATA_PP1), pp);
    EXPECT_EQ(GetMonData(mon, MON_DATA_HELD_ITEM), item);
    EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(sOrder[0] - 1), MON_DATA_HP), hp);
    EXPECT_EQ(GetMonData(GetSavedPlayerPartyMon(sOrder[0] - 1), MON_DATA_HELD_ITEM), item);
    gFlashMemoryPresent = flash;
}

TEST("Pyramid: large bag additions split without narrowing quantities or losing items")
{
    u32 mode = FRONTIER_LVL_50;
    PARAMETRIZE { mode = FRONTIER_LVL_50; }
    PARAMETRIZE { mode = FRONTIER_LVL_OPEN; }
    gSaveBlock2Ptr->frontier.lvlMode = mode;
    memset(&gSaveBlock2Ptr->frontier.pyramidBag, 0, sizeof(gSaveBlock2Ptr->frontier.pyramidBag));
    EXPECT(AddPyramidBagItem(ITEM_HYPER_POTION, 256));
    u32 total = 0;
    for (u32 i = 0; i < PYRAMID_BAG_ITEMS_COUNT; i++)
    {
        u32 quantity = gSaveBlock2Ptr->frontier.pyramidBag.quantity[mode][i];
        EXPECT_LE(quantity, MAX_PYRAMID_BAG_ITEM_CAPACITY);
        if (gSaveBlock2Ptr->frontier.pyramidBag.itemId[mode][i] == ITEM_HYPER_POTION)
            total += quantity;
    }
    EXPECT_EQ(total, 256);
    struct PyramidBag before = gSaveBlock2Ptr->frontier.pyramidBag;
    EXPECT(!AddPyramidBagItem(ITEM_HYPER_POTION, PYRAMID_BAG_ITEMS_COUNT * MAX_PYRAMID_BAG_ITEM_CAPACITY));
    EXPECT_EQ(memcmp(&before, &gSaveBlock2Ptr->frontier.pyramidBag, sizeof(before)), 0);
}

TEST("Pyramid: removing items with the close-row cursor never touches the other level mode")
{
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    memset(&gSaveBlock2Ptr->frontier.pyramidBag, 0, sizeof(gSaveBlock2Ptr->frontier.pyramidBag));
    gSaveBlock2Ptr->frontier.pyramidBag.itemId[FRONTIER_LVL_50][0] = ITEM_HYPER_POTION;
    gSaveBlock2Ptr->frontier.pyramidBag.quantity[FRONTIER_LVL_50][0] = 3;
    gSaveBlock2Ptr->frontier.pyramidBag.itemId[FRONTIER_LVL_OPEN][0] = ITEM_HYPER_POTION;
    gSaveBlock2Ptr->frontier.pyramidBag.quantity[FRONTIER_LVL_OPEN][0] = 5;
    gPyramidBagMenuState.cursorPosition = 7;
    gPyramidBagMenuState.scrollPosition = 3;
    EXPECT(RemovePyramidBagItem(ITEM_HYPER_POTION, 1));
    EXPECT_EQ(gSaveBlock2Ptr->frontier.pyramidBag.quantity[FRONTIER_LVL_50][0], 2);
    EXPECT_EQ(gSaveBlock2Ptr->frontier.pyramidBag.quantity[FRONTIER_LVL_OPEN][0], 5);
}

TEST("Pyramid: all floors generate bounded nonoverlapping trainer and item objects")
{
    u32 floor = 0;
    for (u32 f = 0; f < FRONTIER_STAGES_PER_CHALLENGE; f++)
        PARAMETRIZE { floor = f; }
    SetUpPyramidParty(FRONTIER_LVL_50);
    SetFacilityPtrsGetLevel();
    gSaveBlock2Ptr->frontier.curChallengeBattleNum = floor;
    SeedRng(0x12345678);
    for (u32 seed = 0; seed < 32; seed++)
    {
        gSpecialVar_0x8004 = BATTLE_PYRAMID_FUNC_SEED_FLOOR;
        CallBattlePyramidFunction();
        LoadBattlePyramidObjectEventTemplates();
        u32 count = GetNumBattlePyramidObjectEvents();
        EXPECT_GE(count, 5);
        EXPECT_LE(count, 11);
        for (u32 i = 0; i < OBJECT_EVENT_TEMPLATES_COUNT; i++)
        {
            const struct ObjectEventTemplate *object = &gSaveBlock1Ptr->objectEventTemplates[i];
            if (i >= count)
            {
                EXPECT_EQ(object->localId, LOCALID_NONE);
                continue;
            }
            EXPECT_EQ(object->localId, i + 1);
            EXPECT_GE(object->x, 0);
            EXPECT_LT(object->x, 32);
            EXPECT_GE(object->y, 0);
            EXPECT_LT(object->y, 32);
            if (object->graphicsId != OBJ_EVENT_GFX_ITEM_BALL)
            {
                EXPECT_LT(i, MAX_PYRAMID_TRAINERS);
                EXPECT_LT(LocalIdToPyramidTrainerId(object->localId), FRONTIER_TRAINERS_COUNT);
            }
            for (u32 j = 0; j < i; j++)
                EXPECT(object->x != gSaveBlock1Ptr->objectEventTemplates[j].x
                    || object->y != gSaveBlock1Ptr->objectEventTemplates[j].y);
        }
    }
}
