MossdeepCity_Mart_MapScripts::
	.byte 0

MossdeepCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart MossdeepCity_Mart_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
MossdeepCity_Mart_Pokemart:
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_NET_BALL
	.2byte ITEM_DIVE_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	pokemartlistend

MossdeepCity_Mart_EventScript_Woman::
	msgbox MossdeepCity_Mart_Text_ReviveIsFantastic, MSGBOX_NPC
	end

MossdeepCity_Mart_EventScript_Boy::
	msgbox MossdeepCity_Mart_Text_MaxRepelLastsLongest, MSGBOX_NPC
	end

MossdeepCity_Mart_EventScript_Sailor::
	msgbox MossdeepCity_Mart_Text_NetAndDiveBallsRare, MSGBOX_NPC
	end

MossdeepCity_Mart_Text_ReviveIsFantastic:
	.string "{JPN}げんきのかけら って すごい!\pこれを あたえると\nひんしの ポケモンも めざめるの!\pでも パワーポイントは\nかいふく しないから きをつけて!$"

MossdeepCity_Mart_Text_MaxRepelLastsLongest:
	.string "{JPN}ゴールドスプレーは\nよわい ポケモンを よせつけない\pかずある スプレーの なかで\nいちばん ながもち するんだ!$"

MossdeepCity_Mart_Text_NetAndDiveBallsRare:
	.string "{JPN}ネットボールと ダイブボールは\nここ トクサネでしか つくっていない\lめずらしい モンスターボール!\pネットボールは むしタイプや\nみずタイプの ポケモンに こうかてき!\pダイブボールは かいていに いる\nポケモンを つかまえ やすいんだ!$"
