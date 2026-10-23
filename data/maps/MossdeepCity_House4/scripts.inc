MossdeepCity_House4_MapScripts::
	.byte 0

MossdeepCity_House4_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, MossdeepCity_House4_EventScript_CanBattleAtSecretBases
	msgbox MossdeepCity_House4_Text_BrotherLikesToFindBases, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_CanBattleAtSecretBases::
	msgbox MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_NinjaBoy::
	lock
	faceplayer
	special CheckPlayerHasSecretBase
	goto_if_eq VAR_RESULT, FALSE, MossdeepCity_House4_EventScript_NoSecretBase
	special GetSecretBaseNearbyMapName
	msgbox MossdeepCity_House4_Text_YouMadeSecretBaseNearX, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_NoSecretBase::
	msgbox MossdeepCity_House4_Text_MakeSecretBase, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_Skitty::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SKITTY, CRY_MODE_NORMAL
	msgbox MossdeepCity_House4_Text_Skitty, MSGBOX_DEFAULT
	waitmoncry
	release
	end

MossdeepCity_House4_Text_BrotherLikesToFindBases:
	.string "おとうと ったら\nひとの ひみつきちを さがすのが\lたのしみ なんだって!$"

MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle:
	.string "おとうと ったら\nともだちの ひみつきちに いって\lポケモンしょうぶを するのが\lたのしみ なんだって!$"

MossdeepCity_House4_Text_YouMadeSecretBaseNearX:
	.string "ひょっとして {STR_VAR_1} に\nひみつきち つくったの きみ?$"

MossdeepCity_House4_Text_MakeSecretBase:
	.string "きみ どこかに ひみつきち つくりなよ!\nぼくが さがしだして みせるからさ$"

MossdeepCity_House4_Text_Skitty:
	.string "エネコ『みゃあん?$"
