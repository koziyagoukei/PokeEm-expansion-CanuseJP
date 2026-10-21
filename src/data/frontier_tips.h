static const u8 sInfoText_TitleTips[] = _("TIPS");
static const u8 sInfoText_TitleFacilities[] = _("{JPN}しせつの ルール");
static const u8 sInfoText_TitleTower[] = _("{JPN}バトルタワー");
static const u8 sInfoText_TitleDome[] = _("{JPN}バトルドーム");
static const u8 sInfoText_TitlePalace[] = _("{JPN}バトルパレス");
static const u8 sInfoText_TitleArena[] = _("{JPN}バトルアリーナ");
static const u8 sInfoText_TitleFactory[] = _("{JPN}バトルファクトリー");
static const u8 sInfoText_TitlePike[] = _("{JPN}バトルチューブ");
static const u8 sInfoText_TitlePyramid[] = _("{JPN}バトルピラミッド");

static const u8 sInfoText_AllGimmicks[] = _(
    "{JPN}つかえる ギミック\n"
    "メガシンカ / Zワザ / ダイマックス\n"
    "テラスタル / ウルトラバースト\n\n"
    "それぞれの しゅるい・もちものなどの\n"
    "じょうけんを みたす ひつようが あります。");

static const u8 sInfoText_Tower[] = _(
    "{JPN}1しゅう 7せんの れんぞくバトル。\n"
    "シングルは 3びき ダブルは 4ひき。\n"
    "マルチは ひとり 2ひきずつ えらび\n"
    "パートナーと きょうりょくします。\n"
    "バトルごとに ポケモンは かいふく。\n"
    "どうぐを バッグからは つかえません。");
static const u8 sInfoText_Dome[] = _(
    "{JPN}16にんの トーナメント。\n"
    "4かい かつと ゆうしょうです。\n"
    "3びきで さんかし あいてを みて\n"
    "バトルに だす 2ひきを えらびます。\n"
    "シングルと ダブルが あります。\n"
    "バトルごとに ポケモンは かいふく。");
static const u8 sInfoText_Palace[] = _(
    "{JPN}3びきで 1しゅう 7せん。\n"
    "シングルと ダブルが あります。\n"
    "ポケモンが せいかくと HPに おうじて\n"
    "わざを えらびます。 じぶんでは\n"
    "わざを しじできず いれかえのみ。\n"
    "こうどう できないことも あります。");
static const u8 sInfoText_PalaceGimmicks[] = _(
    "{JPN}つかえる ギミック\n"
    "メガシンカ / テラスタル\n"
    "おたがいの ポケモンが じどうで\n"
    "しようを はんだんします。\n"
    "Zワザ・ダイマックス・\n"
    "ウルトラバーストは つかえません。");
static const u8 sInfoText_Arena[] = _(
    "{JPN}3びきで 1しゅう 7せん。\n"
    "だす じゅんばんを きめて たたかい\n"
    "バトルちゅうは こうたい できません。\n"
    "わざや もちものの こうたいも ふか。\n"
    "ひんしに なったら つぎの ポケモンが\n"
    "じゅんばんに でてきます。");
static const u8 sInfoText_ArenaJudging[] = _(
    "{JPN}3ターンで けっちゃくが つかなければ\n"
    "こころ・わざ・からだで はんてい。\n"
    "こころは せめる しせい\n"
    "わざは わざの せいこう\n"
    "からだは HPの のこりかたを みます。\n"
    "ひきわけは りょうほうが たおれます。");
static const u8 sInfoText_ArenaGimmicks[] = _(
    "{JPN}つかえる ギミック\n"
    "ダイマックス / Zワザ\n"
    "ウルトラバースト\n\n"
    "メガシンカと テラスタルは\n"
    "おたがいに つかえません。");
static const u8 sInfoText_Factory[] = _(
    "{JPN}レンタルポケモンで 1しゅう 7せん。\n"
    "さいしょに 6ぴきから 3びきを えらび\n"
    "シングルか ダブルで たたかいます。\n"
    "かつと あいての ポケモン 1ぴきと\n"
    "じぶんの 1ぴきを こうかん できます。\n"
    "じぶんの てもちでは さんかしません。");
static const u8 sInfoText_Pike[] = _(
    "{JPN}3びきで 14の へやを すすみます。\n"
    "3つの みちから えらび ゴールへ。\n"
    "バトル・かいふく・じょうたいいじょう\n"
    "など さまざまな できごとが おきます。\n"
    "いつでも かいふくは できません。\n"
    "バッグの どうぐも つかえません。");
static const u8 sInfoText_Pyramid[] = _(
    "{JPN}どうぐを もたない 3びきで さんか。\n"
    "7かいの めいろを のぼります。\n"
    "やせいと トレーナーの バトルがあり\n"
    "かつと みえる はんいが ひろがります。\n"
    "ひろった どうぐを バトルバッグにいれ\n"
    "かいふくなどに つかいましょう。");
static const u8 sInfoText_PyramidGimmicks[] = _(
    "{JPN}つかえる ギミック\n"
    "Zワザ / テラスタル\n"
    "Zクリスタルは なかで ひろえます。\n"
    "メガシンカ・ダイマックス・\n"
    "ウルトラバーストは つかえません。\n"
    "ちゅうだんには やすむを えらびます。");

static const u8 *const sInfoViewerPages_Tower[] = {sInfoText_Tower, sInfoText_AllGimmicks};
static const u8 *const sInfoViewerPages_Dome[] = {sInfoText_Dome, sInfoText_AllGimmicks};
static const u8 *const sInfoViewerPages_Palace[] = {sInfoText_Palace, sInfoText_PalaceGimmicks};
static const u8 *const sInfoViewerPages_Arena[] = {sInfoText_Arena, sInfoText_ArenaJudging, sInfoText_ArenaGimmicks};
static const u8 *const sInfoViewerPages_Factory[] = {sInfoText_Factory, sInfoText_AllGimmicks};
static const u8 *const sInfoViewerPages_Pike[] = {sInfoText_Pike, sInfoText_AllGimmicks};
static const u8 *const sInfoViewerPages_Pyramid[] = {sInfoText_Pyramid, sInfoText_PyramidGimmicks};

static const struct InfoViewerChoice sInfoViewerChoices_Tips[] = {
    {COMPOUND_STRING("{JPN}バトルの きほんルール"), INFO_FRONTIER_RULES},
    {sInfoText_TitleFacilities, INFO_TIPS_FACILITIES},
    {COMPOUND_STRING("{JPN}ギミックの きほん"), INFO_GIMMICK_HELP},
    {sInfoText_TitleBannedMons, INFO_BANNED_MONS},
    {sInfoText_ChoiceClose, INFO_VIEWER_CLOSE},
};
static const struct InfoViewerChoice sInfoViewerChoices_Facilities[] = {
    {sInfoText_TitleTower, INFO_TIPS_TOWER},
    {sInfoText_TitleDome, INFO_TIPS_DOME},
    {sInfoText_TitlePalace, INFO_TIPS_PALACE},
    {sInfoText_TitleArena, INFO_TIPS_ARENA},
    {sInfoText_TitleFactory, INFO_TIPS_FACTORY},
    {sInfoText_TitlePike, INFO_TIPS_PIKE},
    {sInfoText_TitlePyramid, INFO_TIPS_PYRAMID},
    {sInfoText_ChoiceBack, INFO_VIEWER_BACK},
};

#define TIPS_FACILITY_ENTRY(name) { .title = sInfoText_Title##name, \
    .pages = sInfoViewerPages_##name, .pageCount = ARRAY_COUNT(sInfoViewerPages_##name) }
