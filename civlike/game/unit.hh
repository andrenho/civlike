#ifndef UNIT_HH
#define UNIT_HH

#include "common/geometry.hh"
#include "rules/ruleset.hh"

struct Unit {
    Nation::Id   nation_id;
    UnitType::Id unit_type_id;
    Point        pos;
    bool         focused;

    explicit Unit(StartingUnit const& su)
        : nation_id(su.nation_id), unit_type_id(su.unit_type_id), pos(su.initial_pos), focused(false) {}

    void new_round() {}

    [[nodiscard]] bool can_focus() const { return true; }
};

#endif //UNIT_HH
