#include "game.hh"

#include <ranges>
namespace r = std::ranges;

bool Game::move_focused_unit(Nation::Id nation_id, Direction dir)
{
    auto f_unit = focused_unit(nation_id);
    if (f_unit) {
        Unit& unit = **f_unit;

        unsigned int moves_to_enter = tile_moves_to_enter(unit.pos + directions.at(dir));
        if (unit.moves_left >= moves_to_enter) {  // ok, move unit
            Point src_pos = unit.pos;
            unit.moves_left -= moves_to_enter;
            unit.pos = unit.pos + directions.at(dir);
            visual_cues_.push(MoveUnit { .unit_id = unit.id, .src_pos = src_pos, .direction = dir });
            return true;

        } else {  // no moves left
            unit.moves_left = 0;
        }

        if (unit.moves_left <= 0)
            focus_next(nation_id);
    }

    return false;
}
