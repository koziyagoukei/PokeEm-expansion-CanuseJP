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

static const u8 sText_SeveralPeople[] = _("何人か");

static const struct StoryTargetText sStoryTargetTexts[STORY_TARGET_COUNT] =
{
    [STORY_TARGET_KAZUKI] =
    {
        .name = COMPOUND_STRING("カズキ"),
        .othersGone = COMPOUND_STRING("{JPN}カズキ『そういえば 最近\n{STR_VAR_2}の 声が 聞こえないんだ。\pでも 平気！\nみんなが いるもん！』"),
        .wishGone = COMPOUND_STRING("{JPN}カズキ『{STR_VAR_2}も\nこんな 気持ちに なったのかな……。』"),
        .wishShared = COMPOUND_STRING("{JPN}カズキ『みんなの 中にも\n胸が ざわざわしてる 人が\lいるみたい。\pぼくだけじゃ ないんだね。』"),
    },
    [STORY_TARGET_REIKA] =
    {
        .name = COMPOUND_STRING("レイカ"),
        .meet = COMPOUND_STRING("{JPN}レイカ『あら ごきげんよう。\nドームに いらしたのね。\pわたくし レイカと もうしますの。\nこの子は 相棒の マッスグマ。\pドームでは 3びきの 中から\n2ひきを 選んで 戦いますのよ。\p……ふふ。 でも 今は\n誰が でても へいきですの。\pみなさんと 心が\n繋がって いますから。\p選ばれた こも 選ばれなかった こも\n気持ちは 全部 同じですのよ。』"),
        .happy = COMPOUND_STRING("{JPN}レイカ『マッスグマの 気持ちも\nみなさんの 気持ちも\l全部 分かりますの。\pこんなに 穏やかな 気分\n初めてですわ。』"),
        .wish = COMPOUND_STRING("{JPN}レイカ『……あなたの シンボルの 試合\n見ていましたわ。\pあの とき わたくし\nきゅうに 思い出しましたの。\p前に ドームで この マッスグマを\n控えに した ことを。\pみなさんと 繋がっていれば\nマッスグマの 気持ちも 分かるはず。\pなのに 今は なぜか\n分からない 気がするんですの。\p……ねえ {PLAYER}さん。\p相手の 気持ちが 全部\n分からなくても……\lあなたは 相棒を 信じられますの？\p……ごめんなさい。\nへんな ことを ききましたわね。』"),
        .wishAgain = COMPOUND_STRING("{JPN}レイカ『控えに した あの ひ\nマッスグマは なにを\l思っていたのかしら。\p分からない ことが 怖いのか\n分からなくても いいのか……\p……わたくし まだ\n決められませんの。』"),
        .othersGone = COMPOUND_STRING("{JPN}レイカ『そういえば 最近\n{STR_VAR_2}の 声が\l聞こえませんの。\pでも 大丈夫。\nみなさんが いますもの。』"),
        .wishGone = COMPOUND_STRING("{JPN}レイカ『{STR_VAR_2}の 声が\n聞こえなく なったのも……\lこういう 気持ちから だったのかしら。』"),
        .wishShared = COMPOUND_STRING("{JPN}レイカ『みなさんの 中にも\nざわざわしている かたが いますわ。\pわたくしだけでは ないのね……。』"),
    },
    [STORY_TARGET_IKUE] =
    {
        .name = COMPOUND_STRING("イクエ"),
        .meet = COMPOUND_STRING("{JPN}イクエ『こんにちは！\n私 ブリーダーの イクエ。\pこの子は エネコロロ。\nずっと 一緒に 育ててきたの。\pパレスでは ポケモンが 自分で\n考えて 戦うでしょう？\pでも 今は 心配 ないの。\nエネコロロの 気持ちは\l全部 分かるから。\pみんなと 繋がってると\nなにを したいか すぐ 分かるのよ。』"),
        .happy = COMPOUND_STRING("{JPN}イクエ『エネコロロが 今\nなにを したいのか……\lほら 分かる。\pみんなと 一緒だと\n育てるのも かんたんね。』"),
        .wish = COMPOUND_STRING("{JPN}イクエ『あなたの パレスの 試合\n見たわ。\pあなたの ポケモン 自分で 考えて\n楽しそうに うごいていた。\p……ねえ 聞きたい ことが あるの。\pあなたは 相棒の 好きな 遊びを\nどうやって 知ったの？\p……そう。 一緒に 過ごして\n少しずつ 知ったのね。\p私…… エネコロロの 好きな 遊び\n本当は 知らないのかも。\pみんなの 気持ちが 流れ込むから\n知っている 気に なっていただけ……？』"),
        .wishAgain = COMPOUND_STRING("{JPN}イクエ『一緒に 過ごして\n少しずつ 知る……。\pエネコロロの こと 私\nもう いちど 知りたいな。』"),
        .othersGone = COMPOUND_STRING("{JPN}イクエ『そういえば みんなの 中から\n{STR_VAR_2}の 声が きえたの。\p不思議ね。 でも 寂しくないわ。\nみんなが いるもの。』"),
        .wishGone = COMPOUND_STRING("{JPN}イクエ『{STR_VAR_2}の 声が\n聞こえなく なった 訳……\pいまなら ちょっとだけ\n分かる 気がするわ。』"),
        .wishShared = COMPOUND_STRING("{JPN}イクエ『みんなの 中にも\n私と 同じように\l迷っている 人が いるみたい。』"),
    },
    [STORY_TARGET_EISUKE] =
    {
        .name = COMPOUND_STRING("エイスケ"),
        .meet = COMPOUND_STRING("{JPN}エイスケ『よう！ おれは エイスケ。\nトライアスロンで 鍛えてるんだ！\p相棒は スターミー。\n一緒に 泳いで 鍛えてきたぜ。\pアリーナは 3ターンで 決着が\nつかなきゃ 判定に なるんだ。\p前は 判定で 負けると\n悔しかった 気がするけど……\p今は 平気さ！\n誰が 勝っても みんなの 勝ち\lだからな！』"),
        .happy = COMPOUND_STRING("{JPN}エイスケ『勝っても 負けても\nみんなで 一緒！\pゴールの さきで みんなが\n待っててくれるって 最高だぜ！』"),
        .wish = COMPOUND_STRING("{JPN}エイスケ『みたぜ！ シンボルを かけた\nあんたの 勝負！\p……あれを 見てたら\n昔の 試合を 思い出したんだ。\pスターミーと でた アリーナで\n3ターン 戦って 判定 負け。\pあと 少しで 倒せたんだ。\nあと 1ターン あれば……！\p……へんだな。 みんなの かちなら\n悔しく ないはず なのに。\pあの 1ターンが\n頭から 離れないんだ。』"),
        .wishAgain = COMPOUND_STRING("{JPN}エイスケ『あと 1ターン あれば……。\p……おわった しあいなのに\nなんで こんなに\l気に なるんだろうな。』"),
        .othersGone = COMPOUND_STRING("{JPN}エイスケ『そういや 最近\n{STR_VAR_2}の 声が 聞こえないな。\pま いいか！ みんなが いれば\n寂しくないぜ！』"),
        .wishGone = COMPOUND_STRING("{JPN}エイスケ『{STR_VAR_2}の 声が\nきえたのも……\lこんな もやもやの せい なのかな。』"),
        .wishShared = COMPOUND_STRING("{JPN}エイスケ『みんなの 中でも\n誰かが もやもや してるみたいだ。\pおれだけじゃ ないんだな。』"),
    },
    [STORY_TARGET_TOSHIO] =
    {
        .name = COMPOUND_STRING("トシオ"),
        .meet = COMPOUND_STRING("{JPN}トシオ『やあ！ ぼく トシオ。\n怪獣みたいな ポケモンが\lだいすきなんだ！\pぼくの 相棒は ボスゴドラ。\nココドラの ころから 一緒なんだ。\pファクトリーでは ポケモンを\nかりて 戦うんだよ。\pこの 前 かりた トリデプス\nゲンさんって いうんだけど\l一緒に なんども 勝ったんだ！\pみんなで かりて みんなで 戦う。\n誰と くんでも みんな 仲間さ！』"),
        .happy = COMPOUND_STRING("{JPN}トシオ『ゲンさんの 硬い 頭\nかっこいいんだ！\pみんなも そう 思ってる。\nだから ぼくも 嬉しいんだよ。』"),
        .wish = COMPOUND_STRING("{JPN}トシオ『きみの シンボルの 勝負\n見てたよ！\p……あのね。 みんなが いうんだ。\nもっと 強そうな ポケモンに\l交換 したほうが いいって。\pみんなが そう いうなら\nそれが 一番の はず なんだけど……\pなんでだろう。\nゲンさんを かえしたく ないんだ。\p強いから じゃないんだ。\n一緒に 戦った あの子だから\l……なのかな。』"),
        .wishAgain = COMPOUND_STRING("{JPN}トシオ『交換 したほうが いいって\nみんなは いうんだけど……\pぼく まだ ゲンさんと\n一緒に いたいんだ。』"),
        .othersGone = COMPOUND_STRING("{JPN}トシオ『そういえば {STR_VAR_2}の 声\n最近 聞こえないなあ。\pでも 平気！\nみんなが いるからね！』"),
        .wishGone = COMPOUND_STRING("{JPN}トシオ『{STR_VAR_2}も こんな 気持ちで\nみんなから 離れたのかなあ。』"),
        .wishShared = COMPOUND_STRING("{JPN}トシオ『みんなの 中にも\nぼくみたいに 迷ってる 人が\lいるみたい。』"),
    },
    [STORY_TARGET_YAYOI] =
    {
        .name = COMPOUND_STRING("ヤヨイ"),
        .meet = COMPOUND_STRING("{JPN}ヤヨイ『いらっしゃい。\n私 ヤヨイ。 アロマが 好きなの。\pこの子は ロズレイド。\nいつも いい 香りが するのよ。\pチューブでは 道が 3つに\nわかれているでしょう？\pでも 迷わないの。\nみんなの 好きな 道が 分かるから。\pみんな 同じ 香りが 好きで\n同じ 道を えらぶの。\pだから けんかも ないのよ。』"),
        .happy = COMPOUND_STRING("{JPN}ヤヨイ『ロズレイドの 香り\nみんな 好きだって。\p同じ ものが すきって\nほっと するわね。』"),
        .wish = COMPOUND_STRING("{JPN}ヤヨイ『あなたの シンボルの 勝負\n見せて もらったわ。\p……ねえ ひとつ 聞いても いい？\nあなたは どんな 香りが 好き？』\p{PLAYER}は 好きな 香りを\n答えた。\pヤヨイ『……あら。\n私の 好きな 香りと 違うのね。\pみんなとは いつも 同じ なのに\nあなたとは 違う。\p同じ ものを すきじゃ なくても\nなかよく していられるのかしら……。』"),
        .wishAgain = COMPOUND_STRING("{JPN}ヤヨイ『好きな 香りが 違っても\n一緒に いられるのかしら。\p……不思議。 怖いような\n楽しみな ような 気持ち。』"),
        .othersGone = COMPOUND_STRING("{JPN}ヤヨイ『そういえば {STR_VAR_2}の\n声が 聞こえなく なったわ。\pでも みんな 同じ 気持ち。\nだから 寂しくないの。』"),
        .wishGone = COMPOUND_STRING("{JPN}ヤヨイ『{STR_VAR_2}は\n違う 道を 選んだのかしら。』"),
        .wishShared = COMPOUND_STRING("{JPN}ヤヨイ『みんなの 中にも\n違う 香りを 探している 人が\lいるみたい。』"),
    },
    [STORY_TARGET_NOBUO] =
    {
        .name = COMPOUND_STRING("ノブオ"),
        .meet = COMPOUND_STRING("{JPN}ノブオ『おう。おれは ノブオ。\n遺跡を調べるのが 好きでな。\pこいつは 相棒のサンドパン。\n一緒に あちこち掘ってきたんだ。\pピラミッドは 真っ暗だろう？\n……だが 今は迷わんのさ。\p誰かの歩いた道が 分かる。\n誰かが見つけた階段も 分かる。\p助けてくれって 言うまでもない。\n……便利なもんだよなあ。』"),
        .happy = COMPOUND_STRING("{JPN}ノブオ『暗くたって 平気さ。\n皆の記憶が 道を教えてくれる。\p……サンドパンも 分かってる。\nおれ達は 迷わないんだ。』"),
        .wish = COMPOUND_STRING("{JPN}ノブオ『おう きみか。\nシンボルの勝負 見てたぞ。\p……なあ。\n上の階への道を 知らないか。\pさっきから どうにも分からなくてな。\n……え？ おれが迷ってるって？\p……そうか。きみには\n言わなきゃ 分からなかったのか。\p皆には 黙っていたって伝わる。\nだが きみは違う。\p今 おれが聞いたから\nきみは 足を止めてくれたんだな。』"),
        .wishAgain = COMPOUND_STRING("{JPN}ノブオ『……教えてくれって\n言うだけなんだよな。\p黙って分かる方が 楽だ。\nそりゃ 楽なんだが……。』"),
        .othersGone = COMPOUND_STRING("{JPN}ノブオ『そういや {STR_VAR_2}の声が\n聞こえなくなったな。\p……まあ 皆がいれば\n道には 迷わんさ。』"),
        .wishGone = COMPOUND_STRING("{JPN}ノブオ『{STR_VAR_2}も\n誰かに 道を聞いたのかね。\p……今は そうしなきゃ\n分からないんだもんな。』"),
        .wishShared = COMPOUND_STRING("{JPN}ノブオ『皆の中にも\n道に迷ってるやつが いるらしい。\p……知ってるはずなのに\n分からないんだな。お互いに。』"),
    },
    [STORY_TARGET_YORIKO] =
    {
        .name = COMPOUND_STRING("ヨリコ"),
        .meet = COMPOUND_STRING("{JPN}ヨリコ『あら こんにちは。\n私は ヨリコ。\p相棒の ハブネークと\nコンテストに でているの。\p今の コンテストは すてきよ。\nみんなが 同じように 分かってくれて\l同じように 拍手 してくれるの。\p誰にも 誤解 されない えんぎって\nとっても 安心 するわ。』"),
        .happy = COMPOUND_STRING("{JPN}ヨリコ『拍手の おおきさも\nみんな 一緒。\pハブネークも きっと\n嬉しい はずだわ。』"),
        .wish = COMPOUND_STRING("{JPN}ヨリコ『マスターランク 優勝\nおめでとう。\p私と ハブネークも\n同じ 舞台に でていたのよ。\p……ねえ 私達の 演技\nどこが よかった？』\p{PLAYER}は ハブネークの しっぽの\n動きが よかったと 答えた。\pヨリコ『……しっぽ？\p私が 見せたかったのは\nハブネークの 鋭い 眼差し なのに。\pみんなは いつも 私の\n見せたい ところを 分かってくれるのに……\p……どうしてかしら。\nなんだか 胸が ざわざわするわ。』"),
        .wishAgain = COMPOUND_STRING("{JPN}ヨリコ『見る ひとによって\nよい ところが 違う……。\pそれって 怖い ことの はずよね。\nなのに あの 言葉が\l忘れられないの。』"),
        .othersGone = COMPOUND_STRING("{JPN}ヨリコ『そういえば 最近\n{STR_VAR_2}の 声が 聞こえないの。\pでも 大丈夫。\n拍手は みんなから もらえるもの。』"),
        .wishGone = COMPOUND_STRING("{JPN}ヨリコ『{STR_VAR_2}も 違う 拍手を\nもとめて いったのかしら。』"),
        .wishShared = COMPOUND_STRING("{JPN}ヨリコ『みんなの 中にも\n私と 同じように\lざわざわしている 人が いるわ。』"),
    },
};

