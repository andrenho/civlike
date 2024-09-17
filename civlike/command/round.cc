#include "command.hh"

#include <ranges>
namespace r = std::ranges;

namespace cl::cmd {

static size_t unit_starting_moves(Game const& G, Unit const& unit)
{
    return G.ruleset.unit_types[unit.unit_type_id].move_points;
}

void next_round(Game& G)
{
    for (auto& [_, unit]: G.units)
        unit.moves_left = unit_starting_moves(G, unit);
    for (auto& [_, nation]: G.nations) {
        nation.round_ended = false;
        nation.focused_unit = {};
        focus_next(G, nation.id, false);
    }
    ++G.round_nr;
}

void end_round(Game& G, Nation::Id nation_id, bool auto_new_round)
{
    G.nations[nation_id].round_ended = true;

    size_t nations_ended = r::count_if(G.nations, [](auto const& pair) { return pair.second.round_ended; });
    if (auto_new_round && nations_ended == G.nations.size())
        next_round(G);
}

}