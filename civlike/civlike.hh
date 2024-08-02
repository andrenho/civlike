#ifndef CIVLIKE_HH_
#define CIVLIKE_HH_

#include <utility>

#include "game.hh"
#include "rules.hh"

namespace cl {

static bool _opt;

enum class Direction { NW, N, NE, W, E, SW, S, SE };

struct MoveResponse {
    Game game;
    std::optional<Id> unit_moved;
    Position from {0, 0};
};

Game         create_game(Rules const& rules);
Game         new_round(Rules const& rules, Game const& game);
MoveResponse move_focused_unit(Rules const& rules, Game const& game, Direction direction);

}

#endif