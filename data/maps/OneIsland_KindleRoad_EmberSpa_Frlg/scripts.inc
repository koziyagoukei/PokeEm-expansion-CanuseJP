OneIsland_KindleRoad_EmberSpa_Frlg_MapScripts::
	.byte 0

OneIsland_KindleRoad_EmberSpa_EventScript_OldMan::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_WaterWarmsMeToCore, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BaldingMan1::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_EnjoyBowlOfChowder, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BaldingMan2::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_HotSpringIsTherapeutic, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_OldWoman::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_SeeHowSmoothMySkinIs, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BlackBelt::
	lock
	faceplayer
	famechecker FAMECHECKER_BRUNO, 3
	msgbox OneIsland_KindleRoad_EmberSpa_Text_BrunoVisitsSpaOnOccasion
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_RockSmashMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM06, OneIsland_KindleRoad_EmberSpa_EventScript_AlreadyGotHM06
	msgbox OneIsland_KindleRoad_EmberSpa_Text_UsedThisToMakeEmberSpa
	giveitem ITEM_HM06
	setflag FLAG_GOT_HM06
	msgbox OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_AlreadyGotHM06::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_SpaHeal::
	lockall
	fadescreen FADE_TO_WHITE
	special HealPlayerParty
	fadescreen FADE_FROM_WHITE
	msgbox OneIsland_KindleRoad_EmberSpa_Text_WaterExquisiteFullyRefreshed
	setvar VAR_TEMP_1, 1
	releaseall
	end

OneIsland_KindleRoad_EmberSpa_Text_WaterWarmsMeToCore::
	.string "{JPN}むッ…\n"
	.string "はあーーーーーーーーッ!\p"
	.string "ああああ… いい おゆじゃわい\n"
	.string "からだが いきかえる…\p"
	.string "ほれ おまえさんも\n"
	.string "まんなか まで いって\l"
	.string "あったまるが ええ$"

OneIsland_KindleRoad_EmberSpa_Text_EnjoyBowlOfChowder::
	.string "{JPN}この あとの いっぱいが\n"
	.string "おじさんの いちばんの たのしみ!\p"
	.string "うまいんだよなあ…\n"
	.string "ふろあがりの おみそしる!$"

OneIsland_KindleRoad_EmberSpa_Text_WaterExquisiteFullyRefreshed::
	.string "{JPN}おゆが あたたかくて\n"
	.string "とっても きもち いい…\p"
	.string "{PLAYER}と ポケモンは\n"
	.string "すっかり げんきに なった!$"

OneIsland_KindleRoad_EmberSpa_Text_HotSpringIsTherapeutic::
	.string "{JPN}おんせんで からだを なおすことを\n"
	.string "とうじ って いうんだぞ$"

OneIsland_KindleRoad_EmberSpa_Text_SeeHowSmoothMySkinIs::
	.string "{JPN}ほら!\n"
	.string "おばあちゃんの おかお\l"
	.string "いつも おんせんに はいっとるから\l"
	.string "つるつる だろ?$"

OneIsland_KindleRoad_EmberSpa_Text_BrunoVisitsSpaOnOccasion::
	.string "{JPN}れんしゅうで つかれた からだには\n"
	.string "やっぱり おんせんが きくぜ!\p"
	.string "たまに シバ せんぱいも\n"
	.string "この おんせんに はいられて\l"
	.string "せんぱいと ポケモンの ケガを\l"
	.string "なおして いらっしゃるんだ$"

OneIsland_KindleRoad_EmberSpa_Text_UsedThisToMakeEmberSpa::
	.string "{JPN}かざんの そばには\n"
	.string "おんせんが つきものじゃ\p"
	.string "この ともしびおんせんは\n"
	.string "わしが むかし\l"
	.string "おおきな いわを ほりぬいて\l"
	.string "つくったのじゃヨ\p"
	.string "そのとき つかっていた\n"
	.string "これを やろうかナ$"

OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06::
	.string "{JPN}それで いわが\n"
	.string "バリバリ くだけるんじゃヨ$"

