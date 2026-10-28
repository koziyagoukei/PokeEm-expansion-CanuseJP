// What passers-by at the Frontier say (CROWD_STAGE_*, CROWD_VOICE_*). One of a stage's lines for
// the voice is picked each time someone is talked to.

struct CrowdLines
{
    const u8 *const *lines;
    u8 count;
};

#define CROWD_LINES(array) {array, ARRAY_COUNT(array)}

// No one rescued yet: everyone shares everything, and is happy. Only the player can't be heard.
static const u8 *const sCrowdLines_NoneRescued_Child[] =
{
    COMPOUND_STRING("{JPN}ぼく 今日 はじめて ここに 来たのに\n道が ぜんぶ 分かるんだ！\pみんなが 来たことが あるからだよ。"),
    COMPOUND_STRING("{JPN}ねえねえ！ きみの 考えてること\nぜんぜん 聞こえないね。\pふしぎ！ 当てっこ しよう！"),
    COMPOUND_STRING("{JPN}けんかって なあに？\nみんな 同じ 気持ちなのに\lどうやって するの？"),
    COMPOUND_STRING("{JPN}おやつの 味が 二つ 分かるよ！\nぼくのと となりの 子の！"),
};

static const u8 *const sCrowdLines_NoneRescued_Man[] =
{
    COMPOUND_STRING("{JPN}ここの 勝負は 楽しいぞ。\n勝っても 負けても\lみんなで 同じだけ 喜べるからな。"),
    COMPOUND_STRING("{JPN}昔の 失敗も だれかが\n笑って 覚えて いてくれる。\pだから もう 恥ずかしく ないんだ。"),
    COMPOUND_STRING("{JPN}君は どこから 来たんだい？\pふつうは 聞かなくても\n分かるんだがなあ。\l君の ことは なぜか 分からない。"),
    COMPOUND_STRING("{JPN}今日の 予定？\nみんなで 決めたから 安心だ。\p迷う 時間が なくて いいぞ。"),
};

static const u8 *const sCrowdLines_NoneRescued_Woman[] =
{
    COMPOUND_STRING("{JPN}みんなと いると あったかいの。\n思い出も 気持ちも\l分け合えるのよ。"),
    COMPOUND_STRING("{JPN}あの子の 好きな 歌が\nいつのまにか 私の 好きな 歌に\lなっていたの。 すてきでしょ？"),
    COMPOUND_STRING("{JPN}あなたの 声だけ\nみんなの 中に 聞こえないわ。\pでも 大丈夫。\nちゃんと 見えて いるもの。"),
    COMPOUND_STRING("{JPN}悩みごと？ そんなの\nみんなで 分ければ すぐ 消えるわ。"),
};

static const u8 *const sCrowdLines_NoneRescued_Elder[] =
{
    COMPOUND_STRING("{JPN}わしの 若い ころの 話を\nみんなが 覚えて おる。\pもう 忘れる 心配も ないのう。"),
    COMPOUND_STRING("{JPN}孫の 笑い声が\nいつでも 胸に 聞こえるんじゃ。\pありがたい ことじゃ。"),
    COMPOUND_STRING("{JPN}ほっほっ。 お前さんは\nずいぶん 静かな 子じゃのう。\p考えが ちっとも 聞こえん。"),
    COMPOUND_STRING("{JPN}昔は ひとりで 悩んだ ものじゃ。\n今は みんなが おるから 楽じゃのう。"),
};

// The first rescue: one voice has gone quiet, though no one feels lonely.
static const u8 *const sCrowdLines_FirstRescue_Child[] =
{
    COMPOUND_STRING("{JPN}さっきね みんなの 声が\nひとつ 聞こえなく なったの。\pかくれんぼ かなあ？"),
    COMPOUND_STRING("{JPN}だれかが 遠くに 行っちゃった\n気が するんだ。\pでも ぼくは さみしくないよ。\nみんなが いるもん。"),
    COMPOUND_STRING("{JPN}ねえ きみ 何か 知ってる？\n……やっぱり きみの 考えは\l聞こえないや。"),
    COMPOUND_STRING("{JPN}海の 向こうから お団子の\nにおいが した 気が するの。\pおいしそう だったなあ。"),
};

static const u8 *const sCrowdLines_FirstRescue_Man[] =
{
    COMPOUND_STRING("{JPN}なあ 今 一人分 静かに\nならなかったか？\pまあ いい。 気のせいだろう。"),
    COMPOUND_STRING("{JPN}どこかで だれかが\n自分の 道を 選んだらしい。\pそんな こと できるのか？"),
    COMPOUND_STRING("{JPN}一人 いなくなっても\nみんなは みんなだ。\p……そうだよな？"),
    COMPOUND_STRING("{JPN}勝負の 話を しても\n一人分 返事が 足りないんだ。\p変な 感じだな。"),
};

