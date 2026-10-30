BattleFrontier_BattleFactoryLobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattleFactoryLobby_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattleFactoryLobby_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleFactoryLobby_OnWarp
	.byte 0

BattleFrontier_BattleFactoryLobby_OnTransition:
	@ The cook and Pokevirus only appear for Toshio's release battle.
	setflag FLAG_TEMP_1D
	setflag FLAG_TEMP_1E
	setvar VAR_0x8004, STORY_TARGET_TOSHIO
	call FrontierStory_EventScript_SetupTarget
	end

BattleFrontier_BattleFactoryLobby_OnWarp:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattleFactoryLobby_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattleFactoryLobby_EventScript_TurnPlayerNorth::
	setvar VAR_TEMP_1, 1
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattleFactoryLobby_OnFrame:
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, 0, BattleFrontier_BattleFactoryLobby_EventScript_GetChallengeStatus
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING, BattleFrontier_BattleFactoryLobby_EventScript_QuitWithoutSaving
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_PAUSED, BattleFrontier_BattleFactoryLobby_EventScript_ResumeChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_WON, BattleFrontier_BattleFactoryLobby_EventScript_WonChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST, BattleFrontier_BattleFactoryLobby_EventScript_LostChallenge
	map_script_2 VAR_TEMP_E, 0, FactoryStory_EventScript_LobbyArrival
	map_script_2 VAR_TEMP_E, 1, FrontierStory_EventScript_LobbyDarkraiCall
	.2byte 0

BattleFrontier_BattleFactoryLobby_EventScript_GetChallengeStatus::
	frontier_getstatus
	end

BattleFrontier_BattleFactoryLobby_EventScript_QuitWithoutSaving::
	lockall
	msgbox BattleFrontier_BattleFactoryLobby_Text_DidntSaveBeforeQuitting, MSGBOX_DEFAULT
	closemessage
	factory_setswapped
	factory_set FACTORY_DATA_WIN_STREAK_SWAPS, 0
	factory_set FACTORY_DATA_WIN_STREAK, 0
	factory_set FACTORY_DATA_WIN_STREAK_ACTIVE, FALSE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattleFactoryLobby_EventScript_WonChallenge::
	lockall
	frontier_isbrain
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattleFactoryLobby_EventScript_DefeatedFactoryHead
	msgbox BattleFrontier_BattleFactoryLobby_Text_CongratsSevenWins, MSGBOX_DEFAULT
	waitmessage
	goto BattleFrontier_BattleFactoryLobby_EventScript_GiveBattlePoints

BattleFrontier_BattleFactoryLobby_EventScript_DefeatedFactoryHead::
	msgbox BattleFrontier_BattleFactoryLobby_Text_CongratsForDefeatingHead, MSGBOX_DEFAULT
	waitmessage
BattleFrontier_BattleFactoryLobby_EventScript_GiveBattlePoints::
	msgbox BattleFrontier_BattleFactoryLobby_Text_AwardBattlePoints, MSGBOX_DEFAULT
	frontier_givepoints
	msgbox BattleFrontier_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	message BattleFrontier_BattleFactoryLobby_Text_ExchangeMonsAndSave
	waitmessage
	frontier_checkairshow
	special LoadPlayerParty
	callnative UpdateFollowingPokemon
	factory_save 0
	playse SE_SAVE
	waitse
	goto BattleFrontier_BattleFactoryLobby_EventScript_AskRecordBattle
	end

BattleFrontier_BattleFactoryLobby_EventScript_LostChallenge::
	lockall
	message BattleFrontier_BattleFactoryLobby_Text_ReturnMonsSaveResults
	waitmessage
	frontier_checkairshow
	special LoadPlayerParty
	callnative UpdateFollowingPokemon
	factory_set FACTORY_DATA_WIN_STREAK_ACTIVE, FALSE
	factory_save 0
	playse SE_SAVE
	waitse

