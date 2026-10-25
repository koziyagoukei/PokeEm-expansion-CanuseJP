#include "global.h"
#include "battle.h"
#include "ereader_helpers.h"
#include "ereader_screen.h"
#include "event_data.h"
#include "frontier_util.h"
#include "hall_of_fame.h"
#include "load_save.h"
#include "main.h"
#include "mystery_event_menu.h"
#include "mystery_gift_menu.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "recorded_battle.h"
#include "save.h"
#include "task.h"
#include "title_screen.h"
#include "test/test.h"
#include "constants/frontier_util.h"

#define UNCHANGED_WORD 0xA5A5A5A5

static struct
{
    u32 signature;
    u32 counter;
    u16 missing;
    u16 badChecksums;
    u16 firstId;
    u8 rotation;
} sSlots[NUM_SAVE_SLOTS];

static struct
{
    u32 before;
    u32 value;
    u32 after;
} sLoaded[NUM_SECTORS_PER_SLOT];
static struct SaveSectorLocation sLocations[NUM_SECTORS_PER_SLOT];
static u32 sReadCount;
static bool8 sChangeIdDuringCopy;

// Synthesize flash reads only. These tests never write to the emulator's save.
static void ReadTestSector(u8 sectorId, struct SaveSector *sector)
{
    u8 slot = sectorId / NUM_SECTORS_PER_SLOT;
    u8 physicalId = sectorId % NUM_SECTORS_PER_SLOT;
    u16 id = (physicalId + sSlots[slot].rotation) % NUM_SECTORS_PER_SLOT;

    memset(sector, 0, sizeof(*sector));
    sReadCount++;
    sector->signature = sSlots[slot].signature;
    sector->counter = sSlots[slot].counter;
    sector->data[0] = slot + 1;
    sector->checksum = slot + 1;
    sector->id = physicalId == 0 ? sSlots[slot].firstId : id;
    if (sSlots[slot].missing & (1 << physicalId))
        sector->signature = 0xFFFFFFFF;
    if (sSlots[slot].badChecksums & (1 << physicalId))
        sector->checksum ^= 1;
    if (sChangeIdDuringCopy && sReadCount > NUM_SECTORS_PER_SLOT * NUM_SAVE_SLOTS && physicalId == 5)
        sector->id = 0xFFFF;
}

static void ResetFixture(void)
{
    u32 i;

    memset(sSlots, 0, sizeof(sSlots));
    for (i = 0; i < NUM_SAVE_SLOTS; i++)
    {
        sSlots[i].signature = 0xFFFFFFFF;
        sSlots[i].counter = i;
    }
    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        sLoaded[i].before = UNCHANGED_WORD;
        sLoaded[i].value = UNCHANGED_WORD;
        sLoaded[i].after = UNCHANGED_WORD;
        sLocations[i].data = &sLoaded[i].value;
        sLocations[i].size = sizeof(sLoaded[i].value);
    }
    sReadCount = 0;
    sChangeIdDuringCopy = FALSE;
}

static u8 LoadFixture(void)
{
    struct SaveBlock3 backup = gSaveblock3;
    u8 status;

    Save_TestSetReadFlashSector(ReadTestSector);
    status = Save_TestLoadSlot(sLocations);
    Save_TestSetReadFlashSector(NULL);
    gSaveblock3 = backup;
    return status;
}

static void ExpectLoadedWords(u32 word)
{
    u32 i;

    for (i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        EXPECT_EQ(sLoaded[i].before, UNCHANGED_WORD);
        EXPECT_EQ(sLoaded[i].value, word);
        EXPECT_EQ(sLoaded[i].after, UNCHANGED_WORD);
    }
}

TEST("Save format: legacy and different format versions reject without copying")
{
    u32 signature;
    u16 id;
    PARAMETRIZE { signature = LEGACY_SECTOR_SIGNATURE; id = 0; }
    PARAMETRIZE { signature = LEGACY_SECTOR_SIGNATURE; id = 0xFFFF; }
    PARAMETRIZE { signature = SECTOR_SIGNATURE ^ 1; id = 0; }

    ResetFixture();
    sSlots[0].signature = signature;
    sSlots[0].firstId = id;
    EXPECT_EQ(LoadFixture(), SAVE_STATUS_INCOMPATIBLE);
    EXPECT_EQ(sReadCount, NUM_SECTORS_PER_SLOT * NUM_SAVE_SLOTS);
    ExpectLoadedWords(UNCHANGED_WORD);
    EXPECT_EQ(gSaveCounter, 0);
}

