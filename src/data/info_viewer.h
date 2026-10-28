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

static const u8 sInfoText_TitleFrontierRules[] = _("{JPN}バトルフロンティア");
static const u8 sInfoText_TitleBannedMons[] = _("{JPN}禁止ポケモン 一覧");
static const u8 sInfoText_TitleGimmickHelp[] = _("{JPN}ギミック");
static const u8 sInfoText_TitleDiaryTest[] = _("{JPN}日記");
static const u8 sInfoText_TitleKanjiTest[] = _("{KANJI}対戦 フォント");
static const u8 sInfoText_TitleMonologueTest[] = _("{JPN}独り言");
static const u8 sInfoText_TitleChoiceTest[] = _("{JPN}分岐テスト");

static const u8 sInfoText_FrontierRulesPage1[] = _(
    "{JPN}タマゴは 参加できません。\n"
    "同じ 種類の ポケモンや\n"
    "同じ 持ち物は 選べません。\n"
    "禁止ポケモンの 扱いは\n"
    "ハブで 設定した ルールに\n"
    "したがいます。");

static const u8 sInfoText_FrontierRulesPage2[] = _(
    "{JPN}レベル50では レベル51以上も\n"
    "参加でき 挑戦中だけ\n"
    "レベル50に なります。\n"
    "オープンでは 相手の レベルが\n"
    "こちらに 合わせて 決まります。\n"
    "相手は 最低 レベル60です。");

static const u8 sInfoText_BannedMonsPage1[] = _(
    "{JPN}説明の あとに 対象の\n"
    "ポケモンを すべて 表示します。\n"
    "フォルムちがいは まとめています。\n"
    "禁止ルールでは 参加できません。\n"
    "解禁ルールでは おたがいに\n"
    "対象を 1匹まで 使えます。");

static const u8 sInfoText_FrontierRulesPage3[] = _(
    "{JPN}途中で まけるか リタイアすると\n"
    "連勝は おわります。\n"
    "挑戦中は 技の 変更や\n"
    "リゾートへの ワープは できません。\n"
    "中断は 施設の 案内か\n"
    "メニューの やすむを 使います。");

static const u8 sInfoText_GimmickHelpPage1[] = _(
    "{JPN}使える ギミックは 施設ごとに\n"
    "きまっており おたがいに 同じです。\n"
    "メガには 対応する メガストーン\n"
    "Zワザには 技に あう Zクリスタルが\n"
    "必要です。 専用Zワザには\n"
    "種類と 技の 条件も あります。");

static const u8 sInfoText_GimmickHelpPage2[] = _(
    "{JPN}ダイマックスは 3ターン 続きます。\n"
    "テラスタルは テラタイプに かわります。\n"
    "種類や 持ち物に よっては\n"
    "選べない ギミックも あります。\n"
    "一度 ギミックを 使った 個体は\n"
    "別の ギミックを 重ねられません。");

static const u8 sInfoText_GimmickHelpPage3[] = _(
    "{JPN}ウルトラバーストは にっしょくか\n"
    "げっしょくの ネクロズマに\n"
    "ウルトラネクロZを 持たせて 使います。\n"
    "ギミックの かいすうは バトルごとです。\n"
    "施設で 禁止された ギミックは\n"
    "持ち物を 揃えても 使えません。");

static const u8 sInfoText_DiaryTestPage1[] = _(
    "{JPN}今日は フロンティアで\n"
    "新しい ルールを 読んだ\n\n"
    "難しいけれど\n"
    "おもしろい しくみだった");

static const u8 sInfoText_DiaryTestPage2[] = _(
    "{JPN}次に 来るときは\n"
    "素早い ポケモンを\n"
    "つれてこよう\n\n"
    "メモとして 残しておく");

static const u8 sInfoText_DiaryTestKanji[] = _(
    "{KANJI}攻撃 防御 特性\n"
    "技の力と 速さ\n"
    "命中 状態異常\n"
    "回復 対戦 相手\n"
    "物理と 変化\n"
    "{ENG}ABC 123{JPN} かな");

static const u8 sInfoText_MonologueTestPage1[] = _(
    "{JPN}ここまで 来たんだ\n"
    "もうすこし だけ\n"
    "前に 進んでみよう");

static const u8 sInfoText_MonologueTestPage2[] = _(
    "{JPN}迷っても いい\n"
    "でも 立ち止まったままでは\n"
    "なにも かわらない");

static const u8 sInfoText_ChoiceTestPage1[] = _(
    "{JPN}これは 分岐テストです\n"
    "最後まで 読むと\n"
    "選べる 項目が でます\n\n"
    "選んだ 内容で\n"
    "別の ページへ 進みます");

static const u8 sInfoText_ChoiceTestPage2[] = _(
    "{JPN}Aで 選ぶ\n"
    "Bで 前の 画面へ 戻る\n\n"
    "閉じるを 選ぶと\n"
    "スクリプトへ 戻ります");

static const u8 sInfoText_ChoiceReadDiary[] = _("{JPN}日記を 読む");
static const u8 sInfoText_ChoiceReadMonologue[] = _("{JPN}独り言を 読む");
static const u8 sInfoText_ChoiceBack[] = _("{JPN}戻る");
static const u8 sInfoText_ChoiceClose[] = _("{JPN}閉じる");

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

static const u8 *const sInfoViewerPages_KanjiTest[] =
{
    sInfoText_DiaryTestKanji,
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
    [INFO_KANJI_FONT_TEST] =
    {
        .title = sInfoText_TitleKanjiTest,
        .pages = sInfoViewerPages_KanjiTest,
        .pageCount = ARRAY_COUNT(sInfoViewerPages_KanjiTest),
        .style = &sInfoViewerStyle_Default,
    },
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
