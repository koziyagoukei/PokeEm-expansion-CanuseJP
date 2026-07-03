VerdanturfTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VerdanturfTown_OnTransition
	.byte 0

VerdanturfTown_OnTransition:
	setflag FLAG_VISITED_VERDANTURF_TOWN
	setvar VAR_CONTEST_HALL_STATE, 0
	end

VerdanturfTown_EventScript_Twin::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_EventScript_TwinTunnelOpen
	msgbox VerdanturfTown_Text_ManTryingToDigTunnel, MSGBOX_DEFAULT
	applymovement LOCALID_VERDANTURF_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

VerdanturfTown_EventScript_TwinTunnelOpen::
	msgbox VerdanturfTown_Text_ManDugTunnelForLove, MSGBOX_DEFAULT
	applymovement LOCALID_VERDANTURF_TWIN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

VerdanturfTown_EventScript_Man::
	msgbox VerdanturfTown_Text_AirCleanHere, MSGBOX_NPC
	end

VerdanturfTown_EventScript_Camper::
	msgbox VerdanturfTown_Text_MakeBattleTentDebut, MSGBOX_NPC
	end

VerdanturfTown_EventScript_Boy::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_EventScript_BoyTunnelOpen
	msgbox VerdanturfTown_Text_GuyTryingToBustThroughCave, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_EventScript_BoyTunnelOpen::
	msgbox VerdanturfTown_Text_EasyToGetToRustboroNow, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_EventScript_TownSign::
	msgbox VerdanturfTown_Text_TownSign, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_WandasHouseSign::
	msgbox VerdanturfTown_Text_WandasHouse, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_BattleTentSign::
	msgbox VerdanturfTown_Text_BattleTentSign, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_RusturfTunnelSign::
	msgbox VerdanturfTown_Text_RusturfTunnelSign, MSGBOX_SIGN
	end

VerdanturfTown_Text_ManTryingToDigTunnel:
	.string "{JPN}わたしの パパが いってたの\pこの トンネルって おーきな おとに\nはんのうして さわぎだす おくびょうな\lポケモンが いっぱい なんだって!\pだから かいはつ こうじ やめたんだけど\nおとこの ひとが ひとりで\lトンネルを ほろうと してるんだって!$"

VerdanturfTown_Text_ManDugTunnelForLove:
	.string "{JPN}あいする ひとの ために\nどうくつを ほっていた ひとが いるの!\pなんだか すごーい!$"

VerdanturfTown_Text_AirCleanHere:
	.string "{JPN}かざむきの かんけいで このまち には\nかざんばいが とんでくることは ない!\pくうきの おいしい このまちで くらせば\nからだが じょうぶでない ひとでも\lきっと げんきに なれる はずさ!$"

VerdanturfTown_Text_MakeBattleTentDebut:
	.string "{JPN}ぼくと ぼくの ポケモン\nさいきん すごく ちょうしが よくてね\pバトルテント デビュー させたくて\nこの まちに きたんだ!$"

VerdanturfTown_Text_GuyTryingToBustThroughCave:
	.string "{JPN}フレンドリィ ショップの となりに\nどうくつが あるだろ?\pなかを ふさいでいる いわを こわし\nとおれるように しようと がんばっている\lやつが いるらしいんだ!\pとおれるように なると いいのに……\nカナズミにも いきやすく なるから!$"

VerdanturfTown_Text_EasyToGetToRustboroNow:
	.string "{JPN}フレンドリィ ショップの となりの\nどうくつが とおれるように なっただろ!\pカナズミまで デボンの しんせいひんを\nかいに いきやすくなって うれしいよ!$"

VerdanturfTown_Text_TownSign:
	.string "{JPN}ここは シダケ タウン\nくさの においの かぜふく こうげん$"

VerdanturfTown_Text_WandasHouse:
	.string "{JPN}「ミチルの いえ」$"

VerdanturfTown_Text_BattleTentSign:
	.string "{JPN}バトルテント シダケタウン かいじょう\nみまもれ! ポケモンバトル$"

VerdanturfTown_Text_RusturfTunnelSign:
	.string "{JPN}カナズミと シダケを むすぶ\n「カナシダ トンネル」\pかいつう こうじは ちゅうしに なりました$"

