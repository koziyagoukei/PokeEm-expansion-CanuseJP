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
	.string "{JPN}はじめての おきゃくさまですね!\nこちらへ どうぞ!$"

BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier:
	.string "{JPN}ポケモンバトルの さいぜんせん!\nバトルフロンティアへ ようこそ!$"

BattleFrontier_ReceptionGate_Text_IssueFrontierPass:
	.string "{JPN}はじめての おきゃくさまには\nバトルフロンティアの なかで\lごりよう いただける フロンティアパスを\lはっこうして おります!\pどうぞ おうけとり ください!$"

BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass:
	.string "{JPN}{PLAYER}は\nフロンティアパスを もらった!$"

BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass:
	.string "{JPN}{PLAYER}は トレーナーカードを\nフロンティアパスに いれかえた!$"

BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier:
	.string "{JPN}それでは バトルフロンティアを\nぞんぶんに おたのしみ ください!$"

BattleFrontier_ReceptionGate_Text_IfItIsntPlayerYouCame:
	.string "{JPN}???『やあ きて くれたんだね\n{PLAYER}{KUN}!$"

BattleFrontier_ReceptionGate_Text_OhMrScottGoodDay:
	.string "{JPN}うけつけ『あっ エニシダさま!\nおつかれさまです!$"

BattleFrontier_ReceptionGate_Text_ScottGreatToSeeYouHere:
	.string "{JPN}エニシダ『きみが きてくれて\nとっても うれしいよ!\pぜひ ゆっくりと そして じっくりと\nポケモンしょうぶの しんずいを\lあじわって いって くれ!\pわたしの へやも あるから\nひまが あるときに たずねて きてくれよ!$"

BattleFrontier_ReceptionGate_Text_YourGuideToFacilities:
	.string "{JPN}こちらでは バトルフロンティアに ある\nさまざまな しせつの\lごあんないを いたします!$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich2:
	.string "{JPN}どの せつめいが ききたいですか?$"

BattleFrontier_ReceptionGate_Text_BattleTowerInfo:
	.string "{JPN}バトルフロンティアの シンボル とも\nいえる きょだいな とう です\pこちらの たてもの では シングル\nダブル マルチ つうしんマルチの\l4つの バトルルームで たたかいを\lたのしむことが できます!$"

BattleFrontier_ReceptionGate_Text_BattleDomeInfo:
	.string "{JPN}バトルドームは たまごの ような\nかたちをした おおきな たてものです\pこちらでは バトルトーナメント という\nイベントが かいさいされて います\pなお バトルトーナメントには\nシングルバトルよう ダブルバトルようの\l2つの コースが あります!$"

BattleFrontier_ReceptionGate_Text_BattlePalaceInfo:
	.string "{JPN}バトルパレスは バトルフロンティアの\nみぎはじに ある あかい たてものです\pこちらの たてもの では\nシングル ダブルの 2つの\lバトルホールへの ちょうせんが できます$"

BattleFrontier_ReceptionGate_Text_BattleArenaInfo:
	.string "{JPN}バトルアリーナは バトルフロンティアの\nちゅうおう みぎに いちする\lどうじょうの ような たてものです\pこちらでは かちぬきチームバトル という\nイベントを かいさい しております!$"

BattleFrontier_ReceptionGate_Text_BattleFactoryInfo:
	.string "{JPN}バトルファクトリーは ここをでて いちばん\nちかくにある おおきな ビルに なります\pこちらでは バトルトレード という\nイベントを かいさいして おります\pなお バトルトレードには\nシングルバトルよう ダブルバトルようの\l2つの コースが ようい されています!$"

BattleFrontier_ReceptionGate_Text_BattlePikeInfo:
	.string "{JPN}バトルチューブは バトルフロンティアの\nちゅうおう ひだりに いちする\lポケモンの かたちを した たてものです\pこちらでは バトルゲーム という\nイベントを かいさい しております!$"

BattleFrontier_ReceptionGate_Text_BattlePyramidInfo:
	.string "{JPN}バトルピラミッドは きょだいな\nピラミッドの かたちをした たてものです\pこちらでは バトルアドベンチャー という\nイベントを かいさい しております!$"

