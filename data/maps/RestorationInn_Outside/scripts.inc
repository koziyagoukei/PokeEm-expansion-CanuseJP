RestorationInn_Outside_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RestorationInn_Outside_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RestorationInn_Outside_OnFrame
	.byte 0

RestorationInn_Outside_OnTransition:
	setflag FLAG_STORY_INN_VISITED
	call InnStory_EventScript_SetupOutside
	end

RestorationInn_Outside_OnFrame:
	map_script_2 VAR_TEMP_1, 1, FinalStory_EventScript_Gathering
	.2byte 0
