.equ REQUIRED_CAUGHT_MONS, 50

Route15_WestEntrance_2F_Frlg_MapScripts::
	.byte 0

Route15_WestEntrance_2F_EventScript_LeftBinoculars::
	lockall
	msgbox Route15_WestEntrance_2F_Text_LargeShiningBird
	showmonpic SPECIES_ARTICUNO, 10, 3
	delay 20
	waitbuttonpress
	hidemonpic
	setvar VAR_0x8004, SPECIES_ARTICUNO
	special SetSeenMon
	releaseall
	end

Route15_WestEntrance_2F_EventScript_RightBinoculars::
	msgbox Route15_WestEntrance_2F_Text_SmallIslandOnHorizon, MSGBOX_SIGN
	end

Route15_WestEntrance_2F_EventScript_Aide::
	lock
	faceplayer
	call Route15_WestEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_EXP_SHARE_FROM_OAKS_AIDE, Route15_WestEntrance_2F_EventScript_AlreadyGotExpShare
	msgbox Route15_WestEntrance_2F_Text_GiveItemIfCaughtEnough, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route15_WestEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route15_WestEntrance_2F_Text_GreatHereYouGo
	checkitemspace ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route15_WestEntrance_2F_Text_ReceivedItemFromAide, ITEM_EXP_SHARE
	setflag FLAG_GOT_EXP_SHARE_FROM_OAKS_AIDE
	msgbox Route15_WestEntrance_2F_Text_ExplainExpShare
	release
	end

Route15_WestEntrance_2F_EventScript_AlreadyGotExpShare::
	msgbox Route15_WestEntrance_2F_Text_ExplainExpShare
	release
	end

Route15_WestEntrance_2F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferitemname STR_VAR_2, ITEM_EXP_SHARE
	return

Route15_WestEntrance_2F_Text_GiveItemIfCaughtEnough::
	.string "{JPN}ぼく おぼえてます?\n"
	.string "ほら…!\l"
	.string "オーキド はかせの じょしゅ です\p"
	.string "ポケモンずかんが {STR_VAR_1}しゅるい\n"
	.string "あつまってる ようなら\l"
	.string "{STR_VAR_2}を わたす よう\l"
	.string "はかせに いわれ ました\p"
	.string "…… それでは さっそく\n"
	.string "{PLAYER}{KUN}!\l"
	.string "つかまえた ポケモンは\l"
	.string "{STR_VAR_1}しゅるい より おおい かな?$"

Route15_WestEntrance_2F_Text_GreatHereYouGo::
	.string "{JPN}… … なるほど!\n"
	.string "つかまえた ポケモンは…\l"
	.string "{STR_VAR_3}しゅるい ですね!\n"
	.string "おめでとう!\p"
	.string "それでは\n"
	.string "これを おわたし します!$"

Route15_WestEntrance_2F_Text_ReceivedItemFromAide::
	.string "{JPN}{PLAYER}は じょしゅ から\n"
	.string "{STR_VAR_2}を もらった!$"

Route15_WestEntrance_2F_Text_ExplainExpShare::
	.string "{JPN}がくしゅうそうちを\n"
	.string "ポケモンに もたせると\l"
	.string "その ポケモンは\l"
	.string "たたかわなくても\l"
	.string "けいけんちの わけまえが\l"
	.string "もらえます$"

Route15_WestEntrance_2F_Text_LargeShiningBird::
	.string "{JPN}ぼうえんきょうを のぞいた…\p"
	.string "おおきな とりが\n"
	.string "キラキラ ひかりながら\l"
	.string "うみの ほうへ とんでいく…$"

Route15_WestEntrance_2F_Text_SmallIslandOnHorizon::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "うみの むこうに…\n"
	.string "ちいさな しまが みえる!$"

