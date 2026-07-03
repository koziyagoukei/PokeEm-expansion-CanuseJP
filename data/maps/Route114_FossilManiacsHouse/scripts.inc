Route114_FossilManiacsHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_FossilManiacsHouse_OnTransition
	.byte 0

Route114_FossilManiacsHouse_OnTransition:
	setflag FLAG_LANDMARK_FOSSIL_MANIACS_HOUSE
	end

Route114_FossilManiacsHouse_EventScript_FossilManiacsBrother::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_DIG, Route114_FossilManiacsHouse_EventScript_ReceivedDig
	msgbox Route114_FossilManiacsHouse_Text_HaveThisToDigLikeMyBrother, MSGBOX_DEFAULT
	giveitem ITEM_TM_DIG
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_DIG
	release
	end

Route114_FossilManiacsHouse_EventScript_ReceivedDig::
	msgbox Route114_FossilManiacsHouse_Text_DigReturnsYouToEntrance, MSGBOX_DEFAULT
	release
	end

Route114_FossilManiacsHouse_EventScript_RockDisplay::
	msgbox Route114_FossilManiacsHouse_Text_RocksFillDisplayCase, MSGBOX_SIGN
	end

Route114_FossilManiacsHouse_EventScript_Bookshelf::
	msgbox Route114_FossilManiacsHouse_Text_CrammedWithBooks, MSGBOX_SIGN
	end

Route114_FossilManiacsHouse_Text_HaveThisToDigLikeMyBrother:
	.string "{JPN}ぼくの にいちゃん かせきマニア……\nかせき だいすき ナイスガイ さ……\pちなみに あなを ほるのも だいすきで……\nこの あなも じぶんで ほったんだ……\pこれを あげるから……\nきみも にいちゃん みたいに\lあなを ほってみなよ……$"

Route114_FossilManiacsHouse_Text_DigReturnsYouToEntrance:
	.string "{JPN}ちなみに ポケモンが あなをほると\nどうくつの いりぐち まで\lいっしゅんで もどれるんだ……$"

Route114_FossilManiacsHouse_Text_RocksFillDisplayCase:
	.string "{JPN}ケースのなかに かわった かたちの いしが\nいくつも かざってある……$"

Route114_FossilManiacsHouse_Text_CrammedWithBooks:
	.string "{JPN}『ちそうが できるまで』\n『あめ ふって じめん かたまる』\l『いしと つちと がんせきと』\pたくさんの ほんが ならんでる!$"
