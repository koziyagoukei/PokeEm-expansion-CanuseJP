BattleFrontier_Lounge2_MapScripts::
	.byte 0

@ This NPC gives hints about a random facility or battle mode.
@ For battle modes he gives generic advice
@ For facilities, depending on how far the player has progressed he will say either
@    The name of the Frontier Brain there
@    The type and description of the 3 pokemon they use in their silver battle
@    The type and description of the 3 pokemon they use in their gold battle
BattleFrontier_Lounge2_EventScript_FrontierManiac::
	lock
	faceplayer
	goto_if_set FLAG_MET_BATTLE_FRONTIER_MANIAC, BattleFrontier_Lounge2_EventScript_AlreadyMetManiac
	setflag FLAG_MET_BATTLE_FRONTIER_MANIAC
	msgbox BattleFrontier_Lounge2_Text_FrontierManiacIntro, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge2_EventScript_GiveAdvice
	end

BattleFrontier_Lounge2_EventScript_AlreadyMetManiac::
	msgbox BattleFrontier_Lounge2_Text_SwingByForTheLatestWord, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge2_EventScript_GiveAdvice
	end

BattleFrontier_Lounge2_EventScript_GiveAdvice::
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_SINGLES, BattleFrontier_Lounge2_EventScript_BufferSingle
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_DOUBLES, BattleFrontier_Lounge2_EventScript_BufferDouble
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_MULTIS, BattleFrontier_Lounge2_EventScript_BufferMulti
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_LINK, BattleFrontier_Lounge2_EventScript_BufferMultiLink
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_DOME, BattleFrontier_Lounge2_EventScript_BufferBattleDome
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_FACTORY, BattleFrontier_Lounge2_EventScript_BufferBattleFactory
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PALACE, BattleFrontier_Lounge2_EventScript_BufferBattlePalace
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_ARENA, BattleFrontier_Lounge2_EventScript_BufferBattleArena
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PIKE, BattleFrontier_Lounge2_EventScript_BufferBattlePike
	call_if_eq VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_PYRAMID, BattleFrontier_Lounge2_EventScript_BufferBattlePyramid
@ <= FRONTIER_MANIAC_TOWER_LINK is any Battle Tower mode
	call_if_le VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_TOWER_LINK, BattleFrontier_Lounge2_EventScript_BattleTowerNews
@ >= FRONTIER_MANIAC_DOME is any facility other than Battle Tower
	call_if_ge VAR_FRONTIER_MANIAC_FACILITY, FRONTIER_MANIAC_DOME, BattleFrontier_Lounge2_EventScript_FacilityNews
	special ShowFrontierManiacMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge2_EventScript_BattleTowerNews::
	msgbox BattleFrontier_Lounge2_Text_BattleTowerIsHottest, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge2_EventScript_FacilityNews::
	msgbox BattleFrontier_Lounge2_Text_FacilityIsHottest, MSGBOX_DEFAULT
	return

BattleFrontier_Lounge2_EventScript_BufferSingle::
	bufferstdstring STR_VAR_1, STDSTRING_SINGLE
	return

BattleFrontier_Lounge2_EventScript_BufferDouble::
	bufferstdstring STR_VAR_1, STDSTRING_DOUBLE
	return

BattleFrontier_Lounge2_EventScript_BufferMulti::
	bufferstdstring STR_VAR_1, STDSTRING_MULTI
	return

BattleFrontier_Lounge2_EventScript_BufferMultiLink::
	bufferstdstring STR_VAR_1, STDSTRING_MULTI_LINK
	return

BattleFrontier_Lounge2_EventScript_BufferBattleDome::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_DOME
	return

BattleFrontier_Lounge2_EventScript_BufferBattleFactory::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_FACTORY
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePalace::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PALACE
	return

BattleFrontier_Lounge2_EventScript_BufferBattleArena::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_ARENA
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePike::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PIKE
	return

BattleFrontier_Lounge2_EventScript_BufferBattlePyramid::
	bufferstdstring STR_VAR_1, STDSTRING_BATTLE_PYRAMID
	return

