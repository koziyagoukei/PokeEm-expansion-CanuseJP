#include "global.h"
#include "battle.h"
#include "contest.h"
#include "contest_effect.h"
#include "contest_npc.h"
#include "event_data.h"
#include "frontier_tutor.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "constants/form_change_types.h"
#include "test/link.h"
#include "test/test.h"

extern const struct ContestPokemon gContestOpponents[];

static void InitContest(void)
{
    gContestResources = AllocZeroed(sizeof(*gContestResources));
    gContestResources->contest = AllocZeroed(sizeof(struct Contest));
    gContestResources->status = AllocZeroed(CONTESTANT_COUNT * sizeof(struct ContestantStatus));
    gContestResources->gimmickStatus = AllocZeroed(CONTESTANT_COUNT * sizeof(struct ContestGimmickStatus));
    gContestResources->appealResults = AllocZeroed(sizeof(struct ContestAppealMoveResults));
    gContestResources->excitement = AllocZeroed(sizeof(struct ContestExcitement));
    gContestResources->aiData = AllocZeroed(sizeof(struct ContestAIInfo));
    gLinkContestFlags = 0;
    gContestPlayerMonIndex = 3;
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
    {
        gContestMons[i] = (struct ContestPokemon){.species = SPECIES_PIKACHU};
        gContestantTurnOrder[i] = i;
        eContest.prevTurnOrder[i] = i;
        eContestAppealResults.turnOrder[i] = i;
        eContestantStatus[i].effectStringId = CONTEST_STRING_NONE;
        eContestantStatus[i].effectStringId2 = CONTEST_STRING_NONE;
    }
}

static void FreeContest(void)
{
    FREE_AND_SET_NULL(gContestResources->contest);
    FREE_AND_SET_NULL(gContestResources->status);
    FREE_AND_SET_NULL(gContestResources->gimmickStatus);
    FREE_AND_SET_NULL(gContestResources->appealResults);
    FREE_AND_SET_NULL(gContestResources->excitement);
    FREE_AND_SET_NULL(gContestResources->aiData);
    FREE_AND_SET_NULL(gContestResources);
    gLinkContestFlags = 0;
}

static bool32 CanLearn(enum Species species, enum Move move)
{
    const u16 *learnset = GetFrontierFullLearnset(species);
    const struct Fusion *fusion = gFusionTablePointers[species];
    if (learnset != NULL)
        for (u32 i = 0; learnset[i] != MOVE_UNAVAILABLE; i++)
            if (learnset[i] == move)
                return TRUE;
    // Fused Necrozma learns its signature move through the actual fusion table.
    if (fusion != NULL)
        for (u32 i = 0; fusion[i].fusionStorageIndex != FUSION_TERMINATOR; i++)
            if (fusion[i].fusingIntoMon == species && fusion[i].fusionMove == move)
                return TRUE;
    return FALSE;
}

static enum ContestCategories FirstCategory(const struct ContestNpcPreset *preset)
{
    for (u32 i = 0; i < CONTEST_CATEGORIES_COUNT; i++)
        if (preset->categories & (1 << i))
            return i;
    return CONTEST_CATEGORIES_COUNT;
}

static u32 FindPreset(enum Species species, enum ContestGimmick gimmick)
{
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
        if (Contest_GetNpcPreset(i)->species == species && Contest_GetNpcPreset(i)->gimmick == gimmick)
            return i;
    return Contest_GetNpcPresetCount();
}

TEST("Contest NPC: every preset has four legal base moves and an explicit gimmick")
{
    u32 opponent = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
        PARAMETRIZE { opponent = i; }
    const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
    EXPECT_EQ(Contest_GetNpcPresetCount(), 96);
    EXPECT(Contest_GetNpcPreset(96) == NULL);
    EXPECT_GT(preset->species, SPECIES_NONE);
    EXPECT_LT(preset->species, NUM_SPECIES);
    EXPECT_GT(preset->gimmick, CONTEST_GIMMICK_NONE);
    EXPECT_LT(preset->gimmick, CONTEST_GIMMICK_COUNT);
    EXPECT_GE(preset->activationTurn, 1);
    EXPECT_LE(preset->activationTurn, CONTEST_NUM_APPEALS);
    EXPECT_NE(preset->categories, 0);
    for (u32 slot = 0; slot < MAX_MON_MOVES; slot++)
    {
        enum Move move = preset->moves[slot];
        EXPECT_GT(move, MOVE_NONE);
        EXPECT_LT(move, MOVES_COUNT);
        EXPECT_EQ(CanLearn(preset->species, move) ? MOVE_NONE : move, MOVE_NONE);
        EXPECT_LT(GetMoveContestCategory(move), CONTEST_CATEGORIES_COUNT);
        for (u32 other = 0; other < slot; other++)
            EXPECT_NE(move, preset->moves[other]);
    }
}

