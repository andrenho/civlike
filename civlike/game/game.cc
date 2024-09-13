#include "game.hh"

#include <ranges>
namespace r = std::ranges;

Game::Game(Ruleset const& ruleset, GameParameters const& game_par)
    : ruleset(ruleset)
{
    const Map map = ruleset.map(ruleset, game_par);

    map_size.w = map.size();
    map_size.h = map[0].size();

    tiles.reserve(map_size.w);
    for (size_t x = 0; x < map_size.w; ++x) {
        auto& tt = tiles.emplace_back();
        tt.reserve(map_size.h);
        for (size_t y = 0; y < map_size.h; ++y)
            tt.emplace_back(Tile(map[x][y]));
    }

    for (auto const& initial_nation: ruleset.initial_nations(ruleset, game_par))
        nations.push_back(GameNation(initial_nation));

    for (auto const& s_unit: ruleset.starting_units(ruleset, game_par))
        units.push_back(Unit(s_unit));

    new_round();
}

void Game::new_round()
{
    for (auto& unit: units)
        unit.new_round();
    for (auto& nation: nations)
        focus_next(nation);
    ++round_nr;
}

void Game::focus_next(GameNation& nation)
{
    // find focused unit
    auto it = r::find_if(units, [&nation](Unit const& u) { return u.nation_id == nation.nation_id && u.focused; });
    Unit* focused_unit = nullptr;
    if (it != units.end()) {
        focused_unit = &*it;
        focused_unit->focused = false;
        ++it;
    } else {
        it = units.begin();  // if not a focused unit, start from the beginning
    }

    // go to next
    while (it != units.end()) {
        if (it->nation_id == nation.nation_id and it->can_focus()) {
            it->focused = true;
            return;
        }
        ++it;
    }

    // if not found, but there was a focused one, try again
    if (focused_unit) {
        it = units.begin();
        for (; !it->focused; ++it) {
            if (it->nation_id == nation.nation_id and it->can_focus()) {
                it->focused = true;
                return;
            }
        }
    }
}

std::vector<Unit const*> Game::units_in_xy(size_t x, size_t y) const
{
    std::vector<Unit const*> r;
    for (auto const& unit: units)
        if (unit.pos.x == x && unit.pos.y == y)
            r.push_back(&unit);
    return r;
}

bool Game::move_focused_unit(Nation::Id nation_id, Direction dir)
{
    auto focused_it = r::find_if(units, [nation_id](Unit const& u) -> bool { return u.nation_id == nation_id && u.focused; });
    if (focused_it == units.end())
        return false;


    return true;
}
