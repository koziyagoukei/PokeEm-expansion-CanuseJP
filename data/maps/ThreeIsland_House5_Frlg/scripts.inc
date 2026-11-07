ThreeIsland_House5_Frlg_MapScripts::
	.byte 0

ThreeIsland_House5_EventScript_LittleGirl::
	lock
	faceplayer
	goto_if_set FLAG_RESCUED_LOSTELLE, ThreeIsland_House5_EventScript_LittleGirlLostelleFound
	msgbox ThreeIsland_House5_Text_ImNotLostelle
	release
	end

ThreeIsland_House5_EventScript_LittleGirlLostelleFound::
	msgbox ThreeIsland_House5_Text_IllGoPlayWithLostelle
	release
	end

ThreeIsland_House5_Text_ImNotLostelle::
	.string "{JPN}え? あたし\n"
	.string "マヨちゃんじゃ ないよ\p"
	.string "マヨちゃんなら たぶん\n"
	.string "おべんとうに つかう きのみを\l"
	.string "もりで ひろってるよ$"

ThreeIsland_House5_Text_IllGoPlayWithLostelle::
	.string "{JPN}マヨちゃん とこ\n"
	.string "あそびに いこっかなー$"

