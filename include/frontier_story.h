#ifndef GUARD_FRONTIER_STORY_H
#define GUARD_FRONTIER_STORY_H

#include "constants/frontier_story.h"

// Story battles (scripts call these as specials).
void FrontierStory_PrepareReleaseBattle(void);
void FrontierStory_PrepareMatchedBattle(void);
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

// Shared progress and the camp visitors.
void FrontierStory_CountRescued(void);
void FrontierStory_CanDarkraiJoin(void);
void FrontierStory_PickCampVisitor(void);
void FrontierStory_BufferVisitorText(void);
void FrontierStory_RescueVisitor(void);

// Placing people for story scenes.
void FrontierStory_ArrangeReleaseScene(void);
void FrontierStory_PlaceBesidePlayer(void);

#endif // GUARD_FRONTIER_STORY_H
