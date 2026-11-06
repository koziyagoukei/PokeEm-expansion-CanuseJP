@ The Battle Connector's reception (data/scripts/battle_connector.inc)
ArtisanCave_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ArtisanCave_1F_OnFrame
	.byte 0

ArtisanCave_1F_OnFrame:
	map_script_2 VAR_TEMP_0, 0, BattleConnector_EventScript_CheckReturn
	.2byte 0
