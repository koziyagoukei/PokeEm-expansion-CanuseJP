PewterCity_House2_Frlg_MapScripts::
	.byte 0

PewterCity_House2_EventScript_OldMan::
	msgbox PewterCity_House2_Text_MonsLearnTechniquesAsTheyGrow, MSGBOX_NPC
	end

PewterCity_House2_EventScript_LittleBoy::
	msgbox PewterCity_House2_Text_MonsEasierCatchIfStatused, MSGBOX_NPC
	end

PewterCity_House2_Text_MonsLearnTechniquesAsTheyGrow::
	.string "POKEMON learn new techniques as\n"
	.string "they grow.\p"
	.string "But some moves must be taught to\n"
	.string "them by people.$"

PewterCity_House2_Text_MonsEasierCatchIfStatused::
	.string "A POKEMON becomes easier to \n"
	.string "catch if it has a status problem.\p"
	.string "Sleep, poison, burn, or paralysis...\n"
	.string "Those are all effective.\p"
	.string "But catching POKEMON is never\n"
	.string "a sure thing!$"

