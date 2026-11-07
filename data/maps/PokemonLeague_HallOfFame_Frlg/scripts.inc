PokemonLeague_HallOfFame_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_HallOfFame_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_HallOfFame_OnWarp
	.byte 0

PokemonLeague_HallOfFame_OnWarp::
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_HallOfFame_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_HallOfFame_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_HallOfFame_OnFrame::
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_HallOfFame_EventScript_EnterRoom
	.2byte 0

PokemonLeague_HallOfFame_EventScript_EnterRoom::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, PokemonLeague_HallOfFame_Movement_EnterRoom
	waitmovement 0
	applymovement LOCALID_HALL_OF_FAME_PROF_OAK, Common_Movement_WalkInPlaceFasterLeft
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 18
	msgbox PokemonLeague_HallOfFame_Text_OakCongratulations
	closemessage
	applymovement LOCALID_HALL_OF_FAME_PROF_OAK, Common_Movement_WalkInPlaceFasterUp
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	delay 20
	dofieldeffect FLDEFF_HALL_OF_FAME_RECORD_FRLG
	waitfieldeffect FLDEFF_HALL_OF_FAME_RECORD_FRLG
	delay 40
	setvar VAR_TEMP_1, 1
	call EventScript_SetDefeatedEliteFourFlagsVars
	setrespawn HEAL_LOCATION_PALLET_TOWN
	fadescreenspeed FADE_TO_BLACK, 24
	special EnterHallOfFame
	waitstate
	releaseall
	end

PokemonLeague_HallOfFame_Movement_EnterRoom::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PokemonLeague_HallOfFame_Text_OakCongratulations::
	.string "{JPN}オーキド『…… おほんッ!\n"
	.string "おめでとう! {PLAYER}!\p"
	.string "ここは……\n"
	.string "れきだいの ポケモン リーグ\l"
	.string "トレーナーの もとで……\p"
	.string "かつやく した ポケモン たちを\n"
	.string "えいえんに きろく して\l"
	.string "たたえる フロア である!\p"
	.string "ポケモン トレーナーは ここに\n"
	.string "きろく される よろこびを\l"
	.string "でんどういり と よんで いる!\p"
	.string "{PLAYER}! おまえは\n"
	.string "はげしい たたかいの すえ\l"
	.string "リーグ チャンピオンと なった!\p"
	.string "ここに {PLAYER}の なまえと\n"
	.string "ポケモン たちを きろく しよう!$"

