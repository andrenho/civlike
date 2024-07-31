#include "civlike.hh"
#include "game.hh"

namespace cl {

Game create_game(Rules const& rules)
{
    Game game {};

    /*
    Size map_size = rules().map_size().size();
    char default_terrain = rules().default_terrain().id_char();
    for (ssize_t y = 0; y < map_size.h; ++y)
        game.map.terrain.emplace_back(map_size.w, default_terrain);
    game.map.w = map_size.w;
    game.map.h = map_size.h;

    for (size_t i = 0; i < rules().players().size(); ++i) {
        char nation_id = rules().players()[i].nation().id_char();
        for (size_t j = 0; j < rules().players()[i].initial_units().size(); ++j) {
            auto position = rules().players()[i].initial_units()[j].position().position();

            Game::Unit unit {};
            unit.nation = nation_id;
            unit.type = rules().players()[i].initial_units()[j].type().id_char();
            unit.x = position.x;
            unit.y = position.y;
            game.units.emplace_back(unit);
        }
    }
     */

    return game;
}

}