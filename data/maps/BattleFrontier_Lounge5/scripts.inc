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
	msgbox BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_BlackBelt::
	msgbox BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound, MSGBOX_NPC
	end

BattleFrontier_Lounge5_EventScript_LittleBoy::
	msgbox BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot, MSGBOX_NPC
	end

BattleFrontier_Lounge5_Text_NatureGirlGreeting::
	.string "{JPN}へへ! あたち ポケモンの\nきもち わかるの!\pね! ポケモン みちて! みちて!$"

BattleFrontier_Lounge5_Text_NatureGirlNoneShown::
	.string "{JPN}ぷー! けち!!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighAttackLow::
	.string "{JPN}ふんふん……\pこのこ たたかうの ちゅき! って!\nいっぱい いたいいたい でも\lたたかうよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighDefenseLow::
	.string "{JPN}ふんふん……\pこのこ たたかうの ちゅき! って!\nでも あんまし いたいいたい だと\lじぶんが ちんぱい なるよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlAttackHighSupportLow::
	.string "{JPN}ふんふん……\pこのこ たたかうの ちゅき! って!\nでも あんまし いたいいたい だと\lずるいの するよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighAttackLow::
	.string "{JPN}ふんふん……\pこのこ じぶん だいじ! って!\nでも あんまし いたいいたい だと\lちかえし してやる! って!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighDefenseLow::
	.string "{JPN}ふんふん……\pこのこ じぶん だいじ! って!\nいっぱい いたいいたい でも\lじぶん だいじ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlDefenseHighSupportLow::
	.string "{JPN}ふんふん……\pこのこ じぶん だいじ! って!\nでも あんまし いたいいたい だと\lずるいの するよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighAttackLow::
	.string "{JPN}ふんふん……\pこのこ ずるいの ちゅき! って!\nでも あんまし いたいいたい だと\lちかえし してやる! って!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighDefenseLow::
	.string "{JPN}ふんふん……\pこのこ ずるいの ちゅき! って!\nでも あんまし いたいいたい だと\lじぶんが ちんぱい なるよ! って!$"

BattleFrontier_Lounge5_Text_NatureGirlSupportHighSupportLow::
	.string "{JPN}ふんふん……\pこのこ ずるいの ちゅき! って!\nいっぱい いたいいたい でも\lずるいのが いい! って!$"

BattleFrontier_Lounge5_Text_NatureGirlEgg::
	.string "{JPN}たまごは ねんね してる から\nおはなち できない よぉ!$"

BattleFrontier_Lounge5_Text_LadyClaimsSheUnderstandsPokemon::
	.string "{JPN}ポケモンの きもちが わかるとは\nあの おじょうちゃん なかなか\lシャレた ことを もうしますな!$"

BattleFrontier_Lounge5_Text_GirlSayingSomethingProfound::
	.string "{JPN}じぶん あのこが すごく だいじなことを\nいっている きが するで あります!$"

BattleFrontier_Lounge5_Text_GirlPlaysAtRedHouseALot::
	.string "{JPN}ぼく ちってるぜ!\pあのこ あかい おうちで\nよく あそんでる!$"

