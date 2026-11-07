CeruleanCity_House2_Frlg_MapScripts::
	.byte 0

CeruleanCity_House2_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM28_FROM_ROCKET, CeruleanCity_House2_EventScript_HikerGotTM28
	msgbox CeruleanCity_House2_Text_RocketsStoleTMForDig
	release
	end

CeruleanCity_House2_EventScript_HikerGotTM28::
	msgbox CeruleanCity_House2_Text_TeachDiglettDigWithoutTM
	release
	end

CeruleanCity_House2_EventScript_Lass::
	msgbox CeruleanCity_House2_Text_TeamRocketTryingToDigIntoNoGood, MSGBOX_NPC
	end

CeruleanCity_House2_EventScript_WallHole::
	msgbox CeruleanCity_House2_Text_TeamRocketLeftWayOut, MSGBOX_SIGN
	end

CeruleanCity_House2_Text_RocketsStoleTMForDig::
	.string "{JPN}くう…! ロケットだんめ!\p"
	.string "おれの いえを\n"
	.string "こんなに しちまいやがって!\p"
	.string "ぬすまれた わざマシンは\n"
	.string "マンキーや サンドに\p"
	.string "じめんに あなを ほる わざを\n"
	.string "おしえる ものだ!\p"
	.string "あれは たかかったのに$"

CeruleanCity_House2_Text_TeachDiglettDigWithoutTM::
	.string "{JPN}ぬすまれた ものは\n"
	.string "もどって こないと あきらめて\p"
	.string "うちの ディグダには\n"
	.string "きながに そだてながら\p"
	.string "あなほりを\n"
	.string "おしえる ことに したよ$"

CeruleanCity_House2_Text_TeamRocketTryingToDigIntoNoGood::
	.string "{JPN}ロケットだん…!\n"
	.string "ポケモンに あなを ほらせて\p"
	.string "きっと また\n"
	.string "わるだくみを かんがえてるのよ!$"

CeruleanCity_House2_Text_TeamRocketLeftWayOut::
	.string "{JPN}かべに あなが あいてる!\n"
	.string "ここから そとに でられるぞ!$"

