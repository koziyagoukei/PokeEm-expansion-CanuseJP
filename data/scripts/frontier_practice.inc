FrontierPractice_EventScript_Trainer::
	lockall
	faceplayer
	special Special_FrontierPracticeInit
	msgbox FrontierPractice_Text_Welcome, MSGBOX_DEFAULT
	goto FrontierPractice_Menu

FrontierPractice_Menu:
	special Special_FrontierPracticeBufferSettings
	msgbox FrontierPractice_Text_Settings, MSGBOX_DEFAULT
	dynmultipush FrontierPractice_Text_Add, 0
	dynmultipush FrontierPractice_Text_Remove, 1
	dynmultipush FrontierPractice_Text_Team, 2
	dynmultipush FrontierPractice_Text_AI, 3
	dynmultipush FrontierPractice_Text_Format, 4
	dynmultipush FrontierPractice_Text_Level, 5
	dynmultipush FrontierPractice_Text_Battle, 6
	dynmultipush FrontierPractice_Text_Exit, 7
	dynmultistack 0, 0, FALSE, 6, 0, 0, DYN_MULTICHOICE_CB_NONE
	switch VAR_RESULT
	case 0, FrontierPractice_Add
	case 1, FrontierPractice_RemoveMenu
	case 2, FrontierPractice_Team
	case 3, FrontierPractice_AI
	case 4, FrontierPractice_Format
	case 5, FrontierPractice_Level
	case 6, FrontierPractice_Battle
	releaseall
	end

FrontierPractice_Add:
	special Special_FrontierPracticeBufferSettings
	goto_if_ge VAR_RESULT, 6, FrontierPractice_Full
	closemessage
FrontierPractice_ChooseBox::
	chooseboxmon SELECT_PC_MON_FRONTIER_PRACTICE
	waitstate
	goto_if_eq VAR_RESULT, FALSE, FrontierPractice_Menu
	special Special_FrontierPracticeAddSelection
	goto_if_ne VAR_RESULT, PRACTICE_OK, FrontierPractice_Invalid
	msgbox FrontierPractice_Text_Added, MSGBOX_DEFAULT
	goto FrontierPractice_Menu

FrontierPractice_RemoveMenu:
	special Special_FrontierPracticeBufferSettings
	copyvar VAR_0x8008, VAR_RESULT
	goto_if_eq VAR_0x8008, 0, FrontierPractice_NoTeam
	dynmultipush FrontierPractice_Text_Slot1, 0
	goto_if_lt VAR_0x8008, 2, FrontierPractice_RemoveReady
	dynmultipush FrontierPractice_Text_Slot2, 1
	goto_if_lt VAR_0x8008, 3, FrontierPractice_RemoveReady
	dynmultipush FrontierPractice_Text_Slot3, 2
	goto_if_lt VAR_0x8008, 4, FrontierPractice_RemoveReady
	dynmultipush FrontierPractice_Text_Slot4, 3
	goto_if_lt VAR_0x8008, 5, FrontierPractice_RemoveReady
	dynmultipush FrontierPractice_Text_Slot5, 4
	goto_if_lt VAR_0x8008, 6, FrontierPractice_RemoveReady
	dynmultipush FrontierPractice_Text_Slot6, 5
FrontierPractice_RemoveReady:
	dynmultipush FrontierPractice_Text_Clear, 6
	dynmultipush FrontierPractice_Text_Exit, 7
	dynmultistack 0, 0, FALSE, 6, 0, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_ge VAR_RESULT, 7, FrontierPractice_Menu
	goto_if_eq VAR_RESULT, 6, FrontierPractice_Clear
	copyvar VAR_0x8004, VAR_RESULT
	special Special_FrontierPracticeRemove
	goto FrontierPractice_Menu
FrontierPractice_Clear:
	special Special_FrontierPracticeClear
	goto FrontierPractice_Menu

FrontierPractice_Team:
	special Special_FrontierPracticeBufferSettings
	copyvar VAR_0x8008, VAR_RESULT
	goto_if_eq VAR_0x8008, 0, FrontierPractice_NoTeam
	setvar VAR_0x8004, 0
	special Special_FrontierPracticeBufferTeam
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto_if_le VAR_0x8008, 3, FrontierPractice_Menu
	setvar VAR_0x8004, 1
	special Special_FrontierPracticeBufferTeam
	msgbox gStringVar4, MSGBOX_DEFAULT
	goto FrontierPractice_Menu

FrontierPractice_AI:
	multichoice 0, 0, MULTI_FRONTIER_AI_LEVEL, FALSE
	goto_if_ge VAR_RESULT, 4, FrontierPractice_Menu
	setvar VAR_0x8004, PRACTICE_OPTION_AI
	setvar VAR_0x8005, FRONTIER_AI_LEVEL_EASY
	goto_if_eq VAR_RESULT, 0, FrontierPractice_ApplyOption
	setvar VAR_0x8005, FRONTIER_AI_LEVEL_NORMAL
	goto_if_eq VAR_RESULT, 1, FrontierPractice_ApplyOption
	setvar VAR_0x8005, FRONTIER_AI_LEVEL_HARD
	goto_if_eq VAR_RESULT, 2, FrontierPractice_ApplyOption
	setvar VAR_0x8005, FRONTIER_AI_LEVEL_STRONGEST
	goto FrontierPractice_ApplyOption

FrontierPractice_Format:
	dynmultipush FrontierPractice_Text_Single, 0
	dynmultipush FrontierPractice_Text_Double, 1
	dynmultistack 0, 0, FALSE, 2, 0, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_ge VAR_RESULT, 2, FrontierPractice_Menu
	setvar VAR_0x8004, PRACTICE_OPTION_DOUBLE
	copyvar VAR_0x8005, VAR_RESULT
	goto FrontierPractice_ApplyOption

