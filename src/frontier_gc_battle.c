// Frontier content: GC Battle test (GC_MODE_BATTLE, SINGLE 1vs1).
//
// Started from the debug menu. Builds a GcBattleRequestV1 from the player's
// lead Pokemon and the lead of a trainer's party, resolved from this game's
// own move/species/type data, and runs it through the generic GC Extension
// connector. The GameCube owns the temporary battle state; the GBA is the
// command input and second screen. The result is shown; with the "apply"
// variant, HP and PP of the lead are written back once, after the link is
// closed and the result was validated. Nothing else is changed.
#include "global.h"
#include "battle_main.h"
#include "battle_setup.h"
#include "data.h"
#include "event_data.h"
#include "main.h"
#include "malloc.h"
#include "move.h"
#include "pokemon.h"
#include "random.h"
#include "sound.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/characters.h"
#include "constants/songs.h"
#include "gc_extension.h"
#include "gc_ext/battle_protocol.h"
#include "frontier_gc_battle.h"

#define REQUEST_CAPACITY (BATTLE_REQUEST_HEADER + 2 * BATTLE_ENTRY_BYTES + NUMBER_OF_MON_TYPES * NUMBER_OF_MON_TYPES)

enum GbaScreen { SCREEN_PREPARE, SCREEN_COMMAND, SCREEN_MOVES, SCREEN_WAIT, SCREEN_ENDED };

struct FrontierGcBattle
{
    struct Pokemon opponent[PARTY_SIZE];
    u8 request[REQUEST_CAPACITY];
    u32 requestLength;
    u32 battleId;
    u32 leadPersonality;
    u16 trainerId;
    u8 environment;
    u8 leadSlot;
    u8 policy;
    u8 screen;
    u8 cursor;
    u8 supportedMask;
    BattleState state;
    bool8 haveState;
    BattleAction pending;
    bool8 resultAccepted;
    bool8 applied;
    BattleResult result;
    u8 message[48];
};

static EWRAM_DATA struct FrontierGcBattle *sBattle = NULL;
static EWRAM_DATA u8 sPendingPolicy = 0;
static EWRAM_DATA u16 sPendingTrainer = 0;

static const u8 sText_NoMemory[] = _("{ENG}Not enough memory.");
static const u8 sText_NoLead[] = _("{ENG}No able Pokemon with a supported move.");
static const u8 sText_NoOpponent[] = _("{ENG}The trainer has no Pokemon.");
static const u8 sText_NoMoves[] = _("{ENG}Lead has no supported move.");
static const u8 sText_NoOpponentMoves[] = _("{ENG}Opponent has no supported move.");
static const u8 sText_Prepare[] = _("{ENG}GC Battle: waiting for the GameCube");
static const u8 sText_Fight[] = _("たたかう");
static const u8 sText_Cursor[] = _("{ENG}>");
static const u8 sText_Unsupported[] = _("{ENG}(unsupported)");
static const u8 sText_Wait[] = _("{ENG}GameCube is resolving the turn...");
static const u8 sText_Win[] = _("{ENG}WIN!");
static const u8 sText_Lose[] = _("{ENG}LOSE...");
static const u8 sText_Cancelled[] = _("{ENG}Battle cancelled.");
static const u8 sText_Refused[] = _("{ENG}Battle not supported.");
static const u8 sText_HelpCommand[] = _("{ENG}A: Fight   B: Forfeit");
static const u8 sText_HelpMoves[] = _("{ENG}A: Use move   B: Back");
static const u8 sText_HelpEnded[] = _("{ENG}A: Return");
static const u8 sText_Vs[] = _("{ENG}VS ");
static const u8 sText_Lv[] = _("{ENG} Lv.");
static const u8 sText_Hp[] = _("{ENG}HP ");
static const u8 sText_Pp[] = _("{ENG}PP ");
static const u8 sText_Slash[] = _("{ENG}/");
static const u8 sText_Jpn[] = _("{JPN}");
static const u8 sText_Eng[] = _("{ENG}");
static const u8 sText_NotApplied[] = _("{ENG}Result shown only; party unchanged.");
static const u8 sText_NoResult[] = _("{ENG}No battle result; party unchanged.");
static const u8 sText_AppliedHp[] = _("{ENG}Applied to lead: HP ");
static const u8 sText_ApplyRefused[] = _("{ENG}Result not applied (lead changed).");

