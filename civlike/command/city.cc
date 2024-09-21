#include "command.hh"

namespace cl::cmd {

void build_city(Game& G, Unit::Id builder_id, std::string const& name)
{
    Unit const& unit = G.units[builder_id];

    if (unit.pos.x == 0 || unit.pos.y == 0 || unit.pos.x == (G.map_size.w - 1) || unit.pos.y == (G.map_size.h - 1))
        throw std::runtime_error("Cannot create a city this close to the edge of the map.");

    // TODO - check - is there a city already here?

    G.cities.emplace(City {
        .name = name,
        .nation_id = unit.nation_id,
        .pos = unit.pos,
    });
}

}