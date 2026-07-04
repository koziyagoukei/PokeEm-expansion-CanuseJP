FortreeCity_House5_MapScripts::
	.byte 0

FortreeCity_House5_EventScript_PokefanF::
	msgbox FortreeCity_House5_Text_TreeHousesAreGreat, MSGBOX_NPC
	end

FortreeCity_House5_EventScript_Man::
	msgbox FortreeCity_House5_Text_AdaptedToNature, MSGBOX_NPC
	end

FortreeCity_House5_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox FortreeCity_House5_Text_Zigzagoon, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FortreeCity_House5_Text_TreeHousesAreGreat:
	.string "{JPN}ヒワマキの ツリーハウスって さいこう!\nポケモンと いっしょに くらすなら\lいちばんの まちだと おもっているわ!$"

FortreeCity_House5_Text_AdaptedToNature:
	.string "{JPN}ポケモンも ひとも\nしぜんに あわせて くらしてきたからな\pいまさら じぶんらの つごうに\nしぜんを あわせなくて いいんだよ$"

FortreeCity_House5_Text_Zigzagoon:
	.string "{JPN}ジグザグマ『ぶふう!!$"

