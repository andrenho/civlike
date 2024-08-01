#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdint>
#include <lua.hpp>

namespace cl {

struct Size {
    lua_Integer w, h;
};

struct Position {
    lua_Integer x, y;
};

struct Color {
    uint8_t r, g, b;
};

}

#endif