// ------------------------------------------------------------- request

static u8 TypeQuarters(enum Type attack, enum Type defend)
{
    return (gTypeEffectivenessTable[attack][defend] + 512) >> 10;
}

static void ResolveMove(enum Move move, BattleMove *out)
{
    memset(out, 0, sizeof(*out));
    if (move == MOVE_NONE || move >= MOVES_COUNT_ALL)
    {
        out->unsupported = UNSUP_EMPTY;
        return;
    }
    out->power = GetMovePower(move);
    out->type = GetMoveType(move);
    switch (GetMoveCategory(move))
    {
    case DAMAGE_CATEGORY_PHYSICAL: out->category = BCAT_PHYSICAL; break;
    case DAMAGE_CATEGORY_SPECIAL:  out->category = BCAT_SPECIAL; break;
    default:                       out->category = BCAT_STATUS; break;
    }
    out->accuracy = GetMoveAccuracy(move);
    out->priority = GetMovePriority(move);
    switch (GetMoveTarget(move))
    {
    case TARGET_SELECTED:      out->target = BTARGET_SINGLE; break;
    case TARGET_BOTH:
    case TARGET_FOES_AND_ALLY: out->target = BTARGET_ALL_FOES; break;
    default:                   out->target = BTARGET_OTHER; break;
    }
    // Only plain, single-hit, single-target damaging moves are supported by
    // the Phase 3 battle core; everything else is marked, never approximated.
    if (GetMoveEffect(move) != EFFECT_HIT)
        out->unsupported |= UNSUP_EFFECT;
    if (GetMoveAdditionalEffectCount(move) != 0)
        out->unsupported |= UNSUP_SECONDARY;
    if (out->priority != 0)
        out->unsupported |= UNSUP_PRIORITY;
    if (out->target == BTARGET_OTHER)
        out->unsupported |= UNSUP_TARGET;
    if (out->category == BCAT_STATUS || out->power == 0)
        out->unsupported |= UNSUP_STATUS_MOVE;
    if (GetMoveStrikeCount(move) > 1 || gMovesInfo[move].multiHit)
        out->unsupported |= UNSUP_MULTI_HIT;
    if (IsExplosionMove(move) || gMovesInfo[move].alwaysCriticalHit || gMovesInfo[move].cantUseTwice
     || gMovesInfo[move].noAffectOnSameTypeTarget || gMovesInfo[move].accDecreaseIfUserNotSameType
     || out->accuracy > 100 || out->type >= NUMBER_OF_MON_TYPES)
        out->unsupported |= UNSUP_OTHER;
}

static u8 FillEntry(u32 slot, BattleEntry *entry)
{
    struct Pokemon *mon = GcExtension_GetExportMon(slot);
    enum Species species;
    u32 i;
    u8 mask = 0;

    GcExtension_BuildRecord(slot, &entry->record);
    species = SanitizeSpeciesId(GetMonData(mon, MON_DATA_SPECIES));
    // Where the game would place this picture (battle_anim_mons.c).
    entry->mon.front_y = gSpeciesInfo[species].frontPicYOffset;
    entry->mon.back_y = gSpeciesInfo[species].backPicYOffset;
    entry->mon.elevation = gSpeciesInfo[species].enemyMonElevation;
    entry->mon.types[0] = gSpeciesInfo[species].types[0];
    entry->mon.types[1] = gSpeciesInfo[species].types[1];
    entry->mon.type_count = entry->mon.types[0] == entry->mon.types[1] ? 1 : 2;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        ResolveMove(entry->record.moves[i], &entry->mon.moves[i]);
        if (entry->mon.moves[i].unsupported == 0 && entry->record.pp[i] > 0)
            mask |= 1u << i;
    }
    return mask;
}

