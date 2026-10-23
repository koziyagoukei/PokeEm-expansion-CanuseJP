FortreeCity_Gym_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FortreeCity_Gym_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, FortreeCity_Gym_OnWarp
	.byte 0

FortreeCity_Gym_OnTransition:
	special RotatingGate_InitPuzzle
	setflag FLAG_TEMP_HIDE_FOLLOWER @ Hide follower bc of rotating gates
	end

FortreeCity_Gym_OnWarp:
	map_script_2 VAR_TEMP_0, VAR_TEMP_0, FortreeCity_Gym_EventScript_InitRotatingGates
	.2byte 0

@ NOTE: This rotating gate puzzle makes use of VAR_TEMP_0 - VAR_TEMP_3
FortreeCity_Gym_EventScript_InitRotatingGates::
	special RotatingGate_InitPuzzleAndGraphics
	end

FortreeCity_Gym_EventScript_Winona::
	trainerbattle_single TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaIntro, FortreeCity_Gym_Text_WinonaDefeat, FortreeCity_Gym_EventScript_WinonaDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, FortreeCity_Gym_EventScript_WinonaRematch
	goto_if_unset FLAG_RECEIVED_TM_AERIAL_ACE, FortreeCity_Gym_EventScript_GiveAerialAce2
	msgbox FortreeCity_Gym_Text_WinonaPostBattle, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_WinonaDefeated::
	message FortreeCity_Gym_Text_ReceivedFeatherBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FORTREE_GYM
	setflag FLAG_BADGE06_GET
	setvar VAR_0x8008, 6
	call Common_EventScript_SetGymTrainers
	call FortreeCity_Gym_EventScript_GiveAerialAce
	closemessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox FortreeCity_Gym_Text_RegisteredWinona, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_WINONA_MATCH_CALL
	setvar VAR_SCOTT_FORTREE_CALL_STEP_COUNTER, 0
	setflag FLAG_SCOTT_CALL_FORTREE_GYM
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce2::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	release
	end

FortreeCity_Gym_EventScript_GiveAerialAce::
	giveitem ITEM_TM_AERIAL_ACE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_BagIsFull
	msgbox FortreeCity_Gym_Text_ExplainAerialAce, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_TM_AERIAL_ACE
	return

FortreeCity_Gym_EventScript_WinonaRematch::
	trainerbattle_rematch_double TRAINER_WINONA_1, FortreeCity_Gym_Text_WinonaPreRematch, FortreeCity_Gym_Text_WinonaRematchDefeat, FortreeCity_Gym_Text_WinonaRematchNeedTwoMons
	msgbox FortreeCity_Gym_Text_WinonaPostRematch, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Jared::
	trainerbattle_single TRAINER_JARED, FortreeCity_Gym_Text_JaredIntro, FortreeCity_Gym_Text_JaredDefeat
	msgbox FortreeCity_Gym_Text_JaredPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Edwardo::
	trainerbattle_single TRAINER_EDWARDO, FortreeCity_Gym_Text_EdwardoIntro, FortreeCity_Gym_Text_EdwardoDefeat
	msgbox FortreeCity_Gym_Text_EdwardoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Flint::
	trainerbattle_single TRAINER_FLINT, FortreeCity_Gym_Text_FlintIntro, FortreeCity_Gym_Text_FlintDefeat
	msgbox FortreeCity_Gym_Text_FlintPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Ashley::
	trainerbattle_single TRAINER_ASHLEY, FortreeCity_Gym_Text_AshleyIntro, FortreeCity_Gym_Text_AshleyDefeat
	msgbox FortreeCity_Gym_Text_AshleyPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Humberto::
	trainerbattle_single TRAINER_HUMBERTO, FortreeCity_Gym_Text_HumbertoIntro, FortreeCity_Gym_Text_HumbertoDefeat
	msgbox FortreeCity_Gym_Text_HumbertoPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_Darius::
	trainerbattle_single TRAINER_DARIUS, FortreeCity_Gym_Text_DariusIntro, FortreeCity_Gym_Text_DariusDefeat
	msgbox FortreeCity_Gym_Text_DariusPostBattle, MSGBOX_AUTOCLOSE
	end

FortreeCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_FORTREE_GYM, FortreeCity_Gym_EventScript_GymGuidePostVictory
	msgbox FortreeCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_GymGuidePostVictory::
	msgbox FortreeCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

FortreeCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, FortreeCity_Gym_EventScript_GymStatueCertified
	goto FortreeCity_Gym_EventScript_GymStatue
	end

FortreeCity_Gym_EventScript_GymStatueCertified::
	msgbox FortreeCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_EventScript_GymStatue::
	msgbox FortreeCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

FortreeCity_Gym_Text_GymGuideAdvice:
	.string "げんき しとぉや!!\nチャンピオンを めざす {PLAYER}!!\pヒワマキシティ ジムの リーダー ナギは\nひこうタイプ ポケモンの つかいてだ!\pこの かいてんとびら だらけの\nジムの おくで\pかのじょは あらたな チャレンジャーが\nはばたこうと するのを まっている!\pじゃ きばってこォー!!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_GymGuidePostVictory:
	.string "えんやっと きみは はばたいたッ!"
	.string "$"

FortreeCity_Gym_Text_JaredIntro:
	.string "かれいな とりポケモンの たたかいを\nみせて やるぜっ!"
	.string "$"

FortreeCity_Gym_Text_JaredDefeat:
	.string "きみ…… つよいんだな……"
	.string "$"

