Route110_TrickHousePuzzle4_MapScripts::
	.byte 0

Route110_TrickHousePuzzle4_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_4_STATE, 0, Route110_TrickHousePuzzle4_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle4_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_4_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle4_EventScript_Cora::
	trainerbattle_single TRAINER_CORA, Route110_TrickHousePuzzle4_Text_CoraIntro, Route110_TrickHousePuzzle4_Text_CoraDefeat
	msgbox Route110_TrickHousePuzzle4_Text_CoraPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_EventScript_Yuji::
	trainerbattle_single TRAINER_YUJI, Route110_TrickHousePuzzle4_Text_YujiIntro, Route110_TrickHousePuzzle4_Text_YujiDefeat
	msgbox Route110_TrickHousePuzzle4_Text_YujiPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_EventScript_Paula::
	trainerbattle_single TRAINER_PAULA, Route110_TrickHousePuzzle4_Text_PaulaIntro, Route110_TrickHousePuzzle4_Text_PaulaDefeat
	msgbox Route110_TrickHousePuzzle4_Text_PaulaPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle4_Text_WroteSecretCodeLockOpened:
	.string "{JPN}{PLAYER}は あいことばを\nとびらに かきつけた!\p「カラクリだいおう さま かっこいい」\p…… …… ……\pかぎの はずれる おとがした!$"

Route110_TrickHousePuzzle4_Text_CoraIntro:
	.string "{JPN}こういうの かんがえるのって めんどう!\nわたしは しょうぶだけ したいのにー!$"

Route110_TrickHousePuzzle4_Text_CoraDefeat:
	.string "{JPN}まけちゃった けど やっぱり\nしょうぶが いちばん たのしい!$"

Route110_TrickHousePuzzle4_Text_CoraPostBattle:
	.string "{JPN}トレーナーが いそうな ところは\nどこにでも いきたく なるわよね!$"

Route110_TrickHousePuzzle4_Text_YujiIntro:
	.string "{JPN}このくらいの いわは\nゆび いっぽんで らくしょう だぜ!$"

Route110_TrickHousePuzzle4_Text_YujiDefeat:
	.string "{JPN}いわが おせても\nパズルが とけない……$"

Route110_TrickHousePuzzle4_Text_YujiPostBattle:
	.string "{JPN}ちからが つよいだけじゃ ダメだね……\nアタマ つかわないと$"

Route110_TrickHousePuzzle4_Text_PaulaIntro:
	.string "{JPN}カラクリやしきも だんだん\nむずかしく なってきたわね!$"

Route110_TrickHousePuzzle4_Text_PaulaDefeat:
	.string "{JPN}ああっ!$"

Route110_TrickHousePuzzle4_Text_PaulaPostBattle:
	.string "{JPN}さいごまで いった\nひとって いるのかしらね?$"

