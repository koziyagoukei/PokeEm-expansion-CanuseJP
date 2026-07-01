SootopolisCity_LotadAndSeedotHouse_MapScripts::
	.byte 0

SootopolisCity_LotadAndSeedotHouse_EventScript_SeedotBrother::
	special GetSeedotSizeRecordInfo
	lock
	faceplayer
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigSeedot, MSGBOX_DEFAULT
	setvar VAR_0x8009, SPECIES_SEEDOT
	chooseboxmon SELECT_PC_MON_TRADE
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowSeedot
	special CompareSeedotSize
	goto_if_eq VAR_RESULT, COMPARE_SIZE_INCORRECT_SPECIES, SootopolisCity_LotadAndSeedotHouse_EventScript_NotSeedot
	goto_if_eq VAR_RESULT, COMPARE_SIZE_SMALLER, SootopolisCity_LotadAndSeedotHouse_EventScript_SmallSeedot
	goto_if_eq VAR_RESULT, COMPARE_SIZE_LARGER, SootopolisCity_LotadAndSeedotHouse_EventScript_BigSeedot
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigSeedot, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NotSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotSeedot, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_SmallSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerSeedot, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_BigSeedot::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_GoshMightBeBiggerThanLotad, MSGBOX_DEFAULT
	giveitem ITEM_ELIXIR
	goto_if_eq VAR_RESULT, FALSE, SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir1
	closemessage
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir1::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull1, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_LotadBrother::
	special GetLotadSizeRecordInfo
	lock
	faceplayer
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigLotad, MSGBOX_DEFAULT
	setvar VAR_0x8009, SPECIES_LOTAD
	chooseboxmon SELECT_PC_MON_TRADE
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowLotad
	special CompareLotadSize
	goto_if_eq VAR_RESULT, COMPARE_SIZE_INCORRECT_SPECIES, SootopolisCity_LotadAndSeedotHouse_EventScript_NotLotad
	goto_if_eq VAR_RESULT, COMPARE_SIZE_SMALLER, SootopolisCity_LotadAndSeedotHouse_EventScript_SmallLotad
	goto_if_eq VAR_RESULT, COMPARE_SIZE_LARGER, SootopolisCity_LotadAndSeedotHouse_EventScript_BigLotad
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_CancelShowLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigLotad, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NotLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotLotad, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_SmallLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerLotad, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_BigLotad::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_WowMightBeBiggerThanSeedot, MSGBOX_DEFAULT
	giveitem ITEM_ELIXIR
	goto_if_eq VAR_RESULT, FALSE, SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir2
	closemessage
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_NoRoomForElixir2::
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull2, MSGBOX_DEFAULT
	release
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_SeedotSizeRecord::
	special GetSeedotSizeRecordInfo
	lockall
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BiggestSeedotInHistory, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_LotadAndSeedotHouse_EventScript_LotadSizeRecord::
	special GetLotadSizeRecordInfo
	lockall
	msgbox SootopolisCity_LotadAndSeedotHouse_Text_BiggestLotadInHistory, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigSeedot:
	.string "{JPN}きみ タネボーって ポケモン しってる?\nルネでは めったに みられないん だけどね\pわし おおきな タネボーが\nめちゃめちゃ だいすき なのよ!\pおとうとは ハスボーの ほうが\nおおきいって いうんだけど\pタネボーの ほうが\nおおきいに きまってる よね!\pん? きみ タネボー もってるの?\nちょ ちょ ちょっと……\lおじさんに みせて おくれ!$"

SootopolisCity_LotadAndSeedotHouse_Text_GoshMightBeBiggerThanLotad:
	.string "{JPN}{STR_VAR_2}cm!!\nおお! こいつは でかい!\pおとうとが みた でかい ハスボーに\nまけない おおきさ かも!\pみせて くれて ありがとう\nおれいに これを あげるよ!$"

@ Unused
SootopolisCity_LotadAndSeedotHouse_Text_ReceivedPotion1:
	.string "{JPN}{PLAYER}は\nきずぐすりを もらった!$"

SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull1:
	.string "{JPN}ありゃ?\nきみ もちもの いっぱいだね$"

SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerSeedot:
	.string "{JPN}{STR_VAR_2}cmか……\pううむ わしは これより おおきい\nタネボーを みたこと あるな……$"

SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotSeedot:
	.string "{JPN}おお こいつは なかなか…… って\nこれ タネボーじゃ ないよ!$"

SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigSeedot:
	.string "{JPN}おおきい タネボー もってないの?\nそれは ざんねん……\pこんど おおきい タネボー みせてね$"

SootopolisCity_LotadAndSeedotHouse_Text_BiggestSeedotInHistory:
	.string "{JPN}しじょう さいだいの タネボー!\n{STR_VAR_2}の {STR_VAR_3}cmのやつ!\pハスボーより おおきい タネボー\nいつでも ぼしゅうちゅう!$"

SootopolisCity_LotadAndSeedotHouse_Text_PleaseShowMeBigLotad:
	.string "{JPN}きみ ハスボーって ポケモン しってる?\nルネでは めったに みられないん だけどね\pわし おおきな ハスボーが\nめちゃめちゃ だいすき なのよ!\pアニキは タネボーの ほうが\nおおきいって いうんだけど\pハスボーの ほうが\nおおきいに きまってる よね!\pん? きみ ハスボー もってるの?\nちょ ちょ ちょっと……\lおじさんに みせて おくれ!$"

SootopolisCity_LotadAndSeedotHouse_Text_WowMightBeBiggerThanSeedot:
	.string "{JPN}{STR_VAR_2}cm!!\nおお! こいつは でかい!\pあにきが みた でかい タネボーに\nまけない おおきさ かも!\pみせて くれて ありがとう\nおれいに これを あげるよ!$"

@ Unused
SootopolisCity_LotadAndSeedotHouse_Text_ReceivedPotion2:
	.string "{JPN}{PLAYER}は\nきずぐすりを もらった!$"

SootopolisCity_LotadAndSeedotHouse_Text_BagCrammedFull2:
	.string "{JPN}ありゃ?\nきみ もちもの いっぱいだね$"

SootopolisCity_LotadAndSeedotHouse_Text_SeenBiggerLotad:
	.string "{JPN}{STR_VAR_2}cmか……\pううむ わしは これより おおきい\nハスボーを みたこと あるな……$"

SootopolisCity_LotadAndSeedotHouse_Text_ThatsNotLotad:
	.string "{JPN}おお こいつは なかなか…… って\nこれ ハスボーじゃ ないよ!$"

SootopolisCity_LotadAndSeedotHouse_Text_DontHaveBigLotad:
	.string "{JPN}おおきい ハスボー もってないの?\nそれは ざんねん……\pこんど おおきい ハスボー みせてね$"

SootopolisCity_LotadAndSeedotHouse_Text_BiggestLotadInHistory:
	.string "{JPN}しじょう さいだいの ハスボー!\n{STR_VAR_2}の {STR_VAR_3}cmのやつ!\pタネボーより おおきい ハスボー\nいつでも ぼしゅうちゅう!$"
