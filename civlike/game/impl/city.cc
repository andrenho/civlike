#include "../game.hh"

namespace cl {

void Game::city_build(Unit::Id unit_id, std::string const& name)
{
    Unit const& unit = units_[unit_id];

    City::Id city_id = City::next_id();
    cities_.emplace(City {
        .id = city_id,
        .name = name,
        .nation_id = unit.nation_id,
        .pos = unit.pos,
    });
}

}