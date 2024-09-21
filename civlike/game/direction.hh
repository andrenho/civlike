#ifndef GAME_DIRECTION_HH
#define GAME_DIRECTION_HH

#include <unordered_map>

#include "common/geometry.hh"

namespace cl {

enum class Direction {
    NW, N, NE, W, E, SW, S, SE,
};

const inline std::unordered_map<Direction, MapPos> directions = {
    { Direction::SW, MapPos { -1,  1 } },
    { Direction::S,  MapPos {  0,  1 } },
    { Direction::SE, MapPos {  1,  1 } },
    { Direction::W,  MapPos { -1,  0 } },
    { Direction::E,  MapPos {  1,  0 } },
    { Direction::NW, MapPos { -1, -1 } },
    { Direction::N,  MapPos {  0, -1 } },
    { Direction::NE, MapPos {  1, -1 } },
};

}

#endif