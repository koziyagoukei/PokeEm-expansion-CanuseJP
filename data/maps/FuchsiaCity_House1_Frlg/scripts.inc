FuchsiaCity_House1_Frlg_MapScripts::
	.byte 0

FuchsiaCity_House1_EventScript_Woman::
	msgbox FuchsiaCity_House1_Text_WardenIsOldHasFalseTeeth, MSGBOX_NPC
	end

FuchsiaCity_House1_EventScript_OldMan::
	lock
	faceplayer
	famechecker FAMECHECKER_BILL, 2
	msgbox FuchsiaCity_House1_Text_BillIsMyGrandson
	release
	end

FuchsiaCity_House1_EventScript_LittleBoy::
	msgbox FuchsiaCity_House1_Text_BillFilesHisOwnMonData, MSGBOX_NPC
	end

FuchsiaCity_House1_Text_WardenIsOldHasFalseTeeth::
	.string "{JPN}サファリ ゾーンの えんちょう\n"
	.string "おとし なのに すごく げんきよ\p"
	.string "でも…「は」は\n"
	.string "ぜんぶ いれば らしい けど$"

FuchsiaCity_House1_Text_BillIsMyGrandson::
	.string "{JPN}おや? あんた\n"
	.string "マサキの ことを しっとるのかね?\p"
	.string "マサキは わしの まごじゃ!\p"
	.string "ちいさい ころから\n"
	.string "ものを あつめるのが すきな\l"
	.string "こども じゃった!$"

FuchsiaCity_House1_Text_BillFilesHisOwnMonData::
	.string "{JPN}マサキ にいちゃん\n"
	.string "じぶんで あつめた ポケモンも\l"
	.string "パソコンの データに してるよ!\p"
	.string "みせて もらった?$"

