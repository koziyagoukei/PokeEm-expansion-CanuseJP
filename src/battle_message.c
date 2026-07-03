#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_ai_record.h"
#include "battle_controllers.h"
#include "battle_message.h"
#include "battle_setup.h"
#include "battle_special.h"
#include "battle_z_move.h"
#include "data.h"
#include "event_data.h"
#include "frontier_util.h"
#include "graphics.h"
#include "international_string_util.h"
#include "item.h"
#include "link.h"
#include "menu.h"
#include "palette.h"
#include "recorded_battle.h"
#include "string_util.h"
#include "strings.h"
#include "test_runner.h"
#include "text.h"
#include "trainer_hill.h"
#include "trainer_slide.h"
#include "trainer_tower.h"
#include "window.h"
#include "line_break.h"
#include "constants/abilities.h"
#include "constants/battle_dome.h"
#include "constants/battle_string_ids.h"
#include "constants/frontier_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/trainer_hill.h"
#include "constants/weather.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    union {
        struct {
            DEPRECATED("Use color.background instead") u8 bgColor;
            DEPRECATED("Use color.foreground instead") u8 fgColor;
            DEPRECATED("Use color.shadow instead") u8 shadowColor;
            DEPRECATED("Use color.accent instead") u8 accentColor;
        };
        union TextColor color;
    };
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
};

#if TESTING
EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#else
static EWRAM_DATA u16 sBattlerAbilities[MAX_BATTLERS_COUNT] = {0};
#endif
EWRAM_DATA struct BattleMsgData *gBattleMsgDataPtr = NULL;

// todo: make some of those names less vague: attacker/target vs pkmn, etc.

static const u8 sText_EmptyString4[] = _("{JPN}");

const u8 gText_PkmnShroudedInMist[] = _("{JPN}{B_ATK_TEAM1} ポケモンは\nしろいきりに つつまれた！");
const u8 gText_PkmnGettingPumped[] = _("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nはりきっている!");
const u8 gText_PkmnsXPreventsSwitching[] = _("{JPN}{B_BUFF1}の {B_LAST_ABILITY}で にげられない！\p");
const u8 gText_StatSharply[] = _("{JPN} ぐーんと");
const u8 gText_StatRose[] = _("{JPN}あがった！");
const u8 gText_StatFell[] = _("{JPN}さがった！");
const u8 gText_DefendersStatRose[] = _("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_BUFF1}が{B_BUFF2} あがった！");
static const u8 sText_GotAwaySafely[] = _("{JPN}{PLAY_SE SE_FLEE}うまく にげきれた！\p");
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("{JPN}{B_LINK_OPPONENT1_NAME}との\nしょうぶに かった！");
static const u8 sText_TwoLinkTrainersDefeated[] = _("{JPN}{B_LINK_OPPONENT1_NAME}と {B_LINK_OPPONENT2_NAME}との\nしょうぶに かった！");
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("{JPN}{B_LINK_OPPONENT1_NAME}との\nしょうぶに まけた！");
static const u8 sText_PlayerLostToTwo[] = _("{JPN}{B_LINK_OPPONENT1_NAME}と {B_LINK_OPPONENT2_NAME}との\nしょうぶに まけた！");
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("{JPN}{B_LINK_OPPONENT1_NAME}との\nしょうぶに ひきわけた！");
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("{JPN}{B_LINK_OPPONENT1_NAME}と {B_LINK_OPPONENT2_NAME}との\nしょうぶに ひきわけた！");
static const u8 sText_WildFled[] = _("{JPN}{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME}は にげだした！"); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_TwoWildFled[] = _("{JPN}{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME}と {B_LINK_OPPONENT2_NAME}は にげだした！"); //not in gen 5+, replaced with match was forfeited text
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}\nとの しょうぶに かった！\p");
static const u8 sText_OpponentMon1Appeared[] = _("{JPN}あ！  {B_OPPONENT_MON1_NAME}が とびだしてきた！\p");
static const u8 sText_WildPkmnAppeared[] = _("{JPN}あ！  やせいの\n{B_OPPONENT_MON1_NAME}が とびだしてきた!\p");
static const u8 sText_LegendaryPkmnAppeared[] = _("{JPN}あ！  やせいの\n{B_OPPONENT_MON1_NAME}が あらわれた!\p");
static const u8 sText_WildPkmnAppearedPause[] = _("{JPN}あ！  やせいの\n{B_OPPONENT_MON1_NAME}が とびだしてきた！{PAUSE 127}");
static const u8 sText_TwoWildPkmnAppeared[] = _("{JPN}あ！  やせいの\n{B_OPPONENT_MON1_NAME}と {B_OPPONENT_MON2_NAME}が　とびだしてきた!\p");
static const u8 sText_GhostAppearedCantId[] = _("{JPN}ゆうれいが あらわれた!\pだめだ! ゆうれい ポケモンの\nしょうたいが わからない!\p");
static const u8 sText_TheGhostAppeared[] = _("{JPN}ゆうれいが あらわれた！\p");
static const u8 sText_Trainer1WantsToBattle[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}が\nしょうぶを しかけてきた!\p");
static const u8 sText_LinkTrainerWantsToBattle[] = _("{JPN}{B_LINK_OPPONENT1_NAME}が\nしょうぶを しかけてきた!");
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{JPN}{B_LINK_OPPONENT1_NAME}と {B_LINK_OPPONENT2_NAME}が\nしょうぶを しかけてきた！");
static const u8 sText_Trainer1SentOutPkmn[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}は\n{B_OPPONENT_MON1_NAME}を くりだした！");
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{JPN}{JPN}{B_TRAINER1_NAME_WITH_CLASS}は\n{B_OPPONENT_MON1_NAME}と {B_OPPONENT_MON2_NAME}を くりだした!");
static const u8 sText_Trainer1SentOutPkmn2[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}は\n{B_BUFF1}を くりだした!");
static const u8 sText_LinkTrainerIntroSendOutPkmn[] = _("{JPN}{B_LINK_OPPONENT1_NAME}は\n{B_LINK_OPPONENT_MON1_NAME}を くりだした!");
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{JPN}{B_LINK_OPPONENT1_NAME}は\n{B_BUFF1}を くりだした!");
static const u8 sText_LinkTrainer2SentOutPkmn2[] = _("{JPN}{B_LINK_OPPONENT2_NAME}は\n{B_BUFF1}を くりだした!");
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{JPN}{B_LINK_OPPONENT1_NAME}\n{B_OPPONENT_MON1_NAME}と {B_OPPONENT_MON2_NAME}を くりだした！");
static const u8 sText_TwoLinkTrainersIntroSendOutPkmn[] = _("{JPN}{B_LINK_OPPONENT1_NAME}は {B_LINK_OPPONENT_MON1_NAME}を くりだした!\n{B_LINK_OPPONENT2_NAME}は {B_LINK_OPPONENT_MON2_NAME}を くりだした!");
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{JPN}{B_LINK_OPPONENT1_NAME}は\n{B_LINK_OPPONENT_MON2_NAME}を くりだした!");
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{JPN}{B_LINK_SCR_TRAINER_NAME}は\n{B_BUFF1}を くりだした!");
static const u8 sText_GoPkmn[] = _("{JPN}ゆけっ! {B_PLAYER_MON1_NAME}！");
static const u8 sText_GoTwoPkmn[] = _("{JPN}ゆけっ! {B_PLAYER_MON1_NAME}と {B_PLAYER_MON2_NAME}!");
static const u8 sText_GoPkmn2[] = _("{JPN}ゆけっ! {B_BUFF1}!");
static const u8 sText_DoItPkmn[] = _("{JPN}いってこい! {B_BUFF1}!");
static const u8 sText_GoForItPkmn[] = _("{JPN}がんばれ! {B_BUFF1}!");
static const u8 sText_JustALittleMorePkmn[] = _("{JPN}もうすこしだ! がんばれ {B_BUFF1}!"); //currently unused, will require code changes
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("{JPN}あいてが よわっている!\nチャンスだ！ {B_BUFF1}！");
static const u8 sText_LinkPartnerSentOutPkmn1GoPkmn[] = _("{JPN}{B_LINK_PARTNER_NAME}は {B_LINK_PLAYER_MON1_NAME}を くりだした！\nゆけっ！ {B_LINK_PLAYER_MON2_NAME}！");
static const u8 sText_LinkPartnerSentOutPkmn2GoPkmn[] = _("{JPN}{B_LINK_PARTNER_NAME}は {B_LINK_PLAYER_MON2_NAME}を くりだした！\nゆけっ！ {B_LINK_PLAYER_MON1_NAME}！");
static const u8 sText_LinkPartnerSentOutPkmn1[] = _("{JPN}{B_LINK_PARTNER_NAME}は\n{B_BUFF1}を くりだした！");
static const u8 sText_LinkPartnerSentOutPkmn2[] = _("{JPN}{B_LINK_PARTNER_NAME}は\n{B_BUFF1}を くりだした！");
static const u8 sText_LinkPartnerWithdrewPkmn1[] = _("{JPN}{B_LINK_PARTNER_NAME}は\n{B_LINK_PLAYER_MON1_NAME}を ひっこめた！");
static const u8 sText_LinkPartnerWithdrewPkmn2[] = _("{JPN}{B_LINK_PARTNER_NAME}は\n{B_LINK_PLAYER_MON2_NAME}を ひっこめた！");
static const u8 sText_PkmnSwitchOut[] = _("{JPN}こうたいだ！\n{B_BUFF1}！"); //currently unused, I believe its used for when you switch on a Pokemon in shift mode
static const u8 sText_PkmnThatsEnough[] = _("{JPN}{B_BUFF1} もういい！\nもどれ！");
static const u8 sText_PkmnComeBack[] = _("{JPN}{B_BUFF1}\nもどれ！");
static const u8 sText_PkmnOkComeBack[] = _("{JPN}{B_BUFF1} いいぞ！\nもどれ！");
static const u8 sText_PkmnGoodComeBack[] = _("{JPN}{B_BUFF1} よくやった！\nもどれ！");
static const u8 sText_Trainer1WithdrewPkmn[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}は\n{B_BUFF1}を ひっこめた！");
static const u8 sText_Trainer2WithdrewPkmn[] = _("{JPN}{B_TRAINER2_NAME_WITH_CLASS}は\n{B_BUFF1}を ひっこめた！");
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{JPN}{B_LINK_OPPONENT1_NAME}は\n{B_BUFF1}を ひっこめた！");
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{JPN}{B_LINK_OPPONENT2_NAME}は\n{B_BUFF1}を ひっこめた！");
static const u8 sText_WildPkmnPrefix[] = _("{JPN}やせいの ");
static const u8 sText_FoePkmnPrefix[] = _("{JPN}あいての ");
static const u8 sText_WildPkmnPrefixLower[] = _("{JPN}やせいの ");
static const u8 sText_FoePkmnPrefixLower[] = _("{JPN}あいての ");
static const u8 sText_EmptyString8[] = _("{JPN}");
static const u8 sText_FoePkmnPrefix2[] = _("{JPN}あいての");
static const u8 sText_AllyPkmnPrefix[] = _("{JPN}みかたの");
static const u8 sText_FoePkmnPrefix3[] = _("{JPN}あいては");
static const u8 sText_AllyPkmnPrefix2[] = _("{JPN}みかたは");
static const u8 sText_FoePkmnPrefix4[] = _("{JPN}あいてを");
static const u8 sText_AllyPkmnPrefix3[] = _("{JPN}みかたを");
static const u8 sText_AttackerUsedX[] = _("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_BUFF3}！");
static const u8 sText_ExclamationMark[] = _("{JPN}を つかった!");
static const u8 sText_ExclamationMark2[] = _("{JPN}した!");
static const u8 sText_ExclamationMark3[] = _("{JPN}を した!");
static const u8 sText_ExclamationMark4[] = _("{JPN} こうげき!");
static const u8 sText_ExclamationMark5[] = _("{JPN}！");
static const u8 sText_HP[] = _("{JPN}たいりょく");
static const u8 sText_Attack[] = _("{JPN}こうげき");
static const u8 sText_Defense[] = _("{JPN}ぼうぎょ");
static const u8 sText_Speed[] = _("{JPN}すばやさ");
static const u8 sText_SpAttack[] = _("{JPN}とくこう");
static const u8 sText_SpDefense[] = _("{JPN}とくぼう");
static const u8 sText_Accuracy[] = _("{JPN}めいちゅうりつ");
static const u8 sText_Evasiveness[] = _("{JPN}かいひりつ");

const u8 *const gStatNamesTable[NUM_BATTLE_STATS] =
{
    [STAT_HP]      = sText_HP,
    [STAT_ATK]     = sText_Attack,
    [STAT_DEF]     = sText_Defense,
    [STAT_SPEED]   = sText_Speed,
    [STAT_SPATK]   = sText_SpAttack,
    [STAT_SPDEF]   = sText_SpDefense,
    [STAT_ACC]     = sText_Accuracy,
    [STAT_EVASION] = sText_Evasiveness,
};
const u8 *const gPokeblockWasTooXStringTable[FLAVOR_COUNT] =
{
    [FLAVOR_SPICY]  = COMPOUND_STRING("{JPN}からすぎた！"),
    [FLAVOR_DRY]    = COMPOUND_STRING("{JPN}しぶすぎた！"),
    [FLAVOR_SWEET]  = COMPOUND_STRING("{JPN}あますぎた！"),
    [FLAVOR_BITTER] = COMPOUND_STRING("{JPN}にがすぎた！"),
    [FLAVOR_SOUR]   = COMPOUND_STRING("{JPN}すっぱすぎた！"),
};

static const u8 sText_Someones[] = _("{JPN}だれかの");
static const u8 sText_Lanettes[] = _("{JPN}マユミの"); //no decapitalize until it is everywhere
static const u8 sText_Bills[] = _("{JPN}マサキの");
static const u8 sText_EnigmaBerry[] = _("{JPN}ナゾのみ"); //no decapitalize until it is everywhere
static const u8 sText_BerrySuffix[] = _("{JPN}のみ"); //no decapitalize until it is everywhere
const u8 gText_EmptyString3[] = _("{JPN}");

static const u8 sText_TwoInGameTrainersDefeated[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}と\n{B_TRAINER2_NAME_WITH_CLASS}に まけた...…");

// New battle strings.
const u8 gText_drastically[] = _("{JPN} ぐぐーんと");
const u8 gText_severely[] = _("{JPN} がくっと");
static const u8 sText_TerrainReturnedToNormal[] = _("{JPN}フィールドが もとに もどった！"); // Unused

