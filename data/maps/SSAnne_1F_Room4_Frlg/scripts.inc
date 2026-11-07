SSAnne_1F_Room4_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room4_EventScript_Woman::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SSAnne_1F_Room4_EventScript_WomanPlayerMale
	msgbox SSAnne_1F_Room4_Text_WaitressCherryPiePlease
	release
	end

SSAnne_1F_Room4_EventScript_WomanPlayerMale::
	msgbox SSAnne_1F_Room4_Text_WaiterCherryPiePlease
	release
	end

SSAnne_1F_Room4_Text_WaiterCherryPiePlease::
	.string "{JPN}ねぇ ボーイさん!\n"
	.string "わたし ケーキが たべたいの\l"
	.string "ミルフィーユを おねがい!$"

SSAnne_1F_Room4_Text_WaitressCherryPiePlease::
	.string "{JPN}ねぇ ウェイトレスさん!\n"
	.string "わたし ケーキが たべたいの\l"
	.string "ミルフィーユを おねがい!$"

