#include "game.hh"

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

    for (auto const& s_unit: ruleset.starting_units(ruleset, game_par))
        units_.push_back(Unit(s_unit));

    new_round();
}

std::vector<Unit const*> Game::units_in_xy(size_t x, size_t y) const
{
    std::vector<Unit const*> r;
    for (auto const& unit: units_)
        if (unit.pos.x == x && unit.pos.y == y)
            r.push_back(&unit);
    return r;
}