#include "game.hh"

#include <ranges>
namespace r = std::ranges;

bool Game::unit_can_focus(Unit const& unit) const
{
    return unit.moves_left > 0;
}

void Game::new_round()
{
    for (auto& [_, unit]: units_)
        unit.moves_left = unit_starting_moves(unit);
    for (auto& nation: nations_) {
        nation.round_ended = false;
        nation.focused_unit = {};
        focus_next(nation.nation_id, false);
    }
    ++round_nr_;
}

void Game::end_round(Nation::Id nation_id, bool auto_new_round)
{
    nations_.at(nation_id).round_ended = true;

    size_t nations_ended = r::count_if(nations_, [](GameNation const& gn) { return gn.round_ended; });
    if (auto_new_round && nations_ended == nations_.size())
        new_round();
}

void Game::focus_next(Nation::Id nation_id, bool auto_end_round)
{
    // TODO - what if the nation has no units available?

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
        end_round(nation_id, auto_end_round);
}

