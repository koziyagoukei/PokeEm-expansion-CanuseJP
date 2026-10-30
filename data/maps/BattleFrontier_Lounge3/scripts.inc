BattleFrontier_Lounge3_MapScripts::
	.byte 0

	.set BET_AMOUNT_5,  5
	.set BET_AMOUNT_10, 10
	.set BET_AMOUNT_15, 15

BattleFrontier_Lounge3_EventScript_Gambler::
	lock
	faceplayer
	goto_if_set FLAG_MET_BATTLE_FRONTIER_GAMBLER, BattleFrontier_Lounge3_EventScript_AlreadyMetGambler
	call BattleFrontier_Lounge3_EventScript_CountSilverSymbols
	goto_if_le VAR_0x8004, 2, BattleFrontier_Lounge3_EventScript_NotEnoughSilverSymbols
	setflag FLAG_MET_BATTLE_FRONTIER_GAMBLER
	msgbox BattleFrontier_Lounge3_Text_YouLookToughExplainGambling, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_AskToEnterChallenge
	end

BattleFrontier_Lounge3_EventScript_AskToEnterChallenge::
	special ShowFrontierGamblerLookingMessage
	waitmessage
	waitbuttonpress
	msgbox BattleFrontier_Lounge3_Text_HowAboutEnteringEventForMe, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge3_EventScript_DeclineChallenge
	msgbox BattleFrontier_Lounge3_Text_SpotMeSomeBattlePoints, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge3_EventScript_DeclineChallenge
	message BattleFrontier_Lounge3_Text_HowMuchCanYouSpot
	waitmessage
	special ShowBattlePointsWindow
	goto BattleFrontier_Lounge3_EventScript_ChooseBetAmount
	end

BattleFrontier_Lounge3_EventScript_ChooseBetAmount::
	multichoice 20, 4, MULTI_FRONTIER_GAMBLER_BET, FALSE
	copyvar VAR_FRONTIER_GAMBLER_AMOUNT_BET, VAR_RESULT
	switch VAR_RESULT
	case FRONTIER_GAMBLER_BET_5, BattleFrontier_Lounge3_EventScript_Bet5
	case FRONTIER_GAMBLER_BET_10, BattleFrontier_Lounge3_EventScript_Bet10
	case FRONTIER_GAMBLER_BET_15, BattleFrontier_Lounge3_EventScript_Bet15
	case FRONTIER_GAMBLER_BET_CANCEL, BattleFrontier_Lounge3_EventScript_CancelBet
	case MULTI_B_PRESSED, BattleFrontier_Lounge3_EventScript_CancelBet
	end

BattleFrontier_Lounge3_EventScript_Bet5::
	setvar VAR_0x8008, BET_AMOUNT_5
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_Bet10::
	setvar VAR_0x8008, BET_AMOUNT_10
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_Bet15::
	setvar VAR_0x8008, BET_AMOUNT_15
	goto BattleFrontier_Lounge3_EventScript_TryPlaceBet
	end

BattleFrontier_Lounge3_EventScript_TryPlaceBet::
	specialvar VAR_TEMP_1, GetFrontierBattlePoints
	goto_if_ge VAR_TEMP_1, VAR_0x8008, BattleFrontier_Lounge3_EventScript_PlaceBet
	msgbox BattleFrontier_Lounge3_Text_YouDontHaveEnoughPoints, MSGBOX_DEFAULT
	message BattleFrontier_Lounge3_Text_HowMuchCanYouSpot
	waitmessage
	goto BattleFrontier_Lounge3_EventScript_ChooseBetAmount
	end

BattleFrontier_Lounge3_EventScript_PlaceBet::
	copyvar VAR_0x8004, VAR_0x8008
	special TakeFrontierBattlePoints
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET
	special UpdateBattlePointsWindow
	playse SE_SHOP
	msgbox BattleFrontier_Lounge3_Text_ThanksOffYouGo, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FinishBet
	end

BattleFrontier_Lounge3_EventScript_FinishBet::
	special ShowFrontierGamblerGoMessage
	waitmessage
	waitbuttonpress
	special CloseBattlePointsWindow
	release
	end

BattleFrontier_Lounge3_EventScript_CountSilverSymbols::
	setvar VAR_0x8004, 0
	call_if_set FLAG_SYS_TOWER_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_DOME_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PALACE_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_ARENA_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_FACTORY_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PIKE_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	call_if_set FLAG_SYS_PYRAMID_SILVER, BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount
	return

BattleFrontier_Lounge3_EventScript_AddSilverSymbolCount::
	addvar VAR_0x8004, 1
	return

BattleFrontier_Lounge3_EventScript_NotEnoughSilverSymbols::
	msgbox BattleFrontier_Lounge3_Text_CantYouSeeWereBusyHere, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_AlreadyMetGambler::
	msgbox BattleFrontier_Lounge3_Text_Oh, MSGBOX_DEFAULT
	goto_if_ge VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET, BattleFrontier_Lounge3_EventScript_CheckBetResults
	goto BattleFrontier_Lounge3_EventScript_AskToEnterChallenge
	end

