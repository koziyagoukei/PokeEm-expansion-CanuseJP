#ifndef GUARD_FRONTIER_STORY_H
#define GUARD_FRONTIER_STORY_H

#include "constants/frontier_story.h"

// Story battles (scripts call these as specials).
void FrontierStory_PrepareReleaseBattle(void);
void FrontierStory_PrepareMatchedBattle(void);
void FrontierStory_PrepareFinalBattle(void);
void FrontierStory_EndStoryBattle(void);

// Battle engine hooks.
u8 FrontierStory_GetTrainerMonLevel(u8 level);
bool32 FrontierStory_IsReleaseBattle(void);
void FrontierStory_RecordSendOut(u32 battler, bool32 replacingFaintedMon);
bool32 FrontierStory_TryReleaseBoost(u32 battler);
void FrontierStory_OnBattleFinished(void);

// Frontier Brain and seventh-battle guest memories.
bool32 FrontierStory_TryCopyBrainBattleText(u32 facility, bool32 playerWon);
bool32 FrontierStory_TryCopyGuestText(u8 whichText, u16 trainerId);
void FrontierStory_BufferBrainIntro(void);

// The people the network holds at each scenario (STORY_TARGET_*).
void FrontierStory_IsTargetHere(void);
void FrontierStory_CheckTargetApproach(void);
void FrontierStory_TargetComesOver(void);
void FrontierStory_BufferTargetTalk(void);
void FrontierStory_BufferTargetBranch(void);
void FrontierStory_CheckTargetFever(void);
void FrontierStory_FindReturnedTarget(void);
void FrontierStory_StartRescueScene(void);
void FrontierStory_GetRescueScene(void);
void FrontierStory_BufferRescueText(void);
void FrontierStory_ReturnRescueTarget(void);
void FrontierStory_PrepareHubRescue(void);
void FrontierStory_GetRescueMatchCall(void);
void FrontierStory_FinishRescue(void);
void FrontierStory_CheckContestScene(void);
void FrontierStory_BufferContestSceneBranch(void);

// Shared progress and the camp visitors.
void FrontierStory_CountRescued(void);
void FrontierStory_ShouldGather(void);
void FrontierStory_ShouldSuggestInn(void);
void FrontierStory_CanDarkraiJoin(void);
void FrontierStory_PickCampVisitor(void);
void FrontierStory_BufferVisitorText(void);
void FrontierStory_RescueVisitor(void);
void FrontierStory_PickInnGuests(void);
void FrontierStory_BufferInnGuestText(void);

// Passers-by, facility attendants and the Frontier's regulars.
u32 FrontierStory_GetCrowdStage(void);
u32 FrontierStory_GetCrowdVoice(u16 graphicsId);
bool32 FrontierStory_BufferCrowdLine(u32 stage, u32 voice);
void FrontierStory_BufferCrowdTalk(void);
void FrontierStory_BufferStaffAside(void);
u32 FrontierStory_GetRegularPhase(void);
bool32 FrontierStory_BufferRegularLine(u32 regular, u32 phase);
void FrontierStory_BufferRegularTalk(void);

// Cured Brains and guests meeting on the squares.
bool32 FrontierStory_IsNamedTalkShown(u32 talk);
void FrontierStory_SetupNamedTalks(void);
void FrontierStory_FinishNamedTalk(void);

// The end of the story (frontier_ending.c).
void FrontierStory_ShowEnding(void);

// Placing people for story scenes.
void FrontierStory_ArrangeReleaseScene(void);
void FrontierStory_PlaceBesidePlayer(void);
void FrontierStory_ApproachPlayer(void);
void FrontierStory_ReturnFromApproach(void);
void TowerStory_CheckKazukiApproach(void);

#endif // GUARD_FRONTIER_STORY_H
