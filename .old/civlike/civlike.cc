#include "civlike.hh"
#include "game.hh"

namespace cl {

Game create_game(Rules const& rules)
{
    Game g {};

    Size map_size = rules().map_size().size();
    Id default_terrain = rules().default_terrain().id();
    for (ssize_t y = 0; y < map_size.h; ++y)
        g.map.terrain.emplace_back(map_size.w, default_terrain);
    g.map.size = map_size;

    Id player_nation = rules().player_nation().id();
    rules().initial_units().foreach_array([&](PlayerInitialUnits const& pu) {
        g.units.emplace(g.last_unit_id++, Game::Unit {
            .nation = player_nation,
            .type = pu.type().id(),
            .pos = pu.position().position(),
            .moves_left = 0,
        });
    });

    g = new_round(rules, g);
    return g;
}

Game new_round(Rules const& rules, Game const& game)
{
    Game g = game;
    for (auto& [id, unit]: g.units) {
        unit.moves_left = rules().unit_types()[unit.type].moves_per_round().integer(game);
        if (!g.focused_unit)
            g.focused_unit = id;
    }
    return g;
}

MoveResponse move_focused_unit(Rules const& rules, Game const& game, Direction direction)
{
    Game g = game;
    // TODO
    return { .game = game, .unit_moved = {} };
}

}