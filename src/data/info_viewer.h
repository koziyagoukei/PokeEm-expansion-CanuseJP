static const u16 sInfoViewerFramePalette_Blue[] =
{
    RGB_WHITE, RGB(6, 6, 10), RGB(28, 28, 31), RGB(14, 17, 25),
    RGB(4, 5, 9), RGB(20, 23, 30), RGB(31, 31, 31), RGB(10, 12, 18),
    RGB(0, 0, 0), RGB(8, 10, 15), RGB(16, 18, 24), RGB(24, 26, 30),
    RGB(31, 31, 31), RGB(5, 7, 12), RGB(12, 15, 22), RGB_BLACK
};

static const struct InfoViewerStyle sInfoViewerStyle_Default =
{
    .windowFill = PIXEL_FILL(TEXT_COLOR_WHITE),
    .framePaletteNum = STD_WINDOW_PALETTE_NUM,
    .framePalette = NULL,
    .textColors = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY },
};

static const struct InfoViewerStyle sInfoViewerStyle_BlueFrame =
{
    .windowFill = PIXEL_FILL(TEXT_COLOR_WHITE),
    .framePaletteNum = INFO_VIEWER_ALT_FRAME_PALETTE_NUM,
    .framePalette = sInfoViewerFramePalette_Blue,
    .textColors = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY },
};

static const u8 sInfoText_TitleFrontierRules[] = _("バトルフロンティア");
static const u8 sInfoText_TitleBannedMons[] = _("きんしポケモン いちらん");
static const u8 sInfoText_TitleGimmickHelp[] = _("ギミック");
static const u8 sInfoText_TitleDiaryTest[] = _("にっき");
static const u8 sInfoText_TitleMonologueTest[] = _("ひとりごと");
static const u8 sInfoText_TitleChoiceTest[] = _("ぶんきテスト");

static const u8 sInfoText_FrontierRulesPage1[] = _(
    "タマゴは さんかできません。\n"
    "おなじ しゅるいの ポケモンや\n"
    "おなじ もちものは えらべません。\n"
    "きんしポケモンの あつかいは\n"
    "ハブで せっていした ルールに\n"
    "したがいます。");

static const u8 sInfoText_FrontierRulesPage2[] = _(
    "レベル50では レベル51いじょうも\n"
    "さんかでき ちょうせんちゅうだけ\n"
    "レベル50に なります。\n"
    "オープンでは あいての レベルが\n"
    "こちらに あわせて きまります。\n"
    "あいては さいてい レベル60です。");

static const u8 sInfoText_BannedMonsPage1[] = _(
    "せつめいの あとに たいしょうの\n"
    "ポケモンを すべて ひょうじします。\n"
    "フォルムちがいは まとめています。\n"
    "きんしルールでは さんかできません。\n"
    "かいきんルールでは おたがいに\n"
    "たいしょうを 1ぴきまで つかえます。");

static const u8 sInfoText_FrontierRulesPage3[] = _(
    "とちゅうで まけるか リタイアすると\n"
    "れんしょうは おわります。\n"
    "ちょうせんちゅうは わざの へんこうや\n"
    "リゾートへの ワープは できません。\n"
    "ちゅうだんは しせつの あんないか\n"
    "メニューの やすむを つかいます。");

static const u8 sInfoText_GimmickHelpPage1[] = _(
    "つかえる ギミックは しせつごとに\n"
    "きまっており おたがいに おなじです。\n"
    "メガには たいおうする メガストーン\n"
    "Zワザには わざに あう Zクリスタルが\n"
    "ひつようです。 せんようZワザには\n"
    "しゅるいと わざの じょうけんも あります。");

static const u8 sInfoText_GimmickHelpPage2[] = _(
    "ダイマックスは 3ターン つづきます。\n"
    "テラスタルは テラタイプに かわります。\n"
    "しゅるいや もちものに よっては\n"
    "えらべない ギミックも あります。\n"
    "いちど ギミックを つかった こたいは\n"
    "べつの ギミックを かさねられません。");

static const u8 sInfoText_GimmickHelpPage3[] = _(
    "ウルトラバーストは にっしょくか\n"
    "げっしょくの ネクロズマに\n"
    "ウルトラネクロZを もたせて つかいます。\n"
    "ギミックの かいすうは バトルごとです。\n"
    "しせつで きんしされた ギミックは\n"
    "もちものを そろえても つかえません。");

static const u8 sInfoText_DiaryTestPage1[] = _(
    "きょうは フロンティアで\n"
    "あたらしい ルールを よんだ\n\n"
    "むずかしいけれど\n"
    "おもしろい しくみだった");

