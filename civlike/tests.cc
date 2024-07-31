#include <string>
#include <sstream>

#include "civlike.hh"

std::string sample_rules = R"(

return {

    map_size = { 10, 20 },

    terrains = {
       G = { name = "Grassland", color = "#a5f5b3" },
    },

	default_terrain = "G",

	nations = {
		B = { name = "Brazil", color = "#990000" },
	},

	players = {
		{
			nation = "B",
			initial_units = {
				type = "C", position = { 5, 5 },
			},
		},
	},

	unit_types = {
		C = { name = "Colonist", moves_per_round = 1 },
	}

}

)";

#define ASSERT(a, b) { if ((a) != (b)) { std::stringstream ss; ss << "Assertion failed: " #a " == " #b ", found " << a; throw std::runtime_error(ss.str()); } }

int main()
{
    cl::Rules rules(sample_rules);
    cl::Game game = cl::create_game(rules);

    ASSERT(rules().default_terrain().id(), 'G')

    cl::Size size = rules().map_size().size(game);
    ASSERT(size.w, 10)
    ASSERT(size.h, 20)

    ASSERT(rules().terrains()["G"].name().string(), "Grassland");
    ASSERT(rules().players()[0].nation().id(), 'B');
    /*
    ASSERT(rules().nations()["B"].color().color(game).r, 0x99);
    ASSERT(rules().players().size(), 1);
     */
}