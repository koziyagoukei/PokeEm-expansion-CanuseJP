PokemonMansion_B1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PokemonMansion_B1F_OnLoad
	.byte 0

PokemonMansion_B1F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, PokemonMansion_EventScript_PressSwitch_B1F
	end

PokemonMansion_B1F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 3
	call PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

PokemonMansion_B1F_EventScript_DiarySep1st::
	msgbox PokemonMansion_B1F_Text_MewtwoIsFarTooPowerful, MSGBOX_SIGN
	end

PokemonMansion_B1F_EventScript_Lewis::
	trainerbattle_single TRAINER_BURGLAR_LEWIS, PokemonMansion_B1F_Text_LewisIntro, PokemonMansion_B1F_Text_LewisDefeat
	msgbox PokemonMansion_B1F_Text_LewisPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_B1F_EventScript_Ivan::
	trainerbattle_single TRAINER_SCIENTIST_IVAN, PokemonMansion_B1F_Text_IvanIntro, PokemonMansion_B1F_Text_IvanDefeat
	msgbox PokemonMansion_B1F_Text_IvanPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_B1F_Text_LewisIntro::
	.string "{JPN}… ありゃ こまった\n"
	.string "みちに まよったぞ$"

PokemonMansion_B1F_Text_LewisDefeat::
	.string "{JPN}ひゅー どろどろ$"

PokemonMansion_B1F_Text_LewisPostBattle::
	.string "{JPN}まえに すんでた ひとの\n"
	.string "どうぐが けっこう おちてるぜ$"

PokemonMansion_B1F_Text_IvanIntro::
	.string "{JPN}ここは ひろくて\n"
	.string "けんきゅうするには いい ばしょだ$"

PokemonMansion_B1F_Text_IvanDefeat::
	.string "{JPN}なにを するのかね$"

PokemonMansion_B1F_Text_IvanPostBattle::
	.string "{JPN}きに いったよ!\n"
	.string "ここに すめば\l"
	.string "けんきゅうも はかどるな…$"

PokemonMansion_B1F_Text_MewtwoIsFarTooPowerful::
	.string "{JPN}にっき 9がつ1にち\n"
	.string "ポケモン ミュウツーは つよすぎる\p"
	.string "ダメだ…\n"
	.string "わたしの てには おえない!$"

