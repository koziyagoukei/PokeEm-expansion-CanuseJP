MauvilleCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, MauvilleCity_Gym_OnLoad
	.byte 0

MauvilleCity_Gym_OnLoad:
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_DeactivatePuzzle
	switch VAR_MAUVILLE_GYM_STATE
	case 0, MauvilleCity_Gym_EventScript_UpdateBarriers
	case 1, MauvilleCity_Gym_EventScript_Switch1Pressed
	case 2, MauvilleCity_Gym_EventScript_Switch2Pressed
	case 3, MauvilleCity_Gym_EventScript_Switch3Pressed
	case 4, MauvilleCity_Gym_EventScript_Switch4Pressed
	end

MauvilleCity_Gym_EventScript_UpdateBarriers::
	goto_if_set FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_SetAltBarriers
	end

MauvilleCity_Gym_EventScript_SetAltBarriers::
	setmetatile 3, 11, METATILE_MauvilleGym_RedBeamV1_On, TRUE
	setmetatile 3, 12, METATILE_MauvilleGym_RedBeamV2_On, TRUE
	setmetatile 3, 13, METATILE_MauvilleGym_PoleTop_On, TRUE
	setmetatile 4, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 5, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 4, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 5, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 7, 10, METATILE_MauvilleGym_RedBeamH1_On, FALSE
	setmetatile 8, 10, METATILE_MauvilleGym_RedBeamH2_On, FALSE
	setmetatile 7, 11, METATILE_MauvilleGym_RedBeamH3_On, TRUE
	setmetatile 8, 11, METATILE_MauvilleGym_RedBeamH4_On, TRUE
	setmetatile 4, 13, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 13, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 14, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 14, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 1, 10, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 2, 10, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 1, 11, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 2, 11, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	setmetatile 6, 8, METATILE_MauvilleGym_PoleBottom_On, TRUE
	setmetatile 6, 9, METATILE_MauvilleGym_FloorTile, FALSE
	setmetatile 6, 10, METATILE_MauvilleGym_PoleTop_Off, FALSE
	setmetatile 4, 6, METATILE_MauvilleGym_GreenBeamH1_Off, FALSE
	setmetatile 5, 6, METATILE_MauvilleGym_GreenBeamH2_Off, FALSE
	setmetatile 4, 7, METATILE_MauvilleGym_GreenBeamH3_Off, FALSE
	setmetatile 5, 7, METATILE_MauvilleGym_GreenBeamH4_Off, FALSE
	end

MauvilleCity_Gym_EventScript_Switch1Pressed::
	setvar VAR_0x8004, 0
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch2Pressed::
	setvar VAR_0x8004, 1
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch3Pressed::
	setvar VAR_0x8004, 2
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_Switch4Pressed::
	setvar VAR_0x8004, 3
	special MauvilleGymPressSwitch
	goto MauvilleCity_Gym_EventScript_UpdateBarriers
	end

MauvilleCity_Gym_EventScript_DeactivatePuzzle::
	special MauvilleGymDeactivatePuzzle
	end

MauvilleCity_Gym_EventScript_Wattson::
	trainerbattle_single TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonIntro, MauvilleCity_Gym_Text_WattsonDefeat, MauvilleCity_Gym_EventScript_WattsonDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MauvilleCity_Gym_EventScript_WattsonRematch
	goto_if_unset FLAG_RECEIVED_TM_SHOCK_WAVE, MauvilleCity_Gym_EventScript_GiveShockWave2
	goto_if_eq VAR_NEW_MAUVILLE_STATE, 2, MauvilleCity_Gym_EventScript_CompletedNewMauville
	msgbox MauvilleCity_Gym_Text_WattsonPostBattle, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_WattsonDefeated::
	message MauvilleCity_Gym_Text_ReceivedDynamoBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox MauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis, MSGBOX_DEFAULT
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 3
	clearflag FLAG_HIDE_VERDANTURF_TOWN_SCOTT
	setflag FLAG_DEFEATED_MAUVILLE_GYM
	setflag FLAG_BADGE03_GET
	addvar VAR_PETALBURG_GYM_STATE, 1
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	setvar VAR_0x8008, 3
	call Common_EventScript_SetGymTrainers
	special MauvilleGymDeactivatePuzzle
	special DrawWholeMapView
	playse SE_UNLOCK
	call MauvilleCity_Gym_EventScript_GiveShockWave
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox MauvilleCity_Gym_Text_RegisteredWattson, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WATTSON_MATCH_CALL
	release
	end

