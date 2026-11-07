Route24_Frlg_MapScripts::
	.byte 0

Route24_EventScript_Rocket::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ROUTE24, 1, Route24_EventScript_RocketPostBattle
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNugget
	call Route24_EventScript_BattleRocket
	release
	end

Route24_EventScript_NoRoomForNugget::
	msgbox Route24_Text_YouDontHaveAnyRoom
	release
	end

Route24_EventScript_RocketPostBattle::
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	release
	end

Route24_EventScript_RocketTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTriggerRight::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketApproachPlayer
	call_if_eq VAR_TEMP_1, 1, Route24_EventScript_RocketMotionToPlayer
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNuggetTrigger
	call Route24_EventScript_BattleRocket
	releaseall
	end

Route24_EventScript_BattleRocket::
	giveitem_msg Route24_Text_ReceivedNuggetFromMysteryTrainer, ITEM_NUGGET
	message Route24_Text_JoinTeamRocket
	waitmessage
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_ROUTE24_ROCKET
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_6, Route24_Text_RocketDefeat
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	setvar VAR_MAP_SCENE_ROUTE24, 1
	return

Route24_EventScript_RocketApproachPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketApproachPlayer
	waitmovement 0
	return

Route24_EventScript_RocketMotionToPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

Route24_EventScript_NoRoomForNuggetTrigger::
	msgbox Route24_Text_YouDontHaveAnyRoom
	closemessage
	applymovement LOCALID_PLAYER, Route24_Movement_WalkDown
	waitmovement 0
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketWalkBackToPos
	release
	end

Route24_EventScript_RocketWalkBackToPos::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketWalkBackToPos
	waitmovement 0
	return

Route24_Movement_RocketApproachPlayer::
	walk_left
	step_end

Route24_Movement_RocketWalkBackToPos::
	walk_right
	walk_in_place_faster_left
	step_end

Route24_Movement_WalkDown::
	walk_down
	step_end

Route24_Text_JustEarnedFabulousPrize::
	.string "{JPN}おみごとーッ!\n"
	.string "5にんぬき おめでとう!\p"
	.string "しょうひんに\n"
	.string "これを プレゼント しよう!$"

Route24_Text_ReceivedNuggetFromMysteryTrainer::
	.string "{JPN}{PLAYER}は なぞのトレーナーから\n"
	.string "きんのたまを もらった!$"

Route24_Text_YouDontHaveAnyRoom::
	.string "{JPN}もちものが いっぱいだ$"

Route24_Text_JoinTeamRocket::
	.string "{JPN}ところで… ここだけの はなし\n"
	.string "ロケットだんに はいらない?\p"
	.string "おれたちは\n"
	.string "ポケモンを わるいことに\l"
	.string "つかおうっていう グループだ!\p"
	.string "はいりなよ\p"
	.string "はいらないの?\p"
	.string "はいってよ!\p"
	.string "はいれよ!\p"
	.string "… ことわるって かお してんな\p"
	.string "それなら…!\n"
	.string "むりやり いれて やる!\l"
	.string "うりゃーッ!$"

Route24_Text_RocketDefeat::
	.string "{JPN}おまえ…!\n"
	.string "ホント つええな!$"

Route24_Text_YoudBecomeTopRocketLeader::
	.string "{JPN}それだけの うでが あれば\n"
	.string "ロケットだん でも\l"
	.string "えらく なれるのに もったいないぜ!$"

Route24_Text_ShaneIntro::
	.string "{JPN}ふふ…ッ! くさむらから\n"
	.string "きみの かつやくを みてたよ$"

Route24_Text_ShaneDefeat::
	.string "{JPN}… やっぱり だめだ$"

Route24_Text_ShanePostBattle::
	.string "{JPN}… はしを わたるのが こわくて\n"
	.string "ずっと ここに かくれてたんだ$"

Route24_Text_EthanIntro::
	.string "{JPN}きえーいッ! 5にんめ!\n"
	.string "おれが あいてだあ!$"

Route24_Text_EthanDefeat::
	.string "{JPN}たいした もんだ!$"

Route24_Text_EthanPostBattle::
	.string "{JPN}ベストを つくしたんだ\n"
	.string "くいは ない!$"

Route24_Text_ReliIntro::
	.string "{JPN}4にんめは わたしよ!\n"
	.string "そろそろ ばてて\l"
	.string "きたんじゃない?$"

Route24_Text_ReliDefeat::
	.string "{JPN}わたしも まけたのね$"

Route24_Text_ReliPostBattle::
	.string "{JPN}ベストを つくしたんだから\n"
	.string "くいは ないわ!$"

Route24_Text_TimmyIntro::
	.string "{JPN}3にんめ とうじょう!\n"
	.string "そう かんたんには いかないぜ!$"

Route24_Text_TimmyDefeat::
	.string "{JPN}へろへろ もう ダメ$"

Route24_Text_TimmyPostBattle::
	.string "{JPN}ベストを つくしたんだ\n"
	.string "くいは ない!$"

Route24_Text_AliIntro::
	.string "{JPN}ふたりめは わたし!\n"
	.string "これからが ほんばん よ!$"

Route24_Text_AliDefeat::
	.string "{JPN}しんじらんなーい$"

Route24_Text_AliPostBattle::
	.string "{JPN}ベストを つくしたんだから\n"
	.string "くいは ないわ!$"

Route24_Text_CaleIntro::
	.string "{JPN}この はしは ひと よんで\n"
	.string "ゴールデンボール ブリッジ!\p"
	.string "5にん かちぬけば\n"
	.string "ごうかな しょうひんが もらえる!\p"
	.string "さて!\n"
	.string "おまえに ぬけられるかな?$"

Route24_Text_CaleDefeat::
	.string "{JPN}なかなか やるじゃん$"

Route24_Text_CalePostBattle::
	.string "{JPN}ベストを つくしたんだ\n"
	.string "くいは ない!$"
