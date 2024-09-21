#ifndef RULES_RULESET_HH_
#define RULES_RULESET_HH_

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "fnval.hh"
#include "common/color.hh"
#include "common/geometry.hh"
#include "collections/id.hh"
#include "collections/idmap.hh"


namespace cl {

struct Good {
    ID(char)
    std::string name;
    double      initial_value;
    enum class Type { Physical, Conceptual } type = Type::Physical;
};

struct TerrainProduction {
    Good::Id good_id;
    int      production;
};

struct Terrain {
    ID(char)
    std::string                    name;
    Color                          color;
    unsigned int                   cost_to_enter;
    std::vector<TerrainProduction> production;
    bool                           water = false;

    static constexpr decltype(cost_to_enter) Impassable = std::numeric_limits<decltype(cost_to_enter)>::max();
};

struct BuildingProduction {
    std::vector<Good::Id> goods_required;
    Good::Id              good_produced;
    double                rate = 1.0;
};

struct Building {
    ID(char)
    std::string           name;
    uint8_t               max_units;
    BuildingProduction            production;
    uint8_t               productivity;
    bool                  default_ = false;
};

struct UnitType {
    ID(char)
    std::string  name;
    char         char_display;
    unsigned int move_points;
    bool         ship = false;
};

struct Nation {
    ID(char)
    std::string name;
    Color       color;
    bool        playable = true;
};

struct Test {
    struct Id : cl::Id<char> {} id;
    std::string name;
};

struct StartingUnit {
    Nation::Id   nation_id;
    UnitType::Id unit_type_id;
    MapPos        initial_pos;
};

struct TerrainSquare {
    Terrain::Id terrain_id;
};
using Map = std::vector<std::vector<TerrainSquare>>;

struct GameParameters {};   // TODO - ???

struct Ruleset {

    const IdMap<Good>                         goods;
    const IdMap<Terrain>                      terrains;
    const IdMap<Building>                     buildings;
    const IdMap<Nation>                       nations;
    const IdMap<UnitType>                     unit_types;

    const GFnValue<Map>                       map;
    const GFnValue<std::vector<Nation::Id>>   initial_nations;
    const GFnValue<std::vector<StartingUnit>> starting_units;

    static Ruleset create_from_cpp(Ruleset&& ruleset) {
        return ruleset;
    }
};

}

#endif