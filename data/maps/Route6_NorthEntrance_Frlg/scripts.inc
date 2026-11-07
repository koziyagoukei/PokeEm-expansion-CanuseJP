Route6_NorthEntrance_Frlg_MapScripts::
	.byte 0

Route6_NorthEntrance_EventScript_Guard::
	msgbox Route6_NorthEntrance_Text_HiHowsItGoing, MSGBOX_NPC
	end

Route6_NorthEntrance_EventScript_GuardTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route6_NorthEntrance_EventScript_GuardTrigger
	end

Route6_NorthEntrance_EventScript_GuardTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route6_NorthEntrance_EventScript_GuardTrigger
	end

Route6_NorthEntrance_EventScript_GuardTriggerRight::
	lockall
	setvar VAR_TEMP_1, 2
	goto Route6_NorthEntrance_EventScript_GuardTrigger
	end

Route6_NorthEntrance_EventScript_GuardTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto_if_set FLAG_GOT_TEA, Route6_NorthEntrance_EventScript_GiveTea
	msgbox Route6_NorthEntrance_Text_ThirstyOnGuardDuty
	closemessage
	applymovement LOCALID_PLAYER, Route6_NorthEntrance_Movement_BlockPlayerEntry
	waitmovement 0
	releaseall
	end

Route6_NorthEntrance_EventScript_GiveTea::
	removeitem ITEM_TEA
	goto Route6_NorthEntrance_EventScript_GuardDrinkTea
	end

Route6_NorthEntrance_EventScript_GuardDrinkTea::
	msgbox Route6_NorthEntrance_Text_ThatTeaLooksTasty
	closemessage
	call_if_eq VAR_TEMP_1, 0, Route6_NorthEntrance_EventScript_WalkToGuardLeft
	call_if_eq VAR_TEMP_1, 1, Route6_NorthEntrance_EventScript_WalkToGuardMid
	call_if_eq VAR_TEMP_1, 2, Route6_NorthEntrance_EventScript_WalkToGuardRight
	msgbox Route6_NorthEntrance_Text_ThanksIllShareTeaWithGuards
	setvar VAR_MAP_SCENE_ROUTE5_ROUTE6_ROUTE7_ROUTE8_GATES, 1
	releaseall
	end

Route6_NorthEntrance_EventScript_WalkToGuardLeft::
	applymovement LOCALID_PLAYER, Route6_NorthEntrance_Movement_WalkToGuardLeft
	waitmovement 0
	return

Route6_NorthEntrance_EventScript_WalkToGuardMid::
	applymovement LOCALID_PLAYER, Route6_NorthEntrance_Movement_WalkToGuardMid
	waitmovement 0
	return

Route6_NorthEntrance_EventScript_WalkToGuardRight::
	applymovement LOCALID_PLAYER, Route6_NorthEntrance_Movement_WalkToGuardRight
	waitmovement 0
	return

Route6_NorthEntrance_Movement_WalkToGuardLeft::
	walk_right
	walk_right
	step_end

Route6_NorthEntrance_Movement_WalkToGuardMid::
	walk_right
	step_end

Route6_NorthEntrance_Movement_WalkToGuardRight::
	walk_in_place_right
	step_end

Route6_NorthEntrance_Movement_BlockPlayerEntry::
	walk_down
	step_end

Route6_NorthEntrance_Text_ThirstyOnGuardDuty::
	.string "{JPN}わたし まじめな けいびいん\n"
	.string "はー…\l"
	.string "のどが かわいた!\l"
	.string "おっと そっちは\l"
	.string "いま つうこう きんしだよ$"

Route6_NorthEntrance_Text_ThatTeaLooksTasty::
	.string "{JPN}あ それは\l"
	.string "おいしそうな おちゃ…$"

Route6_NorthEntrance_Text_ThanksIllShareTeaWithGuards::
	.string "{JPN}… …\l"
	.string "え?\l"
	.string "ぼくに くれる?\l"
	.string "サンキュー!\n"
	.string "… …\l"
	.string "ゴクゴク…\l"
	.string "…\l"
	.string "ゴクゴク… …\l"
	.string "ヤマブキ シティに\l"
	.string "いく なら…\l"
	.string "…\l"
	.string "… …\l"
	.string "とおって いいよ\l"
	.string "おちゃを\l"
	.string "むこうの ゲートの\l"
	.string "けいびいんにも わけてあげよう…$"

Route6_NorthEntrance_Text_HiHowsItGoing::
	.string "{JPN}あ どーも どーも!$"

