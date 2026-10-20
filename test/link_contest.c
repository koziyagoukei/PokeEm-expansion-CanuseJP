#include "global.h"
#include "battle.h"
#include "contest.h"
#include "contest_effect.h"
#include "contest_link.h"
#include "event_data.h"
#include "link.h"
#include "link_rfu.h"
#include "malloc.h"
#include "move.h"
#include "sprite.h"
#include "task.h"
#include "test/link.h"
#include "test/test.h"
#include "constants/items.h"

static void InitContest(u32 count)
{
    gContestResources = AllocZeroed(sizeof(*gContestResources));
    gContestResources->contest = AllocZeroed(sizeof(struct Contest));
    gContestResources->status = AllocZeroed(CONTESTANT_COUNT * sizeof(struct ContestantStatus));
    gContestResources->gimmickStatus = AllocZeroed(CONTESTANT_COUNT * sizeof(struct ContestGimmickStatus));
    gContestResources->appealResults = AllocZeroed(sizeof(struct ContestAppealMoveResults));
    gContestResources->excitement = AllocZeroed(sizeof(struct ContestExcitement));
    gLinkContestFlags = LINK_CONTEST_FLAG_IS_LINK;
    gNumLinkContestPlayers = count;
    gContestPlayerMonIndex = 0;
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
    {
        gContestMons[i] = (struct ContestPokemon){};
        struct LinkContestGimmickState state = {.species = SPECIES_PIKACHU, .originalSpecies = SPECIES_PIKACHU};
        Contest_SetLinkGimmickState(i, &state);
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
    FREE_AND_SET_NULL(gContestResources);
    gLinkContestFlags = 0;
}

TEST("Link audit: contest gameplay snapshot preserves local sprite IDs and animation caches")
{
    InitContest(4);
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
    {
        struct LinkContestGimmickState state =
        {
            .species = SPECIES_NECROZMA_DUSK_MANE,
            .originalSpecies = SPECIES_NECROZMA_DUSK_MANE,
            .pendingSpecies = SPECIES_NECROZMA_ULTRA,
            .heldItem = ITEM_ULTRANECROZIUM_Z,
            .extraAppeal = 30,
            .selected = CONTEST_GIMMICK_NONE,
            .moveGimmick = CONTEST_GIMMICK_ULTRA_BURST,
            .usedGimmick = CONTEST_GIMMICK_ULTRA_BURST,
            .teraType = TYPE_STELLAR,
            .dynamaxLevel = 10,
            .dynamaxTurns = 2,
            .gmaxFactor = TRUE,
            .megaActive = TRUE,
            .ultraActive = TRUE,
            .teraActive = TRUE,
            .gigantamaxActive = TRUE,
            .zUsed = TRUE,
            .stellarBoostUsed = TRUE,
            .jammedSinceLastAppeal = TRUE,
            .showGimmickIntro = TRUE,
        };
        struct LinkContestGimmickState received;
        eContestGimmickStatus[i].tabIconSpriteId = 20 + i;
        eContestGimmickStatus[i].animMove = MOVE_EXTREME_EVOBOOST;
        eContestGimmickStatus[i].effectMove = MOVE_LAST_RESORT;
        Contest_SetLinkGimmickState(i, &state);
        Contest_GetLinkGimmickState(i, &received);
        EXPECT_EQ(received.species, state.species);
        EXPECT_EQ(received.originalSpecies, state.originalSpecies);
        EXPECT_EQ(received.pendingSpecies, state.pendingSpecies);
        EXPECT_EQ(received.heldItem, state.heldItem);
        EXPECT_EQ(received.extraAppeal, state.extraAppeal);
        EXPECT_EQ(received.selected, state.selected);
        EXPECT_EQ(received.moveGimmick, state.moveGimmick);
        EXPECT_EQ(received.usedGimmick, state.usedGimmick);
        EXPECT_EQ(received.teraType, state.teraType);
        EXPECT_EQ(received.dynamaxLevel, state.dynamaxLevel);
        EXPECT_EQ(received.dynamaxTurns, state.dynamaxTurns);
        EXPECT(received.gmaxFactor == state.gmaxFactor);
        EXPECT(received.megaActive == state.megaActive);
        EXPECT(received.ultraActive == state.ultraActive);
        EXPECT(received.teraActive == state.teraActive);
        EXPECT(received.gigantamaxActive == state.gigantamaxActive);
        EXPECT(received.zUsed == state.zUsed);
        EXPECT(received.stellarBoostUsed == state.stellarBoostUsed);
        EXPECT(received.jammedSinceLastAppeal == state.jammedSinceLastAppeal);
        EXPECT(received.showGimmickIntro == state.showGimmickIntro);
        EXPECT_EQ(eContestGimmickStatus[i].tabIconSpriteId, 20 + i);
        EXPECT_EQ(eContestGimmickStatus[i].animMove, MOVE_EXTREME_EVOBOOST);
        EXPECT_EQ(eContestGimmickStatus[i].effectMove, MOVE_LAST_RESORT);
        state.pendingSpecies = SPECIES_NONE;
        state.zUsed = state.ultraActive = state.gigantamaxActive = FALSE;
        state.dynamaxTurns = 0;
        state.extraAppeal = 0;
        Contest_SetLinkGimmickState(i, &state);
        Contest_GetLinkGimmickState(i, &received);
        EXPECT_EQ(received.pendingSpecies, SPECIES_NONE);
        EXPECT(!received.zUsed && !received.ultraActive && !received.gigantamaxActive);
        EXPECT_EQ(received.dynamaxTurns, 0);
        EXPECT_EQ(received.extraAppeal, 0);
    }
    FreeContest();
}

TEST("Link audit: contest receives gimmick state from the elected leader with two to four humans")
{
    u32 count = 2;
    for (u32 n = 2; n <= 4; n++)
        PARAMETRIZE { count = n; }
    InitContest(count);
    ResetTasks();
    gContestLinkLeaderIndex = count - 1;
    u8 task = CreateTask(Task_LinkContest_CommunicateAppealsState, 0);
    gTasks[task].data[0] = 12;
    gWirelessCommType = TRUE;
    memset(&gRfu, 0, sizeof(gRfu));
    gRfu.playerCount = count;
    gSendCmd[0] = 1;
    gTasks[task].func(task);
    EXPECT_EQ(gTasks[task].data[0], 12);
    gSendCmd[0] = 0;
    gTasks[task].func(task);
    EXPECT_EQ(gTasks[task].data[0], 13);
    memset(&gRfu, 0, sizeof(gRfu));
    memset(gSendCmd, 0, sizeof(gSendCmd));
    gWirelessCommType = FALSE;
    gLinkStatus = count << LINK_STAT_PLAYER_COUNT_SHIFT;
    ResetBlockReceivedFlags();
    for (u32 player = 0; player < count; player++)
    {
        struct LinkContestGimmickState *states = (void *)gBlockRecvBuffer[player];
        memset(states, 0, CONTESTANT_COUNT * sizeof(*states));
        for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        {
            states[i].species = player == gContestLinkLeaderIndex ? SPECIES_BEEDRILL : SPECIES_PIKACHU;
            states[i].pendingSpecies = player == gContestLinkLeaderIndex ? SPECIES_BEEDRILL_MEGA : SPECIES_NONE;
            states[i].moveGimmick = CONTEST_GIMMICK_MEGA;
            states[i].usedGimmick = CONTEST_GIMMICK_MEGA;
            states[i].megaActive = TRUE;
            states[i].extraAppeal = 10 + i;
        }
        gBlockReceivedStatus[player] = TRUE;
    }
    gTasks[task].func(task);
    EXPECT_EQ(gTasks[task].data[0], 14);
    EXPECT_EQ(GetBlockReceivedStatus(), 0);
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
    {
        struct LinkContestGimmickState state;
        Contest_GetLinkGimmickState(i, &state);
        EXPECT_EQ(state.species, SPECIES_BEEDRILL);
        EXPECT_EQ(state.pendingSpecies, SPECIES_BEEDRILL_MEGA);
        EXPECT_EQ(state.moveGimmick, CONTEST_GIMMICK_MEGA);
        EXPECT_EQ(state.extraAppeal, 10 + i);
        EXPECT(state.megaActive);
        EXPECT(!Test_ContestGimmickCanSelect(i, CONTEST_GIMMICK_TERA, MOVE_TACKLE));
    }
    ResetTasks();
    FreeContest();
}

TEST("Link audit: remote Ultra Burst users can Z appeal regardless of the local contestant index")
{
    u32 count = 2;
    for (u32 n = 2; n <= 4; n++)
        PARAMETRIZE { count = n; }
    InitContest(count);
    for (u32 local = 0; local < count; local++)
    {
        gContestPlayerMonIndex = local;
        for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        {
            struct LinkContestGimmickState state =
            {
                .species = SPECIES_NECROZMA_ULTRA,
                .heldItem = ITEM_ULTRANECROZIUM_Z,
                .usedGimmick = CONTEST_GIMMICK_ULTRA_BURST,
                .ultraActive = TRUE,
            };
            Contest_SetLinkGimmickState(i, &state);
            EXPECT_EQ(Test_ContestGimmickCanSelect(i, CONTEST_GIMMICK_Z_APPEAL, MOVE_PHOTON_GEYSER), i < count);
            EXPECT(!Test_ContestGimmickCanSelect(i, CONTEST_GIMMICK_DYNAMAX, MOVE_PHOTON_GEYSER));
            state.zUsed = TRUE;
            Contest_SetLinkGimmickState(i, &state);
            EXPECT(!Test_ContestGimmickCanSelect(i, CONTEST_GIMMICK_Z_APPEAL, MOVE_PHOTON_GEYSER));
        }
    }
    FreeContest();
}

TEST("Link audit: signature Z contest effects use the base move and consume Z only once")
{
    InitContest(4);
    u32 contestant = 2;
    struct LinkContestGimmickState state =
    {
        .species = SPECIES_EEVEE,
        .originalSpecies = SPECIES_EEVEE,
        .heldItem = ITEM_EEVIUM_Z,
        .selected = CONTEST_GIMMICK_Z_APPEAL,
    };
    Contest_SetLinkGimmickState(contestant, &state);
    eContestantStatus[contestant].currMove = MOVE_LAST_RESORT;
    gSpecialVar_ContestCategory = GetMoveContestCategory(MOVE_LAST_RESORT);
    Test_CalculateAppealMoveImpact(contestant);
    EXPECT_EQ(eContestantStatus[contestant].currMove, MOVE_LAST_RESORT);
    EXPECT_EQ(eContestantStatus[contestant].baseAppeal, gContestEffects[GetMoveContestEffect(MOVE_LAST_RESORT)].appeal + 30);
    EXPECT_EQ(eContestExcitement.moveExcitement, 2);
    EXPECT(eContestGimmickStatus[contestant].zUsed);
    EXPECT_EQ(eContestGimmickStatus[contestant].moveGimmick, CONTEST_GIMMICK_Z_APPEAL);
    EXPECT_EQ(eContestGimmickStatus[contestant].selected, CONTEST_GIMMICK_NONE);
    EXPECT(!Test_ContestGimmickCanSelect(contestant, CONTEST_GIMMICK_Z_APPEAL, MOVE_LAST_RESORT));
    FreeContest();
}
