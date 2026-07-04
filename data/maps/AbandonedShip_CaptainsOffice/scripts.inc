AbandonedShip_CaptainsOffice_MapScripts::
	.byte 0

AbandonedShip_CaptainsOffice_EventScript_CaptSternAide::
	lock
	faceplayer
	goto_if_set FLAG_EXCHANGED_SCANNER, AbandonedShip_CaptainsOffice_EventScript_ThisIsSSCactus
	checkitem ITEM_SCANNER
	goto_if_eq VAR_RESULT, TRUE, AbandonedShip_CaptainsOffice_EventScript_CanYouDeliverScanner
	goto_if_set FLAG_ITEM_ABANDONED_SHIP_HIDDEN_FLOOR_ROOM_2_SCANNER, AbandonedShip_CaptainsOffice_EventScript_ThisIsSSCactus
	msgbox AbandonedShip_CaptainsOffice_Text_NoSuccessFindingScanner, MSGBOX_DEFAULT
	release
	end

AbandonedShip_CaptainsOffice_EventScript_CanYouDeliverScanner::
	msgbox AbandonedShip_CaptainsOffice_Text_OhCanYouDeliverScanner, MSGBOX_DEFAULT
	release
	end

AbandonedShip_CaptainsOffice_EventScript_ThisIsSSCactus::
	msgbox AbandonedShip_CaptainsOffice_Text_ThisIsSSCactus, MSGBOX_DEFAULT
	release
	end

AbandonedShip_CaptainsOffice_Text_NoSuccessFindingScanner:
	.string "{JPN}クスノキ かんちょうに たのまれてね\nこのふねの ちょうさを しているんだ\pついでに たんちきを さがしてきて\nほしいって いわれたんだけど\lみあたらないなあ……$"

AbandonedShip_CaptainsOffice_Text_OhCanYouDeliverScanner:
	.string "{JPN}あ! たんちき じゃないか!\p……よかったら それ\nクスノキ かんちょうに\lわたしてきて くれないかなあ\pもうすこし このふね しらべたいんだ$"

AbandonedShip_CaptainsOffice_Text_ThisIsSSCactus:
	.string "{JPN}この ふねは カクタスごうと いってね\nちょいと むかしの ふね らしいんだ$"

