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

namespace cl {

struct Tile {
    Terrain::Id terrain_id;

    explicit Tile(TerrainSquare const& ts) : terrain_id(ts.terrain_id) {}
};
using Tiles = std::vector<std::vector<Tile>>;

struct GameNation {
    Nation::Id              nation_id;
    std::optional<Unit::Id> focused_unit;
    bool                    round_ended = false;
};

class Game {
public:
    Game(Ruleset const& ruleset, GameParameters const& game_par);

    // Round/Focus (see roundfocus.cc)

    void round_new();
    void round_end(Nation::Id nation_id, bool auto_new_round=true);
    void focus_next(Nation::Id nation_id, bool auto_end_round=true);

    // Move unit (see unitactions.cc)

    void unit_move(Unit::Id unit_id, Direction dir);
    void unit_change_state(Unit::Id unit_id, Unit::State state);

    // queries

    [[nodiscard]] std::vector<Unit const*>   units_in_xy(Point p) const;
    [[nodiscard]] std::optional<Unit const*> focused_unit(Nation::Id nation_id) const;
    [[nodiscard]] std::optional<Unit*>       focused_unit(Nation::Id nation_id);

    // getters

    [[nodiscard]] Size const&                     map_size() const { return map_size_; }
    [[nodiscard]] Tiles const&                    tiles() const { return tiles_; }
    [[nodiscard]] std::vector<GameNation> const&  nations() const { return nations_; }
    [[nodiscard]] std::map<Unit::Id, Unit> const& units() const { return units_; }
    [[nodiscard]] size_t                          round_nr() const { return round_nr_; }
    [[nodiscard]] std::queue<VisualCue>&          visual_cues() { return visual_cues_; }

    // fields

    Ruleset const& ruleset;

private:
    [[nodiscard]] unsigned long unit_starting_moves(Unit const& unit) const;
    [[nodiscard]] bool          unit_can_focus(Unit const& unit) const;
    [[nodiscard]] unsigned long tile_moves_to_enter(Point p) const;

    Size                     map_size_ { 0, 0 };
    Tiles                    tiles_;
    std::vector<GameNation>  nations_;
    std::map<Unit::Id, Unit> units_;
    size_t                   round_nr_ = 0;
    std::queue<VisualCue>    visual_cues_;
};

}

#endif