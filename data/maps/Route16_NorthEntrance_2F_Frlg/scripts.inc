.equ REQUIRED_CAUGHT_MONS, 40

Route16_NorthEntrance_2F_Frlg_MapScripts::
	.byte 0

Route16_NorthEntrance_2F_EventScript_LittleBoy::
	msgbox Route16_NorthEntrance_2F_Text_OnBikeRideWithGirlfriend, MSGBOX_NPC
	end

Route16_NorthEntrance_2F_EventScript_LittleGirl::
	msgbox Route16_NorthEntrance_2F_Text_RidingTogetherOnNewBikes, MSGBOX_NPC
	end

Route16_NorthEntrance_2F_EventScript_LeftBinoculars::
	msgbox Route16_NorthEntrance_2F_Text_ItsCeladonDeptStore, MSGBOX_SIGN
	end

Route16_NorthEntrance_2F_EventScript_RightBinoculars::
	msgbox Route16_NorthEntrance_2F_Text_LongPathOverWater, MSGBOX_SIGN
	end

Route16_NorthEntrance_2F_EventScript_Aide::
	lock
	faceplayer
	call Route16_NorthEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_AMULET_COIN_FROM_OAKS_AIDE, Route16_NorthEntrance_2F_EventScript_AlreadyGotAmuletCoin
	msgbox Route16_NorthEntrance_2F_Text_GiveAmuletCoinIfCaught40, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route16_NorthEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route16_NorthEntrance_2F_Text_GreatHereYouGo
	checkitemspace ITEM_AMULET_COIN
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route16_NorthEntrance_2F_Text_ReceivedAmuletCoinFromAide, ITEM_AMULET_COIN
	setflag FLAG_GOT_AMULET_COIN_FROM_OAKS_AIDE
	msgbox Route16_NorthEntrance_2F_Text_ExplainAmuletCoin
	release
	end

Route16_NorthEntrance_2F_EventScript_AlreadyGotAmuletCoin::
	msgbox Route16_NorthEntrance_2F_Text_ExplainAmuletCoin
	release
	end

Route16_NorthEntrance_2F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferitemname STR_VAR_2, ITEM_AMULET_COIN
	return

Route16_NorthEntrance_2F_Text_OnBikeRideWithGirlfriend::
	.string "{JPN}しんぴん じてんしゃで\n"
	.string "かのじょと サイクリングさ!$"

Route16_NorthEntrance_2F_Text_RidingTogetherOnNewBikes::
	.string "{JPN}しんぴん じてんしゃで\n"
	.string "うふふ!\l"
	.string "これから サイクリングに いくの$"

Route16_NorthEntrance_2F_Text_ItsCeladonDeptStore::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "タマムシ デパートが みえる!$"

Route16_NorthEntrance_2F_Text_LongPathOverWater::
	.string "{JPN}ぼうえんきょうを のぞいた!\p"
	.string "むこうに うみの うえを とおる\n"
	.string "ながーい みちが みえる!$"

Route16_NorthEntrance_2F_Text_GiveAmuletCoinIfCaught40::
	.string "{JPN}ぼく おぼえてます?\n"
	.string "ほら…!\l"
	.string "オーキド はかせの じょしゅ です\p"
	.string "ポケモンずかんが 40しゅるい\n"
	.string "あつまってる ようなら\l"
	.string "おまもりこばんを わたす よう\l"
	.string "はかせに いわれ ました\p"
	.string "…… それでは さっそく\n"
	.string "{PLAYER}{KUN}!\l"
	.string "つかまえた ポケモンは\l"
	.string "40しゅるい より おおい かな?$"

Route16_NorthEntrance_2F_Text_GreatHereYouGo::
	.string "{JPN}… … なるほど!\n"
	.string "つかまえた ポケモンは…\l"
	.string "{STR_VAR_3}しゅるい ですね!\n"
	.string "おめでとう!\p"
	.string "それでは\n"
	.string "これを おわたし します!$"

Route16_NorthEntrance_2F_Text_ReceivedAmuletCoinFromAide::
	.string "{JPN}{PLAYER}は じょしゅ から\n"
	.string "おまもりこばんを もらった!$"

Route16_NorthEntrance_2F_Text_ExplainAmuletCoin::
	.string "{JPN}おまもりこばんを もった ポケモンを\n"
	.string "トレーナーとの たたかいに だせば\p"
	.string "かったときに いつもより\n"
	.string "おおく おかねが もらえますよ!$"

