// Story lines for the Frontier Brains and the seventh-battle guests while the
// network has them. Index order follows NAMED_STORY_* (guests, then Brains).

static const u8 sText_NamedCommonWords[] = _("{JPN}皆と一緒なら \n何も 怖くない。");
static const u8 sText_NamedUneasy[] = _("{JPN}……今のは 誰の言葉だ？\n私は 何を言いたかった？");
static const u8 sText_NamedCommonIntro[] = _("{JPN}……皆と一緒なら \n何も 怖くない。\pさあ 始めよう。\n皆で 楽しい勝負を。");
static const u8 sText_NamedUneasyIntro[] = _("{JPN}皆と一緒なら……\n何も……怖く……ない？\p……おかしいな。きみとは\n前にも 戦った気がする。");
static const u8 sText_NamedRescuedChallenge[] = _("{JPN}旅館で ゆっくり休めたよ！\nさあ 勝負を始めよう！");

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
        .recovered = COMPOUND_STRING("{JPN}……そうだ。 私は センリ。\nトウカジムの ジムリーダーだ。"),
        .visit = COMPOUND_STRING("{JPN}センリ『きみに負けて \nようやく 自分の名を思い出した。\p誰かの心が 流れ込んでくる。\nそれは 不思議と楽だった。\p……だが 私には 守りたいものがある。\n自分の足で 立っていたいんだ。』"),
        .cured = COMPOUND_STRING("{JPN}センリ『……頭が すっきりした。\nありがとう。\p今度は ジムリーダーとして\n勝負を 受けて たとう。』"),
        .inn = COMPOUND_STRING("{JPN}センリ『旅館の 風呂は いいな。\n頭が よく まわる。\pジムに 戻る 前に\nもういちど 鍛え直す つもりだ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}あ……あたし アスナ！\nフエンジムの ジムリーダー！"),
        .visit = COMPOUND_STRING("{JPN}アスナ『あのね！ あたし\nアスナだって 思い出したの！\pみんなと 一緒だと\n失敗しても 怖くなかった。\pでも あたしの 炎は\nあたしが 燃やしたいの！』"),
        .cured = COMPOUND_STRING("{JPN}アスナ『わあ 身体が 熱い！\n炎が 戻ってきた かんじ！\pありがとう！ 次は\nあたしの 炎で いくからね！』"),
        .inn = COMPOUND_STRING("{JPN}アスナ『ここの 温泉 最高！\nフエンの お湯と いい 勝負！\pあたし ここで 炎の\n練習 しなおすんだ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……思い出した。\n私は マグマ団の マツブサ。"),
        .visit = COMPOUND_STRING("{JPN}マツブサ『フッ……この私が \n誰かに 救われることになるとは。\p全ての心が ひとつになる。\nかつての私なら 望んだだろうな。\p……だが 自らの意志で 選んでこそ \n世界を 変えられるのではないかね。』"),
        .cured = COMPOUND_STRING("{JPN}マツブサ『……ふむ。 熱が 引いた。\n礼を いおう。\p私の 考えは 私が\n間違え 私が 正す。』"),
        .inn = COMPOUND_STRING("{JPN}マツブサ『……静かな 部屋だ。\n考え事には ちょうど いい。\p大地の ことも 人の ことも\n自分の 頭で\l考え直すと しよう。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ああ そうだ。 私は ミクリ。\nホウエンの チャンピオンだ。"),
        .visit = COMPOUND_STRING("{JPN}ミクリ『やあ。きみとの勝負で \n私は 私に戻れた。\p誰もが 同じものを美しいと感じる。\n……それも 悪くはなかったよ。\pでも 誰の目にも 同じである必要はない。\nきみには きみの美しさがある。』"),
        .cured = COMPOUND_STRING("{JPN}ミクリ『……すみきった 気分だ。\nありがとう。\p今度は 私だけの\n美しい 勝負を 見せよう。』"),
        .inn = COMPOUND_STRING("{JPN}ミクリ『窓から 見える 海が\n美しい。\p繋がりの 中の 美しさは\n誰の 者でも なかった。\pこれからは 私の 目で\n選んでいくよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ おれは アオギリ！\nアクア団の アオギリさまだ！"),
        .visit = COMPOUND_STRING("{JPN}アオギリ『よう！ おまえに 負けて\nやっと 目が 覚めたぜ！\pみんなと 混ざり合う 海は\nたしかに 広かった。\pだがな おれは おれの 船で\n海に でたいんだよ！』"),
        .cured = COMPOUND_STRING("{JPN}アオギリ『ガハハ！ 身体が 軽い！\n助かったぜ！\p今度は おれの やりかたで\nしょうぶして やるからな！』"),
        .inn = COMPOUND_STRING("{JPN}アオギリ『ガハハ！ 海の 見える\n宿とは いい 趣味だ！\pしばらく ここで\nおれの 夢を ねりなおすぜ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ええ。 私は アダン。\nルネジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}アダン『ふふ……あなたのおかげで\n私自身を 取り戻せました。\p全てが溶け合う夢は \n静かで 心地よいものでした。\pですが 一人で踊るからこそ\n見せられる美しさも あるのです。』"),
        .cured = COMPOUND_STRING("{JPN}アダン『……ああ。 目が 覚めました。\n心から 感謝を。\p次は 私 自身の\n勝負を おみせしましょう。』"),
        .inn = COMPOUND_STRING("{JPN}アダン『……おちついた へやですね。\n心が すみわたります。\pルネに 戻る 前に\nもういちど 水の 心を\lみがきます。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ わしは ハルヒコ！\nカチヌキファミリーの 父親だ！"),
        .visit = COMPOUND_STRING("{JPN}ハルヒコ『いやあ 参った！\nきみに負けて 思い出したよ！\p家族の気持ちが 全部分かる。\nそれは 嬉しかったんだが……。\p何を考えてるんだ って 聞いてみるのも\n家族の楽しみだったんだよなあ！』"),
        .cured = COMPOUND_STRING("{JPN}ハルヒコ『おお 治った！\nありがとう！\pさっそく 家族に じまん しないとな！』"),
        .inn = COMPOUND_STRING("{JPN}ハルヒコ『ここの 飯は うまい！\n家族 みんなで きたいもんだ！\pもちろん 勝負も\n家族 そうでで いくぞ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あら 私は ヤスエ。\nカチヌキファミリーの 母親よ。"),
        .visit = COMPOUND_STRING("{JPN}ヤスエ『まあ わざわざありがとう。\nおかげで 思い出せたわ。\p子どもの気持ちが 全部分かるって \n安心だったのよ。\pでも あの子達は あの子達だものね。\n……私は 見守るほうがいいわ。』"),
        .cured = COMPOUND_STRING("{JPN}ヤスエ『ふう…… すっきりしたわ。\n本当に ありがとう。\p今度 うちの こたちと\n遊んで あげてね。』"),
        .inn = COMPOUND_STRING("{JPN}ヤスエ『ゆっくり 休めて いるわ。\n子ども達の 顔が みたいわね。\pみんなが 自分の 足で\n歩いて こられるように\l私も がんばらなくちゃ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あっ アキだ！\nアキは カチヌキファミリーの アキ！"),
        .visit = COMPOUND_STRING("{JPN}アキ『あっ おねえちゃんかな？\nおにいちゃんかな？ えへへ！\pみんなと 一緒だと\nなんでも 分かって 楽しかった！\pでも アキ 自分で 考えて\n勝ちたいの！』"),
        .cured = COMPOUND_STRING("{JPN}アキ『わあ！ 元気 でたよ！\nありがとう！\p次は アキの 作戦で\n勝つからね！』"),
        .inn = COMPOUND_STRING("{JPN}アキ『りょかんって 広ーい！\nかくれんぼ したく なっちゃう！\pでも 今は アキの 作戦\n考えてるの！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうそう 私ゃ ミツヨ。\nカチヌキファミリーの おばあちゃん。"),
        .visit = COMPOUND_STRING("{JPN}ミツヨ『おやまあ よく きたねえ。\nおかげで 思い出したよ。\pみんなと ひとつに なるのは\n子守歌の ようだったねえ。\pでも 長生きの 秘訣はね\n自分の 足で 歩く ことさ。』"),
        .cured = COMPOUND_STRING("{JPN}ミツヨ『ああ 身体が 軽いよ。\nありがとうねえ。\p孫 たちにも よろしく\nいっておくれ。』"),
        .inn = COMPOUND_STRING("{JPN}ミツヨ『ありがたいねえ。\nあったかい お風呂に ごはん。\p孫達の ぶんまで\nゆっくり させて もらうよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……おれは リョウヘイ。\nカチヌキファミリーの 長男だ。"),
        .visit = COMPOUND_STRING("{JPN}リョウヘイ『よう。 きみに 負けて\nやっと 自分に 戻れたよ。\p家族の 中で いちばんじゃなくても\nみんなと 一緒なら 平気だった。\pでも おれは おれとして\nもっと 強く なりたいんだ。』"),
        .cured = COMPOUND_STRING("{JPN}リョウヘイ『……よし。 治った。\nありがとな。\p今度は 必ず 勝ってみせるぜ。』"),
        .inn = COMPOUND_STRING("{JPN}リョウヘイ『……ここ 静かで いいな。\pみんなの 声が ないと\n自分の 声が よく 聞こえる。\p次は おれの 作戦で 勝つ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……私は ナギ。\nヒワマキジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}ナギ『こんにちは。\n……おかげで 思い出せました。\p皆と 同じ風に乗るのは \n穏やかで 心地よかった。\pそれでも 私には 私の翼があります。\nどの風に乗るのか 自分で選びたいのです。』"),
        .cured = COMPOUND_STRING("{JPN}ナギ『……風が かわりました。\nありがとうございます。\p次は 私の 鳥ポケモン達と\nおあいて しますね。』"),
        .inn = COMPOUND_STRING("{JPN}ナギ『ここは 風が よく とおります。\p鳥ポケモン達も\nのびのびと 羽を 休めています。\p私も 少しだけ\n空を 見上げて 過ごします。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ おれは トウキ！\nムロジムの ジムリーダーだぜ！"),
        .visit = COMPOUND_STRING("{JPN}トウキ『よっ！ おまえに 負けて\n目が 覚めたぜ！\pみんなで 同じ 波に のるのも\n気持ち よかったけどさ。\pやっぱ 自分で 選んだ 波に\nのりたいんだよな！』"),
        .cured = COMPOUND_STRING("{JPN}トウキ『うおっ 身体が 動く！\nサンキュー！\p次は おれの でっかい 波で\n勝ちに いくぜ！』"),
        .inn = COMPOUND_STRING("{JPN}トウキ『よう！ ここの 前の 海\nなかなかの 波が たつぜ！\p身体が なまらない ように\n毎朝 泳いでるんだ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}フウ『……ぼくは フウ！』\nラン『私は ラン！』"),
        .visit = COMPOUND_STRING("{JPN}フウ『ぼくたち 思い出したよ！』\nラン『あなたに 負けた おかげ！』\pフウ『二人の 心が ずっと\n繋がってるのは なれてた けど……』\pラン『みんなと 繋がるのは\n違ったの。 私は 私！』"),
        .cured = COMPOUND_STRING("{JPN}フウ『すっきりした！』\nラン『ありがとう！』\pフウ『次は ぼくたち 二人の』\nラン『コンビネーションを 見せるね！』"),
        .inn = COMPOUND_STRING("{JPN}フウ『ここの 部屋 二人で ひとつ！』\nラン『でも ベッドは べつべつ！』\pフウ『違う ことを 考えるのも』\nラン『けっこう 楽しいね！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……へっ 思い出したぜ。\nおれは 四天王の カゲツだ。"),
        .visit = COMPOUND_STRING("{JPN}カゲツ『よう。 おまえの おかげで\nおれに 戻れたぜ。\pみんなと なかよく するのも\nわるかねえ 気分だった。\pだがよ くせものは くせもの らしく\n一人で 立ってたいのさ。』"),
        .cured = COMPOUND_STRING("{JPN}カゲツ『……へっ 治ったか。\n借りが できちまったな。\p次は 遠慮 なしで\nいかせて もらうぜ。』"),
        .inn = COMPOUND_STRING("{JPN}カゲツ『……へっ わるくねえ 宿だ。\p一人で 寝転がって\n考え事ってのも\lたまには いいもんだな。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わたくしは プリム。\n四天王の 一人 ですわ。"),
        .visit = COMPOUND_STRING("{JPN}プリム『ごきげんよう。 あなたの\n勝負で 目が 覚めましたわ。\pみなさまと 溶け合う ぬくもり。\n氷の わたくしには めずらしくて。\pでも わたくしの 心は\nわたくしだけの ものですわ。』"),
        .cured = COMPOUND_STRING("{JPN}プリム『……ひえびえと して\nよい 気分。 感謝しますわ。\p次は わたくしの 氷の\n勝負を おみせしましょう。』"),
        .inn = COMPOUND_STRING("{JPN}プリム『……ここの 夜は\nほどよく ひえて 心地よいですわ。\p一人の ねむりが\nこんなに 静かだなんて。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ウ……ウシオだ！\nアクア団の ウシオだーっ！"),
        .visit = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ おまえに 負けて\n思い出したぞーっ！\pみんなと 一緒に さわぐのは\nめちゃくちゃ 楽しかった！\pでもな おれの おおごえは\nおれが だしたいんだーっ！』"),
        .cured = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ 元気 百倍！\nありがとなーっ！\p次も 全力で\nぶつかって いくぞーっ！』"),
        .inn = COMPOUND_STRING("{JPN}ウシオ『ウハハ！ 飯が うまい！\nお代わり 3ばい めだーっ！\pアオギリさまにも\n食べさせて あげたいなーっ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……そうか。 ぼくは ダイゴ。\n石を 愛する トレーナーだ。"),
        .visit = COMPOUND_STRING("{JPN}ダイゴ『やあ。きみと戦って \nぼくは ぼくに戻れたよ。\p誰とでも 気持ちが通う世界は\n確かに 優しかった。\pでも 石は ひとつずつ違うだろう？\n……ぼくは その違いが好きなんだ。』"),
        .cured = COMPOUND_STRING("{JPN}ダイゴ『……うん。 身体が 軽い。\nありがとう。\p今度は ぼくの 鋼の\n勝負を 見せるよ。』"),
        .inn = COMPOUND_STRING("{JPN}ダイゴ『この あたりの 岩\nめずらしい 縞模様 なんだ。\pしばらく ここで 石を\n調べて 過ごすよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……私は ツツジ。\nカナズミジムの ジムリーダーです。"),
        .visit = COMPOUND_STRING("{JPN}ツツジ『こんにちは。あなたに負けて \nようやく 名前を思い出しました。\p皆の知識が 流れ込んでくる。\n勉強せずに分かるのは 楽でしたわ。\p……でも 自分で調べて\n答えに辿り着くのが 好きなんです。』"),
        .cured = COMPOUND_STRING("{JPN}ツツジ『……治りました。\n本当に ありがとうございます。\p今度は 私の いわタイプで\n勝負 しましょうね。』"),
        .inn = COMPOUND_STRING("{JPN}ツツジ『……ここで 勉強の\nやり直し ですわ。\pみんなの 知識で 分かった 気に\nなっていた ことを\l自分で 確かめたいのです。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……アタシは イズミ。\nアクア団の 幹部よ。"),
        .visit = COMPOUND_STRING("{JPN}イズミ『あら きたのね。\nアンタの おかげで 思い出したわ。\pみんなと 一緒って\n意外と 居心地 よかったのよ。\pでも アタシは アタシの 好きに\nいきたいの。 分かるでしょ？』"),
        .cured = COMPOUND_STRING("{JPN}イズミ『……ふうん すっきりしたわ。\nありがと。\p次は 手加減 しないわよ。』"),
        .inn = COMPOUND_STRING("{JPN}イズミ『……ふん。 わるくない 部屋ね。\pしばらくは のんびり\nさせて もらうわよ。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ホムラだ！\nマグマ団の 幹部 ホムラだ！"),
        .visit = COMPOUND_STRING("{JPN}ホムラ『おう！ おまえに 負けて\n思い出したぜ！\pみんなで ひとつの 考えって\nらくちんでは あったんだがな。\pやっぱ オレは オレの 頭で\n熱く なりたいんだよ！』"),
        .cured = COMPOUND_STRING("{JPN}ホムラ『おっ 治ったぜ！\nあんがとよ！\p次は オレの マグマで\n燃やして やるからな！』"),
        .inn = COMPOUND_STRING("{JPN}ホムラ『おう！ ここの お風呂\n熱くて 最高だぜ！\pオレの マグマも\nまた たぎって きたぜ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}ワハハ…… そうじゃ！\nわしは キンセツジムの テッセン！"),
        .visit = COMPOUND_STRING("{JPN}テッセン『ワハハ！ よう来た！\nおかげで 思い出したわい！\p皆と繋がっておると \n誰かが笑えば わしも愉快でのう。\p……じゃが わしの笑い声じゃ。\nわしが おかしくて笑いたいのじゃよ！』"),
        .cured = COMPOUND_STRING("{JPN}テッセン『ワハハ！ しびれるのう！\nありがとうよ！\p次も ビリビリ いくぞい！』"),
        .inn = COMPOUND_STRING("{JPN}テッセン『ワハハ！ ここの 電気は\nちと よわいのう！\pわしが 直して やろうかの！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……カガリ。\n……アタシは カガリ。"),
        .visit = COMPOUND_STRING("{JPN}カガリ『……きた。\n……アンタに 負けて 思い出した。\p……みんなの 声 ずっと\n聞こえてて…… 静かじゃなかった。\p……アタシは アタシの 声だけ\n聞きたい。』"),
        .cured = COMPOUND_STRING("{JPN}カガリ『……静か。\n……アハ。 ありがと。\p……次は アタシが\nアンタを 調べる 番。』"),
        .inn = COMPOUND_STRING("{JPN}カガリ『……ここ 静か。\n……アハ。 好き。\p……アタシ ここで\nいろいろ 調べてる。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……あたい フヨウ！\n四天王の フヨウだよ！"),
        .visit = COMPOUND_STRING("{JPN}フヨウ『あ 来たね！\nきみに負けて 思い出したんだ。\p皆の気持ちが ふわふわ寄ってくるの \n嫌いじゃなかったよ。\pでも どれが あたいの気持ちなのか\n分からなくなるのは……違うよね。』"),
        .cured = COMPOUND_STRING("{JPN}フヨウ『ふふっ すっきり！\nありがとね！\p次は あたいの ゴースト達と\n遊んで いってよ！』"),
        .inn = COMPOUND_STRING("{JPN}フヨウ『ここの 廊下\n夜 なんか でそうで いいよね！\pゴースト達も\n楽しそうに してるよ！』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……ぼくは ミツル。\n覚えてる。 ぼくは ミツルだ。"),
        .visit = COMPOUND_STRING("{JPN}ミツル『……こんにちは。\nきみに負けて 思い出したんだ。\p皆と一緒だと 身体も心も苦しくない。\n……だから 離れるのは怖いよ。\pでも ぼくが 強くなりたいって思ったのは\n苦しくなくなりたいから だけじゃないんだ。』"),
        .cured = COMPOUND_STRING("{JPN}ミツル『……治った。\n本当に ありがとう！\p今度は ぼくと ポケモン達の\n力で しょうぶするね！』"),
        .inn = COMPOUND_STRING("{JPN}ミツル『ぼく ここで ゆっくり\n休ませて もらってるんだ。\p身体が 弱くても\nぼくは ぼくの ペースで\l強く なりたいな。』"),
    },
    {
        .recovered = COMPOUND_STRING("{JPN}……わしは ゲンジ。\n四天王の ゲンジじゃ。"),
        .visit = COMPOUND_STRING("{JPN}ゲンジ『よう来たな。\nおぬしに敗れて 我を取り戻した。\p皆と ひとつの船に乗るのは \n嵐すら 怖くなかったわい。\pじゃが 舵まで 任せきりにはできん。\n……船乗りの性分よ。』"),
        .cured = COMPOUND_STRING("{JPN}ゲンジ『……うむ。 治ったわい。\n礼を いうぞ。\p次は わしの ドラゴン達が\n相手を しよう。』"),
        .inn = COMPOUND_STRING("{JPN}ゲンジ『……海の においじゃ。\p船に のっていた ころを\n思い出すのう。\pドラゴン達も\nゆっくり 休ませて やるわい。』"),
    },
    // Frontier Brains, in FRONTIER_FACILITY_* order
    {
        .name = COMPOUND_STRING("{JPN}リラ"),
        .recovered = COMPOUND_STRING("{JPN}……私は リラ。\nバトルタワーの タワータイクーン。"),
        .visit = COMPOUND_STRING("{JPN}リラ『……ここが リゾート。\nあなたに負けて 思い出したの。\p皆の心が 聞こえるのは\nとても あたたかかった。\p……それでも 私は\n私でいたいの。』"),
        .cured = COMPOUND_STRING("{JPN}リラ『……声が きえた。\n静かね。 でも わるくない。\pありがとう。 今度は 私\nじしんとして 戦いましょう。』"),
        .inn = COMPOUND_STRING("{JPN}リラ『……静かね。\nでも わるくない。\pこの 静けさの 中で\n私の 声を 探しているの。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ヒース"),
        .recovered = COMPOUND_STRING("{JPN}……ボクは ヒース！\nドームスーパースター ヒースっ!!"),
        .visit = COMPOUND_STRING("{JPN}ヒース『やあっ!! きみの おかげで\nボクは ボクを 思い出したよっ!!\pみんなが ボクに 拍手 してくれる\n世界は 最高 だったけどさっ！\pスターは 自分で 輝くから\nスターなのさっ!!』"),
        .cured = COMPOUND_STRING("{JPN}ヒース『アッハッハッ!! 復活っ!!\nありがとうっ!!\p次も ボクの ステージで\n待ってるよっ!!』"),
        .inn = COMPOUND_STRING("{JPN}ヒース『アッハッハッ!!\nこの 旅館も ボクの ステージっ!!\pおふろあがりの ボクを\nみに きたまえっ!!』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ウコン"),
        .recovered = COMPOUND_STRING("{JPN}……そうじゃ。 わしは ウコン。\nパレスガーディアンの ウコンじゃ。"),
        .visit = COMPOUND_STRING("{JPN}ウコン『カーッ！ よう来た！\nおぬしのおかげで 目が覚めたわい。\pポケモンの心が 全て分かる。\nそれは 楽じゃったがのう。\p分からぬからこそ 顔を覗き 声を聞く。\n……わしは それを忘れかけておった。』"),
        .cured = COMPOUND_STRING("{JPN}ウコン『……うむ。 身体が 軽い。\n感謝 するぞ。\p次は わしと ポケモンの\n絆を 見せてやろう。』"),
        .inn = COMPOUND_STRING("{JPN}ウコン『……ふむ。 よい やどじゃ。\pポケモンの 心は\nポケモンに 聞き直す。\pわしは ここで\nあいつらと 話しておる。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}コゴミ"),
        .recovered = COMPOUND_STRING("{JPN}……あたし コゴミ！\nアリーナタイクーンの コゴミよ！"),
        .visit = COMPOUND_STRING("{JPN}コゴミ『きたわね！ あたしね\nあなたに 負けて 思い出したの！\pみんなと 一緒なら 負けても\n悔しく なかった。\pでも 悔しいって 思えるのが\nあたしの こんじょう なのよ！』"),
        .cured = COMPOUND_STRING("{JPN}コゴミ『よーし 復活！\nありがとっ！\p次は 負けないからね！\n覚悟 しなさい！』"),
        .inn = COMPOUND_STRING("{JPN}コゴミ『ここで 休んでるの！\nでも 身体が うずうず するわ！\pはやく アリーナで\nあばれたい ところね！』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ダツラ"),
        .recovered = COMPOUND_STRING("{JPN}……おれは ダツラ。\nファクトリーヘッドの ダツラだ。"),
        .visit = COMPOUND_STRING("{JPN}ダツラ『よう。あんたに負けて \nやっと 思い出したよ。\p皆の知識が 勝手に入ってくるんだ。\nそりゃ 楽だったさ。\p……でも 分からないことを調べてる時が\nおれは 一番楽しかったんだよな。』"),
        .cured = COMPOUND_STRING("{JPN}ダツラ『……お 治った。\n助かったよ。\p今度は おれの 知識で\n勝負 しようぜ。』"),
        .inn = COMPOUND_STRING("{JPN}ダツラ『……お きたのか。\pここの どうぐ なかなか\nおもしろい つくりでね。\p調べて いるうちに\nつい 長居しちまうんだ。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}アザミ"),
        .recovered = COMPOUND_STRING("{JPN}……アザミ。\n……チューブクイーン アザミ。"),
        .visit = COMPOUND_STRING("{JPN}アザミ『……きた。\n……負けて 思い出した。\p……みんなと いるのは\n嫌じゃ なかった。\p……でも アタシは\n一人で たつ。』"),
        .cured = COMPOUND_STRING("{JPN}アザミ『……治った。\n……ありがと。\p……次は チューブで。』"),
        .inn = COMPOUND_STRING("{JPN}アザミ『……ここ……。\n……落ち着く。\p……チューブは まだ いい。\n……もうすこし。』"),
    },
    {
        .name = COMPOUND_STRING("{JPN}ジンダイ"),
        .recovered = COMPOUND_STRING("{JPN}……そうだ！ わしは ジンダイ！\nピラミッドキングの ジンダイだ！"),
        .visit = COMPOUND_STRING("{JPN}ジンダイ『おお きたか！ おぬしに\n負けて わしを 取り戻したぞ！\pみなと ひとつなら 迷う ことも\nおそれる ことも なかった。\pだが 冒険とは 自分の 足で\n踏み出す 勇気の ことよ！』"),
        .cured = COMPOUND_STRING("{JPN}ジンダイ『ガハハ！ 力が みなぎる！\n礼を いうぞ！\p次は わしの 勇気と\nおぬしの 勇気の 勝負だ！』"),
        .inn = COMPOUND_STRING("{JPN}ジンダイ『ガハハ！ この 旅館\n探検の しがいが あるぞ！\p裏庭まで\n全部 見て まわったわい！』"),
    },
};
