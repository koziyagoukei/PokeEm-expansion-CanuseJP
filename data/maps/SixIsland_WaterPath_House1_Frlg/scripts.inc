SixIsland_WaterPath_House1_Frlg_MapScripts::
	.byte 0

SixIsland_WaterPath_House1_EventScript_Beauty::
	lock
	faceplayer
	setvar VAR_0x8004, SPECIES_HERACROSS
	specialvar VAR_RESULT, DoesPlayerPartyContainSpecies
	goto_if_eq VAR_RESULT, FALSE, SixIsland_WaterPath_House1_EventScript_NoHeracrossInParty
	special GetHeracrossSizeRecordInfo
	msgbox SixIsland_WaterPath_House1_Text_MayIMeasureHeracross
	special ChoosePartyMon
	copyvar VAR_RESULT, VAR_0x8004
	goto_if_ge VAR_RESULT, PARTY_SIZE, SixIsland_WaterPath_House1_EventScript_DontShowMon
	special CompareHeracrossSize
	goto_if_eq VAR_RESULT, 1, SixIsland_WaterPath_House1_EventScript_ShownNonHeracross
	goto_if_eq VAR_RESULT, 2, SixIsland_WaterPath_House1_EventScript_ShownSmallHeracross
	goto_if_eq VAR_RESULT, 3, SixIsland_WaterPath_House1_EventScript_ShownBigHeracross
	goto_if_eq VAR_RESULT, 4, SixIsland_WaterPath_House1_EventScript_ShownTiedHeracross
	release
	end

SixIsland_WaterPath_House1_EventScript_NoHeracrossInParty::
	msgbox SixIsland_WaterPath_House1_Text_LoveItNeedItHeracross
	release
	end

SixIsland_WaterPath_House1_EventScript_DontShowMon::
	release
	end

SixIsland_WaterPath_House1_EventScript_ShownNonHeracross::
	msgbox SixIsland_WaterPath_House1_Text_ThisWontDo
	release
	end

SixIsland_WaterPath_House1_EventScript_ShownSmallHeracross::
	goto_if_unset FLAG_GOT_NEST_BALL_FROM_WATER_PATH_HOUSE_1, SixIsland_WaterPath_House1_EventScript_ShownBigHeracross
	msgbox SixIsland_WaterPath_House1_Text_ItsXSizeYSizeWasBiggest
	release
	end

SixIsland_WaterPath_House1_EventScript_ShownTiedHeracross::
	goto_if_unset FLAG_GOT_NEST_BALL_FROM_WATER_PATH_HOUSE_1, SixIsland_WaterPath_House1_EventScript_ShownBigHeracross
	msgbox SixIsland_WaterPath_House1_Text_ItsXSizeSameAsBefore
	release
	end

SixIsland_WaterPath_House1_EventScript_ShownBigHeracross::
	setflag FLAG_GOT_NEST_BALL_FROM_WATER_PATH_HOUSE_1
	msgbox SixIsland_WaterPath_House1_Text_ItsXSizeDeserveReward
	giveitem ITEM_NEST_BALL
	goto_if_eq VAR_RESULT, FALSE, SixIsland_WaterPath_House1_EventScript_NoRoomForNestBall
	msgbox SixIsland_WaterPath_House1_Text_WantToSeeBiggerOne
	release
	end

SixIsland_WaterPath_House1_EventScript_NoRoomForNestBall::
	msgbox SixIsland_WaterPath_House1_Text_YourBagIsFull
	release
	end

SixIsland_WaterPath_House1_EventScript_SizeRecord::
	lockall
	goto_if_set FLAG_GOT_NEST_BALL_FROM_WATER_PATH_HOUSE_1, SixIsland_WaterPath_House1_EventScript_SizeRecordNonEmpty
	msgbox SixIsland_WaterPath_House1_Text_BlankChartOfSomeSort
	releaseall
	end

SixIsland_WaterPath_House1_EventScript_SizeRecordNonEmpty::
	special GetHeracrossSizeRecordInfo
	msgbox SixIsland_WaterPath_House1_Text_BiggestHeracrossIsXSize
	releaseall
	end

SixIsland_WaterPath_House1_Text_LoveItNeedItHeracross::
	.string "{JPN}へら へら ヘラクロスー\n"
	.string "おおしい むしポケ ヘラクロスー\l"
	.string "すき すき でっかい ヘラクロスー$"

SixIsland_WaterPath_House1_Text_MayIMeasureHeracross::
	.string "{JPN}きゃー!\n"
	.string "それは ヘラクロスねっ!\p"
	.string "ねえねえ おおきさ はからせて!$"

SixIsland_WaterPath_House1_Text_ItsXSizeDeserveReward::
	.string "{JPN}やーん {STR_VAR_2}よ\n"
	.string "こんなの はじめてよー!\l"
	.string "もう これ あげちゃう!$"

SixIsland_WaterPath_House1_Text_WantToSeeBiggerOne::
	.string "{JPN}もっと もっと\n"
	.string "おおきい ヘラクロスが みたいわー\p"
	.string "すき すき でっかい ヘラクロスー$"

SixIsland_WaterPath_House1_Text_ItsXSizeSameAsBefore::
	.string "{JPN}あれ? {STR_VAR_2}って…\n"
	.string "やだ このまえと おなじよー$"

SixIsland_WaterPath_House1_Text_ItsXSizeYSizeWasBiggest::
	.string "{JPN}あん {STR_VAR_2}よ\n"
	.string "まえの {STR_VAR_3}の\l"
	.string "ヘラクロスの ほうが おおきいわ$"

SixIsland_WaterPath_House1_Text_ThisWontDo::
	.string "{JPN}あーん ちがーう!\n"
	.string "ヘラクロスは もっと\l"
	.string "つよそうで かっこよくて\l"
	.string "ツノが すてきな ポケモンなの!$"

SixIsland_WaterPath_House1_Text_YourBagIsFull::
	.string "{JPN}にもつが いっぱいで\n"
	.string "はいらないわよ$"

SixIsland_WaterPath_House1_Text_BiggestHeracrossIsXSize::
	.string "{JPN}わたしが いままでに みた\n"
	.string "すっごく おおきい ヘラクロス…\p"
	.string "{STR_VAR_3}$"

SixIsland_WaterPath_House1_Text_BlankChartOfSomeSort::
	.string "{JPN}はりがみだ!\p"
	.string "なにかの きろくを\n"
	.string "かきこめる ように なっている$"
