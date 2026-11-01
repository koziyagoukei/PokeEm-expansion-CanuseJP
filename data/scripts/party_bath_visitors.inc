PartyBathVisitors_EventScript_Talk::
	lockall
	special PartyBath_BufferVisitorText
	goto_if_eq VAR_RESULT, FALSE, PartyBathVisitors_EventScript_End
	msgbox gStringVar4, MSGBOX_DEFAULT
PartyBathVisitors_EventScript_End:
	releaseall
	end
