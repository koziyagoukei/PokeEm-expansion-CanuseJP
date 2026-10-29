#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_gfx_sfx_util.h"
#include "contest.h"
#include "main.h"
#include "m4a.h"
#include "palette.h"
#include "pokemon.h"
#include "sprite.h"
#include "task.h"
#include "window.h"
#include "test/contest_animation.h"
#include "test/test.h"
#include "test/battle.h"

extern u32 gOamMatrixAllocBitmap;
void AnimTask_GetLycanrocForm(u8 taskId);

static void RunAnimation(void)
{
    u32 frame;
    for (frame = 0; frame < 3600 && BattleAnim_IsContestCutawayBusy(); frame++)
    {
        gAnimScriptCallback();
        RunTasks();
        AnimateSprites();
        BuildOamBuffer();
        UpdatePaletteFade();
        m4aSoundMain();
        VBlankIntrWait();
    }
    EXPECT_LT(frame, 3600);
    EXPECT_EQ(gAnimVisualTaskCount, 0);
    EXPECT_EQ(gAnimSoundTaskCount, 0);
    EXPECT_EQ(gLoadFail, FALSE);
}

static void InitContestAnimation(void)
{
    gMain.inBattle = FALSE;
    gMain.state = 0;
    gLinkContestFlags = 0;
    gContestPlayerMonIndex = 3;
    gContestMonPartyIndex = 0;
    gSpecialVar_ContestCategory = CONTEST_CATEGORY_COOL;
    for (u32 i = 0; i < CONTESTANT_COUNT; i++)
        gContestMons[i] = (struct ContestPokemon){.species = SPECIES_PIKACHU};
    CB2_StartContest();
    ClearBattleAnimationVars();
    gBattleTestRunnerState->data.trial.queuedEvent = 0;
    gBattleTestRunnerState->data.queuedEventsCount = 0;
    gBattleTestRunnerState->forceMoveAnim = TRUE;
}

static void FreeContestAnimation(void)
{
    SetVBlankCallback(NULL);
    Test_ContestFreeResources();
    FreeMonSpritesGfx();
    FreeAllWindowBuffers();
    ResetTasks();
    ResetSpriteData();
}

TEST("Contest animation: repeated cutaways release their affine matrices")
{
    InitContestAnimation();
    u32 matrices = gOamMatrixAllocBitmap;
    for (u32 i = 0; i < 40; i++)
    {
        EXPECT(Test_ContestBeginCutaway(0, MOVE_TACKLE));
        Test_ContestEndCutaway(0);
        EXPECT_EQ(gOamMatrixAllocBitmap, matrices);
    }
    FreeContestAnimation();
}

TEST("Contest animation: target rotation cannot alter the attacker's matrix")
{
    InitContestAnimation();
    EXPECT(Test_ContestBeginCutaway(0, MOVE_TACKLE));
    struct Sprite *attacker = &gSprites[gBattlerSpriteIds[gBattleAnimAttacker]];
    struct Sprite *target = &gSprites[gBattlerSpriteIds[gBattleAnimTarget]];
    EXPECT_NE((u32)target->oam.affineMode, ST_OAM_AFFINE_OFF);
    EXPECT_NE((u32)target->oam.matrixNum, attacker->oam.matrixNum);
    EXPECT_NE((u32)target->oam.tileNum, attacker->oam.tileNum);
    const volatile u32 *tiles = (void *)(OBJ_VRAM0 + target->oam.tileNum * TILE_SIZE_4BPP);
    for (u32 i = 0; i < MON_PIC_SIZE / sizeof(*tiles); i++)
        EXPECT_EQ(tiles[i], 0);
    struct OamMatrix matrix = gOamMatrices[attacker->oam.matrixNum];
    PrepareBattlerSpriteForRotScale(gBattlerSpriteIds[gBattleAnimTarget], ST_OAM_OBJ_NORMAL);
    SetSpriteRotScale(gBattlerSpriteIds[gBattleAnimTarget], 0x180, 0x80, 0x4000);
    EXPECT_EQ(gOamMatrices[attacker->oam.matrixNum].a, matrix.a);
    EXPECT_EQ(gOamMatrices[attacker->oam.matrixNum].d, matrix.d);
    Test_ContestEndCutaway(0);
    FreeContestAnimation();
}

