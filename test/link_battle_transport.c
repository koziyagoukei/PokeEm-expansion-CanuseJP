#include "global.h"
#include "battle.h"
#include "battle_controllers.h"
#include "battle_gimmick.h"
#include "battle_main.h"
#include "battle_tower.h"
#include "contest.h"
#include "event_data.h"
#include "link.h"
#include "link_rfu.h"
#include "malloc.h"
#include "sprite.h"
#include "task.h"
#include "test/link.h"
#include "test/test.h"
#include "constants/battle_frontier.h"
#include "constants/battle_tower.h"

// Task-private queue indices; drive the actual tasks rather than a transport mock.
enum { SEND_STATE = 11, WRAP_FROM, SEND_DELAY, QUEUE_END, QUEUE_START };
enum { SEND_COUNT_PLAYERS = 2, SEND_BEGIN, SEND_FINISH };

static void InitTransport(u32 flags, u32 count, u8 *send, u8 *receive)
{
    ResetTasks();
    ResetSpriteData();
    memset(&gRfu, 0, sizeof(gRfu));
    memset(gSendCmd, 0, sizeof(gSendCmd));
    gWirelessCommType = FALSE;
    gLinkCallback = NULL;
    gLinkStatus = count << LINK_STAT_PLAYER_COUNT_SHIFT;
    gReceivedRemoteLinkPlayers = TRUE;
    ResetBlockReceivedFlags();
    gLink.badChecksum = FALSE;
    gLink.queueFull = QUEUE_FULL_NONE;
    gBattleTypeFlags = BATTLE_TYPE_LINK | BATTLE_TYPE_LINK_IN_BATTLE | BATTLE_TYPE_IS_MASTER | flags;
    gBattlersCount = (flags & BATTLE_TYPE_DOUBLE) ? 4 : 2;
    gBattleControllerExecFlags = 0;
    gLinkBattleSendBuffer = AllocZeroed(BATTLE_BUFFER_LINK_SIZE);
    gLinkBattleRecvBuffer = AllocZeroed(BATTLE_BUFFER_LINK_SIZE);
    Test_InitLinkBattleBufferTasks(send, receive);
}

static void FreeTransport(void)
{
    ResetTasks();
    FREE_AND_SET_NULL(gLinkBattleSendBuffer);
    FREE_AND_SET_NULL(gLinkBattleRecvBuffer);
    gReceivedRemoteLinkPlayers = FALSE;
    gLinkStatus = 0;
}

static void FinishCableBlock(void)
{
    for (u32 i = 0; i < 30 && gLinkCallback != NULL; i++)
        gLinkCallback();
    EXPECT(gLinkCallback == NULL);
}

TEST("Link audit: battle transport uses the correct participant count for each mode")
{
    u32 flags = 0, count = 2;
    PARAMETRIZE { flags = 0; count = 2; }
    PARAMETRIZE { flags = BATTLE_TYPE_DOUBLE; count = 2; }
    PARAMETRIZE { flags = BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI; count = 4; }
    PARAMETRIZE { flags = BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI | BATTLE_TYPE_BATTLE_TOWER; count = 2; }
    u8 send, receive;
    InitTransport(flags, count - 1, &send, &receive);
    gTasks[send].data[SEND_STATE] = SEND_COUNT_PLAYERS;
    gTasks[send].func(send);
    EXPECT_EQ(gTasks[send].data[SEND_STATE], SEND_COUNT_PLAYERS);
    gLinkStatus = count << LINK_STAT_PLAYER_COUNT_SHIFT;
    gTasks[send].func(send);
    EXPECT_EQ(gTasks[send].data[SEND_STATE], SEND_BEGIN);
    FreeTransport();
}

