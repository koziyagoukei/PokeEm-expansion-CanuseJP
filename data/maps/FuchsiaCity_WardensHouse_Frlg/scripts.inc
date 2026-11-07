FuchsiaCity_WardensHouse_Frlg_MapScripts::
	.byte 0

FuchsiaCity_WardensHouse_EventScript_Warden::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM04, FuchsiaCity_WardensHouse_EventScript_ExplainStrength
	goto_if_set FLAG_HIDE_SAFARI_ZONE_WEST_GOLD_TEETH, FuchsiaCity_WardensHouse_EventScript_GiveGoldTeeth
	msgbox FuchsiaCity_WardensHouse_Text_HifFuffHefifoo, MSGBOX_YESNO
	call_if_eq VAR_RESULT, YES, FuchsiaCity_WardensHouse_EventScript_WardenYes
	call_if_eq VAR_RESULT, NO, FuchsiaCity_WardensHouse_EventScript_WardenNo
	release
	end

FuchsiaCity_WardensHouse_EventScript_GiveGoldTeeth::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_TMHM
	message FuchsiaCity_WardensHouse_Text_GaveGoldTeethToWarden
	waitmessage
	waitfanfare
	msgbox FuchsiaCity_WardensHouse_Text_WardenPoppedInHisTeeth
	call EventScript_RestorePrevTextColor
	checkplayergender
	call_if_eq VAR_RESULT, MALE, FuchsiaCity_WardensHouse_EventScript_WardenThanksMale
	call_if_eq VAR_RESULT, FEMALE, FuchsiaCity_WardensHouse_EventScript_WardenThanksFemale
	giveitem_msg FuchsiaCity_WardensHouse_Text_ReceivedHM04FromWarden, ITEM_HM04
	setflag FLAG_GOT_HM04
	removeitem ITEM_GOLD_TEETH
	release
	end

FuchsiaCity_WardensHouse_EventScript_WardenThanksMale::
	msgbox FuchsiaCity_WardensHouse_Text_ThanksSonGiveYouSomething
	return

FuchsiaCity_WardensHouse_EventScript_WardenThanksFemale::
	msgbox FuchsiaCity_WardensHouse_Text_ThanksLassieGiveYouSomething
	return

FuchsiaCity_WardensHouse_EventScript_WardenYes::
	msgbox FuchsiaCity_WardensHouse_Text_AhHowheeHoHoo
	return

FuchsiaCity_WardensHouse_EventScript_WardenNo::
	msgbox FuchsiaCity_WardensHouse_Text_HeOhayHeHaHoo
	return

FuchsiaCity_WardensHouse_EventScript_ExplainStrength::
	msgbox FuchsiaCity_WardensHouse_Text_ExplainStrength
	release
	end

FuchsiaCity_WardensHouse_EventScript_DisplaySign1::
	msgbox FuchsiaCity_WardensHouse_Text_MonPhotosFossilsOnDisplay, MSGBOX_SIGN
	end

FuchsiaCity_WardensHouse_EventScript_DisplaySign2::
	msgbox FuchsiaCity_WardensHouse_Text_OldMonMerchandiseOnDisplay, MSGBOX_SIGN
	end

FuchsiaCity_WardensHouse_Text_HifFuffHefifoo::
	.string "{JPN}えんちょう『ふぁいへんひゃ\n"
	.string "ふへほ はんひ\p"
	.string "ひへは ほはへへ ひほうはッ!\n"
	.string "はッ はふへへ ふへ!$"

FuchsiaCity_WardensHouse_Text_AhHowheeHoHoo::
	.string "{JPN}よほひふ はほひまふは!\n"
	.string "はふは はふぁりほーんひゃ!$"

FuchsiaCity_WardensHouse_Text_HeOhayHeHaHoo::
	.string "{JPN}はひ?\n"
	.string "ほは ほうは…\l"
	.string "ひひ ほはへには はのはんはッ!$"

FuchsiaCity_WardensHouse_Text_GaveGoldTeethToWarden::
	.string "{JPN}えんちょうは {PLAYER}から\n"
	.string "きんのいればを もらった!$"

FuchsiaCity_WardensHouse_Text_WardenPoppedInHisTeeth::
	.string "{JPN}もらった いればを\n"
	.string "いそいで くちに はめた!$"

FuchsiaCity_WardensHouse_Text_ThanksSonGiveYouSomething::
	.string "{JPN}えんちょう『すんまへん!\l"
	.string "ほんま たすかったわあ!\l"
	.string "だれに なに いうても\l"
	.string "ぜんぜん つうじへんし\p"
	.string "はずかしゅうて じむしょも\n"
	.string "かお だせへん かったんや\l"
	.string "そや ぼうず!\l"
	.string "あんがとな\l"
	.string "おれいに ええもん やるわ!$"

FuchsiaCity_WardensHouse_Text_ThanksLassieGiveYouSomething::
	.string "{JPN}えんちょう『すんまへん!\l"
	.string "ほんま たすかったわあ!\l"
	.string "だれに なに いうても\l"
	.string "ぜんぜん つうじへんし\p"
	.string "はずかしゅうて じむしょも\n"
	.string "かお だせへん かったんや\l"
	.string "そや じょーちゃん!\l"
	.string "あんがとな\l"
	.string "おれいに ええもん やるわ!$"

FuchsiaCity_WardensHouse_Text_ReceivedHM04FromWarden::
	.string "{JPN}{PLAYER}は えんちょうから\n"
	.string "ひでんマシン04を もらった!$"

FuchsiaCity_WardensHouse_Text_ExplainStrength::
	.string "{JPN}えんちょう『ひでんマシン04の\n"
	.string "なかみは かいりき やで!\p"
	.string "ポケモンが\n"
	.string "ちから もちに なってな\l"
	.string "たたかわん ときは\l"
	.string "いわを おすように なるで!\p"
	.string "そや… それから!\n"
	.string "サファリ ゾーンで\l"
	.string "トレジャー ハウスは みつけたか?$"

@ Unused
FuchsiaCity_WardensHouse_Text_YouHaveTooMuchStuff::
	.string "なんや にもつ\n"
	.string "いっぱいやんけ!$"

FuchsiaCity_WardensHouse_Text_MonPhotosFossilsOnDisplay::
	.string "{JPN}ポケモンの カセキやら\n"
	.string "しゃしんが かざって ある!$"

FuchsiaCity_WardensHouse_Text_OldMonMerchandiseOnDisplay::
	.string "{JPN}ふるい ポケモン グッズが\n"
	.string "かざって ある!$"

