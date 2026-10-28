BattleFrontier_Lounge4_MapScripts::
	.byte 0

BattleFrontier_Lounge4_EventScript_Woman::
	setvar VAR_0x8004, REGULAR_LOUNGE4_WOMAN
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge4_EventScript_Cook::
	setvar VAR_0x8004, REGULAR_LOUNGE4_COOK
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge4_EventScript_Man::
	setvar VAR_0x8004, REGULAR_LOUNGE4_MAN
	goto FrontierStory_EventScript_RegularTalk

