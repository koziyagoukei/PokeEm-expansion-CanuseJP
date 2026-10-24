#include "global.h"
#include "string_util.h"
#include "text.h"
#include "ultra_help.h"
#include "test/test.h"

TEST("Contest help: all eight topics fit the viewer and return to the contest menu")
{
    const struct InfoViewerEntry *root = GetInfoViewerEntry(INFO_TIPS_CONTEST);
    EXPECT_EQ(root->pageCount, 0);
    EXPECT_EQ(root->choiceCount, 9);
    EXPECT_EQ(root->choices[8].nextInfoId, INFO_VIEWER_BACK);
    for (u32 topic = 0; topic < 8; topic++)
    {
        const struct InfoViewerEntry *entry = GetInfoViewerEntry(root->choices[topic].nextInfoId);
        EXPECT_NE(entry, root);
        EXPECT_GT(entry->pageCount, 0);
        EXPECT_EQ(GetInfoViewerPageCount(root->choices[topic].nextInfoId), entry->pageCount);
        for (u32 page = 0; page < entry->pageCount; page++)
        {
            EXPECT_LE(GetStringWidth(FONT_SMALL, entry->pages[page], 0), 208);
            u32 lines = 1;
            for (const u8 *text = entry->pages[page]; *text != EOS; text++)
                if (*text == CHAR_NEWLINE)
                    lines++;
            EXPECT_LE(lines, 6);
        }
    }
}

TEST("Contest help: original info IDs and invalid-ID fallback remain available")
{
    for (u32 id = INFO_FRONTIER_RULES; id <= INFO_CHOICE_TEST; id++)
        EXPECT_GT(GetInfoViewerPageCount(id), 0);
    EXPECT(GetInfoViewerEntry(INFO_COUNT) == GetInfoViewerEntry(INFO_FRONTIER_RULES));
}
