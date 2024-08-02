#ifndef GAME_HH_
#define GAME_HH_

#include <lua.hpp>

#include <cstdint>

#include <map>
#include <optional>
#include <unordered_map>
#include <string>
#include <vector>
#include "lua/types.hh"

namespace cl {

struct Game {
    struct Map {
        Size                     size;
        std::vector<std::string> terrain {};
    };

    struct Unit {
        Id       nation;
        Id       type;
        Position pos;
        uint8_t  moves_left;
    };

    Map                    map;
    std::map<size_t, Unit> units;
    std::optional<size_t>  focused_unit;

    size_t last_unit_id = 0;

    void push_lua_table(lua_State* L) const;
};

}

#endif