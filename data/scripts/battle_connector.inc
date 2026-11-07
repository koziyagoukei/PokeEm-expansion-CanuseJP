@ Battle Connector (src/battle_connector.c): scripts shared by its maps. The reception is in Artisan Cave 1F.

BattleConnector_EventScript_TurnPlayerNorth::
	setvar VAR_TEMP_4, 1
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

BattleConnector_Movement_StepDown:
	walk_down
	step_end

@ Losing a battle ends the run: the party comes back as it was
BattleConnector_EventScript_Lost::
	special BattleConnector_FailChallenge
	warp MAP_ARTISAN_CAVE_1F, 6, 12
	waitstate
	end

BattleConnector_EventScript_Retire::
	special BattleConnector_RetireChallenge
	closemessage
	warp MAP_ARTISAN_CAVE_1F, 6, 12
	waitstate
	end

@ The reception (Artisan Cave 1F): the challenge, and what happens on coming back
BattleConnector_EventScript_Receptionist::
	lock
	faceplayer
	msgbox BattleConnector_Text_Welcome, MSGBOX_DEFAULT
	msgbox BattleConnector_Text_AskChallenge, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_EventScript_Decline
	@ The mode: Origin Edition, Another Edition (easy), or Panic Rush
	message BattleConnector_Text_ChooseMode
	waitmessage
	dynmultichoice 0, 0, FALSE, 4, 0, DYN_MULTICHOICE_CB_NONE, BattleConnector_Text_ModeOrigin, BattleConnector_Text_ModeAnother, BattleConnector_Text_ModePanic, BattleConnector_Text_ModeCancel
	switch VAR_RESULT
	case 0, BattleConnector_EventScript_ModeOrigin
	case 1, BattleConnector_EventScript_ModeAnother
	case 2, BattleConnector_EventScript_PanicRush
	goto BattleConnector_EventScript_Decline
	end

BattleConnector_EventScript_ModeOrigin::
	setvar VAR_0x8006, CONNECTOR_MODE_ORIGIN
	goto BattleConnector_EventScript_Challenge
	end

BattleConnector_EventScript_ModeAnother::
	setvar VAR_0x8006, CONNECTOR_MODE_ANOTHER
	msgbox BattleConnector_Text_AnotherRules, MSGBOX_DEFAULT
	goto BattleConnector_EventScript_Challenge
	end

BattleConnector_EventScript_Challenge::
	specialvar VAR_RESULT, BattleConnector_HasRoomToTakeHome
	goto_if_eq VAR_RESULT, FALSE, BattleConnector_EventScript_NoRoom
	msgbox BattleConnector_Text_Rules, MSGBOX_DEFAULT
	msgbox BattleConnector_Text_ChooseMon, MSGBOX_DEFAULT
	closemessage
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, BattleConnector_EventScript_Decline
	@ A second Pokemon (optional): VAR_0x8005, PARTY_SIZE for none
	setvar VAR_0x8005, PARTY_SIZE
	copyvar VAR_0x8007, VAR_0x8004
	msgbox BattleConnector_Text_AskSecondMon, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_EventScript_StartChallenge
	special ChoosePartyMon
	copyvar VAR_0x8005, VAR_0x8004
	copyvar VAR_0x8004, VAR_0x8007
	goto_if_ge VAR_0x8005, PARTY_SIZE, BattleConnector_EventScript_StartChallenge
	goto_if_eq VAR_0x8005, VAR_0x8004, BattleConnector_EventScript_SameMon
BattleConnector_EventScript_StartChallenge::
	specialvar VAR_RESULT, BattleConnector_StartChallenge
	goto_if_eq VAR_RESULT, FALSE, BattleConnector_EventScript_CantBring
	msgbox BattleConnector_Text_GoodLuck, MSGBOX_DEFAULT
	closemessage
	warp MAP_BATTLE_CONNECTOR_BRANCH, 6, 9
	waitstate
	end

