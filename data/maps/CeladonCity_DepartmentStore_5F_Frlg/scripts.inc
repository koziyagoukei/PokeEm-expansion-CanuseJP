CeladonCity_DepartmentStore_5F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_5F_EventScript_Gentleman::
	msgbox CeladonCity_DepartmentStore_5F_Text_ExplainStatEnhancers, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_5F_EventScript_Sailor::
	msgbox CeladonCity_DepartmentStore_5F_Text_HereForStatEnhancers, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_5F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_5F_Text_Drugstore, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_5F_EventScript_ClerkXItems::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_5F_XItems
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_5F_XItems::
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFEND
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_SPECIAL
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_GUARD_SPEC
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_5F_EventScript_ClerkVitamins::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_5F_Vitamins
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_5F_Vitamins::
	.2byte ITEM_HP_UP
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_CALCIUM
	.2byte ITEM_ZINC
	.2byte ITEM_CARBOS
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_5F_Text_ExplainStatEnhancers::
	.string "{JPN}ポケモンの のうりょくを あげる\n"
	.string "どうぐは ここでしか かえない\p"
	.string "マックスアップは\n"
	.string "たいりょくを アップ\p"
	.string "リゾチウムは\n"
	.string "とくしゅ こうげきを アップ\p"
	.string "キトサンは\n"
	.string "とくしゅ ぼうぎょを アップ\p"
	.string "インドメタシンは\n"
	.string "すばやさを アップ させるのだ$"

CeladonCity_DepartmentStore_5F_Text_HereForStatEnhancers::
	.string "{JPN}ポケモンの のうりょくを あげる\n"
	.string "アイテムを かいに きたんだ!\p"
	.string "タウリンは\n"
	.string "こうげき りょく アップ!\p"
	.string "ブロムヘキシンは\n"
	.string "ぼうぎょ りょく アップ だぜ!$"

CeladonCity_DepartmentStore_5F_Text_Drugstore::
	.string "{JPN}5かい…ドラッグ ストア$"

