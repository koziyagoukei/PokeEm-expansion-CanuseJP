ThreeIsland_Mart_Frlg_MapScripts::
	.byte 0

ThreeIsland_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart ThreeIsland_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
ThreeIsland_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

ThreeIsland_Mart_EventScript_Picnicker::
	msgbox ThreeIsland_Mart_Text_TrueThatCeldadonDeptStoreBigger, MSGBOX_NPC
	end

ThreeIsland_Mart_EventScript_BugCatcher::
	msgbox ThreeIsland_Mart_Text_PeopleHealWithBerriesFromForest, MSGBOX_NPC
	end

ThreeIsland_Mart_EventScript_Youngster::
	msgbox ThreeIsland_Mart_Text_BikersWereAboutToTrashMart, MSGBOX_NPC
	end

ThreeIsland_Mart_Text_TrueThatCeldadonDeptStoreBigger::
	.string "{JPN}タマムシデパートって\n"
	.string "このおみせの なんばいも\l"
	.string "おおきい って ホント?$"

ThreeIsland_Mart_Text_PeopleHealWithBerriesFromForest::
	.string "{JPN}ここで クスリを\n"
	.string "かう ことも あるけど\l"
	.string "きのみのもりで ひろった きのみで\l"
	.string "ポケモン なおすひとが おおいよ\p"
	.string "タダだし なんどでも\n"
	.string "ひろえるもん$"

ThreeIsland_Mart_Text_BikersWereAboutToTrashMart::
	.string "{JPN}ぼうそうぞくに おみせを\n"
	.string "あらされ かけて いたんだ\l"
	.string "いなくなって くれて よかった!$"

