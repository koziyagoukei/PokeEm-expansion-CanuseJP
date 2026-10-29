// Who meets where once they are all cured (NAMED_TALK_*). Each group stands on its square,
// hidden by its own temporary flag there, until it has talked; the lines are in the scripts.

#define GUEST_ID(name)    (TRAINER_FRONTIER_##name - TRAINER_FRONTIER_GUEST_FIRST)
#define BRAIN_ID(name)    (NAMED_STORY_BRAIN_FIRST + FRONTIER_FACILITY_##name)

struct NamedTalk
{
    u16 map;
    u16 hideFlag;
    u8 members[NAMED_TALK_MAX_MEMBERS];
    u8 count;
};

#define NAMED_TALK(_map, _flag, ...)                                   \
{                                                                      \
    .map = MAP_BATTLE_FRONTIER_##_map,                                 \
    .hideFlag = _flag,                                                 \
    .members = {__VA_ARGS__},                                          \
    .count = ARRAY_COUNT(((const u8[]){__VA_ARGS__})),                 \
}

static const struct NamedTalk sNamedTalks[NAMED_TALK_COUNT] =
{
    [NAMED_TALK_MAGMA_AQUA_LEADERS] = NAMED_TALK(OUTSIDE_WEST, FLAG_TEMP_11, GUEST_ID(MAXIE), GUEST_ID(ARCHIE)),
    [NAMED_TALK_MAGMA_ADMINS]       = NAMED_TALK(OUTSIDE_WEST, FLAG_TEMP_12, GUEST_ID(TABITHA), GUEST_ID(COURTNEY)),
    [NAMED_TALK_AQUA_ADMINS]        = NAMED_TALK(OUTSIDE_EAST, FLAG_TEMP_11, GUEST_ID(MATT), GUEST_ID(SHELLY)),
    [NAMED_TALK_STEVEN_WALLACE]     = NAMED_TALK(OUTSIDE_EAST, FLAG_TEMP_12, GUEST_ID(STEVEN), GUEST_ID(WALLACE)),
    [NAMED_TALK_JUAN_WALLACE]       = NAMED_TALK(OUTSIDE_WEST, FLAG_TEMP_13, GUEST_ID(JUAN), GUEST_ID(WALLACE)),
    [NAMED_TALK_NORMAN_WALLY]       = NAMED_TALK(OUTSIDE_WEST, FLAG_TEMP_14, GUEST_ID(NORMAN), GUEST_ID(WALLY)),
    [NAMED_TALK_WINSTRATES]         = NAMED_TALK(OUTSIDE_EAST, FLAG_TEMP_13, GUEST_ID(VICTOR), GUEST_ID(VICTORIA),
                                                 GUEST_ID(VIVI), GUEST_ID(VICKY), GUEST_ID(VITO)),
    [NAMED_TALK_ELITE_FOUR]         = NAMED_TALK(OUTSIDE_EAST, FLAG_TEMP_14, GUEST_ID(SIDNEY), GUEST_ID(PHOEBE),
                                                 GUEST_ID(GLACIA), GUEST_ID(DRAKE)),
    [NAMED_TALK_BRANDON_STEVEN]     = NAMED_TALK(OUTSIDE_EAST, FLAG_TEMP_15, BRAIN_ID(PYRAMID), GUEST_ID(STEVEN)),
    [NAMED_TALK_ANABEL_TUCKER]      = NAMED_TALK(OUTSIDE_WEST, FLAG_TEMP_15, BRAIN_ID(TOWER), BRAIN_ID(DOME)),
    [NAMED_TALK_SPENSER_GRETA]      = NAMED_TALK(OUTSIDE_EAST, FLAG_TEMP_16, BRAIN_ID(PALACE), BRAIN_ID(ARENA)),
    [NAMED_TALK_LUCY_NOLAND]        = NAMED_TALK(OUTSIDE_WEST, FLAG_TEMP_16, BRAIN_ID(PIKE), BRAIN_ID(FACTORY)),
};

#undef NAMED_TALK
#undef GUEST_ID
#undef BRAIN_ID
