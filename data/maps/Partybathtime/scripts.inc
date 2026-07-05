Partybathtime_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Partybathtime_OnTransition
	.byte 0

Partybathtime_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	special Special_SpawnPartyRoamerMons
	special Special_StartPartyBathTimeFriendship
	end
