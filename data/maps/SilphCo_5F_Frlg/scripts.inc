SilphCo_5F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_5F_OnLoad
	.byte 0

SilphCo_5F_OnLoad::
	call_if_unset FLAG_SILPH_5F_DOOR_1, EventScript_Close5FDoor1
	call_if_unset FLAG_SILPH_5F_DOOR_2, EventScript_Close5FDoor2
	call_if_unset FLAG_SILPH_5F_DOOR_3, EventScript_Close5FDoor3
	end

SilphCo_5F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_5F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_5F_Text_RocketsInUproarAboutIntruder
	release
	end

SilphCo_5F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_5F_Text_YoureOurHeroThankYou
	release
	end

SilphCo_5F_EventScript_PokemonReport1::
	msgbox SilphCo_5F_Text_PorygonFirstVRMon, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_PokemonReport2::
	msgbox SilphCo_5F_Text_Over350TechniquesConfirmed, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_PokemonReport3::
	msgbox SilphCo_5F_Text_SomeMonsEvolveWhenTraded, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_FloorSign::
	msgbox SilphCo_5F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_Beau::
	trainerbattle_single TRAINER_SCIENTIST_BEAU, SilphCo_5F_Text_BeauIntro, SilphCo_5F_Text_BeauDefeat
	msgbox SilphCo_5F_Text_BeauPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_28, SilphCo_5F_Text_Grunt1Intro, SilphCo_5F_Text_Grunt1Defeat
	msgbox SilphCo_5F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_29, SilphCo_5F_Text_Grunt2Intro, SilphCo_5F_Text_Grunt2Defeat
	msgbox SilphCo_5F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_EventScript_Dalton::
	trainerbattle_single TRAINER_JUGGLER_DALTON, SilphCo_5F_Text_DaltonIntro, SilphCo_5F_Text_DaltonDefeat
	msgbox SilphCo_5F_Text_DaltonPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_Text_RocketsInUproarAboutIntruder::
	.string "{JPN}だれか ビルに しのび こんだと\n"
	.string "ロケットだんが さわいでる\l"
	.string "… きみの こと らしいね$"

SilphCo_5F_Text_YoureOurHeroThankYou::
	.string "{JPN}ロケットだんは にげたよ!\n"
	.string "きみが おっぱらったのかい\l"
	.string "… ありがとう!$"

SilphCo_5F_Text_Grunt1Intro::
	.string "{JPN}こどもが うろついてると\n"
	.string "さっき れんらくが あったぜ!$"

SilphCo_5F_Text_Grunt1Defeat::
	.string "{JPN}ばったり…$"

SilphCo_5F_Text_Grunt1PostBattle::
	.string "{JPN}ロケットだんに\n"
	.string "たてつかない ほうが\l"
	.string "みのため だぜ$"

SilphCo_5F_Text_BeauIntro::
	.string "{JPN}この フロアは\n"
	.string "モンスターボールの\l"
	.string "けんきゅう セクションだ!$"

SilphCo_5F_Text_BeauDefeat::
	.string "{JPN}ちくしょう!$"

SilphCo_5F_Text_BeauPostBattle::
	.string "{JPN}ここでは…\n"
	.string "でんせつの ポケモンさえ とれる\l"
	.string "きゅうきょくの モンスターボールを\l"
	.string "けんきゅう してたのだ$"

SilphCo_5F_Text_DaltonIntro::
	.string "{JPN}おかしいな?\n"
	.string "こどもが はいって これる\l"
	.string "わけが ないんだが$"

SilphCo_5F_Text_DaltonDefeat::
	.string "{JPN}こりゃ いかん$"

SilphCo_5F_Text_DaltonPostBattle::
	.string "{JPN}ここは 5かい…\n"
	.string "ボスの いる ところ まで\l"
	.string "まだまだ あるぜ!$"

SilphCo_5F_Text_Grunt2Intro::
	.string "{JPN}ロケットだん あいてに\n"
	.string "なめた まねして くれるぜ$"

SilphCo_5F_Text_Grunt2Defeat::
	.string "{JPN}ふん たまたまだ!$"

SilphCo_5F_Text_Grunt2PostBattle::
	.string "{JPN}たまたま… おもいだした!\p"
	.string "タマタマは\n"
	.string "ナッシーに しんか するぞ$"

SilphCo_5F_Text_PorygonFirstVRMon::
	.string "{JPN}… ポケモン レポートだ!\p"
	.string "ポケモン けんきゅうじょ では\n"
	.string "じんこうの ポケモン\l"
	.string "ポリゴンを つくる ことに\l"
	.string "せいこう した… …$"

SilphCo_5F_Text_Over350TechniquesConfirmed::
	.string "{JPN}… ポケモン レポートだ!\p"
	.string "ポケモンが つかう わざは\n"
	.string "げんざい わかって いるだけで\l"
	.string "350しゅるい いじょうある$"

SilphCo_5F_Text_SomeMonsEvolveWhenTraded::
	.string "{JPN}… ポケモン レポートだ!\p"
	.string "つうしん こうかん すると\n"
	.string "しんかする ポケモンが\l"
	.string "いることが かくにん された$"

SilphCo_5F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "5かい$"

SilphCo_5F_Text_RocketBossLookingForStrongMons::
	.string "{JPN}この ビルに のりこんできた\n"
	.string "やつらの ボスは\l"
	.string "めずらしい つよい ポケモンを\l"
	.string "もとめて いるって いってた\p"
	.string "しゃちょうに ひがいが\n"
	.string "なければ いいのだが…$"
