LavenderTown_Mart_Frlg_MapScripts::
	.byte 0

LavenderTown_Mart_EventScript_BaldingMan::
	msgbox LavenderTown_Mart_Text_SearchingForStatRaiseItems, MSGBOX_NPC
	end

LavenderTown_Mart_EventScript_Rocker::
	msgbox LavenderTown_Mart_Text_DidYouBuyRevives, MSGBOX_NPC
	end

LavenderTown_Mart_EventScript_Youngster::
	msgbox LavenderTown_Mart_Text_TrainerDuosCanChallengeYou, MSGBOX_NPC
	end

LavenderTown_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart LavenderTown_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
LavenderTown_Mart_Items::
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_NONE
	release
	end

LavenderTown_Mart_Text_SearchingForStatRaiseItems::
	.string "{JPN}おじさんが さがしてる のは…\p"
	.string "ポケモンが たたかってる とき\n"
	.string "のうりょくを いちじ てきに\l"
	.string "あげる どうぐ なんだが…\p"
	.string "プラスパワー ディフェンダー\n"
	.string "スピーダー スペシャルアップ…\p"
	.string "…という\n"
	.string "なまえ なんだが しらんかな?$"

LavenderTown_Mart_Text_DidYouBuyRevives::
	.string "{JPN}げんきのかけらは かった?\n"
	.string "ひんしじょうたいの ポケモンを\l"
	.string "げんきに する べんりな どうぐさ$"

LavenderTown_Mart_Text_TrainerDuosCanChallengeYou::
	.string "{JPN}たまに ふたりぐみの トレーナーが\n"
	.string "いっぺんに ポケモンを\l"
	.string "だして くるときが あるよ\p"
	.string "その ときは こっちも\n"
	.string "ポケモンを 2ひき だして\l"
	.string "たたかったら いいんだ$"

LavenderTown_Mart_Text_SoldNuggetFromMountainsFor5000::
	.string "この あいだ やまおくで\n"
	.string "きんのたまを ひろい ましてね!\p"
	.string "つかえない しなもの ですが\n"
	.string "うったら なんと 5000¥でした$"
