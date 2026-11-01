static const struct BathVisit sBathVisits[BATH_VISIT_SCENE_COUNT] =
{
    [BATH_VISIT_STONE_STUDY] = {
        .area = BATH_VISITOR_AREA_ROCK, .count = 2,
        .actors = {
            {TRAINER_FRONTIER_STEVEN, 0, 5, 10, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_ROXANNE, 0, 10, 10, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}ダイゴ『この石の模様\n眺めていると 飽きないね。』\pツツジ『同じ石でも 温まる速さが\n違いますわ。記録しておきます。』\pダイゴ『……あれ。\n休みに 来たんじゃなかったかな。』"),
    },
    [BATH_VISIT_ELITE_REST] = {
        .area = BATH_VISITOR_AREA_ROCK, .count = 2,
        .actors = {
            {TRAINER_FRONTIER_GLACIA, 0, 5, 16, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_PHOEBE, 0, 10, 16, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}プリム『冷たい場所ばかりでは\n身体が 固まってしまいますわ。』\pフヨウ『あたいも ぽかぽか！\n……隣の席 まだ空いてるね。』\pプリム『……誰か いらして？』\pフヨウ『気にしないで。\nみんな 気持ちよさそうだから。』"),
    },
    [BATH_VISIT_PAPAS] = {
        .area = BATH_VISITOR_AREA_SAUNA, .count = 2,
        .actors = {
            {0, OBJ_EVENT_GFX_NORMAN, 7, 11, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_NORMAN, 0, 9, 11, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}パパ『取っておいた きのみが\n全部 ポロックに なっていてね。』\pセンリ『うちもだ。\n選び抜いた 実だったんだが……。』\pパパ『文句を言うつもりだったが\nポケモン達が 嬉しそうでな。』\pセンリ『……それを見たら\n何も 言えなくなるよな。』"),
    },
    [BATH_VISIT_LEADERS] = {
        .area = BATH_VISITOR_AREA_SAUNA, .count = 2,
        .actors = {
            {TRAINER_FRONTIER_MAXIE, 0, 9, 12, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_ARCHIE, 0, 14, 12, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}マツブサ『設定温度は そのままだ。\n勝手に 上げるなよ。』\pアオギリ『おれぁ 触ってねえぞ！\nおまえが 下げたんじゃねえのか？』\pマツブサ『……今日は この辺りで\n引き分けに しておこう。』\pアオギリ『おう。水を飲みに 行くか！』"),
    },
    [BATH_VISIT_SAUNA_GYM] = {
        .area = BATH_VISITOR_AREA_SAUNA, .count = 2,
        .actors = {
            {TRAINER_FRONTIER_FLANNERY, 0, 15, 14, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_WATTSON, 0, 18, 14, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}アスナ『うう……ジムの口上より\nこっちの方が 緊張するかも。』\pテッセン『わっはっは！\nここでは 強がらんで ええぞ！』\pアスナ『それじゃ 先に休憩！\nおじいちゃんも 一緒に来てね！』"),
    },
    [BATH_VISIT_PECHARUNT] = {
        .area = BATH_VISITOR_AREA_WATERFALL, .count = 2,
        .flags = {FLAG_STORY_DARKRAI_JOINED, FLAG_STORY_CURE_FOUND, FLAG_STORY_POKEVIRUS_FADED},
        .actors = {
            {0, OBJ_EVENT_GFX_SPECIES(PECHARUNT), 11, 9, MOVEMENT_TYPE_FACE_UP},
            {0, OBJ_EVENT_GFX_SPECIES(DARKRAI), 15, 10, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}モモワロウは 滝に打たれながら\n小さな身体を じっと固めている。\pダークライ『……修行のつもりらしい。\nあいつなりに 考えたんだろう。』\pモモワロウが 流されかけると\nダークライの影が そっと支えた。\pダークライ『……もう少し いるそうだ。\n冷えきる前には 連れて帰るよ。』"),
    },
    [BATH_VISIT_MOM] = {
        .area = BATH_VISITOR_AREA_WATERFALL, .count = 1,
        .actors = {{0, OBJ_EVENT_GFX_MOM, 8, 12, MOVEMENT_TYPE_FACE_DOWN}},
        .text = COMPOUND_STRING("{JPN}ママ『あら {PLAYER}！\n湯加減が とってもいいの。』\pママ『滝の音を 聞いていると\nつい うとうとしちゃって。』\pママ『帰ったら また\n一緒に ポロックを作りましょ！』"),
    },
    [BATH_VISIT_WATER_MASTERS] = {
        .area = BATH_VISITOR_AREA_WATERFALL, .count = 2,
        .actors = {
            {TRAINER_FRONTIER_JUAN, 0, 7, 13, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_WALLACE, 0, 16, 13, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}アダン『滝の音にも 間がある。\nよい 踊りのようですね。』\pミクリ『では 私達も 静かに\nこの湯を 楽しむとしよう。』\p二人は しばらく話すのをやめ\n流れ落ちる湯を 眺めている。"),
    },
    [BATH_VISIT_SURFERS] = {
        .area = BATH_VISITOR_AREA_DEEP, .count = 2,
        .actors = {
            {TRAINER_FRONTIER_BRAWLY, 0, 6, 16, MOVEMENT_TYPE_FACE_RIGHT},
            {TRAINER_FRONTIER_ARCHIE, 0, 9, 16, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}トウキ『波が 来ないな……。』\pアオギリ『風呂だからな！\n今日は のんびり 浮いてろ！』\pトウキ『ああ。そういう日も\nたまには 悪くないな！』"),
    },
    [BATH_VISIT_COOK] = {
        .area = BATH_VISITOR_AREA_UNDERWATER, .count = 1,
        .actors = {{0, OBJ_EVENT_GFX_ROCKET_COOK, 10, 10, MOVEMENT_TYPE_FACE_RIGHT}},
        .text = COMPOUND_STRING("{JPN}店主『おう。温泉卵 仕込んでんだ。\nこの辺りの 湯加減がよくてな。』\p店主『……おい。\nモンスターボールに 入れるなよ。』\p店主『ポケモンの タマゴじゃねえ。\n晩飯の 追加の一品だ。』"),
    },
    [BATH_VISIT_NOBUO] = {
        .area = BATH_VISITOR_AREA_ROCK, .count = 2,
        .target = STORY_TARGET_NOBUO + 1,
        .actors = {
            {0, OBJ_EVENT_GFX_HIKER, 10, 16, MOVEMENT_TYPE_FACE_RIGHT},
            {0, OBJ_EVENT_GFX_SPECIES(SANDSLASH), 17, 16, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}ノブオ『今日は 道を聞く用もない。\nこいつと 二人で 休むだけだ。』\pサンドパンは 温まった石を\n短い爪で かりかりと撫でている。\pノブオ『……掘るなよ。\n底を抜いたら 風呂が冷める。』"),
    },
    [BATH_VISIT_YORIKO] = {
        .area = BATH_VISITOR_AREA_WATERFALL, .count = 2,
        .target = STORY_TARGET_YORIKO + 1,
        .actors = {
            {0, OBJ_EVENT_GFX_BEAUTY, 8, 14, MOVEMENT_TYPE_FACE_RIGHT},
            {0, OBJ_EVENT_GFX_SPECIES(SEVIPER), 15, 14, MOVEMENT_TYPE_FACE_LEFT},
        },
        .text = COMPOUND_STRING("{JPN}ヨリコ『ハブネークの 鱗が\n湯を弾くところ 綺麗でしょう？』\pハブネークは 尻尾の先だけを\n湯に浸けて くつろいでいる。\pヨリコ『今は 演技じゃないの。\nこの子の 好きなようにさせてるわ。』"),
    },
};

static const struct BathRestSpot sBathRestSpots[BATH_VISITOR_AREA_COUNT] = {
    [BATH_VISITOR_AREA_ROCK] = {5, 10, MOVEMENT_TYPE_FACE_DOWN},
    [BATH_VISITOR_AREA_WATERFALL] = {8, 12, MOVEMENT_TYPE_FACE_DOWN},
    [BATH_VISITOR_AREA_SAUNA] = {8, 11, MOVEMENT_TYPE_FACE_DOWN},
    [BATH_VISITOR_AREA_DEEP] = {8, 16, MOVEMENT_TYPE_FACE_UP},
    [BATH_VISITOR_AREA_UNDERWATER] = {10, 10, MOVEMENT_TYPE_FACE_RIGHT},
};

static const u8 *const sBathSoloTexts[BATH_VISITOR_AREA_COUNT] = {
    [BATH_VISITOR_AREA_ROCK] = COMPOUND_STRING("{JPN}{STR_VAR_1}は 温まった岩盤のそばで\n静かに 身体を休めている。\p時々 目を閉じて\nゆっくりと 深呼吸している。"),
    [BATH_VISITOR_AREA_WATERFALL] = COMPOUND_STRING("{JPN}{STR_VAR_1}は 流れ落ちる湯を\nぼんやりと 眺めている。\p話しかけると\n気持ちよさそうに 頷いた。"),
    [BATH_VISITOR_AREA_SAUNA] = COMPOUND_STRING("{JPN}{STR_VAR_1}は 汗を拭きながら\nのんびり 休憩している。\p勝負の時とは違う\n穏やかな 表情をしている。"),
    [BATH_VISITOR_AREA_DEEP] = COMPOUND_STRING("{JPN}{STR_VAR_1}は 肩まで湯に浸かり\n気持ちよさそうに 息をついた。\p今日は 急がなくていい。\nそんな 顔をしている。"),
};
