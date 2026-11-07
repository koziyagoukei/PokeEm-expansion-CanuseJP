CeladonCity_DepartmentStore_4F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_4F_EventScript_Man::
	msgbox CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_Youngster::
	msgbox CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_4F_Text_FloorSign, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_4F_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_4F_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_Items::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent::
	.string "{JPN}かのじょに プレゼントを かうんだ\p"
	.string "やっぱり ピッピにんぎょう だな!\n"
	.string "にんき あるんだよね!$"

CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll::
	.string "{JPN}ちょっと いい はなし きいたよ\p"
	.string "ポケモンが とびだして きた とき\n"
	.string "ピッピにんぎょうを なげると\l"
	.string "ポケモンは それに きを とられる\p"
	.string "つまり にげれるって ことだよ!$"

CeladonCity_DepartmentStore_4F_Text_FloorSign::
	.string "{JPN}4かい…まごころ おくろう!\n"
	.string "  ワイズマン・ギフト ショップ!\p"
	.string "ただいま ポケモン しんかのいし\n"
	.string "とくべつ はんばい ちゅう!$"
