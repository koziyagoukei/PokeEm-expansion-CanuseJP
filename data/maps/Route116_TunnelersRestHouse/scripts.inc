Route116_TunnelersRestHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route116_TunnelersRestHouse_OnTransition
	.byte 0

Route116_TunnelersRestHouse_OnTransition:
	setflag FLAG_LANDMARK_TUNNELERS_REST_HOUSE
	end

Route116_TunnelersRestHouse_EventScript_Tunneler1::
	msgbox Route116_TunnelersRestHouse_Text_WeHadToStopBoring, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler2::
	msgbox Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler3::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, Route116_TunnelersRestHouse_EventScript_TunnelOpened
	msgbox Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_EventScript_TunnelOpened::
	msgbox Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_Text_WeHadToStopBoring:
	.string "あの カナシダトンネル……\pはじめは たくさんの にんげんと\nさいしんの キカイで ほりすすんでいた\pだが そのまま ほりすすむと\nあたりの ポケモンに\lあくえいきょうを あたえる らしくてね\pで おれら どうしようもなく\nここで ぼんやりと しているわけさ$"

Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf:
	.string "どうしても シダケに いきたい\n"
	.string "やつが じぶんの ちから だけで\l"
	.string "トンネルを ほっているよ\p"
	.string "にんげんの ちからで すこしずつ ほれば\n"
	.string "ポケモンを おどろかせなくて すむし\l"
	.string "まわりの しぜんにも わるい えいきょうを\l"
	.string "あたえないで すむって いってな$"	

Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel:
	.string "トンネルを つかわずに ここから\n"
	.string "シダケに いくなら うみを わたり\l"
	.string "ムロから カイナ そして\l"
	.string "キンセツを とおって いくしか ないな！$"

Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough:
	.string "なんでも シダケ までの\n"
	.string "トンネルが つうじた らしいぞ！\p"
	.string "ねがい つづければ\n"
	.string "いつかは かなう って ことなんだな！$"

