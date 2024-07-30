#include "civlike.hh"
#include "game.hh"

namespace cl {

Game create_game(Rules const& rules)
{
    Game game {};

    Size map_size = rules().map_size().size();
    char default_terrain = rules().default_terrain().id()[0];
    for (ssize_t y = 0; y < map_size.h; ++y)
        game.map.terrain.emplace_back(map_size.w, default_terrain);
    game.map.w = map_size.w;
    game.map.h = map_size.h;

    return game;
}

}