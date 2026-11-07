ViridianCity_Gym_Frlg_MapScripts::
	.byte 0

ViridianCity_Gym_EventScript_Giovanni::
	famechecker FAMECHECKER_GIOVANNI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_GIOVANNI, ViridianCity_Gym_Text_GiovanniIntro, ViridianCity_Gym_Text_GiovanniDefeat, ViridianCity_Gym_EventScript_DefeatedGiovanni, NO_MUSIC
	goto_if_unset FLAG_GOT_TM26_FROM_GIOVANNI, ViridianCity_Gym_EventScript_GiveTM26
	msgbox ViridianCity_Gym_Text_GiovanniPostBattle
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_GIOVANNI
	fadescreen FADE_FROM_BLACK
	release
	end

ViridianCity_Gym_EventScript_DefeatedGiovanni::
	famechecker FAMECHECKER_GIOVANNI, 3
	setflag FLAG_HIDE_MISC_KANTO_ROCKETS
	setflag FLAG_DEFEATED_LEADER_GIOVANNI
	setflag FLAG_BADGE08_GET
	setvar VAR_MAP_SCENE_ROUTE22, 3
	set_gym_trainers_frlg 8
	goto ViridianCity_Gym_EventScript_GiveTM26
	end

ViridianCity_Gym_EventScript_GiveTM26::
	msgbox ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis
	checkitemspace ITEM_TM26
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_Gym_EventScript_NoRoomForTM26
	giveitem_msg ViridianCity_Gym_Text_ReceivedTM26FromGiovanni, ITEM_TM26
	setflag FLAG_GOT_TM26_FROM_GIOVANNI
	msgbox ViridianCity_Gym_Text_ExplainTM26
	release
	end

ViridianCity_Gym_EventScript_NoRoomForTM26::
	msgbox ViridianCity_Gym_Text_YouDoNotHaveSpace
	release
	end

ViridianCity_Gym_EventScript_Jason::
	trainerbattle_single TRAINER_TAMER_JASON, ViridianCity_Gym_Text_JasonIntro, ViridianCity_Gym_Text_JasonDefeat
	msgbox ViridianCity_Gym_Text_JasonPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Cole::
	trainerbattle_single TRAINER_TAMER_COLE, ViridianCity_Gym_Text_ColeIntro, ViridianCity_Gym_Text_ColeDefeat
	msgbox ViridianCity_Gym_Text_ColePostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Atsushi::
	trainerbattle_single TRAINER_BLACK_BELT_ATSUSHI, ViridianCity_Gym_Text_AtsushiIntro, ViridianCity_Gym_Text_AtsushiDefeat
	msgbox ViridianCity_Gym_Text_AtsushiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Kiyo::
	trainerbattle_single TRAINER_BLACK_BELT_KIYO, ViridianCity_Gym_Text_KiyoIntro, ViridianCity_Gym_Text_KiyoDefeat
	msgbox ViridianCity_Gym_Text_KiyoPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Takashi::
	trainerbattle_single TRAINER_BLACK_BELT_TAKASHI, ViridianCity_Gym_Text_TakashiIntro, ViridianCity_Gym_Text_TakashiDefeat
	msgbox ViridianCity_Gym_Text_TakashiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Samuel::
	trainerbattle_single TRAINER_COOLTRAINER_SAMUEL, ViridianCity_Gym_Text_SamuelIntro, ViridianCity_Gym_Text_SamuelDefeat
	msgbox ViridianCity_Gym_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Yuji::
	trainerbattle_single TRAINER_COOLTRAINER_YUJI, ViridianCity_Gym_Text_YujiIntro, ViridianCity_Gym_Text_YujiDefeat
	msgbox ViridianCity_Gym_Text_YujiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Warren::
	trainerbattle_single TRAINER_COOLTRAINER_WARREN, ViridianCity_Gym_Text_WarrenIntro, ViridianCity_Gym_Text_WarrenDefeat
	msgbox ViridianCity_Gym_Text_WarrenPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LEADER_GIOVANNI, ViridianCity_Gym_EventScript_GymGuyPostVictory
	msgbox ViridianCity_Gym_Text_GymGuyAdvice
	release
	end

