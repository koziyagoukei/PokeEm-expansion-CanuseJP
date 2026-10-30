BattleFrontier_BattleArenaLobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_BattleArenaLobby_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, BattleFrontier_BattleArenaLobby_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleFrontier_BattleArenaLobby_OnWarp
	.byte 0

BattleFrontier_BattleArenaLobby_OnTransition:
	@ The cook and Pokevirus only appear for Eisuke's release battle.
	setflag FLAG_TEMP_1D
	setflag FLAG_TEMP_1E
	setvar VAR_0x8004, STORY_TARGET_EISUKE
	call FrontierStory_EventScript_SetupTarget
	end

BattleFrontier_BattleArenaLobby_OnWarp:
	map_script_2 VAR_TEMP_1, 0, BattleFrontier_BattleArenaLobby_EventScript_TurnPlayerNorth
	.2byte 0

BattleFrontier_BattleArenaLobby_EventScript_TurnPlayerNorth::
	setvar VAR_TEMP_1, 1
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleFrontier_BattleArenaLobby_OnFrame:
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, 0, BattleFrontier_BattleArenaLobby_EventScript_GetChallengeStatus
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING, BattleFrontier_BattleArenaLobby_EventScript_QuitWithoutSaving
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_PAUSED, BattleFrontier_BattleArenaLobby_EventScript_ResumeChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_WON, BattleFrontier_BattleArenaLobby_EventScript_WonChallenge
	map_script_2 VAR_TEMP_CHALLENGE_STATUS, CHALLENGE_STATUS_LOST, BattleFrontier_BattleArenaLobby_EventScript_LostChallenge
	map_script_2 VAR_TEMP_E, 0, ArenaStory_EventScript_LobbyArrival
	map_script_2 VAR_TEMP_E, 1, FrontierStory_EventScript_LobbyDarkraiCall
	.2byte 0

BattleFrontier_BattleArenaLobby_EventScript_GetChallengeStatus::
	frontier_getstatus
	end

BattleFrontier_BattleArenaLobby_EventScript_QuitWithoutSaving::
	lockall
	msgbox BattleFrontier_BattleArenaLobby_Text_DidntSaveBeforeShuttingDown, MSGBOX_DEFAULT
	closemessage
	arena_set ARENA_DATA_WIN_STREAK, 0
	arena_set ARENA_DATA_WIN_STREAK_ACTIVE, FALSE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattleArenaLobby_EventScript_WonChallenge::
	lockall
	frontier_isbrain
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattleArenaLobby_EventScript_DefeatedTycoon
	msgbox BattleFrontier_BattleArenaLobby_Text_CongratsOnSevenWins, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_GiveBattlePoints

BattleFrontier_BattleArenaLobby_EventScript_DefeatedTycoon::
	msgbox BattleFrontier_BattleArenaLobby_Text_CongratsOnDefeatingTycoon, MSGBOX_DEFAULT
BattleFrontier_BattleArenaLobby_EventScript_GiveBattlePoints::
	msgbox BattleFrontier_BattleArenaLobby_Text_PleaseAcceptBattlePoints, MSGBOX_DEFAULT
	frontier_givepoints
	msgbox BattleFrontier_Text_ObtainedXBattlePoints, MSGBOX_GETPOINTS
	message BattleFrontier_BattleArenaLobby_Text_RecordAchievement
	waitmessage
	call BattleFrontier_BattleArenaLobby_EventScript_SaveAfterChallenge
	msgbox BattleFrontier_BattleArenaLobby_Text_AwaitAnotherChallenge2, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattleArenaLobby_EventScript_LostChallenge::
	lockall
	message BattleFrontier_BattleArenaLobby_Text_ThankYouWaitWhileSave
	waitmessage
	arena_set ARENA_DATA_WIN_STREAK_ACTIVE, FALSE
	call BattleFrontier_BattleArenaLobby_EventScript_SaveAfterChallenge
	msgbox BattleFrontier_BattleArenaLobby_Text_AwaitAnotherChallenge2, MSGBOX_DEFAULT
	closemessage
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	releaseall
	end