const u8 *const gBattleStringsTable[STRINGID_COUNT] =
{
    [STRINGID_TRAINER1LOSETEXT]                     = COMPOUND_STRING("{JPN}{B_TRAINER1_LOSE_TEXT}"),
    [STRINGID_PKMNGAINEDEXP]                        = COMPOUND_STRING("{JPN}{B_BUFF1}は{B_BUFF2}\n{B_BUFF3}の けいけんちを もらった！\p"),
    [STRINGID_PKMNGREWTOLV]                         = COMPOUND_STRING("{JPN}{B_BUFF1}は レベル{B_BUFF2} に あがった！{WAIT_SE}\p"),
    [STRINGID_PKMNLEARNEDMOVE]                      = COMPOUND_STRING("{JPN}{B_BUFF1}は {B_BUFF2}を おぼえた！{WAIT_SE}\p"),
    [STRINGID_TRYTOLEARNMOVE1]                      = COMPOUND_STRING("{JPN}{B_BUFF1}は あたらしく\n{B_BUFF2}を　おぼえたい………!\p"),
    [STRINGID_TRYTOLEARNMOVE2]                      = COMPOUND_STRING("{JPN}しかし {B_BUFF1}は わざを 4つ\nおぼえるので せいいっぱいだ!\p"),
    [STRINGID_TRYTOLEARNMOVE3]                      = COMPOUND_STRING("{JPN}{B_BUFF2}の かわりに\nほかの わざを わすれさせますか?"),
    [STRINGID_PKMNFORGOTMOVE]                       = COMPOUND_STRING("{JPN}{B_BUFF1}は {B_BUFF2}の\nつかいかたを きれいに わすれた!\p"),
    [STRINGID_STOPLEARNINGMOVE]                     = COMPOUND_STRING("{JPN}{PAUSE 32}それでは…… {B_BUFF1}は {B_BUFF2}を\nおぼえるのを あきらめますか？"),
    [STRINGID_DIDNOTLEARNMOVE]                      = COMPOUND_STRING("{JPN}{B_BUFF1}は {B_BUFF2}おぼえずに おわった!\p"),
    [STRINGID_PKMNLEARNEDMOVE2]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}を おぼえた！"),
    [STRINGID_PKMNPROTECTEDITSELF]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は まもりの たいせいに はいった！"),
    [STRINGID_ITDOESNTAFFECT]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}には\nこうかが ない ようだ……"),
    [STRINGID_ITDOESNTAFFECTSCR]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX2}には\nこうかが ない みたいだ……\p"),
    [STRINGID_BATTLERFAINTED]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は たおれた！\p"),
    [STRINGID_PLAYERGOTMONEY]                       = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は しょうきんとして\n{B_BUFF1}円 てにいれた！\p"),
    [STRINGID_PLAYERWHITEOUT]                       = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}の てもとには\nたたかえる ポケモンが いない!\p"),
    [STRINGID_PLAYERWHITEOUT2_WILD]                 = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は あわてて\n{B_BUFF1}円を なくしてしまった……\p"),
    [STRINGID_PLAYERWHITEOUT2_TRAINER]              = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は しょうきんとして\n{B_BUFF1}円 しはらった……\p"),
    [STRINGID_PLAYERWHITEOUT3]                      = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は\nめのまえが まっくらに なった！"),
    [STRINGID_PREVENTSESCAPE]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\nにげられない！"),
    [STRINGID_HITXTIMES]                            = COMPOUND_STRING("{JPN}{B_BUFF1}かい あたった！"), //SV has dynamic plural here
    [STRINGID_PKMNFELLASLEEP]                       = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nねむってしまった！"),
    [STRINGID_PKMNMADESLEEP]                        = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は ねむってしまった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYASLEEP]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は すでに ねむっている！"),
    [STRINGID_PKMNALREADYASLEEP2]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は すでに ねむっている！"),
    [STRINGID_PKMNWASPOISONED]                      = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は どくを あびた！"),
    [STRINGID_PKMNPOISONEDBY]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は どくを あびた！"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTBYPOISON]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は どくの ダメージを うけている！"),
    [STRINGID_PKMNALREADYPOISONED]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は すでに\nどくを あびている！"),
    [STRINGID_PKMNBADLYPOISONED]                    = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nもうどくを あびた！"),
    [STRINGID_PKMNENERGYDRAINED]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}から\nたいりょくを すいとった!"),
    [STRINGID_PKMNWASBURNED]                        = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nやけどを おった！"),
    [STRINGID_PKMNBURNEDBY]                         = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は やけどを おった！"), //not in gen 5+, ability popup
    [STRINGID_PKMNHURTBYBURN]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nやけどの ダメージを うけている！"),
    [STRINGID_PKMNWASFROZEN]                        = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nこおりづけになった!"),
    [STRINGID_PKMNFROZENBY]                         = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}は こおりづけになった!"), //not in gen 5+, ability popup
    [STRINGID_PKMNISFROZEN]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nこおって しまって うごかない!"),
    [STRINGID_PKMNWASDEFROSTED]                     = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\nこおりが とけた!"),
    [STRINGID_PKMNWASDEFROSTEDBY]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の こおりが\n{B_CURRENT_MOVE}で とけた!"),
    [STRINGID_PKMNWASPARALYZED]                     = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は まひして\nわざが でにくくなった!"),
    [STRINGID_PKMNWASPARALYZEDBY]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}で\n{B_EFF_NAME_WITH_PREFIX2}\l\nまひして わざが でにくくなった!"), //not in gen 5+, ability popup
    [STRINGID_PKMNISPARALYZED]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nからだが しびれて うごけない"),
    [STRINGID_PKMNISALREADYPARALYZED]               = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は すでに\nまひしている"),
    [STRINGID_PKMNHEALEDPARALYSIS]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の\nまひが なおった!"),
    [STRINGID_STATSWONTINCREASE]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\n{B_BUFF1}は もう あがらない！"),
    [STRINGID_STATSWONTDECREASE]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\n{B_BUFF1}は もう さがらない！"),
    [STRINGID_PKMNISCONFUSED]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nこんらんしている!"),
    [STRINGID_PKMNHEALEDCONFUSION]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の\nこんらんが とけた!"),
    [STRINGID_PKMNWASCONFUSED]                      = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nこんらんした!"),
    [STRINGID_PKMNALREADYCONFUSED]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nすでに こんらん している"),
    [STRINGID_PKMNFELLINLOVE]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}\nメロメロに なった!"),
    [STRINGID_PKMNINLOVE]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}に メロメロだ!"),
    [STRINGID_PKMNIMMOBILIZEDBYLOVE]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は メロメロで\nわざが だせなかった!"),
    [STRINGID_PKMNCHANGEDTYPE]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}タイプに なった！"),
    [STRINGID_PKMNFLINCHED]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ひるんで\nうごけなかった!"),
    [STRINGID_PKMNREGAINEDHEALTH]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は たいりょくを\nかいふくした!"),
    [STRINGID_PKMNHPFULL]                           = COMPOUND_STRING("{JPN}しかし {B_DEF_NAME_WITH_PREFIX}の\nたいりょくは まんたんだ!"),
    [STRINGID_PKMNRAISEDSPDEF]                      = COMPOUND_STRING("{JPN}ひかりのかべで {B_ATK_TEAM2}で\nとくこうに つよくなった!"),
    [STRINGID_PKMNRAISEDDEF]                        = COMPOUND_STRING("{JPN}リフレクターで {B_ATK_TEAM2}で\nだげきこうげきに つよくなった！"),
    [STRINGID_PKMNAURORAVEIL]                       = COMPOUND_STRING("{JPN}オーロラベールに {B_ATK_TEAM2} ポケモンはつつまれた！"),
    [STRINGID_PKMNCOVEREDBYVEIL]                    = COMPOUND_STRING("{JPN}{B_ATK_TEAM1}ポケモンは\nしんぴの ベールに つつまれた！"),
    [STRINGID_PKMNUSEDSAFEGUARD]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}ポケモンは\nしんぴのまもりに つつまれた！"),
    [STRINGID_PKMNSAFEGUARDEXPIRED]                 = COMPOUND_STRING("{JPN}{B_ATK_TEAM1} ポケモンの つつんでいた\nしんぴの ベールが なくなった"),
    [STRINGID_PKMNWENTTOSLEEP]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}\nねむりはじめた!"), //not in gen 5+
    [STRINGID_PKMNSLEPTHEALTHY]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nけんこうになって ねむりはじめた!"),
    [STRINGID_PKMNWHIPPEDWHIRLWIND]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の まわりで\nくうきが うずを まく!"),
    [STRINGID_PKMNTOOKSUNLIGHT]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nひかりを きゅうしゅうした!"),
    [STRINGID_PKMNLOWEREDHEAD]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nくびを ひっこめた!"),
    [STRINGID_PKMNFLEWHIGH]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nそらたかく とびあがった!"),
    [STRINGID_PKMNDUGHOLE]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nあなをほって ちちゅうに もぐった!\n"),
    [STRINGID_PKMNSQUEEZEDBYBIND]                   = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}に\nしめつけられた!"),
    [STRINGID_PKMNTRAPPEDINVORTEX]                  = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は うずの なかに\nとじこめられた!"),
    [STRINGID_PKMNWRAPPEDBY]                        = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}に\nまきつかれた!"),
    [STRINGID_PKMNCLAMPED]                          = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_EFF_NAME_WITH_PREFIX2}の\nからに はさまれた!\n"),
    [STRINGID_PKMNHURTBY]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}の\nダメージを うけている"),
    [STRINGID_PKMNFREEDFROM]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}から かいほうされた！"),
    [STRINGID_PKMNCRASHED]                          = COMPOUND_STRING("{JPN}いきおい あまって\n{B_ATK_NAME_WITH_PREFIX}は じめんに ぶつかった!"),
    [STRINGID_PKMNSHROUDEDINMIST]                   = gText_PkmnShroudedInMist,
    [STRINGID_PKMNPROTECTEDBYMIST]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は しろいきりに\nまもられている"),
    [STRINGID_PKMNGETTINGPUMPED]                    = gText_PkmnGettingPumped,
    [STRINGID_PKMNHITWITHRECOIL]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は こうげきの\nはんどうを うけた!"),
    [STRINGID_PKMNPROTECTEDITSELF2]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nまもりの たいせいに はいった！"),
    [STRINGID_PKMNBUFFETEDBYSANDSTORM]              = COMPOUND_STRING("{JPN}すなあらしが {B_ATK_NAME_WITH_PREFIX}を\nおそう!"),
    [STRINGID_PKMNPELTEDBYHAIL]                     = COMPOUND_STRING("{JPN}あられが {B_ATK_NAME_WITH_PREFIX}を\nおそう!"),
    [STRINGID_PKMNSEEDED]                           = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}に\nたねを うえつけた！"),
    [STRINGID_PKMNAVOIDEDATTACK]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nこうげきを かわした!"),
    [STRINGID_BATTLERAVOIDEDATTACK]                 = COMPOUND_STRING("{JPN}やどりぎが {B_SCR_NAME_WITH_PREFIX}の\nたいりょくを うばう!"),
    [STRINGID_PKMNSAPPEDBYLEECHSEED]                = COMPOUND_STRING("{JPN}やどりぎが {B_ATK_NAME_WITH_PREFIX}の\nたいりょくを うばう!"),
    [STRINGID_PKMNFASTASLEEP]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}\nぐうぐう ねむっている"),
    [STRINGID_PKMNWOKEUP]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は めを さました！"),
    [STRINGID_PKMNWOKEUPINUPROAR]                   = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX2}は さわがしくて\nめが さめた!"),
    [STRINGID_PKMNCAUSEDUPROAR]                     = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は さわぎだした!"),
    [STRINGID_PKMNMAKINGUPROAR]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は さわいでいる!"),
    [STRINGID_PKMNCALMEDDOWN]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nおとなしくなった"),
    [STRINGID_PKMNSTOCKPILED]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}つたくわえた!"),
    [STRINGID_PKMNCANTSLEEPINUPROAR2]               = COMPOUND_STRING("{JPN}しかし{B_DEF_NAME_WITH_PREFIX2}は\nさわいでいて ねむらない!"),
    [STRINGID_UPROARKEPTPKMNAWAKE]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}は さわがしくて\nめが さめた!"),
    [STRINGID_PKMNSTAYEDAWAKEUSING]                 = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nねむらない！"),
    [STRINGID_PKMNSTORINGENERGY]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は がまんしている"),
    [STRINGID_PKMNUNLEASHEDENERGY]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の\nがまんが とかれた!"),
    [STRINGID_PKMNFATIGUECONFUSION]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nつかれはてて こんらんした!"),
    [STRINGID_PLAYERPICKEDUPMONEY]                  = COMPOUND_STRING("{JPN}{B_BUFF1}円\nひろった!\p"),
    [STRINGID_PKMNUNAFFECTED]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}には\nぜんぜんきいてない!"),
    [STRINGID_PKMNTRANSFORMEDINTO]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}に へんしんした!"),
    [STRINGID_PKMNMADESUBSTITUTE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の\nぶんしんが あらわれた"),
    [STRINGID_PKMNHASSUBSTITUTE]                    = COMPOUND_STRING("{JPN}しかし {B_ATK_NAME_WITH_PREFIX}の\nみがわりは すでに でていた!"),
    [STRINGID_SUBSTITUTEDAMAGED]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}に かわって\nぶんしんが こうげきを うけた!\p"),
    [STRINGID_PKMNSUBSTITUTEFADED]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の ぶんしんは\nきえてしまった……\p"),
    [STRINGID_PKMNMUSTRECHARGE]                     = COMPOUND_STRING("{JPN}こうげきの はんどうで\n{B_ATK_NAME_WITH_PREFIX}は うごけない!"),
    [STRINGID_PKMNRAGEBUILDING]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の いかりの\nボルテージが あがっていく!"),
    [STRINGID_PKMNMOVEWASDISABLED]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の\n{B_BUFF1}を ふうじこめた!"),
    [STRINGID_PKMNMOVEISDISABLED]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は かなしばりで\n{B_CURRENT_MOVE}が だせない！\p"),
    [STRINGID_PKMNMOVEDISABLEDNOMORE]               = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}のかなしばりがとけた！"),
    [STRINGID_PKMNGOTENCORE]                        = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nアンコールを うけた!"),
    [STRINGID_PKMNGOTENCOREDMOVE]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\n{B_CURRENT_MOVE}しか だせない！\p"),
    [STRINGID_PKMNENCOREENDED]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\nアンコールじょうたいが とけた!"),
    [STRINGID_PKMNTOOKAIM]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX2}に\nねらいを さだめた!"),
    [STRINGID_PKMNSKETCHEDMOVE]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}を スケッチした!"),
    [STRINGID_PKMNTRYINGTOTAKEFOE]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は あいてを\nみちづれに しようとしている"),
    [STRINGID_PKMNTOOKFOE]                          = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}を\nみちづれに した!"),
    [STRINGID_PKMNREDUCEDPP]                        = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の\n{B_BUFF1}を {B_BUFF2}けずった!"),
    [STRINGID_PKMNSTOLEITEM]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX2}から\n{B_LAST_ITEM}を うばいとった!"),
    [STRINGID_TARGETCANTESCAPENOW]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nもう にげられない!"),
    [STRINGID_PKMNFELLINTONIGHTMARE]                = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nあくむを みはじめた!"),
    [STRINGID_PKMNLOCKEDINNIGHTMARE]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nあくむに うなされている!"),
    [STRINGID_PKMNLAIDCURSE]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は じぶんの たいりょくを\n\nけずって {B_DEF_NAME_WITH_PREFIX2}に のろいを かけた!"),
    [STRINGID_PKMNAFFLICTEDBYCURSE]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nのろわれている!"),
    [STRINGID_SPIKESSCATTERED]                      = COMPOUND_STRING("{JPN}{B_DEF_TEAM2} あしもとに\nまきびしが ちらばった!"),
    [STRINGID_PKMNHURTBYSPIKES]                     = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は まきびしの\nダメージを うけた!"),
    [STRINGID_PKMNIDENTIFIED]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の\nしょうたいを みやぶった!"),
    [STRINGID_PKMNPERISHCOUNTFELL]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の ほろびの\nカウントが {B_BUFF1}になった!"),
    [STRINGID_PKMNBRACEDITSELF]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は こらえる\nたいせいに はいった!"),
    [STRINGID_PKMNENDUREDHIT]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は こうげきを\nこらえた!"),
    [STRINGID_MAGNITUDESTRENGTH]                    = COMPOUND_STRING("{JPN}マグニチュード{B_BUFF1}！！"),
    [STRINGID_PKMNCUTHPMAXEDATTACK]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は たいりょくを けずって\nパワーぜんかいに なった!"),
    [STRINGID_PKMNCOPIEDSTATCHANGES]                = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は たいりょくを けずって\nパワーぜんかいに なった!"),
    [STRINGID_PKMNGOTFREE]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}から かいほうされた!"),
    [STRINGID_PKMNSHEDLEECHSEED]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nやどりぎのタネを ふきとばした!"),
    [STRINGID_PKMNBLEWAWAYSPIKES]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は まきびしを\nふきとばした!"), // Not in gen 5+
    [STRINGID_PKMNFLEDFROMBATTLE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は せんとうから\nりだつした!"),
    [STRINGID_PKMNFORESAWATTACK]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は みらいに\nこうげきを よちした!"),
    [STRINGID_PKMNTOOKATTACK]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_BUFF1}の\nこうげきを うけた!"),
    [STRINGID_PKMNATTACK]                           = COMPOUND_STRING("{JPN}{B_BUFF1}の こうげき!"), // Not in gen 5+
    [STRINGID_PKMNCENTERATTENTION]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nちゅうもくの まとに なった!"),
    [STRINGID_PKMNCHARGINGPOWER]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nじゅうでんを はじめた!"),
    [STRINGID_NATUREPOWERTURNEDINTO]                = COMPOUND_STRING("{JPN}しぜんのちからは\n{B_CURRENT_MOVE}に なった!"),
    [STRINGID_PKMNSTATUSNORMAL]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の じょうたいいじょうが\nなおった!"),
    [STRINGID_PKMNHASNOMOVESLEFT]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は だすことの できる\nわざが ない!\p"),
    [STRINGID_PKMNSUBJECTEDTOTORMENT]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nいちゃもんを つけられた!"),
    [STRINGID_PKMNCANTUSEMOVETORMENT]               = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は いちゃもんを つけられたので\nつづけて おなじ わざが　だせない!\p"),
    [STRINGID_PKMNTIGHTENINGFOCUS]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_ATK_NAME_WITH_PREFIX}に\nねらいを さだめた!"),
    [STRINGID_PKMNFELLFORTAUNT]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は \nちょうはつに のってしまった!"),
    [STRINGID_PKMNCANTUSEMOVETAUNT]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ちょうはつ されて\n{B_CURRENT_MOVE}が だせない!\p"),
    [STRINGID_PKMNREADYTOHELP]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX2}を\nてだすけ する たいせいに はいった!"),
    [STRINGID_PKMNSWITCHEDITEMS]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は おたがいの\nどうぐを いれかえた!"),
    [STRINGID_PKMNCOPIEDFOE]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX2}の\nとくせいを コピーした！"),
    [STRINGID_PKMNWISHCAMETRUE]                     = COMPOUND_STRING("{JPN}{B_BUFF1}の\nねがいごとが かなった!"),
    [STRINGID_PKMNPLANTEDROOTS]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ねを はった!"),
    [STRINGID_PKMNABSORBEDNUTRIENTS]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ねから\nようぶんを すいとった!"),
    [STRINGID_PKMNANCHOREDITSELF]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は ねを はって\nうごかない!"),
    [STRINGID_PKMNWASMADEDROWSY]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX}の\nねむけを さそった!"),
    [STRINGID_PKMNKNOCKEDOFF]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX}の\n {B_LAST_ITEM}を はたきおとした!"),
    [STRINGID_PKMNSWAPPEDABILITIES]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は おたがいの\nとくせいを いれかえた!"),
    [STRINGID_PKMNSEALEDOPPONENTMOVE]               = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は あいての\nわざを ふういんした!"),
    [STRINGID_PKMNCANTUSEMOVESEALED]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ふういんで\n{B_CURRENT_MOVE}が だせない!\p"),
    [STRINGID_PKMNWANTSGRUDGE]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は あいてに\nおんねんを かけようと している!"),
    [STRINGID_PKMNLOSTPPGRUDGE]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_BUFF1}は\nおんねんで わざポイントが 0に なった!"),
    [STRINGID_PKMNSHROUDEDITSELF]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_CURRENT_MOVE}に つつまれた!"),
    [STRINGID_PKMNMOVEBOUNCED]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_CURRENT_MOVE}は\nはねかえされた！"),
    [STRINGID_PKMNWAITSFORTARGET]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nあいての でかたを うかがっている!"),
    [STRINGID_PKMNSNATCHEDMOVE]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}の\nわざを よこどりした!"),
    [STRINGID_PKMNMADEITRAIN]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\nあめが ふりはじめた!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPROTECTEDBY]                      = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_DEF_ABILITY}で\nきかなかった!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSUSAGE]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_DEF_ABILITY}で\n{B_CURRENT_MOVE}がつかえない！"), //not in gen 5+, ability popup
    [STRINGID_PKMNRESTOREDHPUSING]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_SCR_ABILITY}で\nかいふくした！"),
    [STRINGID_PKMNCHANGEDTYPEWITH]                  = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_EFF_ABILITY}で\n{B_BUFF1}タイプに なった!"),
    [STRINGID_PKMNPREVENTSROMANCEWITH]              = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_DEF_ABILITY}で\nメロメロに ならない!"), //not in gen 5+, ability popup
    [STRINGID_PKMNPREVENTSCONFUSIONWITH]            = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_SCR_ABILITY}で\nこんらんしない!"),
    [STRINGID_PKMNRAISEDFIREPOWERWITH]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_SCR_ABILITY}で\nほのおの いりょくが あがった!"),
    [STRINGID_PKMNANCHORSITSELFWITH]                = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_DEF_ABILITY}で\nはりついている!"),
    [STRINGID_PKMNPREVENTSSTATLOSSWITH]             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_SCR_ABILITY}で\nのうりょくが さがらない!"),
    [STRINGID_PKMNHURTSWITH]                        = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}の {B_BUFF1}で\n{B_ATK_NAME_WITH_PREFIX}は 　きずついた!"),
    [STRINGID_PKMNTRACED]                           = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_BUFF1}の\n{B_BUFF2}を トレースした!"),
    [STRINGID_STATSHARPLY]                          = gText_StatSharply,
    [STRINGID_STATHARSHLY]                          = COMPOUND_STRING("{JPN} がくっと"),
    [STRINGID_STATROSE]                             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\n{B_BUFF1}が{B_BUFF2} あがった！"),
    [STRINGID_STATFELL]                             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\n{B_BUFF1}が{B_BUFF2} さがった！"),
    [STRINGID_CRITICALHIT]                          = COMPOUND_STRING("{JPN}きゅうしょに あたった！"),
    [STRINGID_ONEHITKO]                             = COMPOUND_STRING("{JPN}いちげき ひっさつ！"),
    [STRINGID_123POOF]                              = COMPOUND_STRING("{JPN}1 {PAUSE 10}2の {PAUSE 10}…{PAUSE 10}…{PAUSE 20}… {PLAY_SE SE_BALL_BOUNCE_1}ポカン！\p"),
    [STRINGID_ANDELLIPSIS]                          = COMPOUND_STRING("{JPN}そして……!\p"),
    [STRINGID_NOTVERYEFFECTIVE]                     = COMPOUND_STRING("{JPN}こうかは いまひとつの ようだ"),
    [STRINGID_SUPEREFFECTIVE]                       = COMPOUND_STRING("{JPN}こうかは ばつぐんだ！"),
    [STRINGID_GOTAWAYSAFELY]                        = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED]                         = COMPOUND_STRING("{JPN}{PLAY_SE SE_FLEE}やせいの {B_BUFF1}は にげだした！"),
    [STRINGID_NORUNNINGFROMTRAINERS]                = COMPOUND_STRING("{JPN}ダメだ! しょうぶの さいちゅうに\nあいてに せなかは みせられない！\p"),
    [STRINGID_CANTESCAPE]                           = COMPOUND_STRING("{JPN}にげることが できない！\p"),
    [STRINGID_DONTLEAVEBIRCH]                       = COMPOUND_STRING("{JPN}オダマキ『み みすてないでくれー!』\p"), //no decapitalize until it is everywhere
    [STRINGID_BUTNOTHINGHAPPENED]                   = COMPOUND_STRING("{JPN}しかし なにも おこらなかった！"),
    [STRINGID_BUTITFAILED]                          = COMPOUND_STRING("{JPN}しかし うまく きまらなかった！"),
    [STRINGID_ITHURTCONFUSION]                      = COMPOUND_STRING("{JPN}わけも わからず\nじぶんを こうげきした!"),
    [STRINGID_STARTEDTORAIN]                        = COMPOUND_STRING("{JPN}あめが ふりはじめた!"),
    [STRINGID_DOWNPOURSTARTED]                      = COMPOUND_STRING("{JPN}おおあめに なった!"), // corresponds to DownpourText in pokegold and pokecrystal and is used by Rain Dance in GSC
    [STRINGID_RAINCONTINUES]                        = COMPOUND_STRING("{JPN}あめが ふりつづいている"), //not in gen 5+
    [STRINGID_DOWNPOURCONTINUES]                    = COMPOUND_STRING("{JPN}おおあめが ふりつづいている"), // unused
    [STRINGID_RAINSTOPPED]                          = COMPOUND_STRING("{JPN}あめが ふりやんだ!"),
    [STRINGID_SANDSTORMBREWED]                      = COMPOUND_STRING("{JPN}すなあらしが ふきはじめた!"),
    [STRINGID_SANDSTORMRAGES]                       = COMPOUND_STRING("{JPN}すなあらしが ふきあれる"),
    [STRINGID_SANDSTORMSUBSIDED]                    = COMPOUND_STRING("{JPN}すなあらしが おさまった!"),
    [STRINGID_SUNLIGHTGOTBRIGHT]                    = COMPOUND_STRING("{JPN}ひざしが つよくなった!"),
    [STRINGID_SUNLIGHTSTRONG]                       = COMPOUND_STRING("{JPN}ひざしが つよい"), //not in gen 5+
    [STRINGID_SUNLIGHTFADED]                        = COMPOUND_STRING("{JPN}ひざしが よわくなった!"),
    [STRINGID_STARTEDHAIL]                          = COMPOUND_STRING("{JPN}あられが ふりはじめた!"),
    [STRINGID_HAILCONTINUES]                        = COMPOUND_STRING("{JPN}あられが ふりつづいている"),
    [STRINGID_HAILSTOPPED]                          = COMPOUND_STRING("{JPN}あられが ふりやんだ!"),
    [STRINGID_STATCHANGESGONE]                      = COMPOUND_STRING("{JPN}すべての ステータスが\nもとに もどった!"),
    [STRINGID_COINSSCATTERED]                       = COMPOUND_STRING("{JPN}こばんが あたりに ちらばった!"),
    [STRINGID_TOOWEAKFORSUBSTITUTE]                 = COMPOUND_STRING("{JPN}しかし ぶんしんを だすには\nたいりょくが たりなかった!"),
    [STRINGID_SHAREDPAIN]                           = COMPOUND_STRING("{JPN}おたがいの たいりょくを\nわかちあった!"),
    [STRINGID_BELLCHIMED]                           = COMPOUND_STRING("{JPN}すずのねが ひびきわたった!"),
    [STRINGID_FAINTINTHREE]                         = COMPOUND_STRING("{JPN}おたがいのポケモンは\n3ターンごに ほろびてしまう!"),
    [STRINGID_NOPPLEFT]                             = COMPOUND_STRING("{JPN}わざの のこりポイントが ない!\p"), //not in gen 5+
    [STRINGID_BUTNOPPLEFT]                          = COMPOUND_STRING("{JPN}しかし\nわざの のこりポイントが なかった!"),
    [STRINGID_PLAYERUSEDITEM]                       = COMPOUND_STRING("{JPN}{B_LAST_ITEM}を つかった！"),
    [STRINGID_WALLYUSEDITEM]                        = COMPOUND_STRING("{JPN}ミツルは\n{B_LAST_ITEM}を つかった！"), //no decapitalize until it is everywhere
    [STRINGID_TRAINERBLOCKEDBALL]                   = COMPOUND_STRING("{JPN}トレーナーに ボールを はじかれた!"),
    [STRINGID_DONTBEATHIEF]                         = COMPOUND_STRING("{JPN}ひとの ものを とったら どろぼう!"),
    [STRINGID_ITDODGEDBALL]                         = COMPOUND_STRING("{JPN}ポケモンに\nうまく あたらなかった!"),
    [STRINGID_PKMNBROKEFREE]                        = COMPOUND_STRING("{JPN}だめだ! ポケモンが\nボールから でてしまった!"),
    [STRINGID_ITAPPEAREDCAUGHT]                     = COMPOUND_STRING("{JPN}ああ!\nつかまえたと おもったのに!"),
    [STRINGID_AARGHALMOSTHADIT]                     = COMPOUND_STRING("{JPN}ざんねん!\nもうすこしで つかまえられたのに!"),
    [STRINGID_SHOOTSOCLOSE]                         = COMPOUND_STRING("{JPN}おしい!\nあと ちょっとの ところだったのに!"),
    [STRINGID_GOTCHAPKMNCAUGHTPLAYER]               = COMPOUND_STRING("{JPN}やったー!\n{B_DEF_NAME}を つかまえたぞ!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p"),
    [STRINGID_GOTCHAPKMNCAUGHTWALLY]                = COMPOUND_STRING("{JPN}やったー!\n{B_DEF_NAME}を つかまえたぞ!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}"),
    [STRINGID_GIVENICKNAMECAPTURED]                 = COMPOUND_STRING("{JPN}つかまえた {B_DEF_NAME}に\nニックネームを つけますか?"),
    [STRINGID_PKMNDATAADDEDTODEX]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME}の データが あたらしく\nポケモンずかんに セーブされます!\p"),
    [STRINGID_ITISRAINING]                          = COMPOUND_STRING("{JPN}あめが ふっている"),
    [STRINGID_SANDSTORMISRAGING]                    = COMPOUND_STRING("{JPN}すなあらしが ふきあれている"),
    [STRINGID_CANTESCAPE2]                          = COMPOUND_STRING("{JPN}にげられない！\p"),
    [STRINGID_PKMNIGNORESASLEEP]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ねむったまま\nめいれいを むしした!"),
    [STRINGID_PKMNIGNOREDORDERS]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は めいれいを むしした!"),
    [STRINGID_PKMNBEGANTONAP]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ひるねを はじめた!"),
    [STRINGID_PKMNLOAFING]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は なまけている!"),
    [STRINGID_PKMNWONTOBEY]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は いうことを きかない!"),
    [STRINGID_PKMNTURNEDAWAY]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は そっぽを むいた!"),
    [STRINGID_PKMNPRETENDNOTNOTICE]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しらんぷりした!"),
    [STRINGID_ENEMYABOUTTOSWITCHPKMN]               = COMPOUND_STRING("{JPN}{B_TRAINER1_NAME_WITH_CLASS}は {B_BUFF2}を くりだそうと している\p{B_PLAYER_NAME}も ポケモンを\nいれかえますか?"),
    [STRINGID_CREPTCLOSER]                          = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は\n{B_OPPONENT_MON1_NAME}に ちかづいた!"), //safari
    [STRINGID_CANTGETCLOSER]                        = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は\nこれいじょう ちかづけない!"), //safari
    [STRINGID_PKMNWATCHINGCAREFULLY]                = COMPOUND_STRING("{JPN}{B_OPPONENT_MON1_NAME}は\nこちらの ようすを うかがっている!"), //safari
    [STRINGID_PKMNCURIOUSABOUTX]                    = COMPOUND_STRING("{JPN}{B_OPPONENT_MON1_NAME}は {B_BUFF1}に\nきょうみを しめしている ようだ!"), //safari
    [STRINGID_PKMNENTHRALLEDBYX]                    = COMPOUND_STRING("{JPN}{B_OPPONENT_MON1_NAME}は {B_BUFF1}に\nむちゅうに なっている ようだ!"), //safari
    [STRINGID_PKMNIGNOREDX]                         = COMPOUND_STRING("{JPN}{B_OPPONENT_MON1_NAME}は {B_BUFF1}に\nみむきも しない ようだ!"), //safari
    [STRINGID_THREWPOKEBLOCKATPKMN]                 = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は {B_OPPONENT_MON1_NAME}に ポロックを なげた!"), //safari
    [STRINGID_OUTOFSAFARIBALLS]                     = COMPOUND_STRING("{JPN}{PLAY_SE SE_DING_DONG}アナウンス『ピンポーン! サファリボールが\nなくなったので しゅうりょうでーす!\p"), //safari
    [STRINGID_PKMNSITEMCUREDPARALYSIS]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nまひが なおった!"),
    [STRINGID_PKMNSITEMCUREDPOISON]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nどくが なおった!"),
    [STRINGID_PKMNSITEMHEALEDBURN]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nやけどが なおった!"),
    [STRINGID_PKMNSITEMDEFROSTEDIT]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nこおりじょうたいが なおった!"),
    [STRINGID_PKMNSITEMWOKEIT]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nねむりから さめた!"),
    [STRINGID_PKMNSITEMSNAPPEDOUT]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nこんらんが なおった!"),
    [STRINGID_PKMNSITEMCUREDPROBLEM]                = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}\nで{B_BUFF1}じょうたいが なおった!"), // Not in Gen 5+
    [STRINGID_PKMNSITEMRESTOREDHEALTH]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nたいりょくを かいふくした!"),
    [STRINGID_PKMNSITEMRESTOREDPP]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\nわざポイントを {B_BUFF1} かいふくした！"),
    [STRINGID_PKMNSITEMRESTOREDSTATUS]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}\nステータスを もとに もどした!"),
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE]           = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nすこし かいふく"),
    [STRINGID_ITEMALLOWSONLYYMOVE]                  = COMPOUND_STRING("{JPN}{B_LAST_ITEM}の こうかで\n{B_CURRENT_MOVE}しか だすことができない!\p"),
    [STRINGID_PKMNHUNGONWITHX]                      = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nもちこたえた!"),
    [STRINGID_EMPTYSTRING3]                         = gText_EmptyString3,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2]             = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は たいりょくを\nかいふくした!"),
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM]             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nすなあらしを おこした！"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXPREVENTSYLOSS]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の\n{B_BUFF1}は さがらない！ "),
    [STRINGID_PKMNSXINFATUATEDY]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nメロメロに なった!"),
    [STRINGID_PKMNSXMADEYINEFFECTIVE]               = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}でどうぐをはずせなち！"),
    [STRINGID_ITSUCKEDLIQUIDOOZE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nヘドロえきを すいとった!"),
    [STRINGID_PKMNTRANSFORMED]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の すがたが\nへんかした!"),
    [STRINGID_ELECTRICITYWEAKENED]                  = COMPOUND_STRING("{JPN}でんきの いりょくが よわまった!"),
    [STRINGID_FIREWEAKENED]                         = COMPOUND_STRING("{JPN}ほのおの いりょくが よわまった!"),
    [STRINGID_PKMNHIDUNDERWATER]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nすいちゅうに みをひそめた!"),
    [STRINGID_PKMNSPRANGUP]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nとびはねた!"),
    [STRINGID_HMMOVESCANTBEFORGOTTEN]               = COMPOUND_STRING("{JPN}それは たいせつな わざです\nわすれさせることは できません!\p"),
    [STRINGID_XFOUNDONEY]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_LAST_ITEM}を みつけた！"),
    [STRINGID_PLAYERDEFEATEDTRAINER1]               = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA]                        = COMPOUND_STRING("{JPN}ここちよい かおりが ひろがった!"),
    [STRINGID_ITEMSCANTBEUSEDNOW]                   = COMPOUND_STRING("{JPN}つかっても こうかがないよ{PAUSE 64}"), // Not present in Gen 5+
    [STRINGID_USINGITEMSTATOFPKMNROSE]              = COMPOUND_STRING("{JPN}{B_LAST_ITEM}で {B_SCR_NAME_WITH_PREFIX2}の\n{B_BUFF1}が {B_BUFF2} あがった！"),
    [STRINGID_USINGITEMSTATOFPKMNFELL]              = COMPOUND_STRING("{JPN}{B_LAST_ITEM}で {B_SCR_NAME_WITH_PREFIX2}の\n{B_BUFF1}が {B_BUFF2} さがった！"), // This string does not exist in Gen 5+. Used to print more info that's otherwise obscured such as using Room Service
    [STRINGID_PKMNUSEDXTOGETPUMPED]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}を つかって\nはりきり だした!"),
    [STRINGID_PKMNSXMADEYUSELESS]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\n{B_CURRENT_MOVE}は きかない！"), //not in gen 5+, ability popup
    [STRINGID_PKMNTRAPPEDBYSANDTOMB]                = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は すなじごくに\nとらわれた!\n"),
    [STRINGID_EMPTYSTRING4]                         = COMPOUND_STRING("{JPN}"),
    [STRINGID_ABOOSTED]                             = COMPOUND_STRING("{JPN} たくさんの"),
    [STRINGID_PKMNSXINTENSIFIEDSUN]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\nひざしが つよくなった!"), //not in gen 5+, ability popup
    [STRINGID_YOUTHROWABALLNOWRIGHT]                = COMPOUND_STRING("{JPN}ここで ボールを なげるんだね\nぼく…… やってみるよ!\n"),
    [STRINGID_PKMNSXTOOKATTACK]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_BUFF1}の\nこうげきを うけた！"),
    [STRINGID_PKMNCHOSEXASDESTINY]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_CURRENT_MOVE}を\nみらいに たくした!"),
    [STRINGID_PKMNLOSTFOCUS]                        = COMPOUND_STRING("{JPN}しゅうちゅうりょくが\nとぎれて わざが だせなかった!"),
    [STRINGID_USENEXTPKMN]                          = COMPOUND_STRING("{JPN}つぎの ポケモンを つかいますか?"),
    [STRINGID_PKMNFLEDUSINGITS]                     = COMPOUND_STRING("{JPN}{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX}は\n{B_LAST_ITEM}を つかって にげた\p"),
    [STRINGID_PKMNFLEDUSING]                        = COMPOUND_STRING("{JPN}{PLAY_SE SE_FLEE}{B_ATK_ABILITY}を つかって にげた\p"), //not in gen 5+
    [STRINGID_PKMNWASDRAGGEDOUT]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は せんとうに\nひきずりだされた!\p"),
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS]            = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nじょうたいいじょうが なおった!"), // Not in Gen 5+
    [STRINGID_TRAINER1USEDITEM]                     = COMPOUND_STRING("{JPN}{B_ATK_TRAINER_NAME_WITH_CLASS}は\n{B_LAST_ITEM}をつかった！"),
    [STRINGID_BOXISFULL]                            = COMPOUND_STRING("{JPN}ボックスが いっぱいで\nこれいじょう つかまえられない!\p"),
    [STRINGID_PKMNSXMADEITINEFFECTIVE]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX} はこうかをうしなった！"),
    [STRINGID_PKMNSXPREVENTSFLINCHING]              = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_EFF_ABILITY}で\nひるまない!"), //not in gen 5+, ability popup
    [STRINGID_PKMNALREADYHASBURN]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nやけどをおった!"),
    [STRINGID_PKMNSXBLOCKSY]                        = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\n{B_CURRENT_MOVE}をふせいだ！"), //not in gen 5+, ability popup
    [STRINGID_PKMNSXWOREOFF]                        = COMPOUND_STRING("{JPN}{B_ATK_TEAM1}の{B_BUFF1}の\nこうかが きれた!"),
    [STRINGID_THEWALLSHATTERED]                     = COMPOUND_STRING("{JPN}かべが こわれた!"), //not in gen5+, uses "your teams light screen wore off!" etc instead
    [STRINGID_PKMNSXCUREDITSYPROBLEM]               = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_SCR_ABILITY}で\n{B_BUFF1}じょうたいが なおった!"), //not in gen 5+, ability popup
    [STRINGID_ATTACKERCANTESCAPE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nもう にげられない!"),
    [STRINGID_PKMNOBTAINEDX]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1} うけとった"),
    [STRINGID_PKMNOBTAINEDX2]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_BUFF2}を うけとった"),
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}を うけとった\n{B_DEF_NAME_WITH_PREFIX}は {B_BUFF2}を うけとった"),
    [STRINGID_BUTNOEFFECT]                          = COMPOUND_STRING("{JPN}しかし こうかが なかった!"),
    [STRINGID_TWOENEMIESDEFEATED]                   = sText_TwoInGameTrainersDefeated,
    [STRINGID_TRAINER2LOSETEXT]                     = COMPOUND_STRING("{JPN}{B_TRAINER2_LOSE_TEXT}"),
    [STRINGID_PKMNINCAPABLEOFPOWER]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nじぶんの ちからを はっき できない ようだ!"),
    [STRINGID_GLINTAPPEARSINEYE]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX2}の めの いろが かわっていく!"),
    [STRINGID_PKMNGETTINGINTOPOSITION]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は たいせいを たてなおしている!"),
    [STRINGID_PKMNBEGANGROWLINGDEEPLY]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ひくく うなり はじめた!"),
    [STRINGID_PKMNEAGERFORMORE]                     = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は まだまだ やるきだ!"),
    [STRINGID_DEFEATEDOPPONENTBYREFEREE]            = COMPOUND_STRING("{JPN}{B_PLAYER_MON1_NAME}は はんていで\nあいての {B_OPPONENT_MON1_NAME}に かった！"),
    [STRINGID_LOSTTOOPPONENTBYREFEREE]              = COMPOUND_STRING("{JPN}{B_PLAYER_MON1_NAME}は はんていで\nあいての {B_OPPONENT_MON1_NAME}に まけた！"),
    [STRINGID_TIEDOPPONENTBYREFEREE]                = COMPOUND_STRING("{JPN}{B_PLAYER_MON1_NAME}は はんていで\nあいての {B_OPPONENT_MON1_NAME}と ひきまわけた！"),
    [STRINGID_QUESTIONFORFEITMATCH]                 = COMPOUND_STRING("{JPN}こうさんしますか？"),
    [STRINGID_FORFEITEDMATCH]                       = COMPOUND_STRING("{JPN}こうさんしました……"),
    [STRINGID_PKMNTRANSFERREDSOMEONESPC]            = gText_PkmnTransferredSomeonesPC,
    [STRINGID_PKMNTRANSFERREDLANETTESPC]            = gText_PkmnTransferredLanettesPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL]                = gText_PkmnTransferredSomeonesPCBoxFull,
    [STRINGID_PKMNBOXLANETTESPCFULL]                = gText_PkmnTransferredLanettesPCBoxFull,
    [STRINGID_TRAINER1WINTEXT]                      = COMPOUND_STRING("{JPN}{B_TRAINER1_WIN_TEXT}"),
    [STRINGID_TRAINER2WINTEXT]                      = COMPOUND_STRING("{JPN}{B_TRAINER2_WIN_TEXT}"),
    [STRINGID_ENDUREDSTURDY]                        = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は{B_DEF_ABILITY}でたえた！"),
    [STRINGID_POWERHERB]                            = COMPOUND_STRING("{JPN}{B_LAST_ITEM}でちからがみなぎった！"),
    [STRINGID_HURTBYITEM]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は{B_LAST_ITEM}できずついた！"),
    [STRINGID_GRAVITYINTENSIFIED]                   = COMPOUND_STRING("{JPN}じゅうりょくがつよくなった！"),
    [STRINGID_TARGETWOKEUP]                         = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}はめをさました！"),
    [STRINGID_TAILWINDBLEW]                         = COMPOUND_STRING("{JPN}おいかぜが{B_ATK_TEAM2}にふいた！"),
    [STRINGID_PKMNWENTBACK]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は{B_ATK_TRAINER_NAME}のもとへもどる！"),
    [STRINGID_PKMNCANTUSEITEMSANYMORE]              = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX} はどうぐをさしおさえられた！"),
    [STRINGID_PKMNFLUNG]                            = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX} は {B_LAST_ITEM}をなげつけた！"),
    [STRINGID_PKMNPREVENTEDFROMHEALING]             = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}はかいふくふうじだ！"),
    [STRINGID_PKMNSWITCHEDATKANDDEF]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}はこうげきとぼうぎょをいれかえた！"),
    [STRINGID_PKMNSABILITYSUPPRESSED]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}のとくせいがきえた！"),
    [STRINGID_SHIELDEDFROMCRITICALHITS]             = COMPOUND_STRING("{JPN}おまじないにより　 {B_ATK_TEAM2}\nポケモン　は　きゅうしょに　あたらない！"),
    [STRINGID_PKMNACQUIREDABILITY]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX} は {B_DEF_ABILITY}になった！"),
    [STRINGID_POISONSPIKESSCATTERED]                = COMPOUND_STRING("{JPN}どくびしが　 {B_DEF_TEAM2} ばに　ちらばった！"),
    [STRINGID_PKMNSWITCHEDSTATCHANGES]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} はあいてと\nのうりょくへんかを　いれかえた！"),
    [STRINGID_PKMNSURROUNDEDWITHVEILOFWATER]        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　みずのベールを　まとった！"),
    [STRINGID_PKMNLEVITATEDONELECTROMAGNETISM]      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} はでんじふゆうで　ういた！"),
    [STRINGID_PKMNTWISTEDDIMENSIONS]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　じくうをゆがめた！"),
    [STRINGID_POINTEDSTONESFLOAT]                   = COMPOUND_STRING("{JPN}{B_DEF_TEAM2} ばに　いわが　ただよった！"),
    [STRINGID_TRAPPEDBYSWIRLINGMAGMA]               = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX} は　マグマのうずに　とじこめられた！"),
    [STRINGID_VANISHEDINSTANTLY]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　いっしゅんで　きえた！"),
    [STRINGID_PROTECTEDTEAM]                        = COMPOUND_STRING("{JPN}{B_CURRENT_MOVE} で {B_ATK_TEAM2}\nポケモンは　まもられた！"),
    [STRINGID_SHAREDITSGUARD]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　あいてと　まもりを　わけあった！"),
    [STRINGID_SHAREDITSPOWER]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　あいてと　ちからを　わけあった！"),
    [STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON]         = COMPOUND_STRING("{JPN}ふしぎなくうかんが　あたりをつつみこんだ！"),
    [STRINGID_BECAMENIMBLE]                         = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX} は　みがるに　なった！"),
    [STRINGID_HURLEDINTOTHEAIR]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX} は　そらたかく　つれさられた！"),
    [STRINGID_HELDITEMSLOSEEFFECTS]                 = COMPOUND_STRING("{JPN}ふしぎなくうかんで　もちもののこうかが　きえた！"),
    [STRINGID_FELLSTRAIGHTDOWN]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}　は　じめんにおちた！"),
    [STRINGID_TARGETCHANGEDTYPE]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_BUFF1}タイプに なった！"),
    [STRINGID_KINDOFFER]                            = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX} は　そのとおりに　した！"),
    [STRINGID_RESETSTARGETSSTATLEVELS]              = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}　の　ステータスへんか　がなくなった！"),
    [STRINGID_ALLYSWITCHPOSITION]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は {B_SCR_NAME_WITH_PREFIX2} といれかわった！"),
    [STRINGID_REFLECTTARGETSTYPE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　{B_DEF_NAME_WITH_PREFIX2}\nとおなじタイプに　なった！"),
    [STRINGID_EMBARGOENDS]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX} は　またどうぐが　つかえる！"),
    [STRINGID_ELECTROMAGNETISM]                     = COMPOUND_STRING("{JPN}でんじふゆう"),
    [STRINGID_BUFFERENDS]                           = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}の\nこうかが きれた！"),
    [STRINGID_TELEKINESISENDS]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は テレキネシスから\nかいほうされた！"),
    [STRINGID_TAILWINDENDS]                         = COMPOUND_STRING("{JPN}{B_ATK_TEAM1} ポケモンの　おいかぜが　やんだ！"),
    [STRINGID_LUCKYCHANTENDS]                       = COMPOUND_STRING("{JPN}{B_ATK_TEAM1} ポケモンの　おまじないが　きえた！"),
    [STRINGID_TRICKROOMENDS]                        = COMPOUND_STRING("{JPN}ゆがんだ じくうが もとに もどった！"),
    [STRINGID_WONDERROOMENDS]                       = COMPOUND_STRING("{JPN}ワンダールームの こうかが きれた！"),
    [STRINGID_MAGICROOMENDS]                        = COMPOUND_STRING("{JPN}マジックルームの こうかが きれた！"),
    [STRINGID_MUDSPORTENDS]                         = COMPOUND_STRING("{JPN}どろあそびの こうかが きれた！"),
    [STRINGID_WATERSPORTENDS]                       = COMPOUND_STRING("{JPN}みずあそびの こうかが きれた！"),
    [STRINGID_GRAVITYENDS]                          = COMPOUND_STRING("{JPN}じゅうりょくが もとに もどった！"),
    [STRINGID_AQUARINGHEAL]                         = COMPOUND_STRING("{JPN}みずのベールで {B_ATK_NAME_WITH_PREFIX2}の\nHPが かいふくした！"),
    [STRINGID_ELECTRICTERRAINENDS]                  = COMPOUND_STRING("{JPN}エレキフィールドが きえた！"),
    [STRINGID_MISTYTERRAINENDS]                     = COMPOUND_STRING("{JPN}ミストフィールドが きえた！"),
    [STRINGID_PSYCHICTERRAINENDS]                   = COMPOUND_STRING("{JPN}サイコフィールドが きえた！"),
    [STRINGID_GRASSYTERRAINENDS]                    = COMPOUND_STRING("{JPN}グラスフィールドが きえた！"),
    [STRINGID_TARGETABILITYSTATRAISE]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_DEF_ABILITY}で\n{B_BUFF1}が{B_BUFF2} あがった！"), // Not in Gen 5+
    [STRINGID_STATWASMAXEDOUT]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}が\nさいだいまで あがった！"),
    [STRINGID_ATTACKERABILITYSTATRAISE]             = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_ATK_ABILITY}で\n{B_BUFF1}が{B_BUFF2} あがった！"), // Not in Gen 5+
    [STRINGID_POISONHEALHPUP]                       = COMPOUND_STRING("{JPN}どくによって {B_ATK_NAME_WITH_PREFIX2}の\nHPが すこし かいふくした！"), // Not in Gen 5+
    [STRINGID_BADDREAMSDMG]                         = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は あくむに うなされている！"),
    [STRINGID_MOLDBREAKERENTERS]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は かたやぶりだ！"),
    [STRINGID_TERAVOLTENTERS]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は はじける オーラを\nはなっている！"),
    [STRINGID_TURBOBLAZEENTERS]                     = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は もえさかる オーラを\nはなっている！"),
    [STRINGID_SLOWSTARTENTERS]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は スロースタートだ！"),
    [STRINGID_SLOWSTARTEND]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ちょうしを とりもどした！"),
    [STRINGID_SOLARPOWERHPDROP]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_ATK_ABILITY}で\nHPを けずられた！"), // Not in Gen 5+
    [STRINGID_AFTERMATHDMG]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ダメージを うけた！"),
    [STRINGID_ANTICIPATIONACTIVATES]                = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は みぶるいした！"),
    [STRINGID_FOREWARNACTIVATES]                    = COMPOUND_STRING("{JPN}{B_SCR_ABILITY}で\n{B_EFF_NAME_WITH_PREFIX2}の {B_BUFF1}を よちした！"),
    [STRINGID_ICEBODYHPGAIN]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_ATK_ABILITY}で\nHPが すこし かいふくした！"), // Not in Gen 5+
    [STRINGID_SNOWWARNINGHAIL]                      = COMPOUND_STRING("{JPN}あられが ふりはじめた！"),
    [STRINGID_FRISKACTIVATES]                       = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\n{B_SCR_NAME_WITH_PREFIX2}の {B_LAST_ITEM}を おみとおしした！"),
    [STRINGID_UNNERVEENTERS]                        = COMPOUND_STRING("{JPN} きんちょうして きのみを たべられない！"),
    [STRINGID_HARVESTBERRY]                         = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_LAST_ITEM}を\nしゅうかくした！"),
    [STRINGID_PROTEANTYPECHANGE]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_ATK_ABILITY}で\n{B_BUFF1}タイプに なった！"),
    [STRINGID_SYMBIOSISITEMPASS]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}を\n{B_EFF_NAME_WITH_PREFIX2}に わたした！"),
    [STRINGID_STEALTHROCKDMG]                       = COMPOUND_STRING("{JPN}いわが {B_SCR_NAME_WITH_PREFIX2}に くいこんだ！"),
    [STRINGID_TOXICSPIKESABSORBED]                  = COMPOUND_STRING("{JPN}{B_EFF_TEAM2} どくびしが きえた！"),
    [STRINGID_TOXICSPIKESPOISONED]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は どくを あびた！"),
    [STRINGID_TOXICSPIKESBADLYPOISONED]             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は もうどくを あびた！"),
    [STRINGID_STICKYWEBSWITCHIN]                    = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は ねばねばネットに\nからめとられた！"),
    [STRINGID_HEALINGWISHCAMETRUE]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX2}に いやしのねがいが\nとどいた！"),
    [STRINGID_HEALINGWISHHEALED]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の HPが かいふくした！"),
    [STRINGID_LUNARDANCECAMETRUE]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nしんぴの つきあかりに つつまれた！"),
    [STRINGID_CURSEDBODYDISABLED]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_BUFF1}は\nかなしばりに なった！"),
    [STRINGID_ATTACKERACQUIREDABILITY]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_ATK_ABILITY}を\nてにいれた！"), // Not in Gen 5+
    [STRINGID_TARGETABILITYSTATLOWER]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_DEF_ABILITY}で\n{B_BUFF1}が{B_BUFF2} さがった！"), // Not in Gen 5+
    [STRINGID_TARGETSTATWONTGOHIGHER]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_BUFF1}は もう あがらない！"),
    [STRINGID_PKMNMOVEBOUNCEDABILITY]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の {B_CURRENT_MOVE}は\nはねかえされた！"),
    [STRINGID_IMPOSTERTRANSFORM]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_DEF_NAME_WITH_PREFIX2}に へんしんした！"),
    [STRINGID_ASSAULTVESTDOESNTALLOW]               = COMPOUND_STRING("{JPN}{B_LAST_ITEM}の こうかで つかえない！\p"),
    [STRINGID_GRAVITYPREVENTSUSAGE]                 = COMPOUND_STRING("{JPN}じゅうりょくで {B_CURRENT_MOVE}は\n つかえない！\p"),
    [STRINGID_HEALBLOCKPREVENTSUSAGE]               = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nかいふくを ふうじられている！\p"),
    [STRINGID_NOTDONEYET]                           = COMPOUND_STRING("{JPN}この わざこうかは まだ みじっそうです！\p"),
    [STRINGID_STICKYWEBUSED]                        = COMPOUND_STRING("{JPN}ねばねばネット　が {B_DEF_TEAM2} ばに　はられた！"),
    [STRINGID_QUASHSUCCESS]                         = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の こうどうを\nあとまわしにした！"),
    [STRINGID_PKMNBLEWAWAYTOXICSPIKES]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は どくびしを\nふきとばした！"),
    [STRINGID_PKMNBLEWAWAYSTICKYWEB]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ねばねばネットを\nふきとばした！"),
    [STRINGID_PKMNBLEWAWAYSTEALTHROCK]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ステルスロックを\nふきとばした！"),
    [STRINGID_IONDELUGEON]                          = COMPOUND_STRING("{JPN}イオンの あめが せんじょうに ふりそそぐ！"),
    [STRINGID_TOPSYTURVYSWITCHEDSTATS]              = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}の のうりょくへんかが\nひっくりかえった！"),
    [STRINGID_TERRAINBECOMESMISTY]                  = COMPOUND_STRING("{JPN}ミストが たちこめた！"),
    [STRINGID_TERRAINBECOMESGRASSY]                 = COMPOUND_STRING("{JPN}くさが おいしげった！"),
    [STRINGID_TERRAINBECOMESELECTRIC]               = COMPOUND_STRING("{JPN}でんきが かけめぐった！"),
    [STRINGID_TERRAINBECOMESPSYCHIC]                = COMPOUND_STRING("{JPN}ふしぎな かんじに なった！"),
    [STRINGID_TARGETELECTRIFIED]                    = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の わざは\nでんきタイプに なった！"),
    [STRINGID_MEGAEVOREACTING]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の {B_LAST_ITEM}が\n{B_ATK_TRAINER_NAME}の メガリングに はんのうした！"), //actually displays the type of mega ring in inventory, but we didnt implement them :(
    [STRINGID_MEGAEVOEVOLVED]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は メガ{B_BUFF1}に メガシンカした！"),
    [STRINGID_DRASTICALLY]                          = gText_drastically,
    [STRINGID_SEVERELY]                             = gText_severely,
    [STRINGID_INFESTATION]                          = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}に\nまとわりつかれた！"),
    [STRINGID_NOEFFECTONTARGET]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}には こうかが ないようだ！"),
    [STRINGID_BURSTINGFLAMESHIT]                    = COMPOUND_STRING("{JPN}はじける ほのおが {B_EFF_NAME_WITH_PREFIX2}に あたった！"),
    [STRINGID_BESTOWITEMGIVING]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_ATK_NAME_WITH_PREFIX2}から\n{B_LAST_ITEM}を もらった！"),
    [STRINGID_THIRDTYPEADDED]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}に {B_BUFF1}タイプが\nついかされた！"),
    [STRINGID_FELLFORFEINT]                         = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は フェイントに ひっかかった！"),
    [STRINGID_POKEMONCANNOTUSEMOVE]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_CURRENT_MOVE}を\nつかえない！"),
    [STRINGID_COVEREDINPOWDER]                      = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は ふんじんを あびた！"),
    [STRINGID_POWDEREXPLODES]                       = COMPOUND_STRING("{JPN}ほのおが ふんじんに ふれて ばくはつした！"),
    [STRINGID_BELCHCANTSELECT]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ゲップがでない！\p"),
    [STRINGID_SPECTRALTHIEFSTEAL]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は あいての\nのうりょくアップを うばった！"),
    [STRINGID_GRAVITYGROUNDING]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は じゅうりょくで\nそらから おちた！"),
    [STRINGID_MISTYTERRAINPREVENTS]                 = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nミストに まもられている！"),
    [STRINGID_GRASSYTERRAINHEALS]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nグラスフィールドで かいふくした！"),
    [STRINGID_ELECTRICTERRAINPREVENTS]              = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nエレキフィールドに まもられている！"),
    [STRINGID_PSYCHICTERRAINPREVENTS]               = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nサイコフィールドに まもられている！"),
    [STRINGID_SAFETYGOGGLESPROTECTED]               = COMPOUND_STRING("{JPN}{B_LAST_ITEM}の こうかで {B_SCR_NAME_WITH_PREFIX}には\nきかない！"),
    [STRINGID_FLOWERVEILPROTECTED]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nはなのベールに まもられている！"),
    [STRINGID_FLOWERVEILPROTECTEDTARGET]            = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nはなのベールに まもられている！"),
    [STRINGID_AROMAVEILPROTECTED]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nアロマベールに まもられている！"),
    [STRINGID_CELEBRATEMESSAGE]                     = COMPOUND_STRING("{JPN}おめでとう {B_PLAYER_NAME}！"),
    [STRINGID_USEDINSTRUCTEDMOVE]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_ATK_NAME_WITH_PREFIX2}の\n しじに したがった！"),
    [STRINGID_THROATCHOPENDS]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}はおとを\nだせるようになった！"),
    [STRINGID_PKMNCANTUSEMOVETHROATCHOP]            = COMPOUND_STRING("{JPN}じごくづきの こうかで {B_ATK_NAME_WITH_PREFIX2}は\nおとを だせない！\p"),
    [STRINGID_LASERFOCUS]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nせいしんを とぎすませた！"),
    [STRINGID_GEMACTIVATES]                         = COMPOUND_STRING("{JPN}{B_LAST_ITEM}が\n{B_ATK_NAME_WITH_PREFIX2}の ちからを つよめた！"),
    [STRINGID_BERRYDMGREDUCES]                      = COMPOUND_STRING("{JPN}{B_LAST_ITEM}で\n{B_SCR_NAME_WITH_PREFIX2}への ダメージが よわまった！"),
    [STRINGID_AIRBALLOONFLOAT]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ふうせんで ういている！"),
    [STRINGID_AIRBALLOONPOP]                        = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の ふうせんが われた！"),
    [STRINGID_INCINERATEBURN]                       = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}の {B_LAST_ITEM}は\nやきつくされた！"),
    [STRINGID_BUGBITE]                              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は あいての\n{B_LAST_ITEM}を うばって たべた！"),
    [STRINGID_ILLUSIONWOREOFF]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の イリュージョンが とけた！"),
    [STRINGID_ATTACKERCUREDTARGETSTATUS]            = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_DEF_NAME_WITH_PREFIX2}の じょうたいを なおした！"),
    [STRINGID_ATTACKERLOSTFIRETYPE]                 = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nほのおタイプでは なくなった！"),
    [STRINGID_HEALERCURE]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}の\nじょうたいが なおった！"),
    [STRINGID_SCRIPTINGABILITYSTATRAISE]            = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\n{B_BUFF1}が{B_BUFF2} あがった！"),
    [STRINGID_RECEIVERABILITYTAKEOVER]              = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_SCR_ABILITY}を ひきついだ！"),
    [STRINGID_PKNMABSORBINGPOWER]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は パワーを すいこんでいる！"),
    [STRINGID_NOONEWILLBEABLETORUNAWAY]             = COMPOUND_STRING("{JPN}つぎのターンは だれも にげられない！"),
    [STRINGID_DESTINYKNOTACTIVATES]                 = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nメロメロに なった！"),
    [STRINGID_CLOAKEDINAFREEZINGLIGHT]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は いてつく ひかりに\nつつまれた！"),
    [STRINGID_CLEARAMULETWONTLOWERSTATS]            = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX2}の {B_LAST_ITEM}で\nのうりょくは さがらない！"),
    [STRINGID_FERVENTWISHREACHED]                   = COMPOUND_STRING("{JPN}{B_ATK_TRAINER_NAME}の あついねがいが\n{B_ATK_NAME_WITH_PREFIX2}に とどいた！"),
    [STRINGID_AIRLOCKACTIVATES]                     = COMPOUND_STRING("{JPN}てんきの こうかが なくなった！"),
    [STRINGID_PRESSUREENTERS]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は プレッシャーを はなっている！"),
    [STRINGID_DARKAURAENTERS]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ダークオーラを はなっている！"),
    [STRINGID_FAIRYAURAENTERS]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は フェアリーオーラを はなっている！"),
    [STRINGID_AURABREAKENTERS]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は すべての オーラを せいあつする！"),
    [STRINGID_COMATOSEENTERS]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nゆめうつつの じょうたい！"),
    [STRINGID_SCREENCLEANERENTERS]                  = COMPOUND_STRING("{JPN}ばが きれいに そうじされた！"),
    [STRINGID_FETCHEDPOKEBALL]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}を みつけた！"),
    [STRINGID_ASANDSTORMKICKEDUP]                   = COMPOUND_STRING("{JPN}すなあらしが ふきあれた！"),
    [STRINGID_PKMNSWILLPERISHIN3TURNS]              = COMPOUND_STRING("{JPN}おたがいの ポケモンは\n3ターンごに たおれる！"),
    [STRINGID_AURAFLAREDTOLIFE]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の オーラが もえあがった！"),
    [STRINGID_ASONEENTERS]                          = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nふたつの とくせいを あわせもつ！"),
    [STRINGID_CURIOUSMEDICINEENTERS]                = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}の\nのうりょくへんかが もとに もどった！"),
    [STRINGID_CANACTFASTERTHANKSTO]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}の おかげで\nいつもより はやく うごける！"),
    [STRINGID_MICLEBERRYACTIVATES]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nつぎの わざが あたりやすくなった！"),
    [STRINGID_PKMNSHOOKOFFTHETAUNT]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ちょうはつが とけた！"),
    [STRINGID_PKMNGOTOVERITSINFATUATION]            = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は メロメロが とけた！"),
    [STRINGID_ITEMCANNOTBEREMOVED]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の どうぐは\nはずせない！"),
    [STRINGID_STICKYBARBTRANSFER]                   = COMPOUND_STRING("{JPN}{B_LAST_ITEM}が\n{B_ATK_NAME_WITH_PREFIX2}に くっついた！"),
    [STRINGID_PKMNBURNHEALED]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の やけどが なおった！"),
    [STRINGID_REDCARDACTIVATE]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_ATK_NAME_WITH_PREFIX2}に\nレッドカードを つきつけた！"),
    [STRINGID_EJECTBUTTONACTIVATE]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nいれかわった！"),
    [STRINGID_ATKGOTOVERINFATUATION]                = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\nメロメロが とけた！"),
    [STRINGID_TORMENTEDNOMORE]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の いちゃもんが とけた！"),
    [STRINGID_HEALBLOCKEDNOMORE]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\nかいふくふうじから かいほうされた！"),
    [STRINGID_ATTACKERBECAMEFULLYCHARGED]           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は トレーナーとの\nきずなで ちからが みなぎった！\p"),
    [STRINGID_ATTACKERBECAMEASHSPECIES]             = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は サトシゲッコウガに なった！\p"),
    [STRINGID_EXTREMELYHARSHSUNLIGHT]               = COMPOUND_STRING("{JPN}ひざしが とても つよくなった！"),
    [STRINGID_EXTREMESUNLIGHTFADED]                 = COMPOUND_STRING("{JPN}とても つよい ひざしが おさまった！"),
    [STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT]     = COMPOUND_STRING("{JPN}みずが じょうはつした！"),
    [STRINGID_EXTREMELYHARSHSUNLIGHTWASNOTLESSENED] = COMPOUND_STRING("{JPN}かんてんの そらもよう！"),
    [STRINGID_HEAVYRAIN]                            = COMPOUND_STRING("{JPN}つよい あめが ふりはじめた！"),
    [STRINGID_HEAVYRAINLIFTED]                      = COMPOUND_STRING("{JPN}つよい あめが あがった！"),
    [STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN]         = COMPOUND_STRING("{JPN}ほのおが つよい あめで かきけされた！"),
    [STRINGID_NORELIEFROMHEAVYRAIN]                 = COMPOUND_STRING("{JPN}ごううが ふりやまない"),
    [STRINGID_MYSTERIOUSAIRCURRENT]                 = COMPOUND_STRING("{JPN}らんきりゅうが ひこうタイプの ポケモンを \nまもっている！"),
    [STRINGID_STRONGWINDSDISSIPATED]                = COMPOUND_STRING("{JPN}らんきりゅうが きえた！"),
    [STRINGID_MYSTERIOUSAIRCURRENTBLOWSON]          = COMPOUND_STRING("{JPN}らんきりゅうは みだれくるう！"),
    [STRINGID_ATTACKWEAKENEDBSTRONGWINDS]           = COMPOUND_STRING("{JPN}らんきりゅうで こうげきが よわめられた！"),
    [STRINGID_STUFFCHEEKSCANTSELECT]                = COMPOUND_STRING("{JPN}きのみを もっていないので\nこのわざは つかえない！\p"),
    [STRINGID_PKMNREVERTEDTOPRIMAL]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の ゲンシカイキ！\nゲンシの すがたに もどった！"),
    [STRINGID_BUTPOKEMONCANTUSETHEMOVE]             = COMPOUND_STRING("{JPN}しかし {B_ATK_NAME_WITH_PREFIX2}は\nそのわざを つかえない！"),
    [STRINGID_BUTHOOPACANTUSEIT]                    = COMPOUND_STRING("{JPN}しかし {B_ATK_NAME_WITH_PREFIX2}は\nいまの すがたでは つかえない！"),
    [STRINGID_BROKETHROUGHPROTECTION]               = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX2}の まもりを\nうちやぶった！"),
    [STRINGID_ABILITYALLOWSONLYMOVE]                = COMPOUND_STRING("{JPN}{B_ATK_ABILITY}の ため\n{B_CURRENT_MOVE}しか つかえない！\p"),
    [STRINGID_SWAPPEDABILITIES]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は あいてと\nとくせいを いれかえた！"),
    [STRINGID_PKMNHEALEDPOISON]                     = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}の どくが なおった！"),
    [STRINGID_BATTLERTYPECHANGEDTO]                 = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_BUFF1}タイプに なった！"),
    [STRINGID_BOTHCANNOLONGERESCAPE]                = COMPOUND_STRING("{JPN}どちらの ポケモンも にげられない！"),
    [STRINGID_CANTESCAPEDUETOUSEDMOVE]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は はいすいのじんで\nもう にげられない！"),
    [STRINGID_PKMNBECAMEWEAKERTOFIRE]               = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nほのおに よわくなった！"),
    [STRINGID_ABOUTTOUSEPOLTERGEIST]                = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\n{B_LAST_ITEM}で こうげきされそうだ！"),
    [STRINGID_CANTESCAPEBECAUSEOFCURRENTMOVE]       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は たこがためで\nもう にげられない！"),
    [STRINGID_NEUTRALIZINGGASENTERS]                = COMPOUND_STRING("{JPN}あたりに かがくへんかガスが じゅうまんした！"),
    [STRINGID_NEUTRALIZINGGASOVER]                  = COMPOUND_STRING("{JPN}かがくへんかガスの こうかが きれた！"),
    [STRINGID_TARGETTOOHEAVY]                       = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は おもすぎて もちあげられない！"),
    [STRINGID_PKMNTOOKTARGETHIGH]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_DEF_NAME_WITH_PREFIX2}を \nそらへ つれさった！"),
    [STRINGID_PKMNINSNAPTRAP]                       = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は トラバサミに とらわれた！"),
    [STRINGID_METEORBEAMCHARGING]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は うちゅうの パワーを \n みなぎらせた！"),
    [STRINGID_HEATUPBEAK]                           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は くちばしを ねっした！"),
    [STRINGID_COURTCHANGE]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は おたがいの ばの こうかを いれかえた！"),
    [STRINGID_ZPOWERSURROUNDS]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は Zパワーを まとった！"),
    [STRINGID_ZMOVEUNLEASHED]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ぜんりょくの Zワザを はなった！"),
    [STRINGID_ZMOVERESETSSTATS]                     = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は Zパワーで さがった\nのうりょくを もどした！"),
    [STRINGID_ZMOVEALLSTATSUP]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は Zパワーで のうりょくが あがった！"),
    [STRINGID_ZMOVEZBOOSTCRIT]                      = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は Zパワーで はりきっている！"),
    [STRINGID_ZMOVERESTOREHP]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は Zパワーで HPを かいふくした！"),
    [STRINGID_ZMOVESTATUP]                          = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は Zパワーで のうりょくが あがった！"),
    [STRINGID_ZMOVEHPTRAP]                          = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は Zパワーで  かいふくした！"),
    [STRINGID_ATTACKEREXPELLEDTHEPOISON]            = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しんぱいさせまいと\nじりきで どくを なおした！"),
    [STRINGID_ATTACKERSHOOKITSELFAWAKE]             = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しんぱいさせまいと\nなんとか めを さました！"),
    [STRINGID_ATTACKERBROKETHROUGHPARALYSIS]        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しんぱいさせまいと\nきあいで まひを なおした！"),
    [STRINGID_ATTACKERHEALEDITSBURN]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しんぱいさせまいと\nこんじょうで やけどを なおした！"),
    [STRINGID_ATTACKERMELTEDTHEICE]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しんぱいさせまいと\nがんばって こおりを とかした！"),
    [STRINGID_TARGETTOUGHEDITOUT]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は かなしませまいと\nもちこたえた！"),
    [STRINGID_ATTACKERLOSTELECTRICTYPE]             = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nでんきを つかいきった！"),
    [STRINGID_ATTACKERSWITCHEDSTATWITHTARGET]       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_BUFF1}を あいてと いれかえた！"),
    [STRINGID_BEINGHITCHARGEDPKMNWITHPOWER]         = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX2}は\n{B_CURRENT_MOVE}を うけて じゅうでんした！"),
    [STRINGID_ORICHALCUMPULSEACTIVATES] = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ひざしを つよめ\nこだいのこどうが あばれだす!!"),
    [STRINGID_HADRONENGINEACTIVATES] = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は エレキフィールドを はり\nみらいのきかんを やくどうさせる!!"),
    [STRINGID_SUNLIGHTACTIVATEDABILITY]             = COMPOUND_STRING("{JPN}つよい ひざしで{B_SCR_NAME_WITH_PREFIX2}の\nこだいかっせいが はつどうした！"),
    [STRINGID_STATWASHEIGHTENED]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_BUFF1}が たかまった！"),
    [STRINGID_ELECTRICTERRAINACTIVATEDABILITY]      = COMPOUND_STRING("{JPN}エレキフィールドで {B_SCR_NAME_WITH_PREFIX2}の\nクォークチャージが はつどうした！"),
    [STRINGID_ABILITYWEAKENEDSURROUNDINGMONSSTAT]   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\nまわりの ポケモンの {B_BUFF1}が よわまった！\p"),
    [STRINGID_ATTACKERGAINEDSTRENGTHFROMTHEFALLEN]  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は たおされた なかまから\nちからを もらった！"),
    [STRINGID_PKMNSABILITYPREVENTSABILITY]          = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_SCR_ABILITY}で\n{B_DEF_NAME_WITH_PREFIX2}の {B_DEF_ABILITY}は はたらかない！"), //not in gen 5+, ability popup
    [STRINGID_PREPARESHELLTRAP]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nトラップシェルを しかけた！"),
    [STRINGID_SHELLTRAPDIDNTWORK]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の\nトラップシェルは うまく きまらなかった！"),
    [STRINGID_SPIKESDISAPPEAREDFROMTEAM]            = COMPOUND_STRING("{JPN} {B_ATK_TEAM2} ばの　まきびしが　きえた！"),
    [STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("{JPN} {B_ATK_TEAM2} ばの　どくびしが　きえた！"),
    [STRINGID_STICKYWEBDISAPPEAREDFROMTEAM]         = COMPOUND_STRING("{JPN} {B_ATK_TEAM2} ばの　ねばねばネット　がきえた！"),
    [STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM]       = COMPOUND_STRING("{JPN}{B_ATK_TEAM2} ばの　いわが　きえた！"),
    [STRINGID_COULDNTFULLYPROTECT]                  = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nまもりきれず ダメージを うけた！"),
    [STRINGID_STOCKPILEDEFFECTWOREOFF]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}の\nたくわえる こうかが きえた！"),
    [STRINGID_PKMNREVIVEDREADYTOFIGHT]              = COMPOUND_STRING("{JPN}{B_BUFF1}は ふっかつして\nまた たたかえるように なった！"),
    [STRINGID_ITEMRESTOREDSPECIESHEALTH]            = COMPOUND_STRING("{JPN}{B_BUFF1}の HPが かいふくした！"),
    [STRINGID_ITEMCUREDSPECIESSTATUS]               = COMPOUND_STRING("{JPN}{B_BUFF1}の じょうたいが なおった！"), // Not in Gen 5+
    [STRINGID_ITEMRESTOREDSPECIESPP]                = COMPOUND_STRING("{JPN}{B_BUFF1}の {B_BUFF2}の\nPPが かいふくした！"),
    [STRINGID_THUNDERCAGETRAPPED]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は\n{B_EFF_NAME_WITH_PREFIX2}を とじこめた！"),
    [STRINGID_PKMNHURTBYFROSTBITE]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nしもやけの ダメージを うけた！"),
    [STRINGID_PKMNGOTFROSTBITE]                     = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は しもやけに なった！"),
    [STRINGID_PKMNSITEMHEALEDFROSTBITE]             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の {B_LAST_ITEM}で\nしもやけが なおった！"),
    [STRINGID_ATTACKERHEALEDITSFROSTBITE]           = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しんぱいさせまいと\nがんばって しもやけを なおした！"),
    [STRINGID_PKMNFROSTBITEHEALED]                  = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の しもやけが なおった！"),
    [STRINGID_PKMNFROSTBITEHEALEDBY]                = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_CURRENT_MOVE}で\nしもやけが なおった！"),
    [STRINGID_MIRRORHERBCOPIED]                     = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ものまねハーブで\nあいての のうりょくへんかを まねた！"),
    [STRINGID_STARTEDSNOW]                          = COMPOUND_STRING("{JPN}ゆきが ふりはじめた！"),
    [STRINGID_SNOWCONTINUES]                        = COMPOUND_STRING("{JPN}ゆきが ふりつづいている"), //not in gen 5+ (lol)
    [STRINGID_SNOWSTOPPED]                          = COMPOUND_STRING("{JPN}ゆきが ふりやんだ！"),
    [STRINGID_SNOWWARNINGSNOW]                      = COMPOUND_STRING("{JPN}ゆきが ふりはじめた！"),
    [STRINGID_PKMNITEMMELTED]                       = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_DEF_NAME_WITH_PREFIX2}の {B_LAST_ITEM}を とかした！"),
    [STRINGID_ULTRABURSTREACTING]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX2}から かがやきが あふれだす！"),
    [STRINGID_ULTRABURSTCOMPLETED]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ウルトラバーストで\n かがやいている！"),
    [STRINGID_TEAMGAINEDEXP]                        = COMPOUND_STRING("{JPN}がくしゅうそうちで てもちも\nけいけんちを もらった！\p"),
    [STRINGID_CURRENTMOVECANTSELECT]                = COMPOUND_STRING("{JPN}{B_BUFF1}は えらべない！\p"),
    [STRINGID_TARGETISBEINGSALTCURED]               = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は しおづけに された！"),
    [STRINGID_TARGETISHURTBYSALTCURE]               = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}で\nダメージを うけた！"),
    [STRINGID_TARGETCOVEREDINSTICKYCANDYSYRUP]      = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は\nねばねばした あめに つつまれた！"),
    [STRINGID_SHARPSTEELFLOATS]                     = COMPOUND_STRING("{JPN}とがった　いわが {B_DEF_TEAM2} ポケモンに　ささる！"),
    [STRINGID_SHARPSTEELDMG]                        = COMPOUND_STRING("{JPN}するどい はがねが\n{B_DEF_NAME_WITH_PREFIX2}に くいこんだ！"),
    [STRINGID_PKMNBLEWAWAYSHARPSTEEL]               = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nするどい はがねを ふきとばした！"),
    [STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM]        = COMPOUND_STRING("{JPN}{B_ATK_TEAM2} ばの　とがったはがねが　きえた！"),
    [STRINGID_TEAMTRAPPEDWITHVINES]                 = COMPOUND_STRING("{JPN}{B_EFF_TEAM1}ポケモンは\nツルに とらわれた！"),
    [STRINGID_PKMNHURTBYVINES]                      = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nキョダイベンタツの ツルで ダメージを うけた！"),
    [STRINGID_TEAMCAUGHTINVORTEX]                   = COMPOUND_STRING("{JPN}{B_EFF_TEAM1}ポケモンは\nみずの うずに とじこめられた！"),
    [STRINGID_PKMNHURTBYVORTEX]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nキョダイホウゲキの うずで ダメージを うけた！"),
    [STRINGID_TEAMSURROUNDEDBYFIRE]                 = COMPOUND_STRING("{JPN}{B_EFF_TEAM1}ポケモンは\nほのおに つつまれた！"),
    [STRINGID_PKMNBURNINGUP]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nキョダイゴクエンの ほのおで ダメージを うけた！"),
    [STRINGID_TEAMSURROUNDEDBYROCKS]                = COMPOUND_STRING("{JPN}{B_EFF_TEAM1}ポケモンは\nいわに とりかこまれた！"),
    [STRINGID_PKMNHURTBYROCKSTHROWN]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nキョダイフンセキの いわで ダメージを うけた！"),
    [STRINGID_MOVEBLOCKEDBYDYNAMAX]                 = COMPOUND_STRING("{JPN}ダイマックスの ちからで わざが ふせがれた！"),
    [STRINGID_ZEROTOHEROTRANSFORMATION]             = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は ヒーローの すがたで\nもどってきた！"),
    [STRINGID_THETWOMOVESBECOMEONE]                 = COMPOUND_STRING("{JPN}ふたつの わざが ひとつになった！\nがったいわざだ！{PAUSE 16}"),
    [STRINGID_ARAINBOWAPPEAREDONSIDE]               = COMPOUND_STRING("{JPN}{B_ATK_TEAM2} ばの　そらににじが　あらわれた！"),
    [STRINGID_THERAINBOWDISAPPEARED]                = COMPOUND_STRING("{JPN}{B_ATK_TEAM2} ばの　にじが　きえた！"),
    [STRINGID_WAITINGFORPARTNERSMOVE]               = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\n{B_ATK_PARTNER_NAME}の わざを まっている...{PAUSE 16}"),
    [STRINGID_SEAOFFIREENVELOPEDSIDE]               = COMPOUND_STRING("{JPN}ほのおの　うみで　 {B_DEF_TEAM2} ばは　つつまれた！"),
    [STRINGID_HURTBYTHESEAOFFIRE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nほのおのうみで ダメージを うけた！"),
    [STRINGID_THESEAOFFIREDISAPPEARED]              = COMPOUND_STRING("{JPN}{B_EFF_TEAM2}チームの まわりの\nほのおのうみが きえた！"),
    [STRINGID_SWAMPENVELOPEDSIDE]                   = COMPOUND_STRING("{JPN}{B_EFF_TEAM2}チームは\nしつげんに つつまれた！"),
    [STRINGID_THESWAMPDISAPPEARED]                  = COMPOUND_STRING("{JPN}{B_EFF_TEAM2}チームの まわりの\nしつげんが きえた！"),
    [STRINGID_PKMNTELLCHILLINGRECEPTIONJOKE]        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nさむいジョークを ひろう！"),
    [STRINGID_HOSPITALITYRESTORATION]               = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_SCR_NAME_WITH_PREFIX2}が\nたてた おちゃを のみほした！"),
    [STRINGID_ELECTROSHOTCHARGING]                  = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は でんきを あつめた！"),
    [STRINGID_ITEMWASUSEDUP]                        = COMPOUND_STRING("{JPN}{B_LAST_ITEM}は なくなった..."),
    [STRINGID_ATTACKERLOSTITSTYPE]                  = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX}は {B_BUFF1}タイプでは なくなった！"),
    [STRINGID_SHEDITSTAIL]                          = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は しっぽを きりはなして\nみがわりを つくった！"),
    [STRINGID_CLOAKEDINAHARSHLIGHT]                 = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nまばゆい ひかりに つつまれた！"),
    [STRINGID_SUPERSWEETAROMAWAFTS]                 = COMPOUND_STRING("{JPN}{B_EFF_NAME_WITH_PREFIX2}を おおう\nシロップから あまいかおりが ただよう！"),
    [STRINGID_DIMENSIONSWERETWISTED]                = COMPOUND_STRING("{JPN}じくうが ゆがんだ！"),
    [STRINGID_BIZARREARENACREATED]                  = COMPOUND_STRING("{JPN}もちものの こうかが なくなる\nふしぎな くうかんが できた！"),
    [STRINGID_BIZARREAREACREATED]                   = COMPOUND_STRING("{JPN}ぼうぎょと とくぼうが いれかわる\nふしぎな くうかんが できた！"),
    [STRINGID_TIDYINGUPCOMPLETE]                    = COMPOUND_STRING("{JPN}おかたづけ かんりょう！"),
    [STRINGID_TIMETOTERASTALLIZE]                   = COMPOUND_STRING("{JPN}ひかりかがやけ テラスタル！"),
    [STRINGID_PKMNTERASTALLIZEDINTO]                = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は {B_BUFF1}タイプに テラスタルした！"), // Does not exist, meant to mimic form change strings
    [STRINGID_BOOSTERENERGYACTIVATES]               = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は {B_LAST_ITEM}で\n{B_SCR_ABILITY}を はつどうした！"),
    [STRINGID_FOGCREPTUP]                           = COMPOUND_STRING("{JPN}こい きりが たちこめた！"),
    [STRINGID_FOGISDEEP]                            = COMPOUND_STRING("{JPN}きりが ふかい"),
    [STRINGID_FOGLIFTED]                            = COMPOUND_STRING("{JPN}きりが はれた"),
    [STRINGID_PKMNMADESHELLGLEAM]                   = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX}は\nこうらを かがやかせた！ タイプあいしょうを ゆがめる！"),
    [STRINGID_FICKLEBEAMDOUBLED]                    = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nこの こうげきに ぜんりょくを そそいだ！"),
    [STRINGID_COMMANDERACTIVATES]                   = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は しれいとう として\n{B_BUFF1}に のみこまれた！"),
    [STRINGID_POKEFLUTECATCHY]                      = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は {B_LAST_ITEM} ふいた！\pうーん！\nすばらしい ねいろだ！"),
    [STRINGID_POKEFLUTE]                            = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は\n{B_LAST_ITEM}を ふいた！"),
    [STRINGID_MONHEARINGFLUTEAWOKE]                 = COMPOUND_STRING("{JPN}ふえのねを きいた\nポケモンは めを さました！\n"),
    [STRINGID_SUNLIGHTISHARSH]                      = COMPOUND_STRING("{JPN}ひざしが つよい！"),
    [STRINGID_ITISHAILING]                          = COMPOUND_STRING("{JPN}あられが ふっている！"),
    [STRINGID_ITISSNOWING]                          = COMPOUND_STRING("{JPN}ゆきが ふっている！"),
    [STRINGID_ISCOVEREDWITHGRASS]                   = COMPOUND_STRING("{JPN}あしもとに くさが しげった！"),
    [STRINGID_MISTSWIRLSAROUND]                     = COMPOUND_STRING("{JPN}あたりに きりが たちこめた！"),
    [STRINGID_ELECTRICCURRENTISRUNNING]             = COMPOUND_STRING("{JPN}あしもとに でんきが かけめぐる！"),
    [STRINGID_SEEMSWEIRD]                           = COMPOUND_STRING("{JPN}あたりが ふしぎな かんじに なった！"),
    [STRINGID_WAGGLINGAFINGER]                      = COMPOUND_STRING("{JPN}ゆびをふって {B_CURRENT_MOVE}が でた！"),
    [STRINGID_BLOCKEDBYSLEEPCLAUSE]                 = COMPOUND_STRING("{JPN}ルールにより\n{B_DEF_NAME_WITH_PREFIX2}は ねむらなかった!"),
        [STRINGID_SUPEREFFECTIVETWOFOES]                = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}と {B_DEF_PARTNER_NAME}には こうかばつぐんだ！"),
    [STRINGID_NOTVERYEFFECTIVETWOFOES]              = COMPOUND_STRING("{JPN}{B_DEF_NAME_WITH_PREFIX2}と {B_DEF_PARTNER_NAME}には\nこうかは いまひとつだ..."),
    [STRINGID_SENDCAUGHTMONPARTYORBOX]              = COMPOUND_STRING("{JPN}{B_DEF_NAME}を てもちに くわえますか？"),
    [STRINGID_PKMNSENTTOPCAFTERCATCH]               = gText_PkmnSentToPCAfterCatch,
    [STRINGID_PKMNDYNAMAXED]                        = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は ダイマックスした！"),
    [STRINGID_PKMNGIGANTAMAXED]                     = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は キョダイマックスした！"),
    [STRINGID_TIMETODYNAMAX]                        = COMPOUND_STRING("{JPN}ダイマックスだ！"),
    [STRINGID_TIMETOGIGANTAMAX]                     = COMPOUND_STRING("{JPN}キョダイマックスだ！"),
    [STRINGID_QUESTIONFORFEITBATTLE]                = COMPOUND_STRING("{JPN}しょうぶを あきらめて\nこうさん しますか?\pこうさん すると\nまけたことに なります"),
    [STRINGID_POWERCONSTRUCTPRESENCEOFMANY]         = COMPOUND_STRING("{JPN}たくさんの けはいを かんじる！"),
    [STRINGID_POWERCONSTRUCTTRANSFORM]              = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は\nパーフェクトフォルムに へんしんした！"),
    [STRINGID_ABILITYSHIELDPROTECTS]                = COMPOUND_STRING("{JPN}{B_LAST_ITEM}の こうかで\n{B_ATK_NAME_WITH_PREFIX}の とくせいは まもられている！"),
    [STRINGID_MONTOOSCAREDTOMOVE]                   = COMPOUND_STRING("{JPN}{B_ATK_NAME_WITH_PREFIX}は こわがっている!\nわざを だすことが できない!"),
    [STRINGID_GHOSTGETOUTGETOUT]                    = COMPOUND_STRING("{JPN}ゆうれい『タチサレ…… タチサレ……"),
    [STRINGID_SILPHSCOPEUNVEILED]                   = COMPOUND_STRING("{JPN}シルフスコープで ゆうれいの\nしょうたいを みやぶった!"),
    [STRINGID_GHOSTWASMAROWAK]                      = COMPOUND_STRING("{JPN}ゆうれいの しょうたいは\nガラガラ だった!\p"),
    [STRINGID_TRAINER1MON1COMEBACK]                 = COMPOUND_STRING("{JPN}{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}！ もどれ！"),
    [STRINGID_THREWROCK]                            = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は {B_OPPONENT_MON1_NAME}に\nいしを なげた！"),
    [STRINGID_THREWBAIT]                            = COMPOUND_STRING("{JPN}{B_PLAYER_NAME}は {B_OPPONENT_MON1_NAME}に\nエサを なげた！"),
    [STRINGID_PKMNANGRY]                            = COMPOUND_STRING("{JPN}{B_OPPONENT_MON1_NAME}は おこっている！"),
    [STRINGID_PKMNEATING]                           = COMPOUND_STRING("{JPN}{B_OPPONENT_MON1_NAME}は エサを たべている！"),
    [STRINGID_PKMNDISGUISEWASBUSTED]                = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の ばけのかわが はがれた！"),
    [STRINGID_ZENMODETRIGGERED]                     = COMPOUND_STRING("{JPN}{B_SCR_ABILITY}が はつどうした！"),
    [STRINGID_ZENMODEENDED]                         = COMPOUND_STRING("{JPN}{B_SCR_ABILITY}が おわった！"),
    [STRINGID_SCRCUREDPARALYSIS]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の まひが なおった！"),
    [STRINGID_SCRCUREDPOISON]                       = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の どくが なおった！"),
    [STRINGID_SCRCUREDBURN]                         = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の やけどが なおった！"),
    [STRINGID_SCRCUREDSLEEP]                        = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}は めを さました！"),
    [STRINGID_SCRCUREDCONFUSION]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の こんらんが とけた！"),
    [STRINGID_PARTYCUREDPARALYSIS]                  = COMPOUND_STRING("{JPN}{B_BUFF1}の まひが なおった！"),
    [STRINGID_PARTYCUREDPOISON]                     = COMPOUND_STRING("{JPN}{B_BUFF1}の どくが なおった！"),
    [STRINGID_PARTYCUREDBURN]                       = COMPOUND_STRING("{JPN}{B_BUFF1}の やけどが なおった！"),
    [STRINGID_PARTYCUREDSLEEP]                      = COMPOUND_STRING("{JPN}{B_BUFF1}は めを さました！"),
    [STRINGID_PARTYCUREDFREEZE]                     = COMPOUND_STRING("{JPN}{B_BUFF1}の こおりが とけた！"),
    [STRINGID_PARTYCUREDFROSTBITE]                  = COMPOUND_STRING("{JPN}{B_BUFF1}の しもやけが なおった！"),
    [STRINGID_PKMNATKNOTLOWERED]                    = COMPOUND_STRING("{JPN}{B_SCR_NAME_WITH_PREFIX}の こうげきは さがらない！"),
    [STRINGID_REFLECTWOREOFF]                       = COMPOUND_STRING("{JPN}{B_DEF_TEAM1} リフレクター　がきえた！"),
    [STRINGID_LIGHTSCREENWOREOFF]                   = COMPOUND_STRING("{JPN}{B_DEF_TEAM1} ひかりのかべが　きえた！"),
    [STRINGID_AURORAVEILWOREOFF]                    = COMPOUND_STRING("{JPN}{B_DEF_TEAM1} オーロラベール　がきえた！"),
    [STRINGID_STICKYWEBDISAPPEAREDFROMYOU]          = COMPOUND_STRING("{JPN}こちらの まわりの\nねばねばネットが きえた！"),

};

