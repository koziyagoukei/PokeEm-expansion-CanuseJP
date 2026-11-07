LavenderTown_House1_Frlg_MapScripts::
	.byte 0

LavenderTown_House1_EventScript_CooltrainerF::
	lock
	faceplayer
	goto_if_set FLAG_RESCUED_MR_FUJI, LavenderTown_House1_EventScript_CooltrainerFGhostGone
	msgbox LavenderTown_House1_Text_RocketsKilledCubonesMother
	release
	end

LavenderTown_House1_EventScript_CooltrainerFGhostGone::
	msgbox LavenderTown_House1_Text_GhostOfPokemonTowerIsGone
	release
	end

LavenderTown_House1_EventScript_Cubone::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CUBONE, CRY_MODE_NORMAL
	msgbox LavenderTown_House1_Text_Cubone
	waitmoncry
	release
	end

LavenderTown_House1_Text_Cubone::
	.string "{JPN}カラカラ『きゃるぐうーっ$"

LavenderTown_House1_Text_RocketsKilledCubonesMother::
	.string "{JPN}わたし…!\n"
	.string "ロケットだんを ゆるさないわ!\p"
	.string "あそこに いる カラカラの\n"
	.string "おかあさんが…\p"
	.string "ロケットだんに つかまって\n"
	.string "にげる とちゅう\l"
	.string "ころされ ちゃったの$"

LavenderTown_House1_Text_GhostOfPokemonTowerIsGone::
	.string "{JPN}ポケモン タワーの\n"
	.string "ゆうれい でなく なったって!\p"
	.string "だれかが まよえる たましいを\n"
	.string "しずめて くれたのね$"

