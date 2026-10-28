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
	.string "{JPN}やあ！ フロンティアマニアだよ！\nみんなの はなしを あつめてるんだ。\pきみの しらない じょうほうも\nあるかもしれないね！$"

BattleFrontier_Lounge2_Text_SwingByForTheLatestWord::
	.string "{JPN}また きてくれたね！\nこんどは どんな はなしを しようか。$"

@ Unused
BattleFrontier_Lounge2_Text_MyInformationsBeenUsefulRight::
	.string "{JPN}どう? どう? どう?\pぼくの じょうほうは もちろん\nすごーく さんこうに なってるだろ?$"

BattleFrontier_Lounge2_Text_FacilityIsHottest::
	.string "そうだねぇ……\pいまは {STR_VAR_1}が\nアッつい みたいだね!$"

BattleFrontier_Lounge2_Text_BattleTowerIsHottest::
	.string "そうだねぇ……\pいまは バトルタワー{STR_VAR_1}が\nアッつい みたいだね!$"

BattleFrontier_Lounge2_Text_SalonMaidenIsThere::
	.string "{JPN}しってるかい?\pあそこは エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lタワータイクーンって いう\lなぞの トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_SalonMaidenSilverMons::
	.string "{JPN}タワータイクーンと たたかうなら\nどんな あいてにも そなえて おこう！\pてもちが いつも おなじとは\nかぎらないんだ。 そこが おもしろいね！$"

BattleFrontier_Lounge2_Text_SalonMaidenGoldMons::
	.string "{JPN}ほんきの タワータイクーンは\nでてくる ポケモンも てごわいぞ！\pひとつの タイプだけじゃなく\nチームぜんたいで たいさくしよう！$"

BattleFrontier_Lounge2_Text_DomeAceIsThere::
	.string "{JPN}しってるかい?\pあそこは エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lドームスーパースターって いう\lどハデな トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_DomeAceSilverMons::
	.string "{JPN}ドームスーパースターと たたかうなら\nしあいまえの じょうほうが だいじさ！\pつよそうな なまえに おどろかず\n2ひきの くみあわせを みておこう！$"

BattleFrontier_Lounge2_Text_DomeAceGoldMons::
	.string "{JPN}ほんきの ドームスーパースターだって\nいつも おなじ てもちとは かぎらない！\pその ときの あいてを みて\nきみの さくせんを きめるんだ！$"

BattleFrontier_Lounge2_Text_FactoryHeadIsThere::
	.string "{JPN}しってるかい?\pあそこは エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lファクトリーヘッドって いう\lやばい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_FactoryHeadSilverMons::
	.string "{JPN}きみは もう ファクトリーヘッドと\nたたかったこと あるかい?\pあいつが つかう ポケモンは\nレンタルポケモンの\lなかから どれか 3ひき!\lきみと ほぼ おなじ じょうけん らしいぜ!$"

BattleFrontier_Lounge2_Text_FactoryHeadGoldMons::
	.string "{JPN}きみは ほんきの ファクトリーヘッドと\nたたかったこと あるかい?\pあいつは ほんきで たたかう ときでも\nレンタルポケモンの\lなかから どれか 3ひき!\lきみと ほとんど おなじ じょうけんで\lたたかう らしいぜ!$"

BattleFrontier_Lounge2_Text_PikeQueenIsThere::
	.string "{JPN}しってるかい?\pあそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lチューブクイーンって いう\lこわーい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_PikeQueenSilverMons::
	.string "{JPN}チューブクイーンに あうまえに\nのこりの HPを たしかめよう！\pどんな ポケモンが でてきても\nうごけることが たいせつだよ。$"

BattleFrontier_Lounge2_Text_PikeQueenGoldMons::
	.string "{JPN}ほんきの チューブクイーンに\nあえたら それだけでも すごいことさ！\pのこしてきた ちからを\nそこで ぜんぶ ぶつけてみよう！$"

BattleFrontier_Lounge2_Text_ArenaTycoonIsThere::
	.string "{JPN}しってるかい?\pあそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lアリーナキャプテンって いう\lかわいい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_ArenaTycoonSilverMons::
	.string "{JPN}アリーナキャプテンとの しょうぶは\nポケモンの じゅんばんが たいせつ！\pこうたいで にげられないぶん\nつぎの 1ひきまで かんがえておこう。$"

BattleFrontier_Lounge2_Text_ArenaTycoonGoldMons::
	.string "{JPN}ほんきの アリーナキャプテンには\n3ターンの つかいかたで いどもう！\pわざを あてることも\nのこりの HPも だいじだからね。$"

BattleFrontier_Lounge2_Text_PalaceMavenIsThere::
	.string "{JPN}あそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lパレスガーディアンって いう\lあやしい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_PalaceMavenSilverMons::
	.string "{JPN}パレスガーディアンに いどむなら\nせいかくと わざを たしかめよう！\pてもちを あてるより\nきみの ポケモンを しることさ。$"

BattleFrontier_Lounge2_Text_PalaceMavenGoldMons::
	.string "{JPN}ほんきの パレスガーディアンの まえでも\nポケモンの きもちは だいじだよ。\pメガシンカや テラスタルの じゅんびも\nちょうせんの まえに しておこう！$"

BattleFrontier_Lounge2_Text_PyramidKingIsThere::
	.string "{JPN}しってるかい?\pあそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lピラミッドキングって いう\lアツーい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_PyramidKingSilverMons::
	.string "{JPN}ピラミッドキングを めざすなら\nまずは たんけんの じゅんびさ！\pもちこめる どうぐと\nチームの いきの ながさを かんがえよう。$"

BattleFrontier_Lounge2_Text_PyramidKingGoldMons::
	.string "{JPN}ほんきの ピラミッドキングまで\nたどりついたら むねを はっていい！\pどんな あいてが でてきても\nきみの たんけんの しめくくりだ！$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice1::
	.string "{JPN}ダブルバトルが できる ばしょは\nいくつか あるけど やっぱり きほんは\lバトルタワーの ダブル バトルルーム!\pまずは ここから バトルフロンティアでの\nダブルバトルを まなんで いくと いいぜ!$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice2::
	.string "{JPN}ここでの たたかいには\nちゅうい しなよ!\pダブルバトル ならではの せんぽうで\nたたかう トレーナーも いるみたいだぜ!$"

BattleFrontier_Lounge2_Text_DoubleBattleAdvice3::
	.string "{JPN}ここの たたかいで\nダブルバトルに じしんが ついたら\lほかの ダブルバトルが できる ばしょに\lチャレンジ してみると いいかもな!$"

BattleFrontier_Lounge2_Text_MultiBattleAdvice::
	.string "{JPN}バトルサロンには\nいろんな トレーナーが あつまるんだぜ!\pもしかしたら きみの ともだちや\nでしにも あえるかも しれないな!\lよーく さがして みると いいぜ!$"

BattleFrontier_Lounge2_Text_LinkMultiBattleAdvice::
	.string "{JPN}ともだちが あつまったら\nつうしんマルチ バトルルーム!!\pつよい ともだちと いっしょに あそべば\nつよい トレーナーたち との\lたたかいが まってるぜ!$"

