MiniMiniVilla_LocalRoom_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LocalRoom_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, LocalRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, LocalRoom_OnWarp
	map_script MAP_SCRIPT_ON_RESUME, LocalRoom_OnResume
	.byte 0

LocalRoom_OnTransition:
	setvar VAR_TEMP_0, 0
	special LocalRoom_PrepareMap
	end

LocalRoom_OnLoad:
	special LocalRoom_LoadDecorations
	end

LocalRoom_OnWarp:
	map_script_2 VAR_TEMP_0, 0, LocalRoom_EventScript_InitObjects
	.2byte 0

LocalRoom_EventScript_InitObjects:
	special LocalRoom_InitObjects
	setvar VAR_TEMP_0, 1
	end

LocalRoom_OnResume:
	setstepcallback STEP_CB_LOCAL_ROOM
	special LocalRoom_Resume
	end

LocalRoom_EventScript_InitDecorations::
	setvar VAR_0x8004, 0
	special InitSecretBaseDecorationSprites
	end

LocalRoom_EventScript_PutAwayDecoration::
	setvar VAR_0x8004, 0
LocalRoom_EventScript_PutAwayLoop:
	special PutAwayDecorationIteration
	goto_if_eq VAR_RESULT, 1, LocalRoom_EventScript_PutAwayDone
	addvar VAR_0x8004, 1
	goto_if_eq VAR_0x8005, 0, LocalRoom_EventScript_PutAwayLoop
	removeobject VAR_0x8006
	goto LocalRoom_EventScript_PutAwayLoop
LocalRoom_EventScript_PutAwayDone:
	end

LocalRoom_EventScript_PC::
	lockall
	playse SE_PC_LOGIN
	msgbox LocalRoom_Text_BootPC, MSGBOX_DEFAULT
	goto LocalRoom_EventScript_PCMainMenu

LocalRoom_EventScript_PCMainMenu::
	lockall
	message LocalRoom_Text_Choose
	waitmessage
	multichoice 0, 0, MULTI_LOCAL_ROOM_PC, FALSE
	switch VAR_RESULT
	case 0, LocalRoom_EventScript_Decorate
	case 1, LocalRoom_EventScript_Floor
	case 2, LocalRoom_EventScript_Wall
	case 3, LocalRoom_EventScript_Roamers
	case 4, LocalRoom_EventScript_Clear
	goto LocalRoom_EventScript_ClosePC

LocalRoom_EventScript_Decorate:
	special LocalRoom_ShowDecorationMenu
	end

LocalRoom_EventScript_Floor:
	message LocalRoom_Text_Floor
	waitmessage
	multichoice 0, 0, MULTI_LOCAL_ROOM_FLOOR_STYLE, FALSE
	special LocalRoom_SetFloorStyle
	goto LocalRoom_EventScript_PCMainMenu

LocalRoom_EventScript_Wall:
	message LocalRoom_Text_Wall
	waitmessage
	multichoice 0, 0, MULTI_LOCAL_ROOM_WALL_STYLE, FALSE
	special LocalRoom_SetWallStyle
	goto LocalRoom_EventScript_PCMainMenu

LocalRoom_EventScript_Roamers:
	message LocalRoom_Text_Roamers
	waitmessage
	multichoice 0, 0, MULTI_LOCAL_ROOM_ROAMERS, FALSE
	special LocalRoom_SetRoamerCount
	goto LocalRoom_EventScript_PCMainMenu

LocalRoom_EventScript_Clear:
	msgbox LocalRoom_Text_Clear, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, LocalRoom_EventScript_PCMainMenu
	special LocalRoom_ClearDecorations
	goto LocalRoom_EventScript_PCMainMenu

LocalRoom_EventScript_ClosePC:
	playse SE_PC_OFF
	closemessage
	releaseall
	end

LocalRoom_EventScript_Mon::
	lock
	faceplayer
	special LocalRoom_BufferMonMessage
	msgbox LocalRoom_Text_Mon, MSGBOX_DEFAULT
	release
	end

LocalRoom_EventScript_Ornament::
	msgbox LocalRoom_Text_Ornament, MSGBOX_SIGN
	end

LocalRoom_EventScript_SandOrnament::
	dofieldeffect FLDEFF_SAND_PILLAR
	waitstate
	end

LocalRoom_Text_BootPC:
	.string "{JPN}マイルームの パソコンを ひらいた！$"
LocalRoom_Text_Choose:
	.string "{JPN}なにを しますか？$"
LocalRoom_Text_Floor:
	.string "{JPN}ゆかを えらんでください。$"
LocalRoom_Text_Wall:
	.string "{JPN}かべを えらんでください。$"
LocalRoom_Text_Roamers:
	.string "{JPN}てもちの ポケモンを\nなんびき はなしますか？$"
LocalRoom_Text_Clear:
	.string "{JPN}へやの かざりを ぜんぶ\nパソコンに もどしますか？$"
LocalRoom_Text_Mon:
	.string "{JPN}{STR_VAR_1}は\nのんびり すごしている。$"
LocalRoom_Text_Ornament:
	.string "{JPN}じぶんの おきにいりの かざりだ。$"
