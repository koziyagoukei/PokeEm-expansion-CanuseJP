MossdeepCity_GameCorner_1F_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MossdeepCity_GameCorner_1F_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, MossdeepCity_GameCorner_1F_OnWarp
	map_script MAP_SCRIPT_ON_LOAD, CableClub_OnLoad
	.byte 0

MossdeepCity_GameCorner_1F_OnWarp:
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_CheckTurnAttendant
	.2byte 0

MossdeepCity_GameCorner_1F_OnFrame:
	map_script_2 VAR_CABLE_CLUB_STATE, USING_MINIGAME, CableClub_EventScript_ExitMinigameRoom
	.2byte 0

@ Script is redundant, the label in the goto also does lock and faceplayer
MossdeepCity_GameCorner_1F_EventScript_InfoMan::
	lock
	faceplayer
	goto MossdeepCity_GameCorner_1F_EventScript_InfoMan2
	release
	end

@ Script is redundant, the label in the goto also does lock and faceplayer
MossdeepCity_GameCorner_1F_EventScript_OldMan::
	lock
	faceplayer
	goto MossdeepCity_GameCorner_1F_EventScript_OldMan2
	release
	end


@ Everything below is leftover from RS
@ The house that was here in RS was moved to Sootopolis and replaced by the Game Corner
@ The text below is unused and duplicated in its replacement in Sootopolis City
@ And the BG Door event was moved inaccessibly into a wall

RS_MysteryEventsHouse_EventScript_Door::
	msgbox RS_MysteryEventsHouse_Text_DoorIsLocked, MSGBOX_SIGN
	end

RS_MysteryEventsHouse_Text_OldManGreeting:
	.string "{JPN}わしも わかいころは\nポケモン トレーナーとして\lせかいじゅうを たび していたのじゃ\pジジイとなった いまでは\nわかい トレーナーの しょうぶを みるのが\lゆいいつの たのしみ じゃ$"

RS_MysteryEventsHouse_Text_DoorIsLocked:
	.string "とびらには かぎが かかって いるようだ!$"

RS_MysteryEventsHouse_Text_ChallengeVisitingTrainer:
	.string "{JPN}いまは {STR_VAR_1} という トレーナーが\nわしの いえに きておる!\p{STR_VAR_1}と しょうぶ してみたいのじゃな?$"

RS_MysteryEventsHouse_Text_YouWontBattle:
	.string "{JPN}しょうぶ せんのか……\nしょうぶを みられなくて ガッカリ……$"

RS_MysteryEventsHouse_Text_KeepItToA3On3:
	.string "{JPN}おお そうか そうか!\pじゃが この いえは\nあんまり がんじょう じゃないから\pせめて 3たい3の\nしょうぶに して くれんかのう?$"

RS_MysteryEventsHouse_Text_SaveYourProgress:
	.string "{JPN}じゃあ しょうぶの まえに\nレポートを かいて おいたほうが よいぞ$"

RS_MysteryEventsHouse_Text_HopeToSeeAGoodMatch:
	.string "{JPN}よい しょうぶを きたい しておるぞ!$"

RS_MysteryEventsHouse_Text_BattleTie:
	.string "{JPN}ひきわけ じゃったか!\pおたがい いっぽも ゆずらぬ\nすばらしい しょうぶ じゃったぞ!$"

RS_MysteryEventsHouse_Text_BattleWon:
	.string "{JPN}みごと じゃったぞ!\pまるで わしの わかい ころを\nみている ようじゃわい!$"

RS_MysteryEventsHouse_Text_BattleLost:
	.string "{JPN}ざんねん じゃったのう\nじゃが よい しょうぶ じゃったぞ!\lつぎこそは かてると よいのう$"
