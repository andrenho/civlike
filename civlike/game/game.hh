#ifndef GAME_GAME_HH_
#define GAME_GAME_HH_

#include <queue>
#include <vector>

#include "city.hh"
#include "direction.hh"
#include "unit.hh"
#include "visualcue.hh"
#include "collections/countermap.hh"
#include "collections/idrefmap.hh"
#include "common/geometry.hh"
#include "rules/ruleset.hh"

namespace cl {

struct Tile {
    Terrain::Id terrain_id;

    explicit Tile(TerrainSquare const& ts) : terrain_id(ts.terrain_id) {}
};
using Tiles = std::vector<std::vector<Tile>>;

struct GameNation {
    Nation::Id              id;
    std::optional<Unit::Id> focused_unit;
    bool                    round_ended = false;
};

class Game {
public:
    Game(Ruleset const& ruleset) : ruleset(ruleset) {}

    Ruleset const&                   ruleset;
    Size                             map_size { 0, 0 };
    Tiles                            tiles;
    IdRefMap<Nation::Id, GameNation> nations;
    CounterMap<Unit>                 units;
    CounterMap<City>                 cities;
    size_t                           round_nr = 0;
    std::queue<VisualCue>            visual_cues;


    std::vector<Unit const*> units_in_xy(Point p) const {
        std::vector<Unit const*> r;
        for (auto const& [_, unit]: units)
            if (unit.pos == p)
                r.push_back(&unit);
        return r;
    }

    std::optional<Unit const*> focused_unit(Nation::Id nation_id) const {
        const auto& funit = nations[nation_id].focused_unit;
        if (funit)
            return &units[*funit];
        return {};
    }

};

}

#endif