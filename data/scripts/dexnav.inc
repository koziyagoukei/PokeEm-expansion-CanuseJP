EventScript_StartDexNavBattle::
	lock
	playse SE_PIN
	applymovement OBJ_EVENT_ID_PLAYER Common_Movement_ExclamationMark
	waitmovement 0
	waitse
	dowildbattle	
	release
	end

EventScript_NotFoundNearby::
	msgbox sText_NotFoundNearby, MSGBOX_SIGN
	end

EventScript_MovedTooFast::
	msgbox sText_TryMovingSlower, MSGBOX_SIGN
	end

EventScript_PokemonGotAway::
	msgbox sText_PokemonGotAway, MSGBOX_SIGN
	end

EventScript_LostSignal::
	msgbox sText_LostSignal, MSGBOX_SIGN
	end

EventScript_TooDark::
	msgbox sText_TooDark, MSGBOX_SIGN
	end

sText_NotFoundNearby:
	.string "ちかくには いないようだ\n"
	.string "べつの ばしょを さがしてみよう!$"

sText_TryMovingSlower:
	.string "ポケモンに にげられた!\n"
	.string "もっと ゆっくり うごいてみよう$"

sText_PokemonGotAway:
	.string "ポケモンに にげられた!$"

sText_LostSignal:
	.string "はんのうが なくなった!\n"
	.string "みうしなって しまったようだ$"

sText_TooDark:
	.string "くらすぎて ポケモンを\nさがすことが できない!$"
