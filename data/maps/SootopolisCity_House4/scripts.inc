SootopolisCity_House4_MapScripts::
	.byte 0

SootopolisCity_House4_EventScript_Man::
	msgbox SootopolisCity_House4_Text_AncientTreasuresWaitingInSea, MSGBOX_NPC
	end

SootopolisCity_House4_EventScript_Woman::
	msgbox SootopolisCity_House4_Text_StrollUnderwaterWithPokemon, MSGBOX_NPC
	end

SootopolisCity_House4_EventScript_Azumarill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZUMARILL, CRY_MODE_NORMAL
	msgbox SootopolisCity_House4_Text_Azumarill, MSGBOX_DEFAULT
	waitmoncry
	release
	end

SootopolisCity_House4_Text_AncientTreasuresWaitingInSea:
	.string "{JPN}いいことを おしえてやろう\pこの あたりの うみの そこには\nこだいの いせきが あるそうだ!\pひょっとしたら むかしの おたからが\nざくざく みつかるかも しれないぜ!$"

SootopolisCity_House4_Text_StrollUnderwaterWithPokemon:
	.string "{JPN}たからもの かぁ……\pあると いいけど\nそうでなくても ポケモンと いっしょに\lかいていを おさんぽ なんて す・て・き!$"

SootopolisCity_House4_Text_Azumarill:
	.string "{JPN}マリルリ『まるりー$"
