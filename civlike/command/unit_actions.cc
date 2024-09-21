#include "command.hh"

namespace cl::cmd {

static unsigned int tile_moves_to_enter(Game const& G, MapPos p)
{
    return G.ruleset.terrains[G.tiles[p.x][p.y].terrain_id].cost_to_enter;
}

void move_unit(Game& G, Unit::Id unit_id, Direction dir)
{
    Unit& unit = G.units[unit_id];
    UnitType const& unit_type = G.ruleset.unit_types[unit.unit_type_id];

    // check if destination is outside of bounds
    MapPos dest = unit.pos + directions.at(dir);
    if (dest.x < 0 || dest.y < 0 || dest.x >= G.map_size.w || dest.y >= G.map_size.h)
        return;

    // is terrain impassable?
    unsigned int moves_to_enter = tile_moves_to_enter(G, unit.pos + directions.at(dir));
    if (moves_to_enter == Terrain::Impassable)
        return;

    // is it water or land
    Terrain dest_terrain = G.ruleset.terrains[G.tiles[dest.x][dest.y].terrain_id];
    if (unit_type.ship && !dest_terrain.water)
        return;
    if (!unit_type.ship && dest_terrain.water)
        return;

    // ok, move unit
    if (unit.moves_left >= moves_to_enter) {
        MapPos src_pos = unit.pos;
        unit.moves_left -= moves_to_enter;
        unit.pos = unit.pos + directions.at(dir);
        G.visual_cues.push(MoveUnit { .unit_id = unit.id, .src_pos = src_pos, .direction = dir });

        // no moves left
    } else {
        unit.moves_left = 0;
    }

    // focus next unit
    if (unit.moves_left <= 0)
        focus_next(G, unit.nation_id, true);
}

void fortify_unit(Game& G, Unit::Id unit_id)
{
    Unit& unit = G.units[unit_id];
    unit.state = Unit::State::Fortify;
    focus_next(G, unit.nation_id, true);
}

}