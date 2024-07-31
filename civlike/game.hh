#ifndef GAME_HH_
#define GAME_HH_

#include <lua.hpp>

#include <string>
#include <vector>

namespace cl {

struct Game {
    struct Map {
        size_t w, h;
        std::vector<std::string> terrain {};
    };

    struct Unit {
        char nation;
        char type;
        size_t x, y;
    };

    Map map;
    std::vector<Unit> units;

    void push_lua_table(lua_State* L) const;
};

}

#endif