TEST("Save format: empty and corrupt saves do not copy a partial payload")
{
    bool32 corrupt;
    PARAMETRIZE { corrupt = FALSE; }
    PARAMETRIZE { corrupt = TRUE; }

    ResetFixture();
    if (corrupt)
    {
        sSlots[0].signature = SECTOR_SIGNATURE;
        sSlots[0].missing = 1 << 5;
    }
    EXPECT_EQ(LoadFixture(), corrupt ? SAVE_STATUS_CORRUPT : SAVE_STATUS_EMPTY);
    ExpectLoadedWords(UNCHANGED_WORD);
}

TEST("Save format: an uncommitted signature is not treated as another version")
{
    bool32 validBackup;
    PARAMETRIZE { validBackup = FALSE; }
    PARAMETRIZE { validBackup = TRUE; }

    ResetFixture();
    sSlots[0].signature = SECTOR_SIGNATURE | 0xFF;
    if (validBackup)
        sSlots[1].signature = SECTOR_SIGNATURE;
    EXPECT_EQ(LoadFixture(), validBackup ? SAVE_STATUS_OK : SAVE_STATUS_EMPTY);
    ExpectLoadedWords(validBackup ? 2 : UNCHANGED_WORD);
}

TEST("Save format: a current save takes precedence over a legacy slot")
{
    u32 currentSlot;
    PARAMETRIZE { currentSlot = 0; }
    PARAMETRIZE { currentSlot = 1; }

    ResetFixture();
    sSlots[currentSlot].signature = SECTOR_SIGNATURE;
    sSlots[currentSlot ^ 1].signature = LEGACY_SECTOR_SIGNATURE;
    sSlots[currentSlot ^ 1].counter = 200;
    EXPECT_EQ(LoadFixture(), SAVE_STATUS_OK);
    ExpectLoadedWords(currentSlot + 1);
    EXPECT_EQ(gSaveCounter, currentSlot);
}

TEST("Save format: damaged new slots recover the valid backup with an error status")
{
    u32 damagedSlot;
    u16 malformedId;
    PARAMETRIZE { damagedSlot = 0; malformedId = NUM_SECTORS_PER_SLOT; }
    PARAMETRIZE { damagedSlot = 1; malformedId = 32; }
    PARAMETRIZE { damagedSlot = 1; malformedId = 0xFFFF; }
    PARAMETRIZE { damagedSlot = 1; malformedId = 1; }

    ResetFixture();
    sSlots[0].signature = SECTOR_SIGNATURE;
    sSlots[1].signature = SECTOR_SIGNATURE;
    sSlots[damagedSlot].firstId = malformedId;
    EXPECT_EQ(LoadFixture(), SAVE_STATUS_ERROR);
    ExpectLoadedWords((damagedSlot ^ 1) + 1);
    EXPECT_EQ(gSaveCounter, damagedSlot ^ 1);
}

TEST("Save format: bad checksums recover the valid current backup")
{
    ResetFixture();
    sSlots[0].signature = SECTOR_SIGNATURE;
    sSlots[1].signature = SECTOR_SIGNATURE;
    sSlots[1].badChecksums = 1 << 5;
    EXPECT_EQ(LoadFixture(), SAVE_STATUS_ERROR);
    ExpectLoadedWords(1);
}

TEST("Save format: newest counters, wraparound and sector rotation are preserved")
{
    u32 counter0, counter1, expectedSlot;
    u8 rotation;
    PARAMETRIZE { counter0 = 2; counter1 = 1; expectedSlot = 0; rotation = 0; }
    PARAMETRIZE { counter0 = 2; counter1 = 3; expectedSlot = 1; rotation = 5; }
    PARAMETRIZE { counter0 = 0; counter1 = 0xFFFFFFFF; expectedSlot = 0; rotation = 13; }

    ResetFixture();
    sSlots[0].signature = SECTOR_SIGNATURE;
    sSlots[1].signature = SECTOR_SIGNATURE;
    sSlots[0].counter = counter0;
    sSlots[1].counter = counter1;
    sSlots[expectedSlot].rotation = rotation;
    sSlots[expectedSlot].firstId = rotation;
    EXPECT_EQ(LoadFixture(), SAVE_STATUS_OK);
    ExpectLoadedWords(expectedSlot + 1);
    EXPECT_EQ(gLastWrittenSector, (NUM_SECTORS_PER_SLOT - rotation) % NUM_SECTORS_PER_SLOT);
}

TEST("Save format: IDs are checked again before copying the selected slot")
{
    ResetFixture();
    sSlots[0].signature = SECTOR_SIGNATURE;
    sChangeIdDuringCopy = TRUE;
    EXPECT_EQ(LoadFixture(), SAVE_STATUS_OK);
    for (u32 i = 0; i < NUM_SECTORS_PER_SLOT; i++)
    {
        EXPECT_EQ(sLoaded[i].value, i == 5 ? UNCHANGED_WORD : 1);
        EXPECT_EQ(sLoaded[i].before, UNCHANGED_WORD);
        EXPECT_EQ(sLoaded[i].after, UNCHANGED_WORD);
    }
}

