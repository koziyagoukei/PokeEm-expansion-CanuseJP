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
        .meet = COMPOUND_STRING("{JPN}トシオ『やあ！ ぼく トシオ。\nかいじゅうみたいな ポケモンが\lだいすきなんだ！\pぼくの あいぼうは ボスゴドラ。\nココドラの ころから いっしょなんだ。\pファクトリーでは ポケモンを\nかりて たたかうんだよ。\pこの まえ かりた トリデプスと\nいっしょに なんども かったんだ！\pみんなで かりて みんなで たたかう。\nだれと くんでも みんな なかまさ！』"),
        .happy = COMPOUND_STRING("{JPN}トシオ『トリデプスの かたい あたま\nかっこいいんだ！\pみんなも そう おもってる。\nだから ぼくも うれしいんだよ。』"),
        .wish = COMPOUND_STRING("{JPN}トシオ『きみの シンボルの しょうぶ\nみてたよ！\p……あのね。 みんなが いうんだ。\nもっと つよそうな ポケモンに\lこうかん したほうが いいって。\pみんなが そう いうなら\nそれが いちばんの はず なんだけど……\pなんでだろう。\nトリデプスを かえしたく ないんだ。\pつよいから じゃないんだ。\nいっしょに たたかった あの こだから\l……なのかな。』"),
        .wishAgain = COMPOUND_STRING("{JPN}トシオ『こうかん したほうが いいって\nみんなは いうんだけど……\pぼく まだ トリデプスと\nいっしょに いたいんだ。』"),
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
