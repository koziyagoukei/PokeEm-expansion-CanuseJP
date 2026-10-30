#include "global.h"
#include "frontier_guest.h"
#include "frontier_story.h"
#include "event_data.h"
#include "frontier_util.h"
#include "random.h"
#include "string_util.h"
#include "constants/battle_frontier.h"
#include "constants/event_objects.h"
#include "constants/frontier_util.h"

#define GUEST(id, name, type, pic, object, class, female) \
    [TRAINER_FRONTIER_##id - TRAINER_FRONTIER_GUEST_FIRST] = {_(name), OBJ_EVENT_GFX_##object, TRAINER_PIC_##pic, TRAINER_CLASS_##class, TYPE_##type, female}

static const struct FrontierGuestTrainer sGuests[FRONTIER_GUEST_COUNT] =
{
    GUEST(NORMAN, "センリ", NORMAL, LEADER_NORMAN, NORMAN, LEADER, FALSE),
    GUEST(FLANNERY, "アスナ", FIRE, LEADER_FLANNERY, FLANNERY, LEADER, TRUE),
    GUEST(MAXIE, "マツブサ", FIRE, MAGMA_LEADER_MAXIE, MAXIE, MAGMA_LEADER, FALSE),
    GUEST(WALLACE, "ミクリ", WATER, CHAMPION_WALLACE, WALLACE, CHAMPION, FALSE),
    GUEST(ARCHIE, "アオギリ", WATER, AQUA_LEADER_ARCHIE, ARCHIE, AQUA_LEADER, FALSE),
    GUEST(JUAN, "アダン", WATER, LEADER_JUAN, JUAN, LEADER, FALSE),
    GUEST(VICTOR, "ハルヒコ", GRASS, POKEFAN_M, POKEFAN_M, WINSTRATE, FALSE),
    GUEST(VICTORIA, "ヤスエ", GRASS, POKEFAN_F, POKEFAN_F, WINSTRATE, TRUE),
    GUEST(VIVI, "アキ", GRASS, LASS, LASS, WINSTRATE, TRUE),
    GUEST(VICKY, "ミツヨ", GRASS, EXPERT_F, EXPERT_F, WINSTRATE, TRUE),
    GUEST(VITO, "リョウヘイ", GRASS, COOLTRAINER_M, MAN_3, COOLTRAINER, FALSE),
    GUEST(WINONA, "ナギ", FLYING, LEADER_WINONA, WINONA, LEADER, TRUE),
    GUEST(BRAWLY, "トウキ", FIGHTING, LEADER_BRAWLY, BRAWLY, LEADER, FALSE),
    GUEST(TATE_AND_LIZA, "フウとラン", PSYCHIC, LEADER_TATE_AND_LIZA, TATE, LEADER, FALSE),
    GUEST(SIDNEY, "カゲツ", DARK, ELITE_FOUR_SIDNEY, SIDNEY, ELITE_FOUR, FALSE),
    GUEST(GLACIA, "プリム", ICE, ELITE_FOUR_GLACIA, GLACIA, ELITE_FOUR, TRUE),
    GUEST(MATT, "ウシオ", ICE, AQUA_ADMIN_M, AQUA_MEMBER_M, AQUA_ADMIN, FALSE),
    GUEST(STEVEN, "ダイゴ", STEEL, STEVEN, STEVEN, CHAMPION, FALSE),
    GUEST(ROXANNE, "ツツジ", ROCK, LEADER_ROXANNE, ROXANNE, LEADER, TRUE),
    GUEST(SHELLY, "イズミ", POISON, AQUA_ADMIN_F, AQUA_MEMBER_F, AQUA_ADMIN, TRUE),
    GUEST(TABITHA, "ホムラ", GROUND, MAGMA_ADMIN, MAGMA_MEMBER_M, MAGMA_ADMIN, FALSE),
    GUEST(WATTSON, "テッセン", ELECTRIC, LEADER_WATTSON, WATTSON, LEADER, FALSE),
    GUEST(COURTNEY, "カガリ", BUG, MAGMA_GRUNT_F, MAGMA_MEMBER_F, MAGMA_ADMIN, TRUE),
    GUEST(PHOEBE, "フヨウ", GHOST, ELITE_FOUR_PHOEBE, PHOEBE, ELITE_FOUR, TRUE),
    GUEST(WALLY, "ミツル", FAIRY, WALLY, WALLY, RIVAL, FALSE),
    GUEST(DRAKE, "ゲンジ", DRAGON, ELITE_FOUR_DRAKE, DRAKE, ELITE_FOUR, FALSE),
    GUEST(TATE, "フウ", PSYCHIC, LEADER_TATE_AND_LIZA, TATE, LEADER, FALSE),
    GUEST(LIZA, "ラン", PSYCHIC, LEADER_TATE_AND_LIZA, LIZA, LEADER, TRUE),
};

#undef GUEST

static const u8 sTextChallenge[] = _("ここで あえるとはね！\nさあ 勝負を 始めよう！");
static const u8 sTextTwinsChallenge[] = _("フウとランの コンビネーション！\nキミに 見せてあげる！");
static const u8 sTextDefeated[] = _("見事な 勝負だった！\nまた ここで あおう！");
static const u8 sTextWon[] = _("いい 勝負だった！\nまたの 挑戦を 待っている！");

STATIC_ASSERT(TRAINER_FRONTIER_GUEST_END < TRAINER_PARTNER(PARTNER_NONE), FrontierGuestIdsDoNotOverlapPartners);

const struct FrontierGuestTrainer *GetFrontierGuestTrainer(u16 trainerId)
{
    if (trainerId < TRAINER_FRONTIER_GUEST_FIRST || trainerId >= TRAINER_FRONTIER_GUEST_END)
        return NULL;
    return &sGuests[trainerId - TRAINER_FRONTIER_GUEST_FIRST];
}

bool8 IsFrontierGuestBattle(u8 battleNum)
{
    u32 facility = VarGet(VAR_FRONTIER_FACILITY);

    if (gSaveBlock2Ptr->frontier.lvlMode == FRONTIER_LVL_TENT
        || battleNum != FRONTIER_STAGES_PER_CHALLENGE - 1)
        return FALSE;
    if (facility != FRONTIER_FACILITY_TOWER && facility != FRONTIER_FACILITY_PALACE
        && facility != FRONTIER_FACILITY_ARENA && facility != FRONTIER_FACILITY_FACTORY)
        return FALSE;
    return GetFrontierBrainStatus() == FRONTIER_BRAIN_NOT_READY;
}

u16 GetRandomFrontierGuestTrainer(void)
{
    return TRAINER_FRONTIER_GUEST_FIRST + Random() % FRONTIER_GUEST_RANDOM_COUNT;
}

void SetFrontierGuestMultiPair(u16 *trainerIds)
{
    u16 first, second;

    if (VarGet(VAR_FRONTIER_FACILITY) != FRONTIER_FACILITY_TOWER
        || (VarGet(VAR_FRONTIER_BATTLE_MODE) != FRONTIER_MODE_MULTIS
            && VarGet(VAR_FRONTIER_BATTLE_MODE) != FRONTIER_MODE_LINK_MULTIS)
        || !IsFrontierGuestBattle(FRONTIER_STAGES_PER_CHALLENGE - 1))
        return;

    first = GetRandomFrontierGuestTrainer();
    second = GetRandomFrontierGuestTrainer();
    if (first == TRAINER_FRONTIER_TATE_AND_LIZA || second == TRAINER_FRONTIER_TATE_AND_LIZA)
    {
        first = TRAINER_FRONTIER_TATE;
        second = TRAINER_FRONTIER_LIZA;
    }
    else
    {
        while (second == first || second == TRAINER_FRONTIER_TATE_AND_LIZA)
            second = GetRandomFrontierGuestTrainer();
    }
    trainerIds[2 * (FRONTIER_STAGES_PER_CHALLENGE - 1)] = first;
    trainerIds[2 * (FRONTIER_STAGES_PER_CHALLENGE - 1) + 1] = second;
}

u8 GetFrontierGuestMonFilter(u16 trainerId, u8 slot)
{
    const struct FrontierGuestTrainer *guest = GetFrontierGuestTrainer(trainerId);

    if (guest != NULL && slot < 2)
        return FRONTIER_MON_FILTER_TYPE(guest->type);
    return FRONTIER_MON_FILTER_NONE;
}

void CopyFrontierGuestText(u8 whichText, u16 trainerId)
{
    // Use a local string, not record-mixing/apprentice Easy Chat storage.
    if (FrontierStory_TryCopyGuestText(whichText, trainerId))
        return;
    if (whichText == FRONTIER_BEFORE_TEXT)
    {
        if (trainerId == TRAINER_FRONTIER_TATE_AND_LIZA
            || trainerId == TRAINER_FRONTIER_TATE || trainerId == TRAINER_FRONTIER_LIZA)
            StringCopy(gStringVar4, sTextTwinsChallenge);
        else
            StringCopy(gStringVar4, sTextChallenge);
    }
    else if (whichText == FRONTIER_PLAYER_WON_TEXT)
        StringCopy(gStringVar4, sTextDefeated);
    else
        StringCopy(gStringVar4, sTextWon);
}
