#include "global.h"
#include "test/test.h"
#include "constants/field_weather.h"
#include "constants/script_commands.h"

extern const u8 FrontierStory_EventScript_CureFlash[];
extern const u8 FrontierStory_EventScript_CampVisitorFadeOut[];
extern const u8 FrontierStory_EventScript_CampVisitorFadeIn[];

TEST("Frontier story: same-map cure and camp fades preserve time-of-day palettes")
{
    const u8 *flash = FrontierStory_EventScript_CureFlash;
    EXPECT_EQ(flash[0], SCR_OP_CLOSEMESSAGE);
    EXPECT_EQ(flash[1], SCR_OP_PLAYSE);
    EXPECT_EQ(flash[4], SCR_OP_FADESCREENSWAPBUFFERS);
    EXPECT_EQ(flash[5], FADE_TO_WHITE);
    EXPECT_EQ(flash[7], SCR_OP_WAITSE);
    EXPECT_EQ(flash[8], SCR_OP_FADESCREENSWAPBUFFERS);
    EXPECT_EQ(flash[9], FADE_FROM_WHITE);
    EXPECT_EQ(flash[11], SCR_OP_RETURN);
    EXPECT_EQ(FrontierStory_EventScript_CampVisitorFadeOut[0], SCR_OP_FADESCREENSWAPBUFFERS);
    EXPECT_EQ(FrontierStory_EventScript_CampVisitorFadeOut[1], FADE_TO_BLACK);
    EXPECT_EQ(FrontierStory_EventScript_CampVisitorFadeIn[0], SCR_OP_FADESCREENSWAPBUFFERS);
    EXPECT_EQ(FrontierStory_EventScript_CampVisitorFadeIn[1], FADE_FROM_BLACK);
}