const u16 gTrainerUsedItemStringIds[] =
{
    STRINGID_PLAYERUSEDITEM, STRINGID_TRAINER1USEDITEM
};

const u16 gZEffectStringIds[] =
{
    [B_MSG_Z_RESET_STATS] = STRINGID_ZMOVERESETSSTATS,
    [B_MSG_Z_ALL_STATS_UP]= STRINGID_ZMOVEALLSTATSUP,
    [B_MSG_Z_BOOST_CRITS] = STRINGID_ZMOVEZBOOSTCRIT,
    [B_MSG_Z_FOLLOW_ME]   = STRINGID_PKMNCENTERATTENTION,
    [B_MSG_Z_RECOVER_HP]  = STRINGID_ZMOVERESTOREHP,
    [B_MSG_Z_STAT_UP]     = STRINGID_ZMOVESTATUP,
    [B_MSG_Z_HP_TRAP]     = STRINGID_ZMOVEHPTRAP,
};

const u16 gMentalHerbCureStringIds[] =
{
    [B_MSG_MENTALHERBCURE_INFATUATION] = STRINGID_ATKGOTOVERINFATUATION,
    [B_MSG_MENTALHERBCURE_TORMENT]     = STRINGID_TORMENTEDNOMORE,
    [B_MSG_MENTALHERBCURE_DISABLE]     = STRINGID_PKMNMOVEDISABLEDNOMORE,
    [B_MSG_MENTALHERBCURE_HEALBLOCK]   = STRINGID_HEALBLOCKEDNOMORE,
    [B_MSG_MENTALHERBCURE_ENCORE]      = STRINGID_PKMNENCOREENDED,
    [B_MSG_MENTALHERBCURE_TAUNT]       = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gStartingStatusStringIds[B_MSG_STARTING_STATUS_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_SET_TRICK_ROOM]       = STRINGID_DIMENSIONSWERETWISTED,
    [B_MSG_SET_MAGIC_ROOM]       = STRINGID_BIZARREARENACREATED,
    [B_MSG_SET_WONDER_ROOM]      = STRINGID_BIZARREAREACREATED,
    [B_MSG_SET_TAILWIND]         = STRINGID_TAILWINDBLEW,
    [B_MSG_SET_RAINBOW]          = STRINGID_ARAINBOWAPPEAREDONSIDE,
    [B_MSG_SET_SEA_OF_FIRE]      = STRINGID_SEAOFFIREENVELOPEDSIDE,
    [B_MSG_SET_SWAMP]            = STRINGID_SWAMPENVELOPEDSIDE,
    [B_MSG_SET_SPIKES]           = STRINGID_SPIKESSCATTERED,
    [B_MSG_SET_POISON_SPIKES]    = STRINGID_POISONSPIKESSCATTERED,
    [B_MSG_SET_STICKY_WEB]       = STRINGID_STICKYWEBUSED,
    [B_MSG_SET_STEALTH_ROCK]     = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SET_SHARP_STEEL]      = STRINGID_SHARPSTEELFLOATS,
};

