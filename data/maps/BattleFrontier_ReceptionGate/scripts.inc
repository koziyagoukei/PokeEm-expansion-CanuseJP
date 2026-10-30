BattleFrontier_ReceptionGate_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_ReceptionGate_OnFrame
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_ReceptionGate_OnTransition
	.byte 0

BattleFrontier_ReceptionGate_OnTransition:
	setflag FLAG_LANDMARK_BATTLE_FRONTIER
	call_if_set FLAG_SYS_FRONTIER_PASS, BattleFrontier_ReceptionGate_AlreadyRegistered
	end

BattleFrontier_ReceptionGate_AlreadyRegistered:
	setvar VAR_HAS_ENTERED_BATTLE_FRONTIER, 1
	return

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
	goto BattleFrontier_ReceptionGate_EventScript_IssueFrontierPass
	end

BattleFrontier_ReceptionGate_EventScript_IssueFrontierPass::
	msgbox BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier, MSGBOX_DEFAULT
	msgbox BattleFrontier_ReceptionGate_Text_IssueFrontierPass, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass
	waitfanfare
	waitmessage
	msgbox BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass, MSGBOX_DEFAULT
	setflag FLAG_SYS_FRONTIER_PASS
	msgbox BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier, MSGBOX_DEFAULT
	releaseall
	end

BattleFrontier_ReceptionGate_Movement_PlayerApproachCounter:
	walk_up
	walk_up
	walk_left
	walk_left
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
	.string "初めての お客様ですね!\nこちらへ どうぞ!$"

BattleFrontier_ReceptionGate_Text_WelcomeToBattleFrontier:
	.string "{JPN}ポケモン勝負の 最前線！\n"
	.string "バトルフロンティアへ ようこそ！$"

BattleFrontier_ReceptionGate_Text_IssueFrontierPass:
	.string "初めての お客様には\nバトルフロンティアの なかで\lごりよう いただける フロンティアパスを\lはっこうして おります!\pどうぞ お受け取り ください!$"

BattleFrontier_ReceptionGate_Text_ObtainedFrontierPass:
	.string "{PLAYER}は\nフロンティアパスを もらった!$"

BattleFrontier_ReceptionGate_Text_PlacedTrainerCardInFrontierPass:
	.string "{PLAYER}は トレーナーカードを\nフロンティアパスに 入れかえた!$"

BattleFrontier_ReceptionGate_Text_EnjoyBattleFrontier:
	.string "{JPN}どの 施設からでも おたのしみください。\nみなみの まちには 準備の ハブも\lコンテストかいじょうも ございます！$"

BattleFrontier_ReceptionGate_Text_YourGuideToFacilities:
	.string "{JPN}7つの 施設と せつびを ご案内します。\nみなみの まちへは とほで どうぞ！$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich2:
	.string "どの 説明が ききたいですか?$"

BattleFrontier_ReceptionGate_Text_BattleTowerInfo:
	.string "バトルフロンティアの シンボル とも\nいえる きょだいな とう です\pこちらの たてもの では シングル\nダブル マルチ 通信マルチの\l4つの バトルルームで 戦いを\lたのしむことが できます!$"

BattleFrontier_ReceptionGate_Text_BattleDomeInfo:
	.string "バトルドームは たまごの ような\nかたちをした 大きな たてものです\pこちらでは バトルトーナメント という\nイベントが かいさいされて います\pなお バトルトーナメントには\nシングルバトルよう ダブルバトルようの\l2つの コースが あります!$"

BattleFrontier_ReceptionGate_Text_BattlePalaceInfo:
	.string "バトルパレスは バトルフロンティアの\n右はじに ある あかい たてものです\pこちらの たてもの では\nシングル ダブルの 2つの\lバトルホールへの 挑戦が できます$"

BattleFrontier_ReceptionGate_Text_BattleArenaInfo:
	.string "バトルアリーナは バトルフロンティアの\nちゅうおう 右に いちする\lどうじょうの ような たてものです\pこちらでは 勝ちぬきチームバトル という\nイベントを かいさい しております!$"

BattleFrontier_ReceptionGate_Text_BattleFactoryInfo:
	.string "バトルファクトリーは ここをでて 一番\n近くにある 大きな ビルに なります\pこちらでは バトルトレード という\nイベントを かいさいして おります\pなお バトルトレードには\nシングルバトルよう ダブルバトルようの\l2つの コースが 用意 されています!$"