BattleFrontier_BattleArenaLobby_EventScript_SaveAfterChallenge::
	frontier_checkairshow
	special LoadPlayerParty
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	arena_save 0
	playse SE_SAVE
	waitse
	call BattleFrontier_EventScript_GetCantRecordBattle
	goto_if_eq VAR_RESULT, TRUE, BattleFrontier_BattleArenaLobby_EventScript_EndSaveAfterChallenge
	message BattleFrontier_BattleArenaLobby_Text_RecordLastMatch
	waitmessage
	multichoicedefault 20, 8, MULTI_YESNO, 1, FALSE
	switch VAR_RESULT
	case 1, BattleFrontier_BattleArenaLobby_EventScript_EndSaveAfterChallenge
	case 0, BattleFrontier_BattleArenaLobby_EventScript_RecordMatch
	case MULTI_B_PRESSED, BattleFrontier_BattleArenaLobby_EventScript_EndSaveAfterChallenge

BattleFrontier_BattleArenaLobby_EventScript_RecordMatch::
	call BattleFrontier_EventScript_SaveBattle
BattleFrontier_BattleArenaLobby_EventScript_EndSaveAfterChallenge::
	return

BattleFrontier_BattleArenaLobby_EventScript_ResumeChallenge::
	lockall
	message BattleFrontier_BattleArenaLobby_Text_LookingForwardToArrivalSaveGame
	waitmessage
	arena_save CHALLENGE_STATUS_SAVING
	playse SE_SAVE
	waitse
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	goto BattleFrontier_BattleArenaLobby_EventScript_EnterChallenge

BattleFrontier_BattleArenaLobby_EventScript_Attendant::
	lock
	faceplayer
	setvar VAR_FRONTIER_FACILITY, FRONTIER_FACILITY_ARENA
	setvar VAR_FRONTIER_BATTLE_MODE, FRONTIER_MODE_SINGLES
	special SavePlayerParty
	msgbox BattleFrontier_BattleArenaLobby_Text_WelcomeToBattleArena, MSGBOX_DEFAULT
	call FrontierStory_EventScript_StaffAside
BattleFrontier_BattleArenaLobby_EventScript_AskTakeChallenge::
	message BattleFrontier_BattleArenaLobby_Text_WishToTakeChallenge
	waitmessage
	multichoice 17, 6, MULTI_CHALLENGEINFO, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattleArenaLobby_EventScript_TryEnterChallenge
	case 1, BattleFrontier_BattleArenaLobby_EventScript_ExplainChallenge
	case 2, BattleFrontier_BattleArenaLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattleArenaLobby_EventScript_CancelChallenge

BattleFrontier_BattleArenaLobby_EventScript_TryEnterChallenge::
	message BattleFrontier_BattleArenaLobby_Text_WhichLevelMode
	waitmessage
	multichoice 17, 6, MULTI_LEVEL_MODE, FALSE
	switch VAR_RESULT
	case FRONTIER_LVL_TENT, BattleFrontier_BattleArenaLobby_EventScript_CancelChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattleArenaLobby_EventScript_CancelChallenge
	frontier_checkineligible
	goto_if_eq VAR_0x8004, TRUE, BattleFrontier_BattleArenaLobby_EventScript_NotEnoughValidMons
	frontier_set FRONTIER_DATA_LVL_MODE, VAR_RESULT
	msgbox BattleFrontier_BattleArenaLobby_Text_SelectThreeMons, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	callnative RemoveFollowingPokemon
	call BattleFrontier_EventScript_GetLvlMode
	copyvar VAR_0x8004, VAR_RESULT
	setvar VAR_0x8005, FRONTIER_PARTY_SIZE
	special ChoosePartyForBattleFrontier
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattleArenaLobby_EventScript_LoadPartyAndCancelChallenge
	msgbox BattleFrontier_BattleArenaLobby_Text_OkayToSave, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, BattleFrontier_BattleArenaLobby_EventScript_LoadPartyAndCancelChallenge
	case YES, BattleFrontier_BattleArenaLobby_EventScript_SaveBeforeChallenge
	case MULTI_B_PRESSED, BattleFrontier_BattleArenaLobby_EventScript_LoadPartyAndCancelChallenge

BattleFrontier_BattleArenaLobby_EventScript_SaveBeforeChallenge::
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
	arena_init
	arena_set ARENA_DATA_WIN_STREAK_ACTIVE, TRUE
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, CHALLENGE_STATUS_SAVING
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	special LoadPlayerParty
	closemessage
	delay 2
	call Common_EventScript_SaveGame
	setvar VAR_TEMP_CHALLENGE_STATUS, 255
	goto_if_eq VAR_RESULT, 0, BattleFrontier_BattleArenaLobby_EventScript_CancelChallengeSaveFailed
