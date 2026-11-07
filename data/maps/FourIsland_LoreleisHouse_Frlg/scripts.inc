FourIsland_LoreleisHouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FourIsland_LoreleisHouse_OnTransition
	.byte 0

FourIsland_LoreleisHouse_OnTransition::
	special UpdateLoreleiDollCollection
	end

FourIsland_LoreleisHouse_EventScript_Lorelei::
	lock
	faceplayer
	goto_if_set FLAG_TALKED_TO_LORELEI_AFTER_WAREHOUSE, FourIsland_LoreleisHouse_EventScript_Lorelei3
	goto_if_set FLAG_DEFEATED_ROCKETS_IN_WAREHOUSE, FourIsland_LoreleisHouse_EventScript_Lorelei2
	msgbox FourIsland_LoreleisHouse_Text_IfAnythingWereToHappenToIsland
	release
	end

FourIsland_LoreleisHouse_EventScript_Lorelei2::
	setflag FLAG_TALKED_TO_LORELEI_AFTER_WAREHOUSE
	msgbox FourIsland_LoreleisHouse_Text_IllReturnToLeagueInShortWhile
	release
	end

FourIsland_LoreleisHouse_EventScript_Lorelei3::
	msgbox FourIsland_LoreleisHouse_Text_WillDoWhatICanHereAndNow
	release
	end

FourIsland_LoreleisHouse_EventScript_Doll::
	lock
	msgbox FourIsland_LoreleisHouse_Text_StuffedMonDollsGalore
	release
	end

FourIsland_LoreleisHouse_Text_IfAnythingWereToHappenToIsland::
	.string "{JPN}カンナ『いま\n"
	.string "なやんで いるの\p"
	.string "じぶんが そだった しまで\n"
	.string "なにか あっても\l"
	.string "ポケモンリーグに いては\l"
	.string "きづく ことが できない\p"
	.string "それは むせきにんな\n"
	.string "ことでは ないかって……$"

FourIsland_LoreleisHouse_Text_IllReturnToLeagueInShortWhile::
	.string "{JPN}カンナ『あなたが すべて\n"
	.string "かいけつ したのね…?\p"
	.string "そう よね\n"
	.string "わたしが どうしても\l"
	.string "ここに いなくては ならない\l"
	.string "という わけでは\l"
	.string "なかったの よね\p"
	.string "ありがとう…\n"
	.string "もう しばらく したら\l"
	.string "ポケモンリーグに かえります$"

FourIsland_LoreleisHouse_Text_WillDoWhatICanHereAndNow::
	.string "{JPN}この さき\n"
	.string "どうなるかは わからない けど…\p"
	.string "いまはただ わたし だけに\n"
	.string "できることを するわ$"

FourIsland_LoreleisHouse_Text_StuffedMonDollsGalore::
	.string "{JPN}ポケモン ぬいぐるみが ある!$"

