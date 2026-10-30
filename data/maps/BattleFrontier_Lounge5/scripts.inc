BattleFrontier_Lounge5_MapScripts::
	.byte 0

BattleFrontier_Lounge5_EventScript_NatureGirl::
	lock
	faceplayer
	msgbox BattleFrontier_Lounge5_Text_NatureGirlGreeting, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	chooseboxmon
	waitstate
	lock
	faceplayer
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, BattleFrontier_Lounge5_EventScript_NatureGirlEgg
	special ShowNatureGirlMessage
	waitmessage
	waitbuttonpress
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlEgg::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlEgg, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_NatureGirlNoneShown::
	msgbox BattleFrontier_Lounge5_Text_NatureGirlNoneShown, MSGBOX_DEFAULT
	release
	end

BattleFrontier_Lounge5_EventScript_Gentleman::
	setvar VAR_0x8004, REGULAR_LOUNGE5_GENTLEMAN
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge5_EventScript_BlackBelt::
	setvar VAR_0x8004, REGULAR_LOUNGE5_BLACK_BELT
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge5_EventScript_LittleBoy::
	setvar VAR_0x8004, REGULAR_LOUNGE5_LITTLE_BOY
	goto FrontierStory_EventScript_RegularTalk

BattleFrontier_Lounge5_Text_NatureGirlGreeting::
	.string "{JPN}ねえ ポケモン 見せて！\nあたち このこの 気持ち 聞いてみたい！$"

BattleFrontier_Lounge5_Text_NatureGirlNoneShown::
	.string "{JPN}そっかあ。 また 今度ね！$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighAttackLow::
	.string "{JPN}ふんふん……\pこのこ 戦うの ちゅき! って!\nいっぱい いたいいたい でも\l戦うよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighDefenseLow::
	.string "{JPN}ふんふん……\pこのこ 戦うの ちゅき! って!\nでも あんまし いたいいたい だと\l自分が ちんぱい なるよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighSupportLow::
	.string "{JPN}ふんふん……\pこのこ 戦うの ちゅき! って!\nでも あんまし いたいいたい だと\lずるいの するよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighAttackLow::
	.string "{JPN}ふんふん……\pこのこ 自分 大事! って!\nでも あんまし いたいいたい だと\lちかえし してやる! って!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighDefenseLow::
	.string "{JPN}ふんふん……\pこのこ 自分 大事! って!\nいっぱい いたいいたい でも\l自分 大事! って!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighSupportLow::
	.string "{JPN}ふんふん……\pこのこ 自分 大事! って!\nでも あんまし いたいいたい だと\lずるいの するよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighAttackLow::
	.string "{JPN}ふんふん……\pこのこ ずるいの ちゅき! って!\nでも あんまし いたいいたい だと\lちかえし してやる! って!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighDefenseLow::
	.string "{JPN}ふんふん……\pこのこ ずるいの ちゅき! って!\nでも あんまし いたいいたい だと\l自分が ちんぱい なるよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighSupportLow::
	.string "{JPN}ふんふん……\pこのこ ずるいの ちゅき! って!\nいっぱい いたいいたい でも\lずるいのが いい! って!$"

BattleFrontier_Lounge5_Text_NatureGirlEgg::
	.string "たまごは ねんね してる から\n"
	.string "おはなち できない よぉ!$"

