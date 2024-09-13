#include "game.hh"

#include <ranges>
namespace r = std::ranges;

void Game::new_round()
{
    for (auto& nation: nations_) {
        nation.focused_unit = {};
        focus_next(nation.nation_id);
    }
    ++round_nr_;
}

void Game::focus_next(Nation::Id nation_id)
{
    auto& nation = nations_.at(nation_id);

    // find focused unit
    std::map<Unit::Id, Unit>::iterator it;
    auto f_unit = nation.focused_unit;
    if (nation.focused_unit) {
        it = units_.find(*nation.focused_unit);
        nation.focused_unit = {};
        ++it;
    } else {
        it = units_.begin();  // if not a focused unit, start from the beginning
    }

    // go to next
    while (it != units_.end()) {
        auto const& [id, unit] = *it;
        if (unit.nation_id == nation_id and unit.can_focus()) {
            nation.focused_unit = id;
            return;
        }
        ++it;
    }

    // if not found, but there was a focused one, try again
    if (f_unit) {
        it = units_.begin();
        for (; it->first != *nation.focused_unit; ++it) {
            auto const& [id, unit] = *it;
            if (unit.nation_id == nation_id and unit.can_focus()) {
                nation.focused_unit = id;
                return;
            }
        }
    }
}

