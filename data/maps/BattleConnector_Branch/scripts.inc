@ Battle Connector: a three-way branch (src/battle_connector.c). Stepping into a doorway gives that path's hint.
BattleConnector_Branch_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, BattleConnector_Branch_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, BattleConnector_Branch_OnWarp
	.byte 0

BattleConnector_Branch_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	special BattleConnector_PrepareBranch
	end

BattleConnector_Branch_OnWarp:
	map_script_2 VAR_TEMP_4, 0, BattleConnector_EventScript_TurnPlayerNorth
	.2byte 0

BattleConnector_Branch_EventScript_LeftPath::
	setvar VAR_0x8004, CONNECTOR_PATH_LEFT
	goto BattleConnector_Branch_EventScript_ChoosePath
	end

BattleConnector_Branch_EventScript_CenterPath::
	setvar VAR_0x8004, CONNECTOR_PATH_CENTER
	goto BattleConnector_Branch_EventScript_ChoosePath
	end

BattleConnector_Branch_EventScript_RightPath::
	setvar VAR_0x8004, CONNECTOR_PATH_RIGHT
	goto BattleConnector_Branch_EventScript_ChoosePath
	end

BattleConnector_Branch_EventScript_ChoosePath::
	lockall
	special BattleConnector_BufferPathHint
	msgbox BattleConnector_Branch_Text_PathHint, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_Branch_EventScript_StepBack
	special BattleConnector_EnterPath
	closemessage
	warpsilent MAP_BATTLE_CONNECTOR_ROOM, 4, 7
	waitstate
	end

BattleConnector_Branch_EventScript_StepBack::
	closemessage
	applymovement LOCALID_PLAYER, BattleConnector_Movement_StepDown
	waitmovement 0
	releaseall
	end

BattleConnector_Branch_EventScript_Staff::
	lock
	faceplayer
	special BattleConnector_BufferStatus
	msgbox BattleConnector_Branch_Text_Status, MSGBOX_DEFAULT
	msgbox BattleConnector_Branch_Text_AskRetire, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleConnector_EventScript_Retire
	msgbox BattleConnector_Branch_Text_KeepGoing, MSGBOX_DEFAULT
	release
	end

BattleConnector_Branch_Text_PathHint:
	.string "{JPN}この さきからは\n{STR_VAR_1}……\pすすみますか？$"

BattleConnector_Branch_Text_Status:
	.string "{JPN}いまは {STR_VAR_1}ばんめの わかれみちです。\nほごした ポケモンは {STR_VAR_2}ひき。$"

BattleConnector_Branch_Text_AskRetire:
	.string "{JPN}ちょうせんを やめて\nうけつけへ もどりますか？$"

BattleConnector_Branch_Text_KeepGoing:
	.string "{JPN}けはいに みみを すまして\nみちを えらんで ください。$"
