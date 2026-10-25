// Story lines for the Frontier Brains and the seventh-battle guests while the
// network has them. Index order follows NAMED_STORY_* (guests, then Brains).

static const u8 sText_NamedCommonWords[] = _("{JPN}みんなと いっしょなら\nなにも こわくない。");
static const u8 sText_NamedUneasy[] = _("{JPN}……いまのは だれの ことばだ？\nわたしは なにを いいたかった？");
static const u8 sText_NamedCommonIntro[] = _("{JPN}……みんなと いっしょなら\nなにも こわくない。\pさあ はじめよう。\nみんなで たのしい しょうぶを。");
static const u8 sText_NamedUneasyIntro[] = _("{JPN}みんなと いっしょなら……\nなにも…… こわく…… ない？\p……おかしいな。 きみとは\nまえにも たたかった きがする。");
static const u8 sText_NamedRescuedChallenge[] = _("{JPN}リゾートで ゆっくり やすめたよ！\nさあ しょうぶを はじめよう！");

struct NamedStoryText
{
    const u8 *name;      // Brains only; guests use their guest entry.
    const u8 *recovered; // In battle, when losing brings the memory back.
    const u8 *visit;     // At the camp, before the kibidango is offered.
    const u8 *cured;     // After eating it.
};

static const struct NamedStoryText sNamedStoryTexts[NAMED_STORY_COUNT] =
{
    // Guests
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ。 わたしは センリ。\nトウカジムの ジムリーダーだ。"),
        .visit = COMPOUND_STRING("{JPN}センリ『きみに まけて\nじぶんの なまえを おもいだした。\pみんなの こころが ながれこむのは\nふしぎと らくだった。\pだが わたしには まもりたい ものが\nある。 じぶんの あしで たちたい。"),
        .cured = COMPOUND_STRING("{JPN}センリ『……あたまが すっきりした。\nありがとう。\pこんどは ジムリーダーとして\nしょうぶを うけて たとう。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}あ……あたし アスナ！\nフエンジムの ジムリーダー！"),
        .visit = COMPOUND_STRING("{JPN}アスナ『あのね！ あたし\nアスナだって おもいだしたの！\pみんなと いっしょだと\nしっぱいしても こわくなかった。\pでも あたしの ほのおは\nあたしが もやしたいの！"),
        .cured = COMPOUND_STRING("{JPN}アスナ『わあ からだが あつい！\nほのおが もどってきた かんじ！\pありがとう！ つぎは\nあたしの ほのおで いくからね！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……おもいだした。\nわたしは マグマだんの マツブサ。"),
        .visit = COMPOUND_STRING("{JPN}マツブサ『フッ…… わたしが\nだれかに すくわれる ことに なるとは。\pすべての こころが ひとつに なる。\nかつての わたしなら のぞんだろう。\pだが みずからの いしで えらんでこそ\nせかいは かわるのだ。"),
        .cured = COMPOUND_STRING("{JPN}マツブサ『……ふむ。 ねつが ひいた。\nれいを いおう。\pわたしの かんがえは わたしが\nまちがえ わたしが ただす。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ああ そうだ。 わたしは ミクリ。\nホウエンの チャンピオンだ。"),
        .visit = COMPOUND_STRING("{JPN}ミクリ『やあ。 きみの しょうぶで\nわたしは わたしに もどれた。\pだれもが おなじ うつくしさを\nみるのも わるくは なかった。\pでも うつくしさは ひとりひとり\nちがうから うつくしいんだ。"),
        .cured = COMPOUND_STRING("{JPN}ミクリ『……すみきった きぶんだ。\nありがとう。\pこんどは わたしだけの\nうつくしい しょうぶを みせよう。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ おれは アオギリ！\nアクアだんの アオギリさまだ！"),
        .visit = COMPOUND_STRING("{JPN}アオギリ『よう！ おまえに まけて\nやっと めが さめたぜ！\pみんなと まざりあう うみは\nたしかに ひろかった。\pだがな おれは おれの ふねで\nうみに でたいんだよ！"),
        .cured = COMPOUND_STRING("{JPN}アオギリ『ガハハ！ からだが かるい！\nたすかったぜ！\pこんどは おれの やりかたで\nしょうぶして やるからな！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ええ。 わたしは アダン。\nルネジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}アダン『ふふ…… あなたの おかげで\nわたしを とりもどせました。\pすべてが とけあう ゆめは\nしずかで ここちよかった。\pですが ひとりで おどるからこそ\nみせられる うつくしさも あるのです。"),
        .cured = COMPOUND_STRING("{JPN}アダン『……ああ。 めが さめました。\nこころから かんしゃを。\pつぎは わたし じしんの\nしょうぶを おみせしましょう。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ わしは ハルヒコ！\nカチヌキファミリーの ちちおやだ！"),
        .visit = COMPOUND_STRING("{JPN}ハルヒコ『いやあ まいった！\nきみに まけて おもいだしたよ！\pかぞく みんなの きもちが\nぜんぶ わかるのは うれしかった。\pでも かぞくは わからない ことを\nはなしあうから たのしいんだ！"),
        .cured = COMPOUND_STRING("{JPN}ハルヒコ『おお なおった！\nありがとう！\pさっそく かぞくに じまん しないとな！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あら わたしは ヤスエ。\nカチヌキファミリーの ははおやよ。"),
        .visit = COMPOUND_STRING("{JPN}ヤスエ『まあ わざわざ ありがとう。\nおかげで おもいだせたわ。\pこどもの きもちが ぜんぶ\nわかるって あんしん だったの。\pでも あのこたちは あのこたち。\nわたしは みまもる ほうが いいわ。"),
        .cured = COMPOUND_STRING("{JPN}ヤスエ『ふう…… すっきりしたわ。\nほんとうに ありがとう。\pこんど うちの こたちと\nあそんで あげてね。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あっ アキだ！\nアキは カチヌキファミリーの アキ！"),
        .visit = COMPOUND_STRING("{JPN}アキ『あっ おねえちゃんかな？\nおにいちゃんかな？ えへへ！\pみんなと いっしょだと\nなんでも わかって たのしかった！\pでも アキ じぶんで かんがえて\nかちたいの！"),
        .cured = COMPOUND_STRING("{JPN}アキ『わあ！ げんき でたよ！\nありがとう！\pつぎは アキの さくせんで\nかつからね！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうそう わたしゃ ミツヨ。\nカチヌキファミリーの おばあちゃん。"),
        .visit = COMPOUND_STRING("{JPN}ミツヨ『おやまあ よく きたねえ。\nおかげで おもいだしたよ。\pみんなと ひとつに なるのは\nこもりうたの ようだったねえ。\pでも ながいきの ひけつはね\nじぶんの あしで あるく ことさ。"),
        .cured = COMPOUND_STRING("{JPN}ミツヨ『ああ からだが かるいよ。\nありがとうねえ。\pまご たちにも よろしく\nいっておくれ。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……おれは リョウヘイ。\nカチヌキファミリーの ちょうなんだ。"),
        .visit = COMPOUND_STRING("{JPN}リョウヘイ『よう。 きみに まけて\nやっと じぶんに もどれたよ。\pかぞくの なかで いちばんじゃなくても\nみんなと いっしょなら へいきだった。\pでも おれは おれとして\nもっと つよく なりたいんだ。"),
        .cured = COMPOUND_STRING("{JPN}リョウヘイ『……よし。 なおった。\nありがとな。\pこんどは かならず かってみせるぜ。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたしは ナギ。\nヒワマキジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}ナギ『こんにちは。 あなたの おかげで\nじぶんを おもいだせました。\pみんなと おなじ かぜに のるのは\nとても おだやかでした。\pでも わたしは わたしの はねで\nそらを とびたいのです。"),
        .cured = COMPOUND_STRING("{JPN}ナギ『……かぜが かわりました。\nありがとうございます。\pつぎは わたしの とりポケモンたちと\nおあいて しますね。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ おれは トウキ！\nムロジムの ジムリーダーだぜ！"),
        .visit = COMPOUND_STRING("{JPN}トウキ『よっ！ おまえに まけて\nめが さめたぜ！\pみんなで おなじ なみに のるのも\nきもち よかったけどさ。\pやっぱ じぶんで えらんだ なみに\nのりたいんだよな！"),
        .cured = COMPOUND_STRING("{JPN}トウキ『うおっ からだが うごく！\nサンキュー！\pつぎは おれの でっかい なみで\nかちに いくぜ！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}フウ『……ぼくは フウ！』\nラン『わたしは ラン！』"),
        .visit = COMPOUND_STRING("{JPN}フウ『ぼくたち おもいだしたよ！』\nラン『あなたに まけた おかげ！』\pフウ『ふたりの こころが ずっと\nつながってるのは なれてた けど……』\pラン『みんなと つながるのは\nちがったの。 わたしは わたし！』"),
        .cured = COMPOUND_STRING("{JPN}フウ『すっきりした！』\nラン『ありがとう！』\pフウ『つぎは ぼくたち ふたりの』\nラン『コンビネーションを みせるね！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……へっ おもいだしたぜ。\nおれは してんのうの カゲツだ。"),
        .visit = COMPOUND_STRING("{JPN}カゲツ『よう。 おまえの おかげで\nおれに もどれたぜ。\pみんなと なかよく するのも\nわるかねえ きぶんだった。\pだがよ くせものは くせもの らしく\nひとりで たってたいのさ。"),
        .cured = COMPOUND_STRING("{JPN}カゲツ『……へっ なおったか。\nかりが できちまったな。\pつぎは えんりょ なしで\nいかせて もらうぜ。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたくしは プリム。\nしてんのうの ひとり ですわ。"),
        .visit = COMPOUND_STRING("{JPN}プリム『ごきげんよう。 あなたの\nしょうぶで めが さめましたわ。\pみなさまと とけあう ぬくもり。\nこおりの わたくしには めずらしくて。\pでも わたくしの こころは\nわたくしだけの ものですわ。"),
        .cured = COMPOUND_STRING("{JPN}プリム『……ひえびえと して\nよい きぶん。 かんしゃしますわ。\pつぎは わたくしの こおりの\nしょうぶを おみせしましょう。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ウ……ウシオだ！\nアクアだんの ウシオだーっ！"),
        .visit = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ おまえに まけて\nおもいだしたぞーっ！\pみんなと いっしょに さわぐのは\nめちゃくちゃ たのしかった！\pでもな おれの おおごえは\nおれが だしたいんだーっ！"),
        .cured = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ げんき ひゃくばい！\nありがとなーっ！\pつぎも ぜんりょくで\nぶつかって いくぞーっ！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうか。 ぼくは ダイゴ。\nいしを あいする トレーナーだ。"),
        .visit = COMPOUND_STRING("{JPN}ダイゴ『やあ。 きみと たたかって\nぼくは ぼくに もどれたよ。\pだれとでも きもちが かよう せかいは\nたしかに やさしかった。\pでも いしは ひとつずつ\nちがうから おもしろいんだ。"),
        .cured = COMPOUND_STRING("{JPN}ダイゴ『……うん。 からだが かるい。\nありがとう。\pこんどは ぼくの はがねの\nしょうぶを みせるよ。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたしは ツツジ。\nカナズミジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}ツツジ『こんにちは。 あなたに まけて\nなまえを おもいだしました。\pみんなの ちしきが ながれこむのは\nべんきょう いらずで らくでした。\pでも わたしは じぶんで まなんで\nわかる ことが すきなんです。"),
        .cured = COMPOUND_STRING("{JPN}ツツジ『……なおりました。\nほんとうに ありがとうございます。\pこんどは わたしの いわタイプで\nしょうぶ しましょうね。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……アタシは イズミ。\nアクアだんの かんぶよ。"),
        .visit = COMPOUND_STRING("{JPN}イズミ『あら きたのね。\nアンタの おかげで おもいだしたわ。\pみんなと いっしょって\nいがいと いごこち よかったのよ。\pでも アタシは アタシの すきに\nいきたいの。 わかるでしょ？"),
        .cured = COMPOUND_STRING("{JPN}イズミ『……ふうん すっきりしたわ。\nありがと。\pつぎは てかげん しないわよ。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ホムラだ！\nマグマだんの かんぶ ホムラだ！"),
        .visit = COMPOUND_STRING("{JPN}ホムラ『おう！ おまえに まけて\nおもいだしたぜ！\pみんなで ひとつの かんがえって\nらくちんでは あったんだがな。\pやっぱ オレは オレの あたまで\nあつく なりたいんだよ！"),
        .cured = COMPOUND_STRING("{JPN}ホムラ『おっ なおったぜ！\nあんがとよ！\pつぎは オレの マグマで\nもやして やるからな！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}ワハハ…… そうじゃ！\nわしは キンセツジムの テッセン！"),
        .visit = COMPOUND_STRING("{JPN}テッセン『ワハハ！ よう きた！\nおかげで おもいだしたわい！\pみんなと でんきが つながってのう\nそれは あたたかかった。\pじゃが わしの ワハハは\nわしが わらいたいのじゃ！"),
        .cured = COMPOUND_STRING("{JPN}テッセン『ワハハ！ しびれるのう！\nありがとうよ！\pつぎも ビリビリ いくぞい！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……カガリ。\n……アタシは カガリ。"),
        .visit = COMPOUND_STRING("{JPN}カガリ『……きた。\n……アンタに まけて おもいだした。\p……みんなの こえ ずっと\nきこえてて…… しずかじゃなかった。\p……アタシは アタシの こえだけ\nききたい。"),
        .cured = COMPOUND_STRING("{JPN}カガリ『……しずか。\n……アハ。 ありがと。\p……つぎは アタシが\nアンタを しらべる ばん。"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あたい フヨウ！\nしてんのうの フヨウだよ！"),
        .visit = COMPOUND_STRING("{JPN}フヨウ『あ きたね！ あたいね\nきみに まけて おもいだしたんだ。\pみんなの きもちが ふわふわ\nよってくるの きらいじゃなかった。\pでも あたいは あたいの ゴーストと\nだけ つながって いたいかな。"),
        .cured = COMPOUND_STRING("{JPN}フヨウ『ふふっ すっきり！\nありがとね！\pつぎは あたいの ゴーストたちと\nあそんで いってよ！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ぼくは ミツル。\nおぼえてる。 ぼくは ミツルだ。"),
        .visit = COMPOUND_STRING("{JPN}ミツル『こんにちは。 きみに まけて\nぼくは ぼくを おもいだしたよ。\pみんなと いっしょだと からだも\nこころも くるしく なかった。\pでも ぼくは じぶんの ちからで\nつよく なりたいんだ。"),
        .cured = COMPOUND_STRING("{JPN}ミツル『……なおった。\nほんとうに ありがとう！\pこんどは ぼくと ポケモンたちの\nちからで しょうぶするね！"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わしは ゲンジ。\nしてんのうの ゲンジじゃ。"),
        .visit = COMPOUND_STRING("{JPN}ゲンジ『よう きたな。 おぬしに\nやぶれて わしを とりもどした。\pみなと ひとつの ふねに のるのは\nあらしも こわく なかった。\pじゃが かじは じぶんで とる。\nそれが ふなのりと いうものよ。"),
        .cured = COMPOUND_STRING("{JPN}ゲンジ『……うむ。 なおったわい。\nれいを いうぞ。\pつぎは わしの ドラゴンたちが\nあいてを しよう。"),
    },
    // Frontier Brains, in FRONTIER_FACILITY_* order
    {
        .name = COMPOUND_STRING("{JPN}リラ"),
        .recovered = COMPOUND_STRING("{JPN}……わたしは リラ。\nバトルタワーの サロンメイド。"),
        .visit = COMPOUND_STRING("{JPN}リラ『……ここが リゾート。\nあなたに まけて おもいだしたの。\pみんなの こころが きこえるのは\nとても あたたかかった。\pそれでも…… わたしは\nわたしで いたい。"),
        .cured = COMPOUND_STRING("{JPN}リラ『……こえが きえた。\nしずかね。 でも わるくない。\pありがとう。 こんどは わたし\nじしんとして たたかいましょう。"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ヒース"),
        .recovered = COMPOUND_STRING("{JPN}……ボクは ヒース！\nドームスーパースター ヒースっ!!"),
        .visit = COMPOUND_STRING("{JPN}ヒース『やあっ!! きみの おかげで\nボクは ボクを おもいだしたよっ!!\pみんなが ボクに はくしゅ してくれる\nせかいは さいこう だったけどさっ！\pスターは じぶんで かがやくから\nスターなのさっ!!"),
        .cured = COMPOUND_STRING("{JPN}ヒース『アッハッハッ!! ふっかつっ!!\nありがとうっ!!\pつぎも ボクの ステージで\nまってるよっ!!"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ウコン"),
        .recovered = COMPOUND_STRING("{JPN}……そうじゃ。 わしは ウコン。\nパレスガーディアンの ウコンじゃ。"),
        .visit = COMPOUND_STRING("{JPN}ウコン『カーッ！ よう きた！\nおぬしの おかげで めが さめたわい。\pポケモンの こころが すべて\nわかる せかいは らくじゃった。\pじゃが わからぬ からこそ\nあゆみよる ものなのじゃ。"),
        .cured = COMPOUND_STRING("{JPN}ウコン『……うむ。 からだが かるい。\nかんしゃ するぞ。\pつぎは わしと ポケモンの\nきずなを みせて やろう。"),
    },
    {
        .name = COMPOUND_STRING("{JPN}コゴミ"),
        .recovered = COMPOUND_STRING("{JPN}……あたし コゴミ！\nアリーナタイクーンの コゴミよ！"),
        .visit = COMPOUND_STRING("{JPN}コゴミ『きたわね！ あたしね\nあなたに まけて おもいだしたの！\pみんなと いっしょなら まけても\nくやしく なかった。\pでも くやしいって おもえるのが\nあたしの こんじょう なのよ！"),
        .cured = COMPOUND_STRING("{JPN}コゴミ『よーし ふっかつ！\nありがとっ！\pつぎは まけないからね！\nかくご しなさい！"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ダツラ"),
        .recovered = COMPOUND_STRING("{JPN}……おれは ダツラ。\nファクトリーヘッドの ダツラだ。"),
        .visit = COMPOUND_STRING("{JPN}ダツラ『よう。 あんたに まけて\nやっと おもいだしたよ。\pみんなの ちしきが ぜんぶ\nあたまに はいってくるのは らくだ。\pでも しらない ことを しるのが\nおれは すきなんだよな。"),
        .cured = COMPOUND_STRING("{JPN}ダツラ『……お なおった。\nたすかったよ。\pこんどは おれの ちしきで\nしょうぶ しようぜ。"),
    },
    {
        .name = COMPOUND_STRING("{JPN}アザミ"),
        .recovered = COMPOUND_STRING("{JPN}……アザミ。\n……チューブクイーン アザミ。"),
        .visit = COMPOUND_STRING("{JPN}アザミ『……きた。\n……まけて おもいだした。\p……みんなと いるのは\nいやじゃ なかった。\p……でも アタシは\nひとりで たつ。"),
        .cured = COMPOUND_STRING("{JPN}アザミ『……なおった。\n……ありがと。\p……つぎは チューブで。"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ジンダイ"),
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ わしは ジンダイ！\nピラミッドキングの ジンダイだ！"),
        .visit = COMPOUND_STRING("{JPN}ジンダイ『おお きたか！ おぬしに\nまけて わしを とりもどしたぞ！\pみなと ひとつなら まよう ことも\nおそれる ことも なかった。\pだが ぼうけんとは じぶんの あしで\nふみだす ゆうきの ことよ！"),
        .cured = COMPOUND_STRING("{JPN}ジンダイ『ガハハ！ ちからが みなぎる！\nれいを いうぞ！\pつぎは わしの ゆうきと\nおぬしの ゆうきの しょうぶだ！"),
    },
};