// Their release battle and cure (everyone but Kazuki, whose scenes are scripted on their own), by
// RESCUE_TEXT_*. Someone without a RESCUE_TEXT_FEVER does not have these scenes yet.
static const u8 *const sStoryTargetRescueTexts[STORY_TARGET_COUNT][RESCUE_TEXT_COUNT] =
{
    [STORY_TARGET_REIKA] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}レイカ『……あら？\nなんだか 頭が ぼんやりしますわ……\p金色の シンボル……\n控えの……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}レイカ『……マッスグマ……\n控え…… 選ばなかった……\pみなさんと 繋がっていれば\n全部 分かる はず……\p分かる はず ですのに……\pでも わたくしは…… わたくしが\n知りたいのは……\p違いますわ。 わたくしたちは。\nわたくし は。 わたくし……\p熱い。 頭が 熱い。\pどうして。 どうして 離れるの？\nどうして 分からない ものを\l信じるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}レイカ『……みなさんの 声が\n遠く なっていきますわ……\pマッスグマの 気持ちも\n誰の 気持ちも\l全部 分かったのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}レイカ『……今の勝負で \n分かりましたの。\pあの日 控えにしたマッスグマを \nわたくしは ずっと大事に思っていた。\pあの子が 何を思っていたのか。\nそれは 今も 分かりませんわ。\p……でも 信じたいの。\n分からないままでも あの子を。\pごめんなさい 皆さん。\nわたくしは 行きますわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}レイカ『……マッスグマ……\nみなさんの 気持ち…… 全部……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は レイカの 口に\n飯を どんどん はこんだ！\pレイカ『んむっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}レイカ『……まあ。\nここは どちらかしら。\p頭が すっきりしていますわ……\nみなさんの 声が…… 聞こえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}レイカ『……全部 覚えていますわ。\pマッスグマの 気持ちも\nみなさんの 気持ちも\l全部 分かって……\p本当に 穏やか でしたの。\pでも これからは 分からない ままでも\nあの子と 一緒に\l歩いて いきますわ。\p……助けて くださって\nありがとうございます。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}レイカ『そうですわ。\nポケナビを おもちかしら？\p連絡先を 交換 しましょう。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}レイカ『わたくし 旅館で\n休ませて いただきますわ。\pマッスグマと おさんぽ するのが\n楽しみ ですの。』"),
    },
    [STORY_TARGET_IKUE] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}イクエ『……あれ？\nなんだか 頭が ふわふわする……\p金色の シンボル……\nエネコロロの…… 気持ち……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}イクエ『……エネコロロ……\n今 なにを したいの……\pみんなと 繋がってれば\n全部 分かる…… 分かるのに……\pでも 私は…… 私が\n知りたいのは……\p違う。 私達は。\n私 は。 私……\p熱い。 頭が 熱い。\pどうして。 どうして 離れるの？\nどうして 分からない 相手を\l育てたがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}イクエ『……みんなの 声が\n遠く なっていく……\pエネコロロの 気持ちも\nみんなの 気持ちも\l流れ込んで きてたのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}イクエ『……今 見ていたの。\nあなたのポケモンのこと。\p言われた通りに 動かなくても \nあなたと 一緒に戦っていた。\pエネコロロも きっと \n私とは 違うことを考えてる。\pそれを 少しずつ知りたいの。\n一緒に過ごして 育てていきたい。\p……ごめんね 皆。\n私 行くわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}イクエ『……エネコロロ……\nみんなの…… 全部…… 分かる……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は イクエの 口に\n飯を どんどん はこんだ！\pイクエ『んぐっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}イクエ『……あれ？\nここ どこかしら。\p頭が すっきりしてる……\nみんなの 声が…… 聞こえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}イクエ『……全部 覚えてる。\pエネコロロの したいことが\nいつでも 分かって……\l育てるのも かんたん だった。\pでも あれは 分かった 気に\nなっていた だけ なのかも。\pこれからは 一緒に 過ごして\n少しずつ 知っていくわ。\p……助けて くれて ありがとう。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}イクエの モンスターボールから\nエネコロロが 飛び出した！\pエネコロロは イクエの 前に\nすわりこむと ボールから\lふいっと 顔を そむけた。\pイクエ『……エネコロロ？\pもしかして…… もう\n戦いたく ないの？』\pエネコロロは イクエの 手に\n頭を 擦り寄せた。\pイクエ『……そう。\nそれが あなたの 気持ち なのね。\p分かったわ。 私\nあなたが 選んだ ことを\l大事に したい。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}イクエ『ねえ ポケナビ もってる？\n連絡先 交換 しましょう。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}イクエ『私 旅館で\nエネコロロと ゆっくり するわ。\pこの子が なにを して\n遊びたいか みつけたいの。』"),
    },
    [STORY_TARGET_TOSHIO] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}トシオ『……あれ？\nなんだか 頭が ぐるぐる する……\p金色の シンボル……\nゲンさん……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}トシオ『……ゲンさん……\nかえさなきゃ…… 交換 しなきゃ……\pみんなが そう いうなら\nそれが 一番…… なのに……\pでも ぼくは…… ぼくが……\n一緒に いたいのは……\p違う。 ぼくたちは。\nぼく は。 ぼく……\p熱い。 頭が 熱い。\pどうして。 どうして 離れるの？\nどうして いつか わかれる こを\l大事に するの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}トシオ『……みんなの 声が\n遠く なっていく……\pみんなで かりて みんなで 戦う。\n誰と くんでも\lみんな 仲間 だったのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}トシオ『……今の勝負 \nゲンさんと 戦えて嬉しかった。\p強いからじゃない。\n一緒に戦ってきた ゲンさんだから。\p……借りたポケモンだもんね。\nいつかは ファクトリーに返さなくちゃ。\pでも 一緒にいた時間まで\nなくなる訳じゃ ないよね。\p……ごめんね 皆。\nぼく 行くよ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}トシオ『……ゲンさん…… みんなの……\n交換…… しなきゃ……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は トシオの 口に\n飯を どんどん はこんだ！\pトシオ『もがっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}トシオ『……あれ？\nここ どこ？\p頭が すっきりしてる……\nみんなの 声が…… 聞こえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}トシオ『……全部 覚えてるよ。\pみんなが すすめる ポケモンに\nかえれば いいんだって\lずっと 思ってた。\pでも ぼく ゲンさんと 戦うのが\n一番 楽しかったんだ。\p……助けて くれて ありがとう！』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}トシオ『そうだ…… ゲンさんを\nファクトリーに かえさなくちゃ。\pかりた こ だもんね。\n本当は もっと 一緒に\lいたいけど……\p最後に ありがとうって\nいって くるよ。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}トシオ『ねえ ポケナビ もってる？\n連絡先 交換 しようよ！』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}トシオ『ゲンさんを かえしたら\n旅館に いくね。\pボスゴドラに 今日の 勝負の\n話を してあげるんだ！』"),
    },
    [STORY_TARGET_EISUKE] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}エイスケ『……あれ？\nなんだか 頭が ぼーっと する……\p金色の シンボル……\nあと 1ターン……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}エイスケ『……あと 1ターン……\nあと 1ターン あれば……\p判定でも みんなの 勝ち。\n悔しく なんか ない…… はず……\pでも おれは…… おれが……\n倒したかった……\p違う。 おれたちは。\nおれ は。 おれ……\p熱い。 ゴールが 見えない。\pどうして。 どうして 離れるの？\nどうして 一人で\l走りたがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}エイスケ『……みんなの 声が\n遠く なっていく……\pゴールの さきで\n待っててくれたのにな。\pみんなと 一緒なら\n負けても 平気 だったのに。』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}エイスケ『……悔しいな。\n今の勝負も あの1ターンも。\p……けど あの試合は\nもう 終わったんだよな。\p悔しくなくなるまで\n立ち止まってなくたって いい。\p悔しいまま 次のスタートを切る。\n……それが おれのレースだ。\p悪いな 皆。\nおれは 行くぜ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}エイスケ『……ゴール…… みんな……\nあと…… 1ターン……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は エイスケの 口に\n飯を どんどん はこんだ！\pエイスケ『むぐっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}エイスケ『……ん？\nここ どこだ？\p頭が すっきりしてる……\nみんなの 声が…… 聞こえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}エイスケ『……全部 覚えてるぜ。\pみんなと 一緒だと\nどんな けっかでも 平気 だった。\pでも あの 判定の 悔しさも\nおれの もの なんだよな。\pもう あの 1ターンは\n振り返らない。\p次の レースに むけて\n走り出すぜ。\p……助けて くれて ありがとな！』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}エイスケ『そうだ ポケナビ もってるか？\n連絡先 交換 しようぜ！』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}エイスケ『おれ 旅館で\n身体を 休めてくる！\p前の 海で 泳ぎ込み だ！』"),
    },
    [STORY_TARGET_YAYOI] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}ヤヨイ『……あら？\nなんだか 頭が くらくらする……\p金色の シンボル……\nみっつの 道……』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}ヤヨイ『……みっつの 道……\nみんなと 同じ 道を……\p同じ 香りが すきなら\nけんかも しない…… はず……\pでも 私は…… 私が\n選びたいのは……\p違う。 私達は。\n私 は。 私……\p熱い。 香りで むせかえりそう。\pどうして。 どうして 離れるの？\nどうして 違う 道を\l選びたがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}ヤヨイ『……みんなの 声が\n遠く なっていく……\pみんな 同じ 香りが 好きで\n同じ 道を 選んで……\pだれとも ぶつからずに\nいられたのに……』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}ヤヨイ『……あなたは 私と\n好きな香りが 違ったわね。\pそれなのに 話していて\n嫌じゃなかった。\l……むしろ 楽しかったの。\p同じ道を 選ばなくても \n一緒にいられるのかもしれない。\p私 それを 確かめてみたい。\n自分で選んだ道を 歩いて。\p……ごめんね 皆。\n私 行くわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}ヤヨイ『……同じ 香り……\nみんなと 同じ…… 道……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は ヤヨイの 口に\n飯を どんどん はこんだ！\pヤヨイ『んむっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}ヤヨイ『……あら？\nここは どこかしら。\p頭が すっきりしてる……\nみんなの 声が…… 聞こえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}ヤヨイ『……全部 覚えているわ。\pみんな 同じ 香りが 好きで\n同じ 道を 選んで……\pだれとも ぶつからない 毎日は\n本当に 穏やか だった。\pでも これからは 私が\n選びたい 道を えらぶわ。\p誰かと 違っても\nなかよく していられるって\l信じて みたいの。\p……助けて くれて ありがとう。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}ヤヨイ『……そういえば さっきの\nおだんご きびの 優しい\l香りが したわ。』\p店主『香り？\nうまいか どうかしか\l気に した ことが なかったぜ。』\pヤヨイ『ふふ。 同じ ものを 食べても\n気に なる ところは\lひとそれぞれ なのね。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}ヤヨイ『ねえ ポケナビは もってる？\n連絡先 交換 しましょう。\pすてきな 香りを みつけたら\nおしえるわね。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}ヤヨイ『私 旅館で\nお茶を いれて 待ってるわ。\p{PLAYER}の 好きな 香りの\nお茶も よういして おくわね。』"),
    },
    [STORY_TARGET_NOBUO] =
    {
        [RESCUE_TEXT_FEVER_STARTING] = COMPOUND_STRING("{JPN}ノブオ『……あれ。\n階段は どっちだったか……。\p金色のシンボル……\n皆は 知ってるはずだろ……？』"),
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}ノブオ『……右だ。\nいや 左だった……。\p皆が 歩いた道だ。\nおれ達は 知ってる。迷わない。\p……なのに どうして\nおれは 道を聞こうとしてる？\p違う。おれ達は。\nおれ は。……おれ……。\p熱い。暗くて 何も見えない。\pどうして 離れるの？\nどうして 道も知らないまま\l一人で行こうとするの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}ノブオ『……静かになったな。\nどっちへ行けば いいのか……。\p皆といた時は\nこんなこと 考えなくてよかった。\p……怖いもんだな。\n知らないってのは。』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}ノブオ『……けど きみは\nおれの声を 聞いてくれた。\p助けてくれって 言えばいい。\n道を教えてくれって。\p……昔は そうしてたんだよな。\nこいつと二人で 迷っちゃあ\l誰かに 声をかけてさ。\pああ。分からなくても いいんだ。\n黙ってなくたって いい。\p……悪いな 皆。\nおれは そっちへ行くよ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}ノブオ『……おい……\n誰か 道を……。\p……聞こえてるか……？』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は ノブオの口に\n飯を どんどん運んだ！\pノブオ『むぐっ……！\n……もぐ……。』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}ノブオ『……ああ。\nここは……飯屋か。\pずいぶん 静かだな。\n……皆の声が しない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}ノブオ『……覚えてるよ。\n皆と繋がってた時のこと。\p歩いたことのない道まで 知ってた。\n迷うのも 怖がるのも忘れてた。\p……だが 今は分からん。\nここから先 どこへ行けばいいのか。\pだから 聞くんだ。\nきみにも 他の誰かにも。\p……ああ 助かった。\nありがとうな。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}ノブオ『……水を もらえないか。』\p店主『おう。』\p店主は ノブオに水を渡した。\pノブオ『……冷たいな。\pおれが言って あんたが渡してくれた。\n……それだけのことだったんだな。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}ノブオ『ポケナビ 持ってるか？\n連絡先を 交換しようぜ。\p迷った時に 頼れる相手は\n多い方が いいからな。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}ノブオ『サンドパンと一緒に\n旅館で 休ませてもらうよ。\p……そっちへ行く船は\n港で聞けば いいんだよな。』"),
    },
    [STORY_TARGET_YORIKO] =
    {
        [RESCUE_TEXT_FEVER] = COMPOUND_STRING("{JPN}ヨリコ『……拍手……\nみんなの 拍手が 聞こえる……\pみんな 同じ ところで\n同じ だけ…… それで いいの。\pでも 私は…… 私が\n見せたいのは…… 見せ……\p違う。 私達は。\n私 は。 私……\p熱い。 ライトが 熱い。\pどうして。 どうして 離れるの？\nどうして 違う 拍手を\lほしがるの？』"),
        [RESCUE_TEXT_VOICES_FADE] = COMPOUND_STRING("{JPN}ヨリコ『……みんなの 拍手が\n遠く なっていく……\pいやね……\nみんなと 一緒なら\l誰にも 誤解 されなかったのに。』"),
        [RESCUE_TEXT_CHOOSES] = COMPOUND_STRING("{JPN}ヨリコ『……あなた 前に\nハブネークの尻尾を 褒めてくれたわね。\p見せたかったのは そこじゃないのに。\n……それでも 嬉しかったの。\pあなたが 自分の目で見て \nそう思ってくれたから。\p見る人によって 違って見えてもいい。\n私と ハブネークの演技をしたい。\p……ごめんね 皆。\n私 行くわ。』"),
        [RESCUE_TEXT_DELIRIOUS] = COMPOUND_STRING("{JPN}ヨリコ『……拍手……\nみんな 同じ……\l私の…… 演技……』"),
        [RESCUE_TEXT_EATS] = COMPOUND_STRING("{JPN}店主は ヨリコの 口に\n飯を どんどん はこんだ！\pヨリコ『んっ…… もぐ……』"),
        [RESCUE_TEXT_WAKES_UP] = COMPOUND_STRING("{JPN}ヨリコ『……あら？\nここは どこかしら。\p頭が すっきりしてる……\nみんなの 拍手が…… 聞こえない。』"),
        [RESCUE_TEXT_REMEMBERS] = COMPOUND_STRING("{JPN}ヨリコ『……全部 覚えているわ。\pみんなが 同じように 分かってくれて\n同じだけ 拍手を くれて……\p誰にも 誤解 されない 舞台は\n本当に 安心 だった。\pでも これからは\n見る 人に よって 違う 拍手を\lもらいに いくわ。\pハブネークと 一緒に\n私達の 演技で。\p……助けて くれて ありがとう。』"),
        [RESCUE_TEXT_AFTER_CURE] = COMPOUND_STRING("{JPN}ヨリコ『……さっきの おだんご\n四角かったわね。\pシンオウちほうの コンテストを\nまわっていた ころ よく 食べたの。\pシンオウでは きびだんごって\n四角い ものなのよ。\p……なつかしい かたちだったわ。』\p店主『へえ…… シンオウじゃ\n四角いのが 普通 なのか。\pうちのも 昔から この かたちでな。\nよそも そうだとは しらなかったぜ。』"),
        [RESCUE_TEXT_ASKS_POKENAV] = COMPOUND_STRING("{JPN}ヨリコ『ねえ ポケナビは もってる？\n次の 舞台が きまったら\lしらせたいの。\p連絡先 交換 しましょう。』"),
        [RESCUE_TEXT_GOES_TO_INN] = COMPOUND_STRING("{JPN}ヨリコ『私 旅館で\n少し 休むわね。\pハブネークと 次の 演技を\n考えておくわ。 みにきてね。』"),
    },
};
