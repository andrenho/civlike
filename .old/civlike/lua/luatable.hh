#ifndef LUATABLE_HH_
#define LUATABLE_HH_

namespace cl {

template <typename T>
struct LuaTable : public LuaObject {

    using LuaObject::LuaObject;

    T operator[](Id key) const {
        return T(L, table_path + ".@" + key.value);
    }

    T operator[](ssize_t key) const {
        return T(L, table_path + ".#" + std::to_string(key));
    }

    T operator[](std::string const& key) const {
        return T(L, table_path + ".@" + key);
    }

    [[nodiscard]] size_t size() const {
        execute_path();
        return (size_t) luaL_len(L, -1);
    }

    template <typename F>
    void foreach_id(F f) {
        execute_path();

        std::vector<Id> key_list;
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            key_list.emplace_back(lua_tostring(L, -2)[0]);
            lua_pop(L, 1);
        }

        for (Id key: key_list)
            f(key, this->operator[](key));
    }

    template <typename F>
    void foreach_key(F f) {
        execute_path();

        std::vector<std::string> key_list;
        lua_pushnil(L);
        while (lua_next(L, -2) != 0) {
            key_list.emplace_back(lua_tostring(L, -2));
            lua_pop(L, 1);
        }

        for (std::string key: key_list)
            f(key, this->operator[](key));
    }

    template <typename F>
    void foreach_array(F f) {
        execute_path();
        lua_Integer n = luaL_len(L, -1);
        for (lua_Integer i = 0; i < n; ++i)
            f(this->operator[](i));
    }
};

}

#endif
