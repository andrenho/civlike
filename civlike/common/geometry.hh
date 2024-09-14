#ifndef COMMON_GEOMETRY_HH_
#define COMMON_GEOMETRY_HH_

#include <cstddef>

namespace cl {

struct Point {
    int x, y;

    bool operator==(Point const& p) const { return p.x == x && p.y == y; }
    bool operator!=(Point const& p) const { return !(*this == p); }
    Point operator+(Point const& p) const { return { x + p.x, y + p.y } ;};
    Point operator*(int v) const { return { x * v, y * v } ;};
    Point operator-() const { return { -x, -y }; }
};

struct Size {
    size_t w, h;
};

}

#endif