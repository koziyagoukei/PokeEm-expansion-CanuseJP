.equ REQUIRED_CAUGHT_MONS, 30

Route11_EastEntrance_2F_Frlg_MapScripts::
	.byte 0

Route11_EastEntrance_2F_EventScript_LeftBinoculars::
	lockall
	goto_if_set FLAG_WOKE_UP_ROUTE_12_SNORLAX, Route11_EastEntrance_2F_EventScript_LeftBinocularsSnorlaxGone
	msgbox Route11_EastEntrance_2F_Text_BigMonAsleepOnRoad
	releaseall
	end

Route11_EastEntrance_2F_EventScript_LeftBinocularsSnorlaxGone::
	msgbox Route11_EastEntrance_2F_Text_WhatABreathtakingView
	releaseall
	end

Route11_EastEntrance_2F_EventScript_RightBinoculars::
	msgbox Route11_EastEntrance_2F_Text_RockTunnelGoodRouteToLavender, MSGBOX_SIGN
	end

Route11_EastEntrance_2F_EventScript_Turner::
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_NIDORINOA
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_NINA_TRADE, Route11_EastEntrance_2F_EventScript_AlreadyTraded
	msgbox Trade_Text_LookingForMonWannaTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route11_EastEntrance_2F_EventScript_DeclineTrade
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, Route11_EastEntrance_2F_EventScript_DeclineTrade
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, Route11_EastEntrance_2F_EventScript_NotRequestedMon
	call EventScript_DoInGameTrade
	msgbox Trade_Text_HeyThanks
	setflag FLAG_DID_NINA_TRADE
	release
	end

Route11_EastEntrance_2F_EventScript_DeclineTrade::
	msgbox Trade_Text_AwwOhWell
	release
	end

Route11_EastEntrance_2F_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_WhatThatsNoMon
	release
	end

Route11_EastEntrance_2F_EventScript_AlreadyTraded::
	msgbox Trade_Text_IsntMyOldMonGreat
	release
	end

Route11_EastEntrance_2F_EventScript_Aide::
	lock
	faceplayer
	call Route11_EastEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_ITEMFINDER, Route11_EastEntrance_2F_EventScript_AlreadyGotItemfinder
	msgbox Route11_EastEntrance_2F_Text_GiveItemfinderIfCaught30, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route11_EastEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route11_EastEntrance_2F_Text_GreatHereYouGo
	checkitemspace ITEM_ITEMFINDER
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route11_EastEntrance_2F_Text_ReceivedItemfinderFromAide, ITEM_ITEMFINDER
	setflag FLAG_GOT_ITEMFINDER
	msgbox Route11_EastEntrance_2F_Text_ExplainItemfinder
	release
	end

Route11_EastEntrance_2F_EventScript_AlreadyGotItemfinder::
	msgbox Route11_EastEntrance_2F_Text_ExplainItemfinder
	release
	end

Route11_EastEntrance_2F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferitemname STR_VAR_2, ITEM_ITEMFINDER
	return

Route11_EastEntrance_2F_Text_GiveItemfinderIfCaught30::
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

Route11_EastEntrance_2F_Text_GreatHereYouGo::
	.string "{JPN}… … なるほど!\n"
	.string "つかまえた ポケモンは…\l"
	.string "{STR_VAR_3}しゅるい ですね!\n"
	.string "おめでとう!\p"
	.string "それでは\n"
	.string "これを おわたし します!$"

Route11_EastEntrance_2F_Text_ReceivedItemfinderFromAide::
	.string "{JPN}{PLAYER}は じょしゅ から\n"
	.string "{STR_VAR_2}を もらった!$"

Route11_EastEntrance_2F_Text_ExplainItemfinder::
	.string "{JPN}めに みえて なくても\n"
	.string "べんりな どうぐが\l"
	.string "あちこちに おちて います!\p"
	.string "ダウジング マシンで\n"
	.string "ちかくに なにか おちてるか\l"
	.string "どうかが わかります!\p"
	.string "くわしい ばしょは\n"
	.string "マシンでも わかりません!\p"
	.string "マシンがしめす ほうこうに\n"
	.string "どうぐは おちて いますから\l"
	.string "その しるしを たよりに\l"
	.string "ちかくを さがして みて ください$"

Route11_EastEntrance_2F_Text_BigMonAsleepOnRoad::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "どうろに ポケモンが ねてる!$"

Route11_EastEntrance_2F_Text_WhatABreathtakingView::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "すばらしい けしきだ!$"

Route11_EastEntrance_2F_Text_RockTunnelGoodRouteToLavender::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "シオン タウンに いく には…\n"
	.string "ハナダ シティ から\l"
	.string "イワヤマを ぬければ よさそう…$"