@ Panic Rush: BP for a chain of double battles against Pokemon in a panic (src/battle_connector.c)
BattleConnector_EventScript_PanicRush::
	msgbox BattleConnector_Text_PanicRules, MSGBOX_DEFAULT
	specialvar VAR_RESULT, BattleConnector_CanStartPanicRush
	goto_if_eq VAR_RESULT, CONNECTOR_PANIC_NO_BP, BattleConnector_EventScript_PanicNoBP
	goto_if_eq VAR_RESULT, CONNECTOR_PANIC_FEW_POKEMON, BattleConnector_EventScript_PanicFewPokemon
	msgbox BattleConnector_Text_PanicAsk, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_EventScript_Decline
	special BattleConnector_PanicResetPicks
BattleConnector_EventScript_PanicPick::
	msgbox BattleConnector_Text_PanicChoose, MSGBOX_DEFAULT
	closemessage
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, BattleConnector_EventScript_Decline
	specialvar VAR_RESULT, BattleConnector_PanicPick
	goto_if_eq VAR_RESULT, 0, BattleConnector_EventScript_PanicPickedAlready
	goto_if_lt VAR_RESULT, BATTLE_CONNECTOR_PANIC_PICKS, BattleConnector_EventScript_PanicPick
	specialvar VAR_RESULT, BattleConnector_StartPanicRush
	goto_if_eq VAR_RESULT, FALSE, BattleConnector_EventScript_CantBring
	playse SE_SHOP
	msgbox BattleConnector_Text_PanicGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_BATTLE_CONNECTOR_ROOM, 4, 7
	waitstate
	end

BattleConnector_EventScript_PanicPickedAlready::
	msgbox BattleConnector_Text_PanicPickedAlready, MSGBOX_DEFAULT
	goto BattleConnector_EventScript_PanicPick
	end

BattleConnector_EventScript_PanicNoBP::
	msgbox BattleConnector_Text_PanicNoBP, MSGBOX_DEFAULT
	release
	end

BattleConnector_EventScript_PanicFewPokemon::
	msgbox BattleConnector_Text_PanicFewPokemon, MSGBOX_DEFAULT
	release
	end

BattleConnector_EventScript_ReturnPanic::
	lockall
	msgbox BattleConnector_Text_ReturnPanic, MSGBOX_DEFAULT
	releaseall
	end

@ A Pokemon taken home needs a place: the party or the PC
BattleConnector_EventScript_NoRoom::
	msgbox BattleConnector_Text_NoRoom, MSGBOX_DEFAULT
	release
	end

BattleConnector_EventScript_SameMon::
	msgbox BattleConnector_Text_SameMon, MSGBOX_DEFAULT
	release
	end

BattleConnector_EventScript_Decline::
	msgbox BattleConnector_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

BattleConnector_EventScript_CantBring::
	msgbox BattleConnector_Text_CantBring, MSGBOX_DEFAULT
	release
	end

BattleConnector_EventScript_CheckReturn::
	setvar VAR_TEMP_0, 1
	specialvar VAR_RESULT, BattleConnector_GetReturnState
	goto_if_eq VAR_RESULT, CONNECTOR_RETURN_NONE, BattleConnector_EventScript_NoReturn
	turnobject LOCALID_PLAYER, DIR_NORTH
	switch VAR_RESULT
	case CONNECTOR_RETURN_CLEARED, BattleConnector_EventScript_Results
	case CONNECTOR_RETURN_FAILED, BattleConnector_EventScript_ReturnFailed
	case CONNECTOR_RETURN_RETIRED, BattleConnector_EventScript_ReturnRetired
	case CONNECTOR_RETURN_PANIC, BattleConnector_EventScript_ReturnPanic
	end

BattleConnector_EventScript_NoReturn::
	end

BattleConnector_EventScript_ReturnFailed::
	lockall
	msgbox BattleConnector_Text_Failed, MSGBOX_DEFAULT
	releaseall
	end

BattleConnector_EventScript_ReturnRetired::
	lockall
	msgbox BattleConnector_Text_Retired, MSGBOX_DEFAULT
	releaseall
	end

@ The results: one of the protected Pokemon goes home with the player
BattleConnector_EventScript_Results::
	lockall
	msgbox BattleConnector_Text_ResultsIntro, MSGBOX_DEFAULT
	specialvar VAR_RESULT, BattleConnector_GetCandidateCount
	goto_if_eq VAR_RESULT, 0, BattleConnector_EventScript_ResultsNone
	msgbox BattleConnector_Text_ResultsRule, MSGBOX_DEFAULT
