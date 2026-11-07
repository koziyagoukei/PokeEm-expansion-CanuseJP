ThreeIsland_House2_Frlg_MapScripts::
	.byte 0

ThreeIsland_House2_EventScript_Man::
	lock
	faceplayer
	famechecker FAMECHECKER_SABRINA, 1
	msgbox ThreeIsland_House2_Text_IAdmireSabrina
	release
	end

ThreeIsland_House2_EventScript_Rocker::
	msgbox ThreeIsland_House2_Text_CantMakeThisSpoonBend, MSGBOX_NPC
	end

ThreeIsland_House2_Text_CantMakeThisSpoonBend::
	.string "{JPN}うううう… えーいっ!\p"
	.string "…ダメだ! この スプーン\n"
	.string "ぜんぜん まがらん\p"
	.string "ぼくには ちょうのうりょく\n"
	.string "ないの かしらん$"

ThreeIsland_House2_Text_IAdmireSabrina::
	.string "{JPN}ヤマブキシティには\n"
	.string "エスパー ポケモンを つかう\l"
	.string "おんなの ジムリーダーが いるだろ\p"
	.string "こいつ そのひとに\n"
	.string "あこがれて いるんだ\l"
	.string "クールな びじん だってさ$"

