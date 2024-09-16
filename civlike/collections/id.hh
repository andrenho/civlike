#ifndef ID_HH
#define ID_HH

#include <functional>
#include <memory>
#include <optional>

namespace cl {

template <typename T>
struct Id {
    T id;

    Id(T id) : id(id) {}

    operator T() const { return id; }
    auto operator<=>(Id const& other) const = default;

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

#define ID(TYPE) struct Id : cl::Id<TYPE> {} id = { 0 };

#endif //ID_HH
