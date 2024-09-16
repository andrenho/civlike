#ifndef COUNTERMAP_HH
#define COUNTERMAP_HH

#include "id.hh"

namespace cl {

template <HasId T>
struct CounterMap {
    void emplace(T const& t) {
        hashmap_.emplace(t.id, t);
    }

    std::optional<T const*> find_first_value(std::function<bool(T const&)> f) const {
        for (auto const& [_, v]: hashmap_)
            if (f(v))
                return &v;
        return {};
    }

    T const& operator[](typename T::Id id) const {
        return hashmap_.at(id);
    }

    T& operator[](typename T::Id id) {
        return hashmap_.at(id);
    }

private:
    std::unordered_map<typename T::Id, T, IdHash<T>> hashmap_;

    // iterators
public:
    using value_type = typename std::map<typename T::Id, T>::value_type;
    auto begin() -> decltype(hashmap_.begin()) { return hashmap_.begin(); }
    auto end() -> decltype(hashmap_.end()) { return hashmap_.end(); }
    auto begin() const -> decltype(hashmap_.begin()) { return hashmap_.begin(); }
    auto end() const -> decltype(hashmap_.end()) { return hashmap_.end(); }
};

}

#endif //COUNTERMAP_HH
