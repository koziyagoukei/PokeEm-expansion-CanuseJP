MtPyre_2F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CaveHole_CheckFallDownHole
	map_script MAP_SCRIPT_ON_TRANSITION, CaveHole_FixCrackedGround
	map_script MAP_SCRIPT_ON_RESUME, MtPyre_2F_SetHoleWarp
	.byte 0

MtPyre_2F_SetHoleWarp:
	setstepcallback STEP_CB_CRACKED_FLOOR
	setholewarp MAP_MT_PYRE_1F
	end

MtPyre_2F_EventScript_Woman::
	msgbox MtPyre_2F_Text_MemoriesOfSkitty, MSGBOX_NPC
	end

MtPyre_2F_EventScript_PokefanM::
	msgbox MtPyre_2F_Text_TumbledFromFloorAbove, MSGBOX_NPC
	end

MtPyre_2F_EventScript_Mark::
	trainerbattle_single TRAINER_MARK, MtPyre_2F_Text_MarkIntro, MtPyre_2F_Text_MarkDefeat
	msgbox MtPyre_2F_Text_MarkPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Luke::
	trainerbattle_double TRAINER_DEZ_AND_LUKE, MtPyre_2F_Text_LukeIntro, MtPyre_2F_Text_LukeDefeat, MtPyre_2F_Text_LukeNotEnoughMons
	msgbox MtPyre_2F_Text_LukePostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Dez::
	trainerbattle_double TRAINER_DEZ_AND_LUKE, MtPyre_2F_Text_DezIntro, MtPyre_2F_Text_DezDefeat, MtPyre_2F_Text_DezNotEnoughMons
	msgbox MtPyre_2F_Text_DezPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Leah::
	trainerbattle_single TRAINER_LEAH, MtPyre_2F_Text_LeahIntro, MtPyre_2F_Text_LeahDefeat
	msgbox MtPyre_2F_Text_LeahPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_EventScript_Zander::
	trainerbattle_single TRAINER_ZANDER, MtPyre_2F_Text_ZanderIntro, MtPyre_2F_Text_ZanderDefeat
	msgbox MtPyre_2F_Text_ZanderPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_2F_Text_MemoriesOfSkitty:
	.string "{JPN}かわいかった エネコとの おもいで……\nおもいだすだけで なみだが あふれる……"
	.string "$"

MtPyre_2F_Text_TumbledFromFloorAbove:
	.string "{JPN}アイタタタ…… ゆかの そこかしこに\nあなが あいているのに きづかないで\lおじさん うえから おちてしまったよ"
	.string ""
	.string ""
	.string "$"

MtPyre_2F_Text_MarkIntro:
	.string "{JPN}おまえも ポケモン さがしに きたのか?\nあとから きた くせに なまいきだ!"
	.string "$"

MtPyre_2F_Text_MarkDefeat:
	.string "{JPN}ふええええええっ!\nゆるして くださいーっ!"
	.string "$"

MtPyre_2F_Text_MarkPostBattle:
	.string "{JPN}めったに ひとが はいらない やま だから\nめずらしい ポケモンが いると おもってさ"
	.string "$"

MtPyre_2F_Text_LukeIntro:
	.string "{JPN}テル『きもだめし するんだ!\nぼくの かっこいい ところ みれば\lかのじょ もっと ほれるからね!\pそうだ! きみを たおして\nかっこいい ところ みせちゃおう!"
	.string ""
	.string ""
	.string ""
	.string "$"

MtPyre_2F_Text_LukeDefeat:
	.string "{JPN}テル『あっちゃー$"

MtPyre_2F_Text_LukePostBattle:
	.string "{JPN}テル『まけちゃったけど だいじょうぶ!\nぼくが いるからさ\lきもだめし なんて へっちゃら だよ!"
	.string ""
	.string "$"

MtPyre_2F_Text_LukeNotEnoughMons:
	.string "{JPN}テル『ぼくと しょうぶ するなら\nもっと ポケモン つれてきなよ!\pそうでないと かのじょの まえで\nいいところ みせられない じゃないか!"
	.string ""
	.string ""
	.string "$"

MtPyre_2F_Text_DezIntro:
	.string "{JPN}マコ『かれしと きもだめしに きたの\nすっごく こわいけど\lかれしが いるから だいじょうぶ!\pそうだ! あなたを たおす\nかれしの かっこいい ところ みせて!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MtPyre_2F_Text_DezDefeat:
	.string "{JPN}マコ『あーん こわーい!$"

MtPyre_2F_Text_DezPostBattle:
	.string "{JPN}マコ『あたしたち ラブラブだから\nまけても へいき なんだもん!"
	.string "$"

MtPyre_2F_Text_DezNotEnoughMons:
	.string "{JPN}マコ『あたしたちと たたかいたいなら\nせめて 2ひきは ポケモン いないと\pあたしの かれし つよいんだから\nいっぴきじゃ はなしに ならないわ"
	.string ""
	.string ""
	.string "$"

MtPyre_2F_Text_LeahIntro:
	.string "{JPN}みかけぬ かおね……\nなにも おきない うちに かえりなさい!"
	.string "$"

MtPyre_2F_Text_LeahDefeat:
	.string "{JPN}むん…… てごわいのね"
	.string "$"

MtPyre_2F_Text_LeahPostBattle:
	.string "{JPN}ひい おばあさまの だい から\nここで トレーナーを やっているの……\lこの やまを まもっていくわ……"
	.string ""
	.string ""
	.string "$"

MtPyre_2F_Text_ZanderIntro:
	.string "{JPN}ぎっ ぎゃああああっ!\nでたあああっ!"
	.string "$"

MtPyre_2F_Text_ZanderDefeat:
	.string "{JPN}ま まけたああああっ!"
	.string "$"

MtPyre_2F_Text_ZanderPostBattle:
	.string "{JPN}な なにかが うごく たびに\nビクっと するよ\lここで しゅぎょう するんじゃ なかったよ"
	.string ""
	.string "$"
