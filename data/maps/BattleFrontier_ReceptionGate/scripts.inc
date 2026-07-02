BattleFrontier_ReceptionGate_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_ReceptionGate_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_ReceptionGate_OnTransition
	.byte 0

BattleFrontier_ReceptionGate_OnTransition:
	setflag FLAG_LANDMARK_BATTLE_FRONTIER
	end

BattleFrontier_ReceptionGate_OnFrame:
	map_script_2 VAR_HAS_ENTERED_BATTLE_FRONTIER, 0, BattleFrontier_ReceptionGate_EventScript_FirstTimeEntering
	.2byte 0

BattleFrontier_ReceptionGate_EventScript_FirstTimeEntering::
	lockall
	setvar VAR_HAS_ENTERED_BATTLE_FRONTIER, 1
	playse SE_PIN
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_Delay48
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter
	waitmovement 0
	goto BattleFrontier_ReceptionGate_EventScript_ScottScene
	end

BattleFrontier_ReceptionGate_EventScript_ScottScene::
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IssueFrontierPass, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass
	waitfanfare
	waitmessage
	msgbox BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass, MSGBOX_DEFAULT
	setflag FLAG_SYS_FRONTIER_PASS
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, Common_Movement_ExclamationMark
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, Common_Movement_ExclamationMark
	applymovement LOCALID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_GREETER, BattleFrontier_ReceptionGate_Movement_GreeterFaceScott
	applymovement LOCALID_FRONTIER_RECEPTION_GUIDE, BattleFrontier_ReceptionGate_Movement_FacilityGuideFaceScott
	applymovement LOCALID_PLAYER, BattleFrontier_ReceptionGate_Movement_PlayerFaceScott
	applymovement LOCALID_FRONTIER_RECEPTION_SCOTT, BattleFrontier_ReceptionGate_Movement_ScottEnter
	waitmovement 0
	msgbox BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_FRONTIER_RECEPTION_SCOTT, BattleFrontier_ReceptionGate_Movement_ScottExit
	waitmovement 0
	removeobject LOCALID_FRONTIER_RECEPTION_SCOTT
	releaseall
	end

BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter:
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

BattleFrontier_ReceptionGate_Movement_PlayerFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

@ Unused
BattleFrontier_ReceptionGate_Movement_WalkDown:
	walk_down
	walk_down
	step_end

BattleFrontier_ReceptionGate_Movement_ScottEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	step_end

BattleFrontier_ReceptionGate_Movement_ScottExit:
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleFrontier_ReceptionGate_Movement_GreeterFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_right
	step_end

BattleFrontier_ReceptionGate_Movement_FacilityGuideFaceScott:
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_left
	step_end

BattleFrontier_ReceptionGate_EventScript_Greeter::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_FacilityGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToFacilities, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhich2
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BF_RECEPTIONIST
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_BattleTower
	case 1, BattleFrontier_ReceptionGate_EventScript_BattleDome
	case 2, BattleFrontier_ReceptionGate_EventScript_BattlePalace
	case 3, BattleFrontier_ReceptionGate_EventScript_BattleArena
	case 4, BattleFrontier_ReceptionGate_EventScript_BattleFactory
	case 5, BattleFrontier_ReceptionGate_EventScript_BattlePike
	case 6, BattleFrontier_ReceptionGate_EventScript_BattlePyramid
	case 7, BattleFrontier_ReceptionGate_EventScript_RankingHall
	case 8, BattleFrontier_ReceptionGate_EventScript_ExchangeCorner
	case 9, BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide
	end

BattleFrontier_ReceptionGate_EventScript_BattleTower::
	msgbox BattleFrontier_ReceptionGate_Text_BattleTowerInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleDome::
	msgbox BattleFrontier_ReceptionGate_Text_BattleDomeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePalace::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePalaceInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleArena::
	msgbox BattleFrontier_ReceptionGate_Text_BattleArenaInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattleFactory::
	msgbox BattleFrontier_ReceptionGate_Text_BattleFactoryInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePike::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePikeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePyramid::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePyramidInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_RankingHall::
	msgbox BattleFrontier_ReceptionGate_Text_RankingHallInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExchangeCorner::
	msgbox BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFacilityToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitFacilityGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_RulesGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToRules, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhat
	waitmessage
	multichoice 15, 0, MULTI_FRONTIER_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_UltraHelp
	case 1, BattleFrontier_ReceptionGate_EventScript_Level50
	case 2, BattleFrontier_ReceptionGate_EventScript_OpenLevel
	case 3, BattleFrontier_ReceptionGate_EventScript_MonEntry
	case 4, BattleFrontier_ReceptionGate_EventScript_HoldItems
	case 5, BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide
	end

