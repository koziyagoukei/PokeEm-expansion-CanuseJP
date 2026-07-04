Route110_TrickHousePuzzle8_MapScripts::
	.byte 0

Route110_TrickHousePuzzle8_EventScript_Scroll::
	lockall
	goto_if_eq VAR_TRICK_HOUSE_PUZZLE_8_STATE, 0, Route110_TrickHousePuzzle8_EventScript_FoundScroll
	goto Route110_TrickHousePuzzle_EventScript_ReadScrollAgain
	end

Route110_TrickHousePuzzle8_EventScript_FoundScroll::
	setvar VAR_TRICK_HOUSE_PUZZLE_8_STATE, 1
	goto Route110_TrickHousePuzzle_EventScript_FoundScroll
	end

Route110_TrickHousePuzzle8_EventScript_Vincent::
	trainerbattle_single TRAINER_VINCENT, Route110_TrickHousePuzzle8_Text_VincentIntro, Route110_TrickHousePuzzle8_Text_VincentDefeat
	msgbox Route110_TrickHousePuzzle8_Text_VincentPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle8_EventScript_Keira::
	trainerbattle_single TRAINER_KEIRA, Route110_TrickHousePuzzle8_Text_KeiraIntro, Route110_TrickHousePuzzle8_Text_KeiraDefeat
	msgbox Route110_TrickHousePuzzle8_Text_KeiraPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle8_EventScript_Leroy::
	trainerbattle_single TRAINER_LEROY, Route110_TrickHousePuzzle8_Text_LeroyIntro, Route110_TrickHousePuzzle8_Text_LeroyDefeat
	msgbox Route110_TrickHousePuzzle8_Text_LeroyPostBattle, MSGBOX_AUTOCLOSE
	end

Route110_TrickHousePuzzle8_Text_WroteSecretCodeLockOpened::
	.string "{JPN}{PLAYER}は あいことばを\nとびらに かきつけた!\p「カラクリだいおう さま あいしてる」\p…… …… ……\pかぎの はずれる おとがした!$"

Route110_TrickHousePuzzle8_Text_VincentIntro:
	.string "{JPN}ここまで こられる トレーナーは\nそうは いないよな!$"

Route110_TrickHousePuzzle8_Text_VincentDefeat:
	.string "{JPN}つまり キミも\nすごいって ことだよな…$"

Route110_TrickHousePuzzle8_Text_VincentPostBattle:
	.string "{JPN}えっ ポケモンリーグの チャンピオンに\nかったって? す すごい……!$"

Route110_TrickHousePuzzle8_Text_KeiraIntro:
	.string "{JPN}わたしと しょうぶ できるなんて\nあなた しあわせよ!$"

Route110_TrickHousePuzzle8_Text_KeiraDefeat:
	.string "{JPN}そんな わたしが\nまけるなんて……!$"

Route110_TrickHousePuzzle8_Text_KeiraPostBattle:
	.string "{JPN}わたしに かつなんて きせきよ!\nじまん できる ことだわ!$"

Route110_TrickHousePuzzle8_Text_LeroyIntro:
	.string "{JPN}あなたも カラクリやしきに\nちょうせん しつづけて いるんですね!$"

Route110_TrickHousePuzzle8_Text_LeroyDefeat:
	.string "{JPN}なるほど……\nすばらしい センスですね!$"

Route110_TrickHousePuzzle8_Text_LeroyPostBattle:
	.string "{JPN}あなた みたいな ひとが きてくれて\nカラクリだいおうも まんぞく でしょうね!$"
