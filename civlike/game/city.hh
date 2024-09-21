#ifndef CITY_HH
#define CITY_HH

#include <string>

#include "rules/ruleset.hh"

namespace cl {

struct City {
    ID(uint16_t)
    std::string name;
    Nation::Id  nation_id;
    MapPos       pos;

    [[nodiscard]] static Id next_id() {
        static Id id_counter { 0 };
        return id_counter.next<Id>();
    }
};

}

#endif //CITY_HH
