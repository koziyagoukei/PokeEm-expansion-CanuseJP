#include "global.h"
#include "event_data.h"
#include "frontier_story.h"
#include "party_eat_visitors.h"
#include "random.h"
#include "string_util.h"
#include "constants/characters.h"
#include "constants/event_objects.h"
#include "constants/frontier_guest.h"
#include "constants/maps.h"

#define VISITOR_COUNT (NAMED_STORY_COUNT + STORY_TARGET_COUNT)
#define TWINS_ID (TRAINER_FRONTIER_TATE_AND_LIZA - TRAINER_FRONTIER_GUEST_FIRST)

static const u8 *const sMealTexts[] = {
    COMPOUND_STRING("{JPN}{STR_VAR_1}は 定食を食べている。\nいつもより 穏やかな顔をしている。\p話しかけると 小さく笑って\n空いた席を 指した。"),
    COMPOUND_STRING("{JPN}{STR_VAR_1}は 出来たての料理を\n少しずつ 味わっている。\p急いで食べる 必要はない。\n今日は ゆっくりしていくようだ。"),
    COMPOUND_STRING("{JPN}{STR_VAR_1}の前には\nきれいに 空になった皿がある。\p店主の方を 見ている。\nもう一品 頼もうか迷っているようだ。"),
    COMPOUND_STRING("{JPN}{STR_VAR_1}は 湯気の立つ料理を\n嬉しそうに 眺めている。\p食事の合間に ポケモン達へ\n優しく 声をかけている。"),
};

static bool32 IsDiningRoom(void)
{
    return gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_PARTY_EAT_TIME)
        && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_PARTY_EAT_TIME);
}

static bool32 IsRescued(u32 id)
{
    if (VarGet(VAR_FRONTIER_INTRO_STATE) != FRONTIER_INTRO_COMPLETE || id >= VISITOR_COUNT)
        return FALSE;
    return id < NAMED_STORY_COUNT ? FrontierStory_GetNamedState(id) == NAMED_STORY_RESCUED
        : FrontierStory_IsTargetRescued(id - NAMED_STORY_COUNT);
}

static void PlaceVisitor(u32 slot, u32 id)
{
    u16 gfx;
    if (id == TWINS_ID)
        gfx = slot == 0 ? OBJ_EVENT_GFX_TATE : OBJ_EVENT_GFX_LIZA;
    else if (id < NAMED_STORY_COUNT)
        gfx = FrontierStory_GetNamedObjectGfx(id);
    else
        gfx = FrontierStory_GetTargetObjectGfx(id - NAMED_STORY_COUNT);
    VarSet(VAR_EAT_VISITOR + slot, id);
    VarSet(VAR_EAT_VISITOR_GFX + slot, gfx);
    FlagClear(FLAG_EAT_VISITOR_HIDDEN + slot);
}

void PartyEat_PickVisitors(void)
{
    u16 candidates[VISITOR_COUNT];
    u32 count = 0;
    if (!IsDiningRoom())
        return;
    for (u32 slot = 0; slot < EAT_VISITOR_SLOTS; slot++)
    {
        VarSet(VAR_EAT_VISITOR + slot, EAT_VISITOR_NONE);
        VarSet(VAR_EAT_VISITOR_GFX + slot, OBJ_EVENT_GFX_NINJA_BOY);
        FlagSet(FLAG_EAT_VISITOR_HIDDEN + slot);
    }
    for (u32 id = 0; id < VISITOR_COUNT; id++)
        if (IsRescued(id))
            candidates[count++] = id;
    if (count == 0)
        return;
    u32 pick = RandomUniform(RNG_NONE, 0, count - 1);
    u32 first = candidates[pick];
    PlaceVisitor(0, first);
    if (first == TWINS_ID)
    {
        PlaceVisitor(1, first);
        return;
    }
    candidates[pick] = candidates[--count];
    // A twin pair must fit together; never add half of it as the second customer.
    for (u32 i = 0; i < count; i++)
        if (candidates[i] == TWINS_ID)
        {
            candidates[i] = candidates[--count];
            break;
        }
    if (count && RandomUniform(RNG_NONE, 1, 2) == 2)
        PlaceVisitor(1, candidates[RandomUniform(RNG_NONE, 0, count - 1)]);
}

void PartyEat_BufferVisitorText(void)
{
    u32 slot = gSpecialVar_LastTalked - EAT_VISITOR_LOCAL_ID;
    gStringVar4[0] = EOS;
    gSpecialVar_Result = FALSE;
    if (!IsDiningRoom() || slot >= EAT_VISITOR_SLOTS || FlagGet(FLAG_EAT_VISITOR_HIDDEN + slot))
        return;
    u32 id = VarGet(VAR_EAT_VISITOR + slot);
    if (!IsRescued(id))
        return;
    const u8 *name = id < NAMED_STORY_COUNT ? FrontierStory_GetNamedName(id)
        : FrontierStory_GetTargetName(id - NAMED_STORY_COUNT);
    StringCopy(gStringVar1, name);
    StringExpandPlaceholders(gStringVar4, sMealTexts[id % ARRAY_COUNT(sMealTexts)]);
    gSpecialVar_Result = TRUE;
}