TEST("Link audit: a busy wireless transport retries without consuming the queued command")
{
    u8 send, receive;
    u8 payload[] = {CONTROLLER_PRINTSTRING, 0, 0, 0};
    InitTransport(0, 2, &send, &receive);
    PrepareBufferDataTransferLink(B_BATTLER_1, B_COMM_TO_CONTROLLER, sizeof(payload), payload);
    gTasks[send].data[SEND_STATE] = SEND_BEGIN;
    gWirelessCommType = TRUE;
    gRfu.playerCount = 2;
    gSendCmd[0] = 1; // RFU rejects SendBlock even though its callback is idle.
    EXPECT(IsLinkTaskFinished());
    for (u32 i = 0; i < 4; i++)
    {
        gTasks[send].func(send);
        EXPECT_EQ(gTasks[send].data[SEND_STATE], SEND_BEGIN);
        EXPECT_EQ(gTasks[send].data[QUEUE_START], 0);
        EXPECT_EQ(gTasks[send].data[QUEUE_END], 12);
    }
    gSendCmd[0] = 0;
    gTasks[send].func(send);
    EXPECT_EQ(gTasks[send].data[SEND_STATE], SEND_FINISH);
    EXPECT_EQ(memcmp(gBlockSendBuffer + 8, payload, sizeof(payload)), 0);
    EXPECT_EQ(gTasks[send].data[QUEUE_START], 0);
    memset(&gRfu, 0, sizeof(gRfu));
    memset(gSendCmd, 0, sizeof(gSendCmd));
    gWirelessCommType = FALSE;
    FreeTransport();
}

TEST("Link audit: a busy cable block is not mistaken for completion of a queued command")
{
    u8 send, receive;
    u8 payload[] = {CONTROLLER_PRINTSTRING, 0, 0, 0};
    u32 unrelated = 0x12345678;
    InitTransport(0, 2, &send, &receive);
    PrepareBufferDataTransferLink(B_BATTLER_1, B_COMM_TO_CONTROLLER, sizeof(payload), payload);
    gTasks[send].data[SEND_STATE] = SEND_BEGIN;
    EXPECT(SendBlock(0, &unrelated, sizeof(unrelated)));
    gTasks[send].func(send);
    EXPECT_EQ(gTasks[send].data[SEND_STATE], SEND_BEGIN);
    FinishCableBlock();
    gTasks[send].func(send);
    EXPECT_EQ(gTasks[send].data[SEND_STATE], SEND_FINISH);
    EXPECT_EQ(memcmp(gBlockSendBuffer + 8, payload, sizeof(payload)), 0);
    FinishCableBlock();
    gTasks[send].func(send);
    EXPECT_EQ(gTasks[send].data[QUEUE_START], gTasks[send].data[QUEUE_END]);
    FreeTransport();
}

TEST("Link audit: move reply and controller completion arrive together for every battler and player")
{
    u32 count = 2;
    PARAMETRIZE { count = 2; }
    PARAMETRIZE { count = 4; }
    u8 send, receive;
    InitTransport(BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI, count, &send, &receive);
    gBattleResources = AllocZeroed(sizeof(*gBattleResources));
    for (u32 player = 0; player < count; player++)
    {
        for (enum BattlerId battler = 0; battler < MAX_BATTLERS_COUNT; battler++)
        {
            gTasks[send].data[QUEUE_START] = gTasks[send].data[QUEUE_END] = 0;
            u32 response = RET_GIMMICK | 2 | (B_BATTLER_3 << RET_TARGET_SHIFT) | (GIMMICK_MEGA << RET_GIMMICK_ID_SHIFT);
            EXPECT(TryQueueLinkBattleControllerReply(battler, B_ACTION_EXEC_SCRIPT, response, player));
            EXPECT_EQ(gTasks[send].data[QUEUE_END], 28);
            memcpy(gBlockRecvBuffer[player], gLinkBattleSendBuffer, 16);
            gBlockReceivedStatus[player] = TRUE;
            TryReceiveLinkBattleData();
            gTasks[receive].func(receive);
            EXPECT_EQ(gBattleResources->bufferB[battler][0], CONTROLLER_TWORETURNVALUES);
            u32 received;
            memcpy(&received, &gBattleResources->bufferB[battler][2], sizeof(received));
            EXPECT_EQ(received, response);
            MarkBattleControllerActiveForPlayer(battler, player);
            memcpy(gBlockRecvBuffer[player], gLinkBattleSendBuffer + 16, 12);
            gBlockReceivedStatus[player] = TRUE;
            TryReceiveLinkBattleData();
            gTasks[receive].func(receive);
            EXPECT(!IsBattleControllerActiveForPlayer(battler, player));
        }
    }
    FREE_AND_SET_NULL(gBattleResources);
    FreeTransport();
}

