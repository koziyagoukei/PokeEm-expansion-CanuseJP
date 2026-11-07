PokemonTower_2F_Frlg_MapScripts::
	.byte 0

PokemonTower_2F_EventScript_Channeler::
	msgbox PokemonTower_2F_Text_SilphScopeCouldUnmaskGhosts, MSGBOX_NPC
	end

PokemonTower_2F_EventScript_RivalTriggerRight::
	lockall
	setvar VAR_TEMP_1, 0
	goto PokemonTower_2F_EventScript_Rival
	end

PokemonTower_2F_EventScript_RivalTriggerDown::
	lockall
	setvar VAR_TEMP_1, 1
	goto PokemonTower_2F_EventScript_Rival
	end

PokemonTower_2F_EventScript_Rival::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	call_if_eq VAR_TEMP_1, 0, PokemonTower_2F_EventScript_RivalFacePlayerRight
	call_if_eq VAR_TEMP_1, 1, PokemonTower_2F_EventScript_RivalFacePlayerDown
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_Delay48
	waitmovement 0
	msgbox PokemonTower_2F_Text_RivalIntro
	setvar VAR_LAST_TALKED, LOCALID_POKEMON_TOWER_RIVAL
	call_if_eq VAR_STARTER_MON, 2, PokemonTower_2F_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, PokemonTower_2F_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, PokemonTower_2F_EventScript_RivalCharmander
	msgbox PokemonTower_2F_Text_RivalPostBattle
	closemessage
	playbgm MUS_RG_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, PokemonTower_2F_EventScript_RivalExitRight
	call_if_eq VAR_TEMP_1, 1, PokemonTower_2F_EventScript_RivalExitDown
	playse SE_EXIT
	delay 25
	fadedefaultbgm
	removeobject LOCALID_POKEMON_TOWER_RIVAL
	setvar VAR_MAP_SCENE_POKEMON_TOWER_2F, 1
	releaseall
	end

PokemonTower_2F_EventScript_RivalFacePlayerRight::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_WalkInPlaceFasterRight
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PokemonTower_2F_EventScript_RivalFacePlayerDown::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	return

PokemonTower_2F_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_POKEMON_TOWER_SQUIRTLE, PokemonTower_2F_Text_RivalDefeat
	return

PokemonTower_2F_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_POKEMON_TOWER_BULBASAUR, PokemonTower_2F_Text_RivalDefeat
	return

PokemonTower_2F_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_POKEMON_TOWER_CHARMANDER, PokemonTower_2F_Text_RivalDefeat
	return

PokemonTower_2F_EventScript_RivalExitRight::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, PokemonTower_2F_Movement_RivalExitRight
	waitmovement 0
	return

PokemonTower_2F_EventScript_RivalExitDown::
	applymovement LOCALID_POKEMON_TOWER_RIVAL, PokemonTower_2F_Movement_RivalExitDown
	waitmovement 0
	return

PokemonTower_2F_Movement_RivalExitRight::
	walk_down
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PokemonTower_2F_Movement_RivalExitDown::
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_in_place_faster_right
	step_end

PokemonTower_2F_Text_RivalIntro::
	.string "{JPN}{RIVAL}『おう! {PLAYER}!\n"
	.string "こんな ところへ\l"
	.string "なにしに きたんだよ?\l"
	.string "おまえの ポケモン しんだのか?\l"
	.string "……あほか! いきてる じゃん\p"
	.string "だったら せめて\n"
	.string "せんとう ふのうに してやるか!\l"
	.string "かかって こいよ!$"

PokemonTower_2F_Text_RivalDefeat::
	.string "{JPN}あ! ちくしょう!\n"
	.string "やりやがったなー!\p"
	.string "せっかく てかげん してやったのに$"

PokemonTower_2F_Text_RivalPostBattle::
	.string "{JPN}おい ところで…!\l"
	.string "ポケモンずかんは どうだよ?\l"
	.string "おれなんか カラカラ\l"
	.string "みつけた もんね!\p"
	.string "おっきい ほうの ガラガラが\n"
	.string "みつからねえんだ!\l"
	.string "どこかなー?\p"
	.string "ああー きっと\n"
	.string "もう このへんには いないな!\l"
	.string "じゃ おれ もう いくわ!\l"
	.string "おまえと ちがって\l"
	.string "おれ いそがしいからよ!\p"
	.string "じゃーな!$"

PokemonTower_2F_Text_SilphScopeCouldUnmaskGhosts::
	.string "{JPN}おぬし! まよい さまよう\n"
	.string "ゆうれいの しょうたいは…\l"
	.string "われわれでも つかめない!\p"
	.string "もし とくべつな どうぐが…\n"
	.string "シルフスコープさえ あれば\l"
	.string "みやぶれるかも しれないが…$"

