#ifndef LUAOBJECT_HH_
#define LUAOBJECT_HH_

#include <lua.hpp>
#include <string>

namespace cl {

struct LuaObject {
    LuaObject(lua_State* L, std::string const& table_path) : L(L), table_path(table_path) {}

protected:
    void execute_path() const;

    lua_State* L;
    std::string table_path;
};

}

#endif