BattleFrontier_BattleFactoryLobby_EventScript_AskRecordBattle::
	call BattleFrontier_EventScript_GetCantRecordBattle
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattleFactoryLobby_EventScript_EndRecordBattle
	message BattleFrontier_BattleFactoryLobby_Text_RecordLastMatch
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	switch VAR_RESULT
	case 1, BattleFrontier_BattleFactoryLobby_EventScript_EndRecordBattle
	case 0, BattleFrontier_BattleFactoryLobby_EventScript_RecordBattle
	case MULTI_B_PRESSED, BattleFrontier_BattleFactoryLobby_EventScript_EndRecordBattle

BattleFrontier_BattleFactoryLobby_EventScript_RecordBattle::
	call BattleFrontier_EventScript_SaveBattle
BattleFrontier_BattleFactoryLobby_EventScript_EndRecordBattle::
	msgbox BattleFrontier_BattleFactoryLobby_Text_LookForwardToNextVisit, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattleFactoryLobby_EventScript_ResumeChallenge::
	lockall
	message BattleFrontier_BattleFactoryLobby_Text_WaitingForYouToResume
	waitmessage
	factory_save CHALLENGE_STATUS_SAVING
	playse SE_SAVE
	waitse
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	setvar VAR_0x8006, 2
	goto BattleFrontier_BattleFactoryLobby_EventScript_EnterChallenge

BattleFrontier_BattleFactoryLobby_EventScript_SinglesAttendant::
	lock
	faceplayer
	setvar VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_FACTORY
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	goto BattleFrontier_BattleFactoryLobby_EventScript_Attendant
	end

BattleFrontier_BattleFactoryLobby_EventScript_DoublesAttendant::
	setvar VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_FACTORY
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_DOUBLES
	goto BattleFrontier_BattleFactoryLobby_EventScript_Attendant
	end

BattleFrontier_BattleFactoryLobby_EventScript_Attendant::
	special SavePlayerParty
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES, BattleFrontier_BattleFactoryLobby_EventScript_WelcomeForSingleBattle
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_DOUBLES, BattleFrontier_BattleFactoryLobby_EventScript_WelcomeForDoubleBattle
	call FrontierStory_EventScript_StaffAside
BattleFrontier_BattleFactoryLobby_EventScript_AskTakeChallenge::
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES, BattleFrontier_BattleFactoryLobby_EventScript_TakeSinglesChallenge
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_DOUBLES, BattleFrontier_BattleFactoryLobby_EventScript_TakeDoublesChallenge
	waitmessage
	multichoice 17, 6, MULTI_CHALLENGEINFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattleFactoryLobby_EventScript_TryEnterChallenge
	case 1, BattleFrontier_BattleFactoryLobby_EventScript_ExplainChallenge
	case 2, BattleFrontier_BattleFactoryLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattleFactoryLobby_EventScript_CancelChallenge

BattleFrontier_BattleFactoryLobby_EventScript_TryEnterChallenge::
	message BattleFrontier_BattleFactoryLobby_Text_WhichLevelMode
	waitmessage
	multichoice 17, 6, MULTI_LEVEL_MODE, FALSE
	switch VAR_RESULT
	case FRONTIER_LVL_TENT, BattleFrontier_BattleFactoryLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattleFactoryLobby_EventScript_CancelChallenge
	frontier_set FRONTIER_DATA_LVL_MODE, VAR_RESULT
	msgbox BattleFrontier_BattleFactoryLobby_Text_OkayToSaveBeforeChallenge, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, BattleFrontier_BattleFactoryLobby_EventScript_LoadPartyAndCancelChallenge
	case YES, BattleFrontier_BattleFactoryLobby_EventScript_SaveBeforeChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattleFactoryLobby_EventScript_LoadPartyAndCancelChallenge

BattleFrontier_BattleFactoryLobby_EventScript_SaveBeforeChallenge::
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	factory_init
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING
	factory_set FACTORY_DATA_WIN_STREAK_ACTIVE, TRUE
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	closemessage
	delay 2
	call Common_EventScript_SaveGame
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattleFactoryLobby_EventScript_CancelChallengeSaveFailed
	setvar VAR_0x8006, 0
