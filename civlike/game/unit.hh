#ifndef UNIT_HH
#define UNIT_HH

#include "common/geometry.hh"
#include "rules/ruleset.hh"

struct Unit {
    struct Id : ::Id<uint32_t> {};
    enum class State { Normal, Fortify };

    Id            id;
    Nation::Id    nation_id;
    UnitType::Id  unit_type_id;
    Point         pos;
    unsigned long moves_left = 0;
    State         state = State::Normal;

    explicit Unit(Id id, StartingUnit const& su)
        : id(id), nation_id(su.nation_id), unit_type_id(su.unit_type_id), pos(su.initial_pos) {}

    [[nodiscard]] static Id next_id() {
        static Id id_counter { 0 };
        return id_counter.next<Id>();
    }
};

#endif //UNIT_HH