const u16 gTerrainStringIds[B_MSG_TERRAIN_COUNT] =
{
    [B_MSG_TERRAIN_SET_MISTY] = STRINGID_TERRAINBECOMESMISTY,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_TERRAINBECOMESELECTRIC,
    [B_MSG_TERRAIN_SET_PSYCHIC] = STRINGID_TERRAINBECOMESPSYCHIC,
    [B_MSG_TERRAIN_SET_GRASSY] = STRINGID_TERRAINBECOMESGRASSY,
    [B_MSG_TERRAIN_END_MISTY] = STRINGID_MISTYTERRAINENDS,
    [B_MSG_TERRAIN_END_ELECTRIC] = STRINGID_ELECTRICTERRAINENDS,
    [B_MSG_TERRAIN_END_PSYCHIC] = STRINGID_PSYCHICTERRAINENDS,
    [B_MSG_TERRAIN_END_GRASSY] = STRINGID_GRASSYTERRAINENDS,
};

const u16 gTerrainPreventsStringIds[] =
{
    [B_MSG_TERRAINPREVENTS_MISTY]    = STRINGID_MISTYTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_ELECTRIC] = STRINGID_ELECTRICTERRAINPREVENTS,
    [B_MSG_TERRAINPREVENTS_PSYCHIC]  = STRINGID_PSYCHICTERRAINPREVENTS
};

