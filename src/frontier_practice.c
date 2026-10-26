#include "global.h"
#include "battle.h"
#include "battle_frontier.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "battle_transition.h"
#include "data.h"
#include "event_data.h"
#include "frontier_practice.h"
#include "frontier_util.h"
#include "main.h"
#include "money.h"
#include "overworld.h"
#include "palette.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "task.h"
#include "constants/battle_frontier.h"
#include "constants/battle_frontier_trainers.h"
#include "constants/flags.h"
#include "constants/frontier_practice.h"
#include "constants/party_menu.h"
#include "constants/vars.h"

struct PracticeMonRef
{
    u32 personality;
    u32 otId;
    u16 species;
    u8 box;
    u8 slot;
};

struct PracticeBackup
{
    struct Pokemon party[PARTY_SIZE];
    u32 battleFlags;
    u32 money;
    const struct BattleFrontierTrainer *facilityTrainers;
    const struct TrainerMon *facilityTrainerMons;
    enum FrontierLevelMode levelMode;
    u8 partyCount;
    bool8 teraCharged;
    bool8 disableRecordBattle;
};

// CB2_InitBattle resets the heap; the original party must outlive that reset.
static EWRAM_DATA struct PracticeBackup sPracticeBackup = {0};

static EWRAM_DATA struct
{
    struct PracticeMonRef mons[PARTY_SIZE];
    struct PracticeBackup *backup;
    u8 count;
    u8 ai;
    bool8 doubles;
    bool8 level50;
    bool8 initialized;
} sPractice = {0};

static const u8 *const sAiNames[] =
{
    [FRONTIER_AI_LEVEL_NORMAL] = COMPOUND_STRING("{JPN}いつもの"),
    [FRONTIER_AI_LEVEL_HARD] = COMPOUND_STRING("{JPN}むずかしい"),
    [FRONTIER_AI_LEVEL_STRONGEST] = COMPOUND_STRING("{JPN}さいきょう"),
    [FRONTIER_AI_LEVEL_EASY] = COMPOUND_STRING("{JPN}かんたん"),
};

static bool32 IsUsableBoxMon(struct BoxPokemon *mon)
{
    u32 species = GetBoxMonData(mon, MON_DATA_SPECIES);
    if (species == SPECIES_NONE || species >= NUM_SPECIES
     || GetBoxMonData(mon, MON_DATA_IS_EGG) || GetBoxMonData(mon, MON_DATA_SANITY_IS_BAD_EGG))
        return FALSE;
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
        if (GetBoxMonData(mon, MON_DATA_MOVE1 + i) != MOVE_NONE)
            return TRUE;
    return FALSE;
}

static bool32 ReferenceMatches(u32 index)
{
    const struct PracticeMonRef *ref = &sPractice.mons[index];
    struct BoxPokemon *mon = GetBoxedMonPtr(ref->box, ref->slot);
    return IsUsableBoxMon(mon)
        && GetBoxMonData(mon, MON_DATA_SPECIES) == ref->species
        && GetBoxMonData(mon, MON_DATA_PERSONALITY) == ref->personality
        && GetBoxMonData(mon, MON_DATA_OT_ID) == ref->otId;
}

u32 FrontierPractice_GetTeamCount(void)
{
    return sPractice.count;
}

bool32 FrontierPractice_IsBattle(void)
{
    return sPractice.backup != NULL && !(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK));
}

u32 FrontierPractice_GetAiLevel(void)
{
    return FrontierPractice_IsBattle() ? sPractice.ai : VarGet(VAR_FRONTIER_AI_LEVEL);
}

bool32 FrontierPractice_SetOptions(u32 ai, bool32 doubles, bool32 level50)
{
    if (sPractice.backup != NULL || ai > FRONTIER_AI_LEVEL_EASY || doubles > TRUE || level50 > TRUE)
        return FALSE;
    sPractice.ai = ai;
    sPractice.doubles = doubles;
    sPractice.level50 = level50;
    return TRUE;
}

