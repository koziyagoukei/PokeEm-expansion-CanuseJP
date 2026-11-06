// Battle Connector: who and what the rooms hold. The rules are in src/battle_connector.c, the numbers in
// include/config/battle_connector.h.

// The rooms a branch rolls, with their weights. A room type appears from branch minBranch on (the first branch is 0).
struct ConnectorRoomWeight
{
    u8 roomType;
    u8 weight;
    u8 minBranch;
};

static const struct ConnectorRoomWeight sConnectorRoomWeights[] =
{
    { CONNECTOR_ROOM_SHADOW,         30, 0 },
    { CONNECTOR_ROOM_STRONG_SHADOW,  12, 1 },
    { CONNECTOR_ROOM_RARE_SHADOW,     5, 2 },
    { CONNECTOR_ROOM_DARK_TRAINER,   18, 0 },
    { CONNECTOR_ROOM_STRONG_TRAINER,  8, 1 },
    { CONNECTOR_ROOM_HEAL,           10, 1 },
    { CONNECTOR_ROOM_CALM,            8, 0 },
    { CONNECTOR_ROOM_ITEM,            8, 0 },
    { CONNECTOR_ROOM_RANDOM,          6, 0 },
};

// What a "random" room turns out to be
static const u8 sConnectorRandomRooms[] =
{
    CONNECTOR_ROOM_SHADOW,
    CONNECTOR_ROOM_STRONG_SHADOW,
    CONNECTOR_ROOM_DARK_TRAINER,
    CONNECTOR_ROOM_HEAL,
    CONNECTOR_ROOM_CALM,
    CONNECTOR_ROOM_ITEM,
};

// Hints: vague on purpose, and several room types share them (like the Battle Pike's)
enum ConnectorHint
{
    CONNECTOR_HINT_POKEMON,
    CONNECTOR_HINT_PEOPLE,
    CONNECTOR_HINT_STRONG,
    CONNECTOR_HINT_QUIET,
    CONNECTOR_HINT_ITEM,
    CONNECTOR_HINT_OMINOUS,
    CONNECTOR_HINT_NOTHING,
    CONNECTOR_HINT_COUNT,
};

static const u8 *const sConnectorHintTexts[CONNECTOR_HINT_COUNT] =
{
    [CONNECTOR_HINT_POKEMON] = COMPOUND_STRING("{JPN}ポケモンの けはいが する"),
    [CONNECTOR_HINT_PEOPLE]  = COMPOUND_STRING("{JPN}ひとの けはいが する"),
    [CONNECTOR_HINT_STRONG]  = COMPOUND_STRING("{JPN}つよい ダークはんのうを かんじる"),
    [CONNECTOR_HINT_QUIET]   = COMPOUND_STRING("{JPN}しずかな けはいが する"),
    [CONNECTOR_HINT_ITEM]    = COMPOUND_STRING("{JPN}なにかが おちている ようだ"),
    [CONNECTOR_HINT_OMINOUS] = COMPOUND_STRING("{JPN}まがまがしい けはいが する"),
    [CONNECTOR_HINT_NOTHING] = COMPOUND_STRING("{JPN}なにも かんじとれない……"),
};

// The hints a room type may give (CONNECTOR_HINT_COUNT ends a list)
static const u8 sConnectorRoomHints[CONNECTOR_ROOM_COUNT][3] =
{
    [CONNECTOR_ROOM_SHADOW]         = { CONNECTOR_HINT_POKEMON, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_STRONG_SHADOW]  = { CONNECTOR_HINT_STRONG, CONNECTOR_HINT_POKEMON, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_RARE_SHADOW]    = { CONNECTOR_HINT_STRONG, CONNECTOR_HINT_OMINOUS, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_DARK_TRAINER]   = { CONNECTOR_HINT_PEOPLE, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_STRONG_TRAINER] = { CONNECTOR_HINT_PEOPLE, CONNECTOR_HINT_OMINOUS, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_HEAL]           = { CONNECTOR_HINT_QUIET, CONNECTOR_HINT_PEOPLE, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_CALM]           = { CONNECTOR_HINT_QUIET, CONNECTOR_HINT_POKEMON, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_ITEM]           = { CONNECTOR_HINT_ITEM, CONNECTOR_HINT_QUIET, CONNECTOR_HINT_COUNT },
    [CONNECTOR_ROOM_RANDOM]         = { CONNECTOR_HINT_NOTHING, CONNECTOR_HINT_COUNT },
};

