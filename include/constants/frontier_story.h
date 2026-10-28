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

// Their Gold Symbol step (at the contest, after its last scene), from FrontierStory_CheckTargetFever:
// the fever begins but Darkrai, not yet with the player, stops them; or the release battle.
#define TARGET_FEVER_NONE       0
#define TARGET_FEVER_ALONE      1
#define TARGET_FEVER_RELEASE    2

// VAR_0x8004 for FrontierStory_BufferRescueText: a part of their release battle and cure.
#define RESCUE_TEXT_FEVER_STARTING 0 // Darkrai has not joined yet.
#define RESCUE_TEXT_FEVER          1
#define RESCUE_TEXT_VOICES_FADE    2 // Won: the network fades...
#define RESCUE_TEXT_CHOOSES        3 // ...and they choose to leave it.
#define RESCUE_TEXT_DELIRIOUS      4 // In the hub, their mind back but not cured.
#define RESCUE_TEXT_EATS           5
#define RESCUE_TEXT_WAKES_UP       6
#define RESCUE_TEXT_REMEMBERS      7
#define RESCUE_TEXT_AFTER_CURE     8 // Optional.
#define RESCUE_TEXT_ASKS_POKENAV   9
#define RESCUE_TEXT_GOES_TO_INN    10
#define RESCUE_TEXT_COUNT          11

// How the person cured in the hub looks (FrontierStory_PrepareHubRescue).
#define VAR_HUB_RESCUED_GFX     VAR_OBJ_GFX_ID_A

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

// The parts of the inn (VAR_0x8004 for FrontierStory_PickInnGuests). Each cured Brain or guest
// rests in one of them; a visit there shows up to INN_GUEST_SLOTS of them, picked at random.
#define INN_AREA_LOBBY          0
#define INN_AREA_EAST_WING      1
#define INN_AREA_WEST_WING      2
#define INN_AREA_COUNT          3
#define INN_GUEST_SLOTS         2
// Per slot, consecutive: whether it is hidden, who is in it (NAMED_STORY_*), and how they look.
#define FLAG_INN_GUEST_HIDDEN   FLAG_TEMP_13
#define VAR_INN_GUEST           VAR_TEMP_8
#define VAR_INN_GUEST_GFX       VAR_OBJ_GFX_ID_B

// Passers-by at the Frontier talk about how far the rescues have come, in a voice for how they
// look (FrontierStory_BufferCrowdTalk).
#define CROWD_STAGE_NONE_RESCUED 0
#define CROWD_STAGE_FIRST_RESCUE 1
#define CROWD_STAGE_FEW_RESCUED  2 // two or three
#define CROWD_STAGE_PATH_OPEN    3 // four or more: the way to the final area is open
#define CROWD_STAGE_AFTER_ENDING 4
#define CROWD_STAGE_COUNT        5

#define CROWD_VOICE_CHILD 0
#define CROWD_VOICE_MAN   1
#define CROWD_VOICE_WOMAN 2
#define CROWD_VOICE_ELDER 3
#define CROWD_VOICE_COUNT 4

// Facility attendants add a word for how far the rescues have come after their welcome, once a
// visit.
#define FLAG_STAFF_ASIDE_SHOWN FLAG_TEMP_1C

// The Frontier's regulars: people with a place or a companion of their own, whose own lines
// change before the rescues, while they go on, and after the ending (VAR_0x8004 for
// FrontierStory_BufferRegularTalk).
#define REGULAR_PHASE_BEFORE_RESCUES 0
#define REGULAR_PHASE_RESCUING       1
#define REGULAR_PHASE_AFTER_ENDING   2
#define REGULAR_PHASE_COUNT          3

#define REGULAR_WEST_CHEER_WOMAN      0 // three cheering together
#define REGULAR_WEST_CHEER_BOY        1
#define REGULAR_WEST_CHEER_OLD_MAN    2
#define REGULAR_EAST_TWIN             3 // and her dad
#define REGULAR_EAST_TWIN_DAD         4
#define REGULAR_EAST_FAN              5 // still full of a battle he saw
#define REGULAR_EAST_FAN_PARTNER      6
#define REGULAR_EAST_MULTI_MANIAC     7 // a multi battle pair
#define REGULAR_EAST_MULTI_WOMAN      8
#define REGULAR_EAST_COOK             9 // with his Zigzagoon
#define REGULAR_EXCHANGE_RICH_BOY     10 // and his mom
#define REGULAR_EXCHANGE_MOM          11
#define REGULAR_LOUNGE1_BOY_1         12 // praised by the breeder
#define REGULAR_LOUNGE1_BOY_2         13
#define REGULAR_LOUNGE2_MANIAC_1      14 // around the Frontier maniac
#define REGULAR_LOUNGE2_MANIAC_2      15
#define REGULAR_LOUNGE2_TRIATHLETE    16
#define REGULAR_LOUNGE2_MANIAC_3      17
#define REGULAR_LOUNGE3_MAN           18 // where people bet on battles
#define REGULAR_LOUNGE4_WOMAN         19
#define REGULAR_LOUNGE4_COOK          20
#define REGULAR_LOUNGE4_MAN           21
#define REGULAR_LOUNGE5_GENTLEMAN     22 // around the girl who tells natures
#define REGULAR_LOUNGE5_BLACK_BELT    23
#define REGULAR_LOUNGE5_LITTLE_BOY    24
#define REGULAR_LOUNGE7_SAILOR        25 // around the two move tutors
#define REGULAR_LOUNGE7_GENTLEMAN     26
#define REGULAR_LOUNGE8_NINJA_BOY     27
#define REGULAR_LOUNGE8_MAN           28
#define REGULAR_LOUNGE8_WOMAN         29 // remembers the Brains
#define REGULAR_RANKING_HALL_GUIDE    30
#define REGULAR_COUNT                 31

#endif // GUARD_CONSTANTS_FRONTIER_STORY_H
