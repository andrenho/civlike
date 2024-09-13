#ifndef VISUALCUE_HH
#define VISUALCUE_HH

#include "common/geometry.hh"
#include "direction.hh"

struct MoveUnit {
    size_t    unit_idx;
    Point     src_pos;
    Direction direction;
};

using VisualCue = std::variant<MoveUnit>;

#endif //VISUALCUE_HH
