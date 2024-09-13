#include "game.hh"

#include <ranges>
namespace r = std::ranges;

bool Game::move_focused_unit(Nation::Id nation_id, Direction dir)
{
    auto f_unit = focused_unit(nation_id);
    if (f_unit) {
        Unit& unit = **f_unit;
        Point src_pos = unit.pos;
        unit.pos = unit.pos + directions.at(dir);
        visual_cues_.push(MoveUnit { .unit_id = unit.id, .src_pos = src_pos, .direction = dir });
        return true;
    }

    return false;

    return false;
    /*
    auto focused_it = r::find_if(units_, [nation_id](auto const& [_, u]) -> bool { return u.nation_id == nation_id && u.focused; });
    if (focused_it == units_.end())
        return false;

    Unit& unit = *focused_it;
    Point prev_pos = unit.pos;
    // unit.pos = unit.pos + directions.at(dir);
    */

    return true;
}
