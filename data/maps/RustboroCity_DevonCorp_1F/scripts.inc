RustboroCity_DevonCorp_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_1F_OnTransition
	.byte 0

RustboroCity_DevonCorp_1F_OnTransition:
	call_if_unset FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_BlockStairs
	end

RustboroCity_DevonCorp_1F_EventScript_BlockStairs::
	setobjectxyperm LOCALID_DEVON_CORP_STAIR_GUARD, 14, 2
	setobjectmovementtype LOCALID_DEVON_CORP_STAIR_GUARD, MOVEMENT_TYPE_FACE_DOWN
	return

RustboroCity_DevonCorp_1F_EventScript_Employee::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright
	msgbox RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RobberWasntBright::
	msgbox RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GoodsRecovered::
	msgbox RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StairGuard::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_GotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_AlwaysWelcome::
	msgbox RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_GotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_Greeter::
	lock
	faceplayer
	goto_if_set FLAG_RETURNED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp
	goto_if_set FLAG_RECOVERED_DEVON_GOODS, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	goto_if_set FLAG_DEVON_GOODS_STOLEN, RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_WelcomeToDevonCorp::
	msgbox RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_StaffGotRobbed::
	msgbox RustboroCity_DevonCorp_1F_Text_StaffGotRobbed, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_1F_EventScript_RocksMetalDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_EventScript_ProductsDisplay::
	msgbox RustboroCity_DevonCorp_1F_Text_ProductDisplay, MSGBOX_SIGN
	end

RustboroCity_DevonCorp_1F_Text_WelcomeToDevonCorp:
	.string "{JPN}いらっしゃいませ!\nデボン コーポレーション です!\pみなさまの せいかつに やくだつ\nどうぐや くすりを おつくりしてます!"
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_1F_Text_StaffGotRobbed:
	.string "{JPN}ドジな けんきゅういんが\nにもつを とられちゃって……"
	.string "$"

RustboroCity_DevonCorp_1F_Text_ThoseShoesAreOurProduct:
	.string "{JPN}おっ! ランニングシューズ!\pじぶんたちの かいしゃの しょうひんを\nじっさいに つかっている ひとを みると\lすごく うれしくなるよね!"
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_1F_Text_RobberWasntVeryBright:
	.string "{JPN}たしかに あの にもつは\nだいじな もの だけど\lほかの ひとが ぬすんでいっても\lつかえないと おもうんだよね\pぼくの すいりでは あの どろぼう……\nたんなる マヌケ なんじゃないかな?"
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

RustboroCity_DevonCorp_1F_Text_SoundsLikeStolenGoodsRecovered:
	.string "{JPN}どうやら デボンのにもつは\nもどってきた らしいね"
	.string "$"

RustboroCity_DevonCorp_1F_Text_OnlyAuthorizedPeopleEnter:
	.string "{JPN}おっと かんけいしゃ いがいの かたは\nここから さきには はいれませんよ!"
	.string "$"

RustboroCity_DevonCorp_1F_Text_HowCouldWeGetRobbed:
	.string "{JPN}どろぼうに にもつを うばわれるなんて\nなんて ドジなんだろう……"
	.string "$"

RustboroCity_DevonCorp_1F_Text_YoureAlwaysWelcomeHere:
	.string "{JPN}やあ!\nきみなら いつでも だいかんげい だよ!"
	.string "$"

RustboroCity_DevonCorp_1F_Text_RocksMetalDisplay:
	.string "{JPN}いしや きんぞくの サンプルが\nガラスケースの なかに ならんでいる!\pパネルに せつめいが かいてあるから\nよんでみよう……\p「デボン コーポレーションは もともと\n やまから いしを きりだしたり\p さてつから てつざいを つくるために\n せつりつ された かいしゃ です」\p「やがて デボン コーポレーションは\n ざいりょう だけに とどまらず\p さまざまな こうぎょう せいひんも\n つくるように なったのです」"
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

RustboroCity_DevonCorp_1F_Text_ProductDisplay:
	.string "{JPN}いろんな どうぐの しさくひんが\nガラスケースの なかに ならんでいる!\pパネルに せつめいが かいてあるから\nよんでみよう……\p「げんざい デボン コーポレーションでは\n こうぎょう せいひんの ほかにも\l にちようひんや くすりなど\p ひとびとの くらしに やくだつ ものを\n かいはつ しています」\p「また げんざいでは モンスターボールや\n ポケナビなどの トレーナーようひんも\l てがけるように なりました」"
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


