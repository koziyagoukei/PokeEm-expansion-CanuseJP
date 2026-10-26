#include "config/general.h"
#include "config/battle.h"
#include "config/item.h"
#include "config/save.h"
#include "constants/global.h"
#include "constants/apprentice.h"
#include "constants/apricorn_tree.h"
#include "constants/battle.h"
#include "constants/battle_arena.h"
#include "constants/battle_dome.h"
#include "constants/battle_factory.h"
#include "constants/battle_frontier.h"
#include "constants/battle_palace.h"
#include "constants/battle_pike.h"
#include "constants/battle_pyramid.h"
#include "constants/battle_special.h"
#include "constants/battle_tent.h"
#include "constants/battle_tower.h"
#include "constants/berry.h"
#include "constants/cable_club.h"
#include "constants/coins.h"
#include "constants/comparison_operators.h"
#include "constants/contest.h"
#include "constants/daycare.h"
#include "constants/decorations.h"
#include "constants/difficulty.h"
#include "constants/easy_chat.h"
#include "constants/event_objects.h"
#include "constants/event_object_movement.h"
#include "constants/fame_checker.h"
#include "constants/field_effects.h"
#include "constants/field_move.h"
#include "constants/field_poison.h"
#include "constants/field_specials.h"
#include "constants/field_tasks.h"
#include "constants/field_weather.h"
#include "constants/flags.h"
#include "constants/follower_npc.h"
#include "constants/frontier_story.h"
#include "constants/frontier_util.h"
#include "constants/frontier_music.h"
#include "constants/frontier_practice.h"
#include "constants/game_stat.h"
#include "constants/info_viewer.h"
#include "constants/item.h"
#include "constants/items.h"
#include "constants/heal_locations.h"
#include "constants/layouts.h"
#include "constants/lilycove_lady.h"
#include "constants/map_scripts.h"
#include "constants/maps.h"
#include "constants/mass_outbreak.h"
#include "constants/mauville_old_man.h"
#include "constants/metatile_labels.h"
#include "constants/move_relearner.h"
#include "constants/moves.h"
#include "constants/mystery_gift.h"
#include "constants/party_menu.h"
#include "constants/pokeball.h"
#include "constants/pokedex.h"
#include "constants/pokemon.h"
#include "constants/pokemon_size_record.h"
#include "constants/random_mon_generation.h"
#include "constants/rtc.h"
#include "constants/roulette.h"
#include "constants/script_menu.h"
#include "constants/seagallop.h"
#include "constants/secret_bases.h"
#include "constants/siirtc.h"
#include "constants/songs.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/trade.h"
#include "constants/trainer_hill.h"
#include "constants/trainer_tower.h"
#include "constants/trainers.h"
#include "constants/trainer_card.h"
#include "constants/tv.h"
#include "constants/union_room.h"
#include "constants/vars.h"
#include "constants/weather.h"
#include "constants/speaker_names.h"
#include "constants/villa_shop.h"

#define NPC_TEXT_COLOR_MALE    0
#define NPC_TEXT_COLOR_FEMALE  1
#define NPC_TEXT_COLOR_NEUTRAL 2

	.include "asm/macros.inc"
	.include "asm/macros/event.inc"
	.include "constants/constants.inc"

	.section script_data, "aw", %progbits

	.set ALLOCATE_SCRIPT_CMD_TABLE, 1
	.include "data/script_cmd_table.inc"

.align 2
gSpecialVars::
	.4byte gSpecialVar_0x8000
	.4byte gSpecialVar_0x8001
	.4byte gSpecialVar_0x8002
	.4byte gSpecialVar_0x8003
	.4byte gSpecialVar_0x8004
	.4byte gSpecialVar_0x8005
	.4byte gSpecialVar_0x8006
	.4byte gSpecialVar_0x8007
	.4byte gSpecialVar_0x8008
	.4byte gSpecialVar_0x8009
	.4byte gSpecialVar_0x800A
	.4byte gSpecialVar_0x800B
	.4byte gSpecialVar_Facing
	.4byte gSpecialVar_Result
	.4byte gSpecialVar_ItemId
	.4byte gSpecialVar_LastTalked
	.4byte gSpecialVar_ContestRank
	.4byte gSpecialVar_ContestCategory
	.4byte gSpecialVar_MonBoxId
	.4byte gSpecialVar_MonBoxPos
	.4byte gSpecialVar_Unused_0x8014
	.4byte gTrainerBattleParameter + 2 // gTrainerBattleParameter.params.opponentA

	.purgem def_special
	.set ALLOCATE_SPECIAL_TABLE, 1
	.include "data/specials.inc"

