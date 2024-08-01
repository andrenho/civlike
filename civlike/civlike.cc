#include "civlike.hh"
#include "game.hh"

namespace cl {

Game create_game(Rules const& rules, bool& success)
{
    Game game {};

    Size map_size = rules().map_size().size();
    Id default_terrain = rules().default_terrain().id();
    for (ssize_t y = 0; y < map_size.h; ++y)
        game.map.terrain.emplace_back(map_size.w, default_terrain);
    game.map.size = map_size;

    rules().players().foreach_array([&game](Player const& player) {
        player.initial_units().foreach_array([&game, &player](PlayerInitialUnits const& pu) {
            game.units.emplace_back(Game::Unit {
                .nation = player.nation().id(),
                .type = pu.type().id(),
                .pos = pu.position().position(),
            });
        });
    });

    success = true;
    return game;
}

Game move_unit(Rules const& rules, Game const& game, Direction direction, bool& success)
{
    Game g = game;
    // TODO
    return g;
}

}