TEST("Contest NPC: each category and rank has at least three opponents before and after the story")
{
    u32 rank = 0, category = 0, postgame = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            for (u32 p = 0; p < 2; p++)
                PARAMETRIZE { rank = r; category = c; postgame = p; }
    u32 count = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
    {
        if (gContestOpponents[i].whichRank != rank)
            continue;
        if ((postgame && gContestOpponents[i].filter == CONTEST_FILTER_NO_POSTGAME)
            || (!postgame && gContestOpponents[i].filter == CONTEST_FILTER_ONLY_POSTGAME))
            continue;
        if (Contest_GetNpcPreset(i)->categories & (1 << category))
            count++;
    }
    EXPECT_GE(count, CONTESTANT_COUNT - 1);
}

TEST("Contest NPC: metadata survives initialization and planned gimmick is selected on its turn")
{
    u32 opponent = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
        PARAMETRIZE { opponent = i; }
    const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
    InitContest();
    gSpecialVar_ContestCategory = FirstCategory(preset);
    Test_ContestLoadNpc(0, opponent, gSpecialVar_ContestCategory);
    EXPECT_EQ(eContestGimmickStatus[0].heldItem, preset->heldItem);
    EXPECT_EQ(eContestGimmickStatus[0].teraType, gContestMons[0].teraType);
    EXPECT(eContestGimmickStatus[0].gmaxFactor == preset->gmaxFactor);
    EXPECT_EQ(eContestGimmickStatus[0].originalSpecies, preset->species);
    EXPECT_EQ(eContestGimmickStatus[0].dynamaxLevel, MAX_DYNAMAX_LEVEL);
    if (preset->activationTurn > 1)
    {
        Test_ContestChooseNpcMove(0);
        EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    }
    eContest.appealNumber = preset->activationTurn - 1;
    u8 move = Test_ContestChooseNpcMove(0);
    EXPECT_LT(move, MAX_MON_MOVES);
    EXPECT_EQ(eContestGimmickStatus[0].selected, preset->gimmick);
    eContestantStatus[0].currMove = gContestMons[0].moves[move];
    Test_ContestActivateGimmick(0);
    EXPECT_EQ(eContestGimmickStatus[0].moveGimmick, preset->gimmick);
    EXPECT_EQ(eContestGimmickStatus[0].usedGimmick, preset->gimmick);
    if (preset->gimmick == CONTEST_GIMMICK_Z_APPEAL)
        EXPECT(eContestGimmickStatus[0].zUsed);
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_TERA, MOVE_TACKLE));
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_DYNAMAX, MOVE_TACKLE));
    FreeContest();
}

TEST("Contest NPC: Tera category matches the entered contest even for multi-category entrants")
{
    u32 opponent = 0, category = 0;
    for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
    {
        const struct ContestNpcPreset *preset = Contest_GetNpcPreset(i);
        if (preset->gimmick != CONTEST_GIMMICK_TERA || preset->teraType == TYPE_STELLAR)
            continue;
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            if (preset->categories & (1 << c))
                PARAMETRIZE { opponent = i; category = c; }
    }
    InitContest();
    gSpecialVar_ContestCategory = category;
    Test_ContestLoadNpc(0, opponent, category);
    eContest.appealNumber = Contest_GetNpcPreset(opponent)->activationTurn - 1;
    u8 move = Test_ContestChooseNpcMove(0);
    eContestantStatus[0].currMove = gContestMons[0].moves[move];
    Test_CalculateAppealMoveImpact(0);
    EXPECT(eContestGimmickStatus[0].teraActive);
    EXPECT_EQ(eContestantStatus[0].moveCategory, category);
    FreeContest();
}

