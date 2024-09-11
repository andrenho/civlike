#include "luaobject.hh"

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
            lua_geti(L, -1, std::strtol(std::string(token.substr(1)).c_str(), nullptr, 10) + 1);
        } else {
            lua_getfield(L, -1, std::string(token).c_str());
        }

        if (lua_isnil(L, -1))
            luaL_error(L, "Field '%s' not found.", table_path.c_str());
    }
}

}