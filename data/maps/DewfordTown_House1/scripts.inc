DewfordTown_House1_MapScripts::
	.byte 0

DewfordTown_House1_EventScript_Man::
	msgbox DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Woman::
	msgbox DewfordTown_House1_Text_LifeGoesSlowlyOnIsland, MSGBOX_NPC
	end

DewfordTown_House1_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox DewfordTown_House1_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

DewfordTown_House1_Text_LotToBeSaidForLivingOnIsland:
	.string "{JPN}こういう ちいさな しまで\nポケモンと かぞく なかよく くらすのも\lなかなか いい もんですよ$"

DewfordTown_House1_Text_LifeGoesSlowlyOnIsland:
	.string "{JPN}わたしは カイナシティ という\nおおきな みなとまち から\lおよめさんに きたのよ\pのんびりとした しま だけれど\nきれいな うみに かこまれて いれば\lそれだけで しあわせ なんだね$"

DewfordTown_House1_Text_Zigzagoon:
	.string "{JPN}ジグザグマ『ぐぐう!$"

