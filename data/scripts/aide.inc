Aide_EventScript_HaventCaughtEnough::
	msgbox Aide_Text_HaventCaughtEnoughMonsForItem
	release
	end

Aide_EventScript_NoRoomForItem::
	msgbox Aide_Text_DontHaveAnyRoomForItem
	release
	end

Aide_EventScript_DeclineCheckMons::
	msgbox Aide_Text_GetEnoughMonsComeBackForItem
	release
	end

Aide_Text_HaventCaughtEnoughMonsForItem::
	.string "{JPN}… … えーと!\n"
	.string "つかまえた ポケモンは……\l"
	.string "あれれー?\l"
	.string "まだ {STR_VAR_3}しゅるい ですよ!\p"
	.string "{STR_VAR_1}しゅるいに たりない なら\n"
	.string "{STR_VAR_2}は\l"
	.string "また こんど ですね$"

Aide_Text_GetEnoughMonsComeBackForItem::
	.string "{JPN}…… …… そう ですか\p"
	.string "{STR_VAR_1}しゅるいに たりない なら\n"
	.string "{STR_VAR_2}は\l"
	.string "また こんど ですね$"

Aide_Text_DontHaveAnyRoomForItem::
	.string "{JPN}… … と おもった けど\n"
	.string "にもつが いっぱい だから\l"
	.string "{STR_VAR_2}は\l"
	.string "また こんど ですね$"