BattleConnector_EventScript_ResultsChoose:
	msgbox BattleConnector_Text_ResultsChoose, MSGBOX_DEFAULT
	closemessage
	special BattleConnector_ShowCandidates
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, BattleConnector_EventScript_ResultsCancel
	special BattleConnector_BufferCandidateDetails
	msgbox gStringVar4, MSGBOX_DEFAULT
	msgbox BattleConnector_Text_ResultsConfirm, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_EventScript_ResultsChoose
	specialvar VAR_RESULT, BattleConnector_TakeHomeCandidate
	goto_if_eq VAR_RESULT, MON_CANT_GIVE, BattleConnector_EventScript_ResultsNoRoom
	@ The party is back (no warp follows): the follower must be its own again, not one of the candidates shown
	special UpdateFollowingPokemon
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, BattleConnector_EventScript_ResultsToPC
	playfanfare MUS_OBTAIN_ITEM
	msgbox BattleConnector_Text_ResultsToParty, MSGBOX_DEFAULT
	waitfanfare
	goto BattleConnector_EventScript_ResultsEnd
	end

BattleConnector_EventScript_ResultsNoRoom::
	msgbox BattleConnector_Text_ResultsNoRoom, MSGBOX_DEFAULT
	goto BattleConnector_EventScript_ResultsChoose
	end

BattleConnector_EventScript_ResultsToPC::
	playfanfare MUS_OBTAIN_ITEM
	msgbox BattleConnector_Text_ResultsToPC, MSGBOX_DEFAULT
	waitfanfare
	goto BattleConnector_EventScript_ResultsEnd
	end

BattleConnector_EventScript_ResultsEnd::
	msgbox BattleConnector_Text_ResultsAfter, MSGBOX_DEFAULT
	releaseall
	end

BattleConnector_EventScript_ResultsCancel::
	msgbox BattleConnector_Text_ResultsAskNone, MSGBOX_YESNO
	goto_if_ne VAR_RESULT, YES, BattleConnector_EventScript_ResultsChoose
	special BattleConnector_TakeNothingHome
	special UpdateFollowingPokemon
	msgbox BattleConnector_Text_ResultsNothing, MSGBOX_DEFAULT
	releaseall
	end

BattleConnector_EventScript_ResultsNone::
	special BattleConnector_TakeNothingHome
	special UpdateFollowingPokemon
	msgbox BattleConnector_Text_ResultsNoneProtected, MSGBOX_DEFAULT
	releaseall
	end

BattleConnector_Text_Welcome:
	.string "{JPN}ここは バトルコネクター。\pヒトポケルスに あらがった すえ\nひとや ポケモンとの つながりを たち\lこころを とざした ひとたちと\lダークポケモンが いる ばしょです。\pたたかいを とおして\nもういちど つなぎなおす……\lそれが この しせつの やくめです。$"

BattleConnector_Text_AskChallenge:
	.string "{JPN}ちょうせん しますか？$"

BattleConnector_Text_Rules:
	.string "{JPN}つれていける ポケモンは 2ひきまで。\pなかで たおした ダークポケモンは\nほごして いっしょに たたかえます。\pてもちは あわせて 6ぴきまでです。\pさいごに ほごした ポケモンの なかから\n1ぴきだけ つれて かえれます。\pなかでは レポートを かけません。\nまけても ポケモンは もとどおりです。$"

BattleConnector_Text_ChooseMon:
	.string "{JPN}いっしょに いく ポケモンを\nえらんで ください。$"

BattleConnector_Text_AskSecondMon:
	.string "{JPN}もう 1ぴき\nつれて いきますか？$"

BattleConnector_Text_NoRoom:
	.string "{JPN}てもちも パソコンも いっぱいですね。\pほごした ポケモンを つれて かえれるよう\nどちらかに あきを つくってから\lまた きてください。$"

BattleConnector_Text_ResultsNoRoom:
	.string "{JPN}てもちも パソコンも いっぱいで\nその ポケモンを おくれません……$"

BattleConnector_Text_ChooseMode:
	.string "{JPN}どの モードで ちょうせん しますか？$"

BattleConnector_Text_ModeOrigin:
	.string "{JPN}オリジンエディション$"

BattleConnector_Text_ModeAnother:
	.string "{JPN}アナザーエディション$"

