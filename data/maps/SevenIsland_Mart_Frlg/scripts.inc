SevenIsland_Mart_Frlg_MapScripts::
	.byte 0

SevenIsland_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart SevenIsland_Mart_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
SevenIsland_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

SevenIsland_Mart_EventScript_Lass::
	msgbox SevenIsland_Mart_Text_MonHavePersonalitiesOfTheirOwn, MSGBOX_NPC
	end

SevenIsland_Mart_EventScript_Hiker::
	msgbox SevenIsland_Mart_Text_PreparationsCompleteForRuins, MSGBOX_NPC
	end

SevenIsland_Mart_EventScript_Fisher::
	msgbox SevenIsland_Mart_Text_NeedToFishOnSevenIsland, MSGBOX_NPC
	end

SevenIsland_Mart_Text_MonHavePersonalitiesOfTheirOwn::
	.string "{JPN}ポケモンって\n"
	.string "いろんな せいかくが あるのよね\p"
	.string "わたしの ピカチュウは\n"
	.string "せっかちな せいかく だから\l"
	.string "すばやい ポケモンに そだったわ!$"

SevenIsland_Mart_Text_PreparationsCompleteForRuins::
	.string "{JPN}じゅんびは ばんたん\n"
	.string "いせき たんけんに いくぞ!$"

SevenIsland_Mart_Text_NeedToFishOnSevenIsland::
	.string "{JPN}7のしまで つりを すれば\n"
	.string "ぜんぶの しまで\l"
	.string "つりをした ことに なるぞ!\l"
	.string "ボールを かいたして おこう$"

