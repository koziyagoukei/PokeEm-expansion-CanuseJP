FortreeCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, FortreeCity_OnResume
	.byte 0

FortreeCity_OnTransition:
	setflag FLAG_VISITED_FORTREE_CITY
	end

FortreeCity_OnResume:
	setstepcallback STEP_CB_FORTREE_BRIDGE
	end

FortreeCity_EventScript_Man::
	msgbox FortreeCity_Text_SawGiganticPokemonInSky, MSGBOX_NPC
	end

FortreeCity_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_KECLEON_FLED_FORTREE, FortreeCity_EventScript_WomanGymAccessible
	msgbox FortreeCity_Text_SomethingBlockingGym, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_WomanGymAccessible::
	msgbox FortreeCity_Text_ThisTimeIllBeatWinona, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_Girl::
	msgbox FortreeCity_Text_TreesGrowByDrinkingRainwater, MSGBOX_NPC
	end

FortreeCity_EventScript_OldMan::
	msgbox FortreeCity_Text_EveryoneHealthyAndLively, MSGBOX_NPC
	end

FortreeCity_EventScript_Boy::
	msgbox FortreeCity_Text_BugPokemonComeThroughWindow, MSGBOX_NPC
	end

FortreeCity_EventScript_GameboyKid::
	msgbox FortreeCity_Text_PokemonThatEvolveWhenTraded, MSGBOX_NPC
	end

FortreeCity_EventScript_CitySign::
	msgbox FortreeCity_Text_CitySign, MSGBOX_SIGN
	end

FortreeCity_EventScript_GymSign::
	msgbox FortreeCity_Text_GymSign, MSGBOX_SIGN
	end

FortreeCity_EventScript_Kecleon::
	lock
	faceplayer
	checkitem ITEM_DEVON_SCOPE
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_EventScript_AskUseDevonScope
	msgbox FortreeCity_Text_SomethingUnseeable, MSGBOX_DEFAULT
	release
	end

FortreeCity_EventScript_AskUseDevonScope::
	msgbox FortreeCity_Text_UnseeableUseDevonScope, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FortreeCity_EventScript_UseDevonScope
	release
	end

FortreeCity_EventScript_UseDevonScope::
	msgbox FortreeCity_Text_UsedDevonScopePokemonFled, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Movement_KecleonAppears
	waitmovement 0
	waitse
	playmoncry SPECIES_KECLEON, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	applymovement VAR_LAST_TALKED, FortreeCity_Movement_KecleonFlee
	waitmovement 0
	removeobject VAR_LAST_TALKED
	setflag FLAG_KECLEON_FLED_FORTREE
	release
	end

FortreeCity_Movement_KecleonFlee:
	walk_right
	step_end

FortreeCity_Text_SawGiganticPokemonInSky:
	.string "だれも しんじて くれないけど\nおおきな ポケモンが からだを くねらせ\lそらを とんでいるのを みたんだ……\pそいつは 131ばん すいどうの ほうへ\nとんでいったよ……\p……ところで\nなんか きみ コゲくさいね\pかざんに でも いってきた?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Text_SomethingBlockingGym:
	.string "ポケモンジムに いきたいのに\nなにかが みちを ふさいでいるのよ!\pせっかく 120ばん どうろで\nポケモン そだてて きたのに!"
	.string ""
	.string ""
	.string "$"

FortreeCity_Text_ThisTimeIllBeatWinona:
	.string "あたしの じまんの ポケモンで\nこんどこそ ナギさんに かってみせるわ!"
	.string "$"

FortreeCity_Text_TreesGrowByDrinkingRainwater:
	.string "あめが じめんに すいこまれて\nその みずを すって きがそだつ……\pみずと つちが りょうほう あるから\nこの ヒワマキシティも あるのよね"
	.string ""
	.string ""
	.string "$"

FortreeCity_Text_EveryoneHealthyAndLively:
	.string "このまちは きのうえに\nいえを たてて くらしている……\pそのせいか みんな げんきで\nイキイキと している のですよ\pわしも このまちに きてから\n30ねんほど わかがえった きぶんです"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Text_BugPokemonComeThroughWindow:
	.string "きのうえで くらすのは いいんだけど\pときどき まどの そとから\nむしポケモンが はいって きたりして\lびっくり しちゃうんだよね"
	.string ""
	.string ""
	.string "$"

FortreeCity_Text_PokemonThatEvolveWhenTraded:
	.string "ともだちと こうかんを すると\nしんか する ポケモンが いるんだって!"
	.string "$"

FortreeCity_Text_SomethingUnseeable:
	.string "みえない なにかが いるようだ$"

FortreeCity_Text_UnseeableUseDevonScope:
	.string "みえない なにかが いるようだ\pデボンスコープを つかいますか?"
	.string "$"

FortreeCity_Text_UsedDevonScopePokemonFled:
	.string "{PLAYER}は\nデボンスコープを つかった!\pとうめいに なっていた\nポケモンの すがたが まるわかりだ!\pポケモンは\nおどろいて にげだした!"
	.string ""
	.string ""
	.string "$"

FortreeCity_Text_CitySign:
	.string "ここは ヒワマキ シティ\nきの うえで しぜんと たわむれる まち"
	.string ""
	.string "$"

FortreeCity_Text_GymSign:
	.string "ヒワマキ シティ ポケモンジム\nリーダー ナギ\lせかいに はばたく とりつかい!"
	.string ""
	.string ""
	.string "$"
