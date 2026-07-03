FallarborTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FallarborTown_OnTransition
	.byte 0

FallarborTown_OnTransition:
	setflag FLAG_VISITED_FALLARBOR_TOWN
	setvar VAR_CONTEST_HALL_STATE, 0
	clearflag FLAG_CONTEST_SKETCH_CREATED
	end

FallarborTown_EventScript_ExpertM::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_EVIL_TEAM_MT_CHIMNEY, FallarborTown_EventScript_ExpertMNormal
	msgbox FallarborTown_Text_ShadyCharactersCozmosHome, MSGBOX_DEFAULT
	release
	end

FallarborTown_EventScript_ExpertMNormal::
	msgbox FallarborTown_Text_RegionKnownForMeteors, MSGBOX_DEFAULT
	release
	end

FallarborTown_EventScript_Girl::
	msgbox FallarborTown_Text_MyPreciousAzurill, MSGBOX_NPC
	end

FallarborTown_EventScript_Gentleman::
	msgbox FallarborTown_Text_HaveYouChallengedFlannery, MSGBOX_NPC
	end

FallarborTown_EventScript_Azurill::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_AZURILL, CRY_MODE_NORMAL
	msgbox FallarborTown_Text_Azurill, MSGBOX_DEFAULT
	waitmoncry
	release
	end

FallarborTown_EventScript_BattleTentSign::
	msgbox FallarborTown_Text_BattleTentSign, MSGBOX_SIGN
	end

FallarborTown_EventScript_TownSign::
	msgbox FallarborTown_Text_TownSign, MSGBOX_SIGN
	end

FallarborTown_EventScript_MoveTutorSign::
	msgbox FallarborTown_Text_MoveTutorSign, MSGBOX_SIGN
	end

FallarborTown_Text_ShadyCharactersCozmosHome:
	.string "{JPN}なんだ なんだ!?\pソライシはかせの いえに\nあやしげな じんぶつが\lではいり しておった ようだが……$"

FallarborTown_Text_RegionKnownForMeteors:
	.string "{JPN}ここいら いったいは\nむかしから いんせきが おおくてな\pりゅうせいのたきも いんせきが おちて\nできたとか そう いわれておる$"

FallarborTown_Text_MyPreciousAzurill:
	.string "{JPN}ほら! みてちょうだい!\nわたしの たいせつな ルリリ!\pツヤツヤの ナデナデの フカフカよ!$"

FallarborTown_Text_Azurill:
	.string "{JPN}ルリリ『るーりーるー$"

FallarborTown_Text_HaveYouChallengedFlannery:
	.string "{JPN}フエンタウンの ジムリーダー\nアスナとは たたかったかね?\pあのこの おじいさんは すごかったよ\nなんでも ポケモンリーグの\lしてんのう だった らしいからね\pきっと アスナも これから\nすばらしい トレーナーに なるだろうね$"

FallarborTown_Text_BattleTentSign:
	.string "{JPN}バトルテント ハジツゲタウン かいじょう\nつどえ! さいきょう チーム$"

FallarborTown_Text_TownSign:
	.string "{JPN}ここは ハジツゲ タウン\nちいさな はたけが ある のうそん$"

FallarborTown_Text_MoveTutorSign:
	.string "{JPN}「わざおしえマニアの いえ」\nポケモンの わざ おしえます!$"
