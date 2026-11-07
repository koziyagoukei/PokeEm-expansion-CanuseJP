FourIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FourIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

FourIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_FOUR_ISLAND
	end

FourIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

FourIsland_PokemonCenter_1F_EventScript_Man::
	msgbox FourIsland_PokemonCenter_1F_Text_RareIceMonsInCave, MSGBOX_NPC
	end

FourIsland_PokemonCenter_1F_EventScript_Woman::
	msgbox FourIsland_PokemonCenter_1F_Text_DayCarePeopleFoundEggBefore, MSGBOX_NPC
	end

FourIsland_PokemonCenter_1F_EventScript_Gentleman::
	msgbox FourIsland_PokemonCenter_1F_Text_NewPokemonJournalHasntArrived, MSGBOX_NPC
	end

@ The 5 texts below are supposed to be in FourIsland_House1
Text_BodySlamTeach::
	.string "{JPN}そーれ ぼよーん!\n"
	.string "どっちが さきに へたばるか?\l"
	.string "のしかかり しょうぶ してるんだ\p"
	.string "きみは なかまに はいったら\n"
	.string "つぶれそうだな…\l"
	.string "かわりに きみの ポケモンに\l"
	.string "のしかかり おしえて やろっか?$"

Text_BodySlamDeclined::
	.string "{JPN}また おいで ぼよーん!$"

Text_BodySlamWhichMon::
	.string "{JPN}どの ポケモンに\n"
	.string "おしえたい かい?$"

Text_BodySlamTaught::
	.string "{JPN}ぼよよーん!$"

FourIsland_House1_Text_FatMan::
	.string "それ ぼよーん!\n"
	.string "おまえ おもいなあー!$"

FourIsland_PokemonCenter_1F_Text_RareIceMonsInCave::
	.string "{JPN}どうくつには めずらしい\n"
	.string "こおりの ポケモンが いましてね\p"
	.string "それが めあてで\n"
	.string "くる ひとも おおいんです$"

FourIsland_PokemonCenter_1F_Text_DayCarePeopleFoundEggBefore::
	.string "{JPN}え ポケモンの タマゴ?\n"
	.string "…さあ\p"
	.string "あ でも そういえば\n"
	.string "そとに ある そだてやさんで\l"
	.string "タマゴが みつかったって\l"
	.string "きいた ことが あるわよ$"

FourIsland_PokemonCenter_1F_Text_NewPokemonJournalHasntArrived::
	.string "{JPN}おや?\n"
	.string "あたらしい ポケモンジャーナル\l"
	.string "この しまには\l"
	.string "とどいて いないのかな?$"

