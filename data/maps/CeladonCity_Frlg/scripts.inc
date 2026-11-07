CeladonCity_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_OnTransition
	.byte 0

CeladonCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CELADON_CITY
	end

CeladonCity_EventScript_LittleGirl::
	msgbox CeladonCity_Text_GotMyKoffingInCinnabar, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan1::
	lock
	msgbox CeladonCity_Text_GymIsGreatFullOfWomen
	release
	end

CeladonCity_EventScript_Woman::
	msgbox CeladonCity_Text_GameCornerIsBadForCitysImage, MSGBOX_NPC
	end

CeladonCity_EventScript_OldMan2::
	msgbox CeladonCity_Text_BlewItAllAtSlots, MSGBOX_NPC
	end

CeladonCity_EventScript_SoftboiledTutor::
	goto EventScript_SoftboiledTutor
	end

CeladonCity_EventScript_FatMan::
	lock
	faceplayer
	msgbox CeladonCity_Text_MyTrustedPalPoliwrath
	closemessage
	applymovement LOCALID_CELADON_FAT_MAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_Poliwrath::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_POLIWRATH, CRY_MODE_NORMAL
	msgbox CeladonCity_Text_Poliwrath
	closemessage
	waitmoncry
	applymovement LOCALID_CELADON_POLIWRATH, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeladonCity_EventScript_RocketGrunt1::
	msgbox CeladonCity_Text_KeepOutOfTeamRocketsWay, MSGBOX_NPC
	end

CeladonCity_EventScript_RocketGrunt2::
	msgbox CeladonCity_Text_GetLostOrIllPunchYou, MSGBOX_NPC
	end

CeladonCity_EventScript_Boy::
	msgbox CeladonCity_Text_ScaldedTongueOnTea, MSGBOX_NPC
	end

CeladonCity_EventScript_SilphCoScientist::
	msgbox CeladonCity_Text_SomeoneStoleSilphScope, MSGBOX_NPC
	end

CeladonCity_EventScript_TrainerTips1::
	msgbox CeladonCity_Text_ExplainXAccuracyDireHit, MSGBOX_SIGN
	end

CeladonCity_EventScript_CitySign::
	msgbox CeladonCity_Text_CitySign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_ERIKA, 0
	msgbox CeladonCity_Text_GymSign
	releaseall
	end

CeladonCity_EventScript_MansionSign::
	msgbox CeladonCity_Text_MansionSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_DeptStoreSign::
	msgbox CeladonCity_Text_DeptStoreSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_TrainerTips2::
	msgbox CeladonCity_Text_GuardSpecProtectsFromStatus, MSGBOX_SIGN
	end

CeladonCity_EventScript_PrizeExchangeSign::
	msgbox CeladonCity_Text_PrizeExchangeSign, MSGBOX_SIGN
	end

CeladonCity_EventScript_GameCornerSign::
	msgbox CeladonCity_Text_GameCornerSign, MSGBOX_SIGN
	end

CeladonCity_Text_GotMyKoffingInCinnabar::
	.string "{JPN}わたしの ドガース!\n"
	.string "グレンじま で つかまえたの!\l"
	.string "おこると どくガス はくけど…\p"
	.string "ふだんは いいこ なのよ$"

CeladonCity_Text_GymIsGreatFullOfWomen::
	.string "{JPN}にひひ! この ジムは ええ!\n"
	.string "おんなのこ ばっかし じゃ!$"

CeladonCity_Text_GameCornerIsBadForCitysImage::
	.string "{JPN}きれいな まちづくりが じまんの\n"
	.string "タマムシに ゲーム コーナーが\l"
	.string "できちゃって! こまった ものよ!$"

CeladonCity_Text_BlewItAllAtSlots::
	.string "{JPN}トホホ…!\n"
	.string "スロットで また まけた!\p"
	.string "コインが たまったら\n"
	.string "けいひんに した ほうが ええと\l"
	.string "わかって おったのに…!$"

