RockTunnel_B1F_Frlg_MapScripts::
	.byte 0

RockTunnel_B1F_EventScript_Sofia::
	trainerbattle_single TRAINER_PICNICKER_SOFIA, RockTunnel_B1F_Text_SofiaIntro, RockTunnel_B1F_Text_SofiaDefeat
	msgbox RockTunnel_B1F_Text_SofiaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Martha::
	trainerbattle_single TRAINER_PICNICKER_MARTHA, RockTunnel_B1F_Text_MarthaIntro, RockTunnel_B1F_Text_MarthaDefeat
	msgbox RockTunnel_B1F_Text_MarthaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Dudley::
	trainerbattle_single TRAINER_HIKER_DUDLEY, RockTunnel_B1F_Text_DudleyIntro, RockTunnel_B1F_Text_DudleyDefeat
	msgbox RockTunnel_B1F_Text_DudleyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Allen::
	trainerbattle_single TRAINER_HIKER_ALLEN, RockTunnel_B1F_Text_AllenIntro, RockTunnel_B1F_Text_AllenDefeat
	msgbox RockTunnel_B1F_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Eric::
	trainerbattle_single TRAINER_HIKER_ERIC, RockTunnel_B1F_Text_EricIntro, RockTunnel_B1F_Text_EricDefeat
	msgbox RockTunnel_B1F_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Cooper::
	trainerbattle_single TRAINER_POKEMANIAC_COOPER, RockTunnel_B1F_Text_CooperIntro, RockTunnel_B1F_Text_CooperDefeat
	msgbox RockTunnel_B1F_Text_CooperPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Steve::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE, RockTunnel_B1F_Text_SteveIntro, RockTunnel_B1F_Text_SteveDefeat
	msgbox RockTunnel_B1F_Text_StevePostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Winston::
	trainerbattle_single TRAINER_POKEMANIAC_WINSTON, RockTunnel_B1F_Text_WinstonIntro, RockTunnel_B1F_Text_WinstonDefeat
	msgbox RockTunnel_B1F_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_Text_SofiaIntro::
	.string "{JPN}やまで まよわない ためには\n"
	.string "めじるしの こえだを\l"
	.string "おって あるくと いいのよ!$"

RockTunnel_B1F_Text_SofiaDefeat::
	.string "{JPN}やーん…!\n"
	.string "がんばったのに$"

RockTunnel_B1F_Text_SofiaPostBattle::
	.string "{JPN}まけちゃったし\n"
	.string "きょうは おうちに かえろう$"

RockTunnel_B1F_Text_DudleyIntro::
	.string "{JPN}はははーッ!\n"
	.string "おれの パワーに かてるか!$"

RockTunnel_B1F_Text_DudleyDefeat::
	.string "{JPN}おっとっと!\n"
	.string "パワー まけ したか!$"

RockTunnel_B1F_Text_DudleyPostBattle::
	.string "{JPN}パワーに こだわるのは\n"
	.string "かんがえながら たたかうのが\l"
	.string "にがて だからだ!$"

RockTunnel_B1F_Text_CooperIntro::
	.string "{JPN}ポケモン ずかん つくってる?\n"
	.string "くそう! ぼくも やりたい!$"

RockTunnel_B1F_Text_CooperDefeat::
	.string "{JPN}ああ もう! くやしい$"

RockTunnel_B1F_Text_CooperPostBattle::
	.string "{JPN}… だからさ\n"
	.string "ポケモン ずかん かんせい したら\l"
	.string "ぼくに くれない?$"

RockTunnel_B1F_Text_SteveIntro::
	.string "{JPN}あの…\n"
	.string "ポケモン コスプレ しってます?$"

RockTunnel_B1F_Text_SteveDefeat::
	.string "{JPN}しょうが ない ですね$"

RockTunnel_B1F_Text_StevePostBattle::
	.string "{JPN}コスプレって いうのは\n"
	.string "ピッピの きぐるみ とか つくって\l"
	.string "ポケモンに なりきるんです$"

RockTunnel_B1F_Text_AllenIntro::
	.string "{JPN}おれの\n"
	.string "ポケモン テクニックで\l"
	.string "ひーひー いわせるぜ!$"

RockTunnel_B1F_Text_AllenDefeat::
	.string "{JPN}まいった!\n"
	.string "おまえの ほうが テクニシャンだ$"

RockTunnel_B1F_Text_AllenPostBattle::
	.string "{JPN}そうだな…\n"
	.string "やまの ポケモン には\l"
	.string "いわ タイプが おおいぞ$"

RockTunnel_B1F_Text_MarthaIntro::
	.string "{JPN}たたかっても いいけど…\n"
	.string "この あたりには\l"
	.string "あまり きた こと ないの$"

RockTunnel_B1F_Text_MarthaDefeat::
	.string "{JPN}まけちゃった$"

RockTunnel_B1F_Text_MarthaPostBattle::
	.string "{JPN}ちっちゃい ポケモンが すき\n"
	.string "おっきいのは こわい から いや$"

RockTunnel_B1F_Text_EricIntro::
	.string "{JPN}よしッ\n"
	.string "どーんと ぶつかって こい!$"

RockTunnel_B1F_Text_EricDefeat::
	.string "{JPN}どーんと!\n"
	.string "こっちが やられた!$"

RockTunnel_B1F_Text_EricPostBattle::
	.string "{JPN}おれも おまえに まけない\n"
	.string "ポケモンを そだてて おくぜ!$"

RockTunnel_B1F_Text_WinstonIntro::
	.string "{JPN}いえに いる ときは\n"
	.string "ポケモンの イラスト とか かいてる$"

RockTunnel_B1F_Text_WinstonDefeat::
	.string "{JPN}ああ…\n"
	.string "ぼく くたびれた$"

RockTunnel_B1F_Text_WinstonPostBattle::
	.string "{JPN}ぼくは たたかうの むいて ないな\n"
	.string "かえって イラスト でも かいてよう$"