// After a calm room the next branch tells what is behind each path
static const u8 *const sConnectorPreciseHintTexts[CONNECTOR_ROOM_COUNT] =
{
    [CONNECTOR_ROOM_SHADOW]         = COMPOUND_STRING("{JPN}ダークポケモンが いる"),
    [CONNECTOR_ROOM_STRONG_SHADOW]  = COMPOUND_STRING("{JPN}つよい ダークポケモンが いる"),
    [CONNECTOR_ROOM_RARE_SHADOW]    = COMPOUND_STRING("{JPN}めずらしい ダークポケモンが いる"),
    [CONNECTOR_ROOM_DARK_TRAINER]   = COMPOUND_STRING("{JPN}こころを とざした ひとが いる"),
    [CONNECTOR_ROOM_STRONG_TRAINER] = COMPOUND_STRING("{JPN}てごわい ひとが まっている"),
    [CONNECTOR_ROOM_HEAL]           = COMPOUND_STRING("{JPN}からだを やすめられる"),
    [CONNECTOR_ROOM_CALM]           = COMPOUND_STRING("{JPN}やさしい かおりが する"),
    [CONNECTOR_ROOM_ITEM]           = COMPOUND_STRING("{JPN}どうぐが おちている"),
    [CONNECTOR_ROOM_RANDOM]         = COMPOUND_STRING("{JPN}なにが あるか わからない"),
};

// The Shadow Pokemon of the rooms are not listed here: they are the Battle Frontier pool's Pokemon of generations 1-3
// (src/battle_connector.c, include/config/battle_connector.h), each with one of its sets.

// The mid-boss (with one of its Battle Frontier sets) and the boss. A Pokemon joins the list once the facility has
// been cleared minClears times and its flag (0: none) is set, so later conditions can add more.
struct ConnectorSpeciesEntry
{
    u16 species;
    u8 minClears;
    u16 flag;
};

static const struct ConnectorSpeciesEntry sConnectorMidbossPool[] =
{
    { SPECIES_ARTICUNO,  0, 0 },
    { SPECIES_ZAPDOS,    0, 0 },
    { SPECIES_MOLTRES,   0, 0 },
    { SPECIES_RAIKOU,    0, 0 },
    { SPECIES_ENTEI,     0, 0 },
    { SPECIES_SUICUNE,   0, 0 },
    { SPECIES_REGIROCK,  0, 0 },
    { SPECIES_REGICE,    0, 0 },
    { SPECIES_REGISTEEL, 0, 0 },
    { SPECIES_LATIAS,    1, 0 },
    { SPECIES_LATIOS,    1, 0 },
};

// The boss: restricted legendaries up to generation 3 (their Shadow rows in src/data/shadow_pokemon.h give their
// purification moves)
static const struct ConnectorSpeciesEntry sConnectorBossPool[] =
{
    { SPECIES_MEWTWO,   0, 0 },
    { SPECIES_LUGIA,    0, 0 },
    { SPECIES_HO_OH,    0, 0 },
    { SPECIES_KYOGRE,   0, 0 },
    { SPECIES_GROUDON,  0, 0 },
    { SPECIES_RAYQUAZA, 0, 0 },
    { SPECIES_MEW,      1, 0 },
    { SPECIES_CELEBI,   1, 0 },
    { SPECIES_JIRACHI,  1, 0 },
    { SPECIES_DEOXYS,   1, 0 },
};

// People who rejected others, and their Pokemon with them, out of fear of the Human Pokerus. They are not villains:
// they push the player away. Their trainers are Emerald slots the hack no longer uses (src/data/trainers.party).
struct ConnectorTrainer
{
    u16 trainerId;
    u16 graphicsId;
    const u8 *intro;  // before the battle
    const u8 *defeat; // in battle, when they lose
    const u8 *after;  // after the battle, before the player protects their Pokemon
};

