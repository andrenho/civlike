#include "citymanagement.hh"

#include "../resources.hh"
#include "mainmap.hh"

static constexpr int START = 50;
static constexpr int AREA_H = 70;

void CityManagement::screen_event(cl::Game& G, SDL_Event* e)
{

}

void CityManagement::set_city(cl::Game const& G, cl::City const* city)
{
    city_ = city;
    reset_hotspots(G);
}

void CityManagement::reset_hotspots(cl::Game const& G)
{
    hotspots_.clear();

    // out of city

    constexpr int OUT_OF_CITY_Y = START + 420;

    hotspots_.push_back(HotSpotDraggableTo {
        .to = HotSpotArea::OutOfCity,
        .rect = { START, OUT_OF_CITY_Y, 600, AREA_H },
        .name = "Out of City",
    });

    int x = START + 10;
    for (auto unit: G.units_in_xy(city_->pos)) {
        hotspots_.push_back(HotSpotDraggableFrom {
            .from = unit->id,
            .point = { x, OUT_OF_CITY_Y + 30 },
        });
        x += 40;
    }
}

void CityManagement::screen_draw(cl::Game const& G) const
{
    res.main_map->screen_draw(G);
    draw_city(G);
    draw_hotspots(G);
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
