#ifndef RULES_RULESET_HH_
#define RULES_RULESET_HH_

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "common/color.hh"
#include "common/geometry.hh"
#include "id.hh"
#include "fnval.hh"

struct Terrain {
    std::string  name;
    Color        color;
    unsigned int cost_to_enter;
    bool         water = false;

    struct Id : ::Id<uint8_t> {};

    static constexpr decltype(Terrain::cost_to_enter) Impassable = std::numeric_limits<decltype(Terrain::cost_to_enter)>::max();
};

struct UnitType {
    std::string  name;
    char         char_display;
    unsigned int move_points;
    bool         ship = false;

    struct Id : ::Id<uint8_t> {};
};

struct Nation {
    std::string name;
    Color       color;
    bool        playable = true;

    struct Id : ::Id<uint8_t> {};
};

struct StartingUnit {
    Nation::Id   nation_id;
    UnitType::Id unit_type_id;
    Point        initial_pos;
};

struct TerrainSquare {
    Terrain::Id terrain_id;
};
using Map = std::vector<std::vector<TerrainSquare>>;

struct GameParameters {};   // TODO - ???

struct Ruleset {

    const std::vector<Terrain>                terrains;
    const std::vector<Nation>                 nations;
    const std::vector<UnitType>               unit_types;

    const GFnValue<Map>                       map;
    const GFnValue<std::vector<Nation::Id>>   initial_nations;
    const GFnValue<std::vector<StartingUnit>> starting_units;

    static Ruleset create_from_cpp(Ruleset&& ruleset) {
        return ruleset;
    }
};

#endif