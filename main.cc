#include <functional>
#include <vector>

#include "civlike.hh"
#include "uiproto/ui.hh"

static Ruleset colonization_ruleset() {

    return {

        .terrains = {
                { .name = "Water", .color = { 59, 62, 163 }, .cost_to_enter = 3, .water = true },
                { .name = "Grassland", .color = { 74, 219, 48 }, .cost_to_enter = 3 },
                { .name = "Ice", .color = { 200, 200, 200 }, .cost_to_enter = 4 },
        },

        .nations = {
                { .name = "England", .color = { 128, 0, 0 } },
        },

        .unit_types = {
                { .name = "Colonist", .char_display = 'C', .move_points = 6 },
        },

        .map = { [](Ruleset const&, GameParameters const&) {
            Map map;
            for (size_t x = 0; x < 10; ++x) {
                std::vector<TerrainSquare> sq;
                for (size_t y = 0; y < 10; ++y)
                    sq.push_back({ Terrain::Id { y > 5 ? 2 : 1 } });
                map.push_back(sq);
            }
            map[4][4].terrain_id = Terrain::Id { 0 };
            return map;
        } },

        .initial_nations = { { { Nation::Id { 0 } } } },

        .starting_units = { {
            { .nation_id = { 0 }, .unit_type_id = { 0 }, .initial_pos = { 3, 3 } },
            { .nation_id = { 0 }, .unit_type_id = { 0 }, .initial_pos = { 5, 5 } },
            { .nation_id = { 0 }, .unit_type_id = { 0 }, .initial_pos = { 7, 7 } },
        } },
    };

}

int main()
{
    const GameParameters par;
    const Ruleset ruleset = Ruleset::create_from_cpp(colonization_ruleset());
    Game game(ruleset, par);
    UI(Nation::Id { 0 }).run(game);
}
