PokevirusDepths_Helix_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokevirusDepths_Helix_OnFrame
	.byte 0

PokevirusDepths_Helix_OnFrame:
	map_script_2 VAR_TEMP_1, 0, FinalStory_EventScript_HelixTalk
	.2byte 0
