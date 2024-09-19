#include "ui.hh"

void UI::do_events(cl::Game& G)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q))
            running_ = false;
        else
            res_.current_screen->do_event(G, &e);
    }
}

void UI::draw(cl::Game const& G) const
{
    res_.current_screen->draw(G);
    SDL_RenderPresent(res_.ren);
}