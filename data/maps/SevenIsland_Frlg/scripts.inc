SevenIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_OnTransition
	.byte 0

SevenIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SEVEN_ISLAND
	end

SevenIsland_EventScript_IslandSign::
	msgbox SevenIsland_Text_IslandSign, MSGBOX_SIGN
	end

SevenIsland_EventScript_OldWoman::
	msgbox SevenIsland_Text_IslandsMadeInSevenDays, MSGBOX_NPC
	end

SevenIsland_EventScript_Scientist::
	msgbox SevenIsland_Text_IslandVirtuallyUntouched, MSGBOX_NPC
	end

Text_SwordsDanceTeach::
	.string "{JPN}あまり ひとが こない\n"
	.string "この ばしょで しゅぎょう すると\l"
	.string "どんどん つよく なっていく\l"
	.string "そんな きが する!\p"
	.string "どんどん つよく なる…\p"
	.string "そう そんな わざ\n"
	.string "つるぎのまい… おぼえて みるか?$"

Text_SwordsDanceDeclined::
	.string "{JPN}ふむ\n"
	.string "それも いいだろう$"

Text_SwordsDanceWhichMon::
	.string "{JPN}よし\n"
	.string "おしえる ポケモンを えらんでくれ$"

Text_SwordsDanceTaught::
	.string "{JPN}われわれも\n"
	.string "どんどん つよく なりたいものだな$"

SevenIsland_Text_IslandSign::
	.string "{JPN}ここは 7のしま\n"
	.string "おわりなく はてしなき もとめじま$"

SevenIsland_Text_IslandsMadeInSevenDays::
	.string "{JPN}しまが 7つあるから\n"
	.string "ナナシマと よばれている…\p"
	.string "と さいきんの わかいものは\n"
	.string "おもって おるが\p"
	.string "7にちで できた しまだという\n"
	.string "いいつたえ から\l"
	.string "この なまえが ついたのじゃ$"

SevenIsland_Text_IslandVirtuallyUntouched::
	.string "{JPN}この しまは\n"
	.string "ほぼ しぜんの かたちを\l"
	.string "のこして あります\p"
	.string "だから すこし\n"
	.string "あるきにくい かも しれませんね$"