gStdScripts::
	.4byte Std_ObtainItem              @ STD_OBTAIN_ITEM
	.4byte Std_FindItem                @ STD_FIND_ITEM
	.4byte Std_MsgboxNPC               @ MSGBOX_NPC
	.4byte Std_MsgboxSign              @ MSGBOX_SIGN
	.4byte Std_MsgboxDefault           @ MSGBOX_DEFAULT
	.4byte Std_MsgboxYesNo             @ MSGBOX_YESNO
	.4byte Std_MsgboxAutoclose         @ MSGBOX_AUTOCLOSE
	.4byte Std_ObtainDecoration        @ STD_OBTAIN_DECORATION
	.4byte Std_RegisteredInMatchCall   @ STD_REGISTER_MATCH_CALL
	.4byte Std_MsgboxGetPoints         @ MSGBOX_GETPOINTS
	.4byte Std_MsgboxPokenav           @ MSGBOX_POKENAV
	.4byte Std_PutItemAway             @ STD_PUT_ITEM_AWAY
	.4byte Std_ReceivedItem            @ STD_RECEIVED_ITEM
gStdScripts_End::

	.include "data/maps/LilycoveCity_LilycoveMuseum_1F/scripts.inc"
	.include "data/maps/LilycoveCity_LilycoveMuseum_2F/scripts.inc"
	.include "data/maps/LilycoveCity_ContestLobby/scripts.inc"
	.include "data/maps/LilycoveCity_ContestHall/scripts.inc"
	.include "data/maps/ArtisanCave_B1F/scripts.inc"
	.include "data/maps/ArtisanCave_1F/scripts.inc"
	.include "data/scripts/shared_secret_base.inc"
	.include "data/scripts/frontier_intro.inc"
	.include "data/scripts/frontier_story.inc"
	.include "data/scripts/frontier_practice.inc"
	.include "data/maps/BattleColosseum_2P/scripts.inc"
	.include "data/maps/TradeCenter/scripts.inc"
	.include "data/maps/RecordCorner/scripts.inc"
	.include "data/maps/BattleColosseum_4P/scripts.inc"
	.include "data/maps/ContestHall/scripts.inc"
	.include "data/maps/BattlePyramidSquare01/scripts.inc"
	.include "data/maps/UnionRoom/scripts.inc"
	.include "data/maps/BattleFrontier_OutsideWest/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerElevator/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerCorridor/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_OutsideEast/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerMultiPartnerRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerMultiCorridor/scripts.inc"
	.include "data/maps/BattleFrontier_BattleTowerMultiBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattleDomeLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattleDomeCorridor/scripts.inc"
	.include "data/maps/BattleFrontier_BattleDomePreBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattleDomeBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePalaceLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePalaceCorridor/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePalaceBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePyramidLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePyramidFloor/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePyramidTop/scripts.inc"
	.include "data/maps/BattleFrontier_BattleArenaLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattleArenaCorridor/scripts.inc"
	.include "data/maps/BattleFrontier_BattleArenaBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattleFactoryLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattleFactoryPreBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattleFactoryBattleRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePikeLobby/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePikeCorridor/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePikeThreePathRoom/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePikeRoomNormal/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePikeRoomFinal/scripts.inc"
	.include "data/maps/BattleFrontier_BattlePikeRoomWildMons/scripts.inc"
	.include "data/maps/BattleFrontier_RankingHall/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge1/scripts.inc"
	.include "data/maps/BattleFrontier_ExchangeServiceCorner/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge2/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge3/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge4/scripts.inc"
	.include "data/maps/BattleFrontier_ScottsHouse/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge5/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge6/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge7/scripts.inc"
	.include "data/maps/BattleFrontier_ReceptionGate/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge8/scripts.inc"
	.include "data/maps/BattleFrontier_Lounge9/scripts.inc"
	.include "data/maps/BattleFrontier_PokemonCenter_1F/scripts.inc"
	.include "data/maps/BattleFrontier_PokemonCenter_2F/scripts.inc"
	.include "data/maps/BattleFrontier_Mart/scripts.inc"

