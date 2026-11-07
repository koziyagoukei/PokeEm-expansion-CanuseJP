VermilionCity_House3_Frlg_MapScripts::
	.byte 0

VermilionCity_House3_EventScript_Boy::
	msgbox VermilionCity_House3_Text_PidgeyFlyLetterToSaffron, MSGBOX_NPC
	end

VermilionCity_House3_EventScript_Lass::
	msgbox VermilionCity_House3_Text_SendMyPidgeyToUnionRoom, MSGBOX_NPC
	end

VermilionCity_House3_EventScript_Pidgey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox VermilionCity_House3_Text_Pidgey
	waitmoncry
	release
	end

VermilionCity_House3_EventScript_Letter::
	msgbox VermilionCity_House3_Text_DearPippiLetter, MSGBOX_SIGN
	end

VermilionCity_House3_Text_PidgeyFlyLetterToSaffron::
	.string "{JPN}きたの ヤマブキシティまで\n"
	.string "ポッポに てがみを\l"
	.string "とどけて もらうのさ!$"

VermilionCity_House3_Text_Pidgey::
	.string "{JPN}ポッポ『クルックー$"

VermilionCity_House3_Text_DearPippiLetter::
	.string "{JPN}…ピピちゃん\n"
	.string "こんど あそびに いきたい です\p"
	.string "ヤマブキ シティでは ロケットだんの\n"
	.string "じけんが おおい そうですね\p"
	.string "クチバは そうでも ありません…$"

VermilionCity_House3_Text_SendMyPidgeyToUnionRoom::
	.string "{JPN}わたしは いろんな ひとと\n"
	.string "メール こうかんを したいから\p"
	.string "いつも ユニオン ルーム で\n"
	.string "ポッポに メールを もたせて\l"
	.string "こうかん してるのよ!$"