MauvilleCity_Gym_EventScript_GiveShockWave2::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	release
	end

MauvilleCity_Gym_EventScript_GiveShockWave::
	giveitem ITEM_TM_SHOCK_WAVE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox MauvilleCity_Gym_Text_ExplainShockWave, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_SHOCK_WAVE
	return

MauvilleCity_Gym_EventScript_CompletedNewMauville::
	msgbox MauvilleCity_Gym_Text_WattsonGoForthAndEndeavor, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_WattsonRematch::
	trainerbattle_rematch_double TRAINER_WATTSON_1, MauvilleCity_Gym_Text_WattsonPreRematch, MauvilleCity_Gym_Text_WattsonRematchDefeat, MauvilleCity_Gym_Text_WattsonRematchNeedTwoMons
	msgbox MauvilleCity_Gym_Text_WattsonPostRematch, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Switch1::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 1, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 1
	setvar VAR_0x8004, 0
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch2::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 2, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 2
	setvar VAR_0x8004, 1
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch3::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 3, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 3
	setvar VAR_0x8004, 2
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_Switch4::
	lockall
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_SwitchDoNothing
	goto_if_eq VAR_MAUVILLE_GYM_STATE, 4, MauvilleCity_Gym_EventScript_SwitchDoNothing
	setvar VAR_MAUVILLE_GYM_STATE, 4
	setvar VAR_0x8004, 3
	goto MauvilleCity_Gym_EventScript_PressFloorSwitch
	end

MauvilleCity_Gym_EventScript_PressFloorSwitch::
	special MauvilleGymSetDefaultBarriers
	special MauvilleGymPressSwitch
	special DrawWholeMapView
	playse SE_UNLOCK
	goto_if_set FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_ClearBarriersAltState
	goto_if_unset FLAG_MAUVILLE_GYM_BARRIERS_STATE, MauvilleCity_Gym_EventScript_SetBarriersAltState
	releaseall
	end

MauvilleCity_Gym_EventScript_SwitchDoNothing::
	releaseall
	end

MauvilleCity_Gym_EventScript_SetBarriersAltState::
	setflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

MauvilleCity_Gym_EventScript_ClearBarriersAltState::
	clearflag FLAG_MAUVILLE_GYM_BARRIERS_STATE
	releaseall
	end

MauvilleCity_Gym_EventScript_Kirk::
	trainerbattle_single TRAINER_KIRK, MauvilleCity_Gym_Text_KirkIntro, MauvilleCity_Gym_Text_KirkDefeat
	msgbox MauvilleCity_Gym_Text_KirkPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Shawn::
	trainerbattle_single TRAINER_SHAWN, MauvilleCity_Gym_Text_ShawnIntro, MauvilleCity_Gym_Text_ShawnDefeat
	msgbox MauvilleCity_Gym_Text_ShawnPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Ben::
	trainerbattle_single TRAINER_BEN, MauvilleCity_Gym_Text_BenIntro, MauvilleCity_Gym_Text_BenDefeat
	msgbox MauvilleCity_Gym_Text_BenPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Vivian::
	trainerbattle_single TRAINER_VIVIAN, MauvilleCity_Gym_Text_VivianIntro, MauvilleCity_Gym_Text_VivianDefeat
	msgbox MauvilleCity_Gym_Text_VivianPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_Angelo::
	trainerbattle_single TRAINER_ANGELO, MauvilleCity_Gym_Text_AngeloIntro, MauvilleCity_Gym_Text_AngeloDefeat
	msgbox MauvilleCity_Gym_Text_AngeloPostBattle, MSGBOX_AUTOCLOSE
	end

MauvilleCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MAUVILLE_GYM, MauvilleCity_Gym_EventScript_GymGuidePostVictory
	msgbox MauvilleCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_GymGuidePostVictory::
	msgbox MauvilleCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

MauvilleCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE03_GET, MauvilleCity_Gym_EventScript_GymStatueCertified
	goto MauvilleCity_Gym_EventScript_GymStatue
	end

MauvilleCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE03_GET, MauvilleCity_Gym_EventScript_GymStatueCertified
	goto MauvilleCity_Gym_EventScript_GymStatue
	end

MauvilleCity_Gym_EventScript_GymStatueCertified::
	msgbox MauvilleCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_Gym_EventScript_GymStatue::
	msgbox MauvilleCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

MauvilleCity_Gym_Text_GymGuideAdvice:
	.string "{JPN}げんき しとぉや!!\nチャンピオンを めざす {PLAYER}!!\pキンセツシティの ジムリーダー テッセンは\nでんきタイプ ポケモンの つかいてだ!\pみずタイプの ポケモンで むかうと\nビリビリ しびれさせられるぞ!\pしかも スイッチで あけたり する\nけったいな とびらを\lジムに つくって しまったんだ!\pきばってこー な!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_GymGuidePostVictory:
	.string "{JPN}こりゃ びっくり たまげたッ!\nきみの おかげで とびらは ひらかれた!"
	.string "$"

MauvilleCity_Gym_Text_KirkIntro:
	.string "{JPN}しびれる オレの ハートが\nきみの やぼうを ふんさい するっ!"
	.string "$"

MauvilleCity_Gym_Text_KirkDefeat:
	.string "{JPN}しびれる たたかい だったぜ……$"

MauvilleCity_Gym_Text_KirkPostBattle:
	.string "{JPN}ポケモンも ロックも ハートが すべて\nいぇーい!"
	.string "$"

MauvilleCity_Gym_Text_ShawnIntro:
	.string "{JPN}テッセンさんに きたえられた この オレが\nそう かんたんに まける はずがねえ!"
	.string "$"

MauvilleCity_Gym_Text_ShawnDefeat:
	.string "{JPN}キュ キュイィィィン……$"

MauvilleCity_Gym_Text_ShawnPostBattle:
	.string "{JPN}このジムの リーダー テッセンさんは\nきみの おやじが うまれる まえから\lポケモンと たたかってきた つわものさ!"
	.string ""
	.string "$"

MauvilleCity_Gym_Text_BenIntro:
	.string "{JPN}この ポケモンジムは\nしかけが あって たのしいな!"
	.string "$"

MauvilleCity_Gym_Text_BenDefeat:
	.string "{JPN}まけると たのしくないよ$"

MauvilleCity_Gym_Text_BenPostBattle:
	.string "{JPN}テッセンさんは スイッチを つかった\nしかけを つくるのが しゅみ なんだって!"
	.string "$"

MauvilleCity_Gym_Text_VivianIntro:
	.string "{JPN}あたしの みりょくと\nポケモンの わざで しびれさせちゃう!"
	.string "$"

MauvilleCity_Gym_Text_VivianDefeat:
	.string "{JPN}あなたの つよさに しびれたわ!$"

MauvilleCity_Gym_Text_VivianPostBattle:
	.string "{JPN}この キンセツシティは\nテッセンさんが つくったって はなし\pあなたや わたしが\nうまれる まえからの トレーナーだもの\lきっと いろんな ことを しってるわ"
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_AngeloIntro:
	.string "{JPN}ぼく ひかるもの だーいすき なんだ!$"