.if IS_FRLG

@ FRLG scripts

	.include "data/scripts/trainer_tower.inc"
	.include "data/scripts/flavor_text.inc"

.endif

	.include "data/scripts/std_msgbox.inc"
	.include "data/scripts/trainer_battle.inc"
	.include "data/scripts/new_game.inc"

	.include "data/scripts/config.inc"
	.include "data/scripts/debug.inc"

EventScript_WhiteOut::
	call EverGrandeCity_HallOfFame_EventScript_ResetEliteFour
	goto EventScript_ResetMrBriney
	end

EventScript_AfterWhiteOutHeal::
	lockall
	msgbox gText_FirstShouldRestoreMonsHealth
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	call_if_unset FLAG_DEFEATED_RUSTBORO_GYM, EventScript_AfterWhiteOutHealMsgPreFirstBoss
	call_if_set FLAG_DEFEATED_RUSTBORO_GYM, EventScript_AfterWhiteOutHealMsg
	applymovement VAR_LAST_TALKED, Movement_PkmnCenterNurse_Bow
	waitmovement 0
	fadedefaultbgm
	releaseall
	end

EventScript_AfterWhiteOutHealMsgPreFirstBoss::
	msgbox gText_MonsHealedShouldBuyPotions
	return

EventScript_AfterWhiteOutHealMsg::
	msgbox gText_MonsHealed
	return

EventScript_AfterWhiteOutMomHeal::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox gText_HadQuiteAnExperienceTakeRest
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox gText_MomExplainHPGetPotions
	fadedefaultbgm
	releaseall
	end

EventScript_ResetMrBriney::
	goto_if_eq VAR_BRINEY_LOCATION, 1, EventScript_MoveMrBrineyToHouse
	goto_if_eq VAR_BRINEY_LOCATION, 2, EventScript_MoveMrBrineyToDewford
	goto_if_eq VAR_BRINEY_LOCATION, 3, EventScript_MoveMrBrineyToRoute109
	end

EventScript_MoveMrBrineyToHouse::
	setflag FLAG_HIDE_MR_BRINEY_DEWFORD_TOWN
	setflag FLAG_HIDE_MR_BRINEY_BOAT_DEWFORD_TOWN
	setflag FLAG_HIDE_ROUTE_109_MR_BRINEY
	setflag FLAG_HIDE_ROUTE_109_MR_BRINEY_BOAT
	clearflag FLAG_HIDE_ROUTE_104_MR_BRINEY_BOAT
	clearflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	clearflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	end

EventScript_MoveMrBrineyToDewford::
	setflag FLAG_HIDE_ROUTE_109_MR_BRINEY
	setflag FLAG_HIDE_ROUTE_109_MR_BRINEY_BOAT
	setflag FLAG_HIDE_ROUTE_104_MR_BRINEY
	setflag FLAG_HIDE_ROUTE_104_MR_BRINEY_BOAT
	setflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	setflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	clearflag FLAG_HIDE_MR_BRINEY_DEWFORD_TOWN
	clearflag FLAG_HIDE_MR_BRINEY_BOAT_DEWFORD_TOWN
	end

EventScript_MoveMrBrineyToRoute109::
	setflag FLAG_HIDE_ROUTE_104_MR_BRINEY
	setflag FLAG_HIDE_ROUTE_104_MR_BRINEY_BOAT
	setflag FLAG_HIDE_BRINEYS_HOUSE_MR_BRINEY
	setflag FLAG_HIDE_BRINEYS_HOUSE_PEEKO
	setflag FLAG_HIDE_MR_BRINEY_DEWFORD_TOWN
	setflag FLAG_HIDE_MR_BRINEY_BOAT_DEWFORD_TOWN
	clearflag FLAG_HIDE_ROUTE_109_MR_BRINEY
	clearflag FLAG_HIDE_ROUTE_109_MR_BRINEY_BOAT
	end

