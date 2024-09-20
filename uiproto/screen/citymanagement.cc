#include "citymanagement.hh"

#include "../resources.hh"
#include "mainmap.hh"

void CityManagement::screen_event(cl::Game& G, SDL_Event* e)
{

}

void CityManagement::draw(cl::Game const& G) const
{
    res.main_map->draw(G);
    draw_city(G);
}

void CityManagement::draw_city(cl::Game const& G) const
{
    static constexpr int BORDER = 30;
    static constexpr int THICKNESS = 3;
    static constexpr int SHADOW = 8;

    int wh, ww;
    SDL_GetWindowSize(res.window, &ww, &wh);

    SDL_Rect r { BORDER + SHADOW, BORDER + SHADOW, ww - (BORDER * 2), wh - (BORDER * 2) };
    SDL_SetRenderDrawColor(res.ren, 128, 128, 128, 128);
    SDL_RenderFillRect(res.ren, &r);

    r.x -= SHADOW, r.y -= SHADOW;
    SDL_SetRenderDrawColor(res.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(res.ren, &r);

    r.x += THICKNESS, r.y += THICKNESS, r.w -= THICKNESS*2, r.h -= THICKNESS*2;
    SDL_SetRenderDrawColor(res.ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(res.ren, &r);
}
