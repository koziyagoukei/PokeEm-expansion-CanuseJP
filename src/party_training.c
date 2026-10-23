#include "global.h"
#include "event_data.h"
#include "international_string_util.h"
#include "map_name_popup.h"
#include "main.h"
#include "menu.h"
#include "party_training.h"
#include "pokemon.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "constants/songs.h"

#define tSlot data[6]
#define tRow data[7]
#define tEditing data[8]
#define tValue data[9]
#define tDigit data[10]
#define tWindow data[11]
#define TRAINING_SAVE_ROW NUM_STATS
#define TRAINING_CANCEL_ROW (NUM_STATS + 1)

static const u8 sStatOrder[NUM_STATS] = {STAT_HP, STAT_ATK, STAT_DEF, STAT_SPATK, STAT_SPDEF, STAT_SPEED};
static const u8 *const sStatNames[NUM_STATS] = {gText_HP3, gText_Attack, gText_Defense, gText_SpAtk, gText_SpDef, gText_Speed};
static const u8 sText_Save[] = _("{JPN}けってい");
static const u8 sText_Cancel[] = _("{JPN}やめる");

bool8 PartyTraining_ApplyEVs(u16 partySlot, const u16 *evs)
{
    u32 stat, total = 0;
    struct Pokemon *mon;

    if (partySlot >= PARTY_SIZE || evs == NULL)
        return FALSE;
    mon = &gParties[B_TRAINER_PLAYER][partySlot];
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
        return FALSE;
    for (stat = 0; stat < NUM_STATS; stat++)
    {
        if (evs[stat] > MAX_PER_STAT_EVS)
            return FALSE;
        total += evs[stat];
    }
    if (total > MAX_TOTAL_EVS)
        return FALSE;
    for (stat = 0; stat < NUM_STATS; stat++)
    {
        u8 value = evs[stat];
        SetMonData(mon, MON_DATA_HP_EV + stat, &value);
    }
    CalculateMonStats(mon);
    return TRUE;
}

static u16 GetTrainingTotal(u8 taskId)
{
    u32 stat, total = 0;
    for (stat = 0; stat < NUM_STATS; stat++)
        total += gTasks[taskId].data[stat];
    return total;
}

static void PrintTrainingText(u8 window, const u8 *text, u8 x, u8 y)
{
    AddTextPrinterParameterized(window, FONT_NORMAL, text, x, y, TEXT_SKIP_DRAW, NULL);
}

static void DrawTrainingMenu(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    u8 window = task->tWindow;
    u8 text[32];
    u32 row;
    u16 total = GetTrainingTotal(taskId);

    FillWindowPixelBuffer(window, PIXEL_FILL(1));
    SetStandardWindowBorderStyle(window, FALSE);
    GetMonData(&gParties[B_TRAINER_PLAYER][task->tSlot], MON_DATA_NICKNAME, text);
    PrintTrainingText(window, text, 0, 0);
    if (task->tEditing)
        total = total - task->data[sStatOrder[task->tRow]] + task->tValue;
    StringCopy(text, COMPOUND_STRING("{JPN}ごうけい "));
    ConvertIntToDecimalStringN(text + StringLength(text), total, STR_CONV_MODE_RIGHT_ALIGN, 3);
    StringAppend(text, COMPOUND_STRING("/510"));
    PrintTrainingText(window, text, GetStringRightAlignXOffset(FONT_NORMAL, text, 208), 0);
    for (row = 0; row < NUM_STATS; row++)
    {
        u16 value = task->tEditing && task->tRow == row ? task->tValue : task->data[sStatOrder[row]];
        PrintTrainingText(window, sStatNames[row], 12, 16 + row * 16);
        ConvertIntToDecimalStringN(text, value, STR_CONV_MODE_LEADING_ZEROS, 3);
        PrintTrainingText(window, text, 176, 16 + row * 16);
    }
    PrintTrainingText(window, sText_Save, 12, 16 + TRAINING_SAVE_ROW * 16);
    PrintTrainingText(window, sText_Cancel, 12, 16 + TRAINING_CANCEL_ROW * 16);
    PrintTrainingText(window, gText_SelectorArrow2, 0, 16 + task->tRow * 16);
    if (task->tEditing)
        FillWindowPixelRect(window, PIXEL_FILL(2), 176 + (2 - task->tDigit) * 6, 30 + task->tRow * 16, 5, 1);
    CopyWindowToVram(window, COPYWIN_FULL);
}

