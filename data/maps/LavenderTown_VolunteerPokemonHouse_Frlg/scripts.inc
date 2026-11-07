LavenderTown_VolunteerPokemonHouse_Frlg_MapScripts::
	.byte 0

LavenderTown_VolunteerPokemonHouse_EventScript_MrFuji::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POKE_FLUTE, LavenderTown_VolunteerPokemonHouse_EventScript_AlreadyHavePokeFlute
	msgbox LavenderTown_VolunteerPokemonHouse_Text_IdLikeYouToHaveThis
	checkitemspace ITEM_POKE_FLUTE
	goto_if_eq VAR_RESULT, FALSE, LavenderTown_VolunteerPokemonHouse_EventScript_NoRoomForPokeFlute
	setflag FLAG_GOT_POKE_FLUTE
	giveitem_msg LavenderTown_VolunteerPokemonHouse_Text_ReceivedPokeFluteFromMrFuji, ITEM_POKE_FLUTE, 1, MUS_RG_OBTAIN_KEY_ITEM
	msgbox LavenderTown_VolunteerPokemonHouse_Text_ExplainPokeFlute
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_AlreadyHavePokeFlute::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_HasPokeFluteHelpedYou
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_NoRoomForPokeFlute::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MustMakeRoomForThis
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_LittleGirl::
	lock
	faceplayer
	famechecker FAMECHECKER_MRFUJI, 0
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MrFujiLooksAfterOrphanedMons
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_LittleBoy::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MonsNiceToHug, MSGBOX_NPC
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Youngster::
	lock
	faceplayer
	goto_if_set FLAG_RESCUED_MR_FUJI, LavenderTown_VolunteerPokemonHouse_EventScript_YoungsterFujiBack
	msgbox LavenderTown_VolunteerPokemonHouse_Text_WhereDidMrFujiGo
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_YoungsterFujiBack::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_MrFujiWasPrayingForCubonesMother
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Nidorino::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_NIDORINO, CRY_MODE_NORMAL
	msgbox LavenderTown_VolunteerPokemonHouse_Text_Nidorino
	waitmoncry
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Psyduck::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PSYDUCK, CRY_MODE_NORMAL
	msgbox LavenderTown_VolunteerPokemonHouse_Text_Psyduck
	waitmoncry
	release
	end

LavenderTown_VolunteerPokemonHouse_EventScript_PokemonFanMagazine::
	lockall
	famechecker FAMECHECKER_MRFUJI, 2
	msgbox LavenderTown_VolunteerPokemonHouse_Text_GrandPrizeDrawingClipped
	releaseall
	end

LavenderTown_VolunteerPokemonHouse_EventScript_Bookshelf::
	msgbox LavenderTown_VolunteerPokemonHouse_Text_PokemonMagazinesLineShelf, MSGBOX_SIGN
	end

LavenderTown_PokemonCenter_1F_Text_HearMrFujiNotFromAroundHere::
	.string "{JPN}さいきん この まちに\n"
	.string "ひっこして きたんだ!\p"
	.string "きけば フジろうじんも\n"
	.string "もともとは この まちの\l"
	.string "しゅっしんでは ないらしいな$"

LavenderTown_VolunteerPokemonHouse_Text_WhereDidMrFujiGo::
	.string "{JPN}おっかしいなー!\n"
	.string "フジ ろうじんが いないぞ!\l"
	.string "どこに いったのかな?$"

LavenderTown_VolunteerPokemonHouse_Text_MrFujiWasPrayingForCubonesMother::
	.string "{JPN}フジ ろうじん ガラガラの れいを\n"
	.string "なぐさめに いってたんだね$"

LavenderTown_VolunteerPokemonHouse_Text_MrFujiLooksAfterOrphanedMons::
	.string "{JPN}ここは もともと\n"
	.string "フジ じいちゃんの おうち なの\p"
	.string "じいちゃん やさしいのよ!\p"
	.string "すてられたり かえなくなった\n"
	.string "ポケモンを あずかって\l"
	.string "せわ してるの$"

LavenderTown_VolunteerPokemonHouse_Text_MonsNiceToHug::
	.string "{JPN}はあ あったかい…!\n"
	.string "ポケモンって だっこ すると\l"
	.string "あったかいんだよねー!$"

LavenderTown_VolunteerPokemonHouse_Text_Nidorino::
	.string "{JPN}ニドリーノ『がぉーっ!$"

LavenderTown_VolunteerPokemonHouse_Text_Psyduck::
	.string "{JPN}コダック『ぐわっぱ!$"

LavenderTown_VolunteerPokemonHouse_Text_IdLikeYouToHaveThis::
	.string "{JPN}フジ『さて {PLAYER}{KUN}…\p"
	.string "ポケモンずかん づくりは\n"
	.string "ポケモンに たいして\l"
	.string "ふかい あいじょうが ないと\l"
	.string "かんせいは たいへん むずかしい\p"
	.string "その たすけに なるか わからんが\n"
	.string "これを あなたに さしあげよう!$"

LavenderTown_VolunteerPokemonHouse_Text_ReceivedPokeFluteFromMrFuji::
	.string "{JPN}{PLAYER}は フジ ろうじんから\n"
	.string "ポケモンのふえを もらった!$"

LavenderTown_VolunteerPokemonHouse_Text_ExplainPokeFlute::
	.string "{JPN}ポケモンのふえを ふくと\n"
	.string "グーグー ねむってる ポケモンでも\l"
	.string "げんきが わいて とびおきる!\p"
	.string "ポケモンが いねむり して\n"
	.string "こまったら つかって みなさい$"

LavenderTown_VolunteerPokemonHouse_Text_MustMakeRoomForThis::
	.string "{JPN}にもつが いっぱいだな$"

LavenderTown_VolunteerPokemonHouse_Text_HasPokeFluteHelpedYou::
	.string "{JPN}フジ『このあいだ さしあげた\n"
	.string "ふえは やくに たってるかな?$"

LavenderTown_VolunteerPokemonHouse_Text_GrandPrizeDrawingClipped::
	.string "{JPN}ごうか しょうひんが あたる!\n"
	.string "げっかん ポケモンの とも\l"
	.string "ビッグ・プレゼント!\p"
	.string "…おうぼ ほうほうは\p"
	.string "…あら きりとって ある!\n"
	.string "けんしょうが すき なのかな?$"

LavenderTown_VolunteerPokemonHouse_Text_PokemonMagazinesLineShelf::
	.string "{JPN}ポケモン ざっしが ならんでる!\p"
	.string "…ポケモン てちょう\p"
	.string "…ポケモン グラフ$"