TEST("Contest NPC: normal entry loads three distinct presets in every category and rank")
{
    u32 rank = 0, category = 0;
    for (u32 r = CONTEST_RANK_NORMAL; r <= CONTEST_RANK_MASTER; r++)
        for (u32 c = 0; c < CONTEST_CATEGORIES_COUNT; c++)
            PARAMETRIZE { rank = r; category = c; }
    InitContest();
    CreateMon(&gParties[B_TRAINER_PLAYER][0], SPECIES_PIKACHU, 50, 0, OTID_STRUCT_PLAYER_ID);
    gContestMonPartyIndex = 0;
    gSpecialVar_ContestCategory = category;
    SetContestants(category, rank);
    EXPECT_EQ(gContestPlayerMonIndex, 3);
    for (u32 slot = 0; slot < CONTESTANT_COUNT - 1; slot++)
    {
        u32 opponent = Contest_GetNpcPresetCount();
        for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
            if (memcmp(gContestMons[slot].trainerName, gContestOpponents[i].trainerName, sizeof(gContestMons[slot].trainerName)) == 0)
                opponent = i;
        EXPECT_LT(opponent, Contest_GetNpcPresetCount());
        const struct ContestNpcPreset *preset = Contest_GetNpcPreset(opponent);
        EXPECT_EQ(gContestMons[slot].species, preset->species);
        EXPECT_EQ(gContestMons[slot].heldItem, preset->heldItem);
        EXPECT(gContestMons[slot].whichRank == rank);
        EXPECT(preset->categories & (1 << category));
        EXPECT(memcmp(gContestMons[slot].moves, preset->moves, sizeof(preset->moves)) == 0);
        for (u32 other = 0; other < slot; other++)
            EXPECT(memcmp(gContestMons[slot].trainerName, gContestMons[other].trainerName, sizeof(gContestMons[slot].trainerName)) != 0);
    }
    FreeContest();
}

TEST("Contest NPC: Mega rewards matching-category appeals unless the entrant was jammed")
{
    bool32 jammed = FALSE;
    PARAMETRIZE { jammed = FALSE; }
    PARAMETRIZE { jammed = TRUE; }
    InitContest();
    Test_ContestLoadNpc(0, FindPreset(SPECIES_ABSOL, CONTEST_GIMMICK_MEGA), CONTEST_CATEGORY_COOL);
    eContestantStatus[0].currMove = MOVE_SLASH;
    eContestGimmickStatus[0].selected = CONTEST_GIMMICK_MEGA;
    eContestGimmickStatus[0].jammedSinceLastAppeal = jammed;
    Test_CalculateAppealMoveImpact(0);
    EXPECT(eContestGimmickStatus[0].megaActive);
    EXPECT_EQ(eContestantStatus[0].appeal, gContestEffects[GetMoveContestEffect(MOVE_SLASH)].appeal + (jammed ? 0 : 20));
    FreeContest();
}

TEST("Contest NPC: Ultra Burst consumes a turn then allows one signature Z appeal")
{
    InitContest();
    Test_ContestLoadNpc(0, FindPreset(SPECIES_NECROZMA_DUSK_MANE, CONTEST_GIMMICK_ULTRA_BURST), CONTEST_CATEGORY_COOL);
    u8 move = Test_ContestChooseNpcMove(0);
    eContestantStatus[0].currMove = gContestMons[0].moves[move];
    Test_CalculateAppealMoveImpact(0);
    EXPECT(eContestGimmickStatus[0].ultraActive);
    EXPECT_EQ(eContestantStatus[0].currMove, MOVE_NONE);
    EXPECT_EQ(eContestantStatus[0].appeal, 0);
    // The cutaway commits the pending form once the transformation animation ends.
    gContestMons[0].species = SPECIES_NECROZMA_ULTRA;
    eContest.appealNumber = 1;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    eContest.appealNumber = 2;
    move = Test_ContestChooseNpcMove(0);
    EXPECT_EQ(gContestMons[0].moves[move], MOVE_PHOTON_GEYSER);
    eContestantStatus[0].currMove = MOVE_PHOTON_GEYSER;
    Test_CalculateAppealMoveImpact(0);
    EXPECT_EQ(eContestGimmickStatus[0].moveGimmick, CONTEST_GIMMICK_Z_APPEAL);
    EXPECT(eContestGimmickStatus[0].zUsed);
    EXPECT_EQ(eContestantStatus[0].currMove, MOVE_PHOTON_GEYSER);
    EXPECT_EQ(eContestantStatus[0].appeal, gContestEffects[GetMoveContestEffect(MOVE_PHOTON_GEYSER)].appeal + 40);
    EXPECT_EQ(eContestExcitement.moveExcitement, GetMoveContestCategory(MOVE_PHOTON_GEYSER) == gSpecialVar_ContestCategory ? 2 : 1);
    eContest.appealNumber = 3;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    FreeContest();
}

