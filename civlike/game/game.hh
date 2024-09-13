#ifndef GAME_GAME_HH_
#define GAME_GAME_HH_

#include <map>
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
    Nation::Id              nation_id;
    std::optional<Unit::Id> focused_unit;
};

class Game {
public:
    Game(Ruleset const& ruleset, GameParameters const& game_par);

    // Round/Focus (see roundfocus.cc)

    void new_round();
    void focus_next(Nation::Id nation_id);

    // Move unit (see moveunit.cc)

    bool move_focused_unit(Nation::Id nation_id, Direction dir);

    // queries

    [[nodiscard]] std::vector<Unit const*> units_in_xy(Point p) const;
    [[nodiscard]] std::optional<Unit const*> focused_unit(Nation::Id nation_id) const;
    [[nodiscard]] std::optional<Unit*> focused_unit(Nation::Id nation_id);

    // getters

    [[nodiscard]] Size const& map_size() const { return map_size_; }
    [[nodiscard]] Tiles const& tiles() const { return tiles_; }
    [[nodiscard]] std::vector<GameNation> const& nations() const { return nations_; }
    [[nodiscard]] std::map<Unit::Id, Unit> const& units() const { return units_; }
    [[nodiscard]] size_t round_nr() const { return round_nr_; }

    [[nodiscard]] std::queue<VisualCue>& visual_cues() { return visual_cues_; }

    // fields

    Ruleset const& ruleset;

private:
    Size                     map_size_ { 0, 0 };
    Tiles                    tiles_;
    std::vector<GameNation>  nations_;
    std::map<Unit::Id, Unit> units_;
    size_t                   round_nr_ = 0;
    std::queue<VisualCue>    visual_cues_;
};

#endif