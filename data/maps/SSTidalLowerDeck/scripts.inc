SSTidalLowerDeck_MapScripts::
	.byte 0

SSTidalLowerDeck_EventScript_Phillip::
	trainerbattle_single TRAINER_PHILLIP, SSTidalLowerDeck_Text_PhillipIntro, SSTidalLowerDeck_Text_PhillipDefeat
	msgbox SSTidalLowerDeck_Text_PhillipPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalLowerDeck_EventScript_Leonard::
	trainerbattle_single TRAINER_LEONARD, SSTidalLowerDeck_Text_LeonardIntro, SSTidalLowerDeck_Text_LeonardDefeat
	msgbox SSTidalLowerDeck_Text_LeonardPostBattle, MSGBOX_AUTOCLOSE
	end

SSTidalLowerDeck_Text_PhillipIntro:
	.string "{JPN}こんなに でっかい ところ\n"
	.string "そうじ してるのも あきたぞ！\l"
	.string "ちょいと しょうぶ しようや！$"

SSTidalLowerDeck_Text_PhillipDefeat:
	.string "{JPN}おとうと まけちまったよ！$"

SSTidalLowerDeck_Text_PhillipPostBattle:
	.string "{JPN}おれたち おそうじ ブラザーズ!\pあにきが せんざい ぶちまけてー\nおとうと せっせと みがくのさー$"

SSTidalLowerDeck_Text_LeonardIntro:
	.string "{JPN}こんなに ひろいから\n"
	.string "ふなぞこで ポケモンしょうぶ しても\l"
	.string "だいじょうぶ だろ？$"

SSTidalLowerDeck_Text_LeonardDefeat:
	.string "{JPN}あにき まけちまったよ！$"

SSTidalLowerDeck_Text_LeonardPostBattle:
	.string "{JPN}おれたち おそうじ ブラザーズ！\p"
	.string "あにきが ぜんざい ぶちまけてー\n"
	.string "おとうと せっせと みがくのさー$"

