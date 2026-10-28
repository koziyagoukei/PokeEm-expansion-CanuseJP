PokevirusDepths_Approach_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokevirusDepths_Approach_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokevirusDepths_Approach_OnFrame
	.byte 0

@ The cook and Darkrai only appear for the first look around.
PokevirusDepths_Approach_OnTransition:
	setflag FLAG_TEMP_1D
	end

PokevirusDepths_Approach_OnFrame:
	map_script_2 VAR_TEMP_1, 0, FinalStory_EventScript_DepthsEntry
	.2byte 0