ViridianCity_Gym_EventScript_GymGuyPostVictory::
	famechecker FAMECHECKER_GIOVANNI, 4
	msgbox ViridianCity_Gym_Text_GymGuyPostVictory
	release
	end

ViridianCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, ViridianCity_Gym_EventScript_GymStatuePostVictory
	msgbox ViridianCity_Gym_Text_GymStatue
	releaseall
	end

ViridianCity_Gym_EventScript_GymStatuePostVictory::
	msgbox ViridianCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

ViridianCity_Gym_Text_GiovanniIntro::
	.string "{JPN}ははははーッ!\n"
	.string "ここは おれの かくれが だ!\p"
	.string "ロケットだん ふっかつの ひ まで\n"
	.string "この ジムで\l"
	.string "たいせいを たてなおすのだ!\p"
	.string "しかし… きみに\n"
	.string "みつかって しまっては\l"
	.string "しょうがない!\l"
	.string "こんどは てかげん なしだ!\l"
	.string "では…\p"
	.string "いま いちど!\n"
	.string "さいきょうの トレーナー\l"
	.string "サカキの うでまえを みよ!{PLAY_BGM MUS_RG_ENCOUNTER_ROCKET}$"

ViridianCity_Gym_Text_GiovanniDefeat::
	.string "{JPN}はー はーッ!\n"
	.string "はげしい たたかい だった!\l"
	.string "きみの かちだ!\l"
	.string "いま…\l"
	.string "その あかしに\l"
	.string "グリーン バッジを わたそう!{PAUSE_MUSIC}{PLAY_BGM MUS_OBTAIN_BADGE}{PAUSE 254}{PAUSE 86}{RESUME_MUSIC}$"

ViridianCity_Gym_Text_GiovanniPostBattle::
	.string "{JPN}このような まけかたを しては\n"
	.string "ぶかたちに しめしが つかない!\l"
	.string "ロケットだんは…\l"
	.string "ほんじつを もって かいさんする!\p"
	.string "わたしは\n"
	.string "ポケモンの しゅぎょうを\l"
	.string "いちから しなおす つもりだ!\p"
	.string "いつの ひか…\n"
	.string "また あおう!\l"
	.string "… さらばだ!$"

ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis::
	.string "{JPN}グリーンバッジさえ あれば\n"
	.string "どんなに レベルの たかい\l"
	.string "ポケモンも いうことを きく!\p"
	.string "きみが いちにんまえの\n"
	.string "トレーナーに なった あかしだ!\p"
	.string "これで ポケモンリーグに\n"
	.string "いどむ ことが できるだろう\p"
	.string "この わざマシンは…!\n"
	.string "ポケモンリーグへ いどむ\l"
	.string "きみへ せんべつだ!$"

ViridianCity_Gym_Text_ReceivedTM26FromGiovanni::
	.string "{JPN}{PLAYER}は サカキから\n"
	.string "わざマシン26を もらった!$"

ViridianCity_Gym_Text_ExplainTM26::
	.string "{JPN}わざマシン26は じしん だ!\n"
	.string "じめんを つよく ゆらし\l"
	.string "てきに だいダメージを あたえる!\l"
	.string "きょうりょくな わざだ!\p"
	.string "おれが むかし… ここで\n"
	.string "ポケモン ジムを やってる とき\l"
	.string "つくった ものだ…$"

ViridianCity_Gym_Text_YouDoNotHaveSpace::
	.string "{JPN}にもつが いっぱいだ!$"

ViridianCity_Gym_Text_YujiIntro::
	.string "{JPN}… さて!\n"
	.string "そろそろ…\l"
	.string "バテて きたんじゃないか$"

ViridianCity_Gym_Text_YujiDefeat::
	.string "{JPN}… ちから つきた$"

ViridianCity_Gym_Text_YujiPostBattle::
	.string "{JPN}たいりょくが ないと\n"
	.string "とても リーダーには かてないぜ!$"

ViridianCity_Gym_Text_AtsushiIntro::
	.string "{JPN}うおーッ!\n"
	.string "おれの イカリは\l"
	.string "さいこうちょう だあッ!$"

ViridianCity_Gym_Text_AtsushiDefeat::
	.string "{JPN}ぬうおーッ!$"

ViridianCity_Gym_Text_AtsushiPostBattle::
	.string "{JPN}ウオースッ!\n"
	.string "まだまだ れんしゅう たりないッス$"

