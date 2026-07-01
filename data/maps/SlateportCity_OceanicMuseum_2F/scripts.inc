SlateportCity_OceanicMuseum_2F_MapScripts::
	.byte 0

SlateportCity_OceanicMuseum_2F_EventScript_CaptStern::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts, MSGBOX_DEFAULT
	closemessage
	playbgm MUS_ENCOUNTER_AQUA, TRUE
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter
	waitmovement 0
	addobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	call_if_eq VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts, MSGBOX_DEFAULT
	call_if_ne VAR_FACING, DIR_EAST, SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou, MSGBOX_DEFAULT
	msgbox SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	waitmovement 0
	call_if_eq VAR_FACING, DIR_SOUTH, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth
	call_if_eq VAR_FACING, DIR_WEST, SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_1, SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat
	msgbox SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_GRUNT_MUSEUM_2, SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated
	waitmovement 0
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1, Common_Movement_WalkInPlaceFasterDown
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_MeddlingKid, MSGBOX_DEFAULT
	closemessage
	delay 35
	addobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter
	applymovement LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2, SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OCEANIC_MUSEUM_2F_ARCHIE, SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_ArchieWarning, MSGBOX_DEFAULT
	closemessage
	savebgm MUS_DUMMY
	fadedefaultbgm
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_OCEANIC_MUSEUM_2F_ARCHIE
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_1
	removeobject LOCALID_OCEANIC_MUSEUM_2F_GRUNT_2
	fadescreen FADE_FROM_BLACK
	delay 30
	setflag FLAG_HIDE_SLATEPORT_CITY_OCEANIC_MUSEUM_AQUA_GRUNTS
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_DEVON_PARTS
	call Common_EventScript_PlayerHandedOverTheItem
	msgbox SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	removeobject LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN
	setflag FLAG_HIDE_ROUTE_110_TEAM_AQUA
	call_if_eq VAR_REGISTER_BIRCH_STATE, 0, SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch
	setflag FLAG_DELIVERED_DEVON_GOODS
	clearflag FLAG_HIDE_ROUTE_116_DEVON_EMPLOYEE
	setflag FLAG_HIDE_RUSTBORO_CITY_DEVON_CORP_3F_EMPLOYEE
	setvar VAR_SLATEPORT_OUTSIDE_MUSEUM_STATE, 1
	fadescreen FADE_FROM_BLACK
	release
	end

SlateportCity_OceanicMuseum_2F_EventScript_ReadyRegisterBirch::
	setvar VAR_REGISTER_BIRCH_STATE, 1
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerFaceGrunts::
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_SternFaceGrunts::
	applymovement LOCALID_OCEANIC_MUSEUM_2F_CAPT_STERN, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntSouth::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_EventScript_PlayerApproachGruntWest::
	applymovement LOCALID_PLAYER, SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest
	waitmovement 0
	return

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntSouth:
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_PlayerApproachGruntWest:
	walk_up
	walk_left
	walk_left
	walk_down
	walk_in_place_faster_left
	step_end

SlateportCity_OceanicMuseum_2F_Movement_Unused:
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_left
	walk_left
	walk_up
	walk_up
	delay_8
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieApproachPlayer:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_ArchieEnter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntApproachToBattle:
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntEnter:
	walk_down
	walk_right
	walk_in_place_faster_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_FirstGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntDefeated:
	lock_facing_direction
	walk_left
	unlock_facing_direction
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntEnter:
	walk_down
	step_end

SlateportCity_OceanicMuseum_2F_Movement_SecondGruntApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

SlateportCity_OceanicMuseum_2F_Movement_GruntMoveForArchie:
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_fast_down
	walk_in_place_faster_up
	step_end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_WaterQualitySample2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_PressureExperiment::
	msgbox SlateportCity_OceanicMuseum_2F_Text_PressureExperiment, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_HoennModel::
	msgbox SlateportCity_OceanicMuseum_2F_Text_HoennModel, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_DeepSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SurfaceSeawaterDisplay::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSTidalReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmarineReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SubmersibleReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_SSAnneReplica::
	msgbox SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica, MSGBOX_SIGN
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron1::
	msgbox SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron2::
	msgbox SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum, MSGBOX_NPC
	end

SlateportCity_OceanicMuseum_2F_EventScript_MuseumPatron3::
	lock
	faceplayer
	msgbox SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SlateportCity_OceanicMuseum_2F_Text_ThankYouForTheParts:
	.string "{JPN}はい? クスノキは わたし だが……?\pおお! それは デボンの ツワブキさんに\nたのんで おいた パーツだね!\pいやー どうも ごくろうさま!\nおかげで しゅっぱつ できそうだ……"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_WellTakeThoseParts:
	.string "{JPN}へへへ その パーツ\nおれたちが いただくぜ!"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SternWhoAreYou:
	.string "{JPN}クスノキ『だ だれだ きみたちは!$"

