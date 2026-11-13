@ Cube Tester: public test room for GBA-GC Extension (Dolphin + Integrated GBA).
@ Every entry is read-only or display-only; nothing here changes the save.
FrontierCubeTester_MapScripts::
	.byte 0

FrontierCubeTester_EventScript_Terminal::
	lock
	faceplayer
	msgbox FrontierCubeTester_Text_Welcome, MSGBOX_DEFAULT
FrontierCubeTester_EventScript_Menu::
	message FrontierCubeTester_Text_Choose
	waitmessage
	dynmultipush FrontierCubeTester_Text_LinkCheck, 0
	dynmultipush FrontierCubeTester_Text_PartyViewer, 1
	dynmultipush FrontierCubeTester_Text_Single, 2
	dynmultipush FrontierCubeTester_Text_Double, 3
	dynmultipush FrontierCubeTester_Text_Triple, 4
	dynmultipush FrontierCubeTester_Text_Rotation, 5
	dynmultipush FrontierCubeTester_Text_Animation, 6
	dynmultipush FrontierCubeTester_Text_ReturnHub, 7
	dynmultipush FrontierCubeTester_Text_Quit, 8
	dynmultistack 0, 0, FALSE, 6, 0, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	switch VAR_RESULT
	case 0, FrontierCubeTester_EventScript_LinkCheck
	case 1, FrontierCubeTester_EventScript_PartyViewer
	case 2, FrontierCubeTester_EventScript_Single
	case 3, FrontierCubeTester_EventScript_Double
	case 4, FrontierCubeTester_EventScript_Triple
	case 5, FrontierCubeTester_EventScript_Rotation
	case 6, FrontierCubeTester_EventScript_Animation
	case 7, FrontierCubeTester_EventScript_ReturnHub
	goto FrontierCubeTester_EventScript_Quit

FrontierCubeTester_EventScript_LinkCheck::
	msgbox FrontierCubeTester_Text_BeforeLink, MSGBOX_DEFAULT
	closemessage
	special GcLinkCheck_Start
	waitstate
	goto FrontierCubeTester_EventScript_Again

FrontierCubeTester_EventScript_PartyViewer::
	msgbox FrontierCubeTester_Text_BeforeLink, MSGBOX_DEFAULT
	closemessage
	special GcCompanion_Start
	waitstate
	goto FrontierCubeTester_EventScript_Again

@ The battles use the GC Battle move effect fixtures (request V8): both
@ parties are temporary test Pokemon and the result is display-only.
FrontierCubeTester_EventScript_Single::
	setvar VAR_0x8006, 0x4001
	goto FrontierCubeTester_EventScript_Battle

FrontierCubeTester_EventScript_Double::
	setvar VAR_0x8006, 0x4002
	goto FrontierCubeTester_EventScript_Battle

FrontierCubeTester_EventScript_Triple::
	setvar VAR_0x8006, 0x4003
	goto FrontierCubeTester_EventScript_Battle

FrontierCubeTester_EventScript_Rotation::
	setvar VAR_0x8006, 0x4004
	goto FrontierCubeTester_EventScript_Battle

FrontierCubeTester_EventScript_Battle::
	msgbox FrontierCubeTester_Text_BeforeBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, FrontierCubeTester_EventScript_Menu
	closemessage
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, TRAINER_CALVIN_1
	special FrontierGcBattle_Start
	waitstate
	goto FrontierCubeTester_EventScript_Again

FrontierCubeTester_EventScript_Animation::
	msgbox FrontierCubeTester_Text_AnimationNotReady, MSGBOX_DEFAULT
	goto FrontierCubeTester_EventScript_Menu

FrontierCubeTester_EventScript_ReturnHub::
	msgbox FrontierCubeTester_Text_ComeAgain, MSGBOX_DEFAULT
	closemessage
	release
	warp MAP_ULTRA_FRONTIE_HUB, 23, 2
	waitstate
	end

FrontierCubeTester_EventScript_Again::
	msgbox FrontierCubeTester_Text_Again, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FrontierCubeTester_EventScript_Menu
FrontierCubeTester_EventScript_Quit::
	msgbox FrontierCubeTester_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

FrontierCubeTester_EventScript_Guide::
	lock
	faceplayer
	msgbox FrontierCubeTester_Text_Guide, MSGBOX_DEFAULT
	release
	end

FrontierCubeTester_Text_Welcome:
	.string "{JPN}キューブテスターへ ようこそ！\n"
	.string "ゲームキューブと つないで\l"
	.string "テストバトルが できます。$"

FrontierCubeTester_Text_Choose:
	.string "{JPN}どれを ためしますか？$"

FrontierCubeTester_Text_LinkCheck:
	.string "{JPN}せつぞく かくにん$"

FrontierCubeTester_Text_PartyViewer:
	.string "{JPN}パーティビューア$"

FrontierCubeTester_Text_Single:
	.string "{JPN}シングルバトル$"

FrontierCubeTester_Text_Double:
	.string "{JPN}ダブルバトル$"

FrontierCubeTester_Text_Triple:
	.string "{JPN}トリプルバトル$"

FrontierCubeTester_Text_Rotation:
	.string "{JPN}ローテーションバトル$"

FrontierCubeTester_Text_Animation:
	.string "{JPN}わざアニメ (じゅんびちゅう)$"

FrontierCubeTester_Text_ReturnHub:
	.string "{JPN}ハブへ もどる$"

FrontierCubeTester_Text_Quit:
	.string "{JPN}やめる$"

FrontierCubeTester_Text_BeforeLink:
	.string "{JPN}ゲームキューブで {ENG}companion.dol{JPN}を\n"
	.string "きどうしてから はじめてください。\p"
	.string "つながらない ときは\n"
	.string "{ENG}B{JPN}ボタンで いつでも もどれます。$"

FrontierCubeTester_Text_BeforeBattle:
	.string "{JPN}テスト用の ポケモンで たたかいます。\n"
	.string "けっかは ひょうじだけで\l"
	.string "レポートには のこりません。\p"
	.string "ゲームキューブと つないで\n"
	.string "はじめますか？$"

FrontierCubeTester_Text_AnimationNotReady:
	.string "{JPN}わざアニメの テストは\n"
	.string "まだ じゅんびちゅうです。$"

FrontierCubeTester_Text_Again:
	.string "{JPN}ほかにも ためしますか？$"

FrontierCubeTester_Text_ComeAgain:
	.string "{JPN}またどうぞ。$"

FrontierCubeTester_Text_Guide:
	.string "{JPN}ここは パブリックアルファの\n"
	.string "テストルームだ。\p"
	.string "わざや とくせい もちものは\n"
	.string "まだ ぜんぶは うごかない。\p"
	.string "おかしな ところが あったら\n"
	.string "{ENG}GitHub Issues{JPN}で おしえてくれ！$"