BattleFrontier_Lounge2_EventScript_Maniac1::
	setvar VAR_0x8004, REGULAR_LOUNGE2_MANIAC_1
	goto FrontierStory_EventScript_RegularAside

BattleFrontier_Lounge2_EventScript_Maniac2::
	setvar VAR_0x8004, REGULAR_LOUNGE2_MANIAC_2
	goto FrontierStory_EventScript_RegularAside

BattleFrontier_Lounge2_EventScript_Maniac3::
	setvar VAR_0x8004, REGULAR_LOUNGE2_MANIAC_3
	goto FrontierStory_EventScript_RegularAside

BattleFrontier_Lounge2_EventScript_TriathleteF::
	setvar VAR_0x8004, REGULAR_LOUNGE2_TRIATHLETE
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge2_Text_FrontierManiacIntro::
	.string "{JPN}やあ！ フロンティアマニアだよ！\nみんなの 話しを 集めてるんだ。\pきみの 知らない 情報も\nあるかもしれないね！$"

BattleFrontier_Lounge2_Text_SwingByForTheLatestWord::
	.string "{JPN}また 来てくれたね！\n今度は どんな 話しを しようか。$"

@ Unused
BattleFrontier_Lounge2_Text_MyInformationsBeenUsefulRight::
	.string "{JPN}どう? どう? どう?\pぼくの 情報は もちろん\nすごーく さんこうに なってるだろ?$"

BattleFrontier_Lounge2_Text_FacilityIsHottest::
	.string "そうだねぇ……\p今は {STR_VAR_1}が\nアッつい みたいだね!$"

BattleFrontier_Lounge2_Text_BattleTowerIsHottest::
	.string "そうだねぇ……\p今は バトルタワー{STR_VAR_1}が\nアッつい みたいだね!$"

BattleFrontier_Lounge2_Text_SalonMaidenIsThere::
	.string "{JPN}知ってるかい?\pあそこは エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lタワータイクーンって 言う\l謎の トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_SalonMaidenSilverMons::
	.string "{JPN}タワータイクーンと 戦うなら\nどんな 相手にも そなえて おこう！\pてもちが いつも 同じとは\nかぎらないんだ。 そこが おもしろいね！$"

BattleFrontier_Lounge2_Text_SalonMaidenGoldMons::
	.string "{JPN}本気の タワータイクーンは\n出てくる ポケモンも てごわいぞ！\pひとつの タイプだけじゃなく\nチームぜんたいで たいさくしよう！$"

BattleFrontier_Lounge2_Text_DomeAceIsThere::
	.string "{JPN}知ってるかい?\pあそこは エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lドームスーパースターって 言う\lどハデな トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_DomeAceSilverMons::
	.string "{JPN}ドームスーパースターと 戦うなら\n試合まえの 情報が 大事さ！\p強そうな 名前に 驚かず\n2匹の 組みあわせを 見ておこう！$"

BattleFrontier_Lounge2_Text_DomeAceGoldMons::
	.string "{JPN}本気の ドームスーパースターだって\nいつも 同じ てもちとは かぎらない！\pその ときの 相手を 見て\nきみの さくせんを 決めるんだ！$"

BattleFrontier_Lounge2_Text_FactoryHeadIsThere::
	.string "{JPN}知ってるかい?\pあそこは エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lファクトリーヘッドって 言う\lやばい トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_FactoryHeadSilverMons::
	.string "{JPN}きみは もう ファクトリーヘッドと\n戦ったこと あるかい?\pあいつが 使う ポケモンは\nレンタルポケモンの\lなかから どれか 3匹!\lきみと ほぼ 同じ 条件 らしいぜ!$"

BattleFrontier_Lounge2_Text_FactoryHeadGoldMons::
	.string "{JPN}きみは 本気の ファクトリーヘッドと\n戦ったこと あるかい?\pあいつは 本気で 戦う ときでも\nレンタルポケモンの\lなかから どれか 3匹!\lきみと ほとんど 同じ 条件で\l戦う らしいぜ!$"

