SixIsland_RuinValley_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SixIsland_RuinValley_OnLoad
	.byte 0

SixIsland_RuinValley_OnLoad::
	call_if_set FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE, SixIsland_RuinValley_EventScript_OpenDottedHoleDoor
	end

SixIsland_RuinValley_EventScript_OpenDottedHoleDoor::
	setmetatile 24, 24, METATILE_SeviiIslands67_DottedHoleDoor_Open, 0
	return

SixIsland_RuinValley_EventScript_Scientist::
	lock
	msgbox SixIsland_RuinValley_Text_CantFigureOutHowToGetInside
	applymovement LOCALID_RUIN_VALLEY_SCIENTIST, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SixIsland_RuinValley_Text_IFoundThisPlace
	applymovement LOCALID_RUIN_VALLEY_SCIENTIST, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SixIsland_RuinValley_EventScript_DottedHoleDoor::
	lockall
	goto_if_set FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE, SixIsland_RuinValley_EventScript_DottedHoleDoorOpen
	msgbox SixIsland_RuinValley_Text_CheckDoorMoreThoroughly, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SixIsland_RuinValley_EventScript_IgnoreDottedHoleDoor
	msgbox SixIsland_RuinValley_Text_SeveralDotsOnTheDoor
	braillemsgbox Braille_Text_Cut
	releaseall
	end

SixIsland_RuinValley_EventScript_DottedHoleDoorOpen::
	msgbox SixIsland_RuinValley_Text_DoorIsOpen
	releaseall
	end

SixIsland_RuinValley_EventScript_IgnoreDottedHoleDoor::
	msgbox SixIsland_RuinValley_Text_LeftDoorAlone
	releaseall
	end

SixIsland_RuinValley_Text_CantFigureOutHowToGetInside::
	.string "{JPN}いったい どうやって\n"
	.string "はいるんだ ここは…?$"

SixIsland_RuinValley_Text_IFoundThisPlace::
	.string "{JPN}ここは ぼくが\n"
	.string "みつけた ばしょだぞ!\l"
	.string "ものほしそうに みんなよ$"

SixIsland_RuinValley_Text_CheckDoorMoreThoroughly::
	.string "{JPN}とびらは びくとも しない!\n"
	.string "もっと よく しらべて みますか?$"

SixIsland_RuinValley_Text_LeftDoorAlone::
	.string "{JPN}{PLAYER}は とびらを\n"
	.string "そのままに しておいた$"

SixIsland_RuinValley_Text_SeveralDotsOnTheDoor::
	.string "{JPN}よくみると\n"
	.string "とびらに いくつかの てんが…?$"

SixIsland_RuinValley_Text_DoorIsOpen::
	.string "{JPN}とびらが ひらいている!$"

SixIsland_RuinValley_Text_StanlyIntro::
	.string "{JPN}この せかい には\n"
	.string "ひみつが いっぱい ありそうだ!$"

SixIsland_RuinValley_Text_StanlyDefeat::
	.string "{JPN}ふむ…$"

SixIsland_RuinValley_Text_StanlyPostBattle::
	.string "{JPN}きみも いちにち いっかいは\n"
	.string "このよの ひみつに ついて\l"
	.string "かんがえて みたまえ$"

SixIsland_RuinValley_Text_FosterIntro::
	.string "{JPN}きみは なにしに ここへ?$"

SixIsland_RuinValley_Text_FosterDefeat::
	.string "{JPN}ひさしぶりの しょうぶで\n"
	.string "カンが くるっとる$"

SixIsland_RuinValley_Text_FosterPostBattle::
	.string "{JPN}ナナシマの いせきは\n"
	.string "いくつか あるが\l"
	.string "かなり むかしから\l"
	.string "ある ものが おおい$"

SixIsland_RuinValley_Text_LarryIntro::
	.string "{JPN}この しまには\n"
	.string "ふしぎな いしが\l"
	.string "あると いわれて いる!\p"
	.string "あの いせきが あやしいのだが…$"

SixIsland_RuinValley_Text_LarryDefeat::
	.string "{JPN}おお これはこれは$"

SixIsland_RuinValley_Text_LarryPostBattle::
	.string "{JPN}あすこに ある いせき…\n"
	.string "どうやって はいったら いいのか\l"
	.string "まだ わかって いないんじゃ$"

SixIsland_RuinValley_Text_DarylIntro::
	.string "{JPN}たかい ところで\n"
	.string "きみと しょうぶ…!$"

SixIsland_RuinValley_Text_DarylDefeat::
	.string "{JPN}きみに あっさり\n"
	.string "まける かなしみ…!$"

SixIsland_RuinValley_Text_DarylPostBattle::
	.string "{JPN}たかい ところに\n"
	.string "のぼる よろこび…!$"

SixIsland_RuinValley_Text_HectorIntro::
	.string "{JPN}このへんなら オレが\n"
	.string "くわしいぜ!$"

SixIsland_RuinValley_Text_HectorDefeat::
	.string "{JPN}そんなに がんばらなくても\n"
	.string "おしえてやる って!$"

SixIsland_RuinValley_Text_HectorPostBattle::
	.string "{JPN}あの いせきの とびらには\n"
	.string "ふしぎな もようが\l"
	.string "きざまれて いるんだぜ\p"
	.string "…いみは わかんない けど$"