const u16 gHealingWishStringIds[] =
{
    STRINGID_HEALINGWISHCAMETRUE,
    STRINGID_LUNARDANCECAMETRUE
};

const u16 gDmgHazardsStringIds[] =
{
    [B_MSG_PKMNHURTBYSPIKES]   = STRINGID_PKMNHURTBYSPIKES,
    [B_MSG_STEALTHROCKDMG]     = STRINGID_STEALTHROCKDMG,
    [B_MSG_SHARPSTEELDMG]      = STRINGID_SHARPSTEELDMG,
    [B_MSG_POINTEDSTONESFLOAT] = STRINGID_POINTEDSTONESFLOAT,
    [B_MSG_SPIKESSCATTERED]    = STRINGID_SPIKESSCATTERED,
    [B_MSG_SHARPSTEELFLOATS]   = STRINGID_SHARPSTEELFLOATS,
};

const u16 gSwitchInAbilityStringIds[] =
{
    [B_MSG_SWITCHIN_MOLDBREAKER] = STRINGID_MOLDBREAKERENTERS,
    [B_MSG_SWITCHIN_TERAVOLT] = STRINGID_TERAVOLTENTERS,
    [B_MSG_SWITCHIN_TURBOBLAZE] = STRINGID_TURBOBLAZEENTERS,
    [B_MSG_SWITCHIN_SLOWSTART] = STRINGID_SLOWSTARTENTERS,
    [B_MSG_SWITCHIN_UNNERVE] = STRINGID_UNNERVEENTERS,
    [B_MSG_SWITCHIN_ANTICIPATION] = STRINGID_ANTICIPATIONACTIVATES,
    [B_MSG_SWITCHIN_FOREWARN] = STRINGID_FOREWARNACTIVATES,
    [B_MSG_SWITCHIN_PRESSURE] = STRINGID_PRESSUREENTERS,
    [B_MSG_SWITCHIN_DARKAURA] = STRINGID_DARKAURAENTERS,
    [B_MSG_SWITCHIN_FAIRYAURA] = STRINGID_FAIRYAURAENTERS,
    [B_MSG_SWITCHIN_AURABREAK] = STRINGID_AURABREAKENTERS,
    [B_MSG_SWITCHIN_COMATOSE] = STRINGID_COMATOSEENTERS,
    [B_MSG_SWITCHIN_SCREENCLEANER] = STRINGID_SCREENCLEANERENTERS,
    [B_MSG_SWITCHIN_ASONE] = STRINGID_ASONEENTERS,
    [B_MSG_SWITCHIN_CURIOUS_MEDICINE] = STRINGID_CURIOUSMEDICINEENTERS,
    [B_MSG_SWITCHIN_PASTEL_VEIL] = STRINGID_PKMNHEALEDPOISON,
    [B_MSG_SWITCHIN_NEUTRALIZING_GAS] = STRINGID_NEUTRALIZINGGASENTERS,
};

