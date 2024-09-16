#ifndef COUNTERMAP_HH
#define COUNTERMAP_HH

#include <algorithm>
#include <functional>
#include <map>

#include "id.hh"

namespace cl {

template <HasId T>
struct CounterMap {
    void emplace(T t) {
        t.id = T::next_id();
        map_.emplace(t.id, t);
    }

    std::optional<T const*> find_first_value(std::function<bool(T const&)> f) const {
        for (auto const& [_, v]: map_)
            if (f(v))
                return &v;
        return {};
    }

    T const& operator[](typename T::Id id) const {
        return map_.at(id);
    }

    T& operator[](typename T::Id id) {
        return map_.at(id);
    }

    std::optional<typename T::Id> circular_next_id(std::optional<typename T::Id> initial_id, std::function<bool(T const&)> f) const
    {
        auto check_function = [f](auto const& pair) { return f(pair.second); };

        // find initial id
        typename std::map<typename T::Id, T>::const_iterator it;
        if (!initial_id) {
            it = std::find_if(map_.begin(), map_.end(), check_function);
            if (it != map_.end())
                return it->first;
        } else {
            it = map_.find(*initial_id);
        }

        auto initial_it = it;

        // search from initial id to the end
        it = std::find_if(++it, map_.end(), check_function);
        if (it != map_.end())
            return it->first;

        // search from the start to the initial id
        it = std::find_if(map_.begin(), initial_it, check_function);
        if (it == initial_it) {
            if (check_function(*it))
                return it->first;
            else
                return {};
        }

        return it->first;
    }

private:
    std::map<typename T::Id, T> map_;

    // iterators
public:
    using value_type = typename std::map<typename T::Id, T>::value_type;
    auto begin() -> decltype(map_.begin()) { return map_.begin(); }
    auto end() -> decltype(map_.end()) { return map_.end(); }
    auto begin() const -> decltype(map_.begin()) { return map_.begin(); }
    auto end() const -> decltype(map_.end()) { return map_.end(); }
};

}

#endif //COUNTERMAP_HH
