#include "command.hh"

namespace cl::cmd {

Game new_game(Ruleset const& ruleset, GameParameters const& game_par)
{
    Game G(ruleset);

    const Map map = ruleset.map(ruleset, game_par);

    G.map_size.w = map.size();
    G.map_size.h = map[0].size();

    G.tiles.reserve(G.map_size.w);
    for (size_t x = 0; x < G.map_size.w; ++x) {
        auto& tt = G.tiles.emplace_back();
        tt.reserve(G.map_size.h);
        for (size_t y = 0; y < G.map_size.h; ++y)
            tt.emplace_back(Tile({ (int) x, (int) y }, map[x][y]));
    }

    for (auto const& initial_nation: ruleset.initial_nations(ruleset, game_par))
        G.nations.emplace(initial_nation, GameNation(initial_nation));

    for (auto const& s_unit: ruleset.starting_units(ruleset, game_par)) {
        G.units.emplace(Unit { s_unit });
    }

    next_round(G);

    return G;
}

}