EverGrandeCity_HallOfFame_EventScript_ResetEliteFour::
	clearflag FLAG_DEFEATED_ELITE_4_SIDNEY
	clearflag FLAG_DEFEATED_ELITE_4_PHOEBE
	clearflag FLAG_DEFEATED_ELITE_4_GLACIA
	clearflag FLAG_DEFEATED_ELITE_4_DRAKE
	setvar VAR_ELITE_4_STATE, 0
	return

Common_EventScript_UpdateBrineyLocation::
	goto_if_unset FLAG_RECEIVED_POKENAV, Common_EventScript_NopReturn
	goto_if_set FLAG_DEFEATED_PETALBURG_GYM, Common_EventScript_NopReturn
	goto_if_unset FLAG_HIDE_ROUTE_104_MR_BRINEY_BOAT, EventScript_SetBrineyLocation_House
	goto_if_unset FLAG_HIDE_MR_BRINEY_DEWFORD_TOWN, EventScript_SetBrineyLocation_Dewford
	goto_if_unset FLAG_HIDE_ROUTE_109_MR_BRINEY, EventScript_SetBrineyLocation_Route109
	return

EventScript_SetBrineyLocation_House::
	setvar VAR_BRINEY_LOCATION, 1
	return

EventScript_SetBrineyLocation_Dewford::
	setvar VAR_BRINEY_LOCATION, 2
	return

EventScript_SetBrineyLocation_Route109::
	setvar VAR_BRINEY_LOCATION, 3
	return

	.include "data/scripts/pkmn_center_nurse.inc"
	.include "data/scripts/obtain_item.inc"
	.include "data/scripts/pc.inc"
	.include "data/scripts/move_relearner.inc"

@ scripts/notices.inc? signs.inc? See comment about text/notices.inc
Common_EventScript_ShowPokemartSign::
	msgbox gText_PokemartSign, MSGBOX_SIGN
	end

Common_EventScript_ShowPokemonCenterSign::
	msgbox gText_PokemonCenterSign, MSGBOX_SIGN
	end

Common_ShowEasyChatScreen::
	fadescreen FADE_TO_BLACK
	special ShowEasyChatScreen
	fadescreen FADE_FROM_BLACK
	return

Common_EventScript_ReadyPetalburgGymForBattle::
	clearflag FLAG_HIDE_PETALBURG_GYM_GREETER
	setflag FLAG_PETALBURG_MART_EXPANDED_ITEMS
	return

Common_EventScript_BufferTrendyPhrase::
	dotimebasedevents
	setvar VAR_0x8004, 0
	special BufferTrendyPhraseString
	return

EventScript_BackupMrBrineyLocation::
	copyvar VAR_0x8008, VAR_BRINEY_LOCATION
	setvar VAR_BRINEY_LOCATION, 0
	return

	.include "data/scripts/surf.inc"

EventScript_CancelMessageBox::
	special UseBlankMessageToCancelPokemonPic
	release
	end

Common_EventScript_ShowBagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	release
	end

Common_EventScript_BagIsFull::
	msgbox gText_TooBadBagIsFull, MSGBOX_DEFAULT
	return

EventScript_BagIsFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox gText_TooBadBagIsFull
	release
	end

Common_EventScript_ShowNoRoomForDecor::
	msgbox gText_NoRoomLeftForAnother, MSGBOX_DEFAULT
	release
	end

Common_EventScript_NoRoomForDecor::
	msgbox gText_NoRoomLeftForAnother, MSGBOX_DEFAULT
	return

Common_EventScript_SetAbnormalWeather::
	setweather WEATHER_ABNORMAL
	return

Common_EventScript_PlayGymBadgeFanfare::
	playfanfare MUS_OBTAIN_BADGE
	waitfanfare
	return