BattleFrontier_ReceptionGate_EventScript_UltraHelp::
	setvar VAR_0x8004, 0
	special ultra_help
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_LevelMode::
	msgbox BattleFrontier_ReceptionGate_Text_LevelModeInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_Level50::
	msgbox BattleFrontier_ReceptionGate_Text_Level50Info, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_OpenLevel::
	msgbox BattleFrontier_ReceptionGate_Text_OpenLevelInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_MonEntry::
	msgbox BattleFrontier_ReceptionGate_Text_MonEntryInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_HoldItems::
	msgbox BattleFrontier_ReceptionGate_Text_HoldItemsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseRuleToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitRulesGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_EventScript_FrontierPassGuide::
	lock
	faceplayer
	msgbox BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout::
	message BattleFrontier_ReceptionGate_Text_LearnAboutWhich1
	waitmessage
	multichoice 16, 4, MULTI_FRONTIER_PASS_INFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_ReceptionGate_EventScript_Symbols
	case 1, BattleFrontier_ReceptionGate_EventScript_RecordBattle
	case 2, BattleFrontier_ReceptionGate_EventScript_BattlePoints
	case 3, BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide
	case MULTI_B_PRESSED, BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide
	end

BattleFrontier_ReceptionGate_EventScript_Symbols::
	msgbox BattleFrontier_ReceptionGate_Text_SymbolsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_RecordBattle::
	msgbox BattleFrontier_ReceptionGate_Text_RecordedBattleInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_BattlePoints::
	msgbox BattleFrontier_ReceptionGate_Text_BattlePointsInfo, MSGBOX_DEFAULT
	goto BattleFrontier_ReceptionGate_EventScript_ChooseFrontierPassInfoToLearnAbout
	end

BattleFrontier_ReceptionGate_EventScript_ExitFrontierPassGuide::
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	release
	end

BattleFrontier_ReceptionGate_Text_FirstTimeHereThisWay:
	.string "{JPN}こちらは はじめて ですか？\n"
	.string "どうぞ こちらへ！$"

BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier:
	.string "{JPN}ポケモンバトルの さいぜんせん！\n"
	.string "バトルフロンティアへ ようこそ！$"

BattleFrontier_ReceptionGate_Text_IssueFrontierPass:
	.string "{JPN}はじめての かたには\n"
	.string "フロンティアパスを おわたしします\p"
	.string "バトルフロンティアの しせつで\n"
	.string "つかう パスです\p"
	.string "どうぞ！$"

BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass:
	.string "{JPN}{PLAYER}は\n"
	.string "フロンティアパスを てにいれた$"

BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass:
	.string "{JPN}{PLAYER}は トレーナーカードを\n"
	.string "フロンティアパスに いれた$"

BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier:
	.string "{JPN}バトルフロンティアを\n"
	.string "こころゆくまで おたのしみください！$"

BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame:
	.string "{JPN}？？？: おお {PLAYER}{KUN}！\n"
	.string "ここまで きてくれたか！$"

BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay:
	.string "{JPN}ガイド: あっ！ エニシダさん！\n"
	.string "こんにちは！$"

BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere:
	.string "{JPN}エニシダ: よく きてくれた！\n"
	.string "ほんとうに うれしいぞ！\p"
	.string "ゆっくり いろいろな しせつを\n"
	.string "みていって くれ\p"
	.string "もちろん しょうぶの しんずいも\n"
	.string "あじわって ほしい！\p"
	.string "わしの いえも ここに あるから\n"
	.string "ひまなら よってくれよ！$"

BattleFrontier_ReceptionGate_Text_YourGuideToFacilities:
	.string "{JPN}バトルフロンティアの\n"
	.string "しせつを ごあんない します$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich2:
	.string "{JPN}どれに ついて ききますか？$"

BattleFrontier_ReceptionGate_Text_BattleTowerInfo:
	.string "{JPN}バトルフロンティアの\n"
	.string "シンボルとも いえる おおきな タワーです\p"
	.string "シングル ダブル マルチ\n"
	.string "つうしんマルチの 4つの\l"
	.string "バトルルームが あります$"

BattleFrontier_ReceptionGate_Text_BattleDomeInfo:
	.string "{JPN}バトルドームは\n"
	.string "おおきな タマゴのような たてものです\p"
	.string "ここでは バトルトーナメントが\n"
	.string "ひらかれます\p"
	.string "シングルと ダブルの\n"
	.string "2つの コースが あります$"

BattleFrontier_ReceptionGate_Text_BattlePalaceInfo:
	.string "{JPN}バトルパレスは\n"
	.string "みぎがわに ある あかい たてものです\p"
	.string "シングルと ダブルの\n"
	.string "2つの バトルホールが あります$"

BattleFrontier_ReceptionGate_Text_BattleArenaInfo:
	.string "{JPN}バトルアリーナは\n"
	.string "まんなかより みぎに ある\l"
	.string "どうじょうのような たてものです\p"
	.string "ここでは 3ターンで しょうぶする\n"
	.string "イベントが おこなわれます$"

BattleFrontier_ReceptionGate_Text_BattleFactoryInfo:
	.string "{JPN}バトルファクトリーは\n"
	.string "ここから いちばん ちかい おおきな たてものです\p"
	.string "ここでは レンタルポケモンを\n"
	.string "こうかんしながら たたかいます\p"
	.string "シングルと ダブルの\n"
	.string "2つの コースが あります$"

