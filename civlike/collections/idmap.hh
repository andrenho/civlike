#ifndef HASHMAP_HH_
#define HASHMAP_HH_

#include "id.hh"

namespace cl {

template <HasId T>
struct IdMap {
    IdMap(std::initializer_list<T> const& ts) {
        for (auto& t: ts)
            hashmap_.emplace(t.id, std::move(t));
    }

    T const& operator[](typename T::Id id) const {
        return hashmap_.at(id);
    }

private:
    std::unordered_map<typename T::Id, T, IdHash<T>> hashmap_;
};

}

#endif