Common_EventScript_OutOfCenterPartyHeal::
	fadescreenswapbuffers FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special HealPlayerParty
	callnative UpdateFollowingPokemon
	fadescreenswapbuffers FADE_FROM_BLACK
	return

Common_EventScript_PlayBrineysBoatMusic::
	setflag FLAG_DONT_TRANSITION_MUSIC
	playbgm MUS_SAILING, FALSE
	return

Common_EventScript_StopBrineysBoatMusic::
	clearflag FLAG_DONT_TRANSITION_MUSIC
	fadedefaultbgm
	return



Common_EventScript_NameReceivedPartyMon::
	fadescreen FADE_TO_BLACK
	special ChangePokemonNickname
	return

Common_EventScript_PlayerHandedOverTheItem::
	bufferitemname STR_VAR_1, VAR_0x8004
	playfanfare MUS_OBTAIN_TMHM
	message gText_PlayerHandedOverTheItem
	waitmessage
	waitfanfare
	removeitem VAR_0x8004
	return

	.include "data/scripts/movement.inc"
	.include "data/scripts/check_furniture.inc"
	.include "data/scripts/mart_clerk.inc"
	.include "data/text/pc.inc"
	.include "data/text/pkmn_center_nurse.inc"
	.include "data/text/obtain_item.inc"
	.include "data/text/move_relearner.inc"

@ The below and surf.inc could be split into some text/notices.inc
gText_PokemartSign::
	.string "べんりなどうぐ いろいろ あります\n"
	.string "{JPN}「フレンドリィショップ」$"

gText_PokemonCenterSign::
	.string "つかれた ポケモンも ひとやすみ!\n"
	.string "{JPN}「ポケモンセンター」$"

gText_MomOrDadMightLikeThisProgram::
	.string "{STR_VAR_1}が すきそうな ばんぐみをやってる!\n"
	.string "{JPN}…………………………………………………\pさきを いそがなきゃ!$"

gText_WhichFloorWouldYouLike::
	.string "ミナモ デパートへ ようこそ!\p"
	.string "{JPN}なんかいへ いきますか?$"

gText_SandstormIsVicious::
	.string "さばくの すなあらしが ひどくて\n"
	.string "{JPN}さきに すすめない!$"

gText_SelectWithoutRegisteredItem::
	.string "バッグに いれてある どうぐを\nべんりボタンに とうろく できます$"

gText_PokemonTrainerSchoolEmail::
	.string "パソコンに\nポケモン トレーナー こうざの\l"
	.string "{JPN}メールが きている!\p"
	.string "{JPN}…… …… ……\p"
	.string "{JPN}ポケモンが おぼえられる わざは 4つ!\p"
	.string "{JPN}どんな わざを おぼえさせるかで\n"
	.string "{JPN}トレーナーの じつりょくが とわれます!\p"
	.string "{JPN}…… …… ……$"

gText_PlayerHouseBootPC::
	.string "{PLAYER}は パソコンの\nスイッチを いれた!$"

gText_PokeblockLinkCanceled::
	.string "つうしんは キャンセルされました$"

gText_UnusedNicknameReceivedPokemon::
	.string "{JPN}もらった {STR_VAR_2}に\n"
	.string "{JPN}ニックネームを つけますか?$"

gText_PlayerWhitedOut::
	.string "{PLAYER}の てもとには\n"
	.string "{JPN}たたかえるポケモンが もういない!\p"
	.string "{JPN}{PLAYER}は\n"
	.string "{JPN}めのまえが まっくらに なった!$"

gText_FirstShouldRestoreMonsHealth::
	.string "まずは ポケモンを\n"
	.string "{JPN}かいふく させましょう$"

gText_MonsHealedShouldBuyPotions::
	.string "みんな げんきに なりましたよ!\p"
	.string "{JPN}ポケモンの たいりょく HPが\n"
	.string "{JPN}なくなってきたら\l"
	.string "{JPN}こまめに ここに きてくださいね\p"
	.string "{JPN}とおく おでかけに なるときは\n"
	.string "{JPN}フレンドリィショップで\l"
	.string "{JPN}キズぐすりを かっておくのが\l"
	.string "{JPN}おすすめです\p"
	.string "{JPN}それでは がんばってください!$"

