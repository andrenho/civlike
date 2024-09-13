#include "game.hh"

#include <ranges>
namespace r = std::ranges;

bool Game::move_focused_unit(Nation::Id nation_id, Direction dir)
{
    auto focused_it = r::find_if(units_, [nation_id](Unit const& u) -> bool { return u.nation_id == nation_id && u.focused; });
    if (focused_it == units_.end())
        return false;


    return true;
}
