/* Appended to the engine text harness by test_remove_redundant_jpn.py. */
int main(int argc, char **argv)
{
    assert(argc == 2);
    FILE *file = fopen(argv[1], "rb");
    assert(file);
    u32 length, record = 0, failures = 0;
    const u8 fonts[] = {FONT_NORMAL, FONT_SMALL, FONT_SHORT, FONT_NARROW, FONT_NARROWER,
        FONT_SMALL_NARROW, FONT_SMALL_NARROWER, FONT_SHORT_NARROW, FONT_SHORT_NARROWER};
    const u8 kana[] = {1, 2, 0x51, EOS};
    const u8 eng[] = {ENG, CHAR_A, CHAR_B, EOS};
    const u8 mixed[] = {ENG, CHAR_A, JPN, 1, EOS};
    const u8 *variants[] = {kana, eng, mixed};
    static u8 input[16384], expanded[16384], old[16384], newer[16384];
    static __typeof__(sTrace) previous;
    while (fread(&length, 4, 1, file) == 1)
    {
        assert(length < sizeof(input));
        assert(fread(input, 1, length, file) == length);
        assert(input[0] == CTRL && input[1] == EXT_CTRL_CODE_JPN);
        bool32 failed = FALSE;
        for (u32 v = 0; v < ARRAY_COUNT(variants) && !failed; v++)
        {
            reset();
            for (u32 i = 0; i < 8; i++)
            {
                sPlaceholders[i] = variants[v];
                DynamicPlaceholderTextUtil_SetPlaceholderPtr(i, variants[v]);
            }
            StringExpandPlaceholders(expanded, input);
            DynamicPlaceholderTextUtil_ExpandPlaceholders(old, expanded);
            StringExpandPlaceholders(expanded, input + 2);
            DynamicPlaceholderTextUtil_ExpandPlaceholders(newer, expanded);
            for (u32 f = 0; f < ARRAY_COUNT(fonts) && !failed; f++)
            {
                sTraceCount = 0;
                sLoadedJapanese = FALSE;
                memset(sTrace, 0, sizeof(sTrace));
                struct TextPrinter a = render(old, fonts[f], 0, WINDOW_TEXT_PRINTER);
                u32 count = sTraceCount;
                memcpy(previous, sTrace, sizeof(sTrace));
                sTraceCount = 0;
                sLoadedJapanese = FALSE;
                memset(sTrace, 0, sizeof(sTrace));
                struct TextPrinter b = render(newer, fonts[f], 0, WINDOW_TEXT_PRINTER);
                if (count != sTraceCount || memcmp(previous, sTrace, count * sizeof(sTrace[0]))
                    || a.printerTemplate.currentX != b.printerTemplate.currentX
                    || a.printerTemplate.currentY != b.printerTemplate.currentY
                    || GetStringWidth(fonts[f], old, 0) != GetStringWidth(fonts[f], newer, 0))
                    failed = TRUE;
            }
        }
        if (failed)
        {
            printf("MISMATCH %u\n", record);
            failures++;
        }
        record++;
    }
    fclose(file);
    printf("Compared %u strings, 9 fonts, 3 placeholder variants; mismatches=%u\n", record, failures);
    return failures != 0;
}
