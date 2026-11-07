SilphCo_6F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_6F_OnLoad
	.byte 0

SilphCo_6F_OnLoad::
	call_if_unset FLAG_SILPH_6F_DOOR, EventScript_Close6FDoor
	end

SilphCo_6F_EventScript_WorkerM2::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerM2RocketsGone
	msgbox SilphCo_6F_Text_TargetedSilphForOurMonProducts
	release
	end

SilphCo_6F_EventScript_WorkerM2RocketsGone::
	msgbox SilphCo_6F_Text_ComeWorkForSilphWhenYoureOlder
	release
	end

SilphCo_6F_EventScript_WorkerM3::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerM3RocketsGone
	msgbox SilphCo_6F_Text_RocketsTookOverBuilding
	release
	end

SilphCo_6F_EventScript_WorkerM3RocketsGone::
	msgbox SilphCo_6F_Text_BetterGetBackToWork
	release
	end

SilphCo_6F_EventScript_WorkerM1::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerM1RocketsGone
	msgbox SilphCo_6F_Text_HelpMePlease
	release
	end

SilphCo_6F_EventScript_WorkerM1RocketsGone::
	msgbox SilphCo_6F_Text_WeGotEngaged
	release
	end

SilphCo_6F_EventScript_WorkerF1::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerF1RocketsGone
	msgbox SilphCo_6F_Text_ThatManIsSuchACoward
	release
	end

SilphCo_6F_EventScript_WorkerF1RocketsGone::
	msgbox SilphCo_6F_Text_NeedsMeToLookAfterHim
	release
	end

SilphCo_6F_EventScript_WorkerF2::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_6F_EventScript_WorkerF2RocketsGone
	msgbox SilphCo_6F_Text_RocketsTryingToConquerWorld
	release
	end

SilphCo_6F_EventScript_WorkerF2RocketsGone::
	msgbox SilphCo_6F_Text_RocketsRanAwayBecauseOfYou
	release
	end

SilphCo_6F_EventScript_FloorSign::
	msgbox SilphCo_6F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_6F_EventScript_Taylor::
	trainerbattle_single TRAINER_SCIENTIST_TAYLOR, SilphCo_6F_Text_TaylorIntro, SilphCo_6F_Text_TaylorDefeat
	msgbox SilphCo_6F_Text_TaylorPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_6F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_30, SilphCo_6F_Text_Grunt1Intro, SilphCo_6F_Text_Grunt1Defeat
	msgbox SilphCo_6F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_6F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_31, SilphCo_6F_Text_Grunt2Intro, SilphCo_6F_Text_Grunt2Defeat
	msgbox SilphCo_6F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_6F_Text_RocketsTookOverBuilding::
	.string "{JPN}やつらが いきなり やってきて\n"
	.string "ビルを のっとったんです$"

SilphCo_6F_Text_BetterGetBackToWork::
	.string "{JPN}… さてと!\n"
	.string "しごと しなきゃ しごと!$"

SilphCo_6F_Text_HelpMePlease::
	.string "{JPN}ぶるぶる…\n"
	.string "かみさま ほとけさま たすけて$"

SilphCo_6F_Text_WeGotEngaged::
	.string "{JPN}ぼくたち こんやく したよ\n"
	.string "… てれるなあ$"

SilphCo_6F_Text_ThatManIsSuchACoward::
	.string "{JPN}となりの ひと…\n"
	.string "おとこ なのに なさけないわ$"

SilphCo_6F_Text_NeedsMeToLookAfterHim::
	.string "{JPN}このひとは\n"
	.string "あたしが ついてなきゃ\l"
	.string "だめだって おもったの$"

SilphCo_6F_Text_RocketsTryingToConquerWorld::
	.string "{JPN}ロケットだんは\n"
	.string "ポケモン つかって\l"
	.string "せかい せいふく したいのよ$"

SilphCo_6F_Text_RocketsRanAwayBecauseOfYou::
	.string "{JPN}ロケットだんが にげたのは\n"
	.string "あなたの おかげね$"

SilphCo_6F_Text_TargetedSilphForOurMonProducts::
	.string "{JPN}シルフ カンパニーは グッズを\n"
	.string "いっぱい つくってる から\l"
	.string "ねらわれたんだろうな$"

SilphCo_6F_Text_ComeWorkForSilphWhenYoureOlder::
	.string "{JPN}おおきく なったら\n"
	.string "シルフで はたらくと いいよ$"

SilphCo_6F_Text_Grunt1Intro::
	.string "{JPN}おれこそは\n"
	.string "ロケット 4きょうだいの ひとり!$"

SilphCo_6F_Text_Grunt1Defeat::
	.string "{JPN}まけたよ$"

SilphCo_6F_Text_Grunt1PostBattle::
	.string "{JPN}まあ いい\n"
	.string "おとうとが かたきを とって くれる$"

SilphCo_6F_Text_TaylorIntro::
	.string "{JPN}しゃちょうめー!\p"
	.string "おれさまを\n"
	.string "ポナヤツングスカ してん\l"
	.string "なんかに とばし やがるから\l"
	.string "ロケットだんに ねらわれるのだ!$"

SilphCo_6F_Text_TaylorDefeat::
	.string "{JPN}がっくしよ$"

SilphCo_6F_Text_TaylorPostBattle::
	.string "{JPN}ポナヤツングスカ してん?\n"
	.string "ああ…\l"
	.string "ロシアの おくの ほうだよ$"

SilphCo_6F_Text_Grunt2Intro::
	.string "{JPN}ロケットだんを うらぎって\n"
	.string "ただで すむと おもうな!$"

SilphCo_6F_Text_Grunt2Defeat::
	.string "{JPN}うらぎりもの め$"

SilphCo_6F_Text_Grunt2PostBattle::
	.string "{JPN}うらぎり じゃない?\n"
	.string "せいぎの みかた なら\l"
	.string "きみは おれらの うらぎりものだ$"

SilphCo_6F_Text_FloorSign::
	.string "{JPN}シルフ カンパニー ほんしゃ\n"
	.string "6かい$"

