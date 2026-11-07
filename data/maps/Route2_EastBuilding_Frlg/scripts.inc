.equ REQUIRED_SEEN_MONS, 10

Route2_EastBuilding_Frlg_MapScripts::
	.byte 0

Route2_EastBuilding_EventScript_Aide::
	lock
	faceplayer
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_HM05, Route2_EastBuilding_EventScript_AlreadyGotHM05
	msgbox Route2_EastBuilding_Text_GiveHM05IfSeen10Mons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_SEEN_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route2_EastBuilding_Text_GreatHereYouGo
	checkitemspace ITEM_HM05
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route2_EastBuilding_Text_ReceivedHM05FromAide, ITEM_HM05
	setflag FLAG_GOT_HM05
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_AlreadyGotHM05::
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_SEEN_MONS
	bufferitemname STR_VAR_2, ITEM_HM05
	return

Route2_EastBuilding_EventScript_Rocker::
	msgbox Route2_EastBuilding_Text_CanGetThroughRockTunnel, MSGBOX_NPC
	end

Route2_EastBuilding_Text_GiveHM05IfSeen10Mons::
	.string "{JPN}ぼく おぼえてます?\n"
	.string "ほら…!\l"
	.string "オーキド はかせの じょしゅ です\p"
	.string "ポケモンずかんが 10しゅるい\n"
	.string "あつまってる ようなら\l"
	.string "ひでんマシン05を わたす よう\l"
	.string "はかせに いわれ ました\p"
	.string "…… それでは さっそく\n"
	.string "{PLAYER}{KUN}!\l"
	.string "つかまえた ポケモンは\l"
	.string "10しゅるい より おおい かな?$"

Route2_EastBuilding_Text_GreatHereYouGo::
	.string "{JPN}… … なるほど!\n"
	.string "つかまえた ポケモンは…\l"
	.string "{STR_VAR_3}しゅるい ですね!\n"
	.string "おめでとう!\p"
	.string "それでは\n"
	.string "これを おわたし します!$"

Route2_EastBuilding_Text_ReceivedHM05FromAide::
	.string "{JPN}{PLAYER}は じょしゅ から\n"
	.string "ひでんマシン05を もらった!$"

Route2_EastBuilding_Text_ExplainHM05::
	.string "{JPN}ひでんの フラッシュで\n"
	.string "どんなに くらいばしょでも\l"
	.string "バッチリ あかるく なります$"

Route2_EastBuilding_Text_CanGetThroughRockTunnel::
	.string "{JPN}ポケモンが\n"
	.string "フラッシュを おぼえたら\l"
	.string "イワヤマトンネルが とおれるね$"

