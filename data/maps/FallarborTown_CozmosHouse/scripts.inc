FallarborTown_CozmosHouse_MapScripts::
	.byte 0

FallarborTown_CozmosHouse_EventScript_ProfCozmo::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_RETURN, FallarborTown_CozmosHouse_EventScript_GaveMeteorite
	checkitem ITEM_METEORITE
	goto_if_eq VAR_RESULT, TRUE, FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite
	msgbox FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_PlayerHasMeteorite::
	call_if_unset FLAG_TEMP_2, FallarborTown_CozmosHouse_EventScript_NoticeMeteorite
	call_if_set FLAG_TEMP_2, FallarborTown_CozmosHouse_EventScript_AskForMeteorite
	goto_if_eq VAR_RESULT, NO, FallarborTown_CozmosHouse_EventScript_DeclineGiveMeteorite
	msgbox FallarborTown_CozmosHouse_Text_PleaseUseThisTM, MSGBOX_DEFAULT
	giveitem ITEM_TM_RETURN
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setvar VAR_0x8004, ITEM_METEORITE
	call Common_EventScript_PlayerHandedOverTheItem
	setflag FLAG_RECEIVED_TM_RETURN
	msgbox FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_NoticeMeteorite::
	msgbox FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow, MSGBOX_DEFAULT
	msgbox FallarborTown_CozmosHouse_Text_IsThatMeteoriteMayIHaveIt, MSGBOX_YESNO
	return

FallarborTown_CozmosHouse_EventScript_AskForMeteorite::
	msgbox FallarborTown_CozmosHouse_Text_MayIHaveMeteorite, MSGBOX_YESNO
	return

FallarborTown_CozmosHouse_EventScript_DeclineGiveMeteorite::
	setflag FLAG_TEMP_2
	msgbox FallarborTown_CozmosHouse_Text_CrushedWithDisappointment, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_GaveMeteorite::
	msgbox FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmosWife::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_RETURN, FallarborTown_CozmosHouse_EventScript_CozmoIsHappy
	goto_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, FallarborTown_CozmosHouse_EventScript_CozmoIsSad
	msgbox FallarborTown_CozmosHouse_Text_CozmoWentToMeteorFalls, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmoIsSad::
	msgbox FallarborTown_CozmosHouse_Text_FeelSorryForCozmo, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_EventScript_CozmoIsHappy::
	msgbox FallarborTown_CozmosHouse_Text_CozmoIsSoHappy, MSGBOX_DEFAULT
	release
	end

FallarborTown_CozmosHouse_Text_MeteoriteWillNeverBeMineNow:
	.string "{JPN}ソライシ『ああ……\nマグマだんに だまされたからと いって\lいんせきが てに はいる ばしょを\lおしえてしまったら だめ だなあ……\pりゅうせいのたき の いんせき\nもう ぼくの モノには ならないよな……$"

FallarborTown_CozmosHouse_Text_IsThatMeteoriteMayIHaveIt:
	.string "{JPN}おお! ええ?\pそれは……\pもしかして もしかすると……\pマグマだんが りゅうせいのたき から\nもっていった いんせき ですか……!?\pよかったら ぼくに ください!\pただで とは いいません\nかわりに この わざマシンを\lあげますから……$"

FallarborTown_CozmosHouse_Text_PleaseUseThisTM:
	.string "{JPN}ソライシ『この わざマシンは\nぼくの きもちです\lぜひ つかってください!$"

FallarborTown_CozmosHouse_Text_ReallyGoingToHelpMyResearch:
	.string "{JPN}ソライシ『いやー すごく うれしいな\nこれで ぼくの けんきゅうも はかどります!$"

FallarborTown_CozmosHouse_Text_CrushedWithDisappointment:
	.string "{JPN}ソライシ『そんな……\nものすごく がっかりだよ……$"

FallarborTown_CozmosHouse_Text_MayIHaveMeteorite:
	.string "{JPN}ソライシ『その いんせき……\nよかったら ぼくに ください!\pただで とは いいません\nかわりに この わざマシンを\lあげますから……$"

FallarborTown_CozmosHouse_Text_CozmoWentToMeteorFalls:
	.string "{JPN}ソライシはかせは\nマグマだんの ひとと いっしょに\l114ばん どうろに ある\lりゅうせいのたきに いきましたよ$"

FallarborTown_CozmosHouse_Text_FeelSorryForCozmo:
	.string "{JPN}ソライシはかせ……\nあんなに おちこんで かわいそう!$"

FallarborTown_CozmosHouse_Text_CozmoIsSoHappy:
	.string "{JPN}ソライシはかせ ったら\nあんなに よろこんで かわいい!$"
