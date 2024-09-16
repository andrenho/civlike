#include "../game.hh"

#include <ranges>
namespace r = std::ranges;

namespace cl {

void Game::unit_move(Unit::Id unit_id, Direction dir)
{
    Unit& unit = units_[unit_id];
    UnitType const& unit_type = ruleset.unit_types[unit.unit_type_id];

    // check if destination is outside of bounds
    Point dest = unit.pos + directions.at(dir);
    if (dest.x < 0 || dest.y < 0 || dest.x >= map_size().w || dest.y >= map_size().h)
        return;

    // is terrain impassable?
    unsigned int moves_to_enter = tile_moves_to_enter(unit.pos + directions.at(dir));
    if (moves_to_enter == Terrain::Impassable)
        return;

    // is it water or land
    Terrain dest_terrain = ruleset.terrains[tiles_[dest.x][dest.y].terrain_id];
    if (unit_type.ship && !dest_terrain.water)
        return;
    if (!unit_type.ship && dest_terrain.water)
        return;

    // ok, move unit
    if (unit.moves_left >= moves_to_enter) {
        Point src_pos = unit.pos;
        unit.moves_left -= moves_to_enter;
        unit.pos = unit.pos + directions.at(dir);
        visual_cues_.push(MoveUnit { .unit_id = unit.id, .src_pos = src_pos, .direction = dir });

        // no moves left
    } else {
        unit.moves_left = 0;
    }

    // focus next unit
    if (unit.moves_left <= 0)
        focus_next(unit.nation_id);
}

void Game::unit_change_state(Unit::Id unit_id, Unit::State state)
{
    Unit& unit = units_[unit_id];
    unit.state = state;
    if (state != Unit::State::Normal)
        focus_next(unit.nation_id);
}

}