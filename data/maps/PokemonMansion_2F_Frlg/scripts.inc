PokemonMansion_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PokemonMansion_2F_OnLoad
	.byte 0

PokemonMansion_2F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, PokemonMansion_EventScript_PressSwitch_2F
	end

PokemonMansion_2F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 1
	call PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

PokemonMansion_2F_EventScript_DiaryJuly5th::
	msgbox PokemonMansion_1F_Text_NewMonDiscoveredInGuyanaJungle, MSGBOX_SIGN
	end

PokemonMansion_2F_EventScript_DiaryJuly10th::
	msgbox PokemonMansion_1F_Text_ChristenedDiscoveredMonMew, MSGBOX_SIGN
	end

PokemonMansion_2F_EventScript_Arnie::
	trainerbattle_single TRAINER_BURGLAR_ARNIE, PokemonMansion_1F_Text_ArnieIntro, PokemonMansion_1F_Text_ArnieDefeat
	msgbox PokemonMansion_1F_Text_ArniePostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_Text_PressSecretSwitch::
	.string "{JPN}ひみつの スイッチが ある!\p"
	.string "おして みますか?$"

PokemonMansion_Text_WhoWouldnt::
	.string "{JPN}おしてみよう! …ポチッとな$"

PokemonMansion_Text_NotQuiteYet::
	.string "{JPN}おすのは あきらめた$"

PokemonMansion_1F_Text_ArnieIntro::
	.string "{JPN}この やしき しかけが してあって\n"
	.string "なかなか すすめませんぜ!$"

PokemonMansion_1F_Text_ArnieDefeat::
	.string "{JPN}わーッ!\n"
	.string "ふろしきの なかみが$"

PokemonMansion_1F_Text_ArniePostBattle::
	.string "{JPN}ひみつの スイッチを おすと\n"
	.string "ふたつの とびらが\l"
	.string "たがい ちがいに ひらくんでさ$"

PokemonMansion_1F_Text_NewMonDiscoveredInGuyanaJungle::
	.string "{JPN}にっき 7がつ5か\n"
	.string "ここは みなみアメリカの ギアナ\p"
	.string "ジャングルの おくちで\n"
	.string "しんしゅの ポケモンを はっけん$"

PokemonMansion_1F_Text_ChristenedDiscoveredMonMew::
	.string "{JPN}にっき 7がつ10か\n"
	.string "しんはっけんの ポケモンを\l"
	.string "わたしは ミュウと なづけた$"

