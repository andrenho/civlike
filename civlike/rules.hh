#ifndef RULES_HH_
#define RULES_HH_

#include <string>

#include "lua/luavalue.hh"
#include "lua/luaobject.hh"
#include "lua/luatable.hh"

namespace cl {

#define OBJ : public LuaObject
#define INIT using LuaObject::LuaObject;

struct Terrain OBJ {
    INIT
    VALUE(name)
    VALUE(color)
};

struct PlayerInitialUnits OBJ {
    INIT
    VALUE(type)
    VALUE(position)
};

struct Nation OBJ {
    INIT
    VALUE(name)
    VALUE(color)
};

struct Player OBJ {
    INIT
    VALUE(nation);
    TABLE(PlayerInitialUnits, initial_units)
};

struct UnitTypes OBJ {
    INIT
    VALUE(name)
    VALUE(moves_per_round)
};

struct BaseRules OBJ {
    INIT
    VALUE(map_size)
    TABLE(Terrain, terrains)
    VALUE(default_terrain)
    TABLE(Nation, nations)
    TABLE(Player, players)
    TABLE(UnitTypes, unit_types)
};

#undef OBJ
#undef INIT

struct Rules {
    explicit Rules(std::string const& lua_rules);
    ~Rules();

    BaseRules operator()() const;

private:
    lua_State* L;
};

}

#endif