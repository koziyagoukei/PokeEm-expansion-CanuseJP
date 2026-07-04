PacifidlogTown_House5_MapScripts::
	.byte 0

PacifidlogTown_House5_EventScript_MirageIslandWatcher::
	lock
	faceplayer
	specialvar VAR_RESULT, IsMirageIslandPresent
	goto_if_eq VAR_RESULT, TRUE, PacifidlogTown_House5_EventScript_MirageIslandPresent
	msgbox PacifidlogTown_House5_Text_CantSeeMirageIslandToday, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House5_EventScript_MirageIslandPresent::
	msgbox PacifidlogTown_House5_Text_CanSeeMirageIslandToday, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House5_EventScript_Gentleman::
	msgbox PacifidlogTown_House5_Text_MirageIslandAppearDependingOnWeather, MSGBOX_NPC
	end

PacifidlogTown_House5_Text_CantSeeMirageIslandToday:
	.string "{JPN}きょうは マボロシじま みえんのう……$"

PacifidlogTown_House5_Text_CanSeeMirageIslandToday:
	.string "{JPN}なっ なんと!\nきょうは マボロシじま みえるのじゃ!$"

PacifidlogTown_House5_Text_MirageIslandAppearDependingOnWeather:
	.string "{JPN}マボロシじま……\nおそらく しんきろうの かんけいで\lみえたり みえなかったり するんだろう\pいや ひょっとしたら ほんとうに\nあらわれたり きえたり しているのか……?$"

