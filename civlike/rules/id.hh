#ifndef ID_HH
#define ID_HH

template <typename T>
struct Id {
    T id;

    Id(T id) : id(id) {}

    operator T() const { return id; }
    bool operator==(Id const& other) const { return id == other.id; }

    template <std::derived_from<Id> U>
    U next() { return U { ++id }; }
};

#endif //ID_HH
