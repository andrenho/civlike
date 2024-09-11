#include "rules.hh"

#include <stdexcept>

namespace cl {

Rules::Rules(std::string const &lua_rules)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_loadstring(L, lua_rules.c_str()) != LUA_OK) {
        throw std::runtime_error("Syntax error in Lua rules file.");
    }

    lua_call(L, 0, 1);
    lua_setglobal(L, "__rules");
}

Rules::~Rules()
{
    lua_close(L);
}

BaseRules Rules::operator()() const
{
    return BaseRules(L, "$__rules");
}

}