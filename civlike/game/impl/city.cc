#include "../game.hh"

namespace cl {

void Game::city_build(Unit::Id unit_id, std::string const& name)
{
    Unit const& unit = units_[unit_id];

    cities_.emplace(City {
        .name = name,
        .nation_id = unit.nation_id,
        .pos = unit.pos,
    });
}

}