ViridianCity_Gym_Text_JasonIntro::
	.string "{JPN}おれと ポケモンの\n"
	.string "コンビネーション!\l"
	.string "バツグン だぜ!$"

ViridianCity_Gym_Text_JasonDefeat::
	.string "{JPN}… おまえと\n"
	.string "ポケモンも いきが あってるな$"

ViridianCity_Gym_Text_JasonPostBattle::
	.string "{JPN}おまえは ここの リーダーが\n"
	.string "だれか しって いるのか…?$"

ViridianCity_Gym_Text_KiyoIntro::
	.string "{JPN}カラテ こそ!\n"
	.string "せかい さいきょうの かくとうぎ!$"

ViridianCity_Gym_Text_KiyoDefeat::
	.string "{JPN}チェーストー!$"

ViridianCity_Gym_Text_KiyoPostBattle::
	.string "{JPN}ウィオッス!\n"
	.string "ポケモンが おれ くらい\l"
	.string "カラテ できれば いいんだが…$"

ViridianCity_Gym_Text_WarrenIntro::
	.string "{JPN}しんに つよい トレーナーは\n"
	.string "かちかたも うつくしい ものだ!$"

ViridianCity_Gym_Text_WarrenDefeat::
	.string "{JPN}てが すべった…!$"

ViridianCity_Gym_Text_WarrenPostBattle::
	.string "{JPN}こんな まけかた したら…\n"
	.string "… リーダーに おこられる!$"

ViridianCity_Gym_Text_TakashiIntro::
	.string "{JPN}キング オブ カラテ!\l"
	.string "ここから さきへは\l"
	.string "いかせないぜ!$"

ViridianCity_Gym_Text_TakashiDefeat::
	.string "{JPN}ドッセーッ!$"

ViridianCity_Gym_Text_TakashiPostBattle::
	.string "{JPN}ウィース!\n"
	.string "ポケモン リーグ めざしてるのか!\l"
	.string "ませた こどもだ!$"

ViridianCity_Gym_Text_ColeIntro::
	.string "{JPN}ピシ ピシッ!\n"
	.string "ほら!\l"
	.string "ビビってるんじゃないよ!$"

ViridianCity_Gym_Text_ColeDefeat::
	.string "{JPN}… い いてて!$"

ViridianCity_Gym_Text_ColePostBattle::
	.string "{JPN}まあ まて!\n"
	.string "ゆだん した だけだ!$"

ViridianCity_Gym_Text_SamuelIntro::
	.string "{JPN}トキワ ジムは\n"
	.string "ずっと へいさ していたが\l"
	.string "リーダーが かえってきて\l"
	.string "きょうから ふっかつだ!$"

ViridianCity_Gym_Text_SamuelDefeat::
	.string "{JPN}… さすがだ!$"

ViridianCity_Gym_Text_SamuelPostBattle::
	.string "{JPN}きみが…\n"
	.string "トキワ リーダーに かてれば\l"
	.string "ポケモンリーグに すすめる だろう$"

ViridianCity_Gym_Text_GymGuyAdvice::
	.string "{JPN}おーす!\n"
	.string "みらいの チャンピオン!\p"
	.string "トキワ リーダーの しょうたいは\n"
	.string "おれにも わからん!\p"
	.string "たしかなのは\n"
	.string "いままでの リーダーの\l"
	.string "だれよりも つよいって ことだ!\p"
	.string "それと…\n"
	.string "どうも この ジムには\l"
	.string "じめん タイプ ポケモンの\l"
	.string "つかいてが あつまってる らしい$"

ViridianCity_Gym_Text_GymGuyPostVictory::
	.string "{JPN}たまげたぜ!\n"
	.string "トキワの リーダーが\l"
	.string "サカキ だったとは!$"

ViridianCity_Gym_Text_GymStatue::
	.string "{JPN}トキワ ポケモン ジム\n"
	.string "??? にんてい トレーナー!\l"
	.string "{RIVAL}$"

ViridianCity_Gym_Text_GymStatuePlayerWon::
	.string "{JPN}トキワ ポケモン ジム\n"
	.string "サカキ にんてい トレーナー!\l"
	.string "{RIVAL} {PLAYER}$"