FrontierPractice_Level:
	dynmultipush FrontierPractice_Text_LevelOriginal, 0
	dynmultipush FrontierPractice_Text_Level50, 1
	dynmultistack 0, 0, FALSE, 2, 0, 0, DYN_MULTICHOICE_CB_NONE
	goto_if_ge VAR_RESULT, 2, FrontierPractice_Menu
	setvar VAR_0x8004, PRACTICE_OPTION_LEVEL50
	copyvar VAR_0x8005, VAR_RESULT
FrontierPractice_ApplyOption:
	special Special_FrontierPracticeSetOption
	goto FrontierPractice_Menu

FrontierPractice_Battle:
	msgbox FrontierPractice_Text_Confirm, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FrontierPractice_Menu
	closemessage
	special Special_FrontierPracticeStart
	goto_if_eq VAR_RESULT, PRACTICE_NO_TEAM, FrontierPractice_NoTeam
	goto_if_eq VAR_RESULT, PRACTICE_TEAM_CHANGED, FrontierPractice_Changed
	goto_if_eq VAR_RESULT, PRACTICE_NOT_ENOUGH_PLAYER, FrontierPractice_TooFew
	goto_if_eq VAR_RESULT, PRACTICE_NOT_ENOUGH_ENEMY, FrontierPractice_TooFew
	goto_if_ne VAR_RESULT, PRACTICE_OK, FrontierPractice_Invalid
	msgbox FrontierPractice_Text_Finished, MSGBOX_DEFAULT
	goto FrontierPractice_Menu

FrontierPractice_NoTeam:
	msgbox FrontierPractice_Text_NoTeam, MSGBOX_DEFAULT
	goto FrontierPractice_Menu
FrontierPractice_TooFew:
	msgbox FrontierPractice_Text_TooFew, MSGBOX_DEFAULT
	goto FrontierPractice_Menu
FrontierPractice_Full:
	msgbox FrontierPractice_Text_Full, MSGBOX_DEFAULT
	goto FrontierPractice_Menu
FrontierPractice_Changed:
	msgbox FrontierPractice_Text_Changed, MSGBOX_DEFAULT
	goto FrontierPractice_Menu
FrontierPractice_Invalid:
	msgbox FrontierPractice_Text_Invalid, MSGBOX_DEFAULT
	goto FrontierPractice_Menu

FrontierPractice_Text_Welcome:
	.string "{JPN}ボックスの ポケモンを かりて\nきみの れんしゅうあいてに なるよ！\pあいては 6ぴきまで えらべるよ。\nもとの ポケモンは かわらないよ。$"
FrontierPractice_Text_Settings:
	.string "{JPN}あいて {STR_VAR_1}ひき  AI {STR_VAR_2}\n{STR_VAR_3}$"
FrontierPractice_Text_Add:
	.string "{JPN}あいてを えらぶ$"
FrontierPractice_Text_Remove:
	.string "{JPN}あいてを はずす$"
FrontierPractice_Text_Team:
	.string "{JPN}あいてを かくにん$"
FrontierPractice_Text_AI:
	.string "{JPN}AIの つよさ$"
FrontierPractice_Text_Format:
	.string "{JPN}たいせん けいしき$"
FrontierPractice_Text_Level:
	.string "{JPN}レベルの せってい$"
FrontierPractice_Text_Battle:
	.string "{JPN}たいせんする$"
FrontierPractice_Text_Exit:
	.string "{JPN}もどる$"
FrontierPractice_Text_Slot1:
	.string "{JPN}1ばんを はずす$"
FrontierPractice_Text_Slot2:
	.string "{JPN}2ばんを はずす$"
FrontierPractice_Text_Slot3:
	.string "{JPN}3ばんを はずす$"
FrontierPractice_Text_Slot4:
	.string "{JPN}4ばんを はずす$"
FrontierPractice_Text_Slot5:
	.string "{JPN}5ばんを はずす$"
FrontierPractice_Text_Slot6:
	.string "{JPN}6ばんを はずす$"
FrontierPractice_Text_Clear:
	.string "{JPN}ぜんぶ はずす$"
FrontierPractice_Text_Single:
	.string "{JPN}シングル$"
FrontierPractice_Text_Double:
	.string "{JPN}ダブル$"
FrontierPractice_Text_LevelOriginal:
	.string "{JPN}そのまま$"
FrontierPractice_Text_Level50:
	.string "{JPN}ぜんいん Lv50$"
FrontierPractice_Text_Confirm:
	.string "{JPN}この せっていで たいせんする？\nしょうはいで おかねは かわらないよ。\pてもちの じょうたいは\nたいせんまえに もどすからね！$"
FrontierPractice_Text_Added:
	.string "{JPN}れんしゅうあいてに くわえたよ！$"
FrontierPractice_Text_Finished:
	.string "{JPN}おつかれさま！\nまた くみあわせを ためしてみよう！$"
FrontierPractice_Text_NoTeam:
	.string "{JPN}まずは ボックスから\nれんしゅうあいてを えらんでね。$"
FrontierPractice_Text_TooFew:
	.string "{JPN}シングルは おたがい 1ぴきいじょう\nダブルは 2ひきいじょう ひつようだよ。$"
FrontierPractice_Text_Full:
	.string "{JPN}あいては 6ぴきまでだよ。\nいれかえるなら さきに はずしてね。$"
FrontierPractice_Text_Changed:
	.string "{JPN}えらんだ ボックスの なかみが\nかわったみたい。 えらびなおしてね。$"
FrontierPractice_Text_Invalid:
	.string "{JPN}じゅんびが できなかったよ。\nポケモンを かくにんしてみてね。$"
