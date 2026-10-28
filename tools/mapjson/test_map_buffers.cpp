#define main mapjson_cli_main
#include "mapjson.cpp"
#undef main
#include <cassert>

static Json layout(double width, double height, const string &id = "LAYOUT_TEST")
{
    return Json::object{{"id", id}, {"width", width}, {"height", height}};
}

int main()
{
    assert(layout_fits_map_buffer(layout(72, 72)));
    assert(layout_fits_map_buffer(layout(17, 242)));
    assert(!layout_fits_map_buffer(layout(18, 242)));
    assert(!layout_fits_map_buffer(layout(0, 32)));
    assert(!layout_fits_map_buffer(layout(-1, 32)));
    assert(!layout_fits_map_buffer(layout(32.5, 32)));
    assert(!layout_fits_map_buffer(layout(1e20, 1e20)));
    assert(layout_fits_map_buffer(layout(8, 8, "LAYOUT_BATTLE_PYRAMID_SQUARE01")));
    assert(!layout_fits_map_buffer(layout(32, 32, "LAYOUT_BATTLE_PYRAMID_SQUARE01")));
    assert(!layout_fits_map_buffer(layout(32, 32, "LAYOUT_BATTLE_FRONTIER_BATTLE_PYRAMID_FLOOR")));
    string error;
    const auto layouts = Json::parse(read_text_file("data/layouts/layouts.json"), error);
    assert(error.empty());
    for (const auto &entry : layouts["layouts"].array_items())
        assert(layout_fits_map_buffer(entry));
    cout << "PASS: retained layouts, border limits, pyramid generation and malformed dimensions" << endl;
}
