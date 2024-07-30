#ifndef CIVLIKE_HH_
#define CIVLIKE_HH_

#include "game.hh"
#include "rules.hh"

namespace cl {

Game create_game(Rules const& rules);

}

#endif