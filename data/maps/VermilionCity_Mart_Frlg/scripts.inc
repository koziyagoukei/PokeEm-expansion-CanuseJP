VermilionCity_Mart_Frlg_MapScripts::
	.byte 0

VermilionCity_Mart_EventScript_CooltrainerF::
	msgbox VermilionCity_Mart_Text_MonsGoodOrBadDependingOnTrainer, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_BaldingMan::
	msgbox VermilionCity_Mart_Text_TeamRocketAreWickedPeople, MSGBOX_NPC
	end

VermilionCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart VermilionCity_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
VermilionCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

VermilionCity_Mart_Text_TeamRocketAreWickedPeople::
	.string "{JPN}よのなか には ポケモンで\n"
	.string "わるだくみを する やつらも いる\p"
	.string "ポケモンを たかく うりさばいたり\n"
	.string "ときには にんきがない とか\p"
	.string "やくに たたない という りゆうで\n"
	.string "ころして しまったり\p"
	.string "そういう わるい こと ばかりする\n"
	.string "あつまりが ロケットだんさ$"

VermilionCity_Mart_Text_MonsGoodOrBadDependingOnTrainer::
	.string "{JPN}ポケモンは つかう ひとに よって\n"
	.string "よくも わるくも なる\l"
	.string "パートナーだと おもうの$"

