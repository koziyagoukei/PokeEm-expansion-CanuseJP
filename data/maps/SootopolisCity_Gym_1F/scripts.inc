SootopolisCity_Gym_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SootopolisCity_Gym_1F_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, SootopolisCity_Gym_1F_OnResume
	map_script MAP_SCRIPT_ON_LOAD, SootopolisCity_Gym_1F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SootopolisCity_Gym_1F_OnTransition
	.byte 0

SootopolisCity_Gym_1F_OnTransition:
	setvar VAR_ICE_STEP_COUNT, 1
	end

SootopolisCity_Gym_1F_OnResume:
	setstepcallback STEP_CB_SOOTOPOLIS_ICE
	end

SootopolisCity_Gym_1F_OnLoad:
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special SetSootopolisGymCrackedIceMetatiles
	end

SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles::
	goto_if_lt VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_StopCheckingStairs  @ All stairs ice
	goto_if_lt VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_OpenFirstStairs
	goto_if_lt VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs
	setmetatile 8, 4, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 5, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstAndSecondStairs::
	setmetatile 8, 10, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 11, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_OpenFirstStairs::
	setmetatile 8, 15, METATILE_SootopolisGym_Stairs, FALSE
	setmetatile 8, 16, METATILE_SootopolisGym_Stairs, FALSE
SootopolisCity_Gym_1F_EventScript_StopCheckingStairs::
	return

SootopolisCity_Gym_1F_OnFrame:
	map_script_2 VAR_ICE_STEP_COUNT, 8, SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs
	map_script_2 VAR_ICE_STEP_COUNT, 28, SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs
	map_script_2 VAR_ICE_STEP_COUNT, 67, SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs
	map_script_2 VAR_ICE_STEP_COUNT, 0, SootopolisCity_Gym_1F_EventScript_FallThroughIce
	.2byte 0

SootopolisCity_Gym_1F_EventScript_UnlockFirstStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockSecondStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_UnlockThirdStairs::
	addvar VAR_ICE_STEP_COUNT, 1
	delay 40
	playse SE_ICE_STAIRS
	call SootopolisCity_Gym_1F_EventScript_CheckSetStairMetatiles
	special DrawWholeMapView
	end

SootopolisCity_Gym_1F_EventScript_FallThroughIce::
	lockall
	delay 20
	applymovement LOCALID_PLAYER, SootopolisCity_Gym_1F_Movement_FallThroughIce
	waitmovement 0
	playse SE_FALL
	delay 60
	warphole MAP_SOOTOPOLIS_CITY_GYM_B1F
	waitstate
	end

SootopolisCity_Gym_1F_Movement_FallThroughIce:
	set_invisible
	step_end

SootopolisCity_Gym_1F_EventScript_Juan::
	trainerbattle_single TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanIntro, SootopolisCity_Gym_1F_Text_JuanDefeat, SootopolisCity_Gym_1F_EventScript_JuanDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, SootopolisCity_Gym_1F_EventScript_JuanRematch
	goto_if_unset FLAG_RECEIVED_TM_WATER_PULSE, SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2
	goto_if_unset FLAG_BADGE06_GET, SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge
	msgbox SootopolisCity_Gym_1F_Text_JuanPostBattle, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanDefeated::
	message SootopolisCity_Gym_1F_Text_ReceivedRainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_SOOTOPOLIS_GYM
	setflag FLAG_BADGE08_GET
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_RESIDENTS
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_STEVEN
	setflag FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE
	setvar VAR_SOOTOPOLIS_CITY_STATE, 6
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_MAN_1
	setvar VAR_0x8008, 8
	call Common_EventScript_SetGymTrainers
	call SootopolisCity_Gym_1F_EventScript_GiveWaterPulse
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox SootopolisCity_Gym_1F_Text_RegisteredJuan, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_JUAN_MATCH_CALL
	release
	end

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	return

SootopolisCity_Gym_1F_EventScript_GiveWaterPulse2::
	giveitem ITEM_TM_WATER_PULSE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox SootopolisCity_Gym_1F_Text_ExplainWaterPulse, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_WATER_PULSE
	release
	end

SootopolisCity_Gym_1F_EventScript_GoGetFortreeBadge::
	msgbox SootopolisCity_Gym_1F_Text_GoGetFortreeBadge, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_JuanRematch::
	trainerbattle_rematch_double TRAINER_JUAN_1, SootopolisCity_Gym_1F_Text_JuanPreRematch, SootopolisCity_Gym_1F_Text_JuanRematchDefeat, SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons
	msgbox SootopolisCity_Gym_1F_Text_JuanPostRematch, MSGBOX_AUTOCLOSE
	end

SootopolisCity_Gym_1F_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SOOTOPOLIS_GYM, SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory
	msgbox SootopolisCity_Gym_1F_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_GymGuidePostVictory::
	msgbox SootopolisCity_Gym_1F_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

SootopolisCity_Gym_1F_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, SootopolisCity_Gym_1F_EventScript_GymStatueCertified
	goto SootopolisCity_Gym_1F_EventScript_GymStatue
	end

SootopolisCity_Gym_1F_EventScript_GymStatueCertified::
	msgbox SootopolisCity_Gym_1F_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_EventScript_GymStatue::
	msgbox SootopolisCity_Gym_1F_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