static const struct ConnectorTrainer sConnectorDarkTrainers[] =
{
    {
        .trainerId = TRAINER_CINDY_2,
        .graphicsId = OBJ_EVENT_GFX_HEX_MANIAC,
        .intro = COMPOUND_STRING("{JPN}……ちかづかないで。\pひとに ふれたら うつるって\nずっと いわれてきたの。"),
        .defeat = COMPOUND_STRING("{JPN}……あなたは にげないのね。"),
        .after = COMPOUND_STRING("{JPN}この子だけは ずっと そばに いた。\nでも もう わたしの こえは とどかない。\p……その ポケモン \nあなたに ついていきたがってる。"),
    },
    {
        .trainerId = TRAINER_DUDLEY,
        .graphicsId = OBJ_EVENT_GFX_HIKER,
        .intro = COMPOUND_STRING("{JPN}ほうっておいてくれ。\pここなら だれとも\nあわずに すむんだ。"),
        .defeat = COMPOUND_STRING("{JPN}……まいったよ。"),
        .after = COMPOUND_STRING("{JPN}ひとから はなれたら\nこいつまで こころを とざしちまった。\pおれじゃ もう だめなんだ。\nつれていって やってくれ。"),
    },
    {
        .trainerId = TRAINER_KAYLEE,
        .graphicsId = OBJ_EVENT_GFX_BEAUTY,
        .intro = COMPOUND_STRING("{JPN}もう だれとも\nかかわりたくないの。"),
        .defeat = COMPOUND_STRING("{JPN}……ふしぎ。 いやじゃ なかった。"),
        .after = COMPOUND_STRING("{JPN}ヒトポケルスが こわくて\nこの子の ことまで さけてしまった。\p……あなたなら この子の こころを\nひらいて あげられる？"),
    },
    {
        .trainerId = TRAINER_AMY_AND_LIV_3,
        .graphicsId = OBJ_EVENT_GFX_PSYCHIC_M,
        .intro = COMPOUND_STRING("{JPN}きこえるんだ。 ひとの こころの こえが。\pだから もう だれにも\nあいたくない。"),
        .defeat = COMPOUND_STRING("{JPN}きみの こえは……しずかだね。"),
        .after = COMPOUND_STRING("{JPN}こいつの こえだけは\nきこえなく なってしまった。\p……でも きみには\nなにか つたえようと している。"),
    },
    {
        .trainerId = TRAINER_GINA_AND_MIA_2,
        .graphicsId = OBJ_EVENT_GFX_BLACK_BELT,
        .intro = COMPOUND_STRING("{JPN}ひとりで つよく なると きめたんだ。\nじゃまを しないでくれ！"),
        .defeat = COMPOUND_STRING("{JPN}ひとりでは……かてないのか。"),
        .after = COMPOUND_STRING("{JPN}ひとりで いいと おもっていた。\nでも こいつは ずっと ないていた。\pおれの かわりに\nそばに いてやってくれ。"),
    },
    {
        .trainerId = TRAINER_GRUNT_UNUSED,
        .graphicsId = OBJ_EVENT_GFX_HIKER,
        .intro = COMPOUND_STRING("{JPN}……ひとの こえなんて\nどれくらい ぶりだろうな。\pわるいが かえってくれ。"),
        .defeat = COMPOUND_STRING("{JPN}……そうか。"),
        .after = COMPOUND_STRING("{JPN}ここに こもってから\nこいつは わらわなく なった。\pあんたの ところなら\nまた わらえるかも しれん。"),
    },
    {
        .trainerId = TRAINER_LUCAS_2,
        .graphicsId = OBJ_EVENT_GFX_GENTLEMAN,
        .intro = COMPOUND_STRING("{JPN}しつれい。\nわたしに かまわないで いただきたい。"),
        .defeat = COMPOUND_STRING("{JPN}……おみごと。"),
        .after = COMPOUND_STRING("{JPN}ひとを さけるうちに\nこの子とも ことばを かわさなくなった。\p……この子は あなたを\nえらんだようですな。"),
    },
    {
        .trainerId = TRAINER_MIKE_1,
        .graphicsId = OBJ_EVENT_GFX_MANIAC,
        .intro = COMPOUND_STRING("{JPN}ポケモンさえ いれば\nそれで よかったんだ。\pなのに……くるな！"),
        .defeat = COMPOUND_STRING("{JPN}……どうして。"),
        .after = COMPOUND_STRING("{JPN}ひとを こわがってたら\nこいつまで ぼくを こわがるように……\p……たのむ。 こいつを\nひとりに しないで やってくれ。"),
    },
};

static const struct ConnectorTrainer sConnectorStrongTrainers[] =
{
    {
        .trainerId = TRAINER_TERRY,
        .graphicsId = OBJ_EVENT_GFX_WOMAN_3,
        .intro = COMPOUND_STRING("{JPN}つながりなんて いらない。\pつながった さきから\nこわれていくんだから。"),
        .defeat = COMPOUND_STRING("{JPN}……つながりが こわくないの？"),
        .after = COMPOUND_STRING("{JPN}この子たちも むかしは\nよく わらう子だった。\p……あなたと いるほうが\nきっと いい。"),
    },
    {
        .trainerId = TRAINER_RED,
        .graphicsId = OBJ_EVENT_GFX_EXPERT_M,
        .intro = COMPOUND_STRING("{JPN}わしは もう だれも\nしんじないと きめた。\pここから さきへは いかせんぞ。"),
        .defeat = COMPOUND_STRING("{JPN}……わかい ちからか。"),
        .after = COMPOUND_STRING("{JPN}こいつらまで まきこんで\nしまった。 わしの まけじゃ。\pつれていけ。 こいつらの\nこころの こえを きいてやってくれ。"),
    },
    {
        .trainerId = TRAINER_LEAF,
        .graphicsId = OBJ_EVENT_GFX_EXPERT_F,
        .intro = COMPOUND_STRING("{JPN}ひとに あえば うつる。\nうつれば うしなう。\pだから わたしは ここに いるの。"),
        .defeat = COMPOUND_STRING("{JPN}……あなたは うしなうのが\nこわくないのね。"),
        .after = COMPOUND_STRING("{JPN}この子たちの ほうが さきに\nわたしを ゆるして くれていたのかも。\p……この子たちを おねがい。"),
    },
};

// What an item room may hold
static const u16 sConnectorRoomItems[] =
{
    ITEM_SUPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_HEAL, ITEM_REVIVE, ITEM_ETHER,
    ITEM_SITRUS_BERRY, ITEM_LUM_BERRY, ITEM_LEPPA_BERRY, ITEM_PP_UP,
};
