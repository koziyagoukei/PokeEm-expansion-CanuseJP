RustboroCity_House3_MapScripts::
	.byte 0

RustboroCity_House3_EventScript_OldMan::
	msgbox RustboroCity_House3_Text_IGivePerfectlySuitedNicknames, MSGBOX_NPC
	end

RustboroCity_House3_EventScript_OldWoman::
	msgbox RustboroCity_House3_Text_NamingPikachuPekachu, MSGBOX_NPC
	end

@ Misspelling on purpose, see nickname
RustboroCity_House3_EventScript_Pekachu::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	msgbox RustboroCity_House3_Text_Pekachu, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RustboroCity_House3_Text_IGivePerfectlySuitedNicknames:
	.string "{JPN}じぶんの ポケモンには\nぴったりの ニックネームを つけるんじゃ\lいわゆる おりじなりてぃ じゃ!$"

RustboroCity_House3_Text_NamingPikachuPekachu:
	.string "{JPN}だからって ピカチュウに\n「ペカチュウ」って つけても\lほとんど かわって ないでしょうに……\pまあ わかりやすいのも\nニックネームには だいじ ですけどねぇ$"

RustboroCity_House3_Text_Pekachu:
	.string "{JPN}ペカチュウ『ぺかー!$"

