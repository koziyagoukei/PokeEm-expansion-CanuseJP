MtPyre_5F_MapScripts::
	.byte 0

@ Seems like the scripts for the 4F and 5F were swapped

MtPyre_4F_EventScript_Tasha::
	trainerbattle_single TRAINER_TASHA, MtPyre_4F_Text_TashaIntro, MtPyre_4F_Text_TashaDefeat
	msgbox MtPyre_4F_Text_TashaPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_4F_Text_TashaIntro:
	.string "こわいものが すきで……\nすきで すきで たまらない……\pあたし ここに いると\nこわくて ふるえるの……"
	.string ""
	.string ""
	.string "$"

MtPyre_4F_Text_TashaDefeat:
	.string "まけるのは きらい……$"

MtPyre_4F_Text_TashaPostBattle:
	.string "こわいもの みたさで\nここから はなれられないの……\pねえ……\nあなたも いっしょに いない……?"
	.string ""
	.string ""
	.string "$"
