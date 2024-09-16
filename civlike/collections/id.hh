#ifndef ID_HH
#define ID_HH

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

namespace cl {

template <typename T>
struct Id {
    T id;

    Id(T id) : id(id) {}

    operator T() const { return id; }
    bool operator==(Id const& other) const { return id == other.id; }

    template <std::derived_from<Id> U>
    U next() { return U { ++id }; }
};

template <typename T>
struct IdHash {
    std::size_t operator()(typename T::Id const& k) const {
        return std::hash<decltype(k.id)>()(k.id);
    }
};

template <typename T>
concept HasId = requires(T t) {
    typename T::Id;
    std::is_class_v<typename T::Id>;
    { t.id } -> std::convertible_to<typename T::Id>;
};

}

#define ID(TYPE) struct Id : cl::Id<TYPE> {} id;

#endif //ID_HH