static const u8 *const sCrowdLines_FirstRescue_Woman[] =
{
    COMPOUND_STRING("{JPN}みんなの 歌に\n一つ 音が 足りないの。\pすぐ 慣れると 思うけど。"),
    COMPOUND_STRING("{JPN}いなくなった あの人\n寂しく ないのかしら。\p一人で 考えるって\nどんな 気持ちなの？"),
    COMPOUND_STRING("{JPN}あなたが 来てから\nなんだか 風が 変わったわ。\pいい 風か どうかは\nまだ 分からないけど。"),
    COMPOUND_STRING("{JPN}あら いけない。\n一瞬 自分の ことだけ\l考えてたわ。"),
};

static const u8 *const sCrowdLines_FirstRescue_Elder[] =
{
    COMPOUND_STRING("{JPN}ひとつ 灯りが\n消えたような 気が するのう。\p……いや 遠くで\n灯って おるのか。"),
    COMPOUND_STRING("{JPN}旅立った 者が おるようじゃ。\n若い 者は 元気じゃのう。"),
    COMPOUND_STRING("{JPN}お前さんの まわりは\n静かで よいのう。\p久しぶりに 自分の 声が\n聞こえる 気が するわい。"),
    COMPOUND_STRING("{JPN}わしの 若い ころの 思い出に\n知らん 顔が 混ざっとる。\p……まあ 楽しい 思い出なら\nだれのでも よいわい。"),
};

// Two or three rescued: the shared dream frays at the edges, and some wonder about choosing.
static const u8 *const sCrowdLines_FewRescued_Child[] =
{
    COMPOUND_STRING("{JPN}また ひとり 声が 消えたよ。\nみんな どこへ 行くのかな？\pぼくも ついて いきたいな……\nあれ？ なんで そう 思ったんだろ。"),
    COMPOUND_STRING("{JPN}ぼくの 好きな 色 何色だっけ。\nみんなに 聞いたら\l分かるかなあ。"),
    COMPOUND_STRING("{JPN}海の 向こうに みんなと\n離れた 人たちが いるんだって！\pどんな 所かなあ。"),
    COMPOUND_STRING("{JPN}きみと 話すと\nぼくの 言葉が 出てくるんだ。\pへんなの！ でも 楽しい！"),
};

static const u8 *const sCrowdLines_FewRescued_Man[] =
{
    COMPOUND_STRING("{JPN}最近 夢の 中で\n自分の 名前を 呼ばれるんだ。\pみんなの じゃなく\nおれだけの 名前を。"),
    COMPOUND_STRING("{JPN}離れた 連中は\n後悔 してないのかね。\p……いや 気に なってる\nわけじゃ ないんだが。"),
    COMPOUND_STRING("{JPN}みんなの 気持ちが\nたまに ざわざわ するんだ。\p波が 立ってる みたいにな。"),
    COMPOUND_STRING("{JPN}勝負に 負けて\n悔しいと 思ったんだ。\pみんなは 平気なのに\nおれだけ 悔しかった。"),
};

static const u8 *const sCrowdLines_FewRescued_Woman[] =
{
    COMPOUND_STRING("{JPN}同じ 夢を 見て いるはずなのに\n夢の はしっこが ほつれてるの。"),
    COMPOUND_STRING("{JPN}自分で 選ぶって\n怖く ないのかしら。\p……ちょっと だけ\nうらやましいかも。"),
    COMPOUND_STRING("{JPN}あなたは いつも\n自分の 足で 歩いてるのね。\p疲れない？ それとも\nそれが 楽しいの？"),
    COMPOUND_STRING("{JPN}海の 向こうで みんな\n何を 話してるのかしら。\p聞こえないのが\n少し もどかしいわ。"),
};

static const u8 *const sCrowdLines_FewRescued_Elder[] =
{
    COMPOUND_STRING("{JPN}近ごろ 静かに なったのう。\nいなくなった 者の ぶん\l風が よく 通る。"),
    COMPOUND_STRING("{JPN}昔 わしも 自分の 道を\n選んだ ことが あった 気が する。\p……はて どんな 道じゃったか。"),
    COMPOUND_STRING("{JPN}若い 者が 次々と\n目を 覚まして おるようじゃ。\p……目を 覚ます？ わしらは\n起きて おるはず じゃがのう。"),
    COMPOUND_STRING("{JPN}お前さんが 来てから\nみんなの 声が 少し\l遠く なったのう。\pふむ。 悪い 気は せんよ。"),
};

