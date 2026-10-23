Route109_SeashoreHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route109_SeashoreHouse_OnTransition
	.byte 0

Route109_SeashoreHouse_OnTransition:
	setflag FLAG_LANDMARK_SEASHORE_HOUSE
	end

Route109_SeashoreHouse_EventScript_Owner::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_6_SODA_POP, Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop
	goto_if_set FLAG_DEFEATED_SEASHORE_HOUSE, Route109_SeashoreHouse_EventScript_DefeatedTrainers
	goto_if_set FLAG_TEMP_2, Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction
	msgbox Route109_SeashoreHouse_Text_SeashoreHouseIntro, MSGBOX_DEFAULT
	setflag FLAG_TEMP_2
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyGaveIntroduction::
	msgbox Route109_SeashoreHouse_Text_ShowMeSomeHotMatches, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_DefeatedTrainers::
	msgbox Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles, MSGBOX_DEFAULT
	giveitem ITEM_SODA_POP, 6
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_BagFull
	setflag FLAG_RECEIVED_6_SODA_POP
	release
	end

Route109_SeashoreHouse_EventScript_BagFull::
	msgbox Route109_SeashoreHouse_Text_BagFull, MSGBOX_DEFAULT
	release
	end

Route109_SeashoreHouse_EventScript_AlreadyReceivedSodaPop::
	showmoneybox 0, 0
	msgbox Route109_SeashoreHouse_Text_WantToBuySodaPop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route109_SeashoreHouse_EventScript_BuySodaPop
	msgbox Route109_SeashoreHouse_Text_ThatsTooBad, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_BuySodaPop::
	checkmoney 300
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughMoney
	checkitemspace ITEM_SODA_POP
	goto_if_eq VAR_RESULT, FALSE, Route109_SeashoreHouse_EventScript_NotEnoughSpace
	msgbox Route109_SeashoreHouse_Text_HereYouGo, MSGBOX_DEFAULT
	removemoney 300
	updatemoneybox
.if OW_SHOW_ITEM_DESCRIPTIONS != OW_ITEM_DESCRIPTIONS_OFF
	hidemoneybox
	giveitem ITEM_SODA_POP
.else
	giveitem ITEM_SODA_POP
	hidemoneybox
.endif
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughMoney::
	msgbox Route109_SeashoreHouse_Text_NotEnoughMoney, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_NotEnoughSpace::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	hidemoneybox
	release
	end

Route109_SeashoreHouse_EventScript_Dwayne::
	trainerbattle_single TRAINER_DWAYNE, Route109_SeashoreHouse_Text_DwayneIntro, Route109_SeashoreHouse_Text_DwayneDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_DwaynePostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Johanna::
	trainerbattle_single TRAINER_JOHANNA, Route109_SeashoreHouse_Text_JohannaIntro, Route109_SeashoreHouse_Text_JohannaDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_JohannaPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_Simon::
	trainerbattle_single TRAINER_SIMON, Route109_SeashoreHouse_Text_SimonIntro, Route109_SeashoreHouse_Text_SimonDefeated, Route109_SeashoreHouse_EventScript_CheckTrainersCompletion
	msgbox Route109_SeashoreHouse_Text_SimonPostBattle, MSGBOX_AUTOCLOSE
	end

Route109_SeashoreHouse_EventScript_CheckTrainersCompletion::
	goto_if_not_defeated TRAINER_DWAYNE, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_JOHANNA, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	goto_if_not_defeated TRAINER_SIMON, Route109_SeashoreHouse_EventScript_TrainersNotCompleted
	setflag FLAG_DEFEATED_SEASHORE_HOUSE
	release
	end

Route109_SeashoreHouse_EventScript_TrainersNotCompleted::
	release
	end

Route109_SeashoreHouse_Text_SeashoreHouseIntro:
	.string "わし うみのいえ の オヤジ!\nひとよんで うみサン!\pとにかく あつい ポケモンしょうぶを\nみることが だいすき なんだよねー!\pキミもねー あつーい ハートで\nここに いる トレーナーぜんいんに\lかてたら いいものを あげるよー$"

Route109_SeashoreHouse_Text_ShowMeSomeHotMatches:
	.string "あつーい しょうぶ みせてくれー!\pそのためだけに わし\nこの うみのいえを やってるんだねー!$"

Route109_SeashoreHouse_Text_TakeTheseSodaPopBottles:
	.string "きみ すごいねー!\nあつい ポケモンしょうぶ だったー\lわし だいまんぞく だよー!\pこれは あつい しょうぶを みせてくれた\nきみへの おれい だよー\pサイコソーダ はんダース もっていきなよー$"

Route109_SeashoreHouse_Text_BagFull:
	.string "と おもったけど バッグ いっぱいかー\nじゃあ あずかっておくよー$"

Route109_SeashoreHouse_Text_WantToBuySodaPop:
	.string "ポケモンも だいすき サイコソーダ\n"
	.string "いっぽん 300えん だよー\l"
	.string "かって いきなよー！$"

Route109_SeashoreHouse_Text_HereYouGo:
	.string "はいよー$"

Route109_SeashoreHouse_Text_NotEnoughMoney:
	.string "おかねが ないよー$"

Route109_SeashoreHouse_Text_ThatsTooBad:
	.string "それは ざんねん だねー$"

Route109_SeashoreHouse_Text_DwayneIntro:
	.string "この うみのいえで いちばん\n"
	.string "あつい ポケモンしょうぶが できるのは\l"
	.string "この オレさま だぜ！！$"

Route109_SeashoreHouse_Text_DwayneDefeated:
	.string "たしかに あつい しょうぶ だった\n"
	.string "まけて なっとく って かんじ だぜ！$"

Route109_SeashoreHouse_Text_DwaynePostBattle:
	.string "オレは カイナに くるたびに\n"
	.string "ここで あつい しょうぶと\l"
	.string "つめたい サイコソーダを たのしむんだ！$"

Route109_SeashoreHouse_Text_JohannaIntro:
	.string "たいくつな しょうぶ なんて ムダ！\p"
	.string "もえあがるような しょうぶ こそ\n"
	.string "トレーナーと ポケモンを きたえるのよ！$"

Route109_SeashoreHouse_Text_JohannaDefeated:
	.string "あつい じゃない！$"

Route109_SeashoreHouse_Text_JohannaPostBattle:
	.string "あーあ のどが かわいちゃった\n"
	.string "サイコソーダ のもうかな？$"

Route109_SeashoreHouse_Text_SimonIntro:
	.string "ぼくの ポケモンの すごいところ\n"
	.string "みせちゃうけど なかないでよ！$"

Route109_SeashoreHouse_Text_SimonDefeated:
	.string "……まけたけど なかないぞ$"

Route109_SeashoreHouse_Text_SimonPostBattle:
	.string "ポケモンが ぼくを のせて\n"
	.string "みずの うえを すすむ わざを つかえたら\l"
	.string "うきわも いらなくなるのに$"