BattleFrontier_ReceptionGate_Text_RankingHallInfo:
	.string "{JPN}ランキングホールは\nバトルタワーの ちかくに あります\pこちらは バトルフロンティアで\nたたかった トレーナーたちの\lもっとも すばらしい きろくを\lみることが できます$"

BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo:
	.string "{JPN}ひきかえサービス コーナーは\nバトルタワーの ちかくに あります\pこちらでは バトルフロンティアの\nなかで ためた バトルポイントを\lすてきな けいひんと\lこうかんする ことが できます!$"

BattleFrontier_ReceptionGate_Text_YourGuideToRules:
	.string "{JPN}こちらでは バトルフロンティアに ある\nすべての しせつに きょうつう した\lちょうせんの ための きほんてきな\lルールを せつめい いたします!$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhat:
	.string "{JPN}どの せつめいが ききたいですか?$"

BattleFrontier_ReceptionGate_Text_LevelModeInfo:
	.string "{JPN}バトルフロンティアに ある\nすべての しせつの ちょうせんは\lおおきく レベル50の コースと\lオープンレベルの コースに わかれています$"

BattleFrontier_ReceptionGate_Text_Level50Info:
	.string "{JPN}レベル50の コースでは なまえの とおり\nレベル50までの ポケモンを\lちょうせん させることが できます\pただし レベル50より ひくい レベルの\nポケモンを つれた トレーナーが\lとうじょう することは ありません\lくれぐれも ごちゅうい ください\pなお このコースが バトルフロンティアの\nたたかいの きほんと なって いますので\lぜひ チャレンジして みてください$"

BattleFrontier_ReceptionGate_Text_OpenLevelInfo:
	.string "{JPN}オープンレベルの コースでは\nちょうせんに さんかする ポケモンの\lレベルに せいげんが ありません\pあなたの ポケモンの レベルに あわせて\nたいせんする トレーナーの ポケモンの\lレベルが かわります\pただし レベル60より ひくい レベルの\nポケモンを つれた トレーナーが\lとうじょう することは ありません$"

BattleFrontier_ReceptionGate_Text_MonEntryInfo:
	.string "{JPN}タマゴや いちぶの れいがいを\nのぞいて ほとんどの ポケモンが\lすべての しせつに ちょうせん できます\pちょうせんの ために\nひつような ポケモンの かずは\lしせつに よって ちがいます\pただし どの しせつでも\nおなじ しゅるいの ポケモンを\l2ひき いじょう さんか させることは\lできませんので ごちゅうい ください$"

BattleFrontier_ReceptionGate_Text_HoldItemsInfo:
	.string "{JPN}バトルフロンティアの しせつに\nポケモンを ちょうせん させるとき\pそれぞれの ポケモンが\nおなじ どうぐを もつことは できません\pちょうせん させる ポケモンには\nすべて ちがう どうぐを\lもたせて おくように してください$"

BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass:
	.string "{JPN}こちらでは フロンティアパスの\nせつめいを いたします!$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich1:
	.string "{JPN}どの せつめいが ききたいですか?$"

BattleFrontier_ReceptionGate_Text_SymbolsInfo:
	.string "{JPN}バトルフロンティアの 7つの しせつで\nつよさを みとめられた トレーナーが\lそれぞれの しせつの シンボルを\lもらえることが あります\pシンボルを もらう ためには\nなんにんも トレーナーを かちぬいたり\lなんども せいは したり……\pなかなか たいへんな みちのりですが\nぜひ がんばって くださいね!$"

BattleFrontier_ReceptionGate_Text_RecordedBattleInfo:
	.string "{JPN}あなたの たいせんを ひとつだけ\nパスに きろくする ことが できます\pおともだち との たいせんか\nバトルチューブ バトルピラミッド いがいの\lバトルフロンティアでの\lたいせんが きろくできます\pたいせんが おわった ときに\nきろく するか どうかを きめられます$"

BattleFrontier_ReceptionGate_Text_BattlePointsInfo:
	.string "{JPN}バトルポイントは\nバトルフロンティアで すばらしい\lたたかいを した トレーナーに\lあたえられる ごほうびです\pためた ポイントは ひきかえサービスで\nしょうひんと ひきかえることが できます$"
