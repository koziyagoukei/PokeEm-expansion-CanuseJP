PacifidlogTown_House4_MapScripts::
	.byte 0

PacifidlogTown_House4_EventScript_LittleGirl::
	msgbox PacifidlogTown_House4_Text_SkyPokemon, MSGBOX_NPC
	end

PacifidlogTown_House4_EventScript_Woman::
	msgbox PacifidlogTown_House4_Text_PeopleSawHighFlyingPokemon, MSGBOX_NPC
	end

PacifidlogTown_House4_EventScript_Boy::
	lock
	faceplayer
	msgbox PacifidlogTown_House4_Text_WhereDidYouComeFrom, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PacifidlogTown_House4_EventScript_Yes
	goto_if_eq VAR_RESULT, NO, PacifidlogTown_House4_EventScript_No
	end

PacifidlogTown_House4_EventScript_Yes::
	msgbox PacifidlogTown_House4_Text_YesTown, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House4_EventScript_No::
	msgbox PacifidlogTown_House4_Text_YouHaveToComeFromSomewhere, MSGBOX_DEFAULT
	release
	end

PacifidlogTown_House4_Text_PeopleSawHighFlyingPokemon:
	.string "{JPN}ホウエンの そらたかくを とんでいく\nポケモンを みた! って ひとが いるけど\pずっと とびまわって いるのかしら?\nどこかで やすんだり しそうだけど$"

PacifidlogTown_House4_Text_SkyPokemon:
	.string "{JPN}おそらの ポケモン!\nおそらの ポケモン!$"

PacifidlogTown_House4_Text_WhereDidYouComeFrom:
	.string "{JPN}きみ どこから きたの?$"

PacifidlogTown_House4_Text_YesTown:
	.string "{JPN}はい……?\nハイ… タウン……?\pしらないなぁ そんな まち$"

PacifidlogTown_House4_Text_YouHaveToComeFromSomewhere:
	.string "{JPN}いいえ って そんな……\nどこからも きてない はずが ないでしょ\pま まさか きみ……\nうみの そこで うまれた とか……?$"