static void CloseTrainingMenu(u8 taskId, bool8 apply)
{
    u16 evs[NUM_STATS];
    u32 stat;
    gSpecialVar_Result = FALSE;
    if (apply)
    {
        for (stat = 0; stat < NUM_STATS; stat++)
            evs[stat] = gTasks[taskId].data[stat];
        gSpecialVar_Result = PartyTraining_ApplyEVs(gTasks[taskId].tSlot, evs);
        if (!gSpecialVar_Result)
        {
            PlaySE(SE_FAILURE);
            return;
        }
    }
    PlaySE(SE_SELECT);
    ClearStdWindowAndFrameToTransparent(gTasks[taskId].tWindow, TRUE);
    RemoveWindow(gTasks[taskId].tWindow);
    DestroyTask(taskId);
    ScriptContext_Enable();
}

static void Task_PartyTrainingInput(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    if (task->tEditing)
    {
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            if (JOY_NEW(A_BUTTON))
                task->data[sStatOrder[task->tRow]] = task->tValue;
            task->tEditing = FALSE;
        }
        else if (JOY_REPEAT(DPAD_LEFT | DPAD_RIGHT))
            task->tDigit = (task->tDigit + (JOY_REPEAT(DPAD_LEFT) ? 1 : 2)) % 3;
        else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
        {
            static const u16 steps[] = {1, 10, 100};
            s32 value = task->tValue + (JOY_REPEAT(DPAD_UP) ? steps[task->tDigit] : -steps[task->tDigit]);
            s32 remaining = MAX_TOTAL_EVS - GetTrainingTotal(taskId) + task->data[sStatOrder[task->tRow]];
            task->tValue = max(0, min(value, min(MAX_PER_STAT_EVS, remaining)));
        }
        else
            return;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        CloseTrainingMenu(taskId, FALSE);
        return;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        if (task->tRow >= NUM_STATS)
        {
            CloseTrainingMenu(taskId, task->tRow == TRAINING_SAVE_ROW);
            return;
        }
        task->tValue = task->data[sStatOrder[task->tRow]];
        task->tDigit = 0;
        task->tEditing = TRUE;
    }
    else if (JOY_REPEAT(DPAD_UP | DPAD_DOWN))
        task->tRow = (task->tRow + (JOY_REPEAT(DPAD_UP) ? TRAINING_CANCEL_ROW : 1)) % (NUM_STATS + 2);
    else
        return;
    PlaySE(SE_SELECT);
    DrawTrainingMenu(taskId);
}

static void Task_PartyTrainingResume(u8 taskId)
{
    DestroyTask(taskId);
    ScriptContext_Enable();
}

void Special_OpenPartyTraining(void)
{
    const struct WindowTemplate window = {0, 2, 1, 26, 18, 15, 64};
    u8 taskId = CreateTask(Task_PartyTrainingResume, 8);
    struct Task *task = &gTasks[taskId];
    struct Pokemon *mon;
    u32 stat;

    gSpecialVar_Result = FALSE;
    if (gSpecialVar_0x8004 >= PARTY_SIZE)
        return;
    mon = &gParties[B_TRAINER_PLAYER][gSpecialVar_0x8004];
    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(mon, MON_DATA_IS_EGG))
        return;
    HideMapNamePopUpWindow();
    LoadMessageBoxAndBorderGfx();
    task->tWindow = AddWindow(&window);
    if (task->tWindow == WINDOW_NONE)
        return;
    task->tSlot = gSpecialVar_0x8004;
    for (stat = 0; stat < NUM_STATS; stat++)
        task->data[stat] = GetMonData(mon, MON_DATA_HP_EV + stat);
    task->func = Task_PartyTrainingInput;
    DrawTrainingMenu(taskId);
}
