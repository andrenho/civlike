#ifndef LUAVALUE_HH_
#define LUAVALUE_HH_

#include <cstdint>

#include <optional>
#include <string>

#include <lua.hpp>

#include "game.hh"

#define VALUE(name) [[nodiscard]] LuaValue name() const { lua_getfield(L, -1, #name); return LuaValue(L); }
#define STRUCT(Type, name) [[nodiscard]] Type name() const { lua_getfield(L, -1, #name); return Type(L); }
#define TABLE(Type, name) [[nodiscard]] LuaTable<Type> name() const { lua_getfield(L, -1, #name); if (lua_isnil(L, -1)) luaL_error(L, "Table '" #name "' not found"); return LuaTable<Type>(L); }

namespace cl {

struct Size {
    lua_Integer w, h;
};

struct Position {
    lua_Integer x, y;
};

struct Color {
    uint8_t r, g, b;
};

struct LuaObject {
    explicit LuaObject(lua_State* L) : L(L) {}

protected:
    lua_State* L;
};

struct LuaValue : public LuaObject {
    using LuaObject::LuaObject;

    [[nodiscard]] std::optional<lua_Integer> opt_integer(std::optional<Game> const& game={}) const;
    [[nodiscard]] std::optional<std::string> opt_string(std::optional<Game> const& game={}) const;
    [[nodiscard]] std::optional<std::string> opt_id(std::optional<Game> const& game={}) const;
    [[nodiscard]] std::optional<Size> opt_size(std::optional<Game> const& game={}) const;
    [[nodiscard]] std::optional<Position> opt_position(std::optional<Game> const& game={}) const;
    [[nodiscard]] std::optional<Color> opt_color(std::optional<Game> const& game={}) const;

#define NON_OPT(Type, name) [[nodiscard]] Type name(std::optional<Game> const& game={}) const { std::optional<Type> t = opt_ ## name(game); if (!t.has_value()) luaL_error(L, "Field of '" #name "' type missing mandatory value."); return *t; }
    NON_OPT(lua_Integer, integer)
    NON_OPT(std::string, string)
    NON_OPT(std::string, id)
    NON_OPT(Size, size)
    NON_OPT(Position, position)
    NON_OPT(Color, color)
#undef NON_OPT

private:
    void execute_if_function(std::optional<Game> const& game) const;
};

template <typename T>
struct LuaTable : public LuaObject {
    using LuaObject::LuaObject;

    T operator[](lua_Integer key) const {
        lua_geti(L, -1, key + 1);
        return T(L);
    }

    T operator[](std::string const& key) const {
        lua_getfield(L, -1, key.c_str());
        return T(L);
    }

    lua_Integer size() const {
        return luaL_len(L, -1);
    }
};

}

#endif