#ifndef COLONIZATION_HH_
#define COLONIZATION_HH_

#include "civlike.hh"

inline static cl::Ruleset colonization {

    .goods = {{
        { .id = 'F', .name = "Food", .initial_value = 1.0 },
    }},

    .terrains = {{
        { .id = '~', .name = "Water",     .color = { 59, 62, 163 },   .cost_to_enter = 3, .default_good = 'F', .water = true },
        { .id = 'G', .name = "Grassland", .color = { 74, 219, 48 },   .cost_to_enter = 3, .default_good = 'F', .production = {
            { .good_id = 'F', .production = 3 },
        }},
        { .id = 'I', .name = "Ice",       .color = { 200, 200, 200 }, .cost_to_enter = 4, .default_good = 'F' },
    }},

    .buildings = {{
    }},

    .nations = {{
        { .id = 'E', .name = "England", .color = { 128, 0, 0 } },
    }},

    .unit_types = {{
        { .id = 'C', .name = "Colonist", .char_display = 'C', .move_points = 6 },
    }},

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

#endif