BattleFrontier_ReceptionGate_Text_BattlePikeInfo:
	.string "{JPN}バトルチューブは\n"
	.string "まんなかより ひだりに ある\l"
	.string "ポケモンの かたちをした たてものです\p"
	.string "3つの へやから 1つを えらんで\n"
	.string "すすんでいきます$"

BattleFrontier_ReceptionGate_Text_BattlePyramidInfo:
	.string "{JPN}バトルピラミッドは\n"
	.string "とても おおきな ピラミッドです\p"
	.string "くらい フロアを すすんで\n"
	.string "ちょうじょうを めざします$"

BattleFrontier_ReceptionGate_Text_RankingHallInfo:
	.string "{JPN}ランキングホールは\n"
	.string "バトルタワーの ちかくに あります\p"
	.string "バトルフロンティアに いどんだ\n"
	.string "トレーナーたちの すばらしい\l"
	.string "きろくを みることが できます$"

BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo:
	.string "{JPN}こうかんサービスコーナーは\n"
	.string "バトルタワーの ちかくに あります\p"
	.string "バトルフロンティアで ためた\n"
	.string "バトルポイントを すてきな\l"
	.string "しょうひんと こうかんできます$"

BattleFrontier_ReceptionGate_Text_YourGuideToRules:
	.string "{JPN}バトルフロンティアの しせつに\n"
	.string "きょうつうする きほんルールを\l"
	.string "ごあんないします$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhat:
	.string "{JPN}なにに ついて ききますか？$"

BattleFrontier_ReceptionGate_Text_LevelModeInfo:
	.string "{JPN}バトルフロンティアの しせつには\n"
	.string "レベル50と オープンレベルの\l"
	.string "2つの コースが あります$"

BattleFrontier_ReceptionGate_Text_Level50Info:
	.string "{JPN}レベル50は レベル50までの\n"
	.string "ポケモンで ちょうせんできます\p"
	.string "ただし あいての ポケモンは\n"
	.string "レベル50みまんには なりません\p"
	.string "バトルフロンティアの\n"
	.string "きほんとなる コースです\p"
	.string "まずは この コースに\n"
	.string "ちょうせんしてみてください$"

BattleFrontier_ReceptionGate_Text_OpenLevelInfo:
	.string "{JPN}オープンレベルは\n"
	.string "ちょうせんする ポケモンの レベルに\l"
	.string "せいげんが ありません\p"
	.string "あいての レベルは\n"
	.string "あなたの ポケモンに あわせて\l"
	.string "ちょうせいされます\p"
	.string "ただし あいての ポケモンは\n"
	.string "レベル60みまんには なりません$"

BattleFrontier_ReceptionGate_Text_MonEntryInfo:
	.string "{JPN}ほとんどの ポケモンは\n"
	.string "どの しせつにも ちょうせんできます\p"
	.string "ただし タマゴと いちぶの\n"
	.string "ポケモンは さんかできません\p"
	.string "ひつような ポケモンの かずは\n"
	.string "しせつに よって ちがいます\p"
	.string "ただし どの しせつでも\n"
	.string "おなじ ポケモンを 2ひき いじょう\l"
	.string "つれていくことは できません$"

BattleFrontier_ReceptionGate_Text_HoldItemsInfo:
	.string "{JPN}バトルフロンティアの しせつでは\n"
	.string "おなじ どうぐを もった ポケモンを\l"
	.string "いっしょに さんかさせられません\p"
	.string "ちょうせんする ポケモンには\n"
	.string "それぞれ ちがう どうぐを\l"
	.string "もたせてください$"

BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass:
	.string "{JPN}フロンティアパスに ついて\n"
	.string "ごあんないします$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich1:
	.string "{JPN}どれに ついて ききますか？$"

BattleFrontier_ReceptionGate_Text_SymbolsInfo:
	.string "{JPN}バトルフロンティアには\n"
	.string "7つの しせつが あります\p"
	.string "じつりょくを みとめられた\n"
	.string "トレーナーは それぞれの しせつで\l"
	.string "シンボルを もらえることがあります\p"
	.string "シンボルを えるには\n"
	.string "トレーナーに れんぞくで かったり\l"
	.string "イベントで なんども かつ ひつようが\l"
	.string "あります\p"
	.string "シンボルを えるのは かんたんでは\n"
	.string "ありません がんばってください！$"

BattleFrontier_ReceptionGate_Text_RecordedBattleInfo:
	.string "{JPN}フロンティアパスには\n"
	.string "バトルを 1つ きろくできます\p"
	.string "ともだちとの バトルを\n"
	.string "きろくすることも できます\p"
	.string "また バトルチューブと\n"
	.string "バトルピラミッドを のぞく\l"
	.string "バトルフロンティアの しょうぶも\l"
	.string "きろくできます\p"
	.string "バトルの あとに\n"
	.string "きろくするか えらべます$"

BattleFrontier_ReceptionGate_Text_BattlePointsInfo:
	.string "{JPN}バトルポイントは\n"
	.string "バトルフロンティアで すぐれた\l"
	.string "しょうぶをした トレーナーに\l"
	.string "おくられる ポイントです\p"
	.string "こうかんサービスコーナーで\n"
	.string "しょうひんと こうかんできます$"