u32 FrontierPractice_AddBoxMon(u32 box, u32 slot)
{
    struct BoxPokemon *mon;
    struct PracticeMonRef *ref;
    if (sPractice.backup != NULL || box >= TOTAL_BOXES_COUNT || slot >= IN_BOX_COUNT)
        return PRACTICE_INVALID_MON;
    if (sPractice.count >= PARTY_SIZE)
        return PRACTICE_TEAM_FULL;
    mon = GetBoxedMonPtr(box, slot);
    if (!IsUsableBoxMon(mon))
        return PRACTICE_INVALID_MON;
    for (u32 i = 0; i < sPractice.count; i++)
        if (sPractice.mons[i].box == box && sPractice.mons[i].slot == slot)
            return PRACTICE_DUPLICATE;
    ref = &sPractice.mons[sPractice.count++];
    ref->box = box;
    ref->slot = slot;
    ref->species = GetBoxMonData(mon, MON_DATA_SPECIES);
    ref->personality = GetBoxMonData(mon, MON_DATA_PERSONALITY);
    ref->otId = GetBoxMonData(mon, MON_DATA_OT_ID);
    return PRACTICE_OK;
}

bool32 FrontierPractice_CanSelectBoxMon(struct BoxPokemon *mon)
{
    u32 offset = (u32)mon - (u32)gPokemonStoragePtr->boxes;
    if (offset >= sizeof(gPokemonStoragePtr->boxes) || offset % sizeof(struct BoxPokemon) != 0
     || sPractice.count >= PARTY_SIZE || !IsUsableBoxMon(mon))
        return FALSE;
    for (u32 i = 0; i < sPractice.count; i++)
        if (GetBoxedMonPtr(sPractice.mons[i].box, sPractice.mons[i].slot) == mon)
            return FALSE;
    return TRUE;
}

bool32 FrontierPractice_RemoveMon(u32 index)
{
    if (sPractice.backup != NULL || index >= sPractice.count)
        return FALSE;
    for (u32 i = index; i + 1 < sPractice.count; i++)
        sPractice.mons[i] = sPractice.mons[i + 1];
    memset(&sPractice.mons[--sPractice.count], 0, sizeof(sPractice.mons[0]));
    return TRUE;
}

static void PrepareMon(struct Pokemon *mon)
{
    u32 value = 0;
    if (sPractice.level50)
    {
        u32 exp = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][50];
        SetMonData(mon, MON_DATA_EXP, &exp);
    }
    CalculateMonStats(mon);
    SetMonData(mon, MON_DATA_STATUS, &value);
    value = GetMonData(mon, MON_DATA_MAX_HP);
    SetMonData(mon, MON_DATA_HP, &value);
    MonRestorePP(mon);
}