// Test rule: the player's battler is the first party member that can battle
// and knows at least one move the GC battle core supports.
static s32 FindLead(BattleEntry *scratch)
{
    u32 i;
    for (i = 0; i < gPartiesCount[B_TRAINER_PLAYER]; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG)
         && GetMonData(mon, MON_DATA_HP) != 0 && FillEntry(i, scratch) != 0)
            return i;
    }
    return -1;
}

static bool8 BuildRequest(const u8 **reason)
{
    BattleRequest *request = AllocZeroedUnchecked(sizeof(*request));
    const struct Trainer *trainer = GetTrainerStructFromId(sBattle->trainerId);
    u32 a, d;
    s32 lead;

    if (request == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    lead = FindLead(&request->player[0]);
    if (lead < 0)
    {
        *reason = sText_NoLead;
        Free(request);
        return FALSE;
    }
    // The opponent lives in a mode-owned buffer, exported as slot 0x80.
    CreateNPCTrainerPartyFromTrainer(sBattle->opponent, trainer);
    if (GetMonData(&sBattle->opponent[0], MON_DATA_SPECIES) == SPECIES_NONE)
    {
        *reason = sText_NoOpponent;
        Free(request);
        return FALSE;
    }
    GcExtension_SetExtraMons(sBattle->opponent, 1);

    sBattle->leadSlot = lead;
    sBattle->leadPersonality = GetMonData(&gParties[B_TRAINER_PLAYER][lead], MON_DATA_PERSONALITY);
    sBattle->battleId = Random32() | 1;
    request->format = BATTLE_FORMAT_SINGLE;
    request->battle_type = BTYPE_TRAINER;
    request->player_count = 1;
    request->opponent_count = 1;
    request->active_per_side = 1;
    request->rules = RULE_IGNORE_ABILITIES | RULE_IGNORE_ITEMS | RULE_IGNORE_STATUS | RULE_NO_GIMMICKS
                   | RULE_NO_CRITICAL_HITS | RULE_SUPPORTED_MOVES_ONLY;
    request->trainer_id = sBattle->trainerId;
    request->trainer_class = trainer->trainerClass;
    request->reward_mode = 0;
    request->result_policy = sBattle->policy;
    request->type_count = NUMBER_OF_MON_TYPES;
    request->battle_id = sBattle->battleId;
    request->environment = sBattle->environment;
    sBattle->supportedMask = FillEntry(lead, &request->player[0]);
    if (FillEntry(GC_EXTRA_SLOT_BASE, &request->opponent[0]) == 0)
    {
        *reason = sText_NoOpponentMoves;
        Free(request);
        return FALSE;
    }
    if (sBattle->supportedMask == 0)
    {
        *reason = sText_NoMoves;
        Free(request);
        return FALSE;
    }
    for (a = 0; a < NUMBER_OF_MON_TYPES; a++)
        for (d = 0; d < NUMBER_OF_MON_TYPES; d++)
            request->type_chart[a * NUMBER_OF_MON_TYPES + d] = TypeQuarters(a, d);
    sBattle->requestLength = battle_request_encode(request, sBattle->request, sizeof(sBattle->request));
    Free(request);
    if (sBattle->requestLength == 0)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    return TRUE;
}

// ------------------------------------------------------------- handler

static bool8 Init(const u8 **reason)
{
    sBattle = AllocZeroedUnchecked(sizeof(*sBattle));
    if (sBattle == NULL)
    {
        *reason = sText_NoMemory;
        return FALSE;
    }
    sBattle->policy = sPendingPolicy;
    sBattle->trainerId = sPendingTrainer;
    sBattle->environment = BattleSetup_GetEnvironmentId();
    sBattle->screen = SCREEN_PREPARE;
    return BuildRequest(reason);
}

static const u8 *GetRequest(u32 *length)
{
    *length = sBattle->requestLength;
    return sBattle->request;
}

static u16 HandleApp(u16 opcode, const u8 *args, u16 argLength, u8 *out, u16 *outLength)
{
    BattleState state;
    BattleAction action = {0};

    switch (opcode)
    {
    case BATTLE_OP_STATE:
        if (!battle_state_decode(args, argLength, &state))
            return APP_BAD_ARGS;
        sBattle->state = state;
        sBattle->haveState = TRUE;
        memset(&sBattle->pending, 0, sizeof(sBattle->pending));
        if (state.phase == BPHASE_CHOOSE)
        {
            sBattle->screen = SCREEN_COMMAND;
            sBattle->cursor = 0;
        }
        else
        {
            sBattle->screen = state.phase == BPHASE_ENDED ? SCREEN_ENDED : SCREEN_WAIT;
        }
        GcExtension_Redraw();
        return APP_OK;
    case BATTLE_OP_ACTION:
        if (sBattle->pending.action != BACTION_NONE)
            action = sBattle->pending;
        else
            action.turn = sBattle->state.turn;
        if (GcExtension_IsExitRequested())
            action.flags |= BACTION_FLAG_EXIT;
        *outLength = battle_action_encode(&action, out);
        return APP_OK;
    }
    return APP_UNKNOWN_OP;
}

static u16 OnResult(u16 version, const u8 *data, u16 length)
{
    BattleResult result;
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u32 i;

    if (version != BATTLE_RESULT_VERSION || !battle_result_decode(data, length, &result))
        return RESULT_REJECTED;
    if (result.battle_id != sBattle->battleId || result.player_count != 1 || result.player[0].slot != sBattle->leadSlot
     || result.player[0].hp > GetMonData(lead, MON_DATA_MAX_HP))
        return RESULT_REJECTED;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u16 move = GetMonData(lead, MON_DATA_MOVE1 + i);
        u8 maxPp = move != MOVE_NONE ? CalculatePPWithBonus(move, GetMonData(lead, MON_DATA_PP_BONUSES), i) : 0;
        if (result.player[0].pp[i] > maxPp)
            return RESULT_REJECTED;
    }
    sBattle->result = result;
    sBattle->resultAccepted = TRUE;
    return RESULT_ACCEPTED;
}