BattleFrontier_Lounge3_EventScript_CheckBetResults::
	goto_if_eq VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_PLACED_BET, BattleFrontier_Lounge3_EventScript_ChallengeNotAttempted
	goto_if_eq VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WON, BattleFrontier_Lounge3_EventScript_WonChallenge
	goto BattleFrontier_Lounge3_EventScript_LostChallenge
	end

BattleFrontier_Lounge3_EventScript_WonChallenge::
	msgbox BattleFrontier_Lounge3_Text_HelloChampHeresYourPoints, MSGBOX_DEFAULT
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_5, BattleFrontier_Lounge3_EventScript_RewardBet5
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_10, BattleFrontier_Lounge3_EventScript_RewardBet10
	call_if_eq VAR_FRONTIER_GAMBLER_AMOUNT_BET, FRONTIER_GAMBLER_BET_15, BattleFrontier_Lounge3_EventScript_RewardBet15
	msgbox BattleFrontier_Lounge3_Text_ObtainedBattlePoints, MSGBOX_GETPOINTS
	special GiveFrontierBattlePoints
	msgbox BattleFrontier_Lounge3_Text_ThinkOfMeForAnotherChallenge, MSGBOX_DEFAULT
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WAITING
	release
	end

BattleFrontier_Lounge3_EventScript_LostChallenge::
	msgbox BattleFrontier_Lounge3_Text_NiceTryCantReturnPoints, MSGBOX_DEFAULT
	setvar VAR_FRONTIER_GAMBLER_STATE, FRONTIER_GAMBLER_WAITING
	release
	end

BattleFrontier_Lounge3_EventScript_RewardBet5::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_5 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_5 * 2)
	return

BattleFrontier_Lounge3_EventScript_RewardBet10::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_10 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_10 * 2)
	return

BattleFrontier_Lounge3_EventScript_RewardBet15::
	buffernumberstring STR_VAR_1, (BET_AMOUNT_15 * 2)
	setvar VAR_0x8004, (BET_AMOUNT_15 * 2)
	return

BattleFrontier_Lounge3_EventScript_ChallengeNotAttempted::
	special ShowFrontierGamblerGoMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge3_EventScript_DeclineChallenge::
	msgbox BattleFrontier_Lounge3_Text_NotInterested, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge3_EventScript_CancelBet::
	special CloseBattlePointsWindow
	goto BattleFrontier_Lounge3_EventScript_DeclineChallenge
	end

BattleFrontier_Lounge3_EventScript_Man::
	setvar VAR_0x8004, REGULAR_LOUNGE3_MAN
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge3_EventScript_Woman::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_BackedWrongTrainer, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_PokefanF::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_KnowWinnerWhenISeeOne, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_FatMan::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge3_Text_TrainerGoodButRattled, MSGBOX_DEFAULT
	goto BattleFrontier_Lounge3_EventScript_FaceOriginalDirection
	end

BattleFrontier_Lounge3_EventScript_FaceOriginalDirection::
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

BattleFrontier_Lounge3_Text_CantYouSeeWereBusyHere::
	.string "{JPN}今は みんなの 勝負を\n応援してる ところなんだ。\pぎんの シンボルを 3つ 集めたら\nあんたにも 話しを 持ちかけたいねえ。$"

BattleFrontier_Lounge3_Text_YouLookToughExplainGambling::
	.string "…… ん?\nあんた 強そうだねぇ……\pへっへ…… ……\nちょっと ィイはなしが\lあるから 聞いてけよ!\pじつは おれたち グループはな\nバトルフロンティアの なかで\lちょっとした ゲームを してんのよ……\pルールは かんたん だぜー\nまずは まいかい どこかの 施設を\lひとつ 選ぶ!\p次に そこに 挑戦 してる\nトレーナーの だれか 一人を 選んで\l自分の バトルポイントを かけるのよ!\pんで 一番 いい 成績を\nだした トレーナーに かけてた やつが\lバトルポイントを 一人じめ すんの!\pなあっ? かんたん だろ?\nんで……$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerSingle::
	.string "{JPN}ちょうど 今は よぉ\nバトルタワーの シングル バトルルームに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerDouble::
	.string "{JPN}ちょうど 今は よぉ\nバトルタワーの ダブル バトルルームに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleTowerMulti::
	.string "{JPN}ちょうど 今は よぉ\nバトルタワーの マルチ バトルルームに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleDomeSingle::
	.string "{JPN}ちょうど 今は よぉ\nバトルドームで やってる\lシングル バトルトーナメントに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleDomeDouble::
	.string "{JPN}ちょうど 今は よぉ\nバトルドームで やってる\lダブル バトルトーナメントに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleFactorySingle::
	.string "{JPN}ちょうど 今は よぉ\nバトルファクトリーで やってる\lバトルトレード シングルに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleFactoryDouble::
	.string "{JPN}ちょうど 今は よぉ\nバトルファクトリーで やってる\lバトルトレード ダブルに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattlePalaceSingle::
	.string "{JPN}ちょうど 今は よぉ\nバトルパレスの\lシングル バトルホールに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattlePalaceDouble::
	.string "{JPN}ちょうど 今は よぉ\nバトルパレスの\lダブル バトルホールに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattleArena::
	.string "{JPN}ちょうど 今は よぉ\nバトルアリーナで やってる\l勝ちぬきチームバトルに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattlePike::
	.string "{JPN}ちょうど 今は よぉ\nバトルチューブで やってる\lバトルゲームに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_ChallengeBattlePyramid::
	.string "{JPN}ちょうど 今は よぉ\nバトルピラミッドで やってる\lバトルアドベンチャーに\l挑戦する トレーナーを\l探してん だけどよ……\pピンとくる トレーナーが いなくてなぁ……\nなぁ……$"

