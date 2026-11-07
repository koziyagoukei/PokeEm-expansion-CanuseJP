CeladonCity_Restaurant_Frlg_MapScripts::
	.byte 0

CeladonCity_Restaurant_EventScript_Chef::
	msgbox CeladonCity_Restaurant_Text_TakingBreakRightNow, MSGBOX_NPC
	end

CeladonCity_Restaurant_EventScript_Woman::
	msgbox CeladonCity_Restaurant_Text_OftenGoToDrugstore, MSGBOX_NPC
	end

CeladonCity_Restaurant_EventScript_CoinCaseMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_COIN_CASE, CeladonCity_Restaurant_EventScript_AlreadyGotCoinCase
	msgbox CeladonCity_Restaurant_Text_TakeThisImBusted
	checkitemspace ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_Restaurant_EventScript_NoRoomForCoinCase
	giveitem_msg CeladonCity_Restaurant_Text_ReceivedCoinCaseFromMan, ITEM_COIN_CASE
	setflag FLAG_GOT_COIN_CASE
	release
	end

CeladonCity_Restaurant_EventScript_NoRoomForCoinCase::
	msgbox CeladonCity_Restaurant_Text_MakeRoomForThis
	release
	end

CeladonCity_Restaurant_EventScript_AlreadyGotCoinCase::
	msgbox CeladonCity_Restaurant_Text_ThoughtIdWinItBack
	release
	end

CeladonCity_Restaurant_EventScript_WorkerM::
	msgbox CeladonCity_Restaurant_Text_PsstBasementUnderGameCorner, MSGBOX_NPC
	end

CeladonCity_Restaurant_EventScript_FatMan::
	msgbox CeladonCity_Restaurant_Text_ManLostItAllAtSlots, MSGBOX_NPC
	end

CeladonCity_Restaurant_Text_TakingBreakRightNow::
	.string "{JPN}いらっしゃい!\p"
	.string "じょうれん ばかりで わるいね!$"

CeladonCity_Restaurant_Text_OftenGoToDrugstore::
	.string "{JPN}うちで かってる ポケモン…\n"
	.string "たいりょく ないから いつも\l"
	.string "デパートで おくすり かって くるの$"

CeladonCity_Restaurant_Text_PsstBasementUnderGameCorner::
	.string "{JPN}ひそひそ… むしゃ むしゃ…\n"
	.string "… ゲーム コーナーの したに\l"
	.string "ちかしつが ある らしいね…$"

CeladonCity_Restaurant_Text_ManLostItAllAtSlots::
	.string "{JPN}もぐもぐ…!\p"
	.string "あそこの テーブルの おっさん\n"
	.string "スロットで おかね ぜんぶ\l"
	.string "つかっちゃった らしいよ!$"

CeladonCity_Restaurant_Text_TakeThisImBusted::
	.string "{JPN}うおう… わらってくれ!\p"
	.string "ゲーム コーナーで すって\n"
	.string "いちもん なしに なっちまった!\p"
	.string "もう スロットは やらん!\n"
	.string "じみちに はたらくぞ!\p"
	.string "… というわけで\n"
	.string "これは おまえに やる!$"

CeladonCity_Restaurant_Text_ReceivedCoinCaseFromMan::
	.string "{JPN}{PLAYER}は おっさん から\n"
	.string "コインケースを もらった!$"

CeladonCity_Restaurant_Text_MakeRoomForThis::
	.string "{JPN}にもつが いっぱい だぜ$"

CeladonCity_Restaurant_Text_ThoughtIdWinItBack::
	.string "{JPN}あーあ!\n"
	.string "いつか かならず かてる!\l"
	.string "と おもったのに この ざまだ…!$"

