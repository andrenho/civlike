#include "luavalue.hh"

namespace cl {

lua_Integer LuaValue::integer(std::optional<Game> const& game) const
{
    execute_if_function(game);
    lua_Integer value = luaL_checkinteger(L, -1);
    lua_settop(L, 0);
    return value;
}

std::string LuaValue::string(std::optional<Game> const& game) const
{
    execute_if_function(game);
    const char* str = luaL_checkstring(L, -1);
    lua_settop(L, 0);
    return str;
}

std::string LuaValue::id(std::optional<Game> const& game) const
{
    std::string r = this->string(game);
    if (r.length() != 1)
        luaL_error(L, "Ids should have a single character.");
    return r;
}

Size LuaValue::size(std::optional<Game> const &game) const
{
    execute_if_function(game);
    luaL_checktype(L, -1, LUA_TTABLE);
    Size size {};
    lua_geti(L, -1, 1); size.w = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_geti(L, -1, 2); size.h = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return size;
}

Position LuaValue::position(std::optional<Game> const &game) const
{
    execute_if_function(game);
    luaL_checktype(L, -1, LUA_TTABLE);
    Position pos {};
    lua_geti(L, -1, 1); pos.x = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_geti(L, -1, 2); pos.y = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return pos;
}

Color LuaValue::color(std::optional<Game> const& game) const
{
    execute_if_function(game);
    std::string scolor = this->string(game);
    if (scolor.size() != 7 || scolor[0] != '#')
        luaL_error(L, "Invalid color format: %s", scolor.c_str());

    Color c {};
    c.r = std::strtoul(scolor.substr(1, 2).c_str(), nullptr, 16);
    c.g = std::strtoul(scolor.substr(3, 2).c_str(), nullptr, 16);
    c.b = std::strtoul(scolor.substr(5, 2).c_str(), nullptr, 16);
    return c;
}

void LuaValue::execute_if_function(std::optional<Game> const& game) const
{
    if (lua_type(L, -1) == LUA_TFUNCTION) {
        lua_getglobal(L, "__rules");
        if (game.has_value())
            game->push_lua_table(L);
        else
            lua_pushnil(L);
        lua_call(L, 2, 1);
    }
}

}