#include "game.hh"

#include <ranges>
namespace r = std::ranges;

Game::Game(Ruleset const& ruleset, GameParameters const& game_par)
    : ruleset(ruleset)
{
    const Map map = ruleset.map(ruleset, game_par);

    map_size_.w = map.size();
    map_size_.h = map[0].size();

    tiles_.reserve(map_size_.w);
    for (size_t x = 0; x < map_size_.w; ++x) {
        auto& tt = tiles_.emplace_back();
        tt.reserve(map_size_.h);
        for (size_t y = 0; y < map_size_.h; ++y)
            tt.emplace_back(Tile(map[x][y]));
    }

    for (auto const& initial_nation: ruleset.initial_nations(ruleset, game_par))
        nations_.push_back(GameNation(initial_nation));

    for (auto const& s_unit: ruleset.starting_units(ruleset, game_par)) {
        Unit::Id id = Unit::next_id();
        units_.emplace(id, Unit { id, s_unit });
    }

    new_round();
}

std::vector<Unit const*> Game::units_in_xy(Point p) const
{
    std::vector<Unit const*> r;
    for (auto const& [_, unit]: units_)
        if (unit.pos == p)
            r.push_back(&unit);
    return r;
}

std::optional<Unit const*> Game::focused_unit(Nation::Id nation_id) const
{
    const auto& funit = nations_.at(nation_id).focused_unit;
    if (funit)
        return &units_.at(*funit);
    return {};
}

std::optional<Unit*> Game::focused_unit(Nation::Id nation_id)
{
    auto f_unit = const_cast<Game const*>(this)->focused_unit(nation_id);
    return f_unit ? std::optional(const_cast<Unit *>(*f_unit)) : std::optional<Unit*>();
}