gText_MonsHealed::
	.string "みんな げんきに なりましたよ!\p"
	.string "{JPN}それでは がんばってください!$"

gText_HadQuiteAnExperienceTakeRest::
	.string "ママ『{PLAYER}!\n"
	.string "{JPN}おかえりなさい\p"
	.string "{JPN}たいへん だったわね\p"
	.string "{JPN}すこし やすんで いったら\n"
	.string "{JPN}どうかしら……?\l"
	.string "{JPN}…… …… ……$"

gText_MomExplainHPGetPotions::
	.string "ママ『あらあら!\n"
	.string "{JPN}あなたも ポケモンも\l"
	.string "{JPN}げんき いっぱいね!\p"
	.string "{JPN}さっき オダマキはかせから\n"
	.string "{JPN}きいたんだけど……\p"
	.string "{JPN}ポケモンの たいりょく HPが\n"
	.string "{JPN}なくなってきたら\p"
	.string "{JPN}こまめに ポケモンセンターで\n"
	.string "{JPN}かいふく させるか\p"
	.string "{JPN}とおくに ぼうけんするときは\n"
	.string "{JPN}フレンドリィショップで\l"
	.string "{JPN}キズぐすりを かっておくのが\l"
	.string "{JPN}きほん らしいわよ\p"
	.string "{JPN}それじゃ おうえん しているわ!\p"
	.string "{JPN}きを つけて!\n"
	.string "{JPN}いってらっしゃい!$"

gText_RegisteredTrainerinPokeNav::
	.string "{STR_VAR_1}の {STR_VAR_2}を\n"
	.string "{JPN}ポケナビに とうろく した!$"

gText_ComeBackWithSecretPower::
	.string "「ひみつのちから」って\n"
	.string "{JPN}わざマシン しってる?\p"
	.string "{JPN}おれら わざマシン 「ひみつのちから」が\n"
	.string "{JPN}だいすき なんだ\p"
	.string "{JPN}おれらの メンバーが どこかで くれるから\n"
	.string "{JPN}それを もらったら また おいで!\p"
	.string "{JPN}きみも メンバーとして\n"
	.string "{JPN}ひみつで いいものを うってあげるよ$"

gText_PokerusExplanation::
	.string "おあずかりした ポケモンに\n"
	.string "{JPN}ポケルスが ついて いるようです\p"
	.string "{JPN}くわしいことは わかって いないのですが\n"
	.string "{JPN}ポケルスと いうのは ポケモンに くっつく\l"
	.string "{JPN}ちいさな せいめいたいで\l"
	.string "{JPN}これが ついている あいだ\l"
	.string "{JPN}ポケモンが よく そだつ みたいです$"

	.include "data/text/surf.inc"

gText_DoorOpenedFarAway::
	.string "どこか とおくの とびらが\n"
	.string "{JPN}ひらいたような おとだ……$"

gText_BigHoleInTheWall::
	.string "かべに おおきな あなが あいている!$"

gText_SorryWirelessClubAdjustments::
	.string "もうしわけ ございません\n"
	.string "{JPN}ポケモン ワイヤレス クラブは\l"
	.string "{JPN}ただいま ちょうせいちゅう です$"

gText_UndergoingAdjustments::
	.string "ちょうせいちゅうの ようだ$"

@ Unused
gText_SorryTradeCenterInspections::
	.string "{JPN}もうしわけ ございません\n"
	.string "{JPN}ただいま こちらの トレードセンターは\l"
	.string "{JPN}てんけんちゅう です$"

@ Unused
gText_SorryRecordCornerPreparation::
	.string "{JPN}もうしわけ ございません\n"
	.string "{JPN}ただいま こちらの レコードコーナーは\l"
	.string "{JPN}じゅんびちゅう です$"

gText_PlayerHandedOverTheItem::
	.string "{PLAYER}は\n"
	.string "{JPN}{STR_VAR_1}を わたした!$"

