#include "../game.hh"

#include <algorithm>
#include <ranges>
namespace r = std::ranges;

namespace cl {

bool Game::unit_can_focus(Unit const& unit) const
{
    return unit.moves_left > 0 && unit.state == Unit::State::Normal;
}

void Game::round_new()
{
    for (auto& [_, unit]: units_)
        unit.moves_left = unit_starting_moves(unit);
    for (auto& [_, nation]: nations_) {
        nation.round_ended = false;
        nation.focused_unit = {};
        focus_next(nation.nation_id, false);
    }
    ++round_nr_;
}

void Game::round_end(Nation::Id nation_id, bool auto_new_round)
{
    nations_.at(nation_id).round_ended = true;

    size_t nations_ended = r::count_if(nations_, [](auto const& pair) { return pair.second.round_ended; });
    if (auto_new_round && nations_ended == nations_.size())
        round_new();
}

void Game::focus_next(Nation::Id nation_id, bool auto_end_round)
{
    /*
    auto& nation = nations_.at(nation_id);

    // find focused unit
    std::map<Unit::Id, Unit>::iterator it;
    auto f_unit = nation.focused_unit;
    if (nation.focused_unit) {
        auto it = std::find(units_.begin(), units_.end(), *nation.focused_unit);
        nation.focused_unit = {};
        ++it;
    } else {
        it = units_.begin();  // if not a focused unit, start from the beginning
    }

    // go to next
    while (it != units_.end()) {
        auto const& [id, unit] = *it;
        if (unit.nation_id == nation_id and unit_can_focus(unit)) {
            nation.focused_unit = id;
            return;
        }
        ++it;
    }

    // if not found, but there was a focused one, try again
    if (f_unit) {
        it = units_.begin();
        for (; it->first != *nation.focused_unit && it != units_.end(); ++it) {
            auto const& [id, unit] = *it;
            if (unit.nation_id == nation_id and unit_can_focus(unit)) {
                nation.focused_unit = id;
                return;
            }
        }
    }

    // if we get here, it means no more units are available, so we end the round
    if (auto_end_round)
        round_end(nation_id, auto_end_round);
        */
}

void Game::focus_unit(Unit::Id unit_id, bool auto_end_round)
{
    Unit const& unit = units_[unit_id];
    if (unit_can_focus(unit))
        nations_.at(unit.nation_id).focused_unit = unit_id;
    else
        focus_next(unit.nation_id, auto_end_round);
}

}