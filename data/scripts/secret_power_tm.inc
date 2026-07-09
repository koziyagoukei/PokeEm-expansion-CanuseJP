Route111_Text_MakingRoomUseTMToMakeYourOwn:
	.string "{JPN}えっ なにを してるかって?\pポケモンの わざを つかって\nここに ぼくだけの へやを\lつくろうと おもってね!\pそうだっ この わざマシンを あげるから\nきみも つくって みるかい?$"

Route111_Text_ExplainSecretPower:
	.string "{JPN}ツルが おちそうな きにむかって\nひみつのちからを つかってごらん!\lツルが おちてきて きのうえに のぼれるよ!\pそれでね うえには ひろいへやが あるから\nぼくは そこに すきなものを おくんだ!\lじぶんだけの ひみつのへや…\lそう ひみつきちさ!\lきみも つくると いいよー\pそうそう ひみつきちを つくれる ばしょは\nきのうえ だけじゃ ないよ!\pちいさな くぼみがある かべなんかにも\nひみつのちからを つかって みるといいよ!\pぼくも ほかの ばしょを さがして みるね\nそれじゃ!$"

Route111_Text_DontWantThis:
	.string "{JPN}そう いらないんだ…\nほしくなったら また こえかけてね!$"

Route111_Text_DontHaveAnyRoom:
	.string "{JPN}あれ?\nそれいじょうは もてない みたいだよ\pまた こんど とりに きてね!$"

Route111_EventScript_SecretPowerMan::
	lock
	faceplayer
	msgbox Route111_Text_MakingRoomUseTMToMakeYourOwn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route111_EventScript_GiveSecretPower
	msgbox Route111_Text_DontWantThis, MSGBOX_DEFAULT
	release
	end

Route111_EventScript_GiveSecretPower::
	giveitem ITEM_TM_SECRET_POWER
	goto_if_eq VAR_RESULT, FALSE, Route111_EventScript_NoRoomForSecretPower
	msgbox Route111_Text_ExplainSecretPower, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RECEIVED_SECRET_POWER
	clearflag FLAG_HIDE_SLATEPORT_CITY_TM_SALESMAN
	call_if_eq VAR_FACING, DIR_WEST, Route111_EventScript_SecretPowerManExit
	call_if_eq VAR_FACING, DIR_EAST, Route111_EventScript_SecretPowerManExit
	call_if_eq VAR_FACING, DIR_NORTH, Route111_EventScript_SecretPowerManExitNorth
	removeobject VAR_LAST_TALKED
	release
	end

Route111_EventScript_SecretPowerManExit::
	applymovement VAR_LAST_TALKED, Route111_Movement_SecretPowerManExit
	waitmovement 0
	return

Route111_EventScript_SecretPowerManExitNorth::
	applymovement VAR_LAST_TALKED, Route111_Movement_SecretPowerManExitNorth
	waitmovement 0
	return

Route111_EventScript_NoRoomForSecretPower::
	msgbox Route111_Text_DontHaveAnyRoom, MSGBOX_DEFAULT
	release
	end

Route111_Movement_SecretPowerManExit:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end

Route111_Movement_SecretPowerManExitNorth:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	step_end
