Partybathtime_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Partybathtime_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, Partybathtime_OnTransition
	.byte 0

Partybathtime_OnLoad:
	@ Update the shared layout retained by saves made in the bath before the gallery.
	setmaplayoutindex LAYOUT_PARTYBATHTIME
	setmetatile 3, 2, METATILE_PartyBath_EmptyFrame_Left, TRUE
	setmetatile 4, 2, METATILE_PartyBath_EmptyFrame_Right, TRUE
	setmetatile 6, 2, METATILE_PartyBath_EmptyFrame_Left, TRUE
	setmetatile 7, 2, METATILE_PartyBath_EmptyFrame_Right, TRUE
	setmetatile 9, 2, METATILE_PartyBath_EmptyFrame_Left, TRUE
	setmetatile 10, 2, METATILE_PartyBath_EmptyFrame_Right, TRUE
	setmetatile 12, 2, METATILE_PartyBath_EmptyFrame_Left, TRUE
	setmetatile 13, 2, METATILE_PartyBath_EmptyFrame_Right, TRUE
	setmetatile 15, 2, METATILE_PartyBath_EmptyFrame_Left, TRUE
	setmetatile 16, 2, METATILE_PartyBath_EmptyFrame_Right, TRUE
	call_if_set FLAG_COOL_PAINTING_MADE, Partybathtime_EventScript_SetCoolPainting
	call_if_set FLAG_BEAUTY_PAINTING_MADE, Partybathtime_EventScript_SetBeautyPainting
	call_if_set FLAG_CUTE_PAINTING_MADE, Partybathtime_EventScript_SetCutePainting
	call_if_set FLAG_SMART_PAINTING_MADE, Partybathtime_EventScript_SetSmartPainting
	call_if_set FLAG_TOUGH_PAINTING_MADE, Partybathtime_EventScript_SetToughPainting
	end

Partybathtime_EventScript_SetCoolPainting:
	setmetatile 3, 2, METATILE_PartyBath_CoolPainting_Left, TRUE
	setmetatile 4, 2, METATILE_PartyBath_CoolPainting_Right, TRUE
	return

Partybathtime_EventScript_SetBeautyPainting:
	setmetatile 6, 2, METATILE_PartyBath_BeautyPainting_Left, TRUE
	setmetatile 7, 2, METATILE_PartyBath_BeautyPainting_Right, TRUE
	return

Partybathtime_EventScript_SetCutePainting:
	setmetatile 9, 2, METATILE_PartyBath_CutePainting_Left, TRUE
	setmetatile 10, 2, METATILE_PartyBath_CutePainting_Right, TRUE
	return

Partybathtime_EventScript_SetSmartPainting:
	setmetatile 12, 2, METATILE_PartyBath_SmartPainting_Left, TRUE
	setmetatile 13, 2, METATILE_PartyBath_SmartPainting_Right, TRUE
	return

Partybathtime_EventScript_SetToughPainting:
	setmetatile 15, 2, METATILE_PartyBath_ToughPainting_Left, TRUE
	setmetatile 16, 2, METATILE_PartyBath_ToughPainting_Right, TRUE
	return

Partybathtime_OnTransition:
	setflag FLAG_TEMP_HIDE_FOLLOWER
	special Special_SpawnPartyRoamerMons
	special Special_StartPartyBathTimeFriendship
	end
