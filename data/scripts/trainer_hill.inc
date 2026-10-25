TrainerHill_1F_EventScript_Lost::
	trainerhill_settrainerflags
	trainerhill_lost
	setvar VAR_TEMP_1, 1
	end

TrainerHill_1F_Movement_SetInvisible::
	set_invisible
	step_end

@ TRAINER_PHILLIP is an actual Trainer on the SS Tidal, but is used as a placeholder here
TrainerHill_EventScript_TrainerBattle::
	facilitytrainerbattle FACILITY_BATTLE_TRAINER_HILL
	trainerhill_postbattletext
	waitmessage
	waitbuttonpress
	closemessage
	end
