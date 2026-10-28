#define main mapjson_cli_main
#include "mapjson.cpp"
#undef main
#include <cassert>
#include <chrono>

int main()
{
    const auto fixture = std::filesystem::temp_directory_path()
        / ("mapjson-heal-" + std::to_string(std::filesystem::file_time_type::clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(fixture / "src/data");
    std::filesystem::current_path(fixture);
    write_text_file("src/data/heal_locations.json", R"({
  "heal_locations": [
    {
      "respawn_map": "MAP_REMOVED",
      "respawn_npc": "LOCALID_REMOVED_NURSE"
    },
    {
      "respawn_map": "MAP_REMOVED_ZERO",
      "respawn_npc": "0"
    },
    {
      "respawn_map": "MAP_KEEP",
      "respawn_npc": "LOCALID_FRONTIER_NURSE"
    },
    {
      "respawn_map": "MAP_KEEP",
      "respawn_npc": "0"
    }
  ]
})");
    vector<string> valid_maps = {"MAP_KEEP"};
    for (int pass = 0; pass < 2; ++pass) {
        clean_heal_locations(valid_maps);
        string error;
        const auto data = Json::parse(read_text_file("src/data/heal_locations.json"), error);
        assert(error.empty());
        const auto &locations = data["heal_locations"].array_items();
        assert(locations.size() == 4);
        assert(locations[0]["respawn_npc"].string_value() == "0");
        assert(locations[1]["respawn_npc"].string_value() == "0");
        assert(locations[2]["respawn_npc"].string_value() == "LOCALID_FRONTIER_NURSE");
        assert(locations[3]["respawn_npc"].string_value() == "0");
    }
    const auto path = std::filesystem::path("src/data/heal_locations.json");
    std::filesystem::last_write_time(path, std::filesystem::file_time_type::clock::now() - std::chrono::hours(1));
    const auto before = std::filesystem::last_write_time(path);
    clean_heal_locations(valid_maps);
    assert(std::filesystem::last_write_time(path) == before);
    cout << "PASS: removed/zero/retained respawns and unchanged-file preservation" << endl;
    return 0;
}
