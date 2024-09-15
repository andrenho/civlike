#ifndef CITY_HH
#define CITY_HH

#include <string>

#include "rules/ruleset.hh"

namespace cl {

struct City {
    std::string name;
    Nation::Id  nation_id;
    Point       pos;
};

}

#endif //CITY_HH
