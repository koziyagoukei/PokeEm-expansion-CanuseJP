// What the people the network holds say in their lobbies, before their Gold Symbol step.
// Index order follows STORY_TARGET_*. Kazuki's scenes are scripted in frontier_story.inc;
// only his lines about the others are here. {STR_VAR_2} is someone who has left the network,
// or "なんにんか" when several have.

struct StoryTargetText
{
    const u8 *name;
    const u8 *meet;       // The first time; they are happy in the network.
    const u8 *happy;      // Talking again before their step.
    const u8 *wish;       // After the Silver Symbol (the first Master Rank win at the contest).
    const u8 *wishAgain;  // Talking again afterwards.
    const u8 *othersGone; // Before their step, once someone else has left the network.
    const u8 *wishGone;   // After it, once someone else has left.
    const u8 *wishShared; // After it, while someone else is wavering too.
};

static const u8 sText_SeveralPeople[] = _("なんにんか");

static const struct StoryTargetText sStoryTargetTexts[STORY_TARGET_COUNT] =
{
    [STORY_TARGET_KAZUKI] =
    {
        .name = COMPOUND_STRING("カズキ"),
        .othersGone = COMPOUND_STRING("{JPN}カズキ『そういえば さいきん\n{STR_VAR_2}の こえが きこえないんだ。\pでも へいき！\nみんなが いるもん！』"),
        .wishGone = COMPOUND_STRING("{JPN}カズキ『{STR_VAR_2}も\nこんな きもちに なったのかな……。』"),
        .wishShared = COMPOUND_STRING("{JPN}カズキ『みんなの なかにも\nむねが ざわざわしてる ひとが\lいるみたい。\pぼくだけじゃ ないんだね。』"),
    },
    [STORY_TARGET_REIKA] =
    {
        .name = COMPOUND_STRING("レイカ"),
        .meet = COMPOUND_STRING("{JPN}レイカ『あら ごきげんよう。\nドームに いらしたのね。\pわたくし レイカと もうしますの。\nこの こは あいぼうの マッスグマ。\pドームでは 3びきの なかから\n2ひきを えらんで たたかいますのよ。\p……ふふ。 でも いまは\nだれが でても へいきですの。\pみなさんと こころが\nつながって いますから。\pえらばれた こも えらばれなかった こも\nきもちは ぜんぶ おなじですのよ。』"),
        .happy = COMPOUND_STRING("{JPN}レイカ『マッスグマの きもちも\nみなさんの きもちも\lぜんぶ わかりますの。\pこんなに おだやかな きぶん\nはじめてですわ。』"),
        .wish = COMPOUND_STRING("{JPN}レイカ『……あなたの シンボルの しあい\nみていましたわ。\pあの とき わたくし\nきゅうに おもいだしましたの。\pまえに ドームで この マッスグマを\nひかえに した ことを。\pみなさんと つながっていれば\nマッスグマの きもちも わかるはず。\pなのに いまは なぜか\nわからない きが するんですの。\p……ねえ {PLAYER}さん。\pあいての きもちが ぜんぶ\nわからなくても……\lあなたは あいぼうを しんじられますの？\p……ごめんなさい。\nへんな ことを ききましたわね。』"),
        .wishAgain = COMPOUND_STRING("{JPN}レイカ『ひかえに した あの ひ\nマッスグマは なにを\lおもっていたのかしら。\pわからない ことが こわいのか\nわからなくても いいのか……\p……わたくし まだ\nきめられませんの。』"),
        .othersGone = COMPOUND_STRING("{JPN}レイカ『そういえば さいきん\n{STR_VAR_2}の こえが\lきこえませんの。\pでも だいじょうぶ。\nみなさんが いますもの。』"),
        .wishGone = COMPOUND_STRING("{JPN}レイカ『{STR_VAR_2}の こえが\nきこえなく なったのも……\lこういう きもちから だったのかしら。』"),
        .wishShared = COMPOUND_STRING("{JPN}レイカ『みなさんの なかにも\nざわざわしている かたが いますわ。\pわたくしだけでは ないのね……。』"),
    },
    [STORY_TARGET_IKUE] =
    {
        .name = COMPOUND_STRING("イクエ"),
        .meet = COMPOUND_STRING("{JPN}イクエ『こんにちは！\nわたし ブリーダーの イクエ。\pこの こは エネコロロ。\nずっと いっしょに そだててきたの。\pパレスでは ポケモンが じぶんで\nかんがえて たたかうでしょう？\pでも いまは しんぱい ないの。\nエネコロロの きもちは\lぜんぶ わかるから。\pみんなと つながってると\nなにを したいか すぐ わかるのよ。』"),
        .happy = COMPOUND_STRING("{JPN}イクエ『エネコロロが いま\nなにを したいのか……\lほら わかる。\pみんなと いっしょだと\nそだてるのも かんたんね。』"),
        .wish = COMPOUND_STRING("{JPN}イクエ『あなたの パレスの しあい\nみたわ。\pあなたの ポケモン じぶんで かんがえて\nたのしそうに うごいていた。\p……ねえ ききたい ことが あるの。\pあなたは あいぼうの すきな あそびを\nどうやって しったの？\p……そう。 いっしょに すごして\nすこしずつ しったのね。\pわたし…… エネコロロの すきな あそび\nほんとうは しらないのかも。\pみんなの きもちが ながれこむから\nしっている きに なっていただけ……？』"),
        .wishAgain = COMPOUND_STRING("{JPN}イクエ『いっしょに すごして\nすこしずつ しる……。\pエネコロロの こと わたし\nもう いちど しりたいな。』"),
        .othersGone = COMPOUND_STRING("{JPN}イクエ『そういえば みんなの なかから\n{STR_VAR_2}の こえが きえたの。\pふしぎね。 でも さみしくないわ。\nみんなが いるもの。』"),
        .wishGone = COMPOUND_STRING("{JPN}イクエ『{STR_VAR_2}の こえが\nきこえなく なった わけ……\pいまなら ちょっとだけ\nわかる きが するわ。』"),
        .wishShared = COMPOUND_STRING("{JPN}イクエ『みんなの なかにも\nわたしと おなじように\lまよっている ひとが いるみたい。』"),
    },
    [STORY_TARGET_EISUKE] =
    {
        .name = COMPOUND_STRING("エイスケ"),
        .meet = COMPOUND_STRING("{JPN}エイスケ『よう！ おれは エイスケ。\nトライアスロンで きたえてるんだ！\pあいぼうは スターミー。\nいっしょに およいで きたえてきたぜ。\pアリーナは 3ターンで けっちゃくが\nつかなきゃ はんていに なるんだ。\pまえは はんていで まけると\nくやしかった きが するけど……\pいまは へいきさ！\nだれが かっても みんなの かち\lだからな！』"),
        .happy = COMPOUND_STRING("{JPN}エイスケ『かっても まけても\nみんなで いっしょ！\pゴールの さきで みんなが\nまっててくれるって さいこうだぜ！』"),
        .wish = COMPOUND_STRING("{JPN}エイスケ『みたぜ！ シンボルを かけた\nあんたの しょうぶ！\p……あれを みてたら\nむかしの しあいを おもいだしたんだ。\pスターミーと でた アリーナで\n3ターン たたかって はんてい まけ。\pあと すこしで たおせたんだ。\nあと 1ターン あれば……！\p……へんだな。 みんなの かちなら\nくやしく ないはず なのに。\pあの 1ターンが\nあたまから はなれないんだ。』"),
        .wishAgain = COMPOUND_STRING("{JPN}エイスケ『あと 1ターン あれば……。\p……おわった しあいなのに\nなんで こんなに\lきに なるんだろうな。』"),
        .othersGone = COMPOUND_STRING("{JPN}エイスケ『そういや さいきん\n{STR_VAR_2}の こえが きこえないな。\pま いいか！ みんなが いれば\nさびしくないぜ！』"),
        .wishGone = COMPOUND_STRING("{JPN}エイスケ『{STR_VAR_2}の こえが\nきえたのも……\lこんな もやもやの せい なのかな。』"),
        .wishShared = COMPOUND_STRING("{JPN}エイスケ『みんなの なかでも\nだれかが もやもや してるみたいだ。\pおれだけじゃ ないんだな。』"),
    },
    [STORY_TARGET_TOSHIO] =
    {
        .name = COMPOUND_STRING("トシオ"),
        .meet = COMPOUND_STRING("{JPN}トシオ『やあ！ ぼく トシオ。\nかいじゅうみたいな ポケモンが\lだいすきなんだ！\pぼくの あいぼうは ボスゴドラ。\nココドラの ころから いっしょなんだ。\pファクトリーでは ポケモンを\nかりて たたかうんだよ。\pこの まえ かりた トリデプス\nゲンさんって いうんだけど\lいっしょに なんども かったんだ！\pみんなで かりて みんなで たたかう。\nだれと くんでも みんな なかまさ！』"),
        .happy = COMPOUND_STRING("{JPN}トシオ『ゲンさんの かたい あたま\nかっこいいんだ！\pみんなも そう おもってる。\nだから ぼくも うれしいんだよ。』"),
        .wish = COMPOUND_STRING("{JPN}トシオ『きみの シンボルの しょうぶ\nみてたよ！\p……あのね。 みんなが いうんだ。\nもっと つよそうな ポケモンに\lこうかん したほうが いいって。\pみんなが そう いうなら\nそれが いちばんの はず なんだけど……\pなんでだろう。\nゲンさんを かえしたく ないんだ。\pつよいから じゃないんだ。\nいっしょに たたかった あの こだから\l……なのかな。』"),
        .wishAgain = COMPOUND_STRING("{JPN}トシオ『こうかん したほうが いいって\nみんなは いうんだけど……\pぼく まだ ゲンさんと\nいっしょに いたいんだ。』"),
        .othersGone = COMPOUND_STRING("{JPN}トシオ『そういえば {STR_VAR_2}の こえ\nさいきん きこえないなあ。\pでも へいき！\nみんなが いるからね！』"),
        .wishGone = COMPOUND_STRING("{JPN}トシオ『{STR_VAR_2}も こんな きもちで\nみんなから はなれたのかなあ。』"),
        .wishShared = COMPOUND_STRING("{JPN}トシオ『みんなの なかにも\nぼくみたいに まよってる ひとが\lいるみたい。』"),
    },
    [STORY_TARGET_YAYOI] =
    {
        .name = COMPOUND_STRING("ヤヨイ"),
        .meet = COMPOUND_STRING("{JPN}ヤヨイ『いらっしゃい。\nわたし ヤヨイ。 アロマが すきなの。\pこの こは ロズレイド。\nいつも いい かおりが するのよ。\pチューブでは みちが 3つに\nわかれているでしょう？\pでも まよわないの。\nみんなの すきな みちが わかるから。\pみんな おなじ かおりが すきで\nおなじ みちを えらぶの。\pだから けんかも ないのよ。』"),
        .happy = COMPOUND_STRING("{JPN}ヤヨイ『ロズレイドの かおり\nみんな すきだって。\pおなじ ものが すきって\nほっと するわね。』"),
        .wish = COMPOUND_STRING("{JPN}ヤヨイ『あなたの シンボルの しょうぶ\nみせて もらったわ。\p……ねえ ひとつ きいても いい？\nあなたは どんな かおりが すき？』\p{PLAYER}は すきな かおりを\nこたえた。\pヤヨイ『……あら。\nわたしの すきな かおりと ちがうのね。\pみんなとは いつも おなじ なのに\nあなたとは ちがう。\pおなじ ものを すきじゃ なくても\nなかよく していられるのかしら……。』"),
        .wishAgain = COMPOUND_STRING("{JPN}ヤヨイ『すきな かおりが ちがっても\nいっしょに いられるのかしら。\p……ふしぎ。 こわいような\nたのしみな ような きもち。』"),
        .othersGone = COMPOUND_STRING("{JPN}ヤヨイ『そういえば {STR_VAR_2}の\nこえが きこえなく なったわ。\pでも みんな おなじ きもち。\nだから さみしくないの。』"),
        .wishGone = COMPOUND_STRING("{JPN}ヤヨイ『{STR_VAR_2}は\nちがう みちを えらんだのかしら。』"),
        .wishShared = COMPOUND_STRING("{JPN}ヤヨイ『みんなの なかにも\nちがう かおりを さがしている ひとが\lいるみたい。』"),
    },
    [STORY_TARGET_NOBUO] =
    {
        .name = COMPOUND_STRING("ノブオ"),
        .meet = COMPOUND_STRING("{JPN}ノブオ『おう。 おれは ノブオ。\nいせきを しらべるのが すきでな。\pこいつは あいぼうの サンドパン。\nいっしょに あなを ほってきた。\pピラミッドは まっくらで\nまよいやすいって いうだろう？\pだが おれは まよわないのさ。\nみんなが みちを しってるからな。\pひとりで ぜんぶ わかるってのは\nいい もんだぞ。』"),
        .happy = COMPOUND_STRING("{JPN}ノブオ『くらい みちでも\nみんなの きおくが てらしてくれる。\pたすけを たのむ ひつようも\nないってわけさ。』"),
        .wish = COMPOUND_STRING("{JPN}ノブオ『おう きみか。\nシンボルの しょうぶ みたぞ。\p……ところで ちょっと いいか。\nピラミッドで うえの かいへ\lのぼる みちを なにか しらないか？\p……ん？ いま おれ\nまよってるって いったのか。\pみんなには いわなくても\nつたわってる はず なんだが……\pきみには きいて はじめて\nつたわったんだな。\p……ことばに しないと\nつたわらない あいても いるのか。』"),
        .wishAgain = COMPOUND_STRING("{JPN}ノブオ『きいて はじめて つたわる……。\pひとりで ぜんぶ わかる ほうが\nらく だったんだがな。』"),
        .othersGone = COMPOUND_STRING("{JPN}ノブオ『そういや {STR_VAR_2}の こえが\nきこえなく なったな。\p……まあ いい。\nみんなが いれば まよわんさ。』"),
        .wishGone = COMPOUND_STRING("{JPN}ノブオ『{STR_VAR_2}も だれかに\nことばで つたえたのかもな。』"),
        .wishShared = COMPOUND_STRING("{JPN}ノブオ『みんなの なかにも\nみちに まよってる やつが\lいるみたいだな。』"),
    },
    [STORY_TARGET_YORIKO] =
    {
        .name = COMPOUND_STRING("ヨリコ"),
        .meet = COMPOUND_STRING("{JPN}ヨリコ『あら こんにちは。\nわたしは ヨリコ。\pあいぼうの ハブネークと\nコンテストに でているの。\pいまの コンテストは すてきよ。\nみんなが おなじように わかってくれて\lおなじように はくしゅ してくれるの。\pだれにも ごかい されない えんぎって\nとっても あんしん するわ。』"),
        .happy = COMPOUND_STRING("{JPN}ヨリコ『はくしゅの おおきさも\nみんな いっしょ。\pハブネークも きっと\nうれしい はずだわ。』"),
        .wish = COMPOUND_STRING("{JPN}ヨリコ『マスターランク ゆうしょう\nおめでとう。\pわたしと ハブネークも\nおなじ ぶたいに でていたのよ。\p……ねえ わたしたちの えんぎ\nどこが よかった？』\p{PLAYER}は ハブネークの しっぽの\nうごきが よかったと こたえた。\pヨリコ『……しっぽ？\pわたしが みせたかったのは\nハブネークの するどい まなざし なのに。\pみんなは いつも わたしの\nみせたい ところを わかってくれるのに……\p……どうしてかしら。\nなんだか むねが ざわざわするわ。』"),
        .wishAgain = COMPOUND_STRING("{JPN}ヨリコ『みる ひとによって\nよい ところが ちがう……。\pそれって こわい ことの はずよね。\nなのに あの ことばが\lわすれられないの。』"),
        .othersGone = COMPOUND_STRING("{JPN}ヨリコ『そういえば さいきん\n{STR_VAR_2}の こえが きこえないの。\pでも だいじょうぶ。\nはくしゅは みんなから もらえるもの。』"),
        .wishGone = COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}も ちがう はくしゅを\nもとめて いったのかしら。』"),
        .wishShared = COMPOUND_STRING("{JPN}ヨリコ『みんなの なかにも\nわたしと おなじように\lざわざわしている ひとが いるわ。』"),
    },
};

