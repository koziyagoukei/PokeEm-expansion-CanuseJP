MtPyre_4F_MapScripts::
	.byte 0

@ Seems like the scripts for the 4F and 5F were swapped

MtPyre_5F_EventScript_Atsushi::
	trainerbattle_single TRAINER_ATSUSHI, MtPyre_5F_Text_AtsushiIntro, MtPyre_5F_Text_AtsushiDefeat
	msgbox MtPyre_5F_Text_AtsushiPostBattle, MSGBOX_AUTOCLOSE
	end

MtPyre_5F_Text_AtsushiIntro:
	.string "おししょうさま……\nいまの わたしの じつりょく\lみていて ください!"
	.string "$"

MtPyre_5F_Text_AtsushiDefeat:
	.string "おししょうさま……\nすんません……"
	.string "$"

MtPyre_5F_Text_AtsushiPostBattle:
	.string "もっと つよくならないと\nここに ねむる おししょうさまも\lあんしん できないよな……"
	.string "$"