BattleFrontier_BattleArenaLobby_EventScript_EnterChallenge::
	special SavePlayerParty
	frontier_setpartyorder FRONTIER_PARTY_SIZE
	msgbox BattleFrontier_BattleArenaLobby_Text_GuideYouToArena, MSGBOX_DEFAULT
	closemessage
	frontier_get FRONTIER_DATA_LVL_MODE
	call_if_eq VAR_RESULT, FRONTIER_LVL_50, BattleFrontier_BattleArenaLobby_EventScript_WalkToDoorLv50
	call_if_eq VAR_RESULT, FRONTIER_LVL_OPEN, BattleFrontier_BattleArenaLobby_EventScript_WalkToDoorLvOpen
	warp MAP_BATTLE_FRONTIER_BATTLE_ARENA_CORRIDOR, 9, 13
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	waitstate
	end

BattleFrontier_BattleArenaLobby_EventScript_ExplainChallenge::
	msgbox BattleFrontier_BattleArenaLobby_Text_ExplainChallenge, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_AskTakeChallenge

BattleFrontier_BattleArenaLobby_EventScript_NotEnoughValidMons::
	switch VAR_RESULT
	case FRONTIER_LVL_50, BattleFrontier_BattleArenaLobby_EventScript_NotEnoughValidMonsLv50
	case FRONTIER_LVL_OPEN, BattleFrontier_BattleArenaLobby_EventScript_NotEnoughValidMonsLvOpen

BattleFrontier_BattleArenaLobby_EventScript_NotEnoughValidMonsLv50::
	msgbox BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsLv50, MSGBOX_DEFAULT
	call BattleFrontier_ShowCaughtBannedSpecies
	msgbox BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsEnd, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_EndCancelChallenge

BattleFrontier_BattleArenaLobby_EventScript_NotEnoughValidMonsLvOpen::
	msgbox BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsLvOpen, MSGBOX_DEFAULT
	call BattleFrontier_ShowCaughtBannedSpecies
	msgbox BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsEnd, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_EndCancelChallenge

BattleFrontier_BattleArenaLobby_EventScript_CancelChallengeSaveFailed::
	frontier_set FRONTIER_DATA_CHALLENGE_STATUS, 0
	goto BattleFrontier_BattleArenaLobby_EventScript_CancelChallenge

BattleFrontier_BattleArenaLobby_EventScript_LoadPartyAndCancelChallenge::
	special LoadPlayerParty
	callnative UpdateFollowingPokemon
BattleFrontier_BattleArenaLobby_EventScript_CancelChallenge::
	msgbox BattleFrontier_BattleArenaLobby_Text_AwaitAnotherChallenge, MSGBOX_DEFAULT
BattleFrontier_BattleArenaLobby_EventScript_EndCancelChallenge::
	release
	end

BattleFrontier_BattleArenaLobby_EventScript_WalkToDoorLv50::
	applymovement LOCALID_ARENA_ATTENDANT, BattleFrontier_BattleArenaLobby_Movement_AttendantWalkToLeftDoor
	applymovement LOCALID_PLAYER, BattleFrontier_BattleArenaLobby_Movement_PlayerWalkToLeftDoor
	waitmovement 0
	opendoor 2, 2
	waitdooranim
	applymovement LOCALID_ARENA_ATTENDANT, BattleFrontier_BattleArenaLobby_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, BattleFrontier_BattleArenaLobby_Movement_PlayerEnterDoor
	waitmovement 0
	closedoor 2, 2
	waitdooranim
	return

BattleFrontier_BattleArenaLobby_Movement_AttendantWalkToLeftDoor:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

BattleFrontier_BattleArenaLobby_Movement_AttendantEnterDoor:
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleArenaLobby_Movement_PlayerWalkToLeftDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

BattleFrontier_BattleArenaLobby_Movement_PlayerEnterDoor:
	walk_up
	walk_up
	set_invisible
	step_end

BattleFrontier_BattleArenaLobby_EventScript_WalkToDoorLvOpen::
	applymovement LOCALID_ARENA_ATTENDANT, BattleFrontier_BattleArenaLobby_Movement_AttendantWalkToRightDoor
	applymovement LOCALID_PLAYER, BattleFrontier_BattleArenaLobby_Movement_PlayerWalkToRightDoor
	waitmovement 0
	opendoor 11, 2
	waitdooranim
	applymovement LOCALID_ARENA_ATTENDANT, BattleFrontier_BattleArenaLobby_Movement_AttendantEnterDoor
	applymovement LOCALID_PLAYER, BattleFrontier_BattleArenaLobby_Movement_PlayerEnterDoor
	waitmovement 0
	closedoor 11, 2
	waitdooranim
	return

BattleFrontier_BattleArenaLobby_Movement_AttendantWalkToRightDoor:
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	step_end