// Four or more rescued, and the way ahead has opened: their words have reached everyone.
static const u8 *const sCrowdLines_PathOpen_Child[] =
{
    COMPOUND_STRING("{JPN}だれかの 声が 聞こえたよ。\n『それでも いっしょに 生きたい』って。\pいっしょ なのに？ へんなの。"),
    COMPOUND_STRING("{JPN}どこかに 新しい 道が\nできたんだって！\pみんな ざわざわ してる。\nぼくも ドキドキ してる！"),
    COMPOUND_STRING("{JPN}ぼく 大きく なったら\n何に なりたいか\l自分で 考えて みたんだ。\pまだ ないしょ！"),
    COMPOUND_STRING("{JPN}きみと ちがう ことを 考えても\nいっしょに 遊べる？\p……そっか！ よかった！"),
};

static const u8 *const sCrowdLines_PathOpen_Man[] =
{
    COMPOUND_STRING("{JPN}みんなの 気持ちが\n一つの 方へ 引っぱられてる。\pまるで だれかが 必死に\n呼び止めて いるみたいだ。"),
    COMPOUND_STRING("{JPN}離れた 連中の 言葉が\n胸に 残って 消えないんだ。\p困難ばかりでも\n他人と 生きたい…… か。"),
    COMPOUND_STRING("{JPN}なあ 君。 違う だれかと\n生きるのって 大変か？\p……そうか。 大変でも\n悪くは ないんだな。"),
    COMPOUND_STRING("{JPN}勝負で 負けた 悔しさを\n今日は 一人で 抱えて みた。\p……案外 悪くないな。"),
};

static const u8 *const sCrowdLines_PathOpen_Woman[] =
{
    COMPOUND_STRING("{JPN}最近 よく 空を 見るの。\n私が 見たい 空を。\pみんなの 空じゃ なくてね。"),
    COMPOUND_STRING("{JPN}だれかが 開いた 道の 先に\n何が あるのかしら。\p怖いけど…… 知りたいわ。"),
    COMPOUND_STRING("{JPN}みんなの 中に いると\n安心 なのに 胸が 苦しいの。\pこれって 何かしら。"),
    COMPOUND_STRING("{JPN}あなたの 好きな ものを\n聞いても いい？\p……ふふ。 私とは 違うのね。\nそれが 今は 楽しいの。"),
};

static const u8 *const sCrowdLines_PathOpen_Elder[] =
{
    COMPOUND_STRING("{JPN}長く 生きて おると\n別れも 出会いも あったわい。\pどちらも わしだけの\n宝物 じゃったのう。"),
    COMPOUND_STRING("{JPN}みんなが 一つに なろうと\nふんばって おる。\p……じゃが 解けかけた 結び目は\nもう 戻らんかも しれんのう。"),
    COMPOUND_STRING("{JPN}目を 覚ました 者たちの 声が\n夜明けの 鳥の ようじゃ。\pわしも そろそろ\n起きる 時間かのう。"),
    COMPOUND_STRING("{JPN}お前さん。 年寄りの\n独り言に 付き合って くれるか。\pこうして 一人で 話すのも\n久しぶりじゃのう。"),
};

// After the ending: the voice calling everyone together is gone. They still dream, but calmly,
// and look forward to waking.
static const u8 *const sCrowdLines_AfterEnding_Child[] =
{
    COMPOUND_STRING("{JPN}みんなの 声が\nしずかに なったよ。\pでも ぼく まだ\nゆめの 中に いるみたい。"),
    COMPOUND_STRING("{JPN}だれかが 『はじめまして』って\n言った 気が するの。\pはじめまして！ ぼくも 言うね！"),
    COMPOUND_STRING("{JPN}おきたら 何して 遊ぼうかな。\nきみも いっしょに 遊ぼうね！"),
    COMPOUND_STRING("{JPN}ゆめの 中で ずっと\nだれかと 遊んでた 気が する。\pだれだったのかな？"),
};

