#include "command.hh"

#include <variant>

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

void move_unit_to_city_tile(Game& G, Unit::Id unit_id, City::Id city_id, MapPos tile_pos, std::optional<Good::Id> production)
{
    Unit& unit = G.units[unit_id];
    City const& city = G.cities[city_id];

    // check if unit is in city tile
    if (unit.pos != city.pos)
        return;

    // check tile position
    if (tile_pos.x < (city.pos.x - 1) || tile_pos.x > (city.pos.x + 1) || tile_pos.y < (city.pos.y - 1) || tile_pos.y > (city.pos.y + 1))
        return;
    if (tile_pos == city.pos)
        return;

    // is there another unit in that tile
    for (auto const& [_, ounit]: G.units) {
        if (unit.workplace)
            if (FieldWork const* fw = std::get_if<FieldWork>(&*unit.workplace); fw && fw->pos == tile_pos)
                return;
    }

    auto default_good = G.terrain(tile_pos).default_good;
    unit.workplace = FieldWork { .pos = tile_pos, .production = production.value_or(default_good) };
}


}