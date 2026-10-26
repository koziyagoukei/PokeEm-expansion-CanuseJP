#ifndef GUARD_CONSTANTS_FRONTIER_STORY_H
#define GUARD_CONSTANTS_FRONTIER_STORY_H

// Kazuki's story at the Battle Tower (VAR_TOWER_STORY_STATE).
#define TOWER_STORY_NOT_MET     0
#define TOWER_STORY_MET         1
#define TOWER_STORY_WISH        2 // His own wish surfaced after the player's Silver Symbol.
#define TOWER_STORY_RETURNED    3 // Won the release battle; his mind is back but he is not cured yet.
#define TOWER_STORY_RESCUED     4

// The person the network holds at each scenario, in FLAG_STORY_RESCUED_* order.
#define STORY_TARGET_KAZUKI     0 // Battle Tower
#define STORY_TARGET_REIKA      1 // Battle Dome
#define STORY_TARGET_IKUE       2 // Battle Palace
#define STORY_TARGET_EISUKE     3 // Battle Arena
#define STORY_TARGET_TOSHIO     4 // Battle Factory
#define STORY_TARGET_YAYOI      5 // Battle Pike
#define STORY_TARGET_NOBUO      6 // Battle Pyramid
#define STORY_TARGET_YORIKO     7 // Contest
#define STORY_TARGET_COUNT      8

// Each person's step of their story. Kazuki's is VAR_TOWER_STORY_STATE (TOWER_STORY_*, the same
// values); the others share VAR_STORY_TARGET_STATES and VAR_STORY_TARGET_STATES_2, four bits each.
#define TARGET_STORY_NOT_MET    0
#define TARGET_STORY_MET        1
#define TARGET_STORY_WISH       2 // After the Silver Symbol; at the contest, the first Master Rank win.
#define TARGET_STORY_RETURNED   3
#define TARGET_STORY_RESCUED    4

// VAR_0x8005 for FrontierStory_BufferTargetBranch: which part of their story the person is in.
#define TARGET_TALK_HAPPY       0
#define TARGET_TALK_WISH        1

// The contest's scenes about the world, one after each category won at Master Rank past the
// first (VAR_CONTEST_STORY_SCENES counts those seen). VAR_0x8004 for
// FrontierStory_BufferContestSceneBranch is whose words: CONTEST_SCENE_YORIKO or _DARKRAI.
#define CONTEST_STORY_SCENE_COUNT 4
#define CONTEST_SCENE_YORIKO      0
#define CONTEST_SCENE_DARKRAI     1
// Lines that change with the rescues so far: none, one, two, three, then four or more.
#define STORY_RESCUE_STAGE_COUNT  5

// Memory of a Frontier Brain or seventh-battle guest, stored in two flags per person.
#define NAMED_STORY_INFECTED    0
#define NAMED_STORY_UNEASY      1 // Lost to the player once and questioned their own words.
#define NAMED_STORY_RECOVERED   2 // Remembers who they are and can be cured at the camp.
#define NAMED_STORY_RESCUED     3

// Guests come first in TRAINER_FRONTIER_* order; Tate and Liza share one entry.
#define NAMED_STORY_GUEST_COUNT 26
#define NAMED_STORY_BRAIN_FIRST NAMED_STORY_GUEST_COUNT
#define NAMED_STORY_COUNT       (NAMED_STORY_GUEST_COUNT + 7)
#define NAMED_STORY_NONE        0xFF

// The person visiting the camp on this visit, if any.
#define VAR_CAMP_VISITOR        VAR_TEMP_9
#define VAR_CAMP_VISITOR_GFX    VAR_OBJ_GFX_ID_D

// VAR_0x8004 for FrontierStory_BufferVisitorText.
#define CAMP_VISITOR_TEXT_GREETING 0
#define CAMP_VISITOR_TEXT_CURED    1

#endif // GUARD_CONSTANTS_FRONTIER_STORY_H
