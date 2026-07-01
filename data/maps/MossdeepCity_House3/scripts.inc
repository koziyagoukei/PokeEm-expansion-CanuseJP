MossdeepCity_House3_MapScripts::
	.byte 0

MossdeepCity_House3_EventScript_SuperRodFisherman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SUPER_ROD, MossdeepCity_House3_EventScript_ReceivedSuperRod
	msgbox MossdeepCity_House3_Text_YouWantSuperRod, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, MossdeepCity_House3_EventScript_DeclineSuperRod
	msgbox MossdeepCity_House3_Text_SuperRodIsSuper, MSGBOX_DEFAULT
	giveitem ITEM_SUPER_ROD
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_SUPER_ROD
	msgbox MossdeepCity_House3_Text_TryDroppingRodInWater, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_EventScript_ReceivedSuperRod::
	msgbox MossdeepCity_House3_Text_GoAfterSeafloorPokemon, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_EventScript_DeclineSuperRod::
	msgbox MossdeepCity_House3_Text_DontYouLikeToFish, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House3_Text_YouWantSuperRod:
	.string "{JPN}おーい! トレーナーよお!\nすごいつりざおは ほんとうに すごいぞお\pなんたって かいていに いる\nポケモンも つりあげられる からのお!\pどお? ほしく なった?$"

MossdeepCity_House3_Text_SuperRodIsSuper:
	.string "{JPN}そうだろ そうだろ! すごいつりざおは\nほんとうに すごい からのお!$"

MossdeepCity_House3_Text_TryDroppingRodInWater:
	.string "{JPN}みずの ある ところなら\nどこでも つりざお つかうと いいぞお!$"

MossdeepCity_House3_Text_DontYouLikeToFish:
	.string "{JPN}あら?\nつりは きらい なのかね?$"

MossdeepCity_House3_Text_GoAfterSeafloorPokemon:
	.string "{JPN}すごいつりざおで\nかいていの ポケモンも つりあげろお!$"