BattleFrontier_BattleFactoryLobby_EventScript_EnterChallenge::
	special SavePlayerParty
	msgbox BattleFrontier_BattleFactoryLobby_Text_StepThisWay, MSGBOX_DEFAULT
	closemessage
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES, BattleFrontier_BattleFactoryLobby_EventScript_TalkedToSinglesAttendant
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_DOUBLES, BattleFrontier_BattleFactoryLobby_EventScript_TalkedToDoublesAttendant
	applymovement VAR_LAST_TALKED, BattleFrontier_BattleFactoryLobby_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, BattleFrontier_BattleFactoryLobby_Movement_PlayerEnterDoor
	waitmovement 0
	warp MAP_BATTLE_FRONTIER_BATTLE_FACTORY_PRE_BATTLE_ROOM, 8, 13
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	waitstate
	end

BattleFrontier_BattleFactoryLobby_EventScript_TalkedToSinglesAttendant::
	setvar VAR_LAST_TALKED, LOCALID_FACTORY_ATTENDANT_SINGLES
	return

BattleFrontier_BattleFactoryLobby_EventScript_TalkedToDoublesAttendant::
	setvar VAR_LAST_TALKED, LOCALID_FACTORY_ATTENDANT_DOUBLES
	return

BattleFrontier_BattleFactoryLobby_EventScript_ExplainChallenge::
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES, BattleFrontier_BattleFactoryLobby_EventScript_ExplainSinglesChallenge
	call_if_eq VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_DOUBLES, BattleFrontier_BattleFactoryLobby_EventScript_ExplainDoublesChallenge
	goto BattleFrontier_BattleFactoryLobby_EventScript_AskTakeChallenge

BattleFrontier_BattleFactoryLobby_EventScript_CancelChallengeSaveFailed::
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	goto BattleFrontier_BattleFactoryLobby_EventScript_CancelChallenge

BattleFrontier_BattleFactoryLobby_EventScript_LoadPartyAndCancelChallenge::
	special LoadPlayerParty
	callnative UpdateFollowingPokemon
BattleFrontier_BattleFactoryLobby_EventScript_CancelChallenge::
	msgbox BattleFrontier_BattleFactoryLobby_Text_LookForwardToNextVisit, MSGBOX_DEFAULT
	release
	end

BattleFrontier_BattleFactoryLobby_Movement_AttendantEnterDoor:
	walk_up
	walk_up
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleFactoryLobby_Movement_PlayerEnterDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleFrontier_BattleFactoryLobby_EventScript_WelcomeForSingleBattle::
	msgbox BattleFrontier_BattleFactoryLobby_Text_WelcomeForSingleBattle, MSGBOX_DEFAULT
	return

BattleFrontier_BattleFactoryLobby_EventScript_WelcomeForDoubleBattle::
	msgbox BattleFrontier_BattleFactoryLobby_Text_WelcomeForDoubleBattle, MSGBOX_DEFAULT
	return

BattleFrontier_BattleFactoryLobby_EventScript_TakeSinglesChallenge::
	message BattleFrontier_BattleFactoryLobby_Text_TakeSinglesChallenge
	return

BattleFrontier_BattleFactoryLobby_EventScript_TakeDoublesChallenge::
	message BattleFrontier_BattleFactoryLobby_Text_TakeDoublesChallenge
	return

BattleFrontier_BattleFactoryLobby_EventScript_ExplainSinglesChallenge::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainSinglesChallenge, MSGBOX_DEFAULT
	return

BattleFrontier_BattleFactoryLobby_EventScript_ExplainDoublesChallenge::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainDoublesChallenge, MSGBOX_DEFAULT
	return

BattleFrontier_BattleFactoryLobby_EventScript_ShowSinglesResults::
	lockall
	frontier_results FRONTIER_FACILITY_FACTORY, FRONTIER_MODE_SINGLES
	waitbuttonpress
	special RemoveRecordsWindow
	releaseall
	end

BattleFrontier_BattleFactoryLobby_EventScript_ShowDoublesResults::
	lockall
	frontier_results FRONTIER_FACILITY_FACTORY, FRONTIER_MODE_DOUBLES
	waitbuttonpress
	special RemoveRecordsWindow
	releaseall
	end

