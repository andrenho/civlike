#include "game.hh"

#include <ranges>
namespace r = std::ranges;

void Game::new_round()
{
    for (auto& unit: units_)
        unit.new_round();
    for (auto& nation: nations_)
        focus_next(nation);
    ++round_nr_;
}

void Game::focus_next(GameNation& nation)
{
    // find focused unit
    auto it = r::find_if(units_, [&nation](Unit const& u) { return u.nation_id == nation.nation_id && u.focused; });
    Unit* focused_unit = nullptr;
    if (it != units_.end()) {
        focused_unit = &*it;
        focused_unit->focused = false;
        ++it;
    } else {
        it = units_.begin();  // if not a focused unit, start from the beginning
    }

    // go to next
    while (it != units_.end()) {
        if (it->nation_id == nation.nation_id and it->can_focus()) {
            it->focused = true;
            return;
        }
        ++it;
    }

    // if not found, but there was a focused one, try again
    if (focused_unit) {
        it = units_.begin();
        for (; !it->focused; ++it) {
            if (it->nation_id == nation.nation_id and it->can_focus()) {
                it->focused = true;
                return;
            }
        }
    }
}

