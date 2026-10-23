/* Appended to the actual text engine and Pokedex formatters by the host test. */
static void checkMeasurement(const u8 *text)
{
    assert(text == sAllocation + 8);
    assert(text[0] == CTRL && text[1] == EXT_CTRL_CODE_ENG);
    assert(memchr(text, EOS, WEIGHT_HEIGHT_STR_MEM) != NULL);
    for (u32 i = 0; i < 8; i++)
    {
        assert(sAllocation[i] == 0xA5);
        assert(sAllocation[8 + WEIGHT_HEIGHT_STR_MEM + i] == 0xA5);
    }
    parity(text, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER, 0);
    for (u32 i = 0; i < sTraceCount; i++)
        assert(!sTrace[i].japanese);
}

int main(int argc, char **argv)
{
    assert(argc == 2);
    if (!strcmp(argv[1], "measurements"))
    {
        const u32 values[] = {0, 1, 17, 99, 100, 655, 1000, 9999};
        for (u32 i = 0; i < ARRAY_COUNT(values); i++)
        {
            checkMeasurement(ConvertMonHeightToString(values[i]));
            checkMeasurement(ConvertMonWeightToString(values[i]));
        }
        checkMeasurement(GetUnknownMonHeightString());
        checkMeasurement(GetUnknownMonWeightString());
    }
    else if (!strcmp(argv[1], "layouts"))
    {
        const u8 number[] = {CHAR_SPACE, CHAR_1, EOS};
        const u8 *layouts[] = {sStatsLeftColumnLayout, sStatsLeftIVEVColumnLayout,
            sStatsRightColumnLayout, sMovesPPLayout};
        for (u32 l = 0; l < ARRAY_COUNT(layouts); l++)
        {
            u8 expanded[256];
            reset();
            for (u32 i = 0; i < 4; i++)
                DynamicPlaceholderTextUtil_SetPlaceholderPtr(i, number);
            DynamicPlaceholderTextUtil_ExpandPlaceholders(expanded, layouts[l]);
            render(expanded, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER);
            for (u32 i = 0; i < sTraceCount; i++)
                assert(!sTrace[i].japanese);
        }
    }
    else if (!strcmp(argv[1], "decimals"))
    {
        const u8 number[] = {CHAR_1, EOS};
        const u8 *layouts[] = {gText_XDotY, gText_XDotY2, gText_XDotY3, sText_Var1DotVar2Cm};
        for (u32 l = 0; l < ARRAY_COUNT(layouts); l++)
        {
            u8 expanded[256];
            reset();
            for (u32 i = 0; i < 8; i++)
                sPlaceholders[i] = number;
            StringExpandPlaceholders(expanded, layouts[l]);
            render(expanded, FONT_NORMAL, 0, WINDOW_TEXT_PRINTER);
            for (u32 i = 0; i < sTraceCount; i++)
                assert(!sTrace[i].japanese);
        }
    }
    else abort();
    return 0;
}
