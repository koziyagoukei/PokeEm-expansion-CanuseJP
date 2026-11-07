SSAnne_Deck_Frlg_MapScripts::
	.byte 0

SSAnne_Deck_EventScript_Youngster::
	msgbox SSAnne_Deck_Text_ShipDepartingSoon, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_BaldingMan::
	msgbox SSAnne_Deck_Text_FeelSeasick, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_Sailor::
	msgbox SSAnne_Deck_Text_ScrubbingDecksHardWork, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_Edmond::
	trainerbattle_single TRAINER_SAILOR_EDMOND, SSAnne_Deck_Text_EdmondIntro, SSAnne_Deck_Text_EdmondDefeat
	msgbox SSAnne_Deck_Text_EdmondPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_Deck_EventScript_Trevor::
	trainerbattle_single TRAINER_SAILOR_TREVOR, SSAnne_Deck_Text_TrevorIntro, SSAnne_Deck_Text_TrevorDefeat
	msgbox SSAnne_Deck_Text_TrevorPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_Deck_Text_ShipDepartingSoon::
	.string "{JPN}パーティも そろそろ おわるし\n"
	.string "ふねの しゅっぱつの じかん だな$"

SSAnne_Deck_Text_ScrubbingDecksHardWork::
	.string "{JPN}まったく!\n"
	.string "デッキ そうじは らくじゃねえぜ$"

SSAnne_Deck_Text_FeelSeasick::
	.string "{JPN}うう… きもち わるい!\p"
	.string "… ふなよい したので\n"
	.string "かぜに あたってるんです…$"

SSAnne_Deck_Text_EdmondIntro::
	.string "{JPN}おうおう!\p"
	.string "ふなのりの おれと しょうぶだ!$"

SSAnne_Deck_Text_EdmondDefeat::
	.string "{JPN}つよいな\n"
	.string "びっくり したぜ$"

SSAnne_Deck_Text_EdmondPostBattle::
	.string "{JPN}なあ! この せかいに\n"
	.string "ポケモンは ぜんぶで どのくらい\l"
	.string "しゅるいが あると おもうか?$"

SSAnne_Deck_Text_TrevorIntro::
	.string "{JPN}おい あんた!\n"
	.string "のってて ふなよい しないか?$"

SSAnne_Deck_Text_TrevorDefeat::
	.string "{JPN}ゆだん したか$"

SSAnne_Deck_Text_TrevorPostBattle::
	.string "{JPN}おれの おやじは ポケモンは\n"
	.string "100しゅるい いると いってたが\l"
	.string "おれは それ いじょう だと おもう$"

