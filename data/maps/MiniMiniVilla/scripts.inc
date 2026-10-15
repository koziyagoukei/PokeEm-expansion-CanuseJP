MiniMiniVilla_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MiniMiniVilla_OnTransition
	.byte 0

MiniMiniVilla_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	end