u32 FrontierPractice_PrepareBattle(void)
{
    u32 playerCount = 0;
    if (sPractice.backup != NULL)
        return PRACTICE_INVALID_MON;
    if (sPractice.count == 0)
        return PRACTICE_NO_TEAM;
    if (sPractice.doubles && sPractice.count < 2)
        return PRACTICE_NOT_ENOUGH_ENEMY;
    for (u32 i = 0; i < sPractice.count; i++)
        if (!ReferenceMatches(i))
            return PRACTICE_TEAM_CHANGED;
    for (u32 i = 0; i < PARTY_SIZE; i++)
        playerCount += IsUsableBoxMon(&gParties[B_TRAINER_PLAYER][i].box);
    if (playerCount < (sPractice.doubles ? 2 : 1))
        return PRACTICE_NOT_ENOUGH_PLAYER;
    sPractice.backup = &sPracticeBackup;
    memcpy(sPractice.backup->party, gParties[B_TRAINER_PLAYER], sizeof(sPractice.backup->party));
    sPractice.backup->partyCount = gPartiesCount[B_TRAINER_PLAYER];
    sPractice.backup->battleFlags = gBattleTypeFlags;
    sPractice.backup->money = GetMoney(&gSaveBlock1Ptr->money);
    sPractice.backup->facilityTrainers = gFacilityTrainers;
    sPractice.backup->facilityTrainerMons = gFacilityTrainerMons;
    sPractice.backup->levelMode = gSaveBlock2Ptr->frontier.lvlMode;
    sPractice.backup->teraCharged = FlagGet(FLAG_EXPANSION_TERA_ORB_CHARGED);
    sPractice.backup->disableRecordBattle = gSaveBlock2Ptr->frontier.disableRecordBattle;

    // All mutations are confined to battle copies, never the storage originals.
    ZeroPartyMons(gParties[B_TRAINER_OPPONENT_A]);
    for (u32 i = 0; i < sPractice.count; i++)
    {
        BoxMonAtToMon(sPractice.mons[i].box, sPractice.mons[i].slot, &gParties[B_TRAINER_OPPONENT_A][i]);
        PrepareMon(&gParties[B_TRAINER_OPPONENT_A][i]);
    }
    gPartiesCount[B_TRAINER_OPPONENT_A] = sPractice.count;
    ZeroPlayerPartyMons();
    playerCount = 0;
    for (u32 i = 0; i < PARTY_SIZE; i++)
    {
        if (!IsUsableBoxMon(&sPractice.backup->party[i].box))
            continue;
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][playerCount++];
        *mon = sPractice.backup->party[i];
        PrepareMon(mon);
    }
    gPartiesCount[B_TRAINER_PLAYER] = playerCount;
    InitTrainerBattleParameter();
    TRAINER_BATTLE_PARAM.opponentA = FRONTIER_TRAINER_BRADY;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_BATTLE_TOWER;
    if (sPractice.doubles)
        gBattleTypeFlags |= BATTLE_TYPE_DOUBLE;
    // Trainer graphics, names and AI read these before any facility entry script.
    // Do not inherit a previous Battle Tent's metadata or require a prior visit.
    gSaveBlock2Ptr->frontier.lvlMode = sPractice.level50 ? FRONTIER_LVL_50 : FRONTIER_LVL_OPEN;
    SetFacilityPtrsGetLevel();
    return PRACTICE_OK;
}

void FrontierPractice_RestoreBattle(void)
{
    if (sPractice.backup == NULL)
        return;
    memcpy(gParties[B_TRAINER_PLAYER], sPractice.backup->party, sizeof(sPractice.backup->party));
    gPartiesCount[B_TRAINER_PLAYER] = sPractice.backup->partyCount;
    SetMoney(&gSaveBlock1Ptr->money, sPractice.backup->money);
    gSaveBlock2Ptr->frontier.disableRecordBattle = sPractice.backup->disableRecordBattle;
    gSaveBlock2Ptr->frontier.lvlMode = sPractice.backup->levelMode;
    gFacilityTrainers = sPractice.backup->facilityTrainers;
    gFacilityTrainerMons = sPractice.backup->facilityTrainerMons;
    if (sPractice.backup->teraCharged)
        FlagSet(FLAG_EXPANSION_TERA_ORB_CHARGED);
    else
        FlagClear(FLAG_EXPANSION_TERA_ORB_CHARGED);
    gBattleTypeFlags = sPractice.backup->battleFlags;
    memset(&sPracticeBackup, 0, sizeof(sPracticeBackup));
    sPractice.backup = NULL;
    ZeroPartyMons(gParties[B_TRAINER_OPPONENT_A]);
    gPartiesCount[B_TRAINER_OPPONENT_A] = 0;
}

void FrontierPractice_Reset(void)
{
    FrontierPractice_RestoreBattle();
    memset(&sPractice, 0, sizeof(sPractice));
    sPractice.ai = FRONTIER_AI_LEVEL_NORMAL;
    sPractice.level50 = TRUE;
    sPractice.initialized = TRUE;
}

