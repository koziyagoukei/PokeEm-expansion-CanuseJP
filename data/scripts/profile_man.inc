ProfileMan_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_SYS_CHAT_USED, ProfileMan_EventScript_GivenProfileBefore
	msgbox ProfileMan_Text_CollectTrainerProfiles, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeProfile
	end

ProfileMan_EventScript_AskToSeeProfile::
	msgbox ProfileMan_Text_MayISeeYourProfile, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, ProfileMan_EventScript_CreateProfile
	case 1, ProfileMan_EventScript_DeclineShowProfile
	case 2, ProfileMan_EventScript_Info
	case MULTI_B_PRESSED, ProfileMan_EventScript_DeclineShowProfile
	end

ProfileMan_EventScript_Info::
	msgbox ProfileMan_Text_EasyChatExplanation, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeProfile
	end

ProfileMan_EventScript_CreateProfile::
	msgbox ProfileMan_Text_LetsSeeItThen, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, ProfileMan_EventScript_CancelShowProfile
	goto_if_eq VAR_RESULT, 1, ProfileMan_EventScript_ShowProfile
	end

ProfileMan_EventScript_CancelShowProfile::
	msgbox ProfileMan_Text_NotIntoItRightNow, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_ShowProfile::
	setvar VAR_0x8004, 0
	special ShowEasyChatProfile
	waitmessage
	delay 80
	msgbox ProfileMan_Text_FantasticProfile, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_DeclineShowProfile::
	msgbox ProfileMan_Text_ImagineYouWouldHaveWonderfulProfile, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_GivenProfileBefore::
	msgbox ProfileMan_Text_YouHaveWonderfulSmile, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeNewProfile
	end

ProfileMan_EventScript_AskToSeeNewProfile::
	msgbox ProfileMan_Text_MayISeeYourNewProfile, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, ProfileMan_EventScript_CreateNewProfile
	case 1, ProfileMan_EventScript_DeclineNewProfile
	case 2, ProfileMan_EventScript_InfoNewProfile
	case MULTI_B_PRESSED, ProfileMan_EventScript_DeclineNewProfile
	end

ProfileMan_EventScript_InfoNewProfile::
	msgbox ProfileMan_Text_EasyChatExplanation, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeNewProfile
	end

ProfileMan_EventScript_CreateNewProfile::
	msgbox ProfileMan_Text_EvenBetterThanLastProfile, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, ProfileMan_EventScript_CancelShowProfile
	goto_if_eq VAR_RESULT, 1, ProfileMan_EventScript_ShowProfile
	end

ProfileMan_EventScript_DeclineNewProfile::
	msgbox ProfileMan_Text_LikeProfileWayItIs, MSGBOX_DEFAULT
	release
	end

@ Unused
ProfileMan_EventScript_Ret::
	return

ProfileMan_Text_CollectTrainerProfiles:
	.string "{JPN}こんにちは\nえがおの すてきな トレーナーさん!\pぼくは ポケモントレーナーさん たちの\nプロフィールを あつめるのが\lしゅみ なんだけど……$"

ProfileMan_Text_MayISeeYourProfile:
	.string "{JPN}どうかな? きみの プロフィール\nぼくに おしえて もらえないかな?$"

ProfileMan_Text_EasyChatExplanation:
	.string "{JPN}プロフィールは すきな ことばを\n4つ くみあわせて つくるんだよ!\pこれから ぼくが\nてきとうな 4つの ことばで つくった\lプロフィールを みせるから\pきみは その ことばを\nすきなものに いれかえて\lきみだけの プロフィールに してね!\pえらべる ことばは\nすごく たくさん あるから\lグループに わかれて いるんだ\lポケモン とか せいかつ とか しゅみ…\lという ぐあいにね!\pだから まずは\nグループを えらんで……\lそのあとに えらんだ グループの\lことばが いくつも でてくるから\lそこから すきな ことばを\lみつけてね!$"

ProfileMan_Text_LetsSeeItThen:
	.string "{JPN}やった! ありがとう!\nじゃ さっそく きかせてよ!$"

ProfileMan_Text_ImagineYouWouldHaveWonderfulProfile:
	.string "{JPN}そんなあ……\pきみの プロフィールは さぞかし\nすてきだと おもうんだけど なぁ……$"

ProfileMan_Text_NotIntoItRightNow:
	.string "{JPN}あれ? いまは きが のらないんだね?\pだったら いつでも いいよ!$"

ProfileMan_Text_YouHaveWonderfulSmile:
	.string "{JPN}こんにちは\nえがおの すてきな トレーナーさん!$"

ProfileMan_Text_MayISeeYourNewProfile:
	.string "{JPN}きみの あたらしい プロフィールを\nおしえて くれるのかな?$"

ProfileMan_Text_EvenBetterThanLastProfile:
	.string "{JPN}やった! ありがとう!\pじゃあ まえにも まして\nすてきな やつを きたい してるよ!$"

ProfileMan_Text_LikeProfileWayItIs:
	.string "{JPN}いまの ままが いいんだね?\pそうだよね とても すてきな\nプロフィール だもの!$"

ProfileMan_Text_FantasticProfile:
	.string "{JPN}すっ すばらしい!\pまさに きみ そのもの って かんじの\nすてきな プロフィール!\pこれを きいた ひとは\nだれもが きみの とりこに なるだろうね!\pどうも ありがとう!$"

@ Unused
ProfileMan_Text_YouKnowSecretSaying:
	.string "{JPN}ややっ!\nその ことばを しっている とは……\pという ことは きみも\nふしぎな できごと クラブの なかま だね!$"
