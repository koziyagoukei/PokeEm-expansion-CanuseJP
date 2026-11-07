SSAnne_Kitchen_Frlg_MapScripts::
	.byte 0

SSAnne_Kitchen_EventScript_Chef1::
	msgbox SSAnne_Kitchen_Text_BusyOutOfTheWay, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef2::
	msgbox SSAnne_Kitchen_Text_SawOddBerryInTrash, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef3::
	msgbox SSAnne_Kitchen_Text_SoBusyImDizzy, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef4::
	lock
	faceplayer
	msgbox SSAnne_Kitchen_Text_IAmLeChefMainCourseIs
	random 3
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, 0, SSAnne_Kitchen_EventScript_SalmonDuSalad
	call_if_eq VAR_0x8008, 1, SSAnne_Kitchen_EventScript_EelsAuBarbecue
	call_if_eq VAR_0x8008, 2, SSAnne_Kitchen_EventScript_PrimeBeefsteak
	release
	end

SSAnne_Kitchen_EventScript_SalmonDuSalad::
	msgbox SSAnne_Kitchen_Text_SalmonDuSalad
	return

SSAnne_Kitchen_EventScript_EelsAuBarbecue::
	msgbox SSAnne_Kitchen_Text_EelsAuBarbecue
	return

SSAnne_Kitchen_EventScript_PrimeBeefsteak::
	msgbox SSAnne_Kitchen_Text_PrimeBeefsteak
	return

SSAnne_Kitchen_EventScript_Chef5::
	msgbox SSAnne_Kitchen_Text_PeelSpudsEveryDay, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef6::
	msgbox SSAnne_Kitchen_Text_HearAboutSnorlaxItsAGlutton, MSGBOX_NPC
	end

SSAnne_Kitchen_EventScript_Chef7::
	msgbox SSAnne_Kitchen_Text_OnlyGetToPeelOnions, MSGBOX_NPC
	end

SSAnne_Kitchen_Text_BusyOutOfTheWay::
	.string "{JPN}ほら!\n"
	.string "そこの きみ じゃまだよ!\l"
	.string "どいた どいた!$"

SSAnne_Kitchen_Text_SawOddBerryInTrash::
	.string "{JPN}ゴミばこに へんな きのみが\n"
	.string "すてて あったけど なんだろう?$"

SSAnne_Kitchen_Text_SoBusyImDizzy::
	.string "{JPN}いそがしくって めが まわる!\n"
	.string "たのむ から あっち いってくれ!$"

SSAnne_Kitchen_Text_PeelSpudsEveryDay::
	.string "{JPN}せっせ せっせ…\p"
	.string "おれは まいにち\n"
	.string "イモの かわむきだ!\l"
	.string "せっせ せっせ…$"

SSAnne_Kitchen_Text_HearAboutSnorlaxItsAGlutton::
	.string "{JPN}そういえば くいしんぼうの\n"
	.string "カビゴン しってる?\p"
	.string "あんなに よく たべて よく ねる\n"
	.string "ポケモンは ほかに いないよ!$"

SSAnne_Kitchen_Text_OnlyGetToPeelOnions::
	.string "{JPN}しくしく しくしく…\p"
	.string "おれは まいにち\n"
	.string "タマネギの かわむきだー\l"
	.string "しくしく しくしく…$"

SSAnne_Kitchen_Text_IAmLeChefMainCourseIs::
	.string "{JPN}おっほん!\n"
	.string "わたしが ここの シェフです!\p"
	.string "きょうの メイン ディッシュは$"

SSAnne_Kitchen_Text_SalmonDuSalad::
	.string "{JPN}したびらめの ムニエル!\p"
	.string "しかし おきゃくさんに\n"
	.string "また さかな? と いわれそうだ$"

SSAnne_Kitchen_Text_EelsAuBarbecue::
	.string "{JPN}さんまの しおやき!\p"
	.string "…じゃ おきゃく さんに\n"
	.string "おこられるか?$"

SSAnne_Kitchen_Text_PrimeBeefsteak::
	.string "{JPN}ぎゅう フィレの ステーキ!\p"
	.string "でも ざいりょう\n"
	.string "にんずう ぶん あったかな?$"