BattleFrontier_Lounge3_Text_HowAboutEnteringEventForMe::
	.string "あんたにも もうけさせて やるからよぅー\nサクっと 挑戦 してきて くんねぇ?$"

BattleFrontier_Lounge3_Text_SpotMeSomeBattlePoints::
	.string "じゃあよ…… あんたの\nバトルポイント ちょーっとで いいから\lおれに 預けて くんねぇ?\pちゃーんと おれいは するからよ!$"

BattleFrontier_Lounge3_Text_HowMuchCanYouSpot::
	.string "ィイね! ィイね!\n"
	.string "で? で? で? いくら 貸して くれます?$"

BattleFrontier_Lounge3_Text_YouDontHaveEnoughPoints::
	.string "いやいやいやー!!\p"
	.string "バトルポイントが 足らない じゃない!\n"
	.string "つまんない 冗談 やめましょうや!$"

BattleFrontier_Lounge3_Text_ThanksOffYouGo::
	.string "へっへっへっ…… あんがとよ!\nそんじゃー$"

BattleFrontier_Lounge3_Text_GetToBattleTowerSingle::
	.string "{JPN}サクサクッと\nバトルタワーの シングル バトルルームに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerDouble::
	.string "{JPN}サクサクッと\nバトルタワーの ダブル バトルルームに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleTowerMulti::
	.string "{JPN}サクサクッと\nバトルタワーの マルチ バトルルームに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleDomeSingle::
	.string "{JPN}サクサクッと バトルドームの\nシングル バトルトーナメントに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleDomeDouble::
	.string "{JPN}サクサクッと バトルドームの\nダブル バトルトーナメントに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleFactorySingle::
	.string "{JPN}サクサクッと バトルファクトリーの\nバトルトレード シングルに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleFactoryDouble::
	.string "{JPN}サクサクッと バトルファクトリーの\nバトルトレード ダブルに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattlePalaceSingle::
	.string "{JPN}サクサクッと バトルパレスの\nシングル バトルホールに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattlePalaceDouble::
	.string "{JPN}サクサクッと バトルパレスの\nダブル バトルホールに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattleArena::
	.string "{JPN}サクサクッと バトルアリーナの\n勝ちぬきチームバトルに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattlePike::
	.string "{JPN}サクサクッと バトルチューブの\nバトルゲームに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_GetToBattlePyramid::
	.string "{JPN}サクサクッと バトルピラミッドの\nバトルアドベンチャーに\lいってきて ちょーだいっ!\pがんばれ よぉー!\n間違えんな よぉー!\lおれと あんたの 未来の ためだかんな!$"

BattleFrontier_Lounge3_Text_NiceTryCantReturnPoints::
	.string "あぁ あんた…… ごくろう さん……\pわりーけど バトルポイントは\nかえせねぇよ……\p…… まぁ おたがい 次の きかいに\nがんばり ましょうや……$"

BattleFrontier_Lounge3_Text_HelloChampHeresYourPoints::
	.string "ぃよおっ!! 先生!!\pさすが! さすが ですねぇー!!\nわたくし おおもうけで ございますよぉ!\pバトルポイント おかえし しますよっ!\nしっかり のしつけて 差し上げますよぉ!$"

BattleFrontier_Lounge3_Text_ObtainedBattlePoints::
	.string "{PLAYER}は バトルポイントを\n{STR_VAR_1}BP てにいれた!$"

BattleFrontier_Lounge3_Text_ThinkOfMeForAnotherChallenge::
	.string "また なんか あったら\nよろしく 頼みますよっ!!$"

BattleFrontier_Lounge3_Text_NotInterested::
	.string "そりゃ ないぜー!\n"
	.string "世知辛ぇ お子さま だなぁ!!$"

BattleFrontier_Lounge3_Text_Oh::
	.string "おっ……$"

BattleFrontier_Lounge3_Text_BackedWrongTrainer::
	.string "{JPN}また はずれちゃった！\nでも あのこが がんばったのは 見てたよ。\p次は あたしも\n自分で 挑戦しようかな！$"

BattleFrontier_Lounge3_Text_TrainerGoodButRattled::
	.string "{JPN}あの トレーナーは 強いのに\nドームだと きんちょうしちゃうんだ。\pこっちまで てに あせを かくよ。$"

BattleFrontier_Lounge3_Text_KnowWinnerWhenISeeOne::
	.string "{JPN}やった！ 応援した ひとが 勝った！\nBPまで 増えちゃったわ！$"

