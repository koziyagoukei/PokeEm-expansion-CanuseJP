Route119_WeatherInstitute_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route119_WeatherInstitute_1F_OnTransition
	.byte 0

Route119_WeatherInstitute_1F_OnTransition:
	call_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_1F_EventScript_SetLittleBoyPos
	end

Route119_WeatherInstitute_1F_EventScript_SetLittleBoyPos::
	setobjectxyperm LOCALID_WEATHER_INSTITUTE_LITTLE_BOY, 0, 5
	setobjectmovementtype LOCALID_WEATHER_INSTITUTE_LITTLE_BOY, MOVEMENT_TYPE_FACE_RIGHT
	return

Route119_WeatherInstitute_1F_EventScript_LittleBoy::
	lock
	faceplayer
	special GetPlayerBigGuyGirlString
	goto_if_eq VAR_WEATHER_INSTITUTE_STATE, 0, Route119_WeatherInstitute_1F_EventScript_LittleBoyTeamAquaHere
	msgbox Route119_WeatherInstitute_1F_Text_WowYoureStrong, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_LittleBoyTeamAquaHere::
	msgbox Route119_WeatherInstitute_1F_Text_EveryoneWentUpstairs, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_InstituteWorker1::
	lock
	faceplayer
	goto_if_unset FLAG_SYS_GAME_CLEAR, Route119_WeatherInstitute_1F_EventScript_StudyingRain
	setvar VAR_0x8004, 0
	call_if_set FLAG_DEFEATED_KYOGRE, Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated
	call_if_set FLAG_DEFEATED_GROUDON, Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated
	goto_if_eq VAR_0x8004, 2, Route119_WeatherInstitute_1F_EventScript_StudyingRain  @ Both defeated
	msgbox Route119_WeatherInstitute_1F_Text_NoticingAbnormalWeather, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_LegendaryDefeated::
	addvar VAR_0x8004, 1
	return

Route119_WeatherInstitute_1F_EventScript_StudyingRain::
	msgbox Route119_WeatherInstitute_1F_Text_ProfStudyingRain, MSGBOX_DEFAULT
	release
	end

Route119_WeatherInstitute_1F_EventScript_InstituteWorker2::
	msgbox Route119_WeatherInstitute_1F_Text_WhatWereAquasUpTo, MSGBOX_NPC
	end

Route119_WeatherInstitute_1F_EventScript_Bed::
	lockall
	msgbox Route119_WeatherInstitute_1F_Text_TakeRestInBed, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	releaseall
	end

Route119_WeatherInstitute_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_1, Route119_WeatherInstitute_1F_Text_Grunt1Intro, Route119_WeatherInstitute_1F_Text_Grunt1Defeat
	msgbox Route119_WeatherInstitute_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_1F_EventScript_Grunt4::
	trainerbattle_single TRAINER_GRUNT_WEATHER_INST_4, Route119_WeatherInstitute_1F_Text_Grunt4Intro, Route119_WeatherInstitute_1F_Text_Grunt4Defeat
	msgbox Route119_WeatherInstitute_1F_Text_Grunt4PostBattle, MSGBOX_AUTOCLOSE
	end

Route119_WeatherInstitute_1F_Text_Grunt1Intro:
	.string "リーダーが ここでの けんきゅうに\nきょうみを もったらしくて\lおれらを よこしたんだ! ジャマするな!"
	.string ""
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_Grunt1Defeat:
	.string "くそー\nこどもに まけた……"
	.string "$"

Route119_WeatherInstitute_1F_Text_Grunt1PostBattle:
	.string "リーダーは なんでも しっている……\pしかし なにを かんがえて いるのか\nしたっぱの おれらには さっぱりだ"
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_Grunt4Intro:
	.string "あら?\nどうして ここに こどもが いるのかしら?"
	.string "$"

Route119_WeatherInstitute_1F_Text_Grunt4Defeat:
	.string "あらー\nまけちゃったー!?"
	.string "$"

Route119_WeatherInstitute_1F_Text_Grunt4PostBattle:
	.string "あらら……\nこどもに まけたら おこられちゃう……\pベッドで ねてようかしら?"
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_EveryoneWentUpstairs:
	.string "ぼくが ベッドで ねてる あいだに\nみんな 2かいに いっちゃったよ"
	.string "$"

Route119_WeatherInstitute_1F_Text_WowYoureStrong:
	.string "{STR_VAR_1} つよいんだね!\pぼくも {STR_VAR_1} みたいな\nポケモントレーナーに なりたいや!"
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_ProfStudyingRain:
	.string "はかせは あめが だいすき なのです\pですが あめが ふりつづくと\nこまってしまう ひとも いるわけです\pそこで はかせは\nこの あめを なにかに つかえないか\lけんきゅう なさっているのです"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_NoticingAbnormalWeather:
	.string "この けんきゅうじょの 2かいでは\nホウエンちほうの てんきの\lうつりかわりを しらべています\pさいきん また あちこちで\nいちじてきな ひでりや おおあめが\lかんそく されている ようですが……"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_WhatWereAquasUpTo:
	.string "やあ!\nあなたの おかげで たすかりましたよ!\lそれにしても……\pアクアだんの れんちゅうは\nなにを するつもり だったのでしょう?"
	.string ""
	.string ""
	.string "$"

Route119_WeatherInstitute_1F_Text_TakeRestInBed:
	.string "ベッドが ある……\nやすんでいこう"
	.string "$"
