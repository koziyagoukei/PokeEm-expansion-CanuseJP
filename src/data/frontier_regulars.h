// What the facility attendants add after their welcome, once a visit (CROWD_STAGE_*); one of a
// stage's lines at random.
#define STAFF_ASIDE_LINES 3

static const u8 *const sStaffAsideLines[CROWD_STAGE_COUNT][STAFF_ASIDE_LINES] =
{
    [CROWD_STAGE_NONE_RESCUED] =
    {
        COMPOUND_STRING("{JPN}今日も 大勢の 方が\n見に 来て いますよ。\pみんな 同じ 気持ちで\nあなたを 応援して います！"),
        COMPOUND_STRING("{JPN}あなたの 勝ち負けを\nみんなで 味わえたら\l素敵 なのですが……\pあなたの 気持ちは\nなぜか 伝わって こないのです。"),
        COMPOUND_STRING("{JPN}挑戦者の 皆さんの 記録は\n私たち みんなで\l覚えて います。\p忘れる 心配は ありませんよ！"),
    },
    [CROWD_STAGE_FIRST_RESCUE] =
    {
        COMPOUND_STRING("{JPN}そういえば 最近\n挑戦者の 方が お一人\l見えなく なりました。\pきっと どこかで\nお元気 なのでしょうね。"),
        COMPOUND_STRING("{JPN}おかしいですね。\n受付の 名簿が 一行\l空いて いるような……\pいえ 何でも ありません！"),
        COMPOUND_STRING("{JPN}あなたが 勝つと\n少しだけ 胸が ざわつくのです。\p……どうしてでしょうね。"),
    },
    [CROWD_STAGE_FEW_RESCUED] =
    {
        COMPOUND_STRING("{JPN}近ごろ 施設の 中が\n少し 静かに なりました。\pお客様の 声が\n減ったような……"),
        COMPOUND_STRING("{JPN}私も いつか 自分で\n何かを 選んで みたい……\pなんて 思って しまいました。\n失礼 いたしました！"),
        COMPOUND_STRING("{JPN}海の 向こうへ 渡った\n挑戦者の 方々は\lお元気 でしょうか。"),
    },
    [CROWD_STAGE_PATH_OPEN] =
    {
        COMPOUND_STRING("{JPN}どこかで 新しい 道が\n開いたと 聞きました。\pみんな 落ち着かない ようです。\nもちろん 私も……"),
        COMPOUND_STRING("{JPN}受付が 言う ことでは\nないかも しれませんが……\pあなたの 挑戦を\n私 自身が 楽しみに しています。"),
        COMPOUND_STRING("{JPN}みんなの 気持ちが\nどこかへ 引っぱられて いる\lそんな 気が するのです。\pどうか お気をつけて。"),
    },
    [CROWD_STAGE_AFTER_ENDING] =
    {
        COMPOUND_STRING("{JPN}なんだか 長い 夢から\n覚めかけて いる 気分です。\pそれでも 受付は\nしっかり 務めますよ！"),
        COMPOUND_STRING("{JPN}ゆっくりで いいので\nみんなが 目を 覚ませるよう\l祈って います。"),
        COMPOUND_STRING("{JPN}ご家族で お越しですか？\nどうぞ ごゆっくり\l楽しんで いって ください！"),
    },
};

