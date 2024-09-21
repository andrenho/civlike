#ifndef GAME_GAME_HH_
#define GAME_GAME_HH_

#include <queue>
#include <ranges>
#include <vector>
namespace r = std::ranges;

#include "city.hh"
#include "unit.hh"
#include "visualcue.hh"
#include "collections/countermap.hh"
#include "collections/idrefmap.hh"
#include "common/geometry.hh"
#include "rules/ruleset.hh"

namespace cl {

struct Tile {
    Terrain::Id terrain_id;
    MapPos      pos;

    explicit Tile(MapPos pos, TerrainSquare const& ts) : pos(pos), terrain_id(ts.terrain_id) {}
};
using Tiles = std::vector<std::vector<Tile>>;

struct GameNation {
    Nation::Id              id;
    std::optional<Unit::Id> focused_unit;
    bool                    round_ended = false;
};

class Game {
public:
    explicit Game(Ruleset const& ruleset) : ruleset(ruleset) {}

    Ruleset const&                   ruleset;
    Size                             map_size { 0, 0 };
    Tiles                            tiles;
    IdRefMap<Nation::Id, GameNation> nations;
    CounterMap<Unit>                 units;
    CounterMap<City>                 cities;
    size_t                           round_nr = 0;
    mutable std::queue<VisualCue>    visual_cues;

    Terrain const& terrain(MapPos p) const {
        return ruleset.terrains[tiles[p.x][p.y].terrain_id];
    }

    std::vector<Unit const*> units_in_xy(MapPos p) const {
        std::vector<Unit const*> r;
        for (auto const& [_, unit]: units)
            if (unit.pos == p)
                r.push_back(&unit);
        return r;
    }

    std::optional<City const*> city_in_xy(MapPos p) {
        auto it = r::find_if(cities, [&p](auto const& pair) { return pair.second.pos == p; });
        return it == cities.end() ? std::optional<City const*>{} : &it->second;
    }

    std::optional<Unit const*> focused_unit(Nation::Id nation_id) const {
        const auto& funit = nations[nation_id].focused_unit;
        return funit ? &units[*funit] : std::optional<Unit const*>{};
    }

};

}

#endif