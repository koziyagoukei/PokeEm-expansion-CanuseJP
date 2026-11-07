MtMoon_B2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_B2F_OnTransition
	.byte 0

MtMoon_B2F_OnTransition::
	call_if_unset FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_ShowFossils
	end

MtMoon_B2F_EventScript_ShowFossils::
	clearflag FLAG_HIDE_DOME_FOSSIL
	clearflag FLAG_HIDE_HELIX_FOSSIL
	return

MtMoon_B2F_EventScript_MiguelTrigger::
	lockall
	applymovement LOCALID_MIGUEL, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call MtMoon_B2F_EventScript_BattleMiguel
	releaseall
	end

MtMoon_B2F_EventScript_Miguel::
	lock
	faceplayer
	goto_if_set FLAG_GOT_FOSSIL_FROM_MT_MOON, MtMoon_B2F_EventScript_MiguelFossilPicked
	goto_if_defeated TRAINER_SUPER_NERD_MIGUEL, MtMoon_B2F_EventScript_MiguelGoPickFossil
	call MtMoon_B2F_EventScript_BattleMiguel
	release
	end

MtMoon_B2F_EventScript_BattleMiguel::
	textcolor NPC_TEXT_COLOR_MALE
	message MtMoon_B2F_Text_MiguelIntro
	waitmessage
	playbgm MUS_RG_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_MIGUEL
	trainerbattle_no_intro TRAINER_SUPER_NERD_MIGUEL, MtMoon_B2F_Text_MiguelDefeat
	setvar VAR_MAP_SCENE_MT_MOON_B2F, 1
	msgbox MtMoon_B2F_Text_WellEachTakeAFossil
	return

MtMoon_B2F_EventScript_MiguelFossilPicked::
	msgbox MtMoon_B2F_Text_LabOnCinnabarRegeneratesFossils
	release
	end

MtMoon_B2F_EventScript_MiguelGoPickFossil::
	msgbox MtMoon_B2F_Text_WellEachTakeAFossil
	release
	end

MtMoon_B2F_EventScript_DomeFossil::
	lock
	faceplayer
	msgbox MtMoon_B2F_Text_YouWantDomeFossil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoon_B2F_EventScript_DontTakeFossil
	removeobject LOCALID_DOME_FOSSIL
	giveitem_msg MtMoon_B2F_Text_ObtainedDomeFossil, ITEM_DOME_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	closemessage
	delay 10
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToHelixFossil
	waitmovement 0
	copyobjectxytoperm LOCALID_MIGUEL
	textcolor NPC_TEXT_COLOR_MALE
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message MtMoon_B2F_Text_ThenThisFossilIsMine
	waitmessage
	waitfanfare
	removeobject LOCALID_HELIX_FOSSIL
	setflag FLAG_GOT_DOME_FOSSIL
	setflag FLAG_GOT_FOSSIL_FROM_MT_MOON
	release
	end

MtMoon_B2F_Movement_MiguelToHelixFossil::
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_B2F_EventScript_HelixFossil::
	lock
	faceplayer
	msgbox MtMoon_B2F_Text_YouWantHelixFossil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MtMoon_B2F_EventScript_DontTakeFossil
	removeobject LOCALID_HELIX_FOSSIL
	giveitem_msg MtMoon_B2F_Text_ObtainedHelixFossil, ITEM_HELIX_FOSSIL, 1, MUS_RG_OBTAIN_KEY_ITEM
	closemessage
	delay 10
	applymovement LOCALID_MIGUEL, MtMoon_B2F_Movement_MiguelToDomeFossil
	waitmovement 0
	copyobjectxytoperm LOCALID_MIGUEL
	textcolor NPC_TEXT_COLOR_MALE
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message MtMoon_B2F_Text_ThenThisFossilIsMine
	waitmessage
	waitfanfare
	removeobject LOCALID_DOME_FOSSIL
	setflag FLAG_GOT_HELIX_FOSSIL
	setflag FLAG_GOT_FOSSIL_FROM_MT_MOON
	release
	end

MtMoon_B2F_EventScript_DontTakeFossil::
	release
	end

MtMoon_B2F_Movement_MiguelToDomeFossil::
	walk_up
	walk_up
	walk_up
	step_end

