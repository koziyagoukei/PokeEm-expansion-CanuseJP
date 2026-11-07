PokemonTower_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_1F_OnTransition
	.byte 0

PokemonTower_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POKEMON_TOWER_1F
	end

PokemonTower_1F_EventScript_Channeler::
	msgbox PokemonTower_1F_Text_SenseSpiritsUpToMischief, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman1::
	msgbox PokemonTower_1F_Text_GrowlitheWhyDidYouDie, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_BaldingMan::
	msgbox PokemonTower_1F_Text_CameToPrayForDepartedClefairy, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman2::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, PokemonTower_1F_EventScript_Woman2MalePlayer
	msgbox PokemonTower_1F_Text_ComeToPayRespectsGirl
	release
	end

PokemonTower_1F_EventScript_Woman2MalePlayer::
	msgbox PokemonTower_1F_Text_ComeToPayRespectsSon
	release
	end

PokemonTower_1F_EventScript_WorkerF::
	msgbox PokemonTower_1F_Text_ErectedInMemoryOfDeadMons, MSGBOX_NPC
	end

PokemonTower_1F_Text_ErectedInMemoryOfDeadMons::
	.string "{JPN}ポケモン タワーは\n"
	.string "しんだ ポケモンを なぐさめるため\l"
	.string "たてられた とう です$"

PokemonTower_1F_Text_ComeToPayRespectsSon::
	.string "{JPN}あなたも\n"
	.string "おはか まいりに きたの?\l"
	.string "ポケモン おもいの おとこのこ ね$"

PokemonTower_1F_Text_ComeToPayRespectsGirl::
	.string "{JPN}あなたも\n"
	.string "おはか まいりに きたの?\l"
	.string "ポケモン おもいの おんなのこ ね$"

PokemonTower_1F_Text_CameToPrayForDepartedClefairy::
	.string "{JPN}しんだ ピッピの コトが\n"
	.string "わすれられん…!\p"
	.string "…イカン!\n"
	.string "なみだが でて きおった…$"

PokemonTower_1F_Text_GrowlitheWhyDidYouDie::
	.string "{JPN}ああ わたしの ガーディ…\n"
	.string "どうして しんで しまったの…$"

PokemonTower_1F_Text_SenseSpiritsUpToMischief::
	.string "{JPN}わたしは きとうし!\n"
	.string "どうも うえの かいに\l"
	.string "まよえる たましいを かんじる$"