TEST("Link audit: malformed controller packets are rejected before indexing battler buffers")
{
    u32 invalid = 0;
    bool32 wireless = FALSE;
    for (u32 w = 0; w < 2; w++)
        for (u32 i = 0; i < 6; i++)
            PARAMETRIZE { invalid = i; wireless = w; }
    u8 send, receive;
    InitTransport(BATTLE_TYPE_DOUBLE, 2, &send, &receive);
    u8 *packet = (u8 *)gBlockRecvBuffer[1];
    memset(packet, 0, 16);
    packet[0] = B_COMM_TO_ENGINE;
    packet[1] = B_BATTLER_1;
    packet[4] = 4;
    switch (invalid)
    {
    case 0: packet[1] = MAX_BATTLERS_COUNT; break;
    case 1: packet[0] = 0xFF; break;
    case 2: packet[4] = 0; break;
    case 3: packet[4] = 3; break;
    case 4: packet[4] = 252; break;
    case 5: packet[0] = B_COMM_CONTROLLER_IS_DONE; packet[8] = MAX_LINK_PLAYERS; break;
    }
    if (wireless)
    {
        gWirelessCommType = TRUE;
        gRfu.playerCount = 2;
        gRfu.recvBlock[1].receiving = 2; // RFU's private RECV_STATE_FINISHED.
        gRfu.blockReceived[1] = TRUE;
    }
    else
    {
        gBlockReceivedStatus[1] = TRUE;
    }
    TryReceiveLinkBattleData();
    EXPECT(gLink.badChecksum);
    if (wireless)
    {
        EXPECT_EQ(gRfu.status, RFU_STATUS_FATAL_ERROR);
        EXPECT_EQ(gRfu.errorState, RFU_ERROR_STATE_OCCURRED);
    }
    EXPECT_EQ(GetBlockReceivedStatus(), 0);
    EXPECT_EQ(gTasks[receive].data[QUEUE_END], 0);
    memset(&gRfu, 0, sizeof(gRfu));
    gWirelessCommType = FALSE;
    FreeTransport();
}

TEST("Link audit: a full send queue retries the move reply without emitting a partial completion")
{
    u8 send, receive;
    InitTransport(BATTLE_TYPE_DOUBLE, 2, &send, &receive);
    gTasks[send].data[QUEUE_START] = 100;
    gTasks[send].data[QUEUE_END] = 80;
    EXPECT(!TryQueueLinkBattleControllerReply(B_BATTLER_2, B_ACTION_EXEC_SCRIPT, 0x12345678, 1));
    EXPECT_EQ(gTasks[send].data[QUEUE_END], 80);
    EXPECT_EQ(gLink.queueFull, QUEUE_FULL_NONE);
    gTasks[send].data[QUEUE_START] = 200;
    EXPECT(TryQueueLinkBattleControllerReply(B_BATTLER_2, B_ACTION_EXEC_SCRIPT, 0x12345678, 1));
    EXPECT_EQ(gTasks[send].data[QUEUE_END], 108);
    EXPECT_EQ(gLinkBattleSendBuffer[80], B_COMM_TO_ENGINE);
    EXPECT_EQ(gLinkBattleSendBuffer[96], B_COMM_CONTROLLER_IS_DONE);
    EXPECT_EQ(gLinkBattleSendBuffer[104], 1);
    FreeTransport();
}

TEST("Link audit: a full receive queue retains the received block for retry")
{
    u8 send, receive;
    u8 payload[] = {CONTROLLER_PRINTSTRING, 0, 0, 0};
    InitTransport(0, 2, &send, &receive);
    PrepareBufferDataTransferLink(B_BATTLER_1, B_COMM_TO_CONTROLLER, sizeof(payload), payload);
    memcpy(gBlockRecvBuffer[1], gLinkBattleSendBuffer, 12);
    gBlockReceivedStatus[1] = TRUE;
    gTasks[receive].data[QUEUE_START] = 100;
    gTasks[receive].data[QUEUE_END] = 96;
    TryReceiveLinkBattleData();
    EXPECT_EQ(GetBlockReceivedStatus(), 2);
    EXPECT_EQ(gTasks[receive].data[QUEUE_END], 96);
    EXPECT_EQ(gLink.queueFull, QUEUE_FULL_NONE);
    gTasks[receive].data[QUEUE_START] = 200;
    TryReceiveLinkBattleData();
    EXPECT_EQ(GetBlockReceivedStatus(), 0);
    EXPECT_EQ(gTasks[receive].data[QUEUE_END], 108);
    EXPECT_EQ(memcmp(gLinkBattleRecvBuffer + 104, payload, sizeof(payload)), 0);
    FreeTransport();
}

