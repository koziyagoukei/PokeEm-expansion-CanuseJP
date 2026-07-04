LavaridgeTown_House_MapScripts::
	.byte 0

LavaridgeTown_House_EventScript_OldMan::
	msgbox LavaridgeTown_House_Text_WifeWarmingEggInHotSprings, MSGBOX_NPC
	end

LavaridgeTown_House_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox LavaridgeTown_House_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

LavaridgeTown_House_Text_WifeWarmingEggInHotSprings:
	.string "{JPN}おんせんで タマゴを あたためている\nばあさんから きいたんじゃが……\pそだてやに ポケモンを\n2ひき いっしょに あずけたらの\lなんと タマゴが みつかったそうじゃ!!$"

LavaridgeTown_House_Text_Zigzagoon:
	.string "{JPN}ジグザグマ『ぷしゅー!$"