TEST("Contest NPC: generic and signature Z appeals preserve base moves and apply bonuses only once")
{
    enum Species species = SPECIES_POOCHYENA;
    enum Move baseMove = MOVE_BITE;
    enum ContestCategories category = CONTEST_CATEGORY_TOUGH;
    bool32 signature = FALSE;
    PARAMETRIZE { species = SPECIES_POOCHYENA; baseMove = MOVE_BITE; category = CONTEST_CATEGORY_TOUGH; signature = FALSE; }
    PARAMETRIZE { species = SPECIES_EEVEE; baseMove = MOVE_LAST_RESORT; category = CONTEST_CATEGORY_CUTE; signature = TRUE; }
    InitContest();
    gSpecialVar_ContestCategory = category;
    Test_ContestLoadNpc(0, FindPreset(species, CONTEST_GIMMICK_Z_APPEAL), category);
    eContest.appealNumber = 2;
    eContestantStatus[0].currMove = baseMove;
    eContestGimmickStatus[0].selected = CONTEST_GIMMICK_Z_APPEAL;
    Test_CalculateAppealMoveImpact(0);
    EXPECT_EQ(eContestantStatus[0].currMove, baseMove);
    EXPECT_EQ(eContestantStatus[0].baseAppeal, gContestEffects[GetMoveContestEffect(baseMove)].appeal + 30);
    EXPECT_GE(eContestantStatus[0].appeal, eContestantStatus[0].baseAppeal + (signature ? 10 : 0));
    EXPECT(eContestGimmickStatus[0].zUsed);
    EXPECT_EQ(eContestExcitement.moveExcitement, GetMoveContestCategory(baseMove) == category ? 2 : 1);
    eContest.appealNumber = 3;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_DYNAMAX, baseMove));
    FreeContest();
}

TEST("Contest NPC: Gigantamax has three appeals, no jam, and cannot switch gimmicks afterward")
{
    InitContest();
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    Test_ContestLoadNpc(0, FindPreset(SPECIES_MACHAMP, CONTEST_GIMMICK_DYNAMAX), CONTEST_CATEGORY_COOL);
    for (u32 turn = 1; turn < 4; turn++)
    {
        eContest.appealNumber = turn;
        Test_ContestChooseNpcMove(0);
        eContestantStatus[0].currMove = MOVE_CROSS_CHOP;
        Test_CalculateAppealMoveImpact(0);
        EXPECT_EQ(eContestGimmickStatus[0].dynamaxTurns, 3 - turn);
        EXPECT_EQ(eContestantStatus[0].baseAppeal, GetMoveContestCategory(MOVE_CROSS_CHOP) == CONTEST_CATEGORY_COOL ? 40 : 30);
        EXPECT_EQ(eContestAppealResults.jam, 0);
        EXPECT_EQ(eContestAppealResults.jam2, 0);
        EXPECT_EQ(eContestGimmickStatus[1].extraAppeal, turn * 10);
    }
    eContest.appealNumber = 4;
    Test_ContestChooseNpcMove(0);
    EXPECT_EQ(eContestGimmickStatus[0].selected, CONTEST_GIMMICK_NONE);
    EXPECT(!Test_ContestGimmickCanSelect(0, CONTEST_GIMMICK_TERA, MOVE_CROSS_CHOP));
    FreeContest();
}

TEST("Contest NPC: linked NPCs keep their original roster and never select a gimmick")
{
    InitContest();
    gLinkContestFlags = LINK_CONTEST_FLAG_IS_LINK;
    gNumLinkContestPlayers = 2;
    gContestPlayerMonIndex = 0;
    SetLinkAIContestants(CONTEST_CATEGORY_COOL, CONTEST_RANK_MASTER, TRUE);
    for (u32 slot = 2; slot < CONTESTANT_COUNT; slot++)
    {
        bool32 found = FALSE;
        for (u32 i = 0; i < Contest_GetNpcPresetCount(); i++)
            if (gContestMons[slot].species == gContestOpponents[i].species
                && memcmp(gContestMons[slot].moves, gContestOpponents[i].moves, sizeof(gContestMons[slot].moves)) == 0)
                found = TRUE;
        EXPECT(found);
        eContest.appealNumber = 2;
        Test_ContestChooseNpcMove(slot);
        EXPECT_EQ(eContestGimmickStatus[slot].selected, CONTEST_GIMMICK_NONE);
        EXPECT_EQ(gContestMons[slot].heldItem, ITEM_NONE);
    }
    FreeContest();
}