TEST("Link audit: Tower link multi setup retries rejected challenge and trainer blocks")
{
    u32 state = 0;
    PARAMETRIZE { state = 0; }
    PARAMETRIZE { state = 2; }
    memset(&gRfu, 0, sizeof(gRfu));
    gWirelessCommType = TRUE;
    gSendCmd[0] = 1;
    VarSet(VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_LINK_MULTIS);
    gSaveBlock2Ptr->frontier.lvlMode = FRONTIER_LVL_50;
    gSpecialVar_0x8004 = BATTLE_TOWER_FUNC_LOAD_LINK_OPPONENTS;
    gSpecialVar_Result = state;
    CallBattleTowerFunc();
    EXPECT_EQ(gSpecialVar_Result, state);
    gSendCmd[0] = 0;
    CallBattleTowerFunc();
    EXPECT_EQ(gSpecialVar_Result, state + 1);
    memset(&gRfu, 0, sizeof(gRfu));
    memset(gSendCmd, 0, sizeof(gSendCmd));
    gWirelessCommType = FALSE;
}

TEST("Link audit: all battle startup party and RNG exchanges wait for successful sends")
{
    u32 mode = 0;
    for (u32 i = 0; i < 4; i++)
        PARAMETRIZE { mode = i; }
    const u8 normalStates[] = {3, 7, 11, 16};
    const u8 multiStates[] = {3, 5, 8};
    const u8 towerStates[] = {3, 5, 7, 9, 11, 13, 15, 17, 20};
    const u8 *states = mode == 3 ? towerStates : mode == 2 ? multiStates : normalStates;
    u32 stateCount = mode == 3 ? ARRAY_COUNT(towerStates) : mode == 2 ? ARRAY_COUNT(multiStates) : ARRAY_COUNT(normalStates);
    u32 flags = mode == 0 ? 0 : BATTLE_TYPE_DOUBLE;
    if (mode >= 2)
        flags |= BATTLE_TYPE_MULTI;
    if (mode == 3)
        flags |= BATTLE_TYPE_BATTLE_TOWER;
    u8 send, receive;
    InitTransport(flags, mode == 2 ? 4 : 2, &send, &receive);
    ResetTasks();
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    gWirelessCommType = TRUE;
    for (u32 i = 0; i < stateCount; i++)
    {
        memset(&gRfu, 0, sizeof(gRfu));
        gSendCmd[0] = 1;
        gBattleCommunication[MULTIUSE_STATE] = states[i];
        Test_RunLinkBattleStartState(FALSE);
        EXPECT_EQ(gBattleCommunication[MULTIUSE_STATE], states[i]);
        gSendCmd[0] = 0;
        Test_RunLinkBattleStartState(FALSE);
        EXPECT_EQ(gBattleCommunication[MULTIUSE_STATE], states[i] + 1);
    }
    memset(&gRfu, 0, sizeof(gRfu));
    memset(gSendCmd, 0, sizeof(gSendCmd));
    gWirelessCommType = FALSE;
    FREE_AND_SET_NULL(gBattleStruct);
    FreeTransport();
}

TEST("Link audit: multi party preview retains its buffer while retrying a rejected send")
{
    u8 send, receive;
    InitTransport(BATTLE_TYPE_DOUBLE | BATTLE_TYPE_MULTI, 4, &send, &receive);
    ResetTasks();
    gBattleStruct = AllocZeroed(sizeof(*gBattleStruct));
    gMultiPartnerParty = NULL;
    gBattleCommunication[MULTIUSE_STATE] = 0;
    gWirelessCommType = TRUE;
    gSendCmd[0] = 1;
    Test_RunLinkBattleStartState(TRUE);
    EXPECT_EQ(gBattleCommunication[MULTIUSE_STATE], 0);
    EXPECT(gMultiPartnerParty != NULL);
    void *buffer = gMultiPartnerParty;
    Test_RunLinkBattleStartState(TRUE);
    EXPECT(gMultiPartnerParty == buffer);
    EXPECT_EQ(gBattleCommunication[MULTIUSE_STATE], 0);
    gSendCmd[0] = 0;
    Test_RunLinkBattleStartState(TRUE);
    EXPECT_EQ(gBattleCommunication[MULTIUSE_STATE], 1);
    memset(&gRfu, 0, sizeof(gRfu));
    memset(gSendCmd, 0, sizeof(gSendCmd));
    gWirelessCommType = FALSE;
    FREE_AND_SET_NULL(gMultiPartnerParty);
    FREE_AND_SET_NULL(gBattleStruct);
    FreeTransport();
}
