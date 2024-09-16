#include <functional>
#include <vector>

#include "civlike.hh"
#include "uiproto/ui.hh"

static cl::Ruleset colonization_ruleset() {

    return {

        .terrains = {
                { .id = '~', .name = "Water",     .color = { 59, 62, 163 },   .cost_to_enter = 3, .water = true },
                { .id = 'G', .name = "Grassland", .color = { 74, 219, 48 },   .cost_to_enter = 3 },
                { .id = 'I', .name = "Ice",       .color = { 200, 200, 200 }, .cost_to_enter = 4 },
        },

        .nations = {
                { .id = 'E', .name = "England", .color = { 128, 0, 0 } },
        },

        .unit_types = {
                { .id = 'C', .name = "Colonist", .char_display = 'C', .move_points = 6 },
        },

        .map = { [](cl::Ruleset const&, cl::GameParameters const&) {
            cl::Map map;
            for (size_t x = 0; x < 10; ++x) {
                std::vector<cl::TerrainSquare> sq;
                for (size_t y = 0; y < 10; ++y)
                    sq.push_back({ cl::Terrain::Id { y > 5 ? 'I' : 'G' } });
                map.push_back(sq);
            }
            map[4][4].terrain_id = cl::Terrain::Id { '~' };
            return map;
        } },

        .initial_nations = { { { cl::Nation::Id { 'E' } } } },

        .starting_units = { {
            { .nation_id = { 'E' }, .unit_type_id = { 'C' }, .initial_pos = { 3, 3 } },
            { .nation_id = { 'E' }, .unit_type_id = { 'C' }, .initial_pos = { 5, 5 } },
            { .nation_id = { 'E' }, .unit_type_id = { 'C' }, .initial_pos = { 7, 7 } },
        } },
    };

}

int main()
{
    const cl::GameParameters par;
    const cl::Ruleset ruleset = cl::Ruleset::create_from_cpp(colonization_ruleset());
    cl::Game game(ruleset, par);
    UI(cl::Nation::Id { 'E' }).run(game);
}
