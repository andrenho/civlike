#include "luavalue.hh"

#include <ranges>
#include <string_view>

namespace cl {

void LuaObject::execute_path() const
{
    auto split = table_path
                 | std::ranges::views::split('.')
                 | std::ranges::views::transform([](auto&& str) { return std::string_view(&*str.begin(), std::ranges::distance(str)); });

    for (auto&& token : split) {

        if (token.starts_with('$')) {
            lua_getglobal(L, std::string(token.substr(1)).c_str());
        } else if (token.starts_with('@')) {
            lua_getfield(L, -1, std::string(token.substr(1)).c_str());
        } else if (token.starts_with('#')) {
            lua_geti(L, -1, std::strtol(std::string(token.substr(1)).c_str(), nullptr, 10));
        } else {
            lua_getfield(L, -1, std::string(token).c_str());
        }

        if (lua_isnil(L, -1))
            luaL_error(L, "Field '%s' not found.", table_path.c_str());
    }
}

std::optional<lua_Integer> LuaValue::opt_integer(std::optional<Game> const& game) const
{
    execute_path();
    execute_if_function(game);
    if (lua_isnil(L, -1))
        return {};
    lua_Integer value = luaL_checkinteger(L, -1);
    lua_settop(L, 0);
    return value;
}

std::optional<std::string> LuaValue::opt_string(std::optional<Game> const& game) const
{
    execute_path();
    execute_if_function(game);
    if (lua_isnil(L, -1))
        return {};
    const char* str = luaL_checkstring(L, -1);
    lua_settop(L, 0);
    return str;
}

char LuaValue::id(std::optional<Game> const& game) const
{
    auto r = this->string(game);
    if (r.length() != 1)
        luaL_error(L, "Ids should have a single character.");
    return r.at(0);
}

std::optional<Size> LuaValue::opt_size(std::optional<Game> const &game) const
{
    execute_path();
    execute_if_function(game);
    if (lua_isnil(L, -1))
        return {};
    luaL_checktype(L, -1, LUA_TTABLE);
    Size size {};
    lua_geti(L, -1, 1); size.w = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_geti(L, -1, 2); size.h = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return size;
}

std::optional<Position> LuaValue::opt_position(std::optional<Game> const &game) const
{
    execute_path();
    execute_if_function(game);
    if (lua_isnil(L, -1))
        return {};
    luaL_checktype(L, -1, LUA_TTABLE);
    Position pos {};
    lua_geti(L, -1, 1); pos.x = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_geti(L, -1, 2); pos.y = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return pos;
}

std::optional<Color> LuaValue::opt_color(std::optional<Game> const& game) const
{
    execute_path();
    execute_if_function(game);
    if (lua_isnil(L, -1))
        return {};
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