TEST("Save format: legacy boot does not read trainer offsets or load the party")
{
    bool32 flashPresent = gFlashMemoryPresent;
    u32 partyWord, savedWord, loadedPartyWord;
    u32 originalPersonality = gParties[B_TRAINER_PLAYER][0].box.personality;
    u16 previousFileStatus = gSaveFileStatus;
    u16 loadedFileStatus;
    u8 status;
    u16 offset;

    ResetFixture();
    sSlots[0].signature = LEGACY_SECTOR_SIGNATURE;
    sSlots[0].firstId = 0xFFFF;
    partyWord = GetSavedPlayerPartyMon(0)->box.personality ^ UNCHANGED_WORD;
    gParties[B_TRAINER_PLAYER][0].box.personality = partyWord;
    savedWord = *(u32 *)gSaveBlock1Ptr;
    gFlashMemoryPresent = TRUE;
    Save_TestSetReadFlashSector(ReadTestSector);
    offset = GetSaveBlocksPointersBaseOffset();
    status = LoadGameSave(SAVE_NORMAL);
    loadedFileStatus = gSaveFileStatus;
    gSaveFileStatus = previousFileStatus;
    Save_TestSetReadFlashSector(NULL);
    gFlashMemoryPresent = flashPresent;
    loadedPartyWord = gParties[B_TRAINER_PLAYER][0].box.personality;
    gParties[B_TRAINER_PLAYER][0].box.personality = originalPersonality;
    EXPECT_EQ(offset, 0);
    EXPECT_EQ(status, SAVE_STATUS_INCOMPATIBLE);
    EXPECT_EQ(loadedFileStatus, SAVE_STATUS_INCOMPATIBLE);
    EXPECT_EQ(loadedPartyWord, partyWord);
    EXPECT_EQ(*(u32 *)gSaveBlock1Ptr, savedWord);
}

TEST("Optional features: disabled gift and e-Reader entrypoints do not start tasks or transfers")
{
    MainCallback callback = gMain.callback2;

    ASSUME(FREE_MYSTERY_GIFT);
    ASSUME(FREE_MYSTERY_EVENT_BUFFERS);
    ASSUME(!ENABLE_E_READER);
    ResetTasks();
    CB2_InitMysteryGift();
    EXPECT(gMain.callback2 == CB2_InitTitleScreen);
    CB2_InitMysteryEventMenu();
    EXPECT(gMain.callback2 == CB2_InitTitleScreen);
    CB2_InitEReader();
    EXPECT(gMain.callback2 == CB2_InitTitleScreen);
    CreateEReaderTask();
    EXPECT_EQ(EReaderHandleTransfer(TRUE, 0, NULL, NULL), EREADER_CANCEL_KEY_MASK);
    EXPECT(!TryWriteTrainerHill(NULL));
    EXPECT(!ReadTrainerHillAndValidate());
    for (u32 i = 0; i < NUM_TASKS; i++)
        EXPECT(!gTasks[i].isActive);
    gMain.callback2 = callback;
}

TEST("Optional features: freed special sectors hold boxes instead of Hall of Fame and battle videos")
{
    ASSUME(FREE_SPECIAL_SAVE_SECTORS);
    EXPECT_EQ(NUM_SECTORS_PER_SLOT * NUM_SAVE_SLOTS, SECTORS_COUNT);
    EXPECT_EQ(TOTAL_BOXES_COUNT, 18);

    // Special sector access must fail before touching flash or the buffer.
    EXPECT_EQ(TryReadSpecialSaveSector(SECTOR_ID_RECORDED_BATTLE, NULL), SAVE_STATUS_ERROR);
    EXPECT_EQ(TryWriteSpecialSaveSector(SECTOR_ID_RECORDED_BATTLE, NULL), SAVE_STATUS_ERROR);
    EXPECT_EQ(TryReadSpecialSaveSector(SECTOR_ID_TRAINER_HILL, NULL), SAVE_STATUS_ERROR);
    EXPECT(!CanCopyRecordedBattleSaveData());

    FlagSet(FLAG_SYS_GAME_CLEAR);
    EXPECT(!IsHallOfFamePCAvailable());
    FlagClear(FLAG_SYS_GAME_CLEAR);

    gSaveBlock2Ptr->frontier.disableRecordBattle = FALSE;
    gSpecialVar_0x8004 = FRONTIER_UTIL_FUNC_GET_DATA;
    gSpecialVar_0x8005 = FRONTIER_DATA_RECORD_DISABLED;
    CallFrontierUtilFunc();
    EXPECT_EQ(gSpecialVar_Result, TRUE);
}