BattleFrontier_BattleFactoryLobby_EventScript_RulesBoard::
	lockall
	msgbox BattleFrontier_BattleFactoryLobby_Text_RulesAreListed, MSGBOX_DEFAULT
	goto BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard::
	message BattleFrontier_BattleFactoryLobby_Text_ReadWhichHeading
	waitmessage
	multichoice 17, 0, MULTI_BATTLE_FACTORY_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattleFactoryLobby_EventScript_RulesBasics
	case 1, BattleFrontier_BattleFactoryLobby_EventScript_RulesSwapPartner
	case 2, BattleFrontier_BattleFactoryLobby_EventScript_RulesSwapNumber
	case 3, BattleFrontier_BattleFactoryLobby_EventScript_RulesSwapNotes
	case 4, BattleFrontier_BattleFactoryLobby_EventScript_RulesOpenLv
	case 5, BattleFrontier_BattleFactoryLobby_EventScript_ExitRules
	case MULTI_B_PRESSED, BattleFrontier_BattleFactoryLobby_EventScript_ExitRules
	end

BattleFrontier_BattleFactoryLobby_EventScript_RulesBasics::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainBasicRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleFactoryLobby_EventScript_RulesSwapPartner::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainSwapPartnerRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleFactoryLobby_EventScript_RulesSwapNumber::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainSwapNumberRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleFactoryLobby_EventScript_RulesSwapNotes::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainSwapNotesRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleFactoryLobby_EventScript_RulesOpenLv::
	msgbox BattleFrontier_BattleFactoryLobby_Text_ExplainOpenLvRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleFactoryLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleFactoryLobby_EventScript_ExitRules::
	releaseall
	end

BattleFrontier_BattleFactoryLobby_Text_WelcomeForSingleBattle:
	.string "トレーナーの ちしきを はかる 場所!!\nバトルファクトリーに ようこそ!\pバトルトレード シングルへの\nご案内は こちらです!"
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_TakeSinglesChallenge:
	.string "バトルトレード シングルに\n挑戦 しますか?"
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExplainSinglesChallenge:
	.string "こちらでは レンタルポケモンを 使った\nバトルトレード シングル という\lイベントを 行って います!\pまずは バトルトレード せんようの\nポケモン 3匹を お菓子 します!\p次に そちらの ポケモンを\n使って シングルバトルを\l行って いただきます!\p戦いに 勝利すると 一度 だけ\nポケモンを トレード できます!\pトレードと 戦いを くりかえし\n見事 7れんしょう したら\lバトルポイントを 差し上げます!\pなお 途中で 挑戦を\n中断 したい 場合は\lきちんと レポートを かかないと\l続けて 挑戦が できません!\pくれぐれも きをつけて くださいね!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_LookForwardToNextVisit:
	.string "またの ごりようを お待ちしていますよ!$"

BattleFrontier_BattleFactoryLobby_Text_WhichLevelMode:
	.string "レベル50と オープンレベルの イベント\nどちらに 挑戦 しますか?"
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_OkayToSaveBeforeChallenge:
	.string "では 挑戦を 始める 前に\nレポートを 書きますが よろしいですか?"
	.string "$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_WillHoldMonsForSafekeeping:
	.string "{JPN}それでは ポケモンを\nお預かり します!"
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_StepThisWay:
	.string "さあさあ こちらに どうぞ!$"

