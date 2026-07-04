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
	lock
	msgbox BattleFrontier_Lounge2_Text_NewsGatheringPower, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_Maniac2::
	lock
	msgbox BattleFrontier_Lounge2_Text_AmazingPowersOfObservation, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_Maniac3::
	lock
	msgbox BattleFrontier_Lounge2_Text_AmazingPowerOfPersuasion, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge2_EventScript_TriathleteF::
	msgbox BattleFrontier_Lounge2_Text_ThisPlaceIsScaringMe, MSGBOX_NPC
	end

BattleFrontier_Lounge2_Text_FrontierManiacIntro::
	.string "{JPN}やあやあ ぼくは バトルフロンティアいちの\nじょうほうつう!\pフロンティアマニアって よんでくれよ!\pきみ トレーナーだろ?\pぼくが あつめた バトルフロンティアの\nホットな じょうほうを おしえて あげるよ!$"

BattleFrontier_Lounge2_Text_SwingByForTheLatestWord::
	.string "{JPN}やあやあ また ぼくの じょうほうを\nききに きたんだね! しかたないなー!$"

@ Unused
BattleFrontier_Lounge2_Text_MyInformationsBeenUsefulRight::
	.string "{JPN}どう? どう? どう?\pぼくの じょうほうは もちろん\nすごーく さんこうに なってるだろ?$"

BattleFrontier_Lounge2_Text_FacilityIsHottest::
	.string "{JPN}そうだねぇ……\pいまは {STR_VAR_1}が\nアッつい みたいだね!$"

BattleFrontier_Lounge2_Text_BattleTowerIsHottest::
	.string "{JPN}そうだねぇ……\pいまは バトルタワー{STR_VAR_1}が\nアッつい みたいだね!$"

BattleFrontier_Lounge2_Text_SalonMaidenIsThere::
	.string "{JPN}しってるかい?\pあそこは エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lタワータイクーンって いう\lなぞの トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_SalonMaidenSilverMons::
	.string "{JPN}きみは タワータイクーンと\nたたかったこと あるかい?\pやつは あいての ちからを はかるとき\nエスパーの ねんりきポケモン!\lほのおの かざんポケモン!\lノーマルの いねむりポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_SalonMaidenGoldMons::
	.string "{JPN}きみは ほんきの タワータイクーンと\nたたかったこと あるかい?\pやつは ほんとの ちからで たたかうとき\nドラゴンと エスパーの むげんポケモン!\lでんきの いかづちポケモン!\lノーマルの いねむりポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_DomeAceIsThere::
	.string "{JPN}しってるかい?\pあそこは エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lドームスーパースターって いう\lどハデな トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_DomeAceSilverMons::
	.string "{JPN}きみは ドームスーパースターと\nたたかったこと あるかい?\pやつは あいての ちからを なめてるとき\nドラゴンと ひこうの ドラゴンポケモン!\lみずと じめんの ぬまうおポケモン!\lほのおと ひこうの かえんポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_DomeAceGoldMons::
	.string "{JPN}きみは ほんきの ドームスーパースターと\nたたかったこと あるかい?\pやつは じぶんの せんりゃくを みせるとき\nドラゴンと エスパーの むげんポケモン!\lみずと じめんの ぬまうおポケモン!\lはがねと エスパーの てつあしポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_FactoryHeadIsThere::
	.string "{JPN}しってるかい?\pあそこは エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lファクトリーヘッドって いう\lやばい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_FactoryHeadSilverMons::
	.string "{JPN}きみは もう ファクトリーヘッドと\nたたかったこと あるかい?\pあいつが つかう ポケモンは\nレンタルポケモンの\lなかから どれか 3ひき!\lきみと ほぼ おなじ じょうけん らしいぜ!$"

BattleFrontier_Lounge2_Text_FactoryHeadGoldMons::
	.string "{JPN}きみは ほんきの ファクトリーヘッドと\nたたかったこと あるかい?\pあいつは ほんきで たたかう ときでも\nレンタルポケモンの\lなかから どれか 3ひき!\lきみと ほとんど おなじ じょうけんで\lたたかう らしいぜ!$"