FortreeCity_Gym_Text_JaredPostBattle:
	.string "トレーナーは あたまの かいてんも\nはやく なければ つとまらないぜ!\pきみは リーダーの ところまで\nちゃんと たどりつく ことが できるかい?"
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_EdwardoIntro:
	.string "とりポケモンを あいする ものは\nこの ジムを もくひょうと している!\pヒワマキ ジムの いちいん として\nまける わけには いかないんだ!"
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_EdwardoDefeat:
	.string "オレには にが おもすぎたか……!"
	.string "$"

FortreeCity_Gym_Text_EdwardoPostBattle:
	.string "せかいは ひろい\nつよい トレーナーは たくさん いる\pオレも ここで しゅぎょうを つづけて\nつよくなって みせるよ!"
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_FlintIntro:
	.string "ジムリーダーの ナギさんの\nてを わずらわせるまでも ない!\pきみの あいては オレで じゅうぶん!"
	.string ""
	.string "$"

FortreeCity_Gym_Text_FlintDefeat:
	.string "ナギさん…… まけました……!"
	.string "$"

FortreeCity_Gym_Text_FlintPostBattle:
	.string "ナギさんは かわいくて つよい……\nほんと さいこうの リーダーだよ!\p……ポッ"
	.string ""
	.string "$"

FortreeCity_Gym_Text_AshleyIntro:
	.string "ナギさんの おしえを うけた あたし!\nそう かんたんに たおされないわ!"
	.string "$"

FortreeCity_Gym_Text_AshleyDefeat:
	.string "たおされ ちゃった……$"

FortreeCity_Gym_Text_AshleyPostBattle:
	.string "ナギさんが いてくれるから\nヒワマキの ひとは あんしんして くらせるの"
	.string "$"

FortreeCity_Gym_Text_HumbertoIntro:
	.string "ナギさまが たたかわれる ときの\nあの りりしい おかお……\pおまえ なんかにゃ みせられないぜ!"
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_HumbertoDefeat:
	.string "うう!\nおれでは とめられ なかった"
	.string "$"

FortreeCity_Gym_Text_HumbertoPostBattle:
	.string "ナギさまに みとれて\nボロまけ しないように\lちゅういするんだな!"
	.string ""
	.string "$"

FortreeCity_Gym_Text_DariusIntro:
	.string "ひこうタイプ って いっても\nいろんな ポケモンが いるんだぜ!\lおまえ それ わかってる?"
	.string ""
	.string "$"

FortreeCity_Gym_Text_DariusDefeat:
	.string "なかなか わかってるな!$"

FortreeCity_Gym_Text_DariusPostBattle:
	.string "ぼくは まけたけど\nナギ リーダーの ポケモンは\lほんと てごわいぞ!"
	.string ""
	.string "$"

FortreeCity_Gym_Text_WinonaIntro:
	.string "わたしは この ヒワマキ ジムで\nリーダーを している ナギ……\pとりポケモンと こころを かよわし\nいっしょに おおぞらを まい……\pどんな くるしい しょうぶも\nゆうがに かってみせる……\pさあ わたしと ポケモンが おりなす\nかれいな まいを みせましょう!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_WinonaDefeat:
	.string "わたしより かれいに\nポケモンを たたかわせる ひと……\lはじめて であいました……\pその じつりょくを みとめ\nこの ジムバッジを おわたしします!"
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_ReceivedFeatherBadge:
	.string "{PLAYER}は ナギから\nフェザーバッジを もらった!"
	.string "$"

FortreeCity_Gym_Text_ExplainFeatherBadgeTakeThis:
	.string "フェザーバッジを つけてると\nひとから もらった ポケモンでも\lレベル70までの ポケモンなら\lいうことを すなおに きいてくれます!\pひでんわざの そらをとぶ を\nつかえる ようにも なります!\pそして これを\nあなたに プレゼント します"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_ExplainAerialAce:
	.string "わざマシン40の なかみは\nつばめがえし!\pその すばやさ……\nどんな ポケモンでも\lよけることは できないでしょう!\p…… …… ……"
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_RegisteredWinona:
	.string "ジムリーダー ナギを\nポケナビに とうろく した!"
	.string "$"

FortreeCity_Gym_Text_WinonaPostBattle:
	.string "あなたには まけました けど\pわたしは これからも\nとりポケモンに こだわって いきます"
	.string "$"

FortreeCity_Gym_Text_GymStatue:
	.string "ヒワマキシティ ポケモンジム$"

FortreeCity_Gym_Text_GymStatueCertified:
	.string "ヒワマキシティ ポケモンジム\pナギ にんてい トレーナー!\n{PLAYER}"
	.string ""
	.string "$"

FortreeCity_Gym_Text_WinonaPreRematch:
	.string "ナギ『ひとは ちじょうに いる かぎり\nいんりょくから のがれることは できない\pでも ぜんりょくで はばたけば\nそらを とぶことは できるのです\pチャンピオンにまで のぼりつめた\nあなたの じつりょく\lもういちど みせて いただきますわ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_WinonaRematchDefeat:
	.string "やはり とどかなかった……$"

FortreeCity_Gym_Text_WinonaPostRematch:
	.string "ナギ『たとえ しょうぶに まけても\nこころの つばさが おれて いなければ\lなんどでも うかび あがれる……\pわたしは そう しんじて いますよ!"
	.string ""
	.string ""
	.string ""
	.string "$"

FortreeCity_Gym_Text_WinonaRematchNeedTwoMons:
	.string "ナギ『ひとは ちじょうに いる かぎり\nいんりょくから のがれることは できない\pでも ぜんりょくで はばたけば\nそらを とぶことは できるのです\pチャンピオンにまで のぼりつめた\nあなたの じつりょく\lもういちど みせて いただきますわ!\n……あら? たたかえる ポケモンを\n1ひきしか もっていませんね\pせめて 2ひき いじょうの\nポケモンを もって また きてください"
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
