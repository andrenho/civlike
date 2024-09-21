#ifndef COMMAND_HH
#define COMMAND_HH

#include "game/game.hh"

namespace cl::cmd {

// new game

Game new_game(Ruleset const& ruleset, GameParameters const& game_par);

// round

void next_round(Game& G);
void end_round(Game& G, Nation::Id nation_id, bool auto_new_round);

// focus

void focus_next(Game& G, Nation::Id nation_id, bool auto_end_round);
void focus_unit(Game& G, Unit::Id unit_id, bool auto_end_round);

// unit actions

void move_unit(Game& G, Unit::Id unit_id, Direction dir);
void move_unit_to_city_tile(Game& G, Unit::Id, City::Id city_id, MapPos tile_pos, std::optional<Good::Id> production={});
void fortify_unit(Game& G, Unit::Id unit_id);

// city

void build_city(Game& G, Unit::Id builder_id, std::string const& name);

}

#endif //COMMAND_HH
