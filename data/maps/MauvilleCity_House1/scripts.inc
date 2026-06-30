MauvilleCity_House1_MapScripts::
	.byte 0

MauvilleCity_House1_EventScript_RockSmashDude::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_ROCK_SMASH, MauvilleCity_House1_EventScript_ReceivedRockSmash
	msgbox MauvilleCity_House1_Text_ImRockSmashDudeTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_HM_ROCK_SMASH
	setflag FLAG_RECEIVED_HM_ROCK_SMASH
	setflag FLAG_HIDE_ROUTE_111_ROCK_SMASH_TIP_GUY
	msgbox MauvilleCity_House1_Text_ExplainRockSmash, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House1_EventScript_ReceivedRockSmash::
	msgbox MauvilleCity_House1_Text_MonCanFlyOutOfSmashedRock, MSGBOX_DEFAULT
	release
	end

MauvilleCity_House1_Text_ImRockSmashDudeTakeThis:
	.string "{JPN}ほっほう!\pぼくのことを いわくだきオヤジ\nなんて よぶひとが いるらしいけど\lしつれい しちゃうね!\pぼくは まだ オヤジ じゃないよ\nせめて いわくだきにいさん と\lよんでほしい ものだね!\pほっほう!\pそれはそれとして きみの ポケモン\nなかなか つよそうだな!\pよし きにいった!\nこの ひでんマシンを あげよう!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_House1_Text_ExplainRockSmash:
	.string "{JPN}それは ひでんマシン「いわくだき」!\pきみの いくてを ふさぐ\nじゃまな おおいわが あっても\pその ひでんを つかうことで\nくだいて しまえるのだ!\pがんばって いわを くだきたまえ!\nほっほう!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_House1_Text_MonCanFlyOutOfSmashedRock:
	.string "{JPN}そうそう\nいわを くだくと かくれていた\lポケモンが とびでることも あるからね!\pほっほう!"
	.string ""
	.string ""
	.string "$"
