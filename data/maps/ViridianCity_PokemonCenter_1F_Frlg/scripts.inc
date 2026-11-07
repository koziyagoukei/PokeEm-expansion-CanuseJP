ViridianCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

ViridianCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY
	end

ViridianCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

ViridianCity_PokemonCenter_1F_EventScript_Boy::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCenterInEveryTown, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox ViridianCity_PokemonCenter_1F_Text_FeelFreeToUsePC, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCentersHealMons, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_Text_FeelFreeToUsePC::
	.string "{JPN}すみに ある パソコンは どうぞ\n"
	.string "ごじゆうに おつかいください!\p"
	.string "…と うけつけの ねーちゃんが\n"
	.string "いっとった しんせつやのう$"

ViridianCity_PokemonCenter_1F_Text_PokeCenterInEveryTown::
	.string "{JPN}ポケモンセンターは このさき\n"
	.string "どこの まちに いっても ある!\p"
	.string "なんびき あずけても ただ だし\n"
	.string "こまめに つかうと いいよ!$"

ViridianCity_PokemonCenter_1F_Text_PokeCentersHealMons::
	.string "{JPN}ポケモンセンターに あずけると\n"
	.string "つかれて うごけない\l"
	.string "ポケモンも げんきに なるよ!$"

