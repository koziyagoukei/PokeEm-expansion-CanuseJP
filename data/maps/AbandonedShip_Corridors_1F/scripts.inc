AbandonedShip_Corridors_1F_MapScripts::
	.byte 0

AbandonedShip_Corridors_1F_EventScript_Youngster::
	msgbox AbandonedShip_Corridors_1F_Text_IsntItFunHere, MSGBOX_NPC
	end

AbandonedShip_Corridors_1F_EventScript_Charlie::
	trainerbattle_single TRAINER_CHARLIE, AbandonedShip_Corridors_1F_Text_CharlieIntro, AbandonedShip_Corridors_1F_Text_CharlieDefeat
	msgbox AbandonedShip_Corridors_1F_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
	end

AbandonedShip_Corridors_1F_Text_CharlieIntro:
	.string "{JPN}ふねの なかでも\nうきわ もっていたら おかしい?$"

AbandonedShip_Corridors_1F_Text_CharlieDefeat:
	.string "{JPN}うわぁ つよーい!$"

AbandonedShip_Corridors_1F_Text_CharliePostBattle:
	.string "{JPN}うきわを もったままだと\nモンスターボール なげにくいよ$"

AbandonedShip_Corridors_1F_Text_IsntItFunHere:
	.string "{JPN}ここって おもしろいね!\nいるだけで ワクワク してくるよ!$"
