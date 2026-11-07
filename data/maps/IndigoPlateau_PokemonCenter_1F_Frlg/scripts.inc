IndigoPlateau_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IndigoPlateau_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

IndigoPlateau_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_INDIGO_PLATEAU
	specialvar VAR_RESULT, IsNationalPokedexEnabled
	call_if_eq VAR_RESULT, TRUE, IndigoPlateau_PokemonCenter_1F_EventScript_CheckBlockDoor
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CheckBlockDoor::
	goto_if_set FLAG_IS_CHAMPION, Common_EventScript_NopReturn
	setobjectxyperm LOCALID_LEAGUE_DOOR_GUARD, 4, 2
	return

IndigoPlateau_PokemonCenter_1F_EventScript_DoorGuard::
	lock
	faceplayer
	specialvar VAR_RESULT, IsNationalPokedexEnabled
	goto_if_eq VAR_RESULT, TRUE, IndigoPlateau_PokemonCenter_1F_EventScript_CheckSeviiIslandComplete
	msgbox IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CheckSeviiIslandComplete::
	goto_if_set FLAG_IS_CHAMPION, IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete
	msgbox IndigoPlateau_PokemonCenter_1F_Text_LoreleiIsAbsentClosedForTimeBeing
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_GymGuy::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart IndigoPlateau_PokemonCenter_1F_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
IndigoPlateau_PokemonCenter_1F_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice::
	.string "{JPN}おーす!\n"
	.string "みらいの チャンピオン!\p"
	.string "ポケモン リーグ してんのうは\n"
	.string "4にん つづけて\l"
	.string "しょうぶ する ルールだ!\p"
	.string "まけたら ひとりめ から\n"
	.string "やりなおしに なるぞ!\l"
	.string "これが さいごだ! がんばれよ!$"

IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck::
	.string "{JPN}このさきは してんのうと\n"
	.string "ひとりずつ たたかうの!\l"
	.string "もし かてたら ドアが ひらいて\l"
	.string "おくの へやに すすめるわ!\l"
	.string "がんばって!$"

IndigoPlateau_PokemonCenter_1F_Text_LoreleiIsAbsentClosedForTimeBeing::
	.string "{JPN}せっかく きた のに\n"
	.string "ごめんなさい\p"
	.string "してんのうの カンナさんが\n"
	.string "いま おるすで\l"
	.string "ポケモンリーグは しばらく\l"
	.string "おやすみ してるの$"

IndigoPlateau_PokemonCenter_1F_Text_AgathaWhuppedUs::
	.string "{JPN}キクコの ゴーストポケモンは\n"
	.string "おそろしく つよいぜ…!\p"
	.string "かくとうポケモンを きたえまくって\n"
	.string "はりきって いどんだのに\l"
	.string "ぜんぜん ダメだった\p"
	.string "おまけに…\n"
	.string "きが みじかくて すぐどなるし\l"
	.string "あのバアさん こわすぎるよ…$"

IndigoPlateau_PokemonCenter_1F_Text_LancesCousinGymLeaderFarAway::
	.string "{JPN}してんのうに なるなんて\n"
	.string "エリートの ちすじ なんだろう…\p"
	.string "きいた はなしだと\n"
	.string "ワタルの いとこも\l"
	.string "どこか とおくの まちで\l"
	.string "ジムリーダーを している そうだ$"