SlateportCity_OceanicMuseum_2F_Text_WereTeamAqua:
	.string "{JPN}おれたち アクアだん!\pリーダーが そのパーツを ほしがってるんだ!\nなにも いわずに よこせ!"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_Grunt1Defeat:
	.string "{JPN}うわー!\nこどもに やられちまった!"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_BossGoingToBeFurious:
	.string "{JPN}やばい……\nリーダーに おこられちまうぞ……"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_LetMeTakeCareOfThis:
	.string "{JPN}ふん! だらしない やつめ!\nここは おれに まかせてもらうぜ!"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_Grunt2Defeat:
	.string "{JPN}なんだと!\nおれも まけてしまった!"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_MeddlingKid:
	.string "{JPN}ど どうしよう……\nパーツを うばえないと まずくないか?\pうむ……\nまさか こどもに じゃま されるなんて\lこれっぽっちも おもってなかったぜ"
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_CameToSeeWhatsTakingSoLong:
	.string "{JPN}パーツを うばうのに\nいつまで かかっているのかと おもえば\lこんな こどもに じゃま されていたのか?"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_ArchieWarning:
	.string "{JPN}れわれは\nうみを あいする アクアだん!\pそして おれは\nアクアだんの リーダー アオギリだ\pわれわれの じゃまをする おまえは……\nま まさか!\lマグマだんの なかま なのか!?\pいや それにしては\nみにつけている ふくが ちがうな\p…… …… ……\n…… …… ……\pポケモンも ひとも すべての いのちは\nうみから うまれた……\pだから アクアだんは\nうみを ふやそうと しているのだよ\pどうだい?\nとても すばらしい ことだろう?\pまあ いい……\nまだ わかい おまえに\lわれわれの りそうなど わかるまい……\pだが このさきも われわれの\nかつどうを じゃまする つもりなら\lただでは すまさない からな!\pそれだけは おぼえて おくのだ!"
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
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SternThankYouForSavingUs:
	.string "{JPN}クスノキ『きみは……\nそうか {PLAYER}{KUN} というのか……\pいやあ あぶない ところ だった!\nたすけてくれて ありがとうよ!\pおっと そうだった!\nデボンに たのんでおいた パーツを\lもってきて くれたの わすれていたよ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SternIveGotToGo:
	.string "{JPN}クスノキ『おっと!\nこうしちゃ いられない!\pそろそろ かいてい ちょうさに\nしゅっぱつ する じかんだ!\pわたしは これで しつれい するけど\nきみは ゆっくり かんないを\lけんぶつ していって くれたまえ!"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_RemindsMeOfAbandonedShip:
	.string "{JPN}ふねの もけいを みておると\nムロの ちかくに ある\lすてられぶねを おもいだすのう……"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_DontRunInMuseum:
	.string "{JPN}はくぶつかんの なかを\nはしり まわるんじゃ ありませんよッ!"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_WantToRideSubmarine:
	.string "{JPN}せんすいてい かっこいー!\nおれも のりてーよ!"
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample1:
	.string "{JPN}「すいしつ サンプル 1」\nと かかれた ラベルが はってある!\pうみは みんな つながって いるのに\nちいきに よって すこし ちがうらしい"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_WaterQualitySample2:
	.string "{JPN}「すいしつ サンプル 2」\nと かかれた ラベルが はってある!\pちいきが ちがえば うみの みずの\nしょっぱさも ちがったり するのかな?"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_PressureExperiment:
	.string "{JPN}ゴムで できた ボールが\nふくらんだり しぼんだり している!\p「うみの そこには すいあつ といって\n みずじたいの おもさが かかります」\p「あさい ところでは すいあつも かるく\n ふかい ところでは なんまんトン もの\l すいあつが かかるのです」"
	.string ""
	.string ""
	.string ""
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_HoennModel:
	.string "{JPN}「ホウエン ちほうの もけい」\pホウエンちほう ぜんたいの ミニチュアだ!\nミシロタウンは どのへん だろう?"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_DeepSeawaterDisplay:
	.string "{JPN}かいすいの ながれに ついての てんじだ!\p「うみの そこに ちかい ところ では\n みずの おんどや えんぶんの ちがいで\l うみの みずが ながれて いきます」"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SurfaceSeawaterDisplay:
	.string "{JPN}かいすいの ながれに ついての てんじだ!\p「うみの ひょうめん あたり では\n かぜの ちからに よって\l うみの みずが ながれて いきます」"
	.string ""
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SSTidalReplica:
	.string "{JPN}「れんらくせん タイドリップごう」\pこれは げんざい クスノキ ぞうせんじょで\nつくられている ふねの もけい です!"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SubmarineReplica:
	.string "{JPN}「せんすいてい かいえん 1ごう」\pうみの そこを ちょうさ するための\nこうせいのう せんすいていの もけい です!"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SumbersibleReplica:
	.string "{JPN}「せんすい ポッド」\pかいていで さぎょうを するための\nむじん こがたポッドの もけい です!"
	.string ""
	.string "$"

SlateportCity_OceanicMuseum_2F_Text_SSAnneReplica:
	.string "{JPN}「サント・アンヌ ごう」\pせかいを いっしゅう している\nごうか きゃくせんの もけい です!"
	.string ""
	.string "$"
