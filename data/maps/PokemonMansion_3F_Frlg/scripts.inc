PokemonMansion_3F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, PokemonMansion_3F_OnLoad
	.byte 0

PokemonMansion_3F_OnLoad::
	call_if_set FLAG_POKEMON_MANSION_SWITCH_STATE, PokemonMansion_EventScript_PressSwitch_3F
	end

PokemonMansion_3F_EventScript_Statue::
	lockall
	setvar VAR_0x8004, 2
	call PokemonMansion_EventScript_SecretSwitch
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	releaseall
	end

PokemonMansion_3F_EventScript_DiaryFeb6th::
	msgbox PokemonMansion_1F_Text_MewGaveBirthToMewtwo, MSGBOX_SIGN
	end

PokemonMansion_3F_EventScript_Simon::
	trainerbattle_single TRAINER_BURGLAR_SIMON, PokemonMansion_1F_Text_SimonIntro, PokemonMansion_1F_Text_SimonDefeat
	msgbox PokemonMansion_1F_Text_SimonPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_3F_EventScript_Braydon::
	trainerbattle_single TRAINER_SCIENTIST_BRAYDON, PokemonMansion_1F_Text_BraydonIntro, PokemonMansion_1F_Text_BraydonDefeat
	msgbox PokemonMansion_1F_Text_BraydonPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonMansion_1F_Text_SimonIntro::
	.string "{JPN}それに しても\n"
	.string "でっけー やしき だねえ!$"

PokemonMansion_1F_Text_SimonDefeat::
	.string "{JPN}どろどろ ぼーん!$"

PokemonMansion_1F_Text_SimonPostBattle::
	.string "{JPN}… あいぼうが いたんだ\n"
	.string "どこ いったんだろ?$"

PokemonMansion_1F_Text_BraydonIntro::
	.string "{JPN}ここは かつて\n"
	.string "わたしの せんせいが すんでいた$"

PokemonMansion_1F_Text_BraydonDefeat::
	.string "{JPN}なんと つよい$"

PokemonMansion_1F_Text_BraydonPostBattle::
	.string "{JPN}いきたい ばしょに いけなくて\n"
	.string "こまってるのか… そこから\l"
	.string "おもいきって とびおりて みろ!$"

PokemonMansion_1F_Text_MewGaveBirthToMewtwo::
	.string "{JPN}にっき 2がつ6か\n"
	.string "ミュウが こどもを うむ\p"
	.string "うまれた ばかりの ジュニアを\n"
	.string "ミュウツーと よぶことに…$"