BattleFrontier_BattleArenaLobby_Movement_PlayerWalkToRightDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

BattleFrontier_BattleArenaLobby_EventScript_ShowResults::
	lockall
	frontier_results FRONTIER_FACILITY_ARENA
	waitbuttonpress
	special RemoveRecordsWindow
	releaseall
	end

BattleFrontier_BattleArenaLobby_EventScript_RulesBoard::
	lockall
	msgbox BattleFrontier_BattleArenaLobby_Text_RulesAreListed, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleArenaLobby_EventScript_ReadRulesBoard::
	message BattleFrontier_BattleArenaLobby_Text_ReadWhichHeading
	waitmessage
	multichoice 17, 2, MULTI_BATTLE_ARENA_RULES, FALSE
	switch VAR_RESULT
	case 0, BattleFrontier_BattleArenaLobby_EventScript_BattleRules
	case 1, BattleFrontier_BattleArenaLobby_EventScript_MindRules
	case 2, BattleFrontier_BattleArenaLobby_EventScript_SkillRules
	case 3, BattleFrontier_BattleArenaLobby_EventScript_BodyRules
	case 4, BattleFrontier_BattleArenaLobby_EventScript_ExitRules
	case MULTI_B_PRESSED, BattleFrontier_BattleArenaLobby_EventScript_ExitRules
	end

BattleFrontier_BattleArenaLobby_EventScript_BattleRules::
	msgbox BattleFrontier_BattleArenaLobby_Text_ExplainBattleRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleArenaLobby_EventScript_MindRules::
	msgbox BattleFrontier_BattleArenaLobby_Text_ExplainMindRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleArenaLobby_EventScript_SkillRules::
	msgbox BattleFrontier_BattleArenaLobby_Text_ExplainSkillRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleArenaLobby_EventScript_BodyRules::
	msgbox BattleFrontier_BattleArenaLobby_Text_ExplainBodyRules, MSGBOX_DEFAULT
	goto BattleFrontier_BattleArenaLobby_EventScript_ReadRulesBoard
	end

BattleFrontier_BattleArenaLobby_EventScript_ExitRules::
	releaseall
	end

BattleFrontier_BattleArenaLobby_Text_WelcomeToBattleArena:
	.string "トレーナーの とうしを 試す 場所!!\pバトルアリーナへ\nようこそ おいでなされた!\pこちらでは 勝ちぬきチームバトルの\n案内を しておりますぞ!"
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_WishToTakeChallenge:
	.string "バトルアリーナに\n挑戦 されますかな?"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_AwaitAnotherChallenge:
	.string "またの 挑戦を\nお待ちして おりますぞ!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_ExplainChallenge:
	.string "バトルアリーナは 勝ちぬきチームバトルを\n行う 施設ですぞ!\p参加する かたには 出場 させる\n3匹の ポケモンと 戦いに 出す\lじゅんばんを 決めて いただきますぞ!\p戦いの あいだは\nこの じゅんばん どおりに\lポケモンを 出すことに なりますな!\p戦いに 一度 だした ポケモンは\nけっちゃくが つくまで\l交代 させる ことが できませぬ!\pさらに 3ターン いないに\n戦いの けっちゃくが つかなければ\l判定と なりますぞ!\pまた 途中で 挑戦を\n中断したい 場合は\lレポートを かかないと\l続けて 挑戦が できないので\lおきをつけ くだされ!\p見事 7人ぬきを なされた あかつきには\nバトルポイントを お送り いたしますぞ!"
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
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_OkayToSave:
	.string "バトルアリーナへ お連れする 前に\nレポートを 書きますが よろしいですかな?"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_WhichLevelMode:
	.string "バトルアリーナには\n"
	.string "レベル50と オープンレベルと ありますが\l"
	.string "どちらに 参られますかな?$"

BattleFrontier_BattleArenaLobby_Text_SelectThreeMons:
	.string "では 参加する ポケモンを\n3匹 お選び くだされ!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsLvOpen:
	.string "お客人!!\p参加できる ポケモンが\n3匹 おりませぬ!\p参加する ためには\nすべて 違う 種類の ポケモンを\l3匹 用意して くだされ!\pまた 持たせる 道具も\nすべて 違う 道具に してくだされ!\pなお タマゴ{STR_VAR_1}参加 させる ことは できませぬ!\p準備が できましたら\nまた お越し くだされ!$"

BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsEnd:
	.string "準備が できましたら\nまた お越し くだされ!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_NotEnoughValidMonsLv50:
	.string "お客人!!\p参加できる ポケモンが\n3匹 おりませぬ!\p参加する ためには レベルが 50いかで\nすべて 違う 種類の ポケモンを\l3匹 用意して くだされ!\pまた 持たせる 道具も\nすべて 違う 道具に してくだされ!\pなお タマゴ{STR_VAR_1}参加 させる ことは できませぬ!\p準備が できましたら\nまた お越し くだされ!$"

BattleFrontier_BattleArenaLobby_Text_GuideYouToArena:
	.string "では バトルアリーナへ\nお連れいたしますぞ!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_DidntSaveBeforeShuttingDown:
	.string "お客人!\p前回 レポートを 書いて\n終了 しませんでしたな?\pざんねんですが その 場合\n挑戦は 無効と なりますぞ!\pまた 始めから\n挑戦 してくだされ!!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_CongratsOnSevenWins:
	.string "7人ぬき 達成\nおみごとで ございました!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_RecordAchievement:
	.string "成績を レポートに 書きますので\nお待ちくだされ!"
	.string "$"

@ Unused
BattleFrontier_BattleArenaLobby_Text_PresentYouWithPrize:
	.string "{JPN}7人ぬきを 達成 されましたので\nこの 賞品を お受け取り くだされ!"
	.string "$"

@ Unused
BattleFrontier_BattleArenaLobby_Text_ReceivedPrize:
	.string "{JPN}{PLAYER}は\n{STR_VAR_1}を もらった!"
	.string "$"

@ Unused
BattleFrontier_BattleArenaLobby_Text_BagFullReturnForPrize:
	.string "{JPN}……おおっ!\n道具が いっぱいの ようですな!\p道具を 整理してから\nまた お越しくだされ!"
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_ThankYouWaitWhileSave:
	.string "お疲れさまでした!\p成績を レポートに 書きますので\nお待ちくだされ!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_AwaitAnotherChallenge2:
	.string "またの 挑戦を\nお待ちして おりますぞ!"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_LookingForwardToArrivalSaveGame:
	.string "お待ちしておりました!\pバトルアリーナに お連れする 前に\nレポートを 書きますので お待ちくだされ!"
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_RecordLastMatch:
	.string "バトルアリーナで\n最後に 行った 戦いを\lフロンティアパスに 記録 されますかな?"
	.string "$"

BattleFrontier_BattleArenaLobby_Text_RulesAreListed:
	.string "勝ちぬきチームバトルの\nルールが 書かれている!$"

BattleFrontier_BattleArenaLobby_Text_ReadWhichHeading:
	.string "どの 説明を 読みますか?$"

BattleFrontier_BattleArenaLobby_Text_ExplainBattleRules:
	.string "勝ちぬきチームバトルは ふつうの 戦いと\n違い 特別な ルールが あります\pまず 1かいの 戦いは\n3ターンで 終了と なります\p3ターンで 勝負が つかないときは\n審判に よる 判定に なります\p判定は ポケモンの 戦いかたを\n見て しょうはいを 決定 します\pまた 勝負の けっちゃくが つくまで\nポケモンの 交代は できません"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_ExplainMindRules:
	.string "1つめの 判定 ポイントである\n「心」は\lせめる 気持ちを みられます\pどれだけ トレーナーが 攻撃する 技を\nめいれい したかに よって\l評価が 代わります"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_ExplainSkillRules:
	.string "2つめの 判定 ポイントである\n「技」は ポケモンの 技が\lうまく できたかを みられます\p技が 成功 すると\n評価は あがります\p技が 失敗 すると\n評価は さがります\pさらに 相手に 技を あてたときは\n「効果はばつぐん」で あがり\l「効果はいまひとつ」などで さがります\pまた 「守る」や 「みきり」は\nだしても 評価は あがりません\p相手の 「守る」や 「みきり」で\n技を 出せなくても\l評価は さがりません"
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
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_ExplainBodyRules:
	.string "3つめの 判定 ポイントである\n「体」は 対戦 したあとの\lポケモンの HPを みられます\p対戦が 始まる 前の\nHPと 比べて\l対戦ごの HPが どれだけ あるかで\l評価が 代わります"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_CongratsOnDefeatingTycoon:
	.string "アリーナキャプテンからの きんぼし!\nならびに 7人ぬき 達成!\lまこと 見事で ございました!"
	.string ""
	.string ""
	.string "$"

BattleFrontier_BattleArenaLobby_Text_PleaseAcceptBattlePoints:
	.string "お客人の ふくつの とうしを 称え\nバトルポイントを 差し上げましょう!"
	.string ""
	.string "$"
