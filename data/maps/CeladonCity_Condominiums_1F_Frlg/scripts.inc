CeladonCity_Condominiums_1F_Frlg_MapScripts::
	.byte 0

CeladonCity_Condominiums_1F_EventScript_TeaWoman::
	lock
	faceplayer
	goto_if_set FLAG_TALKED_TO_TEA_LADY_AFTER_HOF, CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea
	goto_if_set FLAG_SYS_GAME_CLEAR, CeladonCity_Condominiums_1F_EventScript_TeaWomanMentionDaisy
	goto_if_set FLAG_GOT_TEA, CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea
	msgbox CeladonCity_Condominiums_1F_Text_TryThisDrinkInstead
	setflag FLAG_GOT_TEA
	giveitem ITEM_TEA
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	msgbox CeladonCity_Condominiums_1F_Text_NothingBeatsThirstLikeTea
	release
	end

CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea::
	msgbox CeladonCity_Condominiums_1F_Text_MyDearMonsKeepMeCompany
	release
	end

CeladonCity_Condominiums_1F_EventScript_TeaWomanMentionDaisy::
	famechecker FAMECHECKER_DAISY, 4
	setflag FLAG_TALKED_TO_TEA_LADY_AFTER_HOF
	msgbox CeladonCity_Condominiums_1F_Text_DaisyComesToBuyTea
	release
	end

CeladonCity_Condominiums_1F_EventScript_Meowth::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Meowth
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_Clefairy::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CLEFAIRY, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Clefairy
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_Nidoran::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_NIDORAN_F, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Nidoran
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_SuiteSign::
	msgbox CeladonCity_Condominiums_1F_Text_ManagersSuite, MSGBOX_SIGN
	end

CeladonCity_Condominiums_1F_Text_Meowth::
	.string "{JPN}ニャース『にゃーご!$"

CeladonCity_Condominiums_1F_Text_TryThisDrinkInstead::
	.string "{JPN}ジュース ばかり\n"
	.string "のんで いては だめよ\p"
	.string "はい!$"

CeladonCity_Condominiums_1F_Text_NothingBeatsThirstLikeTea::
	.string "{JPN}やっぱり のどが かわいたら\n"
	.string "あつい おちゃ!\p"
	.string "これが いちばんよ$"

CeladonCity_Condominiums_1F_Text_MyDearMonsKeepMeCompany::
	.string "{JPN}ポケモンに かこまれてると\n"
	.string "ひとり ぐらしも さびしく ないわ\p"
	.string "うちの ニャース なんか\n"
	.string "たまに おかね ひろって くるの$"

CeladonCity_Condominiums_1F_Text_DaisyComesToBuyTea::
	.string "{JPN}おや あなた\n"
	.string "おちゃは おいしかった かい\p"
	.string "ところで あんた\n"
	.string "マサラタウンの ひと なのかい?\p"
	.string "マサラタウンで おちゃと いえば…\n"
	.string "ナナミさんが まいにち\l"
	.string "こうちゃを のんでるね\p"
	.string "タマムシデパートに たまに\n"
	.string "おちゃを かいにくるよ$"

CeladonCity_Condominiums_1F_Text_Clefairy::
	.string "{JPN}ピッピ『ぴ ぴっぴ!$"

CeladonCity_Condominiums_1F_Text_Nidoran::
	.string "{JPN}ニドラン♀『きゃ きゃう!$"

CeladonCity_Condominiums_1F_Text_ManagersSuite::
	.string "{JPN}タマムシ マンション\n"
	.string "… かんりにん しつ$"