BattleFrontier_Lounge2_Text_PikeQueenIsThere::
	.string "{JPN}しってるかい?\pあそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lチューブクイーンって いう\lこわーい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_PikeQueenSilverMons::
	.string "{JPN}きみは チューブクイーンと\nたたかったこと あるかい?\pやつは きげんの いいとき\nどくの キバへびポケモン!\lむしと いわの はっこうポケモン!\lみずの いつくしみポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_PikeQueenGoldMons::
	.string "{JPN}きみは ほんきの チューブクイーンと\nたたかったこと あるかい?\pやつは ほんきで おこったとき\nどくの キバへびポケモン!\lはがねと じめんの てつへびポケモン!\lみずと ひこうの きょうあくポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_ArenaTycoonIsThere::
	.string "{JPN}しってるかい?\pあそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lアリーナキャプテンって いう\lかわいい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_ArenaTycoonSilverMons::
	.string "{JPN}きみは もう アリーナキャプテンと\nたたかったこと あるかい?\pあのこは あいての ちからを うかがうとき\nむしと かくとうの 1ぽんヅノポケモン!\lあくの げっこうポケモン!\lむしと ゴーストの ぬけがらポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_ArenaTycoonGoldMons::
	.string "{JPN}きみは ほんきの アリーナキャプテンと\nたたかったこと あるかい?\pあのこは マジで たたかう とき\nあくの げっこうポケモン!\lゴーストと どくの シャドーポケモン!\lくさと かくとうの きのこポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_PalaceMavenIsThere::
	.string "{JPN}あそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lパレスガーディアンって いう\lあやしい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_PalaceMavenSilverMons::
	.string "{JPN}きみは パレスガーディアンと\nたたかったこと あるかい?\pやつは あいての こころを ためすとき\nどくと ひこうの こうもりポケモン!\lノーマルの ものぐさポケモン\lみずと こおりの のりものポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_PalaceMavenGoldMons::
	.string "{JPN}きみは ほんきの パレスガーディアンと\nたたかったこと あるかい?\pやつが てかげんぬきで たたかうときは\nほのおの でんせつポケモン!\lノーマルの ものぐさポケモン\lみずの オーロラポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_PyramidKingIsThere::
	.string "{JPN}しってるかい?\pあそこには エニシダが\nフロンティアブレーンって よんでる\lトレーナーの うちの ひとり……\lピラミッドキングって いう\lアツーい トレーナーが おさめてるのさ!$"

BattleFrontier_Lounge2_Text_PyramidKingSilverMons::
	.string "{JPN}きみは もう ピラミッドキングと\nたたかったこと あるかい?\pかれは あいての ちからを ためすとき\nいわの いわやまポケモン!\lこおりの ひょうざんポケモン!\lはがねの くろがねポケモンの\l3ひきを つかってくる らしいぜ!$"

BattleFrontier_Lounge2_Text_PyramidKingGoldMons::
	.string "{JPN}きみは ほんきの ピラミッドキングと\nたたかったこと あるかい?\pかれに あつい ちからが みなぎるときは\nこおりと ひこうの れいとうポケモン!\lでんきと ひこうの でんげきポケモン!\lほのおと ひこうの かえんポケモンの\l3ひきを つかってくる らしいぜ!$"

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

BattleFrontier_Lounge2_Text_NewsGatheringPower::
	.string "{JPN}すっげえ じょうほうりょくー!!\nやっぱ センパイは ちがうっすよー$"

BattleFrontier_Lounge2_Text_AmazingPowersOfObservation::
	.string "{JPN}すっげえ かんさつりょくー!!\nやっぱ センパイは ちがうっすわー$"

BattleFrontier_Lounge2_Text_AmazingPowerOfPersuasion::
	.string "{JPN}すっげえ せっとくりょくー!!\nやっぱ センパイは ちがうっすねー$"

BattleFrontier_Lounge2_Text_ThisPlaceIsScaringMe::
	.string "{JPN}なによ ここ……\nちょっと こわい……$"

