OneIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OneIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OneIsland_OnFrame
	.byte 0

OneIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ONE_ISLAND
	end

OneIsland_OnFrame::
	map_script_2 VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 2, OneIsland_EventScript_EnterOneIslandFirstTime
	.2byte 0

OneIsland_EventScript_EnterOneIslandFirstTime::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, OneIsland_Movement_PlayerExitHarbor
	waitmovement 0
	msgbox OneIsland_Text_BillLetsGoSeeCelio
	closemessage
	applymovement LOCALID_ONE_ISLAND_BILL, OneIsland_Movement_BillWalkToPokeCenter
	applymovement LOCALID_PLAYER, OneIsland_Movement_PlayerWalkToPokeCenter
	waitmovement 0
	opendoor 14, 5
	waitdooranim
	applymovement LOCALID_ONE_ISLAND_BILL, OneIsland_Movement_BillEnterPokeCenter
	applymovement LOCALID_PLAYER, OneIsland_Movement_PlayerEnterPokeCenter
	waitmovement 0
	closedoor 14, 5
	waitdooranim
	removeobject LOCALID_ONE_ISLAND_BILL
	setvar VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 3
	warp MAP_ONE_ISLAND_POKEMON_CENTER_1F, 9, 9
	waitstate
	releaseall
	end

	.macro walk_to_pokecenter
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	.endm

OneIsland_Movement_PlayerWalkToPokeCenter::
	walk_up
	walk_to_pokecenter
	step_end

OneIsland_Movement_PlayerEnterPokeCenter::
	walk_up
	walk_up
	set_invisible
	step_end

OneIsland_Movement_PlayerExitHarbor::
	walk_up
	step_end

OneIsland_Movement_BillWalkToPokeCenter::
	walk_to_pokecenter
	walk_up
	step_end

OneIsland_Movement_BillEnterPokeCenter::
	walk_up
	set_invisible
	step_end

OneIsland_EventScript_OldMan::
	lock
	faceplayer
	goto_if_set FLAG_IS_CHAMPION, OneIsland_EventScript_OldManLinkHoenn
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_EventScript_OldManLinkKanto
	msgbox OneIsland_Text_LuckyToHaveCelioHere
	release
	end

OneIsland_EventScript_OldManLinkKanto::
	msgbox OneIsland_Text_HavePCLinkageWithKanto
	release
	end

OneIsland_EventScript_OldManLinkHoenn::
	msgbox OneIsland_Text_HavePCLinkageWithHoenn
	release
	end

OneIsland_EventScript_BaldingMan::
	msgbox OneIsland_Text_IsntWarmClimateHereGreat, MSGBOX_NPC
	end

OneIsland_EventScript_IslandSign::
	msgbox OneIsland_Text_IslandSign, MSGBOX_SIGN
	end

OneIsland_EventScript_PokemonNetCenterSign::
	msgbox OneIsland_Text_PokemonNetCenterSign, MSGBOX_SIGN
	end

OneIsland_Text_BillLetsGoSeeCelio::
	.string "{JPN}マサキ『ここが 1のしまや!\p"
	.string "このへんには いくつか\n"
	.string "しまが あるんやけど\l"
	.string "まあ そのなかの ひとつや\p"
	.string "きょう ふねを よこしたのは\n"
	.string "しまの パソコンつうしんを\l"
	.string "ひとりで かんりしとる\l"
	.string "ニシキ っちゅう やつや!\p"
	.string "…ここで いってても しゃーない\n"
	.string "まずは ニシキに あいに いこか!$"

Text_WhereDoYouWantToSail::
	.string "{JPN}おつかれさん!\n"
	.string "どこに いきたいんだ?$"

OneIsland_Text_LuckyToHaveCelioHere::
	.string "{JPN}こんな いなかの しまにも\n"
	.string "ニシキくん という\l"
	.string "まじめな わかものが いるんじゃ!\p"
	.string "なにか… ポケモンマニア という\n"
	.string "えらい ひとと しりあいだとか\l"
	.string "マゴが さわいで おった\p"
	.string "わしには よう わからんが\n"
	.string "ニシキくんは えらい!$"

OneIsland_Text_HavePCLinkageWithKanto::
	.string "{JPN}なにか… カントーの ひととも\n"
	.string "パソコンが つながるとか いって\l"
	.string "マゴが さわいで おった\p"
	.string "わしには よう わからんが\n"
	.string "ニシキくんは えらい!$"

OneIsland_Text_HavePCLinkageWithHoenn::
	.string "{JPN}なにか… ついに\n"
	.string "ホウエンちほうの ひとと\l"
	.string "パソコンが つながった とか\l"
	.string "マゴが さわいで おった\p"
	.string "わしには よう わからんが\n"
	.string "ニシキくんは えらい!\l"
	.string "マゴの おむこに ほしいほどじゃ$"

OneIsland_Text_IslandSign::
	.string "{JPN}ここは 1のしま\n"
	.string "ごえんが あつまる むすびじま$"

OneIsland_Text_PokemonNetCenterSign::
	.string "{JPN}ひろがる ポケモン ワールド!\n"
	.string "ポケモンネットワークセンター$"

@ Unused
OneIsland_Text_ReturnToCinnabar::
	.string "グレンタウンに もどりますか?$"

OneIsland_Text_IsntWarmClimateHereGreat::
	.string "{JPN}やあ かんこう かい?\n"
	.string "ここいらは あったかくて いいだろ$"