SootopolisCity_Gym_1F_Text_GymGuideAdvice:
	.string "{JPN}げんき しとぉや!!\nチャンピオンを めざす {PLAYER}!!\pルネシティの ジムリーダー アダンは\nみずタイプ ポケモンを つかいこなす!\pそして アダンがいる ところ までは\nこおりの ゆかが ゆくてを さえぎる……\pごめんな! きみに おしえて\nあげられるのは ここまで なんだ……!\pあとは じぶんで きばってくれッ!$"

SootopolisCity_Gym_1F_Text_GymGuidePostVictory:
	.string "{JPN}ホウエンで いちばん つよいと いわれる\nアダンに かったのかあ!!\pよし! きみの\nトレーナーカードを みたまえ!\pすべての バッジが そろっているなら\nいよいよ ポケモンリーグ ちょうせんだ!$"

SootopolisCity_Gym_1F_Text_JuanIntro:
	.string "{JPN}ユーは しっていますか?\pミクリに ポケモンを おしえたのは\nなにをかくそう このアダン……\lすなわち わたくし だということを\pいちどは ジムリーダーから みをひき\nこのジムも かれに まかせて いましたが\lちょっとした リーズン……\lすなわち りゆうが できましてね\lもういちど カムバック したのです\p……おっと\nおしゃべりは これくらいにして\lそろそろ しょうぶを はじめましょう\pさあ ぞんぶんに ごらんなさい\nわたしと ポケモンが おりなす\lみずの イリュージョンを!$"

@ NOTE: This defeat text actually causes a buffer overflow. It's about 50 bytes too long for
@ the gDisplayedStringBattle buffer that it's put into, and it stomps all over the gBattleTextBuffs
@ after, as well as the otherwise unused array after that. One wonders if that's the reason for
@ the existence of that unused array of ints.
SootopolisCity_Gym_1F_Text_JuanDefeat:
	.string "{JPN}たははは! ユー アー ウィナー!\nすなわち わたくしの まけです\pユーには だれにも まけない\nさいのうの きらめきが かんじられます!\pしかし わたくしや ミクリに くらべると\nちょっと はなやかさが たりませんな!\pわたしの ふくでも\nかして あげましょうか?\p…… …… ……\n…… …… ……\pはっはっは ジョークですよ!\nユーには ふくの かわりに\lこの レインバッジを さしあげましょう!$"

SootopolisCity_Gym_1F_Text_ReceivedRainBadge:
	.string "{JPN}{PLAYER}は アダンから\nレインバッジを もらった!$"

SootopolisCity_Gym_1F_Text_ExplainRainBadgeTakeThis:
	.string "{JPN}レインバッジを もっていると\nすべての ポケモンが\lユーに たいして すなおに なるでしょう\pはげしく ながれる たきも\nひでんわざ たきのぼりを つかって\lのぼれる ように してくれます\pそれから わたくしとの しょうぶを\nわすれないように これを……$"

SootopolisCity_Gym_1F_Text_ExplainWaterPulse:
	.string "{JPN}ユーに わたした その わざマシン\nなかみは みずのはどう\pつかうと ときおり ちょうおんぱで\nあいてを こんらん させるのです!\p…… …… ……$"

SootopolisCity_Gym_1F_Text_RegisteredJuan:
	.string "{JPN}ジムリーダー アダンを\nポケナビに とうろく した!$"

SootopolisCity_Gym_1F_Text_JuanPostBattle:
	.string "{JPN}ホウエンで あつめられる\nすべての ジムバッジを てにいれた\lトレーナーが いくべき ばしょ\pそれが ポケモンリーグ\pポケモンリーグに いくならば\nホウエンちほうの さいはて\lサイユウという しまに むかうのです$"

SootopolisCity_Gym_1F_Text_GoGetFortreeBadge:
	.string "{JPN}ホウエンで あつめられる\nジムバッジは あと ひとつ\pユーが ポケモンリーグに いどむ つもりなら\nヒワマキの ポケモンジムで\lのこりの バッジを てにいれるのです$"

SootopolisCity_Gym_1F_Text_GymStatue:
	.string "{JPN}ルネシティ ポケモンジム$"

SootopolisCity_Gym_1F_Text_GymStatueCertified:
	.string "{JPN}ルネシティ ポケモンジム\pアダン にんてい トレーナー!\n{PLAYER}$"

SootopolisCity_Gym_1F_Text_JuanPreRematch:
	.string "{JPN}アダン『いつもの サイレンス……\nしずけさを とりもどした このジムに\pまたもや わかき タイフーンが\nうでだめしに あらわれましたね!\pいいですとも!\pなんどでも ユーの あいてを\nして さしあげましょう!$"

SootopolisCity_Gym_1F_Text_JuanRematchDefeat:
	.string "{JPN}たははは! ユー アー ウィナー!\nまたしても わたくしの まけです$"

SootopolisCity_Gym_1F_Text_JuanPostRematch:
	.string "{JPN}アダン『わたしの でしに なれと いっても\nきっと ユーは ことわる でしょうね\pせめて わたしが きている コートを\nプレゼント したいの ですが……\lおそらく ユーは うけとらない でしょう\pそれが ジェントルマン というものです\nほっほっほっほ!$"

SootopolisCity_Gym_1F_Text_JuanRematchNeedTwoMons:
	.string "{JPN}アダン『いつもの サイレンス……\nしずけさを とりもどした このジムに\pまたもや わかき タイフーンが\nうでだめしに あらわれましたね!\pいいですとも!\nなんどでも ユーの あいてを……\p……アハン?\nユーは たたかえる ポケモンを\l1ひきしか もっていない ようですね\pねがわくば 2ひき いじょうの\nポケモンを もってきて プリーズ$"
