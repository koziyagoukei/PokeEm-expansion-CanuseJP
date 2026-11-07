CinnabarIsland_PokemonLab_ResearchRoom_Frlg_MapScripts::
	.byte 0

CinnabarIsland_PokemonLab_ResearchRoom_EventScript_MetronomeTutor::
	goto EventScript_MetronomeTutor
	end

CinnabarIsland_PokemonLab_ResearchRoom_EventScript_Scientist::
	msgbox CinnabarIsland_PokemonLab_ResearchRoom_Text_EeveeCanEvolveIntroThreeMons, MSGBOX_NPC
	end

CinnabarIsland_PokemonLab_ResearchRoom_EventScript_Computer::
	msgbox CinnabarIsland_PokemonLab_ResearchRoom_Text_LegendaryBirdEmail, MSGBOX_SIGN
	end

CinnabarIsland_PokemonLab_ResearchRoom_EventScript_AmberPipe::
	msgbox CinnabarIsland_PokemonLab_ResearchRoom_Text_AnAmberPipe, MSGBOX_SIGN
	end

CinnabarIsland_PokemonLab_ResearchRoom_Text_EeveeCanEvolveIntroThreeMons::
	.string "{JPN}そう! イーブイは\n"
	.string "3しゅるいの ポケモンに\l"
	.string "しんか する かのうせいが ある!$"

CinnabarIsland_PokemonLab_ResearchRoom_Text_LegendaryBirdEmail::
	.string "{JPN}パソコンに\n"
	.string "でんしメールが とどいてる!\p"
	.string "… … …\p"
	.string "でんせつの とりポケモンは\n"
	.string "3しゅるい… ファイヤー\l"
	.string "サンダー フリーザー…\l"
	.string "しかし どこに いるのか\l"
	.string "まだ わかって ません\p"
	.string "こんど ハナダ どうくつに\n"
	.string "ちょうさに いきます\l"
	.string "… まずは ほうこく まで\l"
	.string "ポケモン ちょうさ たい\p"
	.string "… … …$"

CinnabarIsland_PokemonLab_ResearchRoom_Text_AnAmberPipe::
	.string "{JPN}コハクの パイプが ある!$"