const u16 gNoEscapeStringIds[] =
{
    [B_MSG_CANT_ESCAPE]          = STRINGID_CANTESCAPE,
    [B_MSG_DONT_LEAVE_BIRCH]     = STRINGID_DONTLEAVEBIRCH,
    [B_MSG_PREVENTS_ESCAPE]      = STRINGID_PREVENTSESCAPE,
    [B_MSG_CANT_ESCAPE_2]        = STRINGID_CANTESCAPE2,
    [B_MSG_ATTACKER_CANT_ESCAPE] = STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] =
{
    [B_MSG_STARTED_RAIN]      = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_DOWNPOUR]  = STRINGID_DOWNPOURSTARTED, // Unused
    [B_MSG_WEATHER_FAILED]    = STRINGID_BUTITFAILED,
    [B_MSG_STARTED_SANDSTORM] = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_SUNLIGHT]  = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL]      = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW]      = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_FOG]       = STRINGID_FOGCREPTUP, // Unused, can use for custom moves that set fog
};

const u16 gAbilityWeatherChangeStringId[] =
{
    [B_MSG_STARTED_DRIZZLE]        = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_SAND_STREAM]    = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_DROUGHT]        = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL_WARNING]   = STRINGID_STARTEDHAIL,
    [B_MSG_STARTED_SNOW_WARNING]   = STRINGID_STARTEDSNOW,
    [B_MSG_STARTED_DESOLATE_LAND]  = STRINGID_EXTREMELYHARSHSUNLIGHT,
    [B_MSG_STARTED_PRIMORDIAL_SEA] = STRINGID_HEAVYRAIN,
    [B_MSG_STARTED_STRONG_WINDS]   = STRINGID_MYSTERIOUSAIRCURRENT,
};

const u16 gWeatherEndsStringIds[B_MSG_WEATHER_END_COUNT] =
{
    [B_MSG_WEATHER_END_RAIN]                       = STRINGID_RAINSTOPPED,
    [B_MSG_WEATHER_END_SUN]                        = STRINGID_SUNLIGHTFADED,
    [B_MSG_WEATHER_END_SANDSTORM]                  = STRINGID_SANDSTORMSUBSIDED,
    [B_MSG_WEATHER_END_HAIL]                       = STRINGID_HAILSTOPPED,
    [B_MSG_WEATHER_END_SNOW]                       = STRINGID_SNOWSTOPPED,
    [B_MSG_WEATHER_END_FOG]                        = STRINGID_FOGLIFTED,
    [B_MSG_WEATHER_END_EXTREMELY_HARSH_SUNLIGHT]   = STRINGID_EXTREMESUNLIGHTFADED,
    [B_MSG_WEATHER_END_HEAVY_RAIN]                 = STRINGID_HEAVYRAINLIFTED,
    [B_MSG_WEATHER_END_STRONG_WINDS]               = STRINGID_STRONGWINDSDISSIPATED,
};

const u16 gWeatherTurnStringIds[] =
{
    [B_MSG_WEATHER_TURN_RAIN]         = STRINGID_RAINCONTINUES,
    [B_MSG_WEATHER_TURN_DOWNPOUR]     = STRINGID_DOWNPOURCONTINUES,
    [B_MSG_WEATHER_TURN_SUN]          = STRINGID_SUNLIGHTSTRONG,
    [B_MSG_WEATHER_TURN_SANDSTORM]    = STRINGID_SANDSTORMRAGES,
    [B_MSG_WEATHER_TURN_HAIL]         = STRINGID_HAILCONTINUES,
    [B_MSG_WEATHER_TURN_SNOW]         = STRINGID_SNOWCONTINUES,
    [B_MSG_WEATHER_TURN_FOG]          = STRINGID_FOGISDEEP,
    [B_MSG_WEATHER_TURN_STRONG_WINDS] = STRINGID_MYSTERIOUSAIRCURRENTBLOWSON,
};

const u16 gSandStormHailDmgStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_PKMNBUFFETEDBYSANDSTORM,
    [B_MSG_HAIL]      = STRINGID_PKMNPELTEDBYHAIL
};

const u16 gProtectLikeUsedStringIds[] =
{
    [B_MSG_PROTECTED_ITSELF] = STRINGID_PKMNPROTECTEDITSELF2,
    [B_MSG_BRACED_ITSELF]    = STRINGID_PKMNBRACEDITSELF,
    [B_MSG_PROTECTED_TEAM]   = STRINGID_PROTECTEDTEAM,
};

const u16 gBrokeProtectionStringIds[] =
{
    [B_MSG_FEINT]           = STRINGID_FELLFORFEINT,
    [B_MSG_HYPERSPACE_FURY] = STRINGID_BROKETHROUGHPROTECTION,
};

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    [B_MSG_SIDE_STATUS_FAILED]     = STRINGID_BUTITFAILED,
    [B_MSG_SET_REFLECT_SINGLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_REFLECT_DOUBLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_LIGHTSCREEN_SINGLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_LIGHTSCREEN_DOUBLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_SAFEGUARD]          = STRINGID_PKMNCOVEREDBYVEIL,
    [B_MSG_SET_AURORA_VEIL]        = STRINGID_PKMNAURORAVEIL,
};

const u16 gLeechSeedStringIds[] =
{
    [B_MSG_LEECH_SEED_SET]   = STRINGID_PKMNSEEDED,
    [B_MSG_LEECH_SEED_MISS]  = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_LEECH_SEED_FAIL]  = STRINGID_ITDOESNTAFFECT,
    [B_MSG_LEECH_SEED_DRAIN] = STRINGID_PKMNSAPPEDBYLEECHSEED,
    [B_MSG_LEECH_SEED_OOZE]  = STRINGID_ITSUCKEDLIQUIDOOZE,
};

const u16 gRestUsedStringIds[] =
{
    [B_MSG_REST]          = STRINGID_PKMNWENTTOSLEEP,
    [B_MSG_REST_STATUSED] = STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] =
{
    [B_MSG_UPROAR_CONTINUES] = STRINGID_PKMNMAKINGUPROAR,
    [B_MSG_UPROAR_ENDS]      = STRINGID_PKMNCALMEDDOWN
};

const u16 gWokeUpStringIds[] =
{
    [B_MSG_WOKE_UP]        = STRINGID_PKMNWOKEUP,
    [B_MSG_WOKE_UP_UPROAR] = STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gUproarAwakeStringIds[] =
{
    [B_MSG_CANT_SLEEP_UPROAR]  = STRINGID_PKMNCANTSLEEPINUPROAR2,
    [B_MSG_UPROAR_KEPT_AWAKE]  = STRINGID_UPROARKEPTPKMNAWAKE,
};

const u16 gStatUpStringIds[] =
{
    [B_MSG_STAT_CHANGED]            = STRINGID_STATROSE,
    [B_MSG_STAT_WONT_CHANGE]        = STRINGID_STATSWONTINCREASE,
    [B_MSG_STAT_MAXED]              = STRINGID_STATWASMAXEDOUT,
    [B_MSG_STAT_CHANGE_EMPTY]       = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]       = STRINGID_USINGITEMSTATOFPKMNROSE,
    [B_MSG_STAT_CHANGED_BELLY_DRUM] = STRINGID_PKMNCUTHPMAXEDATTACK,
    [B_MSG_USED_DIRE_HIT]           = STRINGID_PKMNUSEDXTOGETPUMPED,
};

// Mostly redundant, combine with above in a future pr
const u16 gStatDownStringIds[] =
{
    [B_MSG_STAT_CHANGED]            = STRINGID_STATFELL,
    [B_MSG_STAT_WONT_CHANGE]        = STRINGID_STATSWONTDECREASE,
    [B_MSG_STAT_CHANGE_EMPTY]       = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_CHANGED_ITEM]       = STRINGID_USINGITEMSTATOFPKMNFELL,
    [B_MSG_STAT_CHANGED_BELLY_DRUM] = STRINGID_PKMNCUTHPMAXEDATTACK, // Message for contrary is still printed
};

// Index copied from move's index in sTrappingMoves
const u16 gWrappedStringIds[NUM_TRAPPING_MOVES] =
{
    [B_MSG_WRAPPED_BIND]        = STRINGID_PKMNSQUEEZEDBYBIND,     // MOVE_BIND
    [B_MSG_WRAPPED_WRAP]        = STRINGID_PKMNWRAPPEDBY,          // MOVE_WRAP
    [B_MSG_WRAPPED_FIRE_SPIN]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_FIRE_SPIN
    [B_MSG_WRAPPED_CLAMP]       = STRINGID_PKMNCLAMPED,            // MOVE_CLAMP
    [B_MSG_WRAPPED_WHIRLPOOL]   = STRINGID_PKMNTRAPPEDINVORTEX,    // MOVE_WHIRLPOOL
    [B_MSG_WRAPPED_SAND_TOMB]   = STRINGID_PKMNTRAPPEDBYSANDTOMB,  // MOVE_SAND_TOMB
    [B_MSG_WRAPPED_MAGMA_STORM] = STRINGID_TRAPPEDBYSWIRLINGMAGMA, // MOVE_MAGMA_STORM
    [B_MSG_WRAPPED_INFESTATION] = STRINGID_INFESTATION,            // MOVE_INFESTATION
    [B_MSG_WRAPPED_SNAP_TRAP]   = STRINGID_PKMNINSNAPTRAP,         // MOVE_SNAP_TRAP
    [B_MSG_WRAPPED_THUNDER_CAGE]= STRINGID_THUNDERCAGETRAPPED,     // MOVE_THUNDER_CAGE
};

const u16 gMistUsedStringIds[] =
{
    [B_MSG_SET_MIST]    = STRINGID_PKMNSHROUDEDINMIST,
    [B_MSG_MIST_FAILED] = STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] =
{
    [B_MSG_GETTING_PUMPED]      = STRINGID_PKMNGETTINGPUMPED,
    [B_MSG_FOCUS_ENERGY_FAILED] = STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] =
{
    [B_MSG_TRANSFORMED]      = STRINGID_PKMNTRANSFORMEDINTO,
    [B_MSG_TRANSFORM_FAILED] = STRINGID_BUTITFAILED
};

const u16 gSubstituteUsedStringIds[] =
{
    [B_MSG_SET_SUBSTITUTE]    = STRINGID_PKMNMADESUBSTITUTE,
    [B_MSG_SUBSTITUTE_FAILED] = STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPOISONED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPARALYZED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLASLEEP,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNMADESLEEP,
};

const u16 gGotBurnedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASBURNED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNBURNEDBY
};

const u16 gGotFrostbiteStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNGOTFROSTBITE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNGOTFROSTBITE,
};

const u16 gFrostbiteHealedStringIds[] =
{
    [B_MSG_FROSTBITE_HEALED]         = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_FROSTBITE_HEALED_BY_MOVE] = STRINGID_PKMNFROSTBITEHEALEDBY
};

const u16 gGotFrozenStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASFROZEN,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] =
{
    [B_MSG_DEFROSTED]         = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_DEFROSTED_BY_MOVE] = STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] =
{
    [B_MSG_KO_MISS]       = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_KO_UNAFFECTED] = STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLINLOVE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNSXINFATUATEDY
};

const u16 gAbsorbDrainStringIds[] =
{
    [B_MSG_ABSORB]      = STRINGID_PKMNENERGYDRAINED,
    [B_MSG_ABSORB_OOZE] = STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] =
{
    [B_MSG_WEAKEN_ELECTRIC] = STRINGID_ELECTRICITYWEAKENED,
    [B_MSG_WEAKEN_FIRE]     = STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] =
{
    [B_MSG_BELL]                     = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_ATTACKER] = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_PARTNER]  = STRINGID_BELLCHIMED,
    [B_MSG_BELL_BOTH_SOUNDPROOF]     = STRINGID_BELLCHIMED,
    [B_MSG_SOOTHING_AROMA]           = STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] =
{
    [B_MSG_FUTURE_SIGHT] = STRINGID_PKMNFORESAWATTACK,
    [B_MSG_DOOM_DESIRE]  = STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] =
{
    [BALL_NO_SHAKES]     = STRINGID_PKMNBROKEFREE,
    [BALL_1_SHAKE]       = STRINGID_ITAPPEAREDCAUGHT,
    [BALL_2_SHAKES]      = STRINGID_AARGHALMOSTHADIT,
    [BALL_3_SHAKES_FAIL] = STRINGID_SHOOTSOCLOSE
};

// Overworld weathers that don't have an associated battle weather default to "It is raining."
const u16 gWeatherStartsStringIds[] =
{
    [WEATHER_NONE]               = STRINGID_ITISRAINING,
    [WEATHER_SUNNY_CLOUDS]       = STRINGID_ITISRAINING,
    [WEATHER_SUNNY]              = STRINGID_ITISRAINING,
    [WEATHER_RAIN]               = STRINGID_ITISRAINING,
    [WEATHER_SNOW]               = (B_OVERWORLD_SNOW >= GEN_9 ? STRINGID_ITISSNOWING : STRINGID_ITISHAILING),
    [WEATHER_RAIN_THUNDERSTORM]  = STRINGID_ITISRAINING,
    [WEATHER_FOG_HORIZONTAL]     = STRINGID_FOGISDEEP,
    [WEATHER_VOLCANIC_ASH]       = STRINGID_ITISRAINING,
    [WEATHER_SANDSTORM]          = STRINGID_SANDSTORMISRAGING,
    [WEATHER_FOG_DIAGONAL]       = STRINGID_FOGISDEEP,
    [WEATHER_UNDERWATER]         = STRINGID_ITISRAINING,
    [WEATHER_SHADE]              = STRINGID_ITISRAINING,
    [WEATHER_DROUGHT]            = STRINGID_SUNLIGHTISHARSH,
    [WEATHER_DOWNPOUR]           = STRINGID_ITISRAINING,
    [WEATHER_UNDERWATER_BUBBLES] = STRINGID_ITISRAINING,
    [WEATHER_ABNORMAL]           = STRINGID_ITISRAINING
};

const u16 gTerrainStartsStringIds[] =
{
    [B_MSG_TERRAIN_SET_MISTY]    = STRINGID_MISTSWIRLSAROUND,
    [B_MSG_TERRAIN_SET_ELECTRIC] = STRINGID_ELECTRICCURRENTISRUNNING,
    [B_MSG_TERRAIN_SET_PSYCHIC]  = STRINGID_SEEMSWEIRD,
    [B_MSG_TERRAIN_SET_GRASSY]   = STRINGID_ISCOVEREDWITHGRASS,
};

const u16 gPrimalWeatherBlocksStringIds[] =
{
    [B_MSG_PRIMAL_WEATHER_FIZZLED_BY_RAIN]      = STRINGID_MOVEFIZZLEDOUTINTHEHEAVYRAIN,
    [B_MSG_PRIMAL_WEATHER_EVAPORATED_IN_SUN]    = STRINGID_MOVEEVAPORATEDINTHEHARSHSUNLIGHT,
};

const u16 gInobedientStringIds[] =
{
    [B_MSG_LOAFING]            = STRINGID_PKMNLOAFING,
    [B_MSG_WONT_OBEY]          = STRINGID_PKMNWONTOBEY,
    [B_MSG_TURNED_AWAY]        = STRINGID_PKMNTURNEDAWAY,
    [B_MSG_PRETEND_NOT_NOTICE] = STRINGID_PKMNPRETENDNOTNOTICE,
    [B_MSG_INCAPABLE_OF_POWER] = STRINGID_PKMNINCAPABLEOFPOWER
};

const u16 gSafariReactionStringIds[NUM_SAFARI_REACTIONS] =
{
    [B_MSG_MON_WATCHING] = STRINGID_PKMNWATCHINGCAREFULLY,
    [B_MSG_MON_ANGRY]    = STRINGID_PKMNANGRY,
    [B_MSG_MON_EATING]   = STRINGID_PKMNEATING
};

const u16 gSafariGetNearStringIds[] =
{
    [B_MSG_CREPT_CLOSER]    = STRINGID_CREPTCLOSER,
    [B_MSG_CANT_GET_CLOSER] = STRINGID_CANTGETCLOSER
};

const u16 gSafariPokeblockResultStringIds[] =
{
    [B_MSG_MON_CURIOUS]    = STRINGID_PKMNCURIOUSABOUTX,
    [B_MSG_MON_ENTHRALLED] = STRINGID_PKMNENTHRALLEDBYX,
    [B_MSG_MON_IGNORED]    = STRINGID_PKMNIGNOREDX
};

const u16 CureStatusBerryEffectStringID[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PKMNSITEMCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PKMNSITEMCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PKMNSITEMHEALEDBURN,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNSITEMDEFROSTEDIT,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNSITEMHEALEDFROSTBITE,
    [B_MSG_CURED_SLEEP] = STRINGID_PKMNSITEMWOKEIT,
    [B_MSG_CURED_CONFUSION] = STRINGID_PKMNSITEMSNAPPEDOUT,
};

const u16 gItemSwapStringIds[] =
{
    [B_MSG_ITEM_SWAP_TAKEN] = STRINGID_PKMNOBTAINEDX,
    [B_MSG_ITEM_SWAP_GIVEN] = STRINGID_PKMNOBTAINEDX2,
    [B_MSG_ITEM_SWAP_BOTH]  = STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] =
{
    [B_MSG_FLASH_FIRE_BOOST]    = STRINGID_PKMNRAISEDFIREPOWERWITH,
    [B_MSG_FLASH_FIRE_NO_BOOST] = STRINGID_PKMNSXMADEITINEFFECTIVE
};

const u16 gCaughtMonStringIds[] =
{
    [B_MSG_SENT_SOMEONES_PC]   = STRINGID_PKMNTRANSFERREDSOMEONESPC,
    [B_MSG_SENT_LANETTES_PC]   = STRINGID_PKMNTRANSFERREDLANETTESPC,
    [B_MSG_SOMEONES_BOX_FULL]  = STRINGID_PKMNBOXSOMEONESPCFULL,
    [B_MSG_LANETTES_BOX_FULL]  = STRINGID_PKMNBOXLANETTESPCFULL,
    [B_MSG_SWAPPED_INTO_PARTY] = STRINGID_PKMNSENTTOPCAFTERCATCH,
};

const u16 gRoomsStringIds[] =
{
    STRINGID_PKMNTWISTEDDIMENSIONS, STRINGID_TRICKROOMENDS,
    STRINGID_SWAPSDEFANDSPDEFOFALLPOKEMON, STRINGID_WONDERROOMENDS,
    STRINGID_HELDITEMSLOSEEFFECTS, STRINGID_MAGICROOMENDS,
    STRINGID_EMPTYSTRING3
};

const u16 gStatusConditionsStringIds[] =
{
    STRINGID_PKMNWASPOISONED, STRINGID_PKMNBADLYPOISONED, STRINGID_PKMNWASBURNED, STRINGID_PKMNWASPARALYZED, STRINGID_PKMNFELLASLEEP, STRINGID_PKMNGOTFROSTBITE
};

const u16 gDamageNonTypesStartStringIds[] =
{
    [B_MSG_TRAPPED_WITH_VINES]  = STRINGID_TEAMTRAPPEDWITHVINES,
    [B_MSG_CAUGHT_IN_VORTEX]    = STRINGID_TEAMCAUGHTINVORTEX,
    [B_MSG_SURROUNDED_BY_FIRE]  = STRINGID_TEAMSURROUNDEDBYFIRE,
    [B_MSG_SURROUNDED_BY_ROCKS] = STRINGID_TEAMSURROUNDEDBYROCKS,
};

const u16 gDamageNonTypesDmgStringIds[] =
{
    [B_MSG_HURT_BY_VINES]        = STRINGID_PKMNHURTBYVINES,
    [B_MSG_HURT_BY_VORTEX]       = STRINGID_PKMNHURTBYVORTEX,
    [B_MSG_BURNING_UP]           = STRINGID_PKMNBURNINGUP,
    [B_MSG_HURT_BY_ROCKS_THROWN] = STRINGID_PKMNHURTBYROCKSTHROWN,
};

const u16 gRemoveHazardsStringIds[] =
{
    [HAZARDS_SPIKES] = STRINGID_SPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STICKY_WEB] = STRINGID_STICKYWEBDISAPPEAREDFROMTEAM,
    [HAZARDS_TOXIC_SPIKES] = STRINGID_TOXICSPIKESDISAPPEAREDFROMTEAM,
    [HAZARDS_STEALTH_ROCK] = STRINGID_STEALTHROCKDISAPPEAREDFROMTEAM,
    [HAZARDS_STEELSURGE] = STRINGID_SHARPSTEELDISAPPEAREDFROMTEAM,
};

const u16 gZenModeStringIds[] =
{
    [B_MSG_ZEN_MODE_TRIGGERED] = STRINGID_ZENMODETRIGGERED,
    [B_MSG_ZEN_MODE_ENDED] = STRINGID_ZENMODEENDED
};

