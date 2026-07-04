Partybathtime_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Partybathtime_OnTransition
	.byte 0

Partybathtime_OnTransition:
	special Special_StartPartyBathTimeFriendship
	end
