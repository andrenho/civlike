#ifndef CIVLIKE_HH_
#define CIVLIKE_HH_

#include <utility>

#include "game.hh"
#include "rules.hh"

namespace cl {

static bool _opt;

enum class Direction { NW, N, NE, W, E, SW, S, SE };

Game create_game(Rules const& rules);
Game new_round(Rules const& rules, Game const& game);
std::pair<Game, bool> move_unit(Rules const& rules, Game const& game, Direction direction);

}

#endif