static void AppendNumber(u8 *dest, s32 value)
{
    u8 digits[8];
    ConvertIntToDecimalStringN(digits, value, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringAppend(dest, sText_Eng);
    StringAppend(dest, digits);
}

// Runs once, after the link is closed. Only the "apply" policy writes HP/PP,
// only for a validated result, only to the same lead Pokemon, only once.
static const u8 *Finish(bool8 *changedParty)
{
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u32 i, before;

    if (!sBattle->resultAccepted)
        return sText_NoResult;
    if (sBattle->policy != RESULT_POLICY_APPLY_HP_PP || sBattle->applied
     || sBattle->result.outcome == OUTCOME_CANCELLED || sBattle->result.outcome == OUTCOME_UNSUPPORTED)
        return sText_NotApplied;
    if (GetMonData(lead, MON_DATA_PERSONALITY) != sBattle->leadPersonality)
        return sText_ApplyRefused;
    before = GetMonData(lead, MON_DATA_HP);
    SetMonData(lead, MON_DATA_HP, &sBattle->result.player[0].hp);
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonData(lead, MON_DATA_PP1 + i, &sBattle->result.player[0].pp[i]);
    sBattle->applied = TRUE;
    *changedParty = TRUE;
    StringCopy(sBattle->message, sText_AppliedHp);
    AppendNumber(sBattle->message, before);
    StringAppend(sBattle->message, sText_Slash);
    AppendNumber(sBattle->message, sBattle->result.player[0].hp);
    return sBattle->message;
}

void FrontierGcBattle_GetScene(struct FrontierGcBattleScene *scene)
{
    scene->player = sBattle != NULL ? &gParties[B_TRAINER_PLAYER][sBattle->leadSlot] : NULL;
    scene->opponent = sBattle != NULL ? &sBattle->opponent[0] : NULL;
    scene->trainerId = sBattle != NULL ? sBattle->trainerId : 0;
    scene->environment = sBattle != NULL ? sBattle->environment : 0;
}

static void FreeBattle(void)
{
    FrontierGcBattle_FreeGfx();
    GcExtension_SetExtraMons(NULL, 0);
    TRY_FREE_AND_SET_NULL(sBattle);
}

// ------------------------------------------------------------- GBA screen

static void AppendMonLine(u8 *text, struct Pokemon *mon)
{
    u8 name[POKEMON_NAME_BUFFER_SIZE];
    GetMonData(mon, MON_DATA_NICKNAME, name);
    StringAppend(text, sText_Jpn);
    StringAppend(text, name);
    StringAppend(text, sText_Lv);
    AppendNumber(text, GetMonData(mon, MON_DATA_LEVEL));
}

static void DrawHp(u8 body, u8 line, u16 hp, u16 max)
{
    u8 text[32];
    StringCopy(text, sText_Hp);
    AppendNumber(text, hp);
    StringAppend(text, sText_Slash);
    AppendNumber(text, max);
    GcExtension_PrintLine(body, text, 136, line, GC_COLOR_NORMAL);
}

static void Draw(u8 body, u8 help)
{
    struct Pokemon *lead = &gParties[B_TRAINER_PLAYER][sBattle->leadSlot];
    u8 text[64];
    u32 i;

    if (!sBattle->haveState)
    {
        GcExtension_PrintLine(body, sText_Prepare, 0, 0, GC_COLOR_NORMAL);
        return;
    }
    StringCopy(text, sText_Vs);
    AppendMonLine(text, &sBattle->opponent[0]);
    GcExtension_PrintLine(body, text, 0, 0, GC_COLOR_DIM);
    DrawHp(body, 0, sBattle->state.opponent_hp, sBattle->state.opponent_max_hp);
    text[0] = EOS;
    AppendMonLine(text, lead);
    GcExtension_PrintLine(body, text, 0, 1, GC_COLOR_NORMAL);
    DrawHp(body, 1, sBattle->state.player_hp, sBattle->state.player_max_hp);

    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        GcExtension_PrintLine(body, sText_Cursor, 0, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(body, sText_Fight, 12, 3, GC_COLOR_HIGHLIGHT);
        GcExtension_PrintLine(help, sText_HelpCommand, 0, 0, GC_COLOR_NORMAL);
        break;
    case SCREEN_MOVES:
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            u16 move = GetMonData(lead, MON_DATA_MOVE1 + i);
            bool8 usable = (sBattle->supportedMask & (1u << i)) && sBattle->state.pp[i] > 0;
            u8 color = !usable ? GC_COLOR_DIM : i == sBattle->cursor ? GC_COLOR_HIGHLIGHT : GC_COLOR_NORMAL;
            if (move == MOVE_NONE)
                continue;
            if (i == sBattle->cursor)
                GcExtension_PrintLine(body, sText_Cursor, 0, 2 + i, color);
            StringCopy(text, sText_Jpn);
            StringAppend(text, GetMoveName(move));
            GcExtension_PrintLine(body, text, 12, 2 + i, color);
            if (!(sBattle->supportedMask & (1u << i)))
            {
                GcExtension_PrintLine(body, sText_Unsupported, 120, 2 + i, GC_COLOR_DIM);
            }
            else
            {
                StringCopy(text, sText_Pp);
                AppendNumber(text, sBattle->state.pp[i]);
                StringAppend(text, sText_Slash);
                AppendNumber(text, CalculatePPWithBonus(move, GetMonData(lead, MON_DATA_PP_BONUSES), i));
                GcExtension_PrintLine(body, text, 150, 2 + i, color);
            }
        }
        GcExtension_PrintLine(help, sText_HelpMoves, 0, 0, GC_COLOR_NORMAL);
        break;
    case SCREEN_WAIT:
        GcExtension_PrintLine(body, sText_Wait, 0, 3, GC_COLOR_DIM);
        break;
    case SCREEN_ENDED:
        switch (sBattle->state.outcome)
        {
        case OUTCOME_WIN:  GcExtension_PrintLine(body, sText_Win, 0, 3, GC_COLOR_HIGHLIGHT); break;
        case OUTCOME_LOSE: GcExtension_PrintLine(body, sText_Lose, 0, 3, GC_COLOR_ERROR); break;
        case OUTCOME_CANCELLED: GcExtension_PrintLine(body, sText_Cancelled, 0, 3, GC_COLOR_NORMAL); break;
        default: GcExtension_PrintLine(body, sText_Refused, 0, 3, GC_COLOR_ERROR); break;
        }
        GcExtension_PrintLine(help, sText_HelpEnded, 0, 0, GC_COLOR_NORMAL);
        break;
    }
}

