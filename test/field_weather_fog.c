#include "global.h"
#include "field_weather.h"
#include "test/test.h"
#include "constants/weather.h"

// The deep bath and the waterfall bath use WEATHER_FOG in their headers. It has no weather
// callbacks of its own, and leaving those maps (or loading a save made there) crashed.
TEST("Overworld weather: a map's WEATHER_FOG is saved and shown as diagonal fog")
{
    u8 oldSaved = GetSavedWeather();

    SetSavedWeather(WEATHER_FOG);
    EXPECT_EQ(GetSavedWeather(), WEATHER_FOG_DIAGONAL);
    gSaveBlock1Ptr->weather = oldSaved;
}

TEST("Overworld weather: weather without callbacks is never made current")
{
    u8 weather = WEATHER_NONE, expected = WEATHER_NONE;

    PARAMETRIZE { weather = WEATHER_FOG; expected = WEATHER_FOG_DIAGONAL; }
    PARAMETRIZE { weather = WEATHER_DYNAMIC; expected = WEATHER_NONE; }
    PARAMETRIZE { weather = WEATHER_COUNT; expected = WEATHER_NONE; }
    PARAMETRIZE { weather = 0xFF; expected = WEATHER_NONE; }
    PARAMETRIZE { weather = WEATHER_FOG_HORIZONTAL; expected = WEATHER_FOG_HORIZONTAL; }

    // As when a save made in the deep bath is loaded.
    SetCurrentAndNextWeather(weather);
    EXPECT_EQ(gWeatherPtr->currWeather, expected);
    EXPECT_EQ(gWeatherPtr->nextWeather, expected);
    SetNextWeather(weather);
    EXPECT_EQ(gWeatherPtr->nextWeather, expected);
    SetCurrentAndNextWeather(WEATHER_NONE);
}