BattleFrontier_Lounge2_Text_PikeQueenIsThere::
	.string "{JPN}知ってるかい?\pあそこには エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lチューブクイーンって 言う\lこわーい トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_PikeQueenSilverMons::
	.string "{JPN}チューブクイーンに あうまえに\n残りの HPを 確かめよう！\pどんな ポケモンが 出てきても\n動けることが 大切だよ。$"

BattleFrontier_Lounge2_Text_PikeQueenGoldMons::
	.string "{JPN}本気の チューブクイーンに\nあえたら それだけでも すごいことさ！\p残してきた 力を\nそこで 全部 ぶつけてみよう！$"

BattleFrontier_Lounge2_Text_ArenaTycoonIsThere::
	.string "{JPN}知ってるかい?\pあそこには エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lアリーナキャプテンって 言う\lかわいい トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_ArenaTycoonSilverMons::
	.string "{JPN}アリーナキャプテンとの 勝負は\nポケモンの じゅんばんが 大切！\p交代で にげられないぶん\n次の 1匹まで 考えておこう。$"

BattleFrontier_Lounge2_Text_ArenaTycoonGoldMons::
	.string "{JPN}本気の アリーナキャプテンには\n3ターンの 使いかたで いどもう！\p技を あてることも\n残りの HPも 大事だからね。$"

BattleFrontier_Lounge2_Text_PalaceMavenIsThere::
	.string "{JPN}あそこには エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lパレスガーディアンって 言う\lあやしい トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_PalaceMavenSilverMons::
	.string "{JPN}パレスガーディアンに 挑むなら\n性格と 技を 確かめよう！\pてもちを あてるより\nきみの ポケモンを しることさ。$"

BattleFrontier_Lounge2_Text_PalaceMavenGoldMons::
	.string "{JPN}本気の パレスガーディアンの 前でも\nポケモンの 気持ちは 大事だよ。\pメガシンカや テラスタルの 準備も\n挑戦の 前に しておこう！$"

BattleFrontier_Lounge2_Text_PyramidKingIsThere::
	.string "{JPN}知ってるかい?\pあそこには エニシダが\nフロンティアブレーンって 呼んでる\lトレーナーの うちの 一人……\lピラミッドキングって 言う\lアツーい トレーナーが 収めてるのさ!$"

BattleFrontier_Lounge2_Text_PyramidKingSilverMons::
	.string "{JPN}ピラミッドキングを めざすなら\nまずは たんけんの 準備さ！\p持ちこめる 道具と\nチームの いきの ながさを 考えよう。$"

BattleFrontier_Lounge2_Text_PyramidKingGoldMons::
	.string "{JPN}本気の ピラミッドキングまで\nたどりついたら むねを はっていい！\pどんな 相手が 出てきても\nきみの たんけんの しめくくりだ！$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice1::
	.string "{JPN}ダブルバトルが できる 場所は\nいくつか あるけど やっぱり きほんは\lバトルタワーの ダブル バトルルーム!\pまずは ここから バトルフロンティアでの\nダブルバトルを 学んで いくと いいぜ!$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice2::
	.string "{JPN}ここでの 戦いには\n注意 しなよ!\pダブルバトル ならではの せんぽうで\n戦う トレーナーも いるみたいだぜ!$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice3::
	.string "{JPN}ここの 戦いで\nダブルバトルに 自信が ついたら\lほかの ダブルバトルが できる 場所に\lチャレンジ してみると いいかもな!$"

BattleFrontier_Lounge2_Text_MultiBattleAdvice::
	.string "{JPN}バトルサロンには\nいろんな トレーナーが 集まるんだぜ!\pもしかしたら きみの 友だちや\n弟子にも あえるかも しれないな!\lよーく 探して 見ると いいぜ!$"

BattleFrontier_Lounge2_Text_LinkMultiBattleAdvice::
	.string "{JPN}友だちが あつまったら\n通信マルチ バトルルーム!!\p強い 友だちと 一緒に あそべば\n強い トレーナーたち との\l戦いが 待ってるぜ!$"