MauvilleCity_Gym_Text_AngeloDefeat:
	.string "{JPN}ああ…\nめのまえが チカ チカ"
	.string "$"

MauvilleCity_Gym_Text_AngeloPostBattle:
	.string "{JPN}キンセツジムも テッセンさんの おデコも\nピカピカ ひかってて うれしいな!"
	.string "$"

MauvilleCity_Gym_Text_WattsonIntro:
	.string "{JPN}まちを かいぞうする けいかくは\nあきらめた わし じゃが\pいまは このジムの とびらの しかけを\nつくることに うちこんで おるのじゃ!\p……あれ? おまえさん\nなんで わしの まえに いるんじゃ?\pなにー! とびらの しかけを\nくぐりぬけて きたと いうのかっ!\pわっはははは! こりゃ おもしろい!\pならば この キンセツ ポケモン ジムの\nリーダー テッセンが\lポケモンで しびれさせて やるぞい!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_WattsonDefeat:
	.string "{JPN}わっはははは! わしの まけじゃ!\nおまえさんには しびれた わい\lこの バッジ もっていくが いいぞ!"
	.string ""
	.string "$"

MauvilleCity_Gym_Text_ReceivedDynamoBadge:
	.string "{JPN}{PLAYER}は テッセンから\nダイナモバッジを もらった!"
	.string "$"

MauvilleCity_Gym_Text_ExplainDynamoBadgeTakeThis:
	.string "{JPN}ダイナモバッジを もっておると\nひでんの いわくだきが\lつかえるように なるんじゃ!\pそれに おまえさんの ポケモン\nいっそう すばやく なるぞい!\pうむ……\nこれも もっていくが いいぞ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_ExplainShockWave:
	.string "{JPN}その わざマシン34 には\nでんげきは が はいっておる!\pかならず めいちゅう する\nたのもしい わざ なんじゃよ!\p…… …… ……"
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_RegisteredWattson:
	.string "{JPN}ジムリーダー テッセンを\nポケナビに とうろく した!"
	.string "$"

MauvilleCity_Gym_Text_WattsonPostBattle:
	.string "{JPN}みどころの ある わかものの しゅつげん\nじつに たのもしい ことじゃ!"
	.string "$"

MauvilleCity_Gym_Text_WattsonGoForthAndEndeavor:
	.string "{JPN}わっはははは!\nわかものよ もっともっと がんばるんじゃ!"
	.string "$"

MauvilleCity_Gym_Text_GymStatue:
	.string "{JPN}キンセツシティ ポケモンジム$"

MauvilleCity_Gym_Text_GymStatueCertified:
	.string "{JPN}キンセツシティ ポケモンジム\pテッセン にんてい トレーナー!\n{PLAYER}"
	.string ""
	.string "$"

MauvilleCity_Gym_Text_WattsonPreRematch:
	.string "{JPN}テッセン『おーっと あらわれたな!\nわかっとる! わしと ポケモンの\lしょうぶを したいんじゃろう!?\pわっはははは!\pひばな バチバチ いわせて やるから\nかくご するんじゃぞ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_WattsonRematchDefeat:
	.string "{JPN}ふぅ バッテリーぎれ じゃ……$"

MauvilleCity_Gym_Text_WattsonPostRematch:
	.string "{JPN}テッセン『わしゃ じゅうでんの しなおしだ\pたいせいを たてなおしたら\nまた きみの ちょうせんを うけるから\lしばらく してから きておくれ……"
	.string ""
	.string ""
	.string "$"

MauvilleCity_Gym_Text_WattsonRematchNeedTwoMons:
	.string "{JPN}テッセン『おーっと あらわれたな!\nわかっとる! わしと ポケモンの\lしょうぶを したいんじゃろう!?\pわっはははは!\p……ありゃ? たたかえる ポケモンを\n1ひきしか もっておらん ようだな\pすまんが 2ひき いじょうの\nポケモンを もって きておくれ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"