BattleFrontier_ReceptionGate_Text_BattlePikeInfo:
	.string "バトルチューブは バトルフロンティアの\nちゅうおう 左に いちする\lポケモンの かたちを した たてものです\pこちらでは バトルゲーム という\nイベントを かいさい しております!$"

BattleFrontier_ReceptionGate_Text_BattlePyramidInfo:
	.string "バトルピラミッドは 巨大な\n"
	.string "ピラミッドの 形をした 建物です\p"
	.string "こちらでは バトルアドベンチャー という\n"
	.string "イベントを 開催 しております!$"

BattleFrontier_ReceptionGate_Text_RankingHallInfo:
	.string "ランキングホールは\nバトルタワーの 近くに あります\pこちらは バトルフロンティアで\n戦った トレーナーたちの\lもっとも すばらしい 記録を\l見ることが できます$"

BattleFrontier_ReceptionGate_Text_ExchangeCornerInfo:
	.string "ひきかえサービス コーナーは\nバトルタワーの 近くに あります\pこちらでは バトルフロンティアの\nなかで ためた バトルポイントを\lすてきな けいひんと\l交換する ことが できます!$"

BattleFrontier_ReceptionGate_Text_YourGuideToRules:
	.string "こちらでは バトルフロンティアに ある\nすべての 施設に きょうつう した\l挑戦の ための きほんてきな\lルールを 説明 いたします!$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhat:
	.string "どの 説明が ききたいですか?$"

BattleFrontier_ReceptionGate_Text_LevelModeInfo:
	.string "バトルフロンティアに ある\nすべての 施設の 挑戦は\lおおきく レベル50の コースと\lオープンレベルの コースに わかれています$"

BattleFrontier_ReceptionGate_Text_Level50Info:
	.string "{JPN}レベル50の コースでは\n高い レベルの ポケモンも\lレベル50に あわせて 挑戦できます。\p挑戦が おわれば\nもとの レベルに 戻ります。$"

BattleFrontier_ReceptionGate_Text_OpenLevelInfo:
	.string "オープンレベルの コースでは\n挑戦に 参加する ポケモンの\lレベルに 制限が ありません\pあなたの ポケモンの レベルに あわせて\n対戦する トレーナーの ポケモンの\lレベルが 代わります\pただし レベル60より 低い レベルの\nポケモンを つれた トレーナーが\lとうじょう することは ありません$"

BattleFrontier_ReceptionGate_Text_MonEntryInfo:
	.string "タマゴや 一部の れいがいを\nのぞいて ほとんどの ポケモンが\lすべての 施設に 挑戦 できます\p挑戦の ために\n必要な ポケモンの かずは\l施設に よって 違います\pただし どの 施設でも\n同じ 種類の ポケモンを\l2匹 以上 参加 させることは\lできませんので ごちゅうい ください$"

BattleFrontier_ReceptionGate_Text_HoldItemsInfo:
	.string "バトルフロンティアの 施設に\nポケモンを 挑戦 させるとき\pそれぞれの ポケモンが\n同じ 道具を 持つことは できません\p挑戦 させる ポケモンには\nすべて 違う 道具を\l持たせて おくように してください$"

BattleFrontier_ReceptionGate_Text_YourGuideToFrontierPass:
	.string "こちらでは フロンティアパスの\n説明を いたします!$"

BattleFrontier_ReceptionGate_Text_LearnAboutWhich1:
	.string "どの 説明が ききたいですか?$"

BattleFrontier_ReceptionGate_Text_SymbolsInfo:
	.string "バトルフロンティアの 7つの 施設で\n強さを 認められた トレーナーが\lそれぞれの 施設の シンボルを\lもらえることが あります\pシンボルを もらう ためには\nなんにんも トレーナーを 勝ちぬいたり\l何度も 制覇 したり……\pなかなか 大変な 道のりですが\nぜひ がんばって くださいね!$"

BattleFrontier_ReceptionGate_Text_RecordedBattleInfo:
	.string "{JPN}対戦の あとで バトルビデオを\n1つ 記録することが できます。\p施設へ 挑戦している あいだは\nビデオを さいせいできません。$"

BattleFrontier_ReceptionGate_Text_BattlePointsInfo:
	.string "{JPN}バトルポイントは 施設での かつやくや\nコンテストの 優勝で えられます。\pけいひんや べっそうの ショップなど\n好きなことに おつかいください！$"