static const u8 *const sCrowdLines_AfterEnding_Man[] =
{
    COMPOUND_STRING("{JPN}みんなを 呼んでいた 声が\nもう 聞こえないんだ。\pなんだか ほっとしたような\nさみしいような……"),
    COMPOUND_STRING("{JPN}まだ 夢の 中みたいだが\n夢から 覚めたら\l君と 勝負したいな。"),
    COMPOUND_STRING("{JPN}あの 声は きっと\n悪気が なかったんだろうな。\pただ みんなと\nいたかった だけなんだ。"),
    COMPOUND_STRING("{JPN}目が 覚めたら まず\n自分の 好きな めしを 食おう。"),
};

static const u8 *const sCrowdLines_AfterEnding_Woman[] =
{
    COMPOUND_STRING("{JPN}朝の においが するの。\n夢の 中なのに 不思議ね。"),
    COMPOUND_STRING("{JPN}だれかが 最後に\nとても 嬉しそう だったの。\pだれかと 話せた って。\n……よかったわね。"),
    COMPOUND_STRING("{JPN}目が 覚めたら\nあなたに お礼を 言いたいわ。\p今は まだ うまく\n言葉に ならないけど。"),
    COMPOUND_STRING("{JPN}みんなと いた 夢も\n悪く なかったわ。\pでも 次は 自分の 目で\n世界を 見たいの。"),
};

static const u8 *const sCrowdLines_AfterEnding_Elder[] =
{
    COMPOUND_STRING("{JPN}長い 夢じゃったのう。\nそろそろ 目覚めの 時間かの。"),
    COMPOUND_STRING("{JPN}わしの 思い出が\nわしの 所へ 帰って きたわい。\pおかえり と 言いたい 気分じゃ。"),
    COMPOUND_STRING("{JPN}ゆっくりで よいぞ。\nわしらも ゆっくり 起きるでな。"),
    COMPOUND_STRING("{JPN}家族旅行 かね？\nいいのう。 楽しんで いきなされ。"),
};

static const struct CrowdLines sCrowdLines[CROWD_STAGE_COUNT][CROWD_VOICE_COUNT] =
{
    [CROWD_STAGE_NONE_RESCUED] =
    {
        [CROWD_VOICE_CHILD] = CROWD_LINES(sCrowdLines_NoneRescued_Child),
        [CROWD_VOICE_MAN]   = CROWD_LINES(sCrowdLines_NoneRescued_Man),
        [CROWD_VOICE_WOMAN] = CROWD_LINES(sCrowdLines_NoneRescued_Woman),
        [CROWD_VOICE_ELDER] = CROWD_LINES(sCrowdLines_NoneRescued_Elder),
    },
    [CROWD_STAGE_FIRST_RESCUE] =
    {
        [CROWD_VOICE_CHILD] = CROWD_LINES(sCrowdLines_FirstRescue_Child),
        [CROWD_VOICE_MAN]   = CROWD_LINES(sCrowdLines_FirstRescue_Man),
        [CROWD_VOICE_WOMAN] = CROWD_LINES(sCrowdLines_FirstRescue_Woman),
        [CROWD_VOICE_ELDER] = CROWD_LINES(sCrowdLines_FirstRescue_Elder),
    },
    [CROWD_STAGE_FEW_RESCUED] =
    {
        [CROWD_VOICE_CHILD] = CROWD_LINES(sCrowdLines_FewRescued_Child),
        [CROWD_VOICE_MAN]   = CROWD_LINES(sCrowdLines_FewRescued_Man),
        [CROWD_VOICE_WOMAN] = CROWD_LINES(sCrowdLines_FewRescued_Woman),
        [CROWD_VOICE_ELDER] = CROWD_LINES(sCrowdLines_FewRescued_Elder),
    },
    [CROWD_STAGE_PATH_OPEN] =
    {
        [CROWD_VOICE_CHILD] = CROWD_LINES(sCrowdLines_PathOpen_Child),
        [CROWD_VOICE_MAN]   = CROWD_LINES(sCrowdLines_PathOpen_Man),
        [CROWD_VOICE_WOMAN] = CROWD_LINES(sCrowdLines_PathOpen_Woman),
        [CROWD_VOICE_ELDER] = CROWD_LINES(sCrowdLines_PathOpen_Elder),
    },
    [CROWD_STAGE_AFTER_ENDING] =
    {
        [CROWD_VOICE_CHILD] = CROWD_LINES(sCrowdLines_AfterEnding_Child),
        [CROWD_VOICE_MAN]   = CROWD_LINES(sCrowdLines_AfterEnding_Man),
        [CROWD_VOICE_WOMAN] = CROWD_LINES(sCrowdLines_AfterEnding_Woman),
        [CROWD_VOICE_ELDER] = CROWD_LINES(sCrowdLines_AfterEnding_Elder),
    },
};
