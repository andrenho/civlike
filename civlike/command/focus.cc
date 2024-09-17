#include "command.hh"

namespace cl::cmd {

static bool unit_can_focus(Game const& G, Unit const& unit)
{
    (void) G;
    return unit.moves_left > 0 && unit.state == Unit::State::Normal;
}

void focus_next(Game& G, Nation::Id nation_id, bool auto_end_round)
{
    auto& nation = G.nations[nation_id];

    auto o_unit_id = G.units.circular_next_id(nation.focused_unit,
        [&G, nation_id](Unit const& u) { return u.nation_id == nation_id && unit_can_focus(G, u); });

    if (o_unit_id) {
        nation.focused_unit = *o_unit_id;
    } else {
        nation.focused_unit = {};
        if (auto_end_round)
            end_round(G, nation_id, auto_end_round);
    }
}

void focus_unit(Game& G, Unit::Id unit_id, bool auto_end_round)
{
    Unit& unit = G.units[unit_id];
    if (unit_can_focus(G, unit)) {
        G.nations[unit.nation_id].focused_unit = unit_id;
        unit.state = Unit::State::Normal;
    } else {
        focus_next(G, unit.nation_id, auto_end_round);
    }
}

}