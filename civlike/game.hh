#ifndef GAME_HH_
#define GAME_HH_

#include <lua.hpp>

#include <string>
#include <vector>
#include "lua/types.hh"

namespace cl {

struct Game {
    struct Map {
        Size size;
        std::vector<std::string> terrain {};
    };

    struct Unit {
        Id nation;
        Id type;
        Position pos;
    };

    Map map;
    std::vector<Unit> units;

    void push_lua_table(lua_State* L) const;
};

}

#endif