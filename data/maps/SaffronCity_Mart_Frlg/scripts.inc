SaffronCity_Mart_Frlg_MapScripts::
	.byte 0

SaffronCity_Mart_EventScript_Lass::
	msgbox SaffronCity_Mart_Text_ReviveIsCostly, MSGBOX_NPC
	end

SaffronCity_Mart_EventScript_Youngster::
	msgbox SaffronCity_Mart_Text_MaxRepelMoreEffectiveThanSuper, MSGBOX_NPC
	end

SaffronCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart SaffronCity_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
SaffronCity_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

SaffronCity_Mart_Text_MaxRepelMoreEffectiveThanSuper::
	.string "{JPN}ゴールドスプレーは\n"
	.string "よわい ポケモンを よせ つけない\l"
	.string "シルバースプレー より\l"
	.string "ながもち するって$"

SaffronCity_Mart_Text_ReviveIsCostly::
	.string "{JPN}げんきのかけら たかい けど\n"
	.string "せんとうふのうの ポケモンが\l"
	.string "げんきに なるの すごいわ!$"