// The regulars (REGULAR_*), by REGULAR_PHASE_*.
static const u8 *const sRegularLines[REGULAR_COUNT][REGULAR_PHASE_COUNT] =
{
    [REGULAR_WEST_CHEER_WOMAN] =
    {
        COMPOUND_STRING("{JPN}あの人 こっちを 見たわ！\n私の 応援が 聞こえたのね！"),
        COMPOUND_STRING("{JPN}応援の 声が\n前より 小さく なった 気が するの。\pでも 私は 今日も\n大きな 声で 応援するわ！"),
        COMPOUND_STRING("{JPN}ねえ 聞いて。\n今日は 私の 好きな 人を\l私の 声で 応援したの！"),
    },
    [REGULAR_WEST_CHEER_BOY] =
    {
        COMPOUND_STRING("{JPN}ぼくも 同じこと 思った！\nねえ 次は いっしょに 応援しよう！"),
        COMPOUND_STRING("{JPN}あれ？ ぼく 今\nちがう こと 思ってた。\pでも いっしょに 応援は\nできるよね！"),
        COMPOUND_STRING("{JPN}ぼくは ぼくの 好きな 人を\n応援する！\pおねえさんは だれを\n応援するの？"),
    },
    [REGULAR_WEST_CHEER_OLD_MAN] =
    {
        COMPOUND_STRING("{JPN}だったら わしも まぜなさい！\nみんなの 声なら よく 通るぞ！"),
        COMPOUND_STRING("{JPN}ふむ…… 声が そろわん。\pじゃが そろわん 声も\nにぎやかで よいもんじゃ。"),
        COMPOUND_STRING("{JPN}ばらばらの 応援も\nなかなか 味が あるのう！\pわしは わしの 声で\n叫ばせて もらうぞい！"),
    },
    [REGULAR_EAST_TWIN] =
    {
        COMPOUND_STRING("{JPN}パパより 私の ほうが 強いよ！\nパパの 作戦も 分かっちゃうもん！"),
        COMPOUND_STRING("{JPN}パパの 作戦が\nちょっとだけ 分からなく なったの。\pでも その ほうが\n勝負は 楽しいね！"),
        COMPOUND_STRING("{JPN}パパ！ 今度は 作戦を\n教えて くれなくて いいよ。\p自分で 考えて 勝つんだ！"),
    },
    [REGULAR_EAST_TWIN_DAD] =
    {
        COMPOUND_STRING("{JPN}ははは！ パパの 作戦が\n伝わってたら そりゃ 手ごわいな！\pいっしょに 楽しんで おいで！"),
        COMPOUND_STRING("{JPN}この子の 考えてる ことが\nたまに 読めなく なるんだ。\p……成長 したって ことかな？"),
        COMPOUND_STRING("{JPN}娘が 自分で 作戦を\n考えたいって 言うんだ。\pパパは 少し さみしいが\n嬉しくも あるよ。"),
    },
    [REGULAR_EAST_FAN] =
    {
        COMPOUND_STRING("{JPN}あんなに 強い 相手が いたなんて！\nまだ 頭の 中が いっぱいだ！\pどう 戦ったのか みんなで\n何度も 思い返してるんだよ。"),
        COMPOUND_STRING("{JPN}あの 試合の 記憶が\n少しずつ 自分だけの ものに\lなって いく 気が する。\pなんだか 不思議だな。"),
        COMPOUND_STRING("{JPN}あの 試合は\nおれが この目で 見たんだ。\pだれかの 記憶じゃなくて\nおれの 思い出だ！"),
    },
    [REGULAR_EAST_FAN_PARTNER] =
    {
        COMPOUND_STRING("{JPN}この人 強い トレーナーに\n会ってから ずっと この 調子なの。\pとなりに いると 私まで\nその 試合を 見たような 気分よ。"),
        COMPOUND_STRING("{JPN}この人の 話を 聞いても\n前ほど 試合が 浮かばないの。\p……今度は 二人で\n見に 行きましょうか。"),
        COMPOUND_STRING("{JPN}この人 同じ 話ばかり するのよ。\pでも 自分の 言葉で\n話して くれるのが 嬉しいの。"),
    },
    [REGULAR_EAST_MULTI_MANIAC] =
    {
        COMPOUND_STRING("{JPN}きみの 楽しい 気持ちが\nそのまま ぼくの 楽しさに なるんだ。\pいっしょに マルチへ 行こうよ。"),
        COMPOUND_STRING("{JPN}きみの 気持ちが\n少し 分からない 時が あるんだ。\p……だから もっと\n話を したく なるのかな。"),
        COMPOUND_STRING("{JPN}ぼくたち 好きな 技が\nぜんぜん 違うんだって。\pでも 組んだら\nけっこう 強かったよ！"),
    },
    [REGULAR_EAST_MULTI_WOMAN] =
    {
        COMPOUND_STRING("{JPN}そうね！ 同じ 気持ちで\nいっしょに 戦いましょ！\p今日も 明日も\n二人で ここに いたいわね！"),
        COMPOUND_STRING("{JPN}同じ 気持ち じゃなくても\nいっしょに いて いいのかしら。\p……いいわよね。 きっと。"),
        COMPOUND_STRING("{JPN}けんかも したけど\nやっぱり この人と 組みたいの。\p違うから 背中を\n任せられるのね。"),
    },
    [REGULAR_EAST_COOK] =
    {
        COMPOUND_STRING("{JPN}ジグちゃん！ 勝負の あとは\nいっしょに おやつに しようね！"),
        COMPOUND_STRING("{JPN}ジグちゃんが 最近\n違う おやつを ほしがるんだ。\p好みが 変わったのかな？"),
        COMPOUND_STRING("{JPN}ジグちゃんの 好きな おやつと\nぼくの 好きな おやつ。\p両方 作って\n半分こ しようね！"),
    },
    [REGULAR_EXCHANGE_RICH_BOY] =
    {
        COMPOUND_STRING("{JPN}ドールも クッションも ほしいなあ。\n並べる 順番まで 考えちゃう！\pあれ？ この 部屋の 思い出は\nとなりの 子の 部屋 だっけ。"),
        COMPOUND_STRING("{JPN}ママが 選んだ ドールより\nぼくは あっちが いいな。\p……言っても いいのかな？"),
        COMPOUND_STRING("{JPN}ママに 自分で\nほしい ドールを 言えたよ！\pちょっと ドキドキ したけど\n言って よかった！"),
    },
    [REGULAR_EXCHANGE_MOM] =
    {
        COMPOUND_STRING("{JPN}まかせといてって！\pあんたの ほしいもん だったら\nなんでも とって きたるよ！！"),
        COMPOUND_STRING("{JPN}あの子の ほしいもんが\n最近 よう 分からんのよ。\p……ちゃんと 聞いて みよか。"),
        COMPOUND_STRING("{JPN}あの子 ほしいもんを\n自分で 言うてくれたんよ！\pうちとは 好みが 違うけど\nそれも ええもんやね！"),
    },
    [REGULAR_LOUNGE1_BOY_1] =
    {
        COMPOUND_STRING("{JPN}ポケモンを ほめて もらったよ！\pこの子と いっしょに 歩いた 道まで\nほめられた 気分だ！"),
        COMPOUND_STRING("{JPN}ほめられて 嬉しいのは\nぼくだけ みたい。\pとなりの 子は\n別の ことで 喜んでるんだって。"),
        COMPOUND_STRING("{JPN}おじいさんに ほめられた ところ\nぼくが 好きな ところと\l同じ だったんだ！ 嬉しいな！"),
    },
    [REGULAR_LOUNGE1_BOY_2] =
    {
        COMPOUND_STRING("{JPN}ぼくの ポケモンも ほめられた！\nとなりの 子が 嬉しいと\lぼくも 嬉しく なるんだ。"),
        COMPOUND_STRING("{JPN}ぼく 何も 特別な ことは\nしてないのに ほめられたんだ。\pぼくの 何が よかったんだろう？"),
        COMPOUND_STRING("{JPN}この子が ほめられたのは\nこの子が がんばったから だよ！\pぼくも 次は\nほめられるように なるぞ！"),
    },
    [REGULAR_LOUNGE2_MANIAC_1] =
    {
        COMPOUND_STRING("{JPN}センパイの 話を 聞くと\n見てない 試合まで 浮かぶッス！"),
        COMPOUND_STRING("{JPN}センパイの 話が\n最近 浮かばない ッス。\p自分の 目で\n見に 行かないと ッスね！"),
        COMPOUND_STRING("{JPN}自分で 見た 試合の 話を\nセンパイに したッス！\pセンパイ 目を 丸く\nしてたッス！"),
    },
    [REGULAR_LOUNGE2_MANIAC_2] =
    {
        COMPOUND_STRING("{JPN}きみも 同じ 試合を 覚えてる？\nやっぱり あの 場面は よかったよなあ！"),
        COMPOUND_STRING("{JPN}あの 場面 よかったよな！\n……え？ きみは 違う 場面が\l好きなのか？"),
        COMPOUND_STRING("{JPN}好きな 場面が\nみんな 違うんだ。\pだから 感想を 言い合うのが\nこんなに 楽しいんだな！"),
    },
    [REGULAR_LOUNGE2_TRIATHLETE] =
    {
        COMPOUND_STRING("{JPN}あの 人たち 話が ぴったりね。\n私が 知らない ことも\lみんな 知ってる。\p私も まぜて もらおうかしら！"),
        COMPOUND_STRING("{JPN}あの 人たち 最近\n話が かみ合わないみたい。\pでも 前より\n楽しそうに 見えるわ。"),
        COMPOUND_STRING("{JPN}知らない 話を\n知らないって 言えるの\l気持ちが いいわね！\p教えて もらうのも 楽しいわ。"),
    },
    [REGULAR_LOUNGE2_MANIAC_3] =
    {
        COMPOUND_STRING("{JPN}みんなで 感想を 言い合うと\n情報も 増えるんスよね！"),
        COMPOUND_STRING("{JPN}感想が 割れる ことが\n増えたッス。\p情報は 減ったけど\n話は 増えたッス！"),
        COMPOUND_STRING("{JPN}反対の 意見を 聞くと\n試合が もう一回\l見えて くるッスね！"),
    },
    [REGULAR_LOUNGE3_MAN] =
    {
        COMPOUND_STRING("{JPN}ここで 試合の 話を 聞くのも\nけっこう おもしろいんだな。\pそろそろ おれも 行ってくるか！"),
        COMPOUND_STRING("{JPN}予想が 当たる はずなのに\n最近 外れて ばかりだ。\p……でも 外れるのも\n悪くない もんだな。"),
        COMPOUND_STRING("{JPN}勝ち負けが 読めないから\n勝負って おもしろいんだよな！"),
    },
    [REGULAR_LOUNGE4_WOMAN] =
    {
        COMPOUND_STRING("{JPN}テレビの インタビューが 好きなの。\nあの 時の ドキドキが\lよみがえるから。\p何度 見ても あきないわ。"),
        COMPOUND_STRING("{JPN}インタビューで 答えてる 人\nみんな 同じ ことを 言うのね。\p……前は 気に ならなかったのに。"),
        COMPOUND_STRING("{JPN}インタビューの 答えが\nみんな ばらばらで 楽しいわ。\p私だったら 何て\n答えるかしら。"),
    },
    [REGULAR_LOUNGE4_COOK] =
    {
        COMPOUND_STRING("{JPN}東の リゾートに\nめし屋が できたんやて。\pどんな 味やろなあ。\n今度 教えて もらお！"),
        COMPOUND_STRING("{JPN}海の 向こうの 宿で\nうまい だんごを 出しとる\lらしいで。\pどんな 味か 想像 つかんわ。"),
        COMPOUND_STRING("{JPN}わしの 味と\nよその 味は 違うて 当然や。\pせやから 料理は\nおもろいんや！"),
    },
    [REGULAR_LOUNGE4_MAN] =
    {
        COMPOUND_STRING("{JPN}ふう。 ここで ひと休み。\nポケモンも のんびり してるだろ。\p次の 作戦は\nお茶を 飲んでからだ！"),
        COMPOUND_STRING("{JPN}ひと休み してると\n自分の 考えが 浮かぶんだ。\p前は みんなの 考えで\nいっぱい だったのにな。"),
        COMPOUND_STRING("{JPN}お茶の 好みも\n作戦の 好みも 人それぞれ。\pおれは 渋い お茶で\n攻めの 作戦だ！"),
    },
    [REGULAR_LOUNGE5_GENTLEMAN] =
    {
        COMPOUND_STRING("{JPN}あの子は ポケモンの 気持ちを\nそのまま 言葉に できるそうですな。\pどんな ふうに 聞こえるのでしょうね。"),
        COMPOUND_STRING("{JPN}あの子の 言葉と\n私の 感じた ことが\l違う 時が あるのです。\pどちらも 本当 なのでしょうな。"),
        COMPOUND_STRING("{JPN}あの子の 言葉を 聞いて\n自分の ポケモンに\l話しかけて みました。\p返事が 来た 気が しますぞ。"),
    },
    [REGULAR_LOUNGE5_BLACK_BELT] =
    {
        COMPOUND_STRING("{JPN}自分も ポケモンの 気持ちを\nよく 聞きたいで あります！\p分かった つもりでも\n改めて 聞くのは 大切です！"),
        COMPOUND_STRING("{JPN}ポケモンの 気持ちが\n分からない 時が 増えたで あります。\pだからこそ\nよく 聞くで あります！"),
        COMPOUND_STRING("{JPN}ポケモンに 聞いたら\n自分と 違う 答えが\l返って きたで あります！\p押忍！ 勉強に なります！"),
    },
    [REGULAR_LOUNGE5_LITTLE_BOY] =
    {
        COMPOUND_STRING("{JPN}あの子 パレスにも\n遊びに 行くんだ！\pぼくも いっしょに\nついて いこうかな。"),
        COMPOUND_STRING("{JPN}あの子 ひとりで\nどこかへ 行っちゃった。\pついて いって いいか\n聞けば よかったな。"),
        COMPOUND_STRING("{JPN}あの子に いっしょに 行っても\nいいか 聞いたんだ。\p『いいよ』 だって！ やった！"),
    },
    [REGULAR_LOUNGE7_SAILOR] =
    {
        COMPOUND_STRING("{JPN}あの 二人は 言い合いを しても\nいつも となりに いるだろ？\p言葉が なくても 分かるけど\n話すのも 好きなんだろうな。"),
        COMPOUND_STRING("{JPN}あの 二人 最近\n言い合いが 長いんだ。\pでも なんだか\n楽しそうなんだよな。"),
        COMPOUND_STRING("{JPN}あの 二人 好きな 技が\n昔から 違うらしい。\pだから 二人で\n教えて いるんだな。"),
    },
    [REGULAR_LOUNGE7_GENTLEMAN] =
    {
        COMPOUND_STRING("{JPN}あの方たちの 技の キレは\n今も 衰えて いませんな。\p短パン小僧だった ころの 私も\nここで 見とれて いる 気分です。"),
        COMPOUND_STRING("{JPN}子どもの ころの 思い出が\n私 一人の ものに\l戻って きた 気が します。"),
        COMPOUND_STRING("{JPN}あの方たちに 見とれた\n短パン小僧は\l確かに 私 だったのです。\pふふ 照れくさいですな。"),
    },
    [REGULAR_LOUNGE8_NINJA_BOY] =
    {
        COMPOUND_STRING("{JPN}バトルを ほめられたけど\nぼくは コンテストも 好き！\p南に 会場が あるから\nどっちも 行けるんだ！"),
        COMPOUND_STRING("{JPN}みんなは バトルの 方が\nいいって 言うんだ。\pでも ぼく やっぱり\nコンテストも 好き！"),
        COMPOUND_STRING("{JPN}バトルも コンテストも\nぼくが 好きだから やるんだ！\p決めるのは ぼく！"),
    },
    [REGULAR_LOUNGE8_MAN] =
    {
        COMPOUND_STRING("{JPN}知識も 戦略も 勇気も！\nみんなの 分まで ある 気が するぞ！\p……ポケモンの 準備？\nあっ それを 忘れてた！"),
        COMPOUND_STRING("{JPN}みんなの 知識が\n少しずつ 抜けて いくんだ。\pよし！ 自分の 頭で\n覚え直すぞ！"),
        COMPOUND_STRING("{JPN}自分で 覚えた 知識は\n忘れにくいな！\p……ポケモンの 準備も\n今日は 忘れて ないぞ！"),
    },
    [REGULAR_LOUNGE8_WOMAN] =
    {
        COMPOUND_STRING("{JPN}7つの 施設には それぞれ\nフロンティアブレーンが いるの。\p強い だけじゃなくて\nみんな 違う 輝きが……\p……あったのよ。 きっと 今も。"),
        COMPOUND_STRING("{JPN}ブレーンの だれかが\n自分の 名前を 思い出した\lらしいの。\pあの 輝きが\n戻って くるのかしら。"),
        COMPOUND_STRING("{JPN}ブレーンたちの 違う 輝き\nまた 見られる 日が\l来るのね。 楽しみだわ！"),
    },
    [REGULAR_RANKING_HALL_GUIDE] =
    {
        COMPOUND_STRING("{JPN}ここは ランキングホール。\nトレーナーたちの 記録を\lたたえる 場所です。\pあなたの 歩みも\nここに 刻まれると いいですね。"),
        COMPOUND_STRING("{JPN}記録を 見ていると\n名前が 一つずつ 違って\l見える ように なりました。\pみんな 一人一人 だったのですね。"),
        COMPOUND_STRING("{JPN}ここに 刻まれた 記録は\nそれぞれの トレーナーの\l歩んだ 道 そのものです。\pあなたの 記録も\nお待ちして います。"),
    },
};
