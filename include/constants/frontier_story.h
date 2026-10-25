#ifndef GUARD_CONSTANTS_FRONTIER_STORY_H
#define GUARD_CONSTANTS_FRONTIER_STORY_H

// Kazuki's story at the Battle Tower (VAR_TOWER_STORY_STATE).
#define TOWER_STORY_NOT_MET     0
#define TOWER_STORY_MET         1
#define TOWER_STORY_WISH        2 // His own wish surfaced after the player's Silver Symbol.
#define TOWER_STORY_RETURNED    3 // Won the release battle; his mind is back but he is not cured yet.
#define TOWER_STORY_RESCUED     4

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