// Their release battle and cure (everyone but Kazuki, whose scenes are scripted on their own), by
// RESCUE_TEXT_*. Someone without a RESCUE_TEXT_FEVER does not have these scenes yet.
static const u8 *const sStoryTargetRescueTexts[STORY_TARGET_COUNT][RESCUE_TEXT_COUNT] =
{
    [STORY_TARGET_REIKA] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}レイカ『……あら？\nなんだか あたまが ぼんやりしますわ……\pきんいろの シンボル……\nひかえの……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}レイカ『……マッスグマ……\nひかえ…… えらばなかった……\pみなさんと つながっていれば\nぜんぶ わかる はず……\pわかる はず ですのに……\pでも わたくしは…… わたくしが\nしりたいのは……\pちがいますわ。 わたくしたちは。\nわたくし は。 わたくし……\pあつい。 あたまが あつい。\pどうして。 どうして はなれるの？\nどうして わからない ものを\lしんじるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}レイカ『……みなさんの こえが\nとおく なっていきますわ……\pマッスグマの きもちも\nだれの きもちも\lぜんぶ わかったのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}レイカ『でも…… いまの しょうぶで\nわかりましたの。\pわたくし あの ひ ひかえに した\nマッスグマを\lずっと だいじに おもっていた。\pあの こが なにを おもって いたのか\nわたくしには わからない。\pそれでも いいの。\nわからなくても\lあの こを しんじたい。\p……ごめんなさい みなさん。\nわたくし いきますわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}レイカ『……マッスグマ……\nみなさんの きもち…… ぜんぶ……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}てんしゅは レイカの くちに\nめしを どんどん はこんだ！\pレイカ『んむっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}レイカ『……まあ。\nここは どちらかしら。\pあたまが すっきりしていますわ……\nみなさんの こえが…… きこえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}レイカ『……ぜんぶ おぼえていますわ。\pマッスグマの きもちも\nみなさんの きもちも\lぜんぶ わかって……\pほんとうに おだやか でしたの。\pでも これからは わからない ままでも\nあの こと いっしょに\lあるいて いきますわ。\p……たすけて くださって\nありがとうございます。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}レイカ『そうですわ。\nポケナビを おもちかしら？\pれんらくさきを こうかん しましょう。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}レイカ『わたくし りょかんで\nやすませて いただきますわ。\pマッスグマと おさんぽ するのが\nたのしみ ですの。』"),
    },
    [STORY_TARGET_IKUE] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}イクエ『……あれ？\nなんだか あたまが ふわふわする……\pきんいろの シンボル……\nエネコロロの…… きもち……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}イクエ『……エネコロロ……\nいま なにを したいの……\pみんなと つながってれば\nぜんぶ わかる…… わかるのに……\pでも わたしは…… わたしが\nしりたいのは……\pちがう。 わたしたちは。\nわたし は。 わたし……\pあつい。 あたまが あつい。\pどうして。 どうして はなれるの？\nどうして わからない あいてを\lそだてたがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}イクエ『……みんなの こえが\nとおく なっていく……\pエネコロロの きもちも\nみんなの きもちも\lながれこんで きてたのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}イクエ『でも…… いまの しょうぶで\nわかったの。\pあなたの ポケモンは\nあなたの いうとおりじゃ なくても\lあなたと いっしょに たたかってた。\pエネコロロも きっと\nわたしとは ちがう ことを\lかんがえてる。\pそれで いいの。\nわたしは その こを しりたい。\lそだてて いきたい。\p……ごめんね みんな。\nわたし いくわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}イクエ『……エネコロロ……\nみんなの…… ぜんぶ…… わかる……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}てんしゅは イクエの くちに\nめしを どんどん はこんだ！\pイクエ『んぐっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}イクエ『……あれ？\nここ どこかしら。\pあたまが すっきりしてる……\nみんなの こえが…… きこえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}イクエ『……ぜんぶ おぼえてる。\pエネコロロの したいことが\nいつでも わかって……\lそだてるのも かんたん だった。\pでも あれは わかった きに\nなっていた だけ なのかも。\pこれからは いっしょに すごして\nすこしずつ しっていくわ。\p……たすけて くれて ありがとう。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}イクエの モンスターボールから\nエネコロロが とびだした！\pエネコロロは イクエの まえに\nすわりこむと ボールから\lふいっと かおを そむけた。\pイクエ『……エネコロロ？\pもしかして…… もう\nたたかいたく ないの？』\pエネコロロは イクエの てに\nあたまを すりよせた。\pイクエ『……そう。\nそれが あなたの きもち なのね。\pわかったわ。 わたし\nあなたが えらんだ ことを\lだいじに したい。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}イクエ『ねえ ポケナビ もってる？\nれんらくさき こうかん しましょう。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}イクエ『わたし りょかんで\nエネコロロと ゆっくり するわ。\pこの こが なにを して\nあそびたいか みつけたいの。』"),
    },
    [STORY_TARGET_TOSHIO] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}トシオ『……あれ？\nなんだか あたまが ぐるぐる する……\pきんいろの シンボル……\nゲンさん……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}トシオ『……ゲンさん……\nかえさなきゃ…… こうかん しなきゃ……\pみんなが そう いうなら\nそれが いちばん…… なのに……\pでも ぼくは…… ぼくが……\nいっしょに いたいのは……\pちがう。 ぼくたちは。\nぼく は。 ぼく……\pあつい。 あたまが あつい。\pどうして。 どうして はなれるの？\nどうして いつか わかれる こを\lだいじに するの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}トシオ『……みんなの こえが\nとおく なっていく……\pみんなで かりて みんなで たたかう。\nだれと くんでも\lみんな なかま だったのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}トシオ『でも…… いまの しょうぶ\nゲンさんと いっしょに たたかえて\lうれしかった。\pつよいから じゃない。\nいっしょに たたかってきた\lゲンさん だから。\pいつかは ファクトリーに\nかえす ひが くる。\pそれでも いっしょに すごした\nじかんは なくならないよね。\p……ごめんね みんな。\nぼく いくよ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}トシオ『……ゲンさん…… みんなの……\nこうかん…… しなきゃ……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}てんしゅは トシオの くちに\nめしを どんどん はこんだ！\pトシオ『もがっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}トシオ『……あれ？\nここ どこ？\pあたまが すっきりしてる……\nみんなの こえが…… きこえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}トシオ『……ぜんぶ おぼえてるよ。\pみんなが すすめる ポケモンに\nかえれば いいんだって\lずっと おもってた。\pでも ぼく ゲンさんと たたかうのが\nいちばん たのしかったんだ。\p……たすけて くれて ありがとう！』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}トシオ『そうだ…… ゲンさんを\nファクトリーに かえさなくちゃ。\pかりた こ だもんね。\nほんとうは もっと いっしょに\lいたいけど……\pさいごに ありがとうって\nいって くるよ。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}トシオ『ねえ ポケナビ もってる？\nれんらくさき こうかん しようよ！』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}トシオ『ゲンさんを かえしたら\nりょかんに いくね。\pボスゴドラに きょうの しょうぶの\nはなしを してあげるんだ！』"),
    },
    [STORY_TARGET_EISUKE] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}エイスケ『……あれ？\nなんだか あたまが ぼーっと する……\pきんいろの シンボル……\nあと 1ターン……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}エイスケ『……あと 1ターン……\nあと 1ターン あれば……\pはんていでも みんなの かち。\nくやしく なんか ない…… はず……\pでも おれは…… おれが……\nたおしたかった……\pちがう。 おれたちは。\nおれ は。 おれ……\pあつい。 ゴールが みえない。\pどうして。 どうして はなれるの？\nどうして ひとりで\lはしりたがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}エイスケ『……みんなの こえが\nとおく なっていく……\pゴールの さきで\nまっててくれたのにな。\pみんなと いっしょなら\nまけても へいき だったのに。』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}エイスケ『でも…… いまの しょうぶ\nくやしかった。\pあの 1ターンも\nずっと くやしかった。\pけど もう いいんだ。\nあの しあいは あの しあいで\lおわったんだ。\pくやしい まま\nつぎの スタートを きる。\lそれが おれの レースだ。\p……わるいな みんな。\nおれ いくぜ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}エイスケ『……ゴール…… みんな……\nあと…… 1ターン……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}てんしゅは エイスケの くちに\nめしを どんどん はこんだ！\pエイスケ『むぐっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}エイスケ『……ん？\nここ どこだ？\pあたまが すっきりしてる……\nみんなの こえが…… きこえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}エイスケ『……ぜんぶ おぼえてるぜ。\pみんなと いっしょだと\nどんな けっかでも へいき だった。\pでも あの はんていの くやしさも\nおれの もの なんだよな。\pもう あの 1ターンは\nふりかえらない。\pつぎの レースに むけて\nはしりだすぜ。\p……たすけて くれて ありがとな！』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}エイスケ『そうだ ポケナビ もってるか？\nれんらくさき こうかん しようぜ！』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}エイスケ『おれ りょかんで\nからだを なおしてくる！\pまえの うみで およぎこみ だ！』"),
    },
    [STORY_TARGET_YAYOI] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}ヤヨイ『……あら？\nなんだか あたまが くらくらする……\pきんいろの シンボル……\nみっつの みち……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}ヤヨイ『……みっつの みち……\nみんなと おなじ みちを……\pおなじ かおりが すきなら\nけんかも しない…… はず……\pでも わたしは…… わたしが\nえらびたいのは……\pちがう。 わたしたちは。\nわたし は。 わたし……\pあつい。 かおりで むせかえりそう。\pどうして。 どうして はなれるの？\nどうして ちがう みちを\lえらびたがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}ヤヨイ『……みんなの こえが\nとおく なっていく……\pみんな おなじ かおりが すきで\nおなじ みちを えらんで……\pだれとも ぶつからずに\nいられたのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}ヤヨイ『でも…… あなたは\nわたしと すきな かおりが ちがった。\pそれでも あなたと はなすのは\nいやじゃ なかった。\lむしろ…… たのしかったの。\pおなじ みちを えらばなくても\nなかよく していられる。\pわたし そう しんじたい。\nわたしが えらびたい みちを\lあるいて いきたいの。\p……ごめんね みんな。\nわたし いくわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}ヤヨイ『……おなじ かおり……\nみんなと おなじ…… みち……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}てんしゅは ヤヨイの くちに\nめしを どんどん はこんだ！\pヤヨイ『んむっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}ヤヨイ『……あら？\nここは どこかしら。\pあたまが すっきりしてる……\nみんなの こえが…… きこえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}ヤヨイ『……ぜんぶ おぼえているわ。\pみんな おなじ かおりが すきで\nおなじ みちを えらんで……\pだれとも ぶつからない まいにちは\nほんとうに おだやか だった。\pでも これからは わたしが\nえらびたい みちを えらぶわ。\pだれかと ちがっても\nなかよく していられるって\lしんじて みたいの。\p……たすけて くれて ありがとう。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}ヤヨイ『……そういえば さっきの\nおだんご きびの やさしい\lかおりが したわ。』\pてんしゅ『かおり？\nうまいか どうかしか\lきに した ことが なかったぜ。』\pヤヨイ『ふふ。 おなじ ものを たべても\nきに なる ところは\lひとそれぞれ なのね。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}ヤヨイ『ねえ ポケナビは もってる？\nれんらくさき こうかん しましょう。\pすてきな かおりを みつけたら\nおしえるわね。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}ヤヨイ『わたし りょかんで\nおちゃを いれて まってるわ。\p{PLAYER}の すきな かおりの\nおちゃも よういして おくわね。』"),
    },
    [STORY_TARGET_YORIKO] =
    {
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}ヨリコ『……はくしゅ……\nみんなの はくしゅが きこえる……\pみんな おなじ ところで\nおなじ だけ…… それで いいの。\pでも わたしは…… わたしが\nみせたいのは…… みせ……\pちがう。 わたしたちは。\nわたし は。 わたし……\pあつい。 ライトが あつい。\pどうして。 どうして はなれるの？\nどうして ちがう はくしゅを\lほしがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}ヨリコ『……みんなの はくしゅが\nとおく なっていく……\pいやね……\nみんなと いっしょなら\lだれにも ごかい されなかったのに。』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}ヨリコ『でも まえに あなたは\nハブネークの しっぽを ほめてくれた。\pわたしの みせたかった ところとは\nちがったけれど……\p……うれしかったの。\nあなたが じぶんの めで みて\lそう おもって くれたから。\pみる ひとに よって\nちがって みえても いい。\pわたしと ハブネークが\nみせたい えんぎを したいの。\p……ごめんね みんな。\nわたし いくわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}ヨリコ『……はくしゅ……\nみんな おなじ……\lわたしの…… えんぎ……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}てんしゅは ヨリコの くちに\nめしを どんどん はこんだ！\pヨリコ『んっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}ヨリコ『……あら？\nここは どこかしら。\pあたまが すっきりしてる……\nみんなの はくしゅが…… きこえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}ヨリコ『……ぜんぶ おぼえているわ。\pみんなが おなじように わかってくれて\nおなじだけ はくしゅを くれて……\pだれにも ごかい されない ぶたいは\nほんとうに あんしん だった。\pでも これからは\nみる ひとに よって ちがう はくしゅを\lもらいに いくわ。\pハブネークと いっしょに\nわたしたちの えんぎで。\p……たすけて くれて ありがとう。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}ヨリコ『……さっきの おだんご\nしかくかったわね。\pシンオウちほうの コンテストを\nまわっていた ころ よく たべたの。\pシンオウでは きびだんごって\nしかくい ものなのよ。\p……なつかしい かたちだったわ。』\pてんしゅ『へえ…… シンオウじゃ\nしかくいのが ふつう なのか。\pうちのも むかしから この かたちでな。\nよそも そうだとは しらなかったぜ。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}ヨリコ『ねえ ポケナビは もってる？\nつぎの ぶたいが きまったら\lしらせたいの。\pれんらくさき こうかん しましょう。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}ヨリコ『わたし りょかんで\nすこし やすむわね。\pハブネークと つぎの えんぎを\nかんがえておくわ。 みにきてね。』"),
    },
};
