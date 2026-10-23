Route110_TrickHousePuzzle1_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route110_TrickHousePuzzle1_OnLoad
	.byte 0

Route110_TrickHousePuzzle1_OnLoad:
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_1_STATE, 2, Route110_TrickHousePuzzle1_EventScript_OpenDoor
	end

Route110_TrickHousePuzzle1_EventScript_OpenDoor::
	setmetatile 13, 1, METATILE_TrickHousePuzzle_Stairs_Down, FALSE
	end

Route110_TrickHousePuzzle1_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_1_STATE, 0, Route110_TrickHousePuzzle1_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle1_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_1_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle1_EventScript_Sally::
	trainerbattle_single TRAINER_SALLY, Route110_TrickHousePuzzle1_Text_SallyIntro, Route110_TrickHousePuzzle1_Text_SallyDefeat
	msgbox Route110_TrickHousePuzzle1_Text_SallyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_EventScript_Eddie::
	trainerbattle_single TRAINER_EDDIE, Route110_TrickHousePuzzle1_Text_EddieIntro, Route110_TrickHousePuzzle1_Text_EddieDefeat
	msgbox Route110_TrickHousePuzzle1_Text_EddiePostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_EventScript_Robin::
	trainerbattle_single TRAINER_ROBIN, Route110_TrickHousePuzzle1_Text_RobinIntro, Route110_TrickHousePuzzle1_Text_RobinDefeat
	msgbox Route110_TrickHousePuzzle1_Text_RobinPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle1_Text_WroteSecretCodeLockOpened::
	.string "{PLAYER}は あいことばを\nとびらに かきつけた!\p「カラクリだいおう さま すてき」\p…… …… ……\pかぎの はずれる おとがした!$"

Route110_TrickHousePuzzle1_Text_SallyIntro:
	.string "おぼえさせた ばかりの いあいぎりで\nきって きって きりまくりよ!$"

Route110_TrickHousePuzzle1_Text_SallyDefeat:
	.string "ほんき ださないでよー!$"

Route110_TrickHousePuzzle1_Text_SallyPostBattle:
	.string "きっても きっても あきないわ!$"

Route110_TrickHousePuzzle1_Text_EddieIntro:
	.string "へんな やしきに\nまよいこんじゃった……$"

Route110_TrickHousePuzzle1_Text_EddieDefeat:
	.string "おまけに まけちゃった……$"

Route110_TrickHousePuzzle1_Text_EddiePostBattle:
	.string "さらに まよっちゃった……\nでられない……$"

Route110_TrickHousePuzzle1_Text_RobinIntro:
	.string "カラクリだいおう って\nなにもの なのかしら?$"

Route110_TrickHousePuzzle1_Text_RobinDefeat:
	.string "きを とられていたら\nまけちゃったわ!$"

Route110_TrickHousePuzzle1_Text_RobinPostBattle:
	.string "つよいわ!\nあなたも なにもの なの?$"

