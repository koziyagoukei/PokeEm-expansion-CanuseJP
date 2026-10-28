PokevirusDepths_Core_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokevirusDepths_Core_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokevirusDepths_Core_OnFrame
	.byte 0

PokevirusDepths_Core_OnTransition:
	call FinalStory_EventScript_SetupCore
	end

PokevirusDepths_Core_OnFrame:
	map_script_2 VAR_TEMP_1, 0, FinalStory_EventScript_CoreArrival
	.2byte 0
