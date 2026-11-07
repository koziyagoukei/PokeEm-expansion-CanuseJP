ViridianCity_School_Frlg_MapScripts::
	.byte 0

ViridianCity_School_EventScript_Lass::
	lock
	faceplayer
	msgbox ViridianCity_School_Text_TryingToMemorizeNotes
	closemessage
	applymovement LOCALID_SCHOOL_LASS, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

ViridianCity_School_EventScript_Woman::
	lock
	faceplayer
	msgbox ViridianCity_School_Text_ReadBlackboardCarefully
	closemessage
	applymovement LOCALID_SCHOOL_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_School_EventScript_Notebook::
	lockall
	msgbox ViridianCity_School_Text_NotebookFirstPage
	msgbox ViridianCity_School_Text_TurnThePage, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_School_EventScript_StopReadingNotebook
	msgbox ViridianCity_School_Text_NotebookSecondPage
	msgbox ViridianCity_School_Text_TurnThePage, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_School_EventScript_StopReadingNotebook
	msgbox ViridianCity_School_Text_NotebookThirdPage
	msgbox ViridianCity_School_Text_TurnThePage, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_School_EventScript_StopReadingNotebook
	msgbox ViridianCity_School_Text_NotebookFourthPage
	applymovement LOCALID_SCHOOL_LASS, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox ViridianCity_School_Text_HeyDontLookAtMyNotes
	releaseall
	end

ViridianCity_School_EventScript_StopReadingNotebook::
	releaseall
	end

ViridianCity_School_EventScript_Blackboard::
	lockall
	msgbox ViridianCity_School_Text_BlackboardListsStatusProblems
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ChooseBlackboardTopic::
	message ViridianCity_School_Text_ReadWhichTopic
	waitmessage
	multichoicegrid 7, 1, MULTI_STATUS_INFO, 3, FALSE
	switch VAR_RESULT
	case 0, ViridianCity_School_EventScript_ReadSleep
	case 1, ViridianCity_School_EventScript_ReadPoison
	case 2, ViridianCity_School_EventScript_ReadParalysis
	case 3, ViridianCity_School_EventScript_ReadBurn
	case 4, ViridianCity_School_EventScript_ReadFreeze
	case 5, ViridianCity_School_EventScript_ExitBlackboard
	case 127, ViridianCity_School_EventScript_ExitBlackboard
	end

ViridianCity_School_EventScript_ReadSleep::
	msgbox ViridianCity_School_Text_ExplainSleep
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadBurn::
	msgbox ViridianCity_School_Text_ExplainBurn
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadPoison::
	msgbox ViridianCity_School_Text_ExplainPoison
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadFreeze::
	msgbox ViridianCity_School_Text_ExplainFreeze
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadParalysis::
	msgbox ViridianCity_School_Text_ExplainParalysis
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ExitBlackboard::
	releaseall
	end

ViridianCity_School_Text_TryingToMemorizeNotes::
	.string "{JPN}ふー\n"
	.string "ノートの なかの こと\l"
	.string "わたし ぜんぶ おぼえるの$"

ViridianCity_School_Text_ReadBlackboardCarefully::
	.string "{JPN}はい!\p"
	.string "こくばんに かかれてる こと\n"
	.string "ちゃんと みて!$"

ViridianCity_School_Text_NotebookFirstPage::
	.string "{JPN}ノートを のぞいた!\p"
	.string "1ページめ …\p"
	.string "ポケモンは\n"
	.string "モンスターボールで つかまえて\l"
	.string "6つまで もち あるける\p"
	.string "ポケモンを そだてて たたかわせる\n"
	.string "ひとを ポケモン トレーナーという$"

ViridianCity_School_Text_NotebookSecondPage::
	.string "{JPN}2ページめ …\p"
	.string "ポケモンを つかまえる ときは\n"
	.string "まず よわらせてから!\l"
	.string "どくや やけどで\p"
	.string "よわらせても いいよ!\n"
	.string "げんきだと うまく とれないかも$"

ViridianCity_School_Text_NotebookThirdPage::
	.string "{JPN}3ページめ …\p"
	.string "ポケモンどうしを たたかわせ\n"
	.string "かつことは トレーナーに とって\l"
	.string "さいこうの よろこび である!\p"
	.string "かくちの ポケモン ジムでは\n"
	.string "いつも はげしい\l"
	.string "たたかいが くりひろげられてる!$"

ViridianCity_School_Text_NotebookFourthPage::
	.string "{JPN}4ページめ …\p"
	.string "ポケモントレーナーの もくひょうは\n"
	.string "かくちの ジムに いる\p"
	.string "つよい トレーナー\n"
	.string "8にんしゅうを たおすこと!\p"
	.string "さらに ポケモンリーグ ほんぶには\n"
	.string "… もうれつに つよい!\p"
	.string "してんのうが くんりん してる!$"

ViridianCity_School_Text_TurnThePage::
	.string "{JPN}つぎのページ あけちゃおうか?$"

ViridianCity_School_Text_HeyDontLookAtMyNotes::
	.string "{JPN}おんなのこ『あー!\n"
	.string "ノート みちゃ だめ!$"

ViridianCity_School_Text_BlackboardListsStatusProblems::
	.string "{JPN}こくばんに\n"
	.string "ポケモンが たたかってる とき\l"
	.string "おこる たいちょうの\l"
	.string "へんかに ついて かかれてる$"

ViridianCity_School_Text_ReadWhichTopic::
	.string "{JPN}どの こうもくを みますか?$"

ViridianCity_School_Text_ExplainSleep::
	.string "{JPN}ねてしまうと その あいだは\n"
	.string "こうげき できません!\p"
	.string "たたかい おわっても ねたまま\n"
	.string "ねむけざましで めが さめます$"

ViridianCity_School_Text_ExplainBurn::
	.string "{JPN}やけどは たいりょくが へって\n"
	.string "こうげきりょくが\l"
	.string "さがって しまいます!\p"
	.string "たたかい おわっても\n"
	.string "やけどは のこります\l"
	.string "また やけどなおしで なおります$"

ViridianCity_School_Text_ExplainPoison::
	.string "{JPN}どくを くらうと\n"
	.string "たいりょくが へって いきます\p"
	.string "たたかい おわっても\n"
	.string "どくは のこりますが\l"
	.string "どくけしで きえます!$"

ViridianCity_School_Text_ExplainFreeze::
	.string "{JPN}こおって しまうと\n"
	.string "まったく うごけなく なります!\p"
	.string "たたかい おわっても こおってます\n"
	.string "こおりなおしで とかさないと\l"
	.string "ポケモンが かわいそう$"

ViridianCity_School_Text_ExplainParalysis::
	.string "{JPN}まひすると わざが\n"
	.string "ときどき でなく なり\l"
	.string "すばやさも さがって しまいます!\p"
	.string "たたかい おわっても\n"
	.string "まひは のこります\l"
	.string "また まひなおしで なおります$"

