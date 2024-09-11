#include <cstdint>
#include <string>
#include <vector>

export module ruleset;

import geometry;

export struct Color {
    const uint8_t r, g, b;
};

export struct Terrain {
    const std::string name;
    const Color       color;
};

export struct Nation {
    const std::string name;
    const Color       color;
};

export struct UnitType {
    const std::string name;
    const char        char_display;
};

export struct StartingUnit {
    const Nation*   nation;
    const UnitType* unit_type;
    const Pos       pos;
};

export struct Ruleset {

    // static rules
    const std::vector<Terrain>  terrains;
    Terrain*                    default_terrain;
    const std::vector<Nation>   nations;
    const std::vector<UnitType> unit_types;

};