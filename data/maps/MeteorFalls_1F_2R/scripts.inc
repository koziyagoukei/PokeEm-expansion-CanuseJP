MeteorFalls_1F_2R_MapScripts::
	.byte 0

MeteorFalls_1F_2R_EventScript_Nicolas::
	trainerbattle_single TRAINER_NICOLAS_1, MeteorFalls_1F_2R_Text_NicolasIntro, MeteorFalls_1F_2R_Text_NicolasDefeat, MeteorFalls_1F_2R_EventScript_RegisterNicolas
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchNicolas
	msgbox MeteorFalls_1F_2R_Text_NicolasPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterNicolas::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox MeteorFalls_1F_2R_Text_NicolasRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_NICOLAS_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchNicolas::
	trainerbattle_rematch TRAINER_NICOLAS_1, MeteorFalls_1F_2R_Text_NicolasRematchIntro, MeteorFalls_1F_2R_Text_NicolasRematchDefeat
	msgbox MeteorFalls_1F_2R_Text_NicolasPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_EventScript_John::
	trainerbattle_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JohnIntro, MeteorFalls_1F_2R_Text_JohnDefeat, MeteorFalls_1F_2R_Text_JohnNotEnoughMons, MeteorFalls_1F_2R_EventScript_RegisterJohn
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchJohn
	msgbox MeteorFalls_1F_2R_Text_JohnPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterJohn::
	msgbox MeteorFalls_1F_2R_Text_JohnRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_JOHN_AND_JAY_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchJohn::
	trainerbattle_rematch_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JohnRematchIntro, MeteorFalls_1F_2R_Text_JohnRematchDefeat, MeteorFalls_1F_2R_Text_JohnRematchNotEnoughMons
	msgbox MeteorFalls_1F_2R_Text_JohnPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_EventScript_Jay::
	trainerbattle_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JayIntro, MeteorFalls_1F_2R_Text_JayDefeat, MeteorFalls_1F_2R_Text_JayNotEnoughMons, MeteorFalls_1F_2R_EventScript_RegisterJay
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, MeteorFalls_1F_2R_EventScript_RematchJay
	msgbox MeteorFalls_1F_2R_Text_JayPostBattle, MSGBOX_DEFAULT
	release
	end

MeteorFalls_1F_2R_EventScript_RegisterJay::
	msgbox MeteorFalls_1F_2R_Text_JohnRegister, MSGBOX_DEFAULT  @ John speaks for both during register
	register_matchcall TRAINER_JOHN_AND_JAY_1
	release
	end

MeteorFalls_1F_2R_EventScript_RematchJay::
	trainerbattle_rematch_double TRAINER_JOHN_AND_JAY_1, MeteorFalls_1F_2R_Text_JayRematchIntro, MeteorFalls_1F_2R_Text_JayRematchDefeat, MeteorFalls_1F_2R_Text_JayRematchNotEnoughMons
	msgbox MeteorFalls_1F_2R_Text_JayPostRematch, MSGBOX_AUTOCLOSE
	end

MeteorFalls_1F_2R_Text_NicolasIntro:
	.string "ここは オレたち\nドラゴンつかいの しゅぎょうの ばしょ!\pチャンピオンも くる ばしょ といったら\nその すごさ わかるだろうか!?"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_NicolasDefeat:
	.string "くっ!\nこれほど つよいとは……!"
	.string "$"

MeteorFalls_1F_2R_Text_NicolasPostBattle:
	.string "まだまだ みちは ながく けわしい\pオレと オレの ポケモンが\nいちばんに なれるのは いつだろう……"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_NicolasRegister:
	.string "きみの つよさを もっと しりたい\nポケナビに とうろく させてくれ!"
	.string "$"

MeteorFalls_1F_2R_Text_NicolasRematchIntro:
	.string "あれから いちばん めざして\nかなり じぶんたちを きたえた……\pどれほど つよくなったか\nきみで たしかめさせて もらおうッ!"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_NicolasRematchDefeat:
	.string "くっ!\nこれほど つよいとは……!"
	.string "$"

MeteorFalls_1F_2R_Text_NicolasPostRematch:
	.string "あいかわらず ポケモンを きたえてるな\pきみが その つよさで あるかぎり\nオレも つよくなれるよ!"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnIntro:
	.string "ゲン『わしらは いつも ふうふで\nポケモンを しておるからのう\lしょうぶにも じしんが あるんじゃよ"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnDefeat:
	.string "ゲン『おやおや\nまけたよ ばあさん"
	.string "$"

MeteorFalls_1F_2R_Text_JohnPostBattle:
	.string "ゲン『けっこんして 50ねん……\nそういや いちども ばあさんに\lポケモンしょうぶで かってないのう"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnNotEnoughMons:
	.string "ゲン『おやおや\nわかい トレーナー じゃな!\pわしらと しょうぶ してくれるのなら\nポケモンを もっと つれてきなされ"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnRegister:
	.string "ゲン『わかい トレーナーさん……\nまた きかいが あれば\lわしらと たたかって くれんかのう"
	.string "$"

MeteorFalls_1F_2R_Text_JayIntro:
	.string "ハツ『けっこんして 50ねん……\nわたしら ふうふの きずなは\lどんな ものでも きれませんよ"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JayDefeat:
	.string "ハツ『おやおや\nまけましたよ じいさん"
	.string "$"

MeteorFalls_1F_2R_Text_JayPostBattle:
	.string "ハツ『けっこんして 50ねん……\nふうふげんかに なるたび\lポケモンしょうぶを していましたねえ"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JayNotEnoughMons:
	.string "ハツ『おやおや\nわかい トレーナーさん ですねえ\pわしらと しょうぶ してくれるのなら\nポケモンを もっと つれておいで"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnRematchIntro:
	.string "ゲン『わしらは いつも ふうふで\nポケモンを たのしんでおる からのう\lしょうぶにも じしんが あるんじゃよ"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnRematchDefeat:
	.string "ゲン『おやおや\nまけたよ ばあさん"
	.string "$"

MeteorFalls_1F_2R_Text_JohnPostRematch:
	.string "ゲン『けっこんして 50ねん……\nそういや まいにち まいにち\lばあさんと しょうぶ していたのう"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JohnRematchNotEnoughMons:
	.string "ゲン『おやおや\nわかい トレーナー じゃな!\pわしらと しょうぶ してくれるのなら\nポケモンを もっと つれてきなされ"
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JayRematchIntro:
	.string "ハツ『けっこんして 50ねん……\nおたがいを ささえあってきた\lわたしら ふうふは つよいですよ"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JayRematchDefeat:
	.string "ハツ『おやおや\nまけましたよ じいさん"
	.string "$"

MeteorFalls_1F_2R_Text_JayPostRematch:
	.string "ハツ『けっこんして 50ねん……\nいろんなことが ありましたねえ\pこれからも じいさんと いっしょに\nたのしいことが つづくと いいですわねえ"
	.string ""
	.string ""
	.string "$"

MeteorFalls_1F_2R_Text_JayRematchNotEnoughMons:
	.string "ハツ『おやおや\nわかい トレーナーさん ですねえ\pわしらと しょうぶ してくれるのなら\nポケモンを もっと つれておいで"
	.string ""
	.string ""
	.string "$"
