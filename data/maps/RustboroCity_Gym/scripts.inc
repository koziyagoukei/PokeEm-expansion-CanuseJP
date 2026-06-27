RustboroCity_Gym_MapScripts::
	.byte 0

RustboroCity_Gym_EventScript_Roxanne::
	trainerbattle_single TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxanneIntro, RustboroCity_Gym_Text_RoxanneDefeat, RustboroCity_Gym_EventScript_RoxanneDefeated, NO_MUSIC
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, RustboroCity_Gym_EventScript_RoxanneRematch
	goto_if_unset FLAG_RECEIVED_TM_ROCK_TOMB, RustboroCity_Gym_EventScript_GiveRockTomb
	msgbox RustboroCity_Gym_Text_RoxannePostBattle, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneDefeated::
	message RustboroCity_Gym_Text_ReceivedStoneBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox RustboroCity_Gym_Text_StoneBadgeInfoTakeThis, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_RUSTBORO_GYM
	setflag FLAG_BADGE01_GET
	setvar VAR_RUSTBORO_CITY_STATE, 1
	addvar VAR_PETALBURG_GYM_STATE, 1
	setvar VAR_0x8008, 1
	call Common_EventScript_SetGymTrainers
	call_if_eq VAR_PETALBURG_GYM_STATE, 6, Common_EventScript_ReadyPetalburgGymForBattle
	goto RustboroCity_Gym_EventScript_GiveRockTomb
	end

RustboroCity_Gym_EventScript_GiveRockTomb::
	giveitem ITEM_TM_ROCK_TOMB
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROCK_TOMB
	msgbox RustboroCity_Gym_Text_ExplainRockTomb, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_RoxanneRematch::
	trainerbattle_rematch_double TRAINER_ROXANNE_1, RustboroCity_Gym_Text_RoxannePreRematch, RustboroCity_Gym_Text_RoxanneRematchDefeat, RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons
	msgbox RustboroCity_Gym_Text_RoxannePostRematch, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Josh::
	trainerbattle_single TRAINER_JOSH, RustboroCity_Gym_Text_JoshIntro, RustboroCity_Gym_Text_JoshDefeat
	msgbox RustboroCity_Gym_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Tommy::
	trainerbattle_single TRAINER_TOMMY, RustboroCity_Gym_Text_TommyIntro, RustboroCity_Gym_Text_TommyDefeat
	msgbox RustboroCity_Gym_Text_TommyPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_Marc::
	trainerbattle_single TRAINER_MARC, RustboroCity_Gym_Text_MarcIntro, RustboroCity_Gym_Text_MarcDefeat
	msgbox RustboroCity_Gym_Text_MarcPostBattle, MSGBOX_AUTOCLOSE
	end

RustboroCity_Gym_EventScript_GymGuide::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_RUSTBORO_GYM, RustboroCity_Gym_EventScript_GymGuidePostVictory
	msgbox RustboroCity_Gym_Text_GymGuideAdvice, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_GymGuidePostVictory::
	msgbox RustboroCity_Gym_Text_GymGuidePostVictory, MSGBOX_DEFAULT
	release
	end

RustboroCity_Gym_EventScript_LeftGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_RightGymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, RustboroCity_Gym_EventScript_GymStatueCertified
	goto RustboroCity_Gym_EventScript_GymStatue
	end

RustboroCity_Gym_EventScript_GymStatueCertified::
	msgbox RustboroCity_Gym_Text_GymStatueCertified, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_GymStatue::
	msgbox RustboroCity_Gym_Text_GymStatue, MSGBOX_DEFAULT
	releaseall
	end

RustboroCity_Gym_EventScript_RegisterRoxanne::
	lockall
	pokenavcall RustboroCity_Gym_Text_RoxanneRegisterCall
	waitmessage
	delay 30
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox RustboroCity_Gym_Text_RegisteredRoxanne, MSGBOX_DEFAULT
	waitfanfare
	closemessage
	delay 30
	setflag FLAG_ENABLE_ROXANNE_MATCH_CALL
	clearflag FLAG_ENABLE_ROXANNE_FIRST_CALL
	releaseall
	end

RustboroCity_Gym_Text_GymGuideAdvice:
	.string "{JPN}げんき しとぉや!!\pいきなり だが きみぃ!\nチャンピオンを めざして みないか!\pぼくは トレーナー じゃないけど\nかつ ために ばっちり\lアドバイス できる!\pよし きまった! いっしょに\nポケモン チャンピオン めざそう!\pトレーナーの もくてき といえば\nジムバッジを あつめること だろう\lだが ジムリーダーは てごわい!\pそこで ぼくの でばん というわけだ!\nきみに ばっちり アドバイス するよ!\pここの ジムリーダー ツツジは\nいわタイプの ポケモンの つかいて!\pいわタイプは がんじょう だけど\nみずタイプや くさタイプの\lこうげきは にがて なんだよ!\pジムリーダーに かてたら\nまた ぼくに はなしかけて くれよ!\pじゃ きばってこォー!!"
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

RustboroCity_Gym_Text_GymGuidePostVictory:
	.string "{JPN}うおォ! みごとな しょうり!!\nぼくの おうえんが よかったのか!?\pよし! きみの\nトレーナーカードを みたまえ!\pさっき てにいれた ストーンバッジが\nきちんと きろく されている だろう\pつまり……\pきみは チャンピオンへの かいだんを\nいっぽ のぼったんだ!\pごったましィー!!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_JoshIntro:
	.string "{JPN}ジム トレーナーを なめるなよ!\nじつりょくの ちがいを みせてやる!"
	.string "$"