const u16 gCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_SCRCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_SCRCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_SCRCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_SCRCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PKMNWASDEFROSTED,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PKMNFROSTBITEHEALED,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gPartyCureStatusStringIds[] =
{
    [B_MSG_CURED_PARALYSIS] = STRINGID_PARTYCUREDPARALYSIS,
    [B_MSG_CURED_POISON] = STRINGID_PARTYCUREDPOISON,
    [B_MSG_CURED_BURN] = STRINGID_PARTYCUREDBURN,
    [B_MSG_CURED_SLEEP] = STRINGID_PARTYCUREDSLEEP,
    [B_MSG_CURED_FREEZE] = STRINGID_PARTYCUREDFREEZE,
    [B_MSG_CURED_FROSTBITE] = STRINGID_PARTYCUREDFROSTBITE,
    [B_MSG_CURED_CONFUSION] = STRINGID_SCRCUREDCONFUSION,
    [B_MSG_CURED_INFATUATION] = STRINGID_PKMNGOTOVERITSINFATUATION,
    [B_MSG_CURED_TAUNT] = STRINGID_PKMNSHOOKOFFTHETAUNT,
};

const u16 gHurtByStringIds[] =
{
    [B_MSG_HURT] = STRINGID_AFTERMATHDMG,
    [B_MSG_HURT_BY_ITEM] = STRINGID_PKMNHURTSWITH,
};

const u16 gBreakScreensStringIds[] =
{
    [B_MSG_BREAK_REFLECT] = STRINGID_REFLECTWOREOFF,
    [B_MSG_BREAK_LIGHT_SCREEN] = STRINGID_LIGHTSCREENWOREOFF,
    [B_MSG_BREAK_AURORA_VEIL] = STRINGID_AURORAVEILWOREOFF,
};

const u8 gText_PkmnIsEvolving[] = _("{JPN}……おや！？\n{STR_VAR_1}の　ようすが……！");
const u8 gText_CongratsPkmnEvolved[] = _("{JPN}おめでとう！ {STR_VAR_1}は\n{STR_VAR_2}に しんかした！{WAIT_SE}\p");
const u8 gText_PkmnStoppedEvolving[] = _("{JPN}おや？\n{STR_VAR_1}の へんかが とまった!\p");
const u8 gText_EllipsisQuestionMark[] = _("{JPN}......?\p");
const u8 gText_WhatWillPkmnDo[] = _("{JPN}{B_BUFF1}は どうする？");
const u8 gText_WhatWillPkmnDo2[] = _("{JPN}{B_PLAYER_NAME}は どうする？");
const u8 gText_WhatWillWallyDo[] = _("{JPN}ミツルは どうする？");
const u8 gText_LinkStandby[] = _("{JPN}{PAUSE 16}つうしんたいきちゅう……");
const u8 gText_BattleMenu[] = _("{JPN}たたかう{CLEAR_TO 56}バッグ\nポケモン{CLEAR_TO 56}にげる");
const u8 gText_SafariZoneMenu[] = _("{JPN}ボール{CLEAR_TO 56}ポロック\nちかづく{CLEAR_TO 56}にげる");
const u8 gText_SafariZoneMenuFrlg[] = _("{JPN}{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}ボール{CLEAR_TO 56}エサ\nいし{CLEAR_TO 56}にげる");
const u8 gText_MoveInterfacePP[] = _("{JPN}PP ");
const u8 gText_MoveInterfaceType[] = _("{JPN}タイプ/");
const u8 gText_MoveInterfacePpType[] = _("{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}PP\nタイプ/");
const u8 gText_MoveInterfaceDynamicColors[] = _("{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}");
const u8 gText_WhichMoveToForget4[] = _("{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}どの わざを\nわすれますか？");
const u8 gText_BattleYesNoChoice[] = _("{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}はい\nいいえ");
const u8 gText_BattleSwitchWhich[] = _("{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}どのこに\nする？");
const u8 gText_BattleSwitchWhich2[] = _("{JPN}{PALETTE 5}{BACKGROUND DYNAMIC_COLOR5}{TEXT_COLORS DYNAMIC_COLOR4 DYNAMIC_COLOR6 DYNAMIC_COLOR5}");
const u8 gText_BattleSwitchWhich3[] = _("{JPN}{UP_ARROW}");
const u8 gText_BattleSwitchWhich4[] = _("{JPN}{ESCAPE 4}");
const u8 gText_BattleSwitchWhich5[] = _("{JPN}-");
const u8 gText_SafariBalls[] = _("{JPN}サファリボール");
const u8 gText_SafariBallLeft[] = _("{JPN}のこり: $");
const u8 gText_Sleep[] = _("{JPN}ねむり");
const u8 gText_Poison[] = _("{JPN}どく");
const u8 gText_Burn[] = _("{JPN}やけど");
const u8 gText_Paralysis[] = _("{JPN}まひ");
const u8 gText_Ice[] = _("{JPN}こおり");
const u8 gText_Confusion[] = _("{JPN}こんらん");
const u8 gText_Love[] = _("{JPN}メロメロ");
const u8 gText_SpaceAndSpace[] = _("{JPN} と ");
const u8 gText_CommaSpace[] = _("{JPN}, ");
const u8 gText_Space2[] = _("{JPN} ");
const u8 gText_LineBreak[] = _("{JPN}\l");
const u8 gText_NewLine[] = _("{JPN}\n");
const u8 gText_Are[] = _("{JPN}は");
const u8 gText_Are2[] = _("{JPN}は");
const u8 gText_BadEgg[] = _("{JPN}ダメタマゴ");
const u8 gText_BattleWallyName[] = _("{JPN}ミツル");
const u8 gText_Win[] = _("{JPN}{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}かち");
const u8 gText_Loss[] = _("{JPN}{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}まけ");
const u8 gText_Draw[] = _("{JPN}{BACKGROUND TRANSPARENT}{ACCENT TRANSPARENT}ひきわけ");
static const u8 sText_SpaceIs[] = _("{JPN}は");
static const u8 sText_ApostropheS[] = _("{JPN}の");
const u8 gText_BattleTourney[] = _("{JPN}バトルトーナメント");

const u8 *const gRoundsStringTable[DOME_ROUNDS_COUNT] =
{
    [DOME_ROUND1]    = COMPOUND_STRING("{JPN}1かい"),
    [DOME_ROUND2]    = COMPOUND_STRING("{JPN}2かい"),
    [DOME_SEMIFINAL] = COMPOUND_STRING("{JPN}じゅんけっしょう"),
    [DOME_FINAL]     = COMPOUND_STRING("{JPN}けっしょう"),
};

const u8 gText_TheGreatNewHope[] = _("{JPN}きたいの おおがた しんじん!\p");
const u8 gText_WillChampionshipDreamComeTrue[] = _("{JPN}ひがんの はつ ゆうしょう なるか!?\p");
const u8 gText_AFormerChampion[] = _("{JPN}もと チャンピオン!\p");
const u8 gText_ThePreviousChampion[] = _("{JPN}ぜんかい チャンピオン!\p");
const u8 gText_TheUnbeatenChampion[] = _("{JPN}むてきの チャンピオン!\p");
const u8 gText_PlayerMon1Name[] = _("{JPN}{B_PLAYER_MON1_NAME}");
const u8 gText_Vs[] = _("{JPN}VS");
const u8 gText_OpponentMon1Name[] = _("{JPN}{B_OPPONENT_MON1_NAME}");
const u8 gText_Mind[] = _("{JPN}こころ");
const u8 gText_Skill[] = _("{JPN}わざ");
const u8 gText_Body[] = _("{JPN}からだ");
const u8 gText_Judgment[] = _("{JPN}{B_BUFF1}{CLEAR 13}はんてい{CLEAR 13}{B_BUFF2}");
static const u8 sText_TwoTrainersSentPkmn[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}は\n{B_OPPONENT_MON1_NAME}を くりだした！\p{B_TRAINER2_NAME_WITH_CLASS}は\n{B_OPPONENT_MON2_NAME}を くりだした！");
static const u8 sText_Trainer2SentOutPkmn[] = _("{JPN}{B_TRAINER2_NAME_WITH_CLASS}は\n{B_BUFF1}を くりだした！\p");
static const u8 sText_TwoTrainersWantToBattle[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}と\n{B_TRAINER2_NAME_WITH_CLASS}が\lしょうぶを しかけてきた！\p");
static const u8 sText_InGamePartnerSentOutZGoN[] = _("{JPN}{B_PARTNER_NAME_WITH_CLASS}は\n{B_PLAYER_MON2_NAME}を くりだした！\lゆけっ！ {B_PLAYER_MON1_NAME}！");
static const u8 sText_InGamePartnerSentOutNGoZ[] = _("{JPN}{B_PARTNER_NAME_WITH_CLASS}は\n{B_PLAYER_MON1_NAME}を くりだした！\lゆけっ！ {B_PLAYER_MON2_NAME}！");
static const u8 sText_InGamePartnerSentOutPkmn1[] = _("{JPN}{B_PARTNER_NAME_WITH_CLASS}は\n{B_PLAYER_MON1_NAME}を くりだした！");
static const u8 sText_InGamePartnerSentOutPkmn2[] = _("{JPN}{B_PARTNER_NAME_WITH_CLASS}は\n{B_PLAYER_MON2_NAME}を くりだした！");
static const u8 sText_InGamePartnerWithdrewPkmn1[] = _("{JPN}{B_PARTNER_NAME_WITH_CLASS}は\n{B_PLAYER_MON1_NAME}を ひっこめた！");
static const u8 sText_InGamePartnerWithdrewPkmn2[] = _("{JPN}{B_PARTNER_NAME_WITH_CLASS}は\n{B_PLAYER_MON2_NAME}を ひっこめた！");

const u16 gBattlePalaceFlavorTextTable[] =
{
    [B_MSG_GLINT_IN_EYE]   = STRINGID_GLINTAPPEARSINEYE,
    [B_MSG_GETTING_IN_POS] = STRINGID_PKMNGETTINGINTOPOSITION,
    [B_MSG_GROWL_DEEPLY]   = STRINGID_PKMNBEGANGROWLINGDEEPLY,
    [B_MSG_EAGER_FOR_MORE] = STRINGID_PKMNEAGERFORMORE,
};

const u8 *const gRefereeStringsTable[] =
{
    [B_MSG_REF_NOTHING_IS_DECIDED] = COMPOUND_STRING("{JPN}しんぱん『あと 3ターンで けっちゃくが\nつかないと はんていに なります!"),
    [B_MSG_REF_THATS_IT]           = COMPOUND_STRING("{JPN}しんぱん『そこまでーーー!\nこの たいけつは はんていに なります!"),
    [B_MSG_REF_JUDGE_MIND]         = COMPOUND_STRING("{JPN}しんぱん『はんてい その1! 「こころ」!\nせめる きもちを みせたもの!\p"),
    [B_MSG_REF_JUDGE_SKILL]        = COMPOUND_STRING("{JPN}しんぱん『はんてい その2! 「わざ」!\nてきかくに わざを くりだしたもの!\p"),
    [B_MSG_REF_JUDGE_BODY]         = COMPOUND_STRING("{JPN}しんぱん『はんてい その3! 「からだ」!\nあふれる たいりょくを もちえるもの!\p"),
    [B_MSG_REF_PLAYER_WON]         = COMPOUND_STRING("{JPN}しんぱん『はんてい {B_BUFF1} たい {B_BUFF2}\nしょうしゃ！ {B_PLAYER_NAME}の  {B_PLAYER_MON1_NAME}！\p"),
    [B_MSG_REF_OPPONENT_WON]       = COMPOUND_STRING("{JPN}しんぱん『はんてい {B_BUFF1} たい {B_BUFF2}\nしょうしゃ！ {B_TRAINER1_NAME}の  {B_OPPONENT_MON1_NAME}！\p"),
    [B_MSG_REF_DRAW]               = COMPOUND_STRING("{JPN}しんぱん『はんてい 3 たい 3\nひきわけーーー!\p"),
    [B_MSG_REF_COMMENCE_BATTLE]    = COMPOUND_STRING("{JPN}しんぱん『{B_PLAYER_MON1_NAME} VS {B_OPPONENT_MON1_NAME}しょうぶ! はじめーーー!!"),
};

static const u8 sText_Trainer1Fled[] = _( "{PLAY_SE SE_FLEE}{B_TRAINER1_NAME_WITH_CLASS}は\nにげだした！");
static const u8 sText_PlayerLostAgainstTrainer1[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}との\n しょうぶに まけた!");
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("{JPN}{B_TRAINER1_NAME_WITH_CLASS}との\n しょうぶに ひきわけた!");
const u8 gText_RecordBattleToPass[] = _("{JPN}いまの たたかいを フロンティアパスに\nきろく しますか?");
const u8 gText_BattleRecordedOnPass[] = _("{JPN}{B_PLAYER_NAME}の たたかいが\nフロンティアパスに きろく された!");
static const u8 sText_LinkTrainerWantsToBattlePause[] = _("{JPN}{B_LINK_OPPONENT1_NAME}が\nしょうぶを しかけてきた!\p");
static const u8 sText_TwoLinkTrainersWantToBattlePause[] = _("{JPN}{B_LINK_OPPONENT1_NAME}と {B_LINK_OPPONENT2_NAME}が\nしょうぶを しかけてきた！\p");
static const u8 sText_Your1[] = _("{JPN}みかたの");
static const u8 sText_Opposing1[] = _("{JPN}あいての");
static const u8 sText_Your2[] = _("{JPN}みかたの");
static const u8 sText_Opposing2[] = _("{JPN}あいての");
static const u8 sText_EmptyStatus[] = _("{JPN}$$$$$$$");

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_KantoTutorial[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 6,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
    [B_WIN_OAK_OLD_MAN] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 1,
        .speed = 1,
        .fgColor = 2,
        .bgColor = 1,
        .shadowColor = 3,
    },
};

static const struct BattleWindowText sTextOnWindowsInfo_Arena[] =
{
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xF),
        .fontId = FONT_NORMAL,
        .x = 1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 15,
        .color.accent = 15,
        .color.shadow = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 13 : 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = B_SHOW_EFFECTIVENESS != SHOW_EFFECTIVENESS_NEVER ? 15 : 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 1,
        .speed = 0,
        .color.foreground = 12,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0),
        .fontId = FONT_NORMAL,
        .x = 32,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.shadow = 2,
    },
    [ARENA_WIN_PLAYER_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 1,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_VS] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_OPPONENT_NAME] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_MIND] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_SKILL] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_BODY] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TITLE] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NORMAL,
        .x = -1,
        .y = 1,
        .speed = 0,
        .color.foreground = 13,
        .color.background = 14,
        .color.accent = 14,
        .color.shadow = 15,
    },
    [ARENA_WIN_JUDGMENT_TEXT] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 1,
        .speed = 1,
        .color.foreground = 2,
        .color.background = 1,
        .color.accent = 1,
        .color.shadow = 3,
    },
    [B_WIN_MOVE_DESCRIPTION] = {
        .fillValue = PIXEL_FILL(0xE),
        .fontId = FONT_NARROW,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .color.foreground = TEXT_DYNAMIC_COLOR_4,
        .color.background = TEXT_DYNAMIC_COLOR_5,
        .color.accent = TEXT_DYNAMIC_COLOR_5,
        .color.shadow = TEXT_DYNAMIC_COLOR_6,
    },
};

static const struct BattleWindowText *const sBattleTextOnWindowsInfo[] =
{
    [B_WIN_TYPE_NORMAL] = sTextOnWindowsInfo_Normal,
    [B_WIN_TYPE_ARENA]  = sTextOnWindowsInfo_Arena,
    [B_WIN_TYPE_KANTO_TUTORIAL] = sTextOnWindowsInfo_KantoTutorial,
};

static const u8 sRecordedBattleTextSpeeds[] = {8, 4, 1, 0};

