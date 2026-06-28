GraniteCave_1F_MapScripts::
	.byte 0

GraniteCave_1F_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, GraniteCave_1F_EventScript_ReceivedFlash
	msgbox GraniteCave_1F_Text_GetsDarkAheadHereYouGo, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	setflag FLAG_RECEIVED_HM_FLASH
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_ReceivedFlash::
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_Text_GetsDarkAheadHereYouGo:
	.string "{JPN}おい おまえ\nこのさきは くらいから\lたんけん するのも たいへん だぞ!\pさっきの おとこ……\nダイゴって いったっけか?\pあいつは フラッシュが つかえたから\nだいじょうぶ だろうが……\pうむ! やまおとこは であった\nあいてに やさしく するのが モットー!\lこれを ゆずってやると するか!$"

GraniteCave_1F_Text_ExplainFlash:
	.string "{JPN}ひでん フラッシュを\nポケモンに おぼえさせて つかうとだな\pまっくらな どうくつも\nあかるく てらせるんだぞ!\pまあ つかうには ムロ ポケモンジムの\nジムバッジが ひつよう だがな$"

