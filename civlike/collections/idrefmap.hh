#ifndef IDREFMAP_HH_
#define IDREFMAP_HH_

#include "id.hh"

namespace cl {

template <typename IdT, typename T>
struct IdRefMap {
    T const& operator[](IdT id) const {
        return hashmap_.at(id);
    }

    T& operator[](IdT id) {
        return hashmap_.at(id);
    }

    void emplace(IdT id, T t) {
        t.id = id;
        hashmap_.emplace(t.id, t);
    }

    size_t size() const { return hashmap_.size(); }

private:
    std::unordered_map<IdT, T, IdHash<IdT>> hashmap_;

    // iterators
public:
    using value_type = typename std::unordered_map<IdT, T>::value_type;
    auto begin() -> decltype(hashmap_.begin()) { return hashmap_.begin(); }
    auto end() -> decltype(hashmap_.end()) { return hashmap_.end(); }
    auto begin() const -> decltype(hashmap_.begin()) { return hashmap_.begin(); }
    auto end() const -> decltype(hashmap_.end()) { return hashmap_.end(); }
};

}

#endif