gText_ThankYouForAccessingMysteryGift::
	.string "ふしぎな おくりものを ごりよう\n"
	.string "{JPN}いただき ありがとう ございます!$"

gText_PlayerFoundOneTMHM::
	.string "{PLAYER}は {STR_VAR_1}\n"
	.string "{JPN}「{STR_VAR_2}」を みつけた!$"

gText_PlayerFoundTMHMs::
	.string "{PLAYER}は {STR_VAR_1}\n"
	.string "{JPN}「{STR_VAR_2}」を {STR_VAR_3}コ みつけた!$"

gText_Sudowoodo_Attacked::
	.string "おかしな きは\n"
	.string "{JPN}ホエルコじょうろを いやがっている!\p"
	.string "{JPN}おかしな きが おそいかかってきた!$"

gText_LegendaryFlewAway::
	.string "{STR_VAR_1}は\nどこかへ とびさって いった!$"

gText_WantWhichFloor::
	.string "なんかいへ いきますか?$"

	.include "data/text/pc_transfer.inc"
	.include "data/text/questionnaire.inc"
	.include "data/text/abnormal_weather.inc"

EventScript_GetInGameTradeSpeciesInfo::
	copyvar VAR_0x8005, VAR_0x8008
	specialvar VAR_0x8009, GetInGameTradeSpeciesInfo
	return

EventScript_ChooseMonForInGameTrade::
	special ChoosePartyMon
	lock
	faceplayer
	return

EventScript_GetInGameTradeSpecies::
	specialvar VAR_RESULT, GetTradeSpecies
	return

EventScript_DoInGameTrade::
	special CreateInGameTradePokemon
	special DoInGameTradeScene
	lock
	faceplayer
	return

EventScript_SelectWithoutRegisteredItem::
	msgbox gText_SelectWithoutRegisteredItem, MSGBOX_SIGN
	end

	.include "data/scripts/field_poison.inc"

Common_EventScript_NopReturn::
	return

EventScript_SetResultTrue::
	setvar VAR_RESULT, TRUE
	return

EventScript_SetResultFalse::
	setvar VAR_RESULT, FALSE
	return

EventScript_GetElevatorFloor::
	special GetElevatorFloor
	return

@ Unused
EventScript_CableClub_SetVarResult1::
	setvar VAR_RESULT, 1
	return

EventScript_CableClub_SetVarResult0::
	setvar VAR_RESULT, 0
	return

Common_EventScript_UnionRoomAttendant::
#if IS_FRLG
	call CableClub_EventScript_UnionRoomAttendant_Frlg
#else
	call CableClub_EventScript_UnionRoomAttendant
#endif
	end

Common_EventScript_WirelessClubAttendant::
#if IS_FRLG
	call CableClub_EventScript_WirelessClubAttendant_Frlg
#else
	call CableClub_EventScript_WirelessClubAttendant
#endif
	end

Common_EventScript_DirectCornerAttendant::
#if IS_FRLG
	call CableClub_EventScript_DirectCornerAttendant_Frlg
#else
	call CableClub_EventScript_DirectCornerAttendant
#endif
	end

Common_EventScript_RemoveStaticPokemon::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	release
	end

Common_EventScript_LegendaryFlewAway::
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject VAR_LAST_TALKED
	fadescreenswapbuffers FADE_FROM_BLACK
	bufferspeciesname STR_VAR_1, VAR_0x8004
	msgbox gText_LegendaryFlewAway, MSGBOX_DEFAULT
	release
	end

EventScript_VsSeekerChargingDone::
	special VsSeekerFreezeObjectsAfterChargeComplete
	waitstate
	special VsSeekerResetObjectMovementAfterChargeComplete
	releaseall
	end

@ FRLG scripts

EventScript_SetExitingCyclingRoad::
	lockall
	clearflag FLAG_SYS_ON_CYCLING_ROAD
	setvar VAR_MAP_SCENE_ROUTE16, 0
	releaseall
	end

EventScript_SetEnteringCyclingRoad::
	lockall
	setvar VAR_MAP_SCENE_ROUTE16, 1
	releaseall
	end

