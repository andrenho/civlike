#ifndef GAME_HH_
#define GAME_HH_

#include <lua.hpp>

namespace cl {

struct Game {
    void push_lua_table(lua_State* L) const;
};

}

#endif