BattleFrontier_BattleFactoryLobby_Text_ReturnMonsSaveResults:
	.string "お疲れさまでした!\pレンタルポケモンと\nお預かり していた ポケモンを\l交換 します!\p成績を レポートに 書きますので\n少々 お待ちください!"
	.string ""
	.string ""
	.string ""
	.string "$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_ReturnMons:
	.string "{JPN}レンタルポケモンと\nお預かり していた ポケモンを\l交換 します!"
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_CongratsSevenWins:
	.string "バトルトレード 7戦 突破\n"
	.string "おめでとう ございます!$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_AwardBattlePointsForStreak:
	.string "{JPN}7れんしょうを 記念して\nバトルポイントを 差し上げます!"
	.string "$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_MaxBattlePoints:
	.string "{JPN}おやや?\pバトルポイントが いっぱい ですねー\pポイントを ごりようしてから\nまた 来てください!"
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_WaitingForYouToResume:
	.string "お待ちしておりましたよ!\p挑戦を 再開する 前に\nレポートを 書きます!"
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_DidntSaveBeforeQuitting:
	.string "お客様ー!!\p前回 ちゃんと レポートを 書いて\n終了 しません でしたね!\pざんねんですが\n挑戦は 無効と なります!"
	.string ""
	.string ""
	.string "$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_WellReturnMons:
	.string "{JPN}お客様の ポケモンを\nおかえしします!$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_ReceivedPrizeItem:
	.string "{JPN}{PLAYER}は\n{STR_VAR_1}を もらった!"
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_WelcomeForDoubleBattle:
	.string "トレーナーの ちしきを はかる 場所!!\nバトルファクトリーに ようこそ!\pバトルトレード ダブルの\nご案内は こちらです!"
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_TakeDoublesChallenge:
	.string "バトルトレード ダブルに\n挑戦 しますか?"
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExplainDoublesChallenge:
	.string "こちらでは レンタルポケモンを 使った\nバトルトレード ダブル という\lイベントを 行って います\pまずは バトルトレード せんようの\nポケモン 3匹を お菓子 します!\p次に そちらの ポケモンを\n使って ダブルバトルを\l行って いただきます!\p戦いに 勝利すると 一度 だけ\nポケモンを トレード できます!\pトレードと 戦いを くりかえし\n見事 7れんしょうを したら\lバトルポイントを 差し上げます!\pなお 途中で 挑戦を\n中断 したい ときには\lきちんと レポートを かかないと\l続けて 挑戦が できません!\pくれぐれも きをつけて くださいね!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_CongratsForDefeatingHead:
	.string "ファクトリーヘッドへの 勝利!\nならびに 7れんしょう 達成\lおめでとう ございます!"
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_AwardBattlePoints:
	.string "お客様の ふかい ちしきを 称え\nバトルポイントを 差し上げます!"
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExchangeMonsAndSave:
	.string "レンタルポケモンと\nお預かり していた ポケモンを\l交換 しましょう!\p成績を レポートに 書きますので\n少々 お待ちください!"
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_RecordLastMatch:
	.string "バトルファクトリーで\n最後に 行った 戦いを\lフロンティアパスに 記録 いたしますか?"
	.string ""
	.string "$"

@ Unused
BattleFrontier_BattleFactoryLobby_Text_RentalMonsAreVaried:
	.string "{JPN}ぼくってー バッチリー 強いけどー\nすっごくー あきっぽい からさー\lとりあえずー\lばんばん トレード しながらー\l戦ってたのさー\lそしたらさー\l続けてー バトルトレードにー\l挑戦 したときー\lレンタル してくれるー\lポケモンのー\l種類がさー\lなんかさー\lちょっとさー\lちがったんだよねー"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_RulesAreListed:
	.string "バトルトレードの ルールが 書かれている!$"

BattleFrontier_BattleFactoryLobby_Text_ReadWhichHeading:
	.string "どの 説明を 読みますか?$"

BattleFrontier_BattleFactoryLobby_Text_ExplainBasicRules:
	.string "バトルトレードで 使うことの できる\nポケモンは 3匹 まで です\pレンタルするとき トレードするとき\nどちらの とき でも\l同じ 種類の ポケモンを\l2匹 以上 持つことは できません"
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExplainSwapPartnerRules:
	.string "トレードが できる 相手は\nあなたが 戦いで\l倒した トレーナーです\pトレードで 受け取る ことが できるのは\nトレーナーが 使っていた ポケモンです"
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExplainSwapNumberRules:
	.string "一度 戦いに かつ たびに\n1かい 1匹 だけ ポケモンを\lトレード することが できます\p1かいの 挑戦で\nトレードが できるのは\l6人めの トレーナーまで です"
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExplainSwapNotesRules:
	.string "トレードを 行う ときには\n2つ 注意する ことが あります\pその1 トレードを 行う ときは\n相手から 受け取る ポケモンの\l強さは 確認 できません\pその2 挑戦を 始めた ときに\n借りた レンタルポケモンの じゅんばんが\lトレードの あとの 戦いにも\lひきつがれます"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleFactoryLobby_Text_ExplainOpenLvRules:
	.string "オープンレベルの 挑戦では\nレンタルできる ポケモンも\l相手の トレーナーが 使う ポケモンも\lすべて レベル100に なります"
	.string ""
	.string "$"
