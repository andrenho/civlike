#ifndef FNVAL_HH
#define FNVAL_HH

#include <functional>
#include <optional>
#include <stdexcept>

namespace cl {

template <typename T, typename... P>
struct FnValue {
    using Function = std::function<T(P...)>;

    FnValue() = default;
    FnValue(T t) : value_(t) {}              // NOLINT(*-explicit-constructor)
    FnValue(Function f) : function_(f) {}    // NOLINT(*-explicit-constructor)

    T operator()(P... args) const {
        auto o_value = optional(args...);
        if (!o_value.has_value())
            throw std::runtime_error("Field is mandatory");
        return *o_value;
    }

    std::optional<T> optional(P... args) const {
        if (value_.has_value())
            return value_;
        if (function_.has_value())
            return (*function_)(args...);
        return {};
    }

private:
    std::optional<T> value_;
    std::optional<Function> function_;
};

#define EXTERN_FNVALUE(KLASS, ...) \
template <typename T> \
struct KLASS : FnValue<T, __VA_ARGS__> { \
using FnValue<T, __VA_ARGS__>::FnValue; \
};

EXTERN_FNVALUE(GFnValue, struct Ruleset const&, struct GameParameters const&)

#undef EXTERN_FNVALUE

}

#endif //FNVAL_HH
