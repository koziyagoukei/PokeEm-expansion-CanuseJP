BattleFrontier_OutsideEast_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, BattleFrontier_OutsideEast_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, BattleFrontier_OutsideEast_OnTransition
	.byte 0

BattleFrontier_OutsideEast_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_TryRemoveSudowoodo::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

BattleFrontier_OutsideEast_OnTransition:
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, 0
	setflag FLAG_HIDE_BATTLE_TOWER_REPORTER
	special FrontierStory_SetupNamedTalks
	call_if_unset FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_ShowSudowoodo
	end

BattleFrontier_OutsideEast_EventScript_ShowSudowoodo::
	clearflag FLAG_HIDE_BATTLE_FRONTIER_SUDOWOODO
	return

BattleFrontier_OutsideEast_EventScript_BattleTowerSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleTowerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePalaceSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePalaceSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattleArenaSign::
	msgbox BattleFrontier_OutsideEast_Text_BattleArenaSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_BattlePyramidSign::
	msgbox BattleFrontier_OutsideEast_Text_BattlePyramidSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_Cook::
	lock
	setvar VAR_0x8004, REGULAR_EAST_COOK
	special FrontierStory_BufferRegularTalk
	msgbox gStringVar4, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_Zigzagoon::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_ZIGZAGOON, CRY_MODE_NORMAL
	msgbox BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant, MSGBOX_DEFAULT
	waitmoncry
	release
	end

BattleFrontier_OutsideEast_EventScript_Twin::
	setvar VAR_0x8004, REGULAR_EAST_TWIN
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_OutsideEast_EventScript_Man2::
	setvar VAR_0x8004, REGULAR_EAST_TWIN_DAD
	goto FrontierStory_EventScript_RegularAside

BattleFrontier_OutsideEast_EventScript_Sudowoodo::
	lock
	faceplayer
	waitse
	playse SE_SUDOWOODO_SHAKE
	applymovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	waitmovement 0
	release
	end

BattleFrontier_OutsideEast_EventScript_WaterSudowoodo::
	lock
	faceplayer
	special DoWateringBerryTreeAnim
	waitse
	playse SE_SUDOWOODO_SHAKE
	applymovement LOCALID_FRONTIER_SUDOWOODO, BattleFrontier_OutsideEast_Movement_SudowoodoShake
	waitmovement 0
	msgbox gText_Sudowoodo_Attacked, MSGBOX_DEFAULT
	closemessage
	waitse
	playmoncry SPECIES_SUDOWOODO, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setvar VAR_LAST_TALKED, LOCALID_FRONTIER_SUDOWOODO
	setwildbattle SPECIES_SUDOWOODO, 40
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo
	setflag FLAG_DEFEATED_SUDOWOODO
	release
	end

BattleFrontier_OutsideEast_EventScript_DefeatedSudowoodo::
	setflag FLAG_DEFEATED_SUDOWOODO
	goto Common_EventScript_RemoveStaticPokemon
	end

BattleFrontier_OutsideEast_Movement_SudowoodoShake:
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	delay_8
	face_right
	delay_8
	face_down
	delay_8
	face_left
	delay_8
	face_down
	step_end

BattleFrontier_OutsideEast_EventScript_Man3::
	setvar VAR_0x8004, REGULAR_EAST_FAN
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_OutsideEast_EventScript_Woman1::
	setvar VAR_0x8004, REGULAR_EAST_FAN_PARTNER
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_OutsideEast_EventScript_Maniac2::
	setvar VAR_0x8004, REGULAR_EAST_MULTI_MANIAC
	goto FrontierStory_EventScript_RegularAside

BattleFrontier_OutsideEast_EventScript_Woman3::
	setvar VAR_0x8004, REGULAR_EAST_MULTI_WOMAN
	goto FrontierStory_EventScript_RegularAside

BattleFrontier_OutsideEast_EventScript_RankingHallSign::
	msgbox BattleFrontier_OutsideEast_Text_RankingHallSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_ExchangeCornerSign::
	msgbox BattleFrontier_OutsideEast_Text_ExchangeCornerSign, MSGBOX_SIGN
	end

BattleFrontier_OutsideEast_EventScript_OldWoman::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SUDOWOODO, BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone
	msgbox BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_EventScript_OldWomanSudowoodoGone::
	msgbox BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere, MSGBOX_DEFAULT
	release
	end

BattleFrontier_OutsideEast_Text_BattleTowerSign:
	.string "ここは バトルタワー!\n勝ちぬけ! 最強 トレーナー!$"

BattleFrontier_OutsideEast_Text_BattlePalaceSign:
	.string "ここは バトルパレス!\nみまもれ! ポケモンバトル!$"

BattleFrontier_OutsideEast_Text_BattleArenaSign:
	.string "ここは バトルアリーナ!\nつどえ! 最強 チーム!$"

BattleFrontier_OutsideEast_Text_BattlePyramidSign:
	.string "ここは バトルピラミッド!\n進め! バトルアドベンチャー!$"

BattleFrontier_OutsideEast_Text_RankingHallSign:
	.string "バトルフロンティア ランキングホール\nきざめ! 最高の 記録!$"

BattleFrontier_OutsideEast_Text_ExchangeCornerSign:
	.string "バトルポイント 引き換えサービス コーナー\n"
	.string "引き換えろ! キミの バトルポイント!$"

BattleFrontier_OutsideEast_Text_ZigzagoonLooksVacant:
	.string "{JPN}ジグザグマは のんびりしている。\nだれかの おやつの 時間を\l待っているようだ。$"

BattleFrontier_OutsideEast_Text_OddTreeHereSeemsToWiggle:
	.string "{JPN}あの きが ゆれるのを 見ると\nなんだか わしまで むずむずするんじゃ。\pおまえさんも ちょいと\n見ていって 遅れ。$"

BattleFrontier_OutsideEast_Text_OnceAnOddTreePastHere:
	.string "{JPN}あそこに へんな きが あったじゃろ。\n今も そこに いる きぶんになるのう。\pおや きみも 覚えているかい？$"

