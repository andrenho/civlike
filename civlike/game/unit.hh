#ifndef UNIT_HH
#define UNIT_HH

#include "common/geometry.hh"
#include "rules/ruleset.hh"

namespace cl {

struct Unit {
    enum class State { Normal, Fortify };

    ID(uint32_t)
    Nation::Id    nation_id;
    UnitType::Id  unit_type_id;
    MapPos         pos;
    unsigned long moves_left = 0;
    State         state = State::Normal;

    explicit Unit(StartingUnit const& su)
        : nation_id(su.nation_id), unit_type_id(su.unit_type_id), pos(su.initial_pos) {}

    [[nodiscard]] static Id next_id() {
        static Id id_counter { 0 };
        return id_counter.next<Id>();
    }
};

}

#endif //UNIT_HH
