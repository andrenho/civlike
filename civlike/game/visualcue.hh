#ifndef VISUALCUE_HH
#define VISUALCUE_HH

#include <variant>

#include "common/geometry.hh"
#include "direction.hh"

struct MoveUnit {
    Unit::Id  unit_id;
    Point     src_pos;
    Direction direction;
};

using VisualCue = std::variant<MoveUnit>;

#endif //VISUALCUE_HH
