MysteryGiftScript_AuroraTicket::
	setvaddress MysteryGiftScript_AuroraTicket
	lock
	faceplayer
	vgoto_if_set FLAG_RECEIVED_AURORA_TICKET, AuroraTicket_Obtained
	vgoto_if_set FLAG_BATTLED_DEOXYS, AuroraTicket_Obtained
	checkitem ITEM_AURORA_TICKET
	vgoto_if_eq VAR_RESULT, TRUE, AuroraTicket_Obtained
	vmessage sText_AuroraTicketForYou
	waitmessage
	waitbuttonpress
	checkitemspace ITEM_AURORA_TICKET
	vgoto_if_eq VAR_RESULT, FALSE, AuroraTicket_NoBagSpace
	giveitem ITEM_AURORA_TICKET
	setflag FLAG_ENABLE_SHIP_BIRTH_ISLAND
	setflag FLAG_RECEIVED_AURORA_TICKET
	vmessage sText_AuroraTicketUseAtPort
	waitmessage
	waitbuttonpress
	release
	end

AuroraTicket_NoBagSpace:
	vmessage sText_AuroraTicketBagFull
	waitmessage
	waitbuttonpress
	release
	end

AuroraTicket_Obtained:
	vmessage sText_AuroraTicketThankYou
	waitmessage
	waitbuttonpress
	release
	end

sText_AuroraTicketForYou:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございます!\l{PLAYER}さま ですね\pあなたさま あてに\nこの チケットが\lおくられて きました$"

sText_AuroraTicketUseAtPort:
	.string "{JPN}ミナモシティの ふなつきば で\nつかえる ようですね\pぜひ ごりよう してみては\nいかがでしょうか?$"

sText_AuroraTicketThankYou:
	.string "{JPN}ふしぎなおくりもの を ごりよう\nいただき ありがとう ございました$"

sText_AuroraTicketBagFull:
	.string "{JPN}しかし {PLAYER}さま!\nたいせつなものが いっぱい です\pパソコンに あずけてから\nまた きてください$"
