CeruleanCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

CeruleanCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	end

CeruleanCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox CeruleanCity_PokemonCenter_1F_Text_EveryoneCallsBillPokemaniac, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_EventScript_Rocker::
	msgbox CeruleanCity_PokemonCenter_1F_Text_BillDoesWhateverForRareMons, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_EventScript_Youngster::
	lock
	faceplayer
	famechecker FAMECHECKER_BILL, 1
	msgbox CeruleanCity_PokemonCenter_1F_Text_BillCollectsRareMons
	release
	end

CeruleanCity_PokemonCenter_1F_EventScript_Lass::
	msgbox CeruleanCity_PokemonCenter_1F_Text_TryTradingUpstairs, MSGBOX_NPC
	end

CeruleanCity_PokemonCenter_1F_Text_BillDoesWhateverForRareMons::
	.string "{JPN}マサキの ヤツ!\p"
	.string "めずらしい ポケモンを\n"
	.string "てに いれる ためには…\p"
	.string "あんな ことも こんな ことも\n"
	.string "へいきで するって きいたよ!$"

CeruleanCity_PokemonCenter_1F_Text_EveryoneCallsBillPokemaniac::
	.string "{JPN}きみも\n"
	.string "マサキの うわさ きいてるか?\p"
	.string "みんなが あいつを\n"
	.string "ポケモン マニアと よんでるよ!\p"
	.string "でも ものを あつめたり\n"
	.string "じまん したい きもちは\l"
	.string "だれでも ある!\p"
	.string "みんなも うらやましいんだな!$"

CeruleanCity_PokemonCenter_1F_Text_BillCollectsRareMons::
	.string "{JPN}マサキ にいちゃん\n"
	.string "ポケモン いっぱい もってるよ!\p"
	.string "めずらしい ポケモンも\n"
	.string "あつめてるって$"

CeruleanCity_PokemonCenter_1F_Text_TryTradingUpstairs::
	.string "{JPN}おともだちと 2かいへ いって\n"
	.string "もってない ポケモン\l"
	.string "こうかんして もらったら\l"
	.string "たくさん あつまるわよ!\p"
	.string "ひとから もらった ポケモンは\n"
	.string "はやく そだつしね!$"

