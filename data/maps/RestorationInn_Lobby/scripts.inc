RestorationInn_Lobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RestorationInn_Lobby_OnTransition
	.byte 0

RestorationInn_Lobby_OnTransition:
	call InnStory_EventScript_SetupLobby
	end
