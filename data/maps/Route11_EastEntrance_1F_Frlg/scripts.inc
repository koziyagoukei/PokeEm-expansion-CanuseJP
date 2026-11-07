Route11_EastEntrance_1F_Frlg_MapScripts::
	.byte 0

Route11_EastEntrance_1F_EventScript_TopGuard::
	msgbox Route11_EastEntrance_1F_Text_ManInLavenderRatesNames, MSGBOX_NPC
	end

Route11_EastEntrance_1F_EventScript_BottomGuard::
	msgbox Route11_EastEntrance_1F_Text_RockTunnelToReachLavender, MSGBOX_NPC
	end

@ Unclear where this is originally from
Route11_EastEntrance_1F_Text_BagIsFull::
	.string "{PLAYER}{KUN}の バッグ\n"
	.string "いっぱい みたい だね$"

Route11_EastEntrance_1F_Text_ManInLavenderRatesNames::
	.string "{JPN}ポケモンを たくさん とると\n"
	.string "なまえを つけて あげるのに\l"
	.string "くろう しちゃう でしょ!\p"
	.string "この さきの シオン タウンに\n"
	.string "ポケモンの なまえを\l"
	.string "うらなって くれる ひとが いるよ\p"
	.string "ポケモンに もっと いい なまえを\n"
	.string "つけ なおす ことも できるよ$"

Route11_EastEntrance_1F_Text_RockTunnelToReachLavender::
	.string "{JPN}シオンタウンへ むかうなら\n"
	.string "イワヤマトンネルを ぬけるといいよ\p"
	.string "ハナダシティから なら\n"
	.string "イワヤマトンネルに いけるよ$"