RustboroCity_Gym_Text_JoshDefeat:
	.string "{JPN}じつりょくが ちがいすぎた……!$"

RustboroCity_Gym_Text_JoshPostBattle:
	.string "{JPN}いわタイプの おそろしさ\nまだまだ こんな ものじゃない!"
	.string "$"

RustboroCity_Gym_Text_TommyIntro:
	.string "{JPN}ボクに かてる じつりょくが ないと\nツツジさん には ぜったい かなわないよ!"
	.string "$"

RustboroCity_Gym_Text_TommyDefeat:
	.string "{JPN}すごい じつりょくだ!$"

RustboroCity_Gym_Text_TommyPostBattle:
	.string "{JPN}ツツジさんは ボクの すうばい つよい!\nゆだん しない ほうが いいよ!"
	.string "$"

RustboroCity_Gym_Text_MarcIntro:
	.string "{JPN}こんな まちなか でも\nいわタイプで おおあばれだ!"
	.string "$"

RustboroCity_Gym_Text_MarcDefeat:
	.string "{JPN}おおーっ!\nいきの いい ちょうせんしゃ だな!"
	.string "$"

RustboroCity_Gym_Text_MarcPostBattle:
	.string "{JPN}わかくして いわタイプに\nめを つける なんて\lうちの ジムリーダーは センスが いいぜ!"
	.string ""
	.string "$"

RustboroCity_Gym_Text_RoxanneIntro:
	.string "{JPN}わたくし カナズミ ポケモンジム\nリーダーの ツツジ といいます\pトレーナーズ スクールで\nまなんだこと しょうぶに いかしたくて\lジムリーダーに なったの\pあなた どのような ポケモンで\nどんなふうに たたかうのか\lわたくしに おしえて くださるかしら?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_RoxanneDefeat:
	.string "{JPN}わたくし まけたのね……\pもっと おおくの ことを\nまなぶ ひつようが あるみたい……\pわかりましたわ\pポケモンリーグの きまりでは\nジムリーダーに かった トレーナーに\lこれを わたすことに なっています\pどうぞ ポケモンリーグ こうにんの\nストーンバッジ うけとって ください"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_ReceivedStoneBadge:
	.string "{PLAYER}は ツツジから\n"
	.string "ストーンバッジを もらった!$"

RustboroCity_Gym_Text_StoneBadgeInfoTakeThis:
	.string "{JPN}ストーンバッジ には あなたの\nポケモンの こうげきりょくを\lたかめる こうかが あるの\pあと たたかっていない ときでも\nひでんわざの いあいぎり が\lつかえるようにも なるわ\pそれから……\nこれも もっていって ください"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_ExplainRockTomb:
	.string "{JPN}その わざマシン39の なかには\nがんせきふうじが はいっているわ\pいわを あいてに むかって おとして\nダメージを あたえる だけでなくて\lすばやさも さげる わざ なの\pちなみに\nわざマシンを つかえば ポケモンに\lいっしゅんで わざを おしえられるの\pただし つかいすて だから\nつかうときは よく かんがえてね"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_RoxannePostBattle:
	.string "{JPN}それだけ つよいなら\nほかの ポケモンジムにも\lちょうせん したら どうかしら?\pおおくの トレーナーと\nしょうぶを することで\lさまざまなこと まなべるはずよ"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_GymStatue:
	.string "{JPN}カナズミシティ ポケモンジム$"

RustboroCity_Gym_Text_GymStatueCertified:
	.string "{JPN}カナズミシティ ポケモンジム\pツツジ にんてい トレーナー!\n{PLAYER}"
	.string ""
	.string "$"

RustboroCity_Gym_Text_RoxanneRegisterCall:
	.string "{JPN}…… …… ……\n…… …… …… ピッ!\pツツジ『あっ {PLAYER}さん ですか?\nわたくし カナズミの ツツジです!\pトウキさんから ききましたよぉ\nあなた すごい いきおいで\lかちすすんで いるらしい ですね!\pこちらも まけっぱなしでは いられません\nもういちど ジムを きたえなおして\lいつかまた ちょうせんを うけますよ!\pそのひが くるのを\nどうぞ まっていて くださいね!\p…… …… ……\n…… …… …… ツー"
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
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_RegisteredRoxanne:
	.string "{JPN}ジムリーダー ツツジを\nポケナビに とうろく した!"
	.string "$"

RustboroCity_Gym_Text_RoxannePreRematch:
	.string "{JPN}ツツジ『また おめにかかれましたね\nジムリーダーの ツツジ です\pあれから おたがいに\nいくつもの しょうぶを けいけんし\lどれほど つよく なれたのか……\pさあ おてあわせ ねがいます!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_RoxanneRematchDefeat:
	.string "{JPN}くうぅ……\nまたしても わたくしの まけ……"
	.string "$"

RustboroCity_Gym_Text_RoxannePostRematch:
	.string "{JPN}ツツジ『まだまだ ポケモン しょうぶは\nまなぶことが たくさん あるのですね\pわたくし…… ポケモン しょうぶが\nますます すきに なりましたよ!"
	.string ""
	.string ""
	.string "$"

RustboroCity_Gym_Text_RoxanneRematchNeedTwoMons:
	.string "{JPN}ツツジ『また おめにかかれましたね\nジムリーダーの ツツジ です\pあれから おたがいに\nいくつもの しょうぶを けいけんし\lどれほど つよく なれたのか……\p……あら? たたかえる ポケモンを\n1ひきしか もっていませんね\pどうぞ 2ひき いじょうの\nポケモンを もって きてください"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"
