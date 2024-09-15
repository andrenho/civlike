#ifndef CITY_HH
#define CITY_HH

#include <string>

#include "rules/ruleset.hh"

namespace cl {

struct City {
    struct Id : cl::Id<uint16_t> {};

    Id          id;
    std::string name;
    Nation::Id  nation_id;
    Point       pos;

    [[nodiscard]] static Id next_id() {
        static Id id_counter { 0 };
        return id_counter.next<Id>();
    }
};

}

#endif //CITY_HH