void BufferStringBattle(enum StringID stringID, enum BattlerId battler)
{
    s32 i;
    const u8 *stringPtr = NULL;

    gBattleMsgDataPtr = (struct BattleMsgData *)(&gBattleResources->bufferA[battler][4]);
    gLastUsedItem = gBattleMsgDataPtr->lastItem;
    gLastUsedAbility = gBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = gBattleMsgDataPtr->scrActive;
    gBattleStruct->scriptPartyIdx = gBattleMsgDataPtr->bakScriptPartyIdx;
    gBattleStruct->hpScale = gBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = gBattleMsgDataPtr->itemEffectBattler;
    gBattleStruct->stringMoveType = gBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = gBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < BATTLE_TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = gBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = gBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = gBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringID)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                {
                    stringPtr = sText_TwoTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                {
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                    {
                        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                            stringPtr = sText_LinkTrainerWantsToBattle;
                        else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                            stringPtr = sText_TwoTrainersWantToBattle;
                        else if (!(gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS))
                            stringPtr = sText_LinkTrainerWantsToBattlePause;
                        else
                            stringPtr = sText_TwoLinkTrainersWantToBattlePause;
                    }
                    else
                    {
                        stringPtr = sText_TwoLinkTrainersWantToBattle;
                    }
                }
                else
                {
                    if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
                        stringPtr = sText_LinkTrainerWantsToBattlePause;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
                if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1WantsToBattle;
                else if (gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER))
                    stringPtr = sText_TwoTrainersWantToBattle;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = sText_TwoTrainersWantToBattle;
                else
                    stringPtr = sText_Trainer1WantsToBattle;
            }
        }
        else
        {
            if (IsGhostBattleWithoutScope())
                stringPtr = sText_GhostAppearedCantId;
            else if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
                stringPtr = sText_TheGhostAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_LegendaryPkmnAppeared;
            else if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT))))
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_CATCH_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (BattlerIsPlayer(battler) || BattlerIsPlayer(BATTLE_PARTNER(battler))
         || BattlerIsWally(battler) || BattlerIsWally(BATTLE_PARTNER(battler)))
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(BATTLE_PARTNER(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI && (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK || gBattleTypeFlags & BATTLE_TYPE_LINK))
                {
                    if (BattlerIsPlayer(battler))
                        stringPtr = sText_LinkPartnerSentOutPkmn2GoPkmn; // Player is on left
                    else
                        stringPtr = sText_LinkPartnerSentOutPkmn1GoPkmn; // Link Partner on left
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                {
                    if (BattlerIsPlayer(battler) && (battler & BIT_FLANK) == B_FLANK_LEFT)
                        stringPtr = sText_InGamePartnerSentOutZGoN; // Player is on left
                    else
                        stringPtr = sText_InGamePartnerSentOutNGoZ; // Partner on left
                }
                else
                {
                    stringPtr = sText_GoTwoPkmn;
                }
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (IsDoubleBattle() && IsValidForBattle(GetBattlerMon(BATTLE_PARTNER(battler))))
            {
                if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else if (BATTLE_TWO_VS_ONE_OPPONENT)
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
                else if ((gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK && gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) || (gBattleTypeFlags & BATTLE_TYPE_MULTI && (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK || gBattleTypeFlags & BATTLE_TYPE_LINK)))
                    stringPtr = sText_TwoLinkTrainersIntroSendOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS || gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                    stringPtr = sText_TwoTrainersSentPkmn;
                else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler))) // Link Opponent doubles and test opponent
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
            }
            else if (BattlerIsLink(battler) || (BattlerIsRecorded(battler) && BattlerIsOpponent(battler)))
            {
                if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerIntroSendOutPkmn;
            }
            else
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if ((GetBattlerPosition(battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(battler) || BattlerIsWally(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerWithdrewPkmn1;
                else
                    stringPtr = sText_InGamePartnerWithdrewPkmn1;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent A and test opponent
            {
                stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_PkmnThatsEnough;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_PkmnComeBack;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_PkmnOkComeBack;
                else
                    stringPtr = sText_PkmnGoodComeBack;
            }
            else if (BattlerIsPartner(battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerWithdrewPkmn2;
                else
                    stringPtr = sText_InGamePartnerWithdrewPkmn2;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (BattleSideHasTwoTrainers(B_SIDE_OPPONENT))
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2WithdrewPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1WithdrewPkmn;
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if ((GetBattlerPosition(gBattleScripting.battler) & BIT_FLANK) == B_FLANK_LEFT) // battler 0 and 1
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(gBattleScripting.battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerSentOutPkmn1;
                else
                    stringPtr = sText_InGamePartnerSentOutPkmn1;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent A and test opponent
            {
                stringPtr = sText_LinkTrainerSentOutPkmn;
            }
            else // Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn;
            }
        }
        else // battler 2 and 3
        {
            if (BattlerIsPlayer(gBattleScripting.battler)) // Player
            {
                if (*(&gBattleStruct->hpScale) == 0)
                    stringPtr = sText_GoPkmn2;
                else if (*(&gBattleStruct->hpScale) == 1 || IsDoubleBattle())
                    stringPtr = sText_DoItPkmn;
                else if (*(&gBattleStruct->hpScale) == 2)
                    stringPtr = sText_GoForItPkmn;
                else
                    stringPtr = sText_YourFoesWeakGetEmPkmn;
            }
            else if (BattlerIsPartner(gBattleScripting.battler)) // Link or Ingame Partner
            {
                if (BattlerIsLink(gBattleScripting.battler) || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
                    stringPtr = sText_LinkPartnerSentOutPkmn2;
                else
                    stringPtr = sText_InGamePartnerSentOutPkmn2;
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_LINK_OPPONENT
             || gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK) // Link Opponent B and test opponent
            {
                if (BattleSideHasTwoTrainers(B_SIDE_OPPONENT))
                    stringPtr = sText_LinkTrainer2SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS) // Opponent B
            {
                stringPtr = sText_Trainer2SentOutPkmn;
            }
            else // Single trainer double Opponent A
            {
                stringPtr = sText_Trainer1SentOutPkmn2;
            }
        }
        break;
    case STRINGID_USEDMOVE: // Pokemon used a move msg
        if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
         && !IsZMove(gBattleMsgDataPtr->currentMove)
         && !IsMaxMove(gBattleMsgDataPtr->currentMove))
            StringCopy(gBattleTextBuff3, gTypesInfo[*(&gBattleStruct->stringMoveType)].generic);
        else
            StringCopy(gBattleTextBuff3, GetMoveName(gBattleMsgDataPtr->currentMove));
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(BATTLE_PARTNER(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (!(BattlerIsPlayer(battler) || BattlerIsPlayer(BATTLE_PARTNER(battler))) && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                        stringPtr = sText_TwoInGameTrainersDefeated;
                    else
                        stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (TRAINER_BATTLE_PARAM.opponentA == TRAINER_UNION_ROOM)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    case STRINGID_TRAINERSLIDE:
        stringPtr = gBattleStruct->trainerSlideMsg;
        break;
    default: // load a string from the table
        if (stringID >= STRINGID_COUNT)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
            stringPtr = gBattleStringsTable[stringID];
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8 *src)
{
#ifndef NDEBUG
    u32 j, strWidth;
    u32 dstID = BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
    for (j = 1;; j++)
    {
        strWidth = GetStringLineWidth(0, gDisplayedStringBattle, 0, j, sizeof(gDisplayedStringBattle));
        if (strWidth == 0)
            break;
    }
    return dstID;
#else
    return BattleStringExpandPlaceholders(src, gDisplayedStringBattle, sizeof(gDisplayedStringBattle));
#endif
}

static const u8 *TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[8];
    u32 chars1, chars2;
    u8 *statusPtr;

    memcpy(status, sText_EmptyStatus, min(ARRAY_COUNT(status), ARRAY_COUNT(sText_EmptyStatus)));

    statusPtr = status;
    for (i = 0; i < ARRAY_COUNT(status); i++)
    {
        if (*src == EOS) break; // one line required to match -g
        *statusPtr = *src;
        src++;
        statusPtr++;
    }

    chars1 = *(u32 *)(&status[0]);
    chars2 = *(u32 *)(&status[4]);

    for (i = 0; i < ARRAY_COUNT(gStatusConditionStringsTable); i++)
    {
        if (chars1 == *(u32 *)(&gStatusConditionStringsTable[i][0][0])
            && chars2 == *(u32 *)(&gStatusConditionStringsTable[i][0][4]))
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

static void GetBattlerNick(enum BattlerId battler, u8 *dst)
{
    struct Pokemon *illusionMon = GetIllusionMonPtr(battler);
    struct Pokemon *mon = GetBattlerMon(battler);

    if (illusionMon != NULL)
        mon = illusionMon;
    GetMonData(mon, MON_DATA_NICKNAME, dst);
    StringGet_Nickname(dst);
}

#define HANDLE_NICKNAME_STRING_CASE(battler)                            \
    if (!IsOnPlayerSide(battler))                                       \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
    }                                                                   \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;

#define HANDLE_NICKNAME_STRING_LOWERCASE(battler)                       \
    if (!IsOnPlayerSide(battler))                       \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefixLower;                           \
        else                                                            \
            toCpy = sText_WildPkmnPrefixLower;                          \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstID] = *toCpy;                                        \
            dstID++;                                                    \
            toCpy++;                                                    \
        }                                                               \
    }                                                                   \
    GetBattlerNick(battler, text);                                      \
    toCpy = text;

static const u8 *BattleStringGetOpponentNameByTrainerId(u16 trainerId, u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
    {
        u32 i;
        for (i = 0; i < ARRAY_COUNT(gBattleResources->secretBase->trainerName); i++)
            text[i] = gBattleResources->secretBase->trainerName[i];
        text[i] = EOS;
        ConvertInternationalString(text, gBattleResources->secretBase->language);
        toCpy = text;
    }
    else if (trainerId == TRAINER_UNION_ROOM)
    {
        toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_LINK_OPPONENT)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler)].name;
        else
            toCpy = gLinkPlayers[GetBattlerMultiplayerId(battler) & BIT_SIDE].name;
    }
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
    {
        CopyFrontierBrainTrainerName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
    {
        GetFrontierTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
    {
        GetTrainerTowerOpponentName(text);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
    {
        GetTrainerHillTrainerName(text, trainerId);
        toCpy = text;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
    {
        GetEreaderTrainerName(text);
        toCpy = text;
    }
    else
    {
        enum TrainerClassID trainerClass = GetTrainerClassFromId(TRAINER_BATTLE_PARAM.opponentA);

        if (trainerClass == TRAINER_CLASS_RIVAL_EARLY_FRLG || trainerClass == TRAINER_CLASS_RIVAL_LATE_FRLG || trainerClass == TRAINER_CLASS_CHAMPION_FRLG)
            toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
        else
            toCpy = GetTrainerNameFromId(trainerId);
    }

    assertf(DoesStringProperlyTerminate(toCpy, TRAINER_NAME_LENGTH + 1),"Opponent needs a valid name")
    {
        return sText_EmptyString4;
    }

    return toCpy;
}

static const u8 *BattleStringGetOpponentName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_OPPONENT_LEFT:
        toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    case B_POSITION_OPPONENT_RIGHT:
        if (gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI) && !BATTLE_TWO_VS_ONE_OPPONENT)
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, battler);
        else
            toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, battler);
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetPlayerName(u8 *text, enum BattlerId battler)
{
    const u8 *toCpy = NULL;

    switch (GetBattlerPosition(battler))
    {
    case B_POSITION_PLAYER_LEFT:
        if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            toCpy = gLinkPlayers[0].name;
        else
            toCpy = gSaveBlock2Ptr->playerName;
        break;
    case B_POSITION_PLAYER_RIGHT:
        if (((gBattleTypeFlags & BATTLE_TYPE_RECORDED) && !(gBattleTypeFlags & (BATTLE_TYPE_MULTI | BATTLE_TYPE_INGAME_PARTNER)))
            || gTestRunnerEnabled)
        {
            toCpy = gLinkPlayers[0].name;
        }
        else if ((gBattleTypeFlags & BATTLE_TYPE_LINK) && gBattleTypeFlags & (BATTLE_TYPE_RECORDED | BATTLE_TYPE_MULTI))
        {
            toCpy = gLinkPlayers[2].name;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
        {
            GetFrontierTrainerName(text, gPartnerTrainerId);
            toCpy = text;
        }
        else
        {
            toCpy = gSaveBlock2Ptr->playerName;
        }
        break;
    default:
        break;
    }

    return toCpy;
}

static const u8 *BattleStringGetTrainerName(u8 *text, u8 multiplayerId, enum BattlerId battler)
{
    if (IsOnPlayerSide(battler))
        return BattleStringGetPlayerName(text, battler);
    else
        return BattleStringGetOpponentName(text, multiplayerId, battler);
}

static const u8 *BattleStringGetOpponentClassByTrainerId(u16 trainerId)
{
    const u8 *toCpy;

    if (gBattleTypeFlags & BATTLE_TYPE_SECRET_BASE)
        toCpy = gTrainerClasses[GetSecretBaseTrainerClass()].name;
    else if (trainerId == TRAINER_UNION_ROOM)
        toCpy = gTrainerClasses[GetUnionRoomTrainerClass()].name;
    else if (trainerId == TRAINER_FRONTIER_BRAIN)
        toCpy = gTrainerClasses[GetFrontierBrainTrainerClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
        toCpy = gTrainerClasses[GetFrontierOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
        toCpy = gTrainerClasses[GetTrainerTowerOpponentClass()].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
        toCpy = gTrainerClasses[GetTrainerHillOpponentClass(trainerId)].name;
    else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
        toCpy = gTrainerClasses[GetEreaderTrainerClassId()].name;
    else if (trainerId == TRAINER_LINK_OPPONENT)
        toCpy = gTrainerClasses[TRAINER_NONE].name;
    else
        toCpy = gTrainerClasses[GetTrainerClassFromId(trainerId)].name;

    return toCpy;
}

// Ensure the defined length for an item name can contain the full defined length of a berry name.
// This ensures that custom Enigma Berry names will fit in the text buffer at the top of BattleStringExpandPlaceholders.
STATIC_ASSERT(BERRY_NAME_LENGTH + ARRAY_COUNT(sText_BerrySuffix) <= ITEM_NAME_LENGTH, BerryNameTooLong);

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst, u32 dstSize)
{
    u32 dstID = 0; // if they used dstID, why not use srcID as well?
    const u8 *toCpy = NULL;
    u8 text[max(max(max(32, TRAINER_NAME_LENGTH + 1), POKEMON_NAME_LENGTH + 1), ITEM_NAME_LENGTH)];
    u8 *textStart = &text[0];
    u8 multiplayerId;
    u8 fontId = FONT_NORMAL;

    if (gBattleTypeFlags & BATTLE_TYPE_RECORDED_LINK)
        multiplayerId = gRecordedBattleMultiplayerId;
    else
        multiplayerId = GetMultiplayerId();

    // Clear destination first
    while (dstID < dstSize)
    {
        dst[dstID] = EOS;
        dstID++;
    }

    dstID = 0;
    while (*src != EOS)
    {
        toCpy = NULL;

        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            u32 classLength = 0;
            u32 nameLength = 0;
            const u8 *classString;
            const u8 *nameString;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                {
                    toCpy = gBattleTextBuff2;
                }
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                {
                    toCpy = gBattleTextBuff3;
                }
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetBattlerNick(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // Unused, to change into sth else.
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                GetBattlerNick(BATTLE_PARTNER(gBattlerAttacker), text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget)
                break;
            case B_TXT_DEF_NAME: // target name
                GetBattlerNick(gBattlerTarget, text);
                toCpy = text;
                break;
            case B_TXT_DEF_PARTNER_NAME: // partner target name
                GetBattlerNick(BATTLE_PARTNER(gBattlerTarget), text);
                toCpy = text;
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battler name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler)
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (gBattleMsgDataPtr->currentMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->currentMove);
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (gBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT
                 && !IsZMove(gBattleMsgDataPtr->currentMove)
                 && !IsMaxMove(gBattleMsgDataPtr->currentMove))
                    toCpy = gTypesInfo[gBattleStruct->stringMoveType].generic;
                else
                    toCpy = GetMoveName(gBattleMsgDataPtr->originallyUsedMove);
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY_E_READER)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleScripting.multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleScripting.multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilitiesInfo[gLastUsedAbility].name;
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerAttacker]].name;
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattlerTarget]].name;
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gBattleScripting.battler]].name;
                break;
            case B_TXT_EFF_ABILITY: // effect battler ability
                toCpy = gAbilitiesInfo[sBattlerAbilities[gEffectBattler]].name;
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                break;
            case B_TXT_TRAINER1_NAME_WITH_CLASS: // trainer1 name with trainer class
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentA, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(LEFT_FOE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(RIGHT_FOE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_LEFT));
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentA);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_26: // ?
                if (!IsOnPlayerSide(gBattleScripting.battler))
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        toCpy = sText_FoePkmnPrefix;
                    else
                        toCpy = sText_WildPkmnPrefix;
                    while (*toCpy != EOS)
                    {
                        dst[dstID] = *toCpy;
                        dstID++;
                        toCpy++;
                    }
                }
                GetMonData(&GetBattlerParty(gBattleScripting.battler)[gBattleStruct->scriptPartyIdx], MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_PC_LANETTE))
                    toCpy = IS_FRLG ? sText_Bills : sText_Lanettes;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_TRAINER2_CLASS:
                toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                break;
            case B_TXT_TRAINER2_NAME:
                toCpy = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, text, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                break;
            case B_TXT_TRAINER2_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetOpponentNameByTrainerId(TRAINER_BATTLE_PARAM.opponentB, textStart, multiplayerId, GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_WON, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerBLoseText();
                }
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER && gMapHeader.regionMapSectionId == MAPSEC_TRAINER_TOWER_2)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 1);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_HILL)
                {
                    CopyTrainerHillTrainerText(TRAINER_HILL_TEXT_PLAYER_LOST, TRAINER_BATTLE_PARAM.opponentB);
                    toCpy = gStringVar4;
                }
                break;
            case B_TXT_PARTNER_CLASS:
                toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                break;
            case B_TXT_PARTNER_NAME:
                toCpy = BattleStringGetPlayerName(text, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                break;
            case B_TXT_PARTNER_NAME_WITH_CLASS:
                toCpy = textStart;
                classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                while (classString[classLength] != EOS)
                {
                    textStart[classLength] = classString[classLength];
                    classLength++;
                }
                textStart[classLength] = CHAR_SPACE;
                textStart += classLength + 1;
                nameString = BattleStringGetPlayerName(textStart, GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT));
                if (nameString != textStart)
                {
                    while (nameString[nameLength] != EOS)
                    {
                        textStart[nameLength] = nameString[nameLength];
                        nameLength++;
                    }
                    textStart[nameLength] = EOS;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME:
                toCpy = BattleStringGetTrainerName(text, multiplayerId, gBattlerAttacker);
                break;
            case B_TXT_ATK_TRAINER_CLASS:
                switch (GetBattlerPosition(gBattlerAttacker))
                {
                case B_POSITION_PLAYER_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                        toCpy = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                    break;
                case B_POSITION_OPPONENT_LEFT:
                    toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                case B_POSITION_OPPONENT_RIGHT:
                    if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                    else
                        toCpy = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                    break;
                default:
                    break;
                }
                break;
            case B_TXT_ATK_TRAINER_NAME_WITH_CLASS:
                toCpy = textStart;
                if (GetBattlerPosition(gBattlerAttacker) == B_POSITION_PLAYER_LEFT)
                {
                    textStart = StringCopy(textStart, BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker));
                }
                else
                {
                    classString = NULL;
                    switch (GetBattlerPosition(gBattlerAttacker))
                    {
                    case B_POSITION_PLAYER_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                            classString = gTrainerClasses[GetFrontierOpponentClass(gPartnerTrainerId)].name;
                        break;
                    case B_POSITION_OPPONENT_LEFT:
                        classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    case B_POSITION_OPPONENT_RIGHT:
                        if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS && !BATTLE_TWO_VS_ONE_OPPONENT)
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentB);
                        else
                            classString = BattleStringGetOpponentClassByTrainerId(TRAINER_BATTLE_PARAM.opponentA);
                        break;
                    default:
                        break;
                    }
                    classLength = 0;
                    nameLength = 0;
                    while (classString[classLength] != EOS)
                    {
                        textStart[classLength] = classString[classLength];
                        classLength++;
                    }
                    textStart[classLength] = CHAR_SPACE;
                    textStart += 1 + classLength;
                    nameString = BattleStringGetTrainerName(textStart, multiplayerId, gBattlerAttacker);
                    if (nameString != textStart)
                    {
                        while (nameString[nameLength] != EOS)
                        {
                            textStart[nameLength] = nameString[nameLength];
                            nameLength++;
                        }
                        textStart[nameLength] = EOS;
                    }
                }
                break;
            case B_TXT_ATK_TEAM1:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_ATK_TEAM2:
                if (IsOnPlayerSide(gBattlerAttacker))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_DEF_TEAM1:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_DEF_TEAM2:
                if (IsOnPlayerSide(gBattlerTarget))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_EFF_TEAM1:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your1;
                else
                    toCpy = sText_Opposing1;
                break;
            case B_TXT_EFF_TEAM2:
                if (IsOnPlayerSide(gEffectBattler))
                    toCpy = sText_Your2;
                else
                    toCpy = sText_Opposing2;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerAttacker)
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattlerTarget)
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gEffectBattler)
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX2:
                HANDLE_NICKNAME_STRING_LOWERCASE(gBattleScripting.battler)
                break;
            }

            if (toCpy != NULL)
            {
                while (*toCpy != EOS)
                {
                    if (*toCpy == CHAR_SPACE)
                        dst[dstID] = CHAR_NBSP;
                    else
                        dst[dstID] = *toCpy;
                    dstID++;
                    toCpy++;
                }
            }

            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER2_LOSE_TEXT
                || *src == B_TXT_TRAINER1_WIN_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstID] = EXT_CTRL_CODE_BEGIN;
                dstID++;
                dst[dstID] = EXT_CTRL_CODE_PAUSE_UNTIL_PRESS;
                dstID++;
            }
        }
        else
        {
            dst[dstID] = *src;
            dstID++;
        }
        src++;
    }

    dst[dstID] = *src;
    dstID++;

    BreakStringAutomatic(dst, BATTLE_MSG_MAX_WIDTH, BATTLE_MSG_MAX_LINES, fontId, SHOW_SCROLL_PROMPT);

    return dstID;
}

static void IllusionNickHack(enum BattlerId battler, u32 partyId, u8 *dst)
{
    u32 id = PARTY_SIZE;
    struct Pokemon *party = GetBattlerParty(battler);
    struct Pokemon *mon = &party[partyId], *partnerMon;

    if (GetMonAbility(mon) == ABILITY_ILLUSION)
    {
        if (IsBattlerAlive(BATTLE_PARTNER(battler)))
            partnerMon = GetBattlerMon(BATTLE_PARTNER(battler));
        else
            partnerMon = mon;

        id = GetIllusionMonPartyId(party, mon, partnerMon, battler);
    }

    if (id != PARTY_SIZE)
        GetMonData(&party[id], MON_DATA_NICKNAME, dst);
    else
        GetMonData(mon, MON_DATA_NICKNAME, dst);
}

void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcID = 1;
    u32 value = 0;
    u8 nickname[POKEMON_NAME_BUFFER_SIZE];
    u16 hword;

    *dst = EOS;
    while (src[srcID] != B_BUFF_EOS)
    {
        switch (src[srcID])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcID + 1]);
            StringAppend(dst, gBattleStringsTable[hword]);
            srcID += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcID + 1])
            {
            case 1:
                value = src[srcID + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcID + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcID + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcID + 2]);
            srcID += src[srcID + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, GetMoveName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypesInfo[src[srcID + 1]].name);
            srcID += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
        case B_BUFF_MON_NICK_WITH_PREFIX_LOWER: // poke nick with lowercase prefix
            if (!IsOnPlayerSide(src[srcID + 1]))
            {
                if (src[srcID] == B_BUFF_MON_NICK_WITH_PREFIX_LOWER)
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefixLower);
                    else
                        StringAppend(dst, sText_WildPkmnPrefixLower);
                }
                else
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                        StringAppend(dst, sText_FoePkmnPrefix);
                    else
                        StringAppend(dst, sText_WildPkmnPrefix);
                }
            }
            GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, nickname);
            StringGet_Nickname(nickname);
            StringAppend(dst, nickname);
            srcID += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_SPECIES: // species name
            StringCopy(dst, GetSpeciesName(T1_READ_16(&src[srcID + 1])));
            srcID += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (src[srcID + 2] == gBattlerPartyIndexes[src[srcID + 1]])
            {
                GetBattlerNick(src[srcID + 1], dst);
            }
            else if (gBattleScripting.illusionNickHack) // for STRINGID_ENEMYABOUTTOSWITCHPKMN
            {
                gBattleScripting.illusionNickHack = 0;
                IllusionNickHack(src[srcID + 1], src[srcID + 2], dst);
                StringGet_Nickname(dst);
            }
            else
            {
                GetMonData(&GetBattlerParty(src[srcID + 1])[src[srcID + 2]], MON_DATA_NICKNAME, dst);
                StringGet_Nickname(dst);
            }
            srcID += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcID + 1]]);
            srcID += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilitiesInfo[T1_READ_16(&src[srcID + 1])].name);
            srcID += 3;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcID + 1]);
            if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            {
                if (hword == ITEM_ENIGMA_BERRY_E_READER)
                {
                    if (gLinkPlayers[gBattleScripting.multiplayerId].id == gPotentialItemEffectBattler)
                    {
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcID += 3;
            break;
        }
    }
}

void BattlePutTextOnWindow(const u8 *text, u8 windowId)
{
    const struct BattleWindowText *textInfo = sBattleTextOnWindowsInfo[gBattleScripting.windowsType];
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;

    if (windowId & B_WIN_COPYTOVRAM)
    {
        windowId &= ~B_WIN_COPYTOVRAM;
        copyToVram = FALSE;
    }
    else
    {
        FillWindowPixelBuffer(windowId, textInfo[windowId].fillValue);
        copyToVram = TRUE;
    }

    printerTemplate.currentChar = text;
    printerTemplate.type = WINDOW_TEXT_PRINTER;
    printerTemplate.windowId = windowId;
    printerTemplate.fontId = textInfo[windowId].fontId;
    printerTemplate.x = textInfo[windowId].x;
    printerTemplate.y = textInfo[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = textInfo[windowId].letterSpacing;
    printerTemplate.lineSpacing = textInfo[windowId].lineSpacing;
    printerTemplate.color = textInfo[windowId].color;

    if (B_WIN_MOVE_NAME_1 <= windowId && windowId <= B_WIN_MOVE_NAME_4)
    {
        // We cannot check the actual width of the window because
        // B_WIN_MOVE_NAME_1 and B_WIN_MOVE_NAME_3 are 16 wide for
        // Z-move details.
        if (gBattleStruct->zmove.viewing && windowId == B_WIN_MOVE_NAME_1)
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 16 * TILE_WIDTH);
        else
            printerTemplate.fontId = GetFontIdToFit(text, printerTemplate.fontId, printerTemplate.letterSpacing, 8 * TILE_WIDTH);
    }

    if (printerTemplate.x == 0xFF)
    {
        u32 width = GetBattleWindowTemplatePixelWidth(gBattleScripting.windowsType, windowId);
        s32 alignX = GetStringCenterAlignXOffsetWithLetterSpacing(printerTemplate.fontId, printerTemplate.currentChar, width, printerTemplate.letterSpacing);
        printerTemplate.x = printerTemplate.currentX = alignX;
    }

    if (windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED)) || gTestRunnerEnabled || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != B_WIN_OAK_OLD_MAN))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == B_WIN_MSG || windowId == ARENA_WIN_JUDGMENT_TEXT || windowId == B_WIN_OAK_OLD_MAN)
    {
        if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK))
            speed = 1;
        else if (gBattleTypeFlags & BATTLE_TYPE_RECORDED)
            speed = sRecordedBattleTextSpeeds[GetTextSpeedInRecordedBattle()];
        else
            speed = GetPlayerTextSpeedDelay();

        gTextFlags.canABSpeedUpPrint = 1;
    }
    else
    {
        speed = textInfo[windowId].speed;
        gTextFlags.canABSpeedUpPrint = 0;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);

    if (copyToVram)
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
}

void SetPpNumbersPaletteInMoveSelection(enum BattlerId battler)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct *)(&gBattleResources->bufferA[battler][4]);
    const u16 *palPtr = gPPTextPalette;
    u8 var;

    if (!gBattleStruct->zmove.viewing)
        var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[battler]],
                         chooseMoveStruct->maxPp[gMoveSelectionCursor[battler]]);
    else
        var = 3;

    gPlttBufferUnfaded[BG_PLTT_ID(5) + 12] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[BG_PLTT_ID(5) + 11] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 12], &gPlttBufferFaded[BG_PLTT_ID(5) + 12], PLTT_SIZEOF(1));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 11], &gPlttBufferFaded[BG_PLTT_ID(5) + 11], PLTT_SIZEOF(1));
}

u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
    {
        return 3;
    }
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        if (currentPp <= maxPp / 4)
            return 1;
        if (currentPp > maxPp / 2)
            return 3;
    }

    return 0;
}
