FiveIsland_ResortGorgeous_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, FiveIsland_ResortGorgeous_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FiveIsland_ResortGorgeous_OnFrame
	.byte 0

FiveIsland_ResortGorgeous_OnWarp::
	map_script_2 VAR_MAP_SCENE_FIVE_ISLAND_RESORT_GORGEOUS, 1, FiveIsland_ResortGorgeous_EventScript_TurnPlayerNorth
	.2byte 0

FiveIsland_ResortGorgeous_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

FiveIsland_ResortGorgeous_OnFrame::
	map_script_2 VAR_MAP_SCENE_FIVE_ISLAND_RESORT_GORGEOUS, 1, FiveIsland_ResortGorgeous_EventScript_SelphyReturnHomeScene
	.2byte 0

FiveIsland_ResortGorgeous_EventScript_SelphyReturnHomeScene::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox FiveIsland_ResortGorgeous_Text_SelphyThanksYouMayGoNow
	closemessage
	applymovement LOCALID_RESORT_GORGEOUS_SELPHY, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor 39, 8
	waitdooranim
	applymovement LOCALID_RESORT_GORGEOUS_SELPHY, FiveIsland_ResortGorgeous_Movement_SelphyEnterHome
	waitmovement 0
	closedoor 39, 8
	waitdooranim
	removeobject LOCALID_RESORT_GORGEOUS_SELPHY
	clearflag FLAG_HIDE_RESORT_GORGEOUS_INSIDE_SELPHY
	setvar VAR_MAP_SCENE_FIVE_ISLAND_RESORT_GORGEOUS, 2
	releaseall
	end

FiveIsland_ResortGorgeous_Movement_SelphyEnterHome::
	walk_up
	set_invisible
	step_end

FiveIsland_ResortGorgeous_EventScript_SelphysHouseSign::
	msgbox FiveIsland_ResortGorgeous_Text_SelphysHouse, MSGBOX_SIGN
	end

FiveIsland_ResortGorgeous_Text_DaisyIntro::
	.string "{JPN}このてで しょうりを えがくわ!$"

FiveIsland_ResortGorgeous_Text_DaisyDefeat::
	.string "{JPN}くやしくて\n"
	.string "かおが まっかだわ!$"

FiveIsland_ResortGorgeous_Text_DaisyPostBattle::
	.string "{JPN}やだ\n"
	.string "えのぐが とんじゃった$"

FiveIsland_ResortGorgeous_Text_CelinaIntro::
	.string "{JPN}ちょっと!\n"
	.string "えを かいてるのよ\l"
	.string "しかいに はいって こないで!$"

FiveIsland_ResortGorgeous_Text_CelinaDefeat::
	.string "{JPN}まけたわ…\n"
	.string "めのまえが まっしろに…$"

FiveIsland_ResortGorgeous_Text_CelinaPostBattle::
	.string "{JPN}ユキノさーん\n"
	.string "すこし くびを かたむけて…\l"
	.string "あっ ステキですー!$"

FiveIsland_ResortGorgeous_Text_RaynaIntro::
	.string "{JPN}うーん\n"
	.string "アングルが いまいち$"

FiveIsland_ResortGorgeous_Text_RaynaDefeat::
	.string "{JPN}ああ\n"
	.string "いまの きもちは ブルー…$"

FiveIsland_ResortGorgeous_Text_RaynaPostBattle::
	.string "{JPN}うみ とか そらって\n"
	.string "すぐに かたちが かわるから\l"
	.string "かきにくいわ$"

FiveIsland_ResortGorgeous_Text_JackiIntro::
	.string "{JPN}おともだちの べっそうに\n"
	.string "きて いますの$"

FiveIsland_ResortGorgeous_Text_JackiDefeat::
	.string "{JPN}おつよいん ですのね$"

FiveIsland_ResortGorgeous_Text_JackiPostBattle::
	.string "{JPN}しおかぜに ふかれ ながら\n"
	.string "のんびり ポケモン というのも\l"
	.string "すてき ですわね$"

FiveIsland_ResortGorgeous_Text_GillianIntro::
	.string "{JPN}おにわに ポケモン せんようの\n"
	.string "プールを つくろうかしら…?$"

FiveIsland_ResortGorgeous_Text_GillianDefeat::
	.string "{JPN}まあ すごい!$"

FiveIsland_ResortGorgeous_Text_GillianPostBattle::
	.string "{JPN}プールも いいけど\n"
	.string "サンルームも いいわねえ…$"

FiveIsland_ResortGorgeous_Text_DestinIntro::
	.string "{JPN}ぼく かけっこ\n"
	.string "こんなに はやいんだ!$"

FiveIsland_ResortGorgeous_Text_DestinDefeat::
	.string "{JPN}いきおい つけすぎた かな?$"

FiveIsland_ResortGorgeous_Text_DestinPostBattle::
	.string "{JPN}かぜに のって\n"
	.string "はしってたら とべそう!$"

FiveIsland_ResortGorgeous_Text_TobyIntro::
	.string "{JPN}リゾートちでの マリンライフを\n"
	.string "エンジョイ してるのさ$"

FiveIsland_ResortGorgeous_Text_TobyDefeat::
	.string "{JPN}ショック!\n"
	.string "そりゃ ないぜ$"

FiveIsland_ResortGorgeous_Text_TobyPostBattle::
	.string "{JPN}そんなに むきに なるのは\n"
	.string "ナンセンスだな$"

FiveIsland_ResortGorgeous_Text_SelphyThanksYouMayGoNow::
	.string "{JPN}アキホ『ありがとう\p"
	.string "もう おかえりに なっても\n"
	.string "よろしくてよ$"

FiveIsland_ResortGorgeous_Text_SelphysHouse::
	.string "{JPN}ここは アキホの おうち$"

