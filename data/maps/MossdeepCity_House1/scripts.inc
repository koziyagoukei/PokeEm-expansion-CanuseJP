MossdeepCity_House1_MapScripts::
	.byte 0

MossdeepCity_House1_EventScript_BlackBelt::
	lock
	faceplayer
	bufferleadmonspeciesname STR_VAR_1
	msgbox MossdeepCity_House1_Text_HmmYourPokemon, MSGBOX_DEFAULT
	specialvar VAR_RESULT, GetPokeblockNameByMonNature
	goto_if_eq VAR_RESULT, 0, MossdeepCity_House1_EventScript_NeutralNature
	msgbox MossdeepCity_House1_Text_ItLikesXPokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_NeutralNature::
	msgbox MossdeepCity_House1_Text_DoesntLikeOrDislikePokeblocks, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House1_EventScript_Woman::
	msgbox MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes, MSGBOX_NPC
	end

MossdeepCity_House1_Text_HmmYourPokemon:
	.string "{JPN}ムムムッ!\nきみの {STR_VAR_1}……$"

MossdeepCity_House1_Text_ItLikesXPokeblocks:
	.string "{JPN}{STR_VAR_1} が すきだろう!\pいや まちがいなく\n{STR_VAR_1} が すきだ!$"

MossdeepCity_House1_Text_DoesntLikeOrDislikePokeblocks:
	.string "{JPN}ポロックの すききらいは ないみたいだな$"

MossdeepCity_House1_Text_HusbandCanTellPokeblockMonLikes:
	.string "{JPN}だんなったら みただけで\nポケモンが すきな ポロック わかるのよ$"
