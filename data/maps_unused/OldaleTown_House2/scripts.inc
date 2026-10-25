OldaleTown_House2_MapScripts::
	.byte 0

OldaleTown_House2_EventScript_Woman::
	msgbox OldaleTown_House2_Text_PokemonLevelUp, MSGBOX_NPC
	end

OldaleTown_House2_EventScript_Man::
	msgbox OldaleTown_House2_Text_YoullGoFurtherWithStrongPokemon, MSGBOX_NPC
	end

OldaleTown_House2_Text_PokemonLevelUp:
	.string "ポケモンって たたかうと\nレベルアップして つよくなるのよ!$"

OldaleTown_House2_Text_YoullGoFurtherWithStrongPokemon:
	.string "つれて あるく ポケモンが つよかったら\nとおくまで いけるように なるね!$"