static void CB2_EndPracticeBattle(void)
{
    gSpecialVar_0x8007 = gBattleOutcome;
    gSpecialVar_Result = PRACTICE_OK;
    FrontierPractice_RestoreBattle();
    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

static void Task_EnterPracticeBattle(u8 taskId)
{
    if (gTasks[taskId].data[0] == 0)
    {
        if (gPaletteFade.active)
            return;
        BattleTransition_StartOnField(B_TRANSITION_BLUR);
        gTasks[taskId].data[0]++;
    }
    else if (IsBattleTransitionDone())
    {
        // Do not invoke follower-partner setup or any facility challenge script.
        CleanupOverworldWindowsAndTilemaps();
        gMain.savedCallback = CB2_EndPracticeBattle;
        SetMainCallback2(CB2_InitBattle);
        DestroyTask(taskId);
    }
}

static void Task_PracticeResumeScript(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void Special_FrontierPracticeStart(void)
{
    u8 task = CreateTask(Task_PracticeResumeScript, 1);
    gSpecialVar_Result = FrontierPractice_PrepareBattle();
    if (gSpecialVar_Result == PRACTICE_OK)
    {
        gTasks[task].func = Task_EnterPracticeBattle;
        PlayMapChosenOrBattleBGM(0);
    }
}

void Special_FrontierPracticeInit(void)
{
    if (!sPractice.initialized)
        FrontierPractice_Reset();
    gSpecialVar_Result = sPractice.count;
}

void Special_FrontierPracticeAddSelection(void)
{
    gSpecialVar_Result = gSpecialVar_Result && gSpecialVar_0x8004 == PC_MON_CHOSEN
        ? FrontierPractice_AddBoxMon(gSpecialVar_MonBoxId, gSpecialVar_MonBoxPos) : PRACTICE_INVALID_MON;
}

void Special_FrontierPracticeRemove(void)
{
    gSpecialVar_Result = FrontierPractice_RemoveMon(gSpecialVar_0x8004);
}

void Special_FrontierPracticeClear(void)
{
    if (sPractice.backup == NULL)
        sPractice.count = 0;
}

void Special_FrontierPracticeSetOption(void)
{
    u32 ai = sPractice.ai, doubles = sPractice.doubles, level50 = sPractice.level50;
    switch (gSpecialVar_0x8004)
    {
    case PRACTICE_OPTION_AI: ai = gSpecialVar_0x8005; break;
    case PRACTICE_OPTION_DOUBLE: doubles = gSpecialVar_0x8005; break;
    case PRACTICE_OPTION_LEVEL50: level50 = gSpecialVar_0x8005; break;
    default: gSpecialVar_Result = FALSE; return;
    }
    gSpecialVar_Result = FrontierPractice_SetOptions(ai, doubles, level50);
}

void Special_FrontierPracticeBufferSettings(void)
{
    ConvertIntToDecimalStringN(gStringVar1, sPractice.count, STR_CONV_MODE_LEFT_ALIGN, 1);
    StringCopy(gStringVar2, sAiNames[sPractice.ai]);
    StringCopy(gStringVar3, sPractice.doubles ? COMPOUND_STRING("{JPN}ダブル") : COMPOUND_STRING("{JPN}シングル"));
    StringAppend(gStringVar3, sPractice.level50 ? COMPOUND_STRING(" Lv50") : COMPOUND_STRING("{JPN} レベルそのまま"));
    gSpecialVar_Result = sPractice.count;
}

void Special_FrontierPracticeBufferTeam(void)
{
    u8 nickname[POKEMON_NAME_BUFFER_SIZE];
    u32 start = gSpecialVar_0x8004 == 0 ? 0 : 3;
    StringCopy(gStringVar4, COMPOUND_STRING("{JPN}れんしゅう あいて\n"));
    for (u32 i = start; i < min(start + 3, sPractice.count); i++)
    {
        u8 number[4];
        ConvertIntToDecimalStringN(number, i + 1, STR_CONV_MODE_LEFT_ALIGN, 1);
        StringAppend(gStringVar4, number);
        StringAppend(gStringVar4, COMPOUND_STRING(" "));
        if (ReferenceMatches(i))
            GetBoxMonData(GetBoxedMonPtr(sPractice.mons[i].box, sPractice.mons[i].slot), MON_DATA_NICKNAME, nickname);
        else
            StringCopy(nickname, COMPOUND_STRING("{JPN}えらびなおしてね"));
        StringAppend(gStringVar4, nickname);
        if (i + 1 < min(start + 3, sPractice.count))
            StringAppend(gStringVar4, COMPOUND_STRING("\l"));
    }
}
