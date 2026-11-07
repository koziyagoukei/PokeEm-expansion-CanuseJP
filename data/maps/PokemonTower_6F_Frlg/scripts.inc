PokemonTower_6F_Frlg_MapScripts::
	.byte 0

PokemonTower_6F_EventScript_MarowakGhost::
	lockall
	textcolor NPC_TEXT_COLOR_MON
	msgbox PokemonTower_6F_Text_BeGoneIntruders
	setwildbattle SPECIES_MAROWAK, 30
	special StartMarowakBattle
	waitstate
	goto_if_eq VAR_RESULT, FALSE, PokemonTower_6F_EventScript_DefeatedMarowakGhost  @ VAR_RESULT set by CB2_EndMarowakBattle
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerUp
	waitmovement 0
	releaseall
	end

PokemonTower_6F_EventScript_DefeatedMarowakGhost::
	message PokemonTower_6F_Text_GhostWasCubonesMother
	waitmessage
	waitse
	playmoncry SPECIES_MAROWAK, CRY_MODE_NORMAL
	waitbuttonpress
	waitmoncry
	msgbox PokemonTower_6F_Text_MothersSpiritWasCalmed
	setvar VAR_MAP_SCENE_POKEMON_TOWER_6F, 1
	releaseall
	end

PokemonTower_6F_Movement_ForcePlayerUp::
	walk_up
	step_end

PokemonTower_6F_EventScript_Angelica::
	trainerbattle_single TRAINER_CHANNELER_ANGELICA, PokemonTower_6F_Text_AngelicaIntro, PokemonTower_6F_Text_AngelicaDefeat
	msgbox PokemonTower_6F_Text_AngelicaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Emilia::
	trainerbattle_single TRAINER_CHANNELER_EMILIA, PokemonTower_6F_Text_EmiliaIntro, PokemonTower_6F_Text_EmiliaDefeat
	msgbox PokemonTower_6F_Text_EmiliaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Jennifer::
	trainerbattle_single TRAINER_CHANNELER_JENNIFER, PokemonTower_6F_Text_JenniferIntro, PokemonTower_6F_Text_JenniferDefeat
	msgbox PokemonTower_6F_Text_JenniferPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_Text_AngelicaIntro::
	.string "{JPN}… チ… ニ… ウエッ!\n"
	.string "… テ… ルッ…!$"

PokemonTower_6F_Text_AngelicaDefeat::
	.string "{JPN}…ううーむ$"

PokemonTower_6F_Text_AngelicaPostBattle::
	.string "{JPN}ああ…! クラクラ する!\n"
	.string "ひんけつ かも しれん…$"

PokemonTower_6F_Text_EmiliaIntro::
	.string "{JPN}… カッ… ナ… シッ!\n"
	.string "… バ… リッ!$"

PokemonTower_6F_Text_EmiliaDefeat::
	.string "{JPN}…なにかが\n"
	.string "わしの からだから ぬけてく$"

PokemonTower_6F_Text_EmiliaPostBattle::
	.string "{JPN}わしの からだ から ぬけたもの…\n"
	.string "それは かみのけ では ない!\l"
	.string "あくりょう じゃ$"

PokemonTower_6F_Text_JenniferIntro::
	.string "{JPN}…ヒヒ… ヒ… ヒッ!\n"
	.string "… ヒ… ヒヒ…ヒヒッ!$"

PokemonTower_6F_Text_JenniferDefeat::
	.string "{JPN}ひいーっ!$"

PokemonTower_6F_Text_JenniferPostBattle::
	.string "{JPN}…ヒ ヒヒ ぃっーくしょん!\n"
	.string "あれ わしは どうしてたんじゃ?$"

PokemonTower_6F_Text_BeGoneIntruders::
	.string "{JPN}… タチサレ!\n"
	.string "… ココカラ タチサレ…$"

PokemonTower_6F_Text_GhostWasCubonesMother::
	.string "{JPN}ゆうれいの しょうたいは…\p"
	.string "カラカラの おかあさんの\n"
	.string "まよえる たましい だった!$"

PokemonTower_6F_Text_MothersSpiritWasCalmed::
	.string "{JPN}やさしい カラカラの おかあさんに\n"
	.string "もどった たましいは\l"
	.string "ぶじ てんに のぼって…\p"
	.string "… …きえて いきました$"

