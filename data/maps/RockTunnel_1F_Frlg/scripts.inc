RockTunnel_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RockTunnel_1F_OnTransition
	.byte 0

RockTunnel_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCK_TUNNEL_1F
	end

RockTunnel_1F_EventScript_RouteSign::
	msgbox RockTunnel_1F_Text_RouteSign, MSGBOX_SIGN
	end

RockTunnel_1F_EventScript_Lenny::
	trainerbattle_single TRAINER_HIKER_LENNY, RockTunnel_1F_Text_LennyIntro, RockTunnel_1F_Text_LennyDefeat
	msgbox RockTunnel_1F_Text_LennyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Oliver::
	trainerbattle_single TRAINER_HIKER_OLIVER, RockTunnel_1F_Text_OliverIntro, RockTunnel_1F_Text_OliverDefeat
	msgbox RockTunnel_1F_Text_OliverPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Lucas::
	trainerbattle_single TRAINER_HIKER_LUCAS, RockTunnel_1F_Text_LucasIntro, RockTunnel_1F_Text_LucasDefeat
	msgbox RockTunnel_1F_Text_LucasPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Ashton::
	trainerbattle_single TRAINER_POKEMANIAC_ASHTON, RockTunnel_1F_Text_AshtonIntro, RockTunnel_1F_Text_AshtonDefeat
	msgbox RockTunnel_1F_Text_AshtonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Leah::
	trainerbattle_single TRAINER_PICNICKER_LEAH, RockTunnel_1F_Text_LeahIntro, RockTunnel_1F_Text_LeahDefeat
	msgbox RockTunnel_1F_Text_LeahPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Ariana::
	trainerbattle_single TRAINER_PICNICKER_ARIANA, RockTunnel_1F_Text_ArianaIntro, RockTunnel_1F_Text_ArianaDefeat
	msgbox RockTunnel_1F_Text_ArianaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Dana::
	trainerbattle_single TRAINER_PICNICKER_DANA, RockTunnel_1F_Text_DanaIntro, RockTunnel_1F_Text_DanaDefeat
	msgbox RockTunnel_1F_Text_DanaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_Text_LennyIntro::
	.string "{JPN}この ながーい\n"
	.string "トンネルを ぬける つもりか!$"

RockTunnel_1F_Text_LennyDefeat::
	.string "{JPN}おいらの まけだ$"

RockTunnel_1F_Text_LennyPostBattle::
	.string "{JPN}たまに でる イワークは\n"
	.string "レベルが たかいと しめつけてくる\l"
	.string "きを つけなよ$"

RockTunnel_1F_Text_OliverIntro::
	.string "{JPN}えーと\n"
	.string "みちに まよったかな?$"

RockTunnel_1F_Text_OliverDefeat::
	.string "{JPN}たたかってる\n"
	.string "ばあいじゃ なかった\l"
	.string "でぐちは どっち だっけ?$"

RockTunnel_1F_Text_OliverPostBattle::
	.string "{JPN}12ばん どうろに\n"
	.string "ポケモンが ねてたろ?\l"
	.string "おれも こまって まわりみちだ$"

RockTunnel_1F_Text_LucasIntro::
	.string "{JPN}おれは\n"
	.string "やまに すんで ながい\l"
	.string "よそものに でかい つらは させん$"

RockTunnel_1F_Text_LucasDefeat::
	.string "{JPN}やるなあ$"

RockTunnel_1F_Text_LucasPostBattle::
	.string "{JPN}おまえ みこみが ある\n"
	.string "やまおとこに ならんか?$"

RockTunnel_1F_Text_AshtonIntro::
	.string "{JPN}ポケモン ファイトー!\n"
	.string "レディー ゴー!$"

RockTunnel_1F_Text_AshtonDefeat::
	.string "{JPN}やぶれたのか$"

RockTunnel_1F_Text_AshtonPostBattle::
	.string "{JPN}ズバットでも\n"
	.string "とって かえろうかな…$"

RockTunnel_1F_Text_LeahIntro::
	.string "{JPN}きゃ びっくり!\n"
	.string "うすぐらい からって\l"
	.string "そんなに ちかづかないで!$"

RockTunnel_1F_Text_LeahDefeat::
	.string "{JPN}… くらくて\n"
	.string "よく みえないんだもん$"

RockTunnel_1F_Text_LeahPostBattle::
	.string "{JPN}トンネルにも ポケモン いるのね\n"
	.string "さっき ワンリキーが いたわ$"

RockTunnel_1F_Text_DanaIntro::
	.string "{JPN}ポケモン さがしに きて\n"
	.string "こんな とこまで きちゃった!$"

RockTunnel_1F_Text_DanaDefeat::
	.string "{JPN}もう ポケモン ないわ$"

RockTunnel_1F_Text_DanaPostBattle::
	.string "{JPN}かわいい かお して\n"
	.string "けっこう つよい じゃない$"

RockTunnel_1F_Text_ArianaIntro::
	.string "{JPN}あ それは ポケモン!\n"
	.string "さっそく はじめましょ!$"

RockTunnel_1F_Text_ArianaDefeat::
	.string "{JPN}でなおして くるわ$"

RockTunnel_1F_Text_ArianaPostBattle::
	.string "{JPN}エキサイト して\n"
	.string "あせ かいちゃったわ$"

RockTunnel_1F_Text_RouteSign::
	.string "{JPN}ここは イワヤマ トンネル\n"
	.string "ハナダ シティ … シオン タウン$"