EventScript_TryDarkenRuins::
	goto_if_set FLAG_SYS_UNLOCKED_TANOBY_RUINS, Common_EventScript_NopReturn
	setweather WEATHER_SHADE
	doweather
	return

Text_MonFlewAway::
	.string "{STR_VAR_1}は とんでいった!$"

@ Call for legendary bird trio
Text_Gyaoo::
	.string "{JPN}ギャーオ!$"

EventScript_BrailleCursorWaitButton::
	special BrailleCursorToggle
	waitbuttonpress
	closebraillemessage
	playse SE_SELECT
	setvar VAR_0x8006, 1
	special BrailleCursorToggle
	return

EventScript_PalletTown_PlayersHouse_2F_ShutDownPC::
	setvar VAR_0x8004, PC_LOCATION_PLAYER_HOUSE_FRLG
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	releaseall
	end

EventScript_PalletTown_PlayersHouse_2F_TurnOnPC::
	lockall
	setvar VAR_0x8004, PC_LOCATION_PLAYER_HOUSE_FRLG
	special DoPCTurnOnEffect
	playse SE_PC_ON
	msgbox gText_PlayerHouseBootPC
	special BedroomPC
	releaseall
	end

	.include "data/scripts/questionnaire.inc"
	.include "data/scripts/abnormal_weather.inc"
	.include "data/scripts/trainer_script.inc"
	.include "data/scripts/berry_tree.inc"
	.include "data/scripts/secret_base.inc"
	.include "data/scripts/cable_club.inc"
	.include "data/text/cable_club.inc"
	.include "data/scripts/contest_hall.inc"
	.include "data/scripts/tv.inc"
	.include "data/text/tv.inc"
	.include "data/scripts/interview.inc"
	.include "data/text/pokemon_news.inc"
	.include "data/scripts/mauville_man.inc"
	.include "data/scripts/field_move_scripts.inc"
	.include "data/scripts/item_ball_scripts.inc"
	.include "data/scripts/day_care.inc"
	.include "data/scripts/flash.inc"
	.include "data/scripts/berry_blender.inc"
	.include "data/text/mauville_man.inc"
	.include "data/text/trainers.inc"
	.include "data/scripts/repel.inc"
	.include "data/scripts/safari_zone.inc"
	.include "data/text/pokedex_rating.inc"
	.include "data/text/check_furniture.inc"
	.include "data/scripts/cave_hole.inc"
	.include "data/text/match_call.inc"
	.include "data/scripts/apprentice.inc"
	.include "data/text/apprentice.inc"
	.include "data/scripts/battle_pike.inc"
	.include "data/text/blend_master.inc"
	.include "data/text/move_tutors.inc"
	.include "data/scripts/move_tutors.inc"
	.include "data/scripts/trainer_hill.inc"
	.include "data/scripts/test_signpost.inc"
	.include "data/scripts/follower.inc"
	.include "data/text/save.inc"
	.include "data/text/birch_speech.inc"
	.include "data/scripts/dexnav.inc"
	.include "data/scripts/battle_frontier.inc"
	.include "data/scripts/wild_encounter.inc"

	.include "data/maps/ultra_frontie_hub/scripts.inc"
	.include "data/maps/BattleFrontier_OutsideSouth/scripts.inc"
	.include "data/maps/PartyRoamerPark/scripts.inc"
	.include "data/maps/MiniMiniVilla/scripts.inc"
	.include "data/maps/MiniMiniVilla_LocalRoom/scripts.inc"
	.include "data/maps/MiniMiniVilla_Shop/scripts.inc"
	.include "data/maps/PartyResortGate/scripts.inc"
	.include "data/maps/RestorationInn_Outside/scripts.inc"
	.include "data/maps/RestorationInn_Lobby/scripts.inc"
	.include "data/maps/RestorationInn_EastWing/scripts.inc"
	.include "data/maps/RestorationInn_WestWing/scripts.inc"

	.include "data/maps/Partybathtime/scripts.inc"

	.include "data/maps/Party_eat_time/scripts.inc"

	.include "data/maps/ultraflontie_PokemonCenter_2F_2/scripts.inc"
