#ifndef GAME_GAME_HH_
#define GAME_GAME_HH_

#include <queue>
#include <vector>

#include "direction.hh"
#include "unit.hh"
#include "visualcue.hh"
#include "common/geometry.hh"
#include "rules/ruleset.hh"

struct Tile {
    Terrain::Id terrain_id;

    explicit Tile(TerrainSquare const& ts) : terrain_id(ts.terrain_id) {}
};
using Tiles = std::vector<std::vector<Tile>>;

struct GameNation {
    Nation::Id nation_id;
};

class Game {
public:
    Game(Ruleset const& ruleset, GameParameters const& game_par);

    void new_round();
    void focus_next(GameNation& nation);

    [[nodiscard]] std::vector<Unit const*> units_in_xy(size_t x, size_t y) const;

    bool move_focused_unit(Nation::Id nation_id, Direction dir);  // in moveunit.ccm

    // TODO - move to private
    Ruleset const&          ruleset;
    Size                    map_size { 0, 0 };
    Tiles                   tiles;
    std::vector<GameNation> nations;
    std::vector<Unit>       units;
    size_t                  round_nr = 0;
    std::queue<VisualCue>   visual_cues_;
};

#endif