static const u8 sInfoText_DiaryTestPage2[] = _(
    "つぎに くるときは\n"
    "すばやい ポケモンを\n"
    "つれてこよう\n\n"
    "メモとして のこしておく");

static const u8 sInfoText_MonologueTestPage1[] = _(
    "ここまで きたんだ\n"
    "もうすこし だけ\n"
    "まえに すすんでみよう");

static const u8 sInfoText_MonologueTestPage2[] = _(
    "まよっても いい\n"
    "でも たちどまったままでは\n"
    "なにも かわらない");

static const u8 sInfoText_ChoiceTestPage1[] = _(
    "これは ぶんきテストです\n"
    "さいごまで よむと\n"
    "えらべる こうもくが でます\n\n"
    "えらんだ ないようで\n"
    "べつの ページへ すすみます");

static const u8 sInfoText_ChoiceTestPage2[] = _(
    "Aで えらぶ\n"
    "Bで まえの がめんへ もどる\n\n"
    "とじるを えらぶと\n"
    "スクリプトへ もどります");

static const u8 sInfoText_ChoiceReadDiary[] = _("にっきを よむ");
static const u8 sInfoText_ChoiceReadMonologue[] = _("ひとりごとを よむ");
static const u8 sInfoText_ChoiceBack[] = _("もどる");
static const u8 sInfoText_ChoiceClose[] = _("とじる");

static const u8 *const sInfoViewerPages_FrontierRules[] =
{
    sInfoText_FrontierRulesPage1,
    sInfoText_FrontierRulesPage2,
    sInfoText_FrontierRulesPage3,
};

static const u8 *const sInfoViewerPages_BannedMons[] =
{
    sInfoText_BannedMonsPage1,
};

static const u8 *const sInfoViewerPages_GimmickHelp[] =
{
    sInfoText_GimmickHelpPage1,
    sInfoText_GimmickHelpPage2,
    sInfoText_GimmickHelpPage3,
};

static const u8 *const sInfoViewerPages_DiaryTest[] =
{
    sInfoText_DiaryTestPage1,
    sInfoText_DiaryTestPage2,
};

static const u8 *const sInfoViewerPages_MonologueTest[] =
{
    sInfoText_MonologueTestPage1,
    sInfoText_MonologueTestPage2,
};

static const u8 *const sInfoViewerPages_ChoiceTest[] =
{
    sInfoText_ChoiceTestPage1,
    sInfoText_ChoiceTestPage2,
};

static const struct InfoViewerChoice sInfoViewerChoices_DiaryTest[] =
{
    {
        .text = sInfoText_ChoiceBack,
        .nextInfoId = INFO_VIEWER_BACK,
    },
    {
        .text = sInfoText_ChoiceClose,
        .nextInfoId = INFO_VIEWER_CLOSE,
    },
};

static const struct InfoViewerChoice sInfoViewerChoices_ChoiceTest[] =
{
    {
        .text = sInfoText_ChoiceReadDiary,
        .nextInfoId = INFO_DIARY_TEST,
    },
    {
        .text = sInfoText_ChoiceReadMonologue,
        .nextInfoId = INFO_MONOLOGUE_TEST,
    },
    {
        .text = sInfoText_ChoiceClose,
        .nextInfoId = INFO_VIEWER_CLOSE,
    },
};

#include "resort_tips.h"
#include "contest_tips.h"
#include "frontier_tips.h"

