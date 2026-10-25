Route111_WinstrateFamilysHouse_MapScripts::
	.byte 0

Route111_WinstrateFamilysHouse_EventScript_Victor::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICTOR
	msgbox Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Victoria::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICTORIA
	goto_if_set FLAG_RECEIVED_MACHO_BRACE, Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace
	msgbox Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace, MSGBOX_DEFAULT
	giveitem ITEM_MACHO_BRACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_MACHO_BRACE
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_ReceivedMachoBrace::
	msgbox Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vivi::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VIVI
	msgbox Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_Vicky::
	lock
	faceplayer
	setvar VAR_0x8008, LOCALID_WINSTRATE_HOUSE_VICKY
	goto_if_set FLAG_TEMP_4, Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrong, MSGBOX_DEFAULT
	setflag FLAG_TEMP_4
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_AlreadySpokenTo::
	msgbox Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort, MSGBOX_DEFAULT
	goto Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection
	end

Route111_WinstrateFamilysHouse_EventScript_FaceOriginalDirection::
	closemessage
	applymovement VAR_0x8008, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

Route111_WinstrateFamilysHouse_Text_MySonIsStrongerThanYou:
	.string "きみほど ポケモンを じょうずに\n"
	.string "つかいこなす トレーナーは はじめてだ！\p"
	.string "だが きみよりも\n"
	.string "わたしの むすこの ほうが つよいぞ！\p"
	.string "なんたって ポケモン リーグへ\n"
	.string "ちょうせんに いった ぐらい だからな！$"

Route111_WinstrateFamilysHouse_Text_LikeYouToHaveMachoBrace:
	.string "この きょうせいギプスを つかって\n"
	.string "わたしたち かぞくは\l"
	.string "ポケモンを きたえて います！\p"
	.string "わたしたち かぞく ぜんいんに かった\n"
	.string "あなたには ひつようない かもしれませんが\l"
	.string "ぜひ うけとって くださいな！$"

Route111_WinstrateFamilysHouse_Text_PassionateAboutBattles:
	.string "わたしたちは ポケモンでの しょうぶに\n"
	.string "じょうねつを そそいで います！$"

Route111_WinstrateFamilysHouse_Text_StrongerFamilyMembers:
	.string "パパより ママが つよくて\p"
	.string "ママより あたしが つよくて\p"
	.string "あたしより おばあちゃんが つよくて\p"
	.string "でも おばあちゃんより つよかったのが\n"
	.string "あたしの あにき なの！$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrong:
	.string "あんたが いくら つよくても\nわしの まごに あったら\lかてなくて なきだすこと じゃろう……\pわしら かぞくが しってる トレーナーの\nだれよりも つよかった からのう!\pまごの やつも いまごろは\nポケモンリーグの チャンピオンに\lちょうせん しているの じゃろうか?\pそれとも わしの まご じゃから\nもう チャンピオン かもな…… ウヒヒ!$"

Route111_WinstrateFamilysHouse_Text_GrandsonStrongShort:
	.string "まごの やつも いまごろは\nポケモンリーグの チャンピオンに\lちょうせん しているの じゃろうか?\pそれとも わしの まご じゃから\nもう チャンピオン かもな…… ウヒヒ!$"
