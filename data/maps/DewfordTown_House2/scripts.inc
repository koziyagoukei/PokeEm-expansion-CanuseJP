DewfordTown_House2_MapScripts::
	.byte 0

DewfordTown_House2_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SILK_SCARF, DewfordTown_House2_EventScript_ExplainSilkScarf
	msgbox DewfordTown_House2_Text_WantYouToHaveSilkScarf, MSGBOX_DEFAULT
	giveitem ITEM_SILK_SCARF
	goto_if_eq VAR_RESULT, FALSE, DewfordTown_House2_EventScript_NoRoomForScarf
	setflag FLAG_RECEIVED_SILK_SCARF
	release
	end

DewfordTown_House2_EventScript_NoRoomForScarf::
	msgbox DewfordTown_House2_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_ExplainSilkScarf::
	msgbox DewfordTown_House2_Text_ExplainSilkScarf, MSGBOX_DEFAULT
	release
	end

DewfordTown_House2_EventScript_Boy::
	msgbox DewfordTown_House2_Text_BrawlySoCool, MSGBOX_NPC
	end

DewfordTown_House2_Text_WantYouToHaveSilkScarf:
	.string "{JPN}ほら! りゅうこう さいせんたんの\nこの シルクのスカーフ ステキだろ!\pおっ めが かがやいてるね!\nオレの センスが わかるんだな きみは!\pうれしいね!\nこれは プレゼント するよ!$"

DewfordTown_House2_Text_NoRoom:
	.string "{JPN}え もてないの?\nきみの にもつを ぜんぶ うってでも\lもつべき スカーフだぜ これは!$"

DewfordTown_House2_Text_ExplainSilkScarf:
	.string "{JPN}シルクのスカーフは ノーマルタイプの\nわざの いりょくを あげてくれる!\pほとんどの ポケモンに にあう\nすてきな スカーフ ってことだぜ!$"

DewfordTown_House2_Text_BrawlySoCool:
	.string "{JPN}へえ! きみ うみを こえて\nわざわざ ムロまで きたの!\pもしかして トウキさんの うわさを きいて\nあいに きたんじゃない?\pトウキさん かっこよくて\nみんなが あこがれるもんな!$"