CeladonCity_Text_MyTrustedPalPoliwrath::
	.string "{JPN}こいつ おれの あいかた!\n"
	.string "ニョロボン!\p"
	.string "みずのいしを あてたら\n"
	.string "ニョロゾ が しんか したよ$"

CeladonCity_Text_Poliwrath::
	.string "{JPN}ニョロボン『ゲロゲーロ!$"

CeladonCity_Text_GetLostOrIllPunchYou::
	.string "{JPN}なんだ ジロジロ みやがって!\n"
	.string "あっち いかねえと ぶんなぐるぞ!$"

CeladonCity_Text_KeepOutOfTeamRocketsWay::
	.string "{JPN}めの まえを チョロチョロうるせえ!\n"
	.string "ロケットだんを なめるなよ!$"

CeladonCity_Text_ExplainXAccuracyDireHit::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "ヨクアタールは わざの\n"
	.string "めいちゅう りつ を あげる!\p"
	.string "クリティカッターは わざが\n"
	.string "きゅうしょに あたり やすくなる\p"
	.string "アイテムの おかいもとめは\n"
	.string "タマムシ デパートで どうぞ!$"

CeladonCity_Text_CitySign::
	.string "{JPN}ここは タマムシ シティ\n"
	.string "タマムシ にじいろ ゆめの いろ$"

CeladonCity_Text_GymSign::
	.string "{JPN}タマムシ シティ ポケモン ジム\n"
	.string "リーダー エリカ\l"
	.string "しぜんを あいする おじょうさま!$"

CeladonCity_Text_MansionSign::
	.string "{JPN}タマムシ マンション$"

CeladonCity_Text_DeptStoreSign::
	.string "{JPN}ほしいもの きっと みつかる!\n"
	.string "タマムシ デパート$"

CeladonCity_Text_GuardSpecProtectsFromStatus::
	.string "{JPN}… おとくな けいじばん!\p"
	.string "エフェクトガードは\n"
	.string "せんとうちゅうに つかうと\l"
	.string "みかたの ポケモンの のうりょくを\l"
	.string "さげられずに すみます!\p"
	.string "どうぐの おかいもとめは\n"
	.string "タマムシ デパートで どうぞ!$"

CeladonCity_Text_PrizeExchangeSign::
	.string "{JPN}コインを ビッグな けいひんに!\n"
	.string "… けいひん こうかんじょ$"

CeladonCity_Text_GameCornerSign::
	.string "{JPN}おとなの あそびば!\n"
	.string "… ロケット ゲーム コーナー$"

CeladonCity_Text_ScaldedTongueOnTea::
	.string "{JPN}ひー いてて…\n"
	.string "べろ やけど しちゃった!\p"
	.string "マンションの ばーさんが\n"
	.string "くれた おちゃが\l"
	.string "あついの なんのって…\p"
	.string "さまさなきゃ のめないや\n"
	.string "ふー ふー$"

Text_SoftboiledTeach::
	.string "{JPN}やあ やあ!\p"
	.string "わたしは いつも きみが\n"
	.string "そこを とおるのを みてたが\l"
	.string "やっと あう ことが できた!\p"
	.string "この こううんを きねんして\n"
	.string "とっときの わざ\l"
	.string "タマゴうみを おしえよう!$"

Text_SoftboiledDeclined::
	.string "{JPN}わたしは いつでも\n"
	.string "みているから!\l"
	.string "また あえると いいな!$"

Text_SoftboiledWhichMon::
	.string "{JPN}タマゴうみを おぼえられる\n"
	.string "ラッキーな ポケモンは?$"

Text_SoftboiledTaught::
	.string "{JPN}きみの こううんを\n"
	.string "いのっている!$"

CeladonCity_Text_SomeoneStoleSilphScope::
	.string "{JPN}おろおろ…\p"
	.string "シルフスコープを ぬすまれた!\n"
	.string "ぬすんだ やつは かくじつに\l"
	.string "この あたりに きたんだが…\p"
	.string "みうしなって しまった!\n"
	.string "どこに はいったんだ?$"

