FourIsland_Mart_Frlg_MapScripts::
	.byte 0

FourIsland_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart FourIsland_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
FourIsland_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

FourIsland_Mart_EventScript_Camper::
	msgbox FourIsland_Mart_Text_IcefallCaveIsFrigid, MSGBOX_NPC
	end

FourIsland_Mart_Text_LoreleiGrewUpOnThisIsland::
	.string "{JPN}この しまから\n"
	.string "えらく つよい トレーナーが\l"
	.string "うまれたんだ!\p"
	.string "しってんだろ?\n"
	.string "してんのうの カンナちゃん!\p"
	.string "おじいさん カンナちゃんが\n"
	.string "まだ ちーさい ときに\l"
	.string "ポケモン おしえて やったんだよ$"

FourIsland_Mart_Text_IcefallCaveIsFrigid::
	.string "{JPN}ナナシマは あたたかい ちほうだが\n"
	.string "いてだきのどうくつ だけは\l"
	.string "せいそく している ポケモンの\l"
	.string "れいきで とても さむいぞ\l"
	.string "きをつけろ!$"