static bool8 MoveUsable(u8 slot)
{
    return (sBattle->supportedMask & (1u << slot)) && sBattle->state.pp[slot] > 0;
}

static void Input(void)
{
    u32 i;
    switch (sBattle->screen)
    {
    case SCREEN_COMMAND:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->screen = SCREEN_MOVES;
            for (sBattle->cursor = 0; sBattle->cursor < MAX_MON_MOVES && !MoveUsable(sBattle->cursor); sBattle->cursor++)
                ;
            if (sBattle->cursor == MAX_MON_MOVES)
                sBattle->cursor = 0;
            GcExtension_Redraw();
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->pending.turn = sBattle->state.turn;
            sBattle->pending.action = BACTION_FORFEIT;
            sBattle->screen = SCREEN_WAIT;
            GcExtension_Redraw();
        }
        break;
    case SCREEN_MOVES:
        if (JOY_REPEAT(DPAD_UP) || JOY_REPEAT(DPAD_DOWN))
        {
            s32 step = JOY_REPEAT(DPAD_UP) ? MAX_MON_MOVES - 1 : 1;
            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                sBattle->cursor = (sBattle->cursor + step) % MAX_MON_MOVES;
                if (GetMonData(&gParties[B_TRAINER_PLAYER][sBattle->leadSlot], MON_DATA_MOVE1 + sBattle->cursor) != MOVE_NONE)
                    break;
            }
            PlaySE(SE_SELECT);
            GcExtension_Redraw();
        }
        else if (JOY_NEW(A_BUTTON) && MoveUsable(sBattle->cursor))
        {
            PlaySE(SE_SELECT);
            sBattle->pending.turn = sBattle->state.turn;
            sBattle->pending.action = BACTION_MOVE;
            sBattle->pending.move_slot = sBattle->cursor;
            sBattle->screen = SCREEN_WAIT;
            GcExtension_Redraw();
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_FAILURE);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            sBattle->screen = SCREEN_COMMAND;
            GcExtension_Redraw();
        }
        break;
    case SCREEN_ENDED:
        if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            GcExtension_RequestExit();
        }
        break;
    }
}

static const struct GcModeHandler sFrontierGcBattleMode =
{
    .modeId = GC_MODE_BATTLE,
    .modeVersion = BATTLE_MODE_VERSION,
    .appId = APP_GC_BATTLE,
    .appVersion = BATTLE_APP_VERSION,
    .init = Init,
    .getRequest = GetRequest,
    .handleApp = HandleApp,
    .draw = Draw,
    .input = Input,
    .onResult = OnResult,
    .finish = Finish,
    .free = FreeBattle,
    .resolveAsset = FrontierGcBattle_ResolveAsset,
};

// Script special (waitstate): VAR_0x8004 = result policy (0 show, 1 apply
// HP/PP), VAR_0x8005 = trainer id whose lead Pokemon is the opponent.
void FrontierGcBattle_Start(void)
{
    sPendingPolicy = gSpecialVar_0x8004 == RESULT_POLICY_APPLY_HP_PP ? RESULT_POLICY_APPLY_HP_PP : RESULT_POLICY_DISPLAY_ONLY;
    sPendingTrainer = gSpecialVar_0x8005;
    GcStartMode(&sFrontierGcBattleMode);
}
