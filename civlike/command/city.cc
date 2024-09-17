#include "command.hh"

namespace cl::cmd {

void build_city(Game& G, Unit::Id builder_id, std::string const& name)
{
    Unit const& unit = G.units[builder_id];

    G.cities.emplace(City {
        .name = name,
        .nation_id = unit.nation_id,
        .pos = unit.pos,
    });
}

}