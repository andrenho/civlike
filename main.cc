#include <functional>
#include <vector>

#include "civlike.hh"
#include "uiproto/ui.hh"

static Ruleset colonization_ruleset() {

    return {

        .terrains = {
                { .name = "Water", .color = { 59, 62, 163 }, .cost_to_enter = Terrain::Impassable },
                { .name = "Grassland", .color = { 74, 219, 48 }, .cost_to_enter = 3 },
        },

        .nations = {
                { .name = "England", .color = { 128, 0, 0 }, .playable = true },
        },

        .unit_types = {
                { .name = "Colonist", .char_display = 'C', .move_points = 6, },
        },

        .map = { [](Ruleset const&, GameParameters const&) {
            Map map;
            for (size_t x = 0; x < 10; ++x) {
                std::vector<TerrainSquare> sq;
                for (size_t y = 0; y < 10; ++y)
                    sq.push_back({ Terrain::Id { 1 } });
                map.push_back(sq);
            }
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