static const struct InfoViewerEntry sInfoViewerEntries[INFO_COUNT] =
{
    [INFO_TIPS] = {
        .title = sInfoText_TitleTips,
        .choices = sInfoViewerChoices_Tips,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Tips),
    },
    [INFO_TIPS_FACILITIES] = {
        .title = sInfoText_TitleFacilities,
        .choices = sInfoViewerChoices_Facilities,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Facilities),
    },
    [INFO_TIPS_TOWER] = TIPS_PAGE_ENTRY(Tower),
    [INFO_TIPS_DOME] = TIPS_PAGE_ENTRY(Dome),
    [INFO_TIPS_PALACE] = TIPS_PAGE_ENTRY(Palace),
    [INFO_TIPS_ARENA] = TIPS_PAGE_ENTRY(Arena),
    [INFO_TIPS_FACTORY] = TIPS_PAGE_ENTRY(Factory),
    [INFO_TIPS_PIKE] = TIPS_PAGE_ENTRY(Pike),
    [INFO_TIPS_PYRAMID] = TIPS_PAGE_ENTRY(Pyramid),
    [INFO_TIPS_HUB] = {
        .title = sInfoText_TitleHub,
        .choices = sInfoViewerChoices_Hub,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Hub),
    },
    [INFO_TIPS_HUB_POKEMON] = TIPS_PAGE_ENTRY(HubPokemon),
    [INFO_TIPS_HUB_ITEMS] = TIPS_PAGE_ENTRY(HubItems),
    [INFO_TIPS_HUB_MUSIC] = TIPS_PAGE_ENTRY(HubMusic),
    [INFO_TIPS_HUB_DIFFICULTY] = TIPS_PAGE_ENTRY(HubDifficulty),
    [INFO_TIPS_HUB_BANNED] = TIPS_PAGE_ENTRY(HubBanned),
    [INFO_TIPS_HUB_SUPPORT] = TIPS_PAGE_ENTRY(HubSupport),
    [INFO_TIPS_RESORT] = {
        .title = sInfoText_TitleResort,
        .choices = sInfoViewerChoices_Resort,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Resort),
    },
    [INFO_TIPS_RESORT_OVERVIEW] = TIPS_PAGE_ENTRY(ResortOverview),
    [INFO_TIPS_RESORT_CAMP] = TIPS_PAGE_ENTRY(ResortCamp),
    [INFO_TIPS_RESORT_DINER] = TIPS_PAGE_ENTRY(ResortDiner),
    [INFO_TIPS_RESORT_BATH] = TIPS_PAGE_ENTRY(ResortBath),
    [INFO_TIPS_RESORT_VILLA] = TIPS_PAGE_ENTRY(ResortVilla),
    [INFO_TIPS_RESORT_SHOP] = TIPS_PAGE_ENTRY(ResortShop),
    [INFO_TIPS_RESORT_ROOM] = TIPS_PAGE_ENTRY(ResortRoom),
    [INFO_TIPS_RESORT_DOLLS] = TIPS_PAGE_ENTRY(ResortDolls),
    [INFO_TIPS_CONTEST] = {
        .title = sInfoText_TitleContest,
        .choices = sInfoViewerChoices_Contest,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_Contest),
    },
    [INFO_TIPS_CONTEST_BASICS] = TIPS_PAGE_ENTRY(ContestBasics),
    [INFO_TIPS_CONTEST_PREPARATION] = TIPS_PAGE_ENTRY(ContestPreparation),
    [INFO_TIPS_CONTEST_SELECTION] = TIPS_PAGE_ENTRY(ContestSelection),
    [INFO_TIPS_CONTEST_MEGA] = TIPS_PAGE_ENTRY(ContestMega),
    [INFO_TIPS_CONTEST_Z] = TIPS_PAGE_ENTRY(ContestZ),
    [INFO_TIPS_CONTEST_TERA] = TIPS_PAGE_ENTRY(ContestTera),
    [INFO_TIPS_CONTEST_DYNAMAX] = TIPS_PAGE_ENTRY(ContestDynamax),
    [INFO_TIPS_CONTEST_REWARDS] = TIPS_PAGE_ENTRY(ContestRewards),
    [INFO_FRONTIER_RULES] =
    {
        .title = sInfoText_TitleFrontierRules,
        .pages = sInfoViewerPages_FrontierRules,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_FrontierRules),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_BANNED_MONS] =
    {
        .title = sInfoText_TitleBannedMons,
        .pages = sInfoViewerPages_BannedMons,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_BannedMons),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_GIMMICK_HELP] =
    {
        .title = sInfoText_TitleGimmickHelp,
        .pages = sInfoViewerPages_GimmickHelp,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_GimmickHelp),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_BlueFrame,
    },
    [INFO_DIARY_TEST] =
    {
        .title = sInfoText_TitleDiaryTest,
        .pages = sInfoViewerPages_DiaryTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_DiaryTest),
        .choices = sInfoViewerChoices_DiaryTest,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_DiaryTest),
        .style = &sInfoViewerStyle_Default,
    },
    [INFO_MONOLOGUE_TEST] =
    {
        .title = sInfoText_TitleMonologueTest,
        .pages = sInfoViewerPages_MonologueTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_MonologueTest),
        .choices = NULL,
        .choiceCount = 0,
        .style = &sInfoViewerStyle_BlueFrame,
    },
    [INFO_CHOICE_TEST] =
    {
        .title = sInfoText_TitleChoiceTest,
        .pages = sInfoViewerPages_ChoiceTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_ChoiceTest),
        .choices = sInfoViewerChoices_ChoiceTest,
        .choiceCount = ARRAY_COUNT(sInfoViewerChoices_ChoiceTest),
        .style = &sInfoViewerStyle_BlueFrame,
    },
};
