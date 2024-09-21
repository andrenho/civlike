#ifndef COMMON_GEOMETRY_HH_
#define COMMON_GEOMETRY_HH_

#include <cstddef>

namespace cl {

struct MapPos {
    int x, y;

    bool operator==(MapPos const& p) const { return p.x == x && p.y == y; }
    bool operator!=(MapPos const& p) const { return !(*this == p); }
    MapPos operator+(MapPos const& p) const { return { x + p.x, y + p.y } ;};
    MapPos operator*(int v) const { return { x * v, y * v } ;};
    MapPos operator-() const { return { -x, -y }; }
};

struct Size {
    size_t w, h;
};

}

#endif