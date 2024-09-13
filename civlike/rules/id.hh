#ifndef ID_HH
#define ID_HH

template <typename T>
struct Id {
    T id;

    operator T() const { return id; }
    bool operator==(Id const& other) const { return id == other.id; }
};

#endif //ID_HH
