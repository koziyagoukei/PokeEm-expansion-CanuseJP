PewterCity_House2_Frlg_MapScripts::
	.byte 0

PewterCity_House2_EventScript_OldMan::
	msgbox PewterCity_House2_Text_MonsLearnTechniquesAsTheyGrow, MSGBOX_NPC
	end

PewterCity_House2_EventScript_LittleBoy::
	msgbox PewterCity_House2_Text_MonsEasierCatchIfStatused, MSGBOX_NPC
	end

PewterCity_House2_Text_MonsLearnTechniquesAsTheyGrow::
	.string "{JPN}ポケモンを そだてて いくと\n"
	.string "わざを おぼえる!\p"
	.string "しかし ひとから おそわらなければ\n"
	.string "おぼえない わざも あるぞ$"

PewterCity_House2_Text_MonsEasierCatchIfStatused::
	.string "{JPN}ねむり どく やけど まひ…\n"
	.string "ポケモンの たいちょうが わるいと\l"
	.string "つかまえ やすいよ\l"
	.string "でも… かならず とれる\l"
	.string "わけじゃ ないみたい$"

