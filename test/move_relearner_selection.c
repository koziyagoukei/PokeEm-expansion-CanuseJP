#include "global.h"
#include "event_data.h"
#include "move_relearner.h"
#include "party_menu.h"
#include "pokemon.h"
#include "string_util.h"
#include "strings.h"
#include "test/test.h"
#include "constants/party_menu.h"

static const u8 *const sTypeLabels[MOVE_RELEARNER_COUNT] = {
    [MOVE_RELEARNER_LEVEL_UP_MOVES] = MoveRelearner_Text_LevelUpMoveLWR,
    [MOVE_RELEARNER_EGG_MOVES] = MoveRelearner_Text_EggMoveLWR,
    [MOVE_RELEARNER_TM_MOVES] = MoveRelearner_Text_TMMoveLWR,
    [MOVE_RELEARNER_TUTOR_MOVES] = MoveRelearner_Text_TutorMoveLWR,
    [MOVE_RELEARNER_FRONTIER_FULL_MOVES] = MoveRelearner_Text_FrontierFullMoveLWR,
};

TEST("Move relearner selection: summary cannot replace the NPC's selected category or label")
{
    enum MoveRelearnerStates state = MOVE_RELEARNER_LEVEL_UP_MOVES;
    for (u32 i = 0; i < MOVE_RELEARNER_COUNT; i++)
        PARAMETRIZE { state = i; }
    gMoveRelearnerState = state;
    gRelearnMode = RELEARN_MODE_SCRIPT;
    MoveRelearner_SaveMonSelectionContext();
    gMoveRelearnerState = MOVE_RELEARNER_FRONTIER_FULL_MOVES;
    gRelearnMode = RELEARN_MODE_NONE;
    ConvertIntToDecimalStringN(gStringVar3, 123, STR_CONV_MODE_LEFT_ALIGN, 3);
    MoveRelearner_RestoreMonSelectionContext();
    EXPECT_EQ(gMoveRelearnerState, state);
    EXPECT_EQ(gRelearnMode, RELEARN_MODE_SCRIPT);
    EXPECT_EQ(StringCompare(gStringVar3, sTypeLabels[state]), 0);
}

TEST("Move relearner selection: repeated summary visits preserve each newly chosen category")
{
    for (u32 i = 0; i < MOVE_RELEARNER_COUNT; i++)
    {
        gMoveRelearnerState = i;
        gRelearnMode = RELEARN_MODE_SCRIPT;
        MoveRelearner_SaveMonSelectionContext();
        gMoveRelearnerState = MOVE_RELEARNER_LEVEL_UP_MOVES;
        StringCopy(gStringVar3, COMPOUND_STRING("{JPN}いじっぱり"));
        MoveRelearner_RestoreMonSelectionContext();
        EXPECT_EQ(gMoveRelearnerState, i);
        EXPECT_EQ(StringCompare(gStringVar3, sTypeLabels[i]), 0);
        MoveRelearner_SaveMonSelectionContext();
        gMoveRelearnerState = MOVE_RELEARNER_FRONTIER_FULL_MOVES;
        MoveRelearner_RestoreMonSelectionContext();
        EXPECT_EQ(gMoveRelearnerState, i);
    }
}

TEST("Move relearner selection: inactive context leaves normal summary defaults and text unchanged")
{
    MoveRelearner_RestoreMonSelectionContext();
    gMoveRelearnerState = MOVE_RELEARNER_FRONTIER_FULL_MOVES;
    gRelearnMode = RELEARN_MODE_PSS_PAGE_BATTLE_MOVES;
    StringCopy(gStringVar3, COMPOUND_STRING("{JPN}フロンティアわざ"));
    MoveRelearner_RestoreMonSelectionContext();
    EXPECT_EQ(gMoveRelearnerState, MOVE_RELEARNER_FRONTIER_FULL_MOVES);
    EXPECT_EQ(gRelearnMode, RELEARN_MODE_PSS_PAGE_BATTLE_MOVES);
    EXPECT_EQ(StringCompare(gStringVar3, COMPOUND_STRING("{JPN}フロンティアわざ")), 0);
}

