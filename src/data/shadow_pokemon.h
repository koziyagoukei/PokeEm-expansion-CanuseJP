// Shadow Pokemon by species (one row each): Shadow moves, full heart gauge and aggression.
// A species without a row (and whose base species has none) uses the defaults in src/shadow_pokemon.c:
// Shadow Rush (plus Shadow Fire / Shadow Bolt / Shadow Chill for a Fire, Electric or Ice type), a gauge of
// SHADOW_DEFAULT_HEART_GAUGE and aggression SHADOW_DEFAULT_AGGRESSION.
//
// - moves: 1-4 Shadow moves in their slot order. All MOVE_NONE = the default moves.
// - heartGauge: the full heart gauge, 1-16382 (0 = the default). It goes down by the per-nature amounts of
//   Pokemon XD for each battle, every 256 steps and each Call out of Hyper / Reverse Mode.
// - aggression: 1 (very high) .. 5 (very low) (0 = the default). With the bars cleared it sets the chance of Reverse
//   Mode (Pokemon XD).
// - purifyMoves: a Shadow Pokemon of the Battle Connector learns one of these (picked by its personality) when it is
//   purified: moves the species cannot learn otherwise. The bosses' are set; any row can have them.
//
// The gauges and aggressions below are Pokemon XD's own (Japanese release, XD's Shadow Pokemon list); where a species
// appears twice in XD the first one is used. Their Shadow moves are not listed here, so they use the defaults.
static const struct ShadowPokemonInfo sShadowPokemonInfo[] =
{
#if TESTING
    // Only in the test build (test/battle/shadow_pokemon.c), to reach every Shadow move in battle. They come first so
    // they win over a row of the same species below.
    { .species = SPECIES_GEODUDE, .moves = { MOVE_SHADOW_END } },
    { .species = SPECIES_SNORLAX, .moves = { MOVE_SHADOW_HALF, MOVE_SHADOW_SHED, MOVE_SHADOW_SKY, MOVE_SHADOW_WAVE } },
    { .species = SPECIES_MISDREAVUS, .moves = { MOVE_SHADOW_HOLD, MOVE_SHADOW_MIST, MOVE_SHADOW_PANIC, MOVE_SHADOW_DOWN } },
#endif
    { .species = SPECIES_TEDDIURSA,     .heartGauge =  3000, .aggression = 2 }, // XD #1 ヒメグマ
    { .species = SPECIES_VULPIX,        .heartGauge =  2000, .aggression = 4 }, // XD #2 ロコン
    { .species = SPECIES_SPHEAL,        .heartGauge =  1500, .aggression = 5 }, // XD #3 タマザラシ
    { .species = SPECIES_BALTOY,        .heartGauge =  1500, .aggression = 5 }, // XD #4 ヤジロン
    { .species = SPECIES_MAREEP,        .heartGauge =  1500, .aggression = 5 }, // XD #5 メリープ
    { .species = SPECIES_GULPIN,        .heartGauge =  1500, .aggression = 5 }, // XD #6 ゴクリン
    { .species = SPECIES_SEEDOT,        .heartGauge =  1500, .aggression = 4 }, // XD #7 タネボー
    { .species = SPECIES_SPINARAK,      .heartGauge =  1500, .aggression = 5 }, // XD #8 イトマル
    { .species = SPECIES_NUMEL,         .heartGauge =  1500, .aggression = 4 }, // XD #9 ドンメル
    { .species = SPECIES_CARVANHA,      .heartGauge =  1700, .aggression = 3 }, // XD #10 キバニア
    { .species = SPECIES_ROSELIA,       .heartGauge =  3000, .aggression = 4 }, // XD #11 ロゼリア
    { .species = SPECIES_DELCATTY,      .heartGauge =  2500, .aggression = 3 }, // XD #12 エネコロロ
    { .species = SPECIES_NOSEPASS,      .heartGauge =  4000, .aggression = 3 }, // XD #13 ノズパス
    { .species = SPECIES_HOUNDOUR,      .heartGauge =  1500, .aggression = 4 }, // XD #14 デルビル
    { .species = SPECIES_MAKUHITA,      .heartGauge =  2000, .aggression = 3 }, // XD #15 マクノシタ
    { .species = SPECIES_DUSKULL,       .heartGauge =  2200, .aggression = 3 }, // XD #16 ヨマワル
    { .species = SPECIES_RALTS,         .heartGauge =  2200, .aggression = 2 }, // XD #17 ラルトス
    { .species = SPECIES_MAWILE,        .heartGauge =  2500, .aggression = 4 }, // XD #18 クチート
    { .species = SPECIES_SNORUNT,       .heartGauge =  2500, .aggression = 4 }, // XD #19 ユキワラシ
    { .species = SPECIES_PINECO,        .heartGauge =  2500, .aggression = 3 }, // XD #20 クヌギダマ
    { .species = SPECIES_SWINUB,        .heartGauge =  2500, .aggression = 5 }, // XD #21 ウリムー
    { .species = SPECIES_NATU,          .heartGauge =  2500, .aggression = 4 }, // XD #22 ネイティ
    { .species = SPECIES_SHROOMISH,     .heartGauge =  1800, .aggression = 4 }, // XD #23 キノココ
    { .species = SPECIES_MEOWTH,        .heartGauge =  3500, .aggression = 3 }, // XD #24 ニャース
    { .species = SPECIES_SPEAROW,       .heartGauge =  4500, .aggression = 2 }, // XD #25 オニスズメ
    { .species = SPECIES_GRIMER,        .heartGauge =  3000, .aggression = 3 }, // XD #26 ベトベター
    { .species = SPECIES_SEEL,          .heartGauge =  3500, .aggression = 4 }, // XD #27 パウワウ
    { .species = SPECIES_LUNATONE,      .heartGauge =  5000, .aggression = 2 }, // XD #28 ルナトーン
    { .species = SPECIES_VOLTORB,       .heartGauge =  2500, .aggression = 2 }, // XD #29 ビリリダマ
    { .species = SPECIES_ZANGOOSE,      .heartGauge =  5000, .aggression = 1 }, // XD #30 ザングース
    { .species = SPECIES_GROWLITHE,     .heartGauge =  4000, .aggression = 3 }, // XD #31 ガーディ
    { .species = SPECIES_PARAS,         .heartGauge =  4000, .aggression = 3 }, // XD #32 パラス
    { .species = SPECIES_SHELLDER,      .heartGauge =  4000, .aggression = 3 }, // XD #33 シェルダー
    { .species = SPECIES_BEEDRILL,      .heartGauge =  4500, .aggression = 1 }, // XD #34 スピアー
    { .species = SPECIES_PIDGEOTTO,     .heartGauge =  4000, .aggression = 2 }, // XD #35 ピジョン
    { .species = SPECIES_BUTTERFREE,    .heartGauge =  4000, .aggression = 4 }, // XD #36 バタフリー
    { .species = SPECIES_TANGELA,       .heartGauge =  4000, .aggression = 3 }, // XD #37 モンジャラ
    { .species = SPECIES_RATICATE,      .heartGauge =  6000, .aggression = 3 }, // XD #38 ラッタ
    { .species = SPECIES_VENOMOTH,      .heartGauge =  4000, .aggression = 4 }, // XD #39 モルフォン
    { .species = SPECIES_WEEPINBELL,    .heartGauge =  4000, .aggression = 5 }, // XD #40 ウツドン
    { .species = SPECIES_ARBOK,         .heartGauge =  5000, .aggression = 1 }, // XD #41 アーボック
    { .species = SPECIES_PRIMEAPE,      .heartGauge =  6000, .aggression = 1 }, // XD #42 オコリザル
    { .species = SPECIES_HYPNO,         .heartGauge =  5500, .aggression = 2 }, // XD #43 スリーパー
    { .species = SPECIES_GOLDUCK,       .heartGauge =  6500, .aggression = 2 }, // XD #44 ゴルダック
    { .species = SPECIES_SABLEYE,       .heartGauge =  7000, .aggression = 2 }, // XD #45 ヤミラミ
    { .species = SPECIES_MAGNETON,      .heartGauge =  4500, .aggression = 3 }, // XD #46 レアコイル
    { .species = SPECIES_DODRIO,        .heartGauge =  8000, .aggression = 1 }, // XD #47 ドードリオ
    { .species = SPECIES_FARFETCHD,     .heartGauge =  5500, .aggression = 4 }, // XD #48 カモネギ
    { .species = SPECIES_ALTARIA,       .heartGauge =  6500, .aggression = 3 }, // XD #49 チルタリス
    { .species = SPECIES_KANGASKHAN,    .heartGauge =  6000, .aggression = 3 }, // XD #50 ガルーラ
    { .species = SPECIES_BANETTE,       .heartGauge =  7000, .aggression = 2 }, // XD #51 ジュペッタ
    { .species = SPECIES_MAGMAR,        .heartGauge =  7000, .aggression = 2 }, // XD #52 ブーバー
    { .species = SPECIES_PINSIR,        .heartGauge =  7000, .aggression = 2 }, // XD #53 カイロス
    { .species = SPECIES_MAGCARGO,      .heartGauge =  5500, .aggression = 4 }, // XD #54 マグカルゴ
    { .species = SPECIES_RAPIDASH,      .heartGauge =  6000, .aggression = 3 }, // XD #55 ギャロップ
    { .species = SPECIES_HITMONCHAN,    .heartGauge =  6000, .aggression = 3 }, // XD #56 エビワラー
    { .species = SPECIES_HITMONLEE,     .heartGauge =  7000, .aggression = 2 }, // XD #57 サワムラー
    { .species = SPECIES_LICKITUNG,     .heartGauge =  5000, .aggression = 4 }, // XD #58 ベロリンガ
    { .species = SPECIES_SCYTHER,       .heartGauge =  8000, .aggression = 1 }, // XD #59 ストライク
    { .species = SPECIES_CHANSEY,       .heartGauge =  4000, .aggression = 5 }, // XD #60 ラッキー
    { .species = SPECIES_SOLROCK,       .heartGauge =  7500, .aggression = 2 }, // XD #61 ソルロック
    { .species = SPECIES_STARMIE,       .heartGauge =  7500, .aggression = 2 }, // XD #62 スターミー
    { .species = SPECIES_ELECTABUZZ,    .heartGauge =  7000, .aggression = 2 }, // XD #63 エレブー
    { .species = SPECIES_SWELLOW,       .heartGauge =  7000, .aggression = 3 }, // XD #64 オオスバメ
    { .species = SPECIES_SNORLAX,       .heartGauge =  9000, .aggression = 1 }, // XD #65 カビゴン
    { .species = SPECIES_POLIWRATH,     .heartGauge =  7500, .aggression = 2 }, // XD #66 ニョロボン
    { .species = SPECIES_MR_MIME,       .heartGauge =  6500, .aggression = 3 }, // XD #67 バリヤード
    { .species = SPECIES_DUGTRIO,       .heartGauge =  5000, .aggression = 4 }, // XD #68 ダグトリオ
    { .species = SPECIES_MANECTRIC,     .heartGauge =  7000, .aggression = 3 }, // XD #69 ライボルト
    { .species = SPECIES_SALAMENCE,     .heartGauge =  9000, .aggression = 1 }, // XD #70 ボーマンダ
    { .species = SPECIES_MAROWAK,       .heartGauge =  6500, .aggression = 3 }, // XD #71 ガラガラ
    { .species = SPECIES_LAPRAS,        .heartGauge =  6000, .aggression = 4 }, // XD #72 ラプラス
    { .species = SPECIES_LUGIA,         .heartGauge = 12000, .aggression = 1,
      .purifyMoves = { MOVE_ESPER_WING, MOVE_PSYCHO_BOOST } }, // XD #73 ルギア
    { .species = SPECIES_ZAPDOS,        .heartGauge = 10000, .aggression = 1 }, // XD #74 サンダー
    { .species = SPECIES_MOLTRES,       .heartGauge = 10000, .aggression = 1 }, // XD #75 ファイヤー
    { .species = SPECIES_ARTICUNO,      .heartGauge = 10000, .aggression = 1 }, // XD #76 フリーザー
    { .species = SPECIES_TAUROS,        .heartGauge =  9000, .aggression = 1 }, // XD #77 ケンタロス
    { .species = SPECIES_RHYDON,        .heartGauge =  7000, .aggression = 3 }, // XD #78 サイドン
    { .species = SPECIES_EXEGGUTOR,     .heartGauge =  9000, .aggression = 1 }, // XD #79 ナッシー
    { .species = SPECIES_DRAGONITE,     .heartGauge =  9000, .aggression = 3 }, // XD #80 カイリュー
    { .species = SPECIES_TOGEPI,        .heartGauge =  4500, .aggression = 5 }, // XD #81 トゲピー
    { .species = SPECIES_POOCHYENA,     .heartGauge =  2500, .aggression = 2 }, // XD #82 ポチエナ
    { .species = SPECIES_LEDYBA,        .heartGauge =  2500, .aggression = 2 }, // XD #83 レディバ

    // The Battle Connector's bosses (Lugia is above). Not Shadow Pokemon in XD: their gauges and aggressions are this
    // game's own, in line with XD's Lugia.
    { .species = SPECIES_MEWTWO,   .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_RAGE_FIST, MOVE_FIERY_WRATH } },
    { .species = SPECIES_MEW,      .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_STRANGE_STEAM, MOVE_PSYSTRIKE } },
    { .species = SPECIES_HO_OH,    .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_HEART_SWAP, MOVE_SEARING_SHOT } },
    { .species = SPECIES_CELEBI,   .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_JUNGLE_HEALING, MOVE_FORESTS_CURSE } },
    { .species = SPECIES_KYOGRE,   .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_STEAM_ERUPTION, MOVE_FREEZE_DRY } },
    { .species = SPECIES_GROUDON,  .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_ROCK_WRECKER, MOVE_MAGMA_STORM } },
    { .species = SPECIES_RAYQUAZA, .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_NO_RETREAT, MOVE_STORM_THROW } },
    { .species = SPECIES_JIRACHI,  .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_LIGHT_OF_RUIN, MOVE_FAIRY_LOCK } },
    { .species = SPECIES_DEOXYS,   .heartGauge = 10000, .aggression = 1, .purifyMoves = { MOVE_LUMINA_CRASH, MOVE_PHOTON_GEYSER } },

    { .species = SPECIES_DIALGA,              .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_DOOM_DESIRE, MOVE_TACHYON_CUTTER } },
    { .species = SPECIES_PALKIA,              .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_FICKLE_BEAM, MOVE_SNIPE_SHOT } },
    { .species = SPECIES_GIRATINA_ALTERED,    .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_SPECTRAL_THIEF, MOVE_INFERNAL_PARADE } },
    { .species = SPECIES_PHIONE,              .heartGauge =  8000, .aggression = 4, .purifyMoves = { MOVE_BOUNCY_BUBBLE, MOVE_AQUA_STEP } },
    { .species = SPECIES_MANAPHY,             .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_SPARKLING_ARIA, MOVE_LUNAR_BLESSING } },
    { .species = SPECIES_DARKRAI,             .heartGauge = 10000, .aggression = 1, .purifyMoves = { MOVE_NIGHT_DAZE, MOVE_BITTER_MALICE } },
    { .species = SPECIES_SHAYMIN_LAND,        .heartGauge =  8000, .aggression = 4, .purifyMoves = { MOVE_POLLEN_PUFF, MOVE_FLOWER_TRICK } },
    { .species = SPECIES_ARCEUS_NORMAL,       .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_DOOM_DESIRE, MOVE_SPARKLY_SWIRL } },
    { .species = SPECIES_VICTINI,             .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_VICTORY_DANCE, MOVE_TORCH_SONG } },
    { .species = SPECIES_RESHIRAM,            .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_ARMOR_CANNON, MOVE_BURNING_BULWARK } },
    { .species = SPECIES_ZEKROM,              .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_PLASMA_FISTS, MOVE_OVERDRIVE } },
    { .species = SPECIES_KYUREM,              .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_MOUNTAIN_GALE, MOVE_FREEZING_GLARE } },
    { .species = SPECIES_KELDEO_ORDINARY,     .heartGauge =  8000, .aggression = 2, .purifyMoves = { MOVE_SNIPE_SHOT, MOVE_AQUA_STEP } },
    { .species = SPECIES_MELOETTA_ARIA,       .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_TORCH_SONG, MOVE_SPARKLING_ARIA } },
    { .species = SPECIES_GENESECT,            .heartGauge = 10000, .aggression = 1, .purifyMoves = { MOVE_TACHYON_CUTTER, MOVE_MIGHTY_CLEAVE } },
    { .species = SPECIES_XERNEAS_NEUTRAL,     .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_LUNAR_BLESSING, MOVE_FLEUR_CANNON } },
    { .species = SPECIES_YVELTAL,             .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_BITTER_MALICE, MOVE_SPECTRAL_THIEF } },
    { .species = SPECIES_ZYGARDE_50,          .heartGauge = 12000, .aggression = 1, .purifyMoves = { MOVE_DRAGON_HAMMER, MOVE_LIFE_DEW } },
    { .species = SPECIES_DIANCIE,             .heartGauge =  8000, .aggression = 3, .purifyMoves = { MOVE_FLEUR_CANNON, MOVE_SALT_CURE } },
    { .species = SPECIES_HOOPA_CONFINED,      .heartGauge = 10000, .aggression = 1, .purifyMoves = { MOVE_TWIN_BEAM, MOVE_EERIE_SPELL } },
    { .species = SPECIES_VOLCANION,           .heartGauge = 10000, .aggression = 1, .purifyMoves = { MOVE_HYDRO_STEAM, MOVE_BURNING_BULWARK } },
};