MtMoon_B2F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT, MtMoon_B2F_Text_Grunt1Intro, MtMoon_B2F_Text_Grunt1Defeat
	msgbox MtMoon_B2F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_2, MtMoon_B2F_Text_Grunt2Intro, MtMoon_B2F_Text_Grunt2Defeat
	msgbox MtMoon_B2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_3, MtMoon_B2F_Text_Grunt3Intro, MtMoon_B2F_Text_Grunt3Defeat
	msgbox MtMoon_B2F_Text_Grunt3PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_EventScript_Grunt4::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_4, MtMoon_B2F_Text_Grunt4Intro, MtMoon_B2F_Text_Grunt4Defeat
	msgbox MtMoon_B2F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_B2F_Text_MiguelIntro::
	.string "{JPN}こら まてよ!\p"
	.string "この カセキは\n"
	.string "ぼくが みつけたんだ\l"
	.string "ふたつ とも ぼくのだ!$"

MtMoon_B2F_Text_MiguelDefeat::
	.string "{JPN}わかった!\n"
	.string "おまえにも わけて やるよ$"

MtMoon_B2F_Text_WellEachTakeAFossil::
	.string "{JPN}ぼくと おまえで\n"
	.string "カセキを 1こ ずつ だ!\l"
	.string "ひとり じめは ダメだぞ!$"

MtMoon_B2F_Text_ThenThisFossilIsMine::
	.string "{JPN}それじゃ\n"
	.string "これは ぼくの ものだ!$"

MtMoon_B2F_Text_LabOnCinnabarRegeneratesFossils::
	.string "{JPN}ここから すごく とおいけど\n"
	.string "グレン タウンに\l"
	.string "ポケモン けんきゅうじょが ある\p"
	.string "カセキを よみがえらせる\n"
	.string "けんきゅうも してる らしいぜ$"

MtMoon_B2F_Text_Grunt1Intro::
	.string "{JPN}カセキは\n"
	.string "ロケットだんが みつけるのだ!\l"
	.string "ふっかつ させれば\l"
	.string "いい かねもうけに なる!$"

MtMoon_B2F_Text_Grunt1Defeat::
	.string "{JPN}うー おこったぞ!$"

MtMoon_B2F_Text_Grunt1PostBattle::
	.string "{JPN}おれを おこらせた\n"
	.string "おまえは ロケットだんの\l"
	.string "ブラック リストに のるぜ$"

MtMoon_B2F_Text_Grunt2Intro::
	.string "{JPN}ポケモン マフィア\n"
	.string "ロケットだんは\l"
	.string "こわくて つよい のだ!$"

MtMoon_B2F_Text_Grunt2Defeat::
	.string "{JPN}しくじったか!$"

MtMoon_B2F_Text_Grunt2PostBattle::
	.string "{JPN}くそ!\n"
	.string "なかまが だまっちゃ いねえぞ$"

MtMoon_B2F_Text_Grunt3Intro::
	.string "{JPN}おれたちは\n"
	.string "だいじな しごとを してるんだ!\l"
	.string "こどもは おうちへ かえりな$"

MtMoon_B2F_Text_Grunt3Defeat::
	.string "{JPN}なかなか やるな$"

MtMoon_B2F_Text_Grunt3PostBattle::
	.string "{JPN}カセキを みつけたら\n"
	.string "ばしょだけ おしえて\l"
	.string "さっさと かえるんだな!$"

MtMoon_B2F_Text_Grunt4Intro::
	.string "{JPN}おうっ!\n"
	.string "おとなの せかいに\l"
	.string "くびを つっこむと あぶないぜ!$"

MtMoon_B2F_Text_Grunt4Defeat::
	.string "{JPN}あたま くるぜ$"

MtMoon_B2F_Text_Grunt4PostBattle::
	.string "{JPN}にんげんが うまれる まえから\n"
	.string "この あたりには\l"
	.string "ポケモンが すんでた らしいぜ$"

MtMoon_B2F_Text_YouWantDomeFossil::
	.string "{JPN}こうらの カセキに する?$"

MtMoon_B2F_Text_YouWantHelixFossil::
	.string "{JPN}かいの カセキに する?$"

MtMoon_B2F_Text_ObtainedHelixFossil::
	.string "{JPN}かいのカセキを てにいれた!$"

MtMoon_B2F_Text_ObtainedDomeFossil::
	.string "{JPN}こうらのカセキを てにいれた!$"

