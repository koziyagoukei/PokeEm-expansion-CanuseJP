// Story lines for the Frontier Brains and the seventh-battle guests while the
// network has them. Index order follows NAMED_STORY_* (guests, then Brains).

static const u8 sText_NamedCommonWords[] = _("{JPN}みんなと いっしょなら\nなにも こわくない。");
static const u8 sText_NamedUneasy[] = _("{JPN}……いまのは だれの ことばだ？\nわたしは なにを いいたかった？");
static const u8 sText_NamedCommonIntro[] = _("{JPN}……みんなと いっしょなら\nなにも こわくない。\pさあ はじめよう。\nみんなで たのしい しょうぶを。");
static const u8 sText_NamedUneasyIntro[] = _("{JPN}みんなと いっしょなら……\nなにも…… こわく…… ない？\p……おかしいな。 きみとは\nまえにも たたかった きがする。");
static const u8 sText_NamedRescuedChallenge[] = _("{JPN}りょかんで ゆっくり やすめたよ！\nさあ しょうぶを はじめよう！");

struct NamedStoryText
{
    const u8 *name;      // Brains only; guests use their guest entry.
    const u8 *recovered; // In battle, when losing brings the memory back.
    const u8 *visit;     // At the camp, before the kibidango is offered.
    const u8 *cured;     // After eating it.
    const u8 *inn;       // Resting at the inn afterwards.
};