TEST("Contest animation: converted moves use their own target instead of the base move")
{
    bool32 dynamax = FALSE;
    PARAMETRIZE_LABEL("%s", "Z") { dynamax = FALSE; }
    PARAMETRIZE_LABEL("%s", "Max") { dynamax = TRUE; }
    InitContestAnimation();
    eContestGimmickStatus[0].moveGimmick = dynamax ? CONTEST_GIMMICK_DYNAMAX : CONTEST_GIMMICK_Z_APPEAL;
    eContestGimmickStatus[0].heldItem = ITEM_NORMALIUM_Z;
    EXPECT(Test_ContestBeginCutaway(0, dynamax ? MOVE_GROWL : MOVE_SWORDS_DANCE));
    Test_ContestSetupCutaway(0, dynamax ? MOVE_GROWL : MOVE_SWORDS_DANCE, FALSE);
    if (dynamax)
        EXPECT_EQ(gBattleAnimTarget, gBattleAnimAttacker); // Max Guard
    else
        EXPECT_NE(gBattleAnimTarget, gBattleAnimAttacker); // Breakneck Blitz
    Test_ContestEndCutaway(0);
    FreeContestAnimation();
}

TEST("Contest animation: all Z and Max move scripts finish without leaking resources")
{
    enum Move move = FIRST_Z_MOVE;
    for (enum Move candidate = FIRST_Z_MOVE; candidate <= LAST_MAX_MOVE; candidate++)
        PARAMETRIZE_LABEL("move %d", candidate) { move = candidate; }

    InitContestAnimation();
    EXPECT(Test_ContestBeginCutaway(0, MOVE_TACKLE));
    Test_ContestSetupCutaway(0, MOVE_TACKLE, FALSE);
    u32 matrices = gOamMatrixAllocBitmap;
    DoMoveAnim(move);
    RunAnimation();
    EXPECT_EQ(gOamMatrixAllocBitmap, matrices);
    Test_ContestEndCutaway(0);
    EXPECT_EQ(gOamMatrixAllocBitmap, 0);
    FreeContestAnimation();
}

TEST("Contest animation: Z and Dynamax intros can be followed by repeated converted attacks")
{
    bool32 dynamax = FALSE;
    PARAMETRIZE_LABEL("%s", "Z") { dynamax = FALSE; }
    PARAMETRIZE_LABEL("%s", "Max") { dynamax = TRUE; }
    InitContestAnimation();
    gTestRunnerState.timeoutSeconds = 180;
    eContestGimmickStatus[0].moveGimmick = dynamax ? CONTEST_GIMMICK_DYNAMAX : CONTEST_GIMMICK_Z_APPEAL;
    eContestGimmickStatus[0].heldItem = ITEM_ELECTRIUM_Z;
    for (u32 i = 0; i < 5; i++)
    {
        EXPECT(Test_ContestBeginCutaway(0, MOVE_THUNDERBOLT));
        Test_ContestSetupCutaway(0, MOVE_THUNDERBOLT, TRUE);
        LaunchBattleAnimation(ANIM_TYPE_GENERAL, dynamax ? B_ANIM_DYNAMAX_GROWTH : B_ANIM_ZMOVE_ACTIVATE);
        RunAnimation();
        Test_ContestSetupCutaway(0, MOVE_THUNDERBOLT, FALSE);
        EXPECT_NE(gBattleAnimAttacker, gBattleAnimTarget);
        DoMoveAnim(dynamax ? MOVE_MAX_LIGHTNING : MOVE_GIGAVOLT_HAVOC);
        RunAnimation();
        Test_ContestEndCutaway(0);
        EXPECT_EQ(gOamMatrixAllocBitmap, 0);
    }
    FreeContestAnimation();
}

TEST("Contest animation: Splintered Stormshards reads the contestant's Lycanroc form")
{
    InitContestAnimation();
    gContestMons[0].species = SPECIES_LYCANROC_DUSK;
    EXPECT(Test_ContestBeginCutaway(0, MOVE_STONE_EDGE));
    gBattleMons[gBattleAnimAttacker].species = SPECIES_LYCANROC_MIDNIGHT;
    u32 taskId = CreateTask(AnimTask_GetLycanrocForm, 5);
    gAnimVisualTaskCount = 1;
    AnimTask_GetLycanrocForm(taskId);
    EXPECT_EQ(gBattleAnimArgs[0], 2);
    Test_ContestEndCutaway(0);
    FreeContestAnimation();
}
