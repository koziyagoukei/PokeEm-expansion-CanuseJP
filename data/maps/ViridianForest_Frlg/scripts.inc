ViridianForest_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianForest_OnTransition
	.byte 0

ViridianForest_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_FOREST
	end

ViridianForest_EventScript_Youngster::
	msgbox ViridianForest_Text_FriendsItchingToBattle, MSGBOX_NPC
	end

ViridianForest_EventScript_Boy::
	msgbox ViridianForest_Text_RanOutOfPokeBalls, MSGBOX_NPC
	end

ViridianForest_EventScript_TrainerTips1::
	msgbox ViridianForest_Text_AvoidGrassyAreasWhenWeak, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips2::
	msgbox ViridianForest_Text_UseAntidoteForPoison, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips3::
	msgbox ViridianForest_Text_ContactOakViaPCToRatePokedex, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips4::
	msgbox ViridianForest_Text_CantCatchOwnedMons, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips5::
	msgbox ViridianForest_Text_WeakenMonsBeforeCapture, MSGBOX_SIGN
	end

ViridianForest_EventScript_ExitSign::
	msgbox ViridianForest_Text_LeavingViridianForest, MSGBOX_SIGN
	end

ViridianForest_EventScript_Rick::
	trainerbattle_single TRAINER_BUG_CATCHER_RICK, ViridianForest_Text_RickIntro, ViridianForest_Text_RickDefeat
	msgbox ViridianForest_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Doug::
	trainerbattle_single TRAINER_BUG_CATCHER_DOUG, ViridianForest_Text_DougIntro, ViridianForest_Text_DougDefeat
	msgbox ViridianForest_Text_DougPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Sammy::
	trainerbattle_single TRAINER_BUG_CATCHER_SAMMY, ViridianForest_Text_SammyIntro, ViridianForest_Text_SammyDefeat
	msgbox ViridianForest_Text_SammyPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Anthony::
	trainerbattle_single TRAINER_BUG_CATCHER_ANTHONY, ViridianForest_Text_AnthonyIntro, ViridianForest_Text_AnthonyDefeat
	msgbox ViridianForest_Text_AnthonyPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Charlie::
	trainerbattle_single TRAINER_BUG_CATCHER_CHARLIE, ViridianForest_Text_CharlieIntro, ViridianForest_Text_CharlieDefeat
	msgbox ViridianForest_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_Text_FriendsItchingToBattle::
	.string "{JPN}ともだちと むし ポケモン\n"
	.string "とりに きてるんだ!\p"
	.string "ポケモン しょうぶ したくて\n"
	.string "みんな ウズウズ してるよ!$"

ViridianForest_Text_RickIntro::
	.string "{JPN}よーしッ!\n"
	.string "きみは ポケモン もってるな?\l"
	.string "しょうぶ しようぜ!$"

ViridianForest_Text_RickDefeat::
	.string "{JPN}まけたあ!\n"
	.string "キャタピー なんか じゃ ダメか$"

ViridianForest_Text_RickPostBattle::
	.string "{JPN}しッ…!\n"
	.string "むしが にげる から またな!$"

ViridianForest_Text_DougIntro::
	.string "{JPN}おーいッ!\n"
	.string "ポケモン トレーナーなら\l"
	.string "しょうぶは ことわれ ないぜ!$"

ViridianForest_Text_DougDefeat::
	.string "{JPN}あれ?\n"
	.string "もう ポケモンが ないや$"

ViridianForest_Text_DougPostBattle::
	.string "{JPN}くやしいな!\n"
	.string "つよいのを つかまえて こよう!$"

ViridianForest_Text_SammyIntro::
	.string "{JPN}ちょっと まったあ!\n"
	.string "そんな いそいで どこに いく?$"

ViridianForest_Text_SammyDefeat::
	.string "{JPN}まいった!\n"
	.string "きみは つよいな!$"

ViridianForest_Text_SammyPostBattle::
	.string "{JPN}みえない ところ でも ホントは\n"
	.string "なにか おちてたり する!\p"
	.string "さっき おとしもの したんだ\n"
	.string "きみも さがして みて くれる?$"

ViridianForest_Text_AnthonyIntro::
	.string "{JPN}ボクが ちいさい からって\n"
	.string "てかげんは いやだよ!$"

ViridianForest_Text_AnthonyDefeat::
	.string "{JPN}あー だめだあ$"

ViridianForest_Text_AnthonyPostBattle::
	.string "{JPN}おこづかいが へって しまったあ$"

ViridianForest_Text_CharlieIntro::
	.string "{JPN}ポケモンが しんか するって\n"
	.string "きみ しってる?$"

ViridianForest_Text_CharlieDefeat::
	.string "{JPN}まけちゃった$"

ViridianForest_Text_CharliePostBattle::
	.string "{JPN}むしポケモンは しんかが\n"
	.string "はやくて たのしいよな!$"

ViridianForest_Text_RanOutOfPokeBalls::
	.string "{JPN}ポケモン とろうとして\n"
	.string "モンスター ボール なげてたら\l"
	.string "すぐ なくなっちゃった\p"
	.string "きみも おおめに\n"
	.string "かって おくと いいよ$"

ViridianForest_Text_AvoidGrassyAreasWhenWeak::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "もってる ポケモンが よわって きて\n"
	.string "たたかわせたく ない ときは\l"
	.string "くさむらを よけて かえろう!$"

ViridianForest_Text_UseAntidoteForPoison::
	.string "{JPN}どくを くらったら どくけし!\n"
	.string "フレンドリィ ショップで!$"

ViridianForest_Text_ContactOakViaPCToRatePokedex::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "ポケモンずかんは\n"
	.string "パソコンつうしんで\l"
	.string "オーキドはかせに みてもらえる!$"

ViridianForest_Text_CantCatchOwnedMons::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "ひとの ポケモンは ひとの もの!\n"
	.string "やせいの ポケモンに だけ\l"
	.string "モンスターボールを なげて\l"
	.string "つかまえよう!$"

ViridianForest_Text_WeakenMonsBeforeCapture::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "ポケモンを つかまえる ときは\n"
	.string "なるべく よわらせてから!\p"
	.string "ポケモンが げんきだと\n"
	.string "にげるかも しれないよ!$"

ViridianForest_Text_LeavingViridianForest::
	.string "{JPN}トキワの もり … でぐち\n"
	.string "このさき ニビ シティ$"