static const struct NamedStoryText sNamedStoryTexts[NAMED_STORY_COUNT] =
{
    // Guests
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ。 わたしは センリ。\nトウカジムの ジムリーダーだ。"),
        .visit = COMPOUND_STRING("{JPN}センリ『きみに まけて\nじぶんの なまえを おもいだした。\pみんなの こころが ながれこむのは\nふしぎと らくだった。\pだが わたしには まもりたい ものが\nある。 じぶんの あしで たちたい。"),
        .cured = COMPOUND_STRING("{JPN}センリ『……あたまが すっきりした。\nありがとう。\pこんどは ジムリーダーとして\nしょうぶを うけて たとう。"),
        .inn = COMPOUND_STRING("{JPN}センリ『りょかんの ふろは いいな。\nあたまが よく まわる。\pジムに もどる まえに\nもういちど きたえなおす つもりだ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}あ……あたし アスナ！\nフエンジムの ジムリーダー！"),
        .visit = COMPOUND_STRING("{JPN}アスナ『あのね！ あたし\nアスナだって おもいだしたの！\pみんなと いっしょだと\nしっぱいしても こわくなかった。\pでも あたしの ほのおは\nあたしが もやしたいの！"),
        .cured = COMPOUND_STRING("{JPN}アスナ『わあ からだが あつい！\nほのおが もどってきた かんじ！\pありがとう！ つぎは\nあたしの ほのおで いくからね！"),
        .inn = COMPOUND_STRING("{JPN}アスナ『ここの おんせん さいこう！\nフエンの おゆと いい しょうぶ！\pあたし ここで ほのおの\nれんしゅう しなおすんだ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……おもいだした。\nわたしは マグマだんの マツブサ。"),
        .visit = COMPOUND_STRING("{JPN}マツブサ『フッ…… わたしが\nだれかに すくわれる ことに なるとは。\pすべての こころが ひとつに なる。\nかつての わたしなら のぞんだろう。\pだが みずからの いしで えらんでこそ\nせかいは かわるのだ。"),
        .cured = COMPOUND_STRING("{JPN}マツブサ『……ふむ。 ねつが ひいた。\nれいを いおう。\pわたしの かんがえは わたしが\nまちがえ わたしが ただす。"),
        .inn = COMPOUND_STRING("{JPN}マツブサ『……しずかな へやだ。\nかんがえごとには ちょうど いい。\pだいちの ことも ひとの ことも\nじぶんの あたまで\lかんがえなおすと しよう。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ああ そうだ。 わたしは ミクリ。\nホウエンの チャンピオンだ。"),
        .visit = COMPOUND_STRING("{JPN}ミクリ『やあ。 きみの しょうぶで\nわたしは わたしに もどれた。\pだれもが おなじ うつくしさを\nみるのも わるくは なかった。\pでも うつくしさは ひとりひとり\nちがうから うつくしいんだ。"),
        .cured = COMPOUND_STRING("{JPN}ミクリ『……すみきった きぶんだ。\nありがとう。\pこんどは わたしだけの\nうつくしい しょうぶを みせよう。"),
        .inn = COMPOUND_STRING("{JPN}ミクリ『まどから みえる うみが\nうつくしい。\pつながりの なかの うつくしさは\nだれの ものでも なかった。\pこれからは わたしの めで\nえらんでいくよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ おれは アオギリ！\nアクアだんの アオギリさまだ！"),
        .visit = COMPOUND_STRING("{JPN}アオギリ『よう！ おまえに まけて\nやっと めが さめたぜ！\pみんなと まざりあう うみは\nたしかに ひろかった。\pだがな おれは おれの ふねで\nうみに でたいんだよ！"),
        .cured = COMPOUND_STRING("{JPN}アオギリ『ガハハ！ からだが かるい！\nたすかったぜ！\pこんどは おれの やりかたで\nしょうぶして やるからな！"),
        .inn = COMPOUND_STRING("{JPN}アオギリ『ガハハ！ うみの みえる\nやどとは いい しゅみだ！\pしばらく ここで\nおれの ゆめを ねりなおすぜ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ええ。 わたしは アダン。\nルネジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}アダン『ふふ…… あなたの おかげで\nわたしを とりもどせました。\pすべてが とけあう ゆめは\nしずかで ここちよかった。\pですが ひとりで おどるからこそ\nみせられる うつくしさも あるのです。"),
        .cured = COMPOUND_STRING("{JPN}アダン『……ああ。 めが さめました。\nこころから かんしゃを。\pつぎは わたし じしんの\nしょうぶを おみせしましょう。"),
        .inn = COMPOUND_STRING("{JPN}アダン『……おちついた へやですね。\nこころが すみわたります。\pルネに もどる まえに\nもういちど みずの こころを\lみがきます。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ わしは ハルヒコ！\nカチヌキファミリーの ちちおやだ！"),
        .visit = COMPOUND_STRING("{JPN}ハルヒコ『いやあ まいった！\nきみに まけて おもいだしたよ！\pかぞく みんなの きもちが\nぜんぶ わかるのは うれしかった。\pでも かぞくは わからない ことを\nはなしあうから たのしいんだ！"),
        .cured = COMPOUND_STRING("{JPN}ハルヒコ『おお なおった！\nありがとう！\pさっそく かぞくに じまん しないとな！"),
        .inn = COMPOUND_STRING("{JPN}ハルヒコ『ここの めしは うまい！\nかぞく みんなで きたいもんだ！\pもちろん しょうぶも\nかぞく そうでで いくぞ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あら わたしは ヤスエ。\nカチヌキファミリーの ははおやよ。"),
        .visit = COMPOUND_STRING("{JPN}ヤスエ『まあ わざわざ ありがとう。\nおかげで おもいだせたわ。\pこどもの きもちが ぜんぶ\nわかるって あんしん だったの。\pでも あのこたちは あのこたち。\nわたしは みまもる ほうが いいわ。"),
        .cured = COMPOUND_STRING("{JPN}ヤスエ『ふう…… すっきりしたわ。\nほんとうに ありがとう。\pこんど うちの こたちと\nあそんで あげてね。"),
        .inn = COMPOUND_STRING("{JPN}ヤスエ『ゆっくり やすめて いるわ。\nこどもたちの かおが みたいわね。\pみんなが じぶんの あしで\nあるいて こられるように\lわたしも がんばらなくちゃ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あっ アキだ！\nアキは カチヌキファミリーの アキ！"),
        .visit = COMPOUND_STRING("{JPN}アキ『あっ おねえちゃんかな？\nおにいちゃんかな？ えへへ！\pみんなと いっしょだと\nなんでも わかって たのしかった！\pでも アキ じぶんで かんがえて\nかちたいの！"),
        .cured = COMPOUND_STRING("{JPN}アキ『わあ！ げんき でたよ！\nありがとう！\pつぎは アキの さくせんで\nかつからね！"),
        .inn = COMPOUND_STRING("{JPN}アキ『りょかんって ひろーい！\nかくれんぼ したく なっちゃう！\pでも いまは アキの さくせん\nかんがえてるの！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうそう わたしゃ ミツヨ。\nカチヌキファミリーの おばあちゃん。"),
        .visit = COMPOUND_STRING("{JPN}ミツヨ『おやまあ よく きたねえ。\nおかげで おもいだしたよ。\pみんなと ひとつに なるのは\nこもりうたの ようだったねえ。\pでも ながいきの ひけつはね\nじぶんの あしで あるく ことさ。"),
        .cured = COMPOUND_STRING("{JPN}ミツヨ『ああ からだが かるいよ。\nありがとうねえ。\pまご たちにも よろしく\nいっておくれ。"),
        .inn = COMPOUND_STRING("{JPN}ミツヨ『ありがたいねえ。\nあったかい おふろに ごはん。\pまごたちの ぶんまで\nゆっくり させて もらうよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……おれは リョウヘイ。\nカチヌキファミリーの ちょうなんだ。"),
        .visit = COMPOUND_STRING("{JPN}リョウヘイ『よう。 きみに まけて\nやっと じぶんに もどれたよ。\pかぞくの なかで いちばんじゃなくても\nみんなと いっしょなら へいきだった。\pでも おれは おれとして\nもっと つよく なりたいんだ。"),
        .cured = COMPOUND_STRING("{JPN}リョウヘイ『……よし。 なおった。\nありがとな。\pこんどは かならず かってみせるぜ。"),
        .inn = COMPOUND_STRING("{JPN}リョウヘイ『……ここ しずかで いいな。\pみんなの こえが ないと\nじぶんの こえが よく きこえる。\pつぎは おれの さくせんで かつ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたしは ナギ。\nヒワマキジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}ナギ『こんにちは。 あなたの おかげで\nじぶんを おもいだせました。\pみんなと おなじ かぜに のるのは\nとても おだやかでした。\pでも わたしは わたしの はねで\nそらを とびたいのです。"),
        .cured = COMPOUND_STRING("{JPN}ナギ『……かぜが かわりました。\nありがとうございます。\pつぎは わたしの とりポケモンたちと\nおあいて しますね。"),
        .inn = COMPOUND_STRING("{JPN}ナギ『ここは かぜが よく とおります。\pとりポケモンたちも\nのびのびと はねを やすめています。\pわたしも すこしだけ\nそらを みあげて すごします。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ おれは トウキ！\nムロジムの ジムリーダーだぜ！"),
        .visit = COMPOUND_STRING("{JPN}トウキ『よっ！ おまえに まけて\nめが さめたぜ！\pみんなで おなじ なみに のるのも\nきもち よかったけどさ。\pやっぱ じぶんで えらんだ なみに\nのりたいんだよな！"),
        .cured = COMPOUND_STRING("{JPN}トウキ『うおっ からだが うごく！\nサンキュー！\pつぎは おれの でっかい なみで\nかちに いくぜ！"),
        .inn = COMPOUND_STRING("{JPN}トウキ『よう！ ここの まえの うみ\nなかなかの なみが たつぜ！\pからだが なまらない ように\nまいあさ およいでるんだ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}フウ『……ぼくは フウ！』\nラン『わたしは ラン！』"),
        .visit = COMPOUND_STRING("{JPN}フウ『ぼくたち おもいだしたよ！』\nラン『あなたに まけた おかげ！』\pフウ『ふたりの こころが ずっと\nつながってるのは なれてた けど……』\pラン『みんなと つながるのは\nちがったの。 わたしは わたし！』"),
        .cured = COMPOUND_STRING("{JPN}フウ『すっきりした！』\nラン『ありがとう！』\pフウ『つぎは ぼくたち ふたりの』\nラン『コンビネーションを みせるね！』"),
        .inn = COMPOUND_STRING("{JPN}フウ『ここの へや ふたりで ひとつ！』\nラン『でも ベッドは べつべつ！』\pフウ『ちがう ことを かんがえるのも』\nラン『けっこう たのしいね！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……へっ おもいだしたぜ。\nおれは してんのうの カゲツだ。"),
        .visit = COMPOUND_STRING("{JPN}カゲツ『よう。 おまえの おかげで\nおれに もどれたぜ。\pみんなと なかよく するのも\nわるかねえ きぶんだった。\pだがよ くせものは くせもの らしく\nひとりで たってたいのさ。"),
        .cured = COMPOUND_STRING("{JPN}カゲツ『……へっ なおったか。\nかりが できちまったな。\pつぎは えんりょ なしで\nいかせて もらうぜ。"),
        .inn = COMPOUND_STRING("{JPN}カゲツ『……へっ わるくねえ やどだ。\pひとりで ねころがって\nかんがえごとってのも\lたまには いいもんだな。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたくしは プリム。\nしてんのうの ひとり ですわ。"),
        .visit = COMPOUND_STRING("{JPN}プリム『ごきげんよう。 あなたの\nしょうぶで めが さめましたわ。\pみなさまと とけあう ぬくもり。\nこおりの わたくしには めずらしくて。\pでも わたくしの こころは\nわたくしだけの ものですわ。"),
        .cured = COMPOUND_STRING("{JPN}プリム『……ひえびえと して\nよい きぶん。 かんしゃしますわ。\pつぎは わたくしの こおりの\nしょうぶを おみせしましょう。"),
        .inn = COMPOUND_STRING("{JPN}プリム『……ここの よるは\nほどよく ひえて ここちよいですわ。\pひとりの ねむりが\nこんなに しずかだなんて。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ウ……ウシオだ！\nアクアだんの ウシオだーっ！"),
        .visit = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ おまえに まけて\nおもいだしたぞーっ！\pみんなと いっしょに さわぐのは\nめちゃくちゃ たのしかった！\pでもな おれの おおごえは\nおれが だしたいんだーっ！"),
        .cured = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ げんき ひゃくばい！\nありがとなーっ！\pつぎも ぜんりょくで\nぶつかって いくぞーっ！"),
        .inn = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ めしが うまい！\nおかわり 3ばい めだーっ！\pアオギリさまにも\nたべさせて あげたいなーっ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうか。 ぼくは ダイゴ。\nいしを あいする トレーナーだ。"),
        .visit = COMPOUND_STRING("{JPN}ダイゴ『やあ。 きみと たたかって\nぼくは ぼくに もどれたよ。\pだれとでも きもちが かよう せかいは\nたしかに やさしかった。\pでも いしは ひとつずつ\nちがうから おもしろいんだ。"),
        .cured = COMPOUND_STRING("{JPN}ダイゴ『……うん。 からだが かるい。\nありがとう。\pこんどは ぼくの はがねの\nしょうぶを みせるよ。"),
        .inn = COMPOUND_STRING("{JPN}ダイゴ『この あたりの いわ\nめずらしい しまもよう なんだ。\pしばらく ここで いしを\nしらべて すごすよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたしは ツツジ。\nカナズミジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}ツツジ『こんにちは。 あなたに まけて\nなまえを おもいだしました。\pみんなの ちしきが ながれこむのは\nべんきょう いらずで らくでした。\pでも わたしは じぶんで まなんで\nわかる ことが すきなんです。"),
        .cured = COMPOUND_STRING("{JPN}ツツジ『……なおりました。\nほんとうに ありがとうございます。\pこんどは わたしの いわタイプで\nしょうぶ しましょうね。"),
        .inn = COMPOUND_STRING("{JPN}ツツジ『……ここで べんきょうの\nやりなおし ですわ。\pみんなの ちしきで わかった きに\nなっていた ことを\lじぶんで たしかめたいのです。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……アタシは イズミ。\nアクアだんの かんぶよ。"),
        .visit = COMPOUND_STRING("{JPN}イズミ『あら きたのね。\nアンタの おかげで おもいだしたわ。\pみんなと いっしょって\nいがいと いごこち よかったのよ。\pでも アタシは アタシの すきに\nいきたいの。 わかるでしょ？"),
        .cured = COMPOUND_STRING("{JPN}イズミ『……ふうん すっきりしたわ。\nありがと。\pつぎは てかげん しないわよ。"),
        .inn = COMPOUND_STRING("{JPN}イズミ『……ふん。 わるくない へやね。\pしばらくは のんびり\nさせて もらうわよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ホムラだ！\nマグマだんの かんぶ ホムラだ！"),
        .visit = COMPOUND_STRING("{JPN}ホムラ『おう！ おまえに まけて\nおもいだしたぜ！\pみんなで ひとつの かんがえって\nらくちんでは あったんだがな。\pやっぱ オレは オレの あたまで\nあつく なりたいんだよ！"),
        .cured = COMPOUND_STRING("{JPN}ホムラ『おっ なおったぜ！\nあんがとよ！\pつぎは オレの マグマで\nもやして やるからな！"),
        .inn = COMPOUND_STRING("{JPN}ホムラ『おう！ ここの おふろ\nあつくて さいこうだぜ！\pオレの マグマも\nまた たぎって きたぜ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}ワハハ…… そうじゃ！\nわしは キンセツジムの テッセン！"),
        .visit = COMPOUND_STRING("{JPN}テッセン『ワハハ！ よう きた！\nおかげで おもいだしたわい！\pみんなと でんきが つながってのう\nそれは あたたかかった。\pじゃが わしの ワハハは\nわしが わらいたいのじゃ！"),
        .cured = COMPOUND_STRING("{JPN}テッセン『ワハハ！ しびれるのう！\nありがとうよ！\pつぎも ビリビリ いくぞい！"),
        .inn = COMPOUND_STRING("{JPN}テッセン『ワハハ！ ここの でんきは\nちと よわいのう！\pわしが なおして やろうかの！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……カガリ。\n……アタシは カガリ。"),
        .visit = COMPOUND_STRING("{JPN}カガリ『……きた。\n……アンタに まけて おもいだした。\p……みんなの こえ ずっと\nきこえてて…… しずかじゃなかった。\p……アタシは アタシの こえだけ\nききたい。"),
        .cured = COMPOUND_STRING("{JPN}カガリ『……しずか。\n……アハ。 ありがと。\p……つぎは アタシが\nアンタを しらべる ばん。"),
        .inn = COMPOUND_STRING("{JPN}カガリ『……ここ しずか。\n……アハ。 すき。\p……アタシ ここで\nいろいろ しらべてる。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あたい フヨウ！\nしてんのうの フヨウだよ！"),
        .visit = COMPOUND_STRING("{JPN}フヨウ『あ きたね！ あたいね\nきみに まけて おもいだしたんだ。\pみんなの きもちが ふわふわ\nよってくるの きらいじゃなかった。\pでも あたいは あたいの ゴーストと\nだけ つながって いたいかな。"),
        .cured = COMPOUND_STRING("{JPN}フヨウ『ふふっ すっきり！\nありがとね！\pつぎは あたいの ゴーストたちと\nあそんで いってよ！"),
        .inn = COMPOUND_STRING("{JPN}フヨウ『ここの ろうか\nよる なんか でそうで いいよね！\pゴーストたちも\nたのしそうに してるよ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ぼくは ミツル。\nおぼえてる。 ぼくは ミツルだ。"),
        .visit = COMPOUND_STRING("{JPN}ミツル『こんにちは。 きみに まけて\nぼくは ぼくを おもいだしたよ。\pみんなと いっしょだと からだも\nこころも くるしく なかった。\pでも ぼくは じぶんの ちからで\nつよく なりたいんだ。"),
        .cured = COMPOUND_STRING("{JPN}ミツル『……なおった。\nほんとうに ありがとう！\pこんどは ぼくと ポケモンたちの\nちからで しょうぶするね！"),
        .inn = COMPOUND_STRING("{JPN}ミツル『ぼく ここで ゆっくり\nやすませて もらってるんだ。\pからだが よわくても\nぼくは ぼくの ペースで\lつよく なりたいな。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わしは ゲンジ。\nしてんのうの ゲンジじゃ。"),
        .visit = COMPOUND_STRING("{JPN}ゲンジ『よう きたな。 おぬしに\nやぶれて わしを とりもどした。\pみなと ひとつの ふねに のるのは\nあらしも こわく なかった。\pじゃが かじは じぶんで とる。\nそれが ふなのりと いうものよ。"),
        .cured = COMPOUND_STRING("{JPN}ゲンジ『……うむ。 なおったわい。\nれいを いうぞ。\pつぎは わしの ドラゴンたちが\nあいてを しよう。"),
        .inn = COMPOUND_STRING("{JPN}ゲンジ『……うみの においじゃ。\pふねに のっていた ころを\nおもいだすのう。\pドラゴンたちも\nゆっくり やすませて やるわい。』"),
    },
    // Frontier Brains, in FRONTIER_FACILITY_* order
    {
        .name = COMPOUND_STRING("{JPN}リラ"),
        .recovered = COMPOUND_STRING("{JPN}……わたしは リラ。\nバトルタワーの サロンメイド。"),
        .visit = COMPOUND_STRING("{JPN}リラ『……ここが リゾート。\nあなたに まけて おもいだしたの。\pみんなの こころが きこえるのは\nとても あたたかかった。\pそれでも…… わたしは\nわたしで いたい。"),
        .cured = COMPOUND_STRING("{JPN}リラ『……こえが きえた。\nしずかね。 でも わるくない。\pありがとう。 こんどは わたし\nじしんとして たたかいましょう。"),
        .inn = COMPOUND_STRING("{JPN}リラ『……しずかね。\nでも わるくない。\pこの しずけさの なかで\nわたしの こえを さがしているの。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ヒース"),
        .recovered = COMPOUND_STRING("{JPN}……ボクは ヒース！\nドームスーパースター ヒースっ!!"),
        .visit = COMPOUND_STRING("{JPN}ヒース『やあっ!! きみの おかげで\nボクは ボクを おもいだしたよっ!!\pみんなが ボクに はくしゅ してくれる\nせかいは さいこう だったけどさっ！\pスターは じぶんで かがやくから\nスターなのさっ!!"),
        .cured = COMPOUND_STRING("{JPN}ヒース『アッハッハッ!! ふっかつっ!!\nありがとうっ!!\pつぎも ボクの ステージで\nまってるよっ!!"),
        .inn = COMPOUND_STRING("{JPN}ヒース『アッハッハッ!!\nこの りょかんも ボクの ステージっ!!\pおふろあがりの ボクを\nみに きたまえっ!!』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ウコン"),
        .recovered = COMPOUND_STRING("{JPN}……そうじゃ。 わしは ウコン。\nパレスガーディアンの ウコンじゃ。"),
        .visit = COMPOUND_STRING("{JPN}ウコン『カーッ！ よう きた！\nおぬしの おかげで めが さめたわい。\pポケモンの こころが すべて\nわかる せかいは らくじゃった。\pじゃが わからぬ からこそ\nあゆみよる ものなのじゃ。"),
        .cured = COMPOUND_STRING("{JPN}ウコン『……うむ。 からだが かるい。\nかんしゃ するぞ。\pつぎは わしと ポケモンの\nきずなを みせて やろう。"),
        .inn = COMPOUND_STRING("{JPN}ウコン『……ふむ。 よい やどじゃ。\pポケモンの こころは\nポケモンに ききなおす。\pわしは ここで\nあいつらと はなしておる。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}コゴミ"),
        .recovered = COMPOUND_STRING("{JPN}……あたし コゴミ！\nアリーナタイクーンの コゴミよ！"),
        .visit = COMPOUND_STRING("{JPN}コゴミ『きたわね！ あたしね\nあなたに まけて おもいだしたの！\pみんなと いっしょなら まけても\nくやしく なかった。\pでも くやしいって おもえるのが\nあたしの こんじょう なのよ！"),
        .cured = COMPOUND_STRING("{JPN}コゴミ『よーし ふっかつ！\nありがとっ！\pつぎは まけないからね！\nかくご しなさい！"),
        .inn = COMPOUND_STRING("{JPN}コゴミ『ここで やすんでるの！\nでも からだが うずうず するわ！\pはやく アリーナで\nあばれたい ところね！』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ダツラ"),
        .recovered = COMPOUND_STRING("{JPN}……おれは ダツラ。\nファクトリーヘッドの ダツラだ。"),
        .visit = COMPOUND_STRING("{JPN}ダツラ『よう。 あんたに まけて\nやっと おもいだしたよ。\pみんなの ちしきが ぜんぶ\nあたまに はいってくるのは らくだ。\pでも しらない ことを しるのが\nおれは すきなんだよな。"),
        .cured = COMPOUND_STRING("{JPN}ダツラ『……お なおった。\nたすかったよ。\pこんどは おれの ちしきで\nしょうぶ しようぜ。"),
        .inn = COMPOUND_STRING("{JPN}ダツラ『……お きたのか。\pここの どうぐ なかなか\nおもしろい つくりでね。\pしらべて いるうちに\nつい ながいしちまうんだ。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}アザミ"),
        .recovered = COMPOUND_STRING("{JPN}……アザミ。\n……チューブクイーン アザミ。"),
        .visit = COMPOUND_STRING("{JPN}アザミ『……きた。\n……まけて おもいだした。\p……みんなと いるのは\nいやじゃ なかった。\p……でも アタシは\nひとりで たつ。"),
        .cured = COMPOUND_STRING("{JPN}アザミ『……なおった。\n……ありがと。\p……つぎは チューブで。"),
        .inn = COMPOUND_STRING("{JPN}アザミ『……ここ……。\n……おちつく。\p……チューブは まだ いい。\n……もうすこし。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ジンダイ"),
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ わしは ジンダイ！\nピラミッドキングの ジンダイだ！"),
        .visit = COMPOUND_STRING("{JPN}ジンダイ『おお きたか！ おぬしに\nまけて わしを とりもどしたぞ！\pみなと ひとつなら まよう ことも\nおそれる ことも なかった。\pだが ぼうけんとは じぶんの あしで\nふみだす ゆうきの ことよ！"),
        .cured = COMPOUND_STRING("{JPN}ジンダイ『ガハハ！ ちからが みなぎる！\nれいを いうぞ！\pつぎは わしの ゆうきと\nおぬしの ゆうきの しょうぶだ！"),
        .inn = COMPOUND_STRING("{JPN}ジンダイ『ガハハ！ この りょかん\nたんけんの しがいが あるぞ！\pうらにわまで\nぜんぶ みて まわったわい！』"),
    },
};