TEST("Move relearner selection: completed context is consumed and cannot overwrite a later screen")
{
    gMoveRelearnerState = MOVE_RELEARNER_EGG_MOVES;
    gRelearnMode = RELEARN_MODE_SCRIPT;
    MoveRelearner_SaveMonSelectionContext();
    MoveRelearner_RestoreMonSelectionContext();
    gMoveRelearnerState = MOVE_RELEARNER_FRONTIER_FULL_MOVES;
    gRelearnMode = RELEARN_MODE_PSS_PAGE_CONTEST_MOVES;
    StringCopy(gStringVar3, COMPOUND_STRING("{JPN}テスト"));
    MoveRelearner_RestoreMonSelectionContext();
    EXPECT_EQ(gMoveRelearnerState, MOVE_RELEARNER_FRONTIER_FULL_MOVES);
    EXPECT_EQ(gRelearnMode, RELEARN_MODE_PSS_PAGE_CONTEST_MOVES);
    EXPECT_EQ(StringCompare(gStringVar3, COMPOUND_STRING("{JPN}テスト")), 0);
}

TEST("Move relearner selection: summary round trip does not change party or PC selection coordinates")
{
    gMoveRelearnerState = MOVE_RELEARNER_TM_MOVES;
    gRelearnMode = RELEARN_MODE_SCRIPT;
    gSpecialVar_0x8004 = PC_MON_CHOSEN;
    gSpecialVar_MonBoxId = 5;
    gSpecialVar_MonBoxPos = 12;
    MoveRelearner_SaveMonSelectionContext();
    gMoveRelearnerState = MOVE_RELEARNER_FRONTIER_FULL_MOVES;
    MoveRelearner_RestoreMonSelectionContext();
    EXPECT_EQ(gSpecialVar_0x8004, PC_MON_CHOSEN);
    EXPECT_EQ(gSpecialVar_MonBoxId, 5);
    EXPECT_EQ(gSpecialVar_MonBoxPos, 12);
}

TEST("Move relearner selection: invalid category cannot index the label table")
{
    gMoveRelearnerState = MOVE_RELEARNER_COUNT;
    MoveRelearner_SaveMonSelectionContext();
    gMoveRelearnerState = MOVE_RELEARNER_FRONTIER_FULL_MOVES;
    MoveRelearner_RestoreMonSelectionContext();
    EXPECT_EQ(gMoveRelearnerState, MOVE_RELEARNER_FRONTIER_FULL_MOVES);
}

TEST("Move relearner selection: forgetting removes only the chosen move and preserves remaining PP")
{
    static const enum Move originalMoves[] = {MOVE_TACKLE, MOVE_GROWL, MOVE_SURF, MOVE_PROTECT};
    u32 numMoves = 4, forgottenSlot = 0;
    u8 bonuses = 0x39;
    struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][0];

    for (u32 count = 2; count <= MAX_MON_MOVES; count++)
        for (u32 slot = 0; slot < count; slot++)
            PARAMETRIZE { numMoves = count; forgottenSlot = slot; }

    CreateMon(mon, SPECIES_SMEARGLE, 50, 0, OTID_STRUCT_PRESET(0));
    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        u8 pp = i < numMoves ? i + 1 : 0;
        SetMonMoveSlot(mon, i < numMoves ? originalMoves[i] : MOVE_NONE, i);
        SetMonData(mon, MON_DATA_PP1 + i, &pp);
    }
    SetMonData(mon, MON_DATA_PP_BONUSES, &bonuses);
    gSpecialVar_0x8004 = 0;
    gSpecialVar_0x8005 = forgottenSlot;
    MoveDeleterForgetMove();

    for (u32 i = 0; i < MAX_MON_MOVES; i++)
    {
        if (i < numMoves - 1)
        {
            u32 oldSlot = i < forgottenSlot ? i : i + 1;
            EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + i), originalMoves[oldSlot]);
            EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + i), oldSlot + 1);
            EXPECT_EQ((GetMonData(mon, MON_DATA_PP_BONUSES) >> (2 * i)) & 3,
                      (bonuses >> (2 * oldSlot)) & 3);
        }
        else
        {
            EXPECT_EQ(GetMonData(mon, MON_DATA_MOVE1 + i), MOVE_NONE);
            EXPECT_EQ(GetMonData(mon, MON_DATA_PP1 + i), 0);
        }
    }
}
