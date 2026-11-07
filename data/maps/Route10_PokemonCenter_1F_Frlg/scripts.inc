.equ REQUIRED_OWNED_MONS, 20

Route10_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route10_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

Route10_PokemonCenter_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F
	setrespawn HEAL_LOCATION_ROUTE10
	end

Route10_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

Route10_PokemonCenter_1F_EventScript_FatMan::
	msgbox Route10_PokemonCenter_1F_Text_NuggetUselessSoldFor5000, MSGBOX_NPC
	end

Route10_PokemonCenter_1F_EventScript_Gentleman::
	msgbox Route10_PokemonCenter_1F_Text_EveryTypeStrongerThanOthers, MSGBOX_NPC
	end

Route10_PokemonCenter_1F_EventScript_Youngster::
	msgbox Route10_PokemonCenter_1F_Text_HeardGhostsHauntLavender, MSGBOX_NPC
	end

Route10_PokemonCenter_1F_EventScript_Aide::
	lock
	faceplayer
	call Route10_PokemonCenter_1F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_EVERSTONE_FROM_OAKS_AIDE, Route10_PokemonCenter_1F_EventScript_AlreadyGotEverstone
	msgbox Route10_PokemonCenter_1F_Text_GiveEverstoneIfCaught20Mons, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route10_PokemonCenter_1F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_OWNED_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route10_PokemonCenter_1F_Text_GreatHereYouGo
	checkitemspace ITEM_EVERSTONE
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route10_PokemonCenter_1F_Text_ReceivedEverstoneFromAide, ITEM_EVERSTONE
	setflag FLAG_GOT_EVERSTONE_FROM_OAKS_AIDE
	msgbox Route10_PokemonCenter_1F_Text_ExplainEverstone
	release
	end

Route10_PokemonCenter_1F_EventScript_AlreadyGotEverstone::
	msgbox Route10_PokemonCenter_1F_Text_ExplainEverstone
	release
	end

Route10_PokemonCenter_1F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_OWNED_MONS
	bufferitemname STR_VAR_2, ITEM_EVERSTONE
	return

Route10_PokemonCenter_1F_Text_EveryTypeStrongerThanOthers::
	.string "{JPN}ポケモンは タイプに よって\n"
	.string "ゆうりに たたかえる あいてと\l"
	.string "にがてな あいてが いるのだ$"

Route10_PokemonCenter_1F_Text_NuggetUselessSoldFor5000::
	.string "{JPN}つかいみち ないから きんのたまを\n"
	.string "うったら 5000円で うれた!$"

Route10_PokemonCenter_1F_Text_HeardGhostsHauntLavender::
	.string "{JPN}シオン タウンで\n"
	.string "ゆうれい ポケモンが でるって\l"
	.string "みんな さわいでるよ$"

Route10_PokemonCenter_1F_Text_GiveEverstoneIfCaught20Mons::
	.string "{JPN}あ… {PLAYER}{KUN}\n"
	.string "さがして いたんですよ!\l"
	.string "ぼく まいど おなじみ\l"
	.string "オーキドはかせの じょしゅです!\p"
	.string "ポケモンずかんが 20しゅるい\n"
	.string "あつまってる ようなら\l"
	.string "はかせから かわらずのいしを\l"
	.string "わたすように いわれて います!\p"
	.string "{PLAYER}{KUN} どうですか?\n"
	.string "つかまえた ポケモンは\l"
	.string "20しゅるい より おおい かな?$"

Route10_PokemonCenter_1F_Text_GreatHereYouGo::
	.string "{JPN}… … なるほど!\n"
	.string "つかまえた ポケモンは…\l"
	.string "{STR_VAR_3}しゅるい ですね!\n"
	.string "おめでとう!\p"
	.string "それでは\n"
	.string "これを おわたし します!$"

Route10_PokemonCenter_1F_Text_ReceivedEverstoneFromAide::
	.string "{JPN}{PLAYER}は じょしゅ から\n"
	.string "かわらずのいしを もらった!$"

Route10_PokemonCenter_1F_Text_ExplainEverstone::
	.string "{JPN}ポケモンを しんか させても\n"
	.string "ずかんは ふえる けど…\p"
	.string "どうしても しんか させたくない\n"
	.string "ポケモンが いたときは\l"
	.string "これを ポケモンに もたせれば\l"
	.string "しんか しないんだって\l"
	.string "はかせが いって いました$"

