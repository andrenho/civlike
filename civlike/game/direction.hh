#ifndef GAME_DIRECTION_HH
#define GAME_DIRECTION_HH

#include <unordered_map>

#include "common/geometry.hh"

namespace cl {

enum class Direction {
    NW, N, NE, W, E, SW, S, SE,
};

const inline std::unordered_map<Direction, Point> directions = {
    { Direction::SW, Point { -1,  1 } },
    { Direction::S,  Point {  0,  1 } },
    { Direction::SE, Point {  1,  1 } },
    { Direction::W,  Point { -1,  0 } },
    { Direction::E,  Point {  1,  0 } },
    { Direction::NW, Point { -1, -1 } },
    { Direction::N,  Point {  0, -1 } },
    { Direction::NE, Point {  1, -1 } },
};

}

#endif