BattleConnector_Text_ModePanic:
	.string "{JPN}パニックラッシュ$"

BattleConnector_Text_ModeCancel:
	.string "{JPN}やめる$"

BattleConnector_Text_AnotherRules:
	.string "{JPN}アナザーエディションは\nかんたんモードです。\pダークポケモンどうしの こうげきも\nふつうに あたります。\pたたかう たびに てもちの\nダークポケモンの こころが\lすこしずつ ひらきます。\pその かわり さいこうの こたいちは\nやくそく できません。$"

BattleConnector_Text_PanicRules:
	.string "{JPN}パニックラッシュでは こころを とざす\nほどでは ないけれど パニックに なった\lポケモンたちを たたかって なだめます。\pさんかには 5BP いただきます。\pてもちから 4ひき えらんで\nダブルバトルが つづきます。\lバッグの どうぐも つかえます。\pなだめた れんさ 1かいに つき\n1BPを おわたし します。$"

BattleConnector_Text_PanicAsk:
	.string "{JPN}5BPで パニックラッシュに\nちょうせん しますか？$"

BattleConnector_Text_PanicChoose:
	.string "{JPN}いっしょに たたかう ポケモンを\n4ひき えらんで ください。$"

BattleConnector_Text_PanicPickedAlready:
	.string "{JPN}その ポケモンは えらべません。$"

BattleConnector_Text_PanicNoBP:
	.string "{JPN}BPが たりない ようです。\nパニックラッシュには 5BP ひつようです。$"

BattleConnector_Text_PanicFewPokemon:
	.string "{JPN}パニックラッシュには ポケモンが\n4ひき ひつようです。$"

BattleConnector_Text_PanicGo:
	.string "{JPN}5BP いただきました。\pそれでは ポケモンたちを\nなだめて あげて ください！$"

BattleConnector_Text_ReturnPanic:
	.string "{JPN}おつかれさまでした！\p{STR_VAR_1}れんさ なだめて くれましたね。\n{STR_VAR_2}BPを どうぞ！$"

BattleConnector_Text_SameMon:
	.string "{JPN}おなじ ポケモンは\n2かい えらべません。$"

BattleConnector_Text_GoodLuck:
	.string "{JPN}それでは いってらっしゃい！$"

BattleConnector_Text_ComeAgain:
	.string "{JPN}また いつでも どうぞ。$"

BattleConnector_Text_CantBring:
	.string "{JPN}その ポケモンは\nつれて いけません。$"

BattleConnector_Text_Failed:
	.string "{JPN}……おつかれさまでした。\pちょうせんは ここまでです。\nポケモンたちは もとに もどしました。$"

BattleConnector_Text_Retired:
	.string "{JPN}おかえりなさい。\nポケモンたちは もとに もどしました。$"

BattleConnector_Text_ResultsIntro:
	.string "{JPN}おかえりなさい！\p……ダークポケモンたちと\nつながれた ようですね。$"

BattleConnector_Text_ResultsRule:
	.string "{JPN}こんかい ほごした ポケモンの なかから\n1ぴきだけ つれて かえれます。$"

BattleConnector_Text_ResultsChoose:
	.string "{JPN}どの ポケモンを\nつれて かえりますか？$"

BattleConnector_Text_ResultsConfirm:
	.string "{JPN}{STR_VAR_1}を\nつれて かえりますか？$"

BattleConnector_Text_ResultsToParty:
	.string "{JPN}{STR_VAR_1}が\nてもちに くわわった！$"

BattleConnector_Text_ResultsToPC:
	.string "{JPN}{STR_VAR_1}は\nパソコンに おくられた！$"

BattleConnector_Text_ResultsAfter:
	.string "{JPN}こころが ひらいたら\nミニミニキャンプで はなしかけて\lあげて くださいね。$"

BattleConnector_Text_ResultsAskNone:
	.string "{JPN}だれも つれて かえらないのですか？$"

BattleConnector_Text_ResultsNothing:
	.string "{JPN}わかりました。\nポケモンたちは わたしたちが あずかります。$"

BattleConnector_Text_ResultsNoneProtected:
	.string "{JPN}こんかいは だれも\nほごできなかった ようですね。\pまた いつでも どうぞ。$"
