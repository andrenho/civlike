#include "screen.hh"

void Screen::do_event(cl::Game& G, SDL_Event* e)
{
    screen_event(G, e);
}

void Screen::draw_hotspots(cl::Game& G)
{
}
