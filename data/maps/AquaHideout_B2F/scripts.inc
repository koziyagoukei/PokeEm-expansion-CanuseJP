AquaHideout_B2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AquaHideout_B2F_OnTransition
	.byte 0

AquaHideout_B2F_OnTransition:
	call_if_set FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE, AquaHideout_B2F_EventScript_PreventMattNoticing
	end

AquaHideout_B2F_EventScript_PreventMattNoticing::
	setvar VAR_TEMP_1, 1
	return

AquaHideout_B2F_EventScript_MattNoticePlayer::
	lockall
	setvar VAR_0x8008, LOCALID_AQUA_HIDEOUT_MATT
	playse SE_PIN
	applymovement VAR_0x8008, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement VAR_0x8008, Common_Movement_FacePlayer
	waitmovement 0
	setvar VAR_TEMP_1, 1
	releaseall
	end

AquaHideout_B2F_EventScript_Matt::
	trainerbattle_single TRAINER_MATT, AquaHideout_B2F_Text_MattIntro, AquaHideout_B2F_Text_MattDefeat, AquaHideout_B2F_EventScript_SubmarineEscape
	msgbox AquaHideout_B2F_Text_MattPostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_B2F_EventScript_SubmarineEscape::
	setvar VAR_0x8008, LOCALID_AQUA_HIDEOUT_MATT
	setvar VAR_0x8009, LOCALID_AQUA_HIDEOUT_SUBMARINE
	applymovement VAR_0x8008, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 20
	applymovement VAR_0x8008, Common_Movement_FacePlayer
	waitmovement 0
	msgbox AquaHideout_B2F_Text_OurBossGotThroughHisPreparations, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_0x8008, Common_Movement_WalkInPlaceFasterLeft
	applymovement VAR_0x8009, AquaHideout_B2F_Movement_SumbarineDepartLeft
	waitmovement 0
	removeobject VAR_0x8009
	delay 20
	applymovement VAR_0x8008, Common_Movement_FacePlayer
	waitmovement 0
	msgbox AquaHideout_B2F_Text_MattPostBattle, MSGBOX_DEFAULT
	setflag FLAG_TEAM_AQUA_ESCAPED_IN_SUBMARINE
	setflag FLAG_HIDE_LILYCOVE_CITY_AQUA_GRUNTS
	release
	end

AquaHideout_B2F_Movement_SumbarineDepartLeft:
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

@ Unused
AquaHideout_B2F_Movement_SumbarineDepartRight:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

AquaHideout_B2F_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_4, AquaHideout_B2F_Text_Grunt4Intro, AquaHideout_B2F_Text_Grunt4Defeat, AquaHideout_B2F_EventScript_Grunt4Defeated
	msgbox AquaHideout_B2F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_EventScript_Grunt4Defeated::
	msgbox AquaHideout_B2F_Text_Grunt4PostBattle, MSGBOX_DEFAULT
	release
	end

AquaHideout_B2F_EventScript_Grunt6::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_6, AquaHideout_B2F_Text_Grunt6Intro, AquaHideout_B2F_Text_Grunt6Defeat
	msgbox AquaHideout_B2F_Text_Grunt6PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_EventScript_Grunt8::
	trainerbattle_single TRAINER_GRUNT_AQUA_HIDEOUT_8, AquaHideout_B2F_Text_Grunt8Intro, AquaHideout_B2F_Text_Grunt8Defeat
	msgbox AquaHideout_B2F_Text_Grunt8PostBattle, MSGBOX_AUTOCLOSE
	end

AquaHideout_B2F_Text_MattIntro:
	.string "{JPN}ウヒョヒョ……\pもう ここまで きたのか!\nおもって いたより やるな!\pだが ここまでだ!\nおれは これまでの したっぱども とは\lひとあじ ちがう!\pじかんかせぎ なんか しねえで\nおまえを コテンパンに してやるぜ!$"

AquaHideout_B2F_Text_MattDefeat:
	.string "{JPN}ウヒョヒョ……\nまけちまったか……$"

AquaHideout_B2F_Text_OurBossGotThroughHisPreparations:
	.string "{JPN}ウヒョヒョ……!\pおれが おまえと\nあそんで やっている あいだに\lリーダーの じゅんびが ととのったぜ!$"

AquaHideout_B2F_Text_MattPostBattle:
	.string "{JPN}ウヒョヒョ……!\pリーダーは どこかの かいていにある\nどうくつへ しゅっぱつ したぜ!\pおいかける つもりなら ミナモの さきに\nひろがる でっかい うみを\lさがしまわって みるが いいぜ!\pそれで みつかれば いいけどな……\nウヒョヒョヒョ!$"

AquaHideout_B2F_Text_Grunt4Intro:
	.string "{JPN}わっはっは まちくたびれたぞ!\nおいらとも しょうぶ してもらおうか!$"

AquaHideout_B2F_Text_Grunt4Defeat:
	.string "{JPN}まちくたびれて\nまけて やぶれて……$"

AquaHideout_B2F_Text_Grunt4PostBattle:
	.string "{JPN}リーダーぁ……\nこんなもんで どうでしょー!$"

AquaHideout_B2F_Text_Grunt6Intro:
	.string "{JPN}アジト じまんの ワープパネル!\pいま じぶんが どこに いるのか\nわかって ないでしょ!\pそうやって つかれはてた あいてを\nたおすのが さくせん なのよ!$"

AquaHideout_B2F_Text_Grunt6Defeat:
	.string "{JPN}なによっ!\nちっとも つかれてない じゃない!$"

AquaHideout_B2F_Text_Grunt6PostBattle:
	.string "{JPN}そういえば…… マスターボールを\nどこに おいたのか わすれちゃったわ\pあれを まもっていないと\nリーダーに おこられるのよ……$"

AquaHideout_B2F_Text_Grunt8Intro:
	.string "{JPN}オレが アクアだんに はいって\nまっさきに おぼえ させられたのが\lこの ワープパネルの はいれつだ!$"

AquaHideout_B2F_Text_Grunt8Defeat:
	.string "{JPN}ワープパネルの ことで\nあたまが いっぱい だったから……$"

AquaHideout_B2F_Text_Grunt8PostBattle:
	.string "{JPN}これからは ポケモンしょうぶの\nコツも おぼえて おかないと なあ……$"
