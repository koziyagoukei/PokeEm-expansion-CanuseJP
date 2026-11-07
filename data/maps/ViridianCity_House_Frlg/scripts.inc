ViridianCity_House_Frlg_MapScripts::
	.byte 0

ViridianCity_House_EventScript_BaldingMan::
	msgbox ViridianCity_House_Text_NicknamingIsFun, MSGBOX_NPC
	end

ViridianCity_House_EventScript_LittleGirl::
	msgbox ViridianCity_House_Text_MyDaddyLovesMonsToo, MSGBOX_NPC
	end

ViridianCity_House_EventScript_Speary::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SPEAROW, CRY_MODE_NORMAL
	msgbox ViridianCity_House_Text_Speary
	waitmoncry
	release
	end

ViridianCity_House_EventScript_NicknameSign::
	msgbox ViridianCity_House_Text_SpearowNameSpeary, MSGBOX_SIGN
	end

ViridianCity_House_Text_NicknamingIsFun::
	.string "{JPN}ニックネームを かんがえることは\n"
	.string "たのしい…が むずかしい!\p"
	.string "こった ニックネームも いいが\n"
	.string "おぼえやすいのが いちばん やな!$"

ViridianCity_House_Text_MyDaddyLovesMonsToo::
	.string "{JPN}うちの とうちゃんも\n"
	.string "ポケモン だいすきなのよ!$"

ViridianCity_House_Text_Speary::
	.string "{JPN}オニチャン『チュ チュン!$"

ViridianCity_House_Text_SpearowNameSpeary::
	.string "{JPN}オニスズメ\n"
	.string "めいめい 「オニチャン」$"

