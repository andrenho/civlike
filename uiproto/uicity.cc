#include "uicity.hh"

#include <ranges>

#include "text.hh"
namespace r = std::ranges;

static constexpr int START = 50;

void UICity::do_events(cl::Game& G)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                SDL_Quit();
            exit(0);
        }
    }
}

void UICity::draw(cl::Game const& G)
{
    hotspots_.clear();

    cl::City const& city = G.cities[*selected_city];
    draw_city(G, city);

    auto unit_ids = std::ranges::views::transform(G.units_in_xy(city.pos), [](const cl::Unit* unit) { return unit->id; })
        | std::ranges::to<std::vector>();
    HotSpot hs { .spot = CityArea::OutOfCity, .area = { START, START + 400, 400, 80 } };
    draw_area(G, city, hs, "Out of City", unit_ids);
}

void UICity::draw_city(cl::Game const& G, cl::City const& city)
{
    static constexpr int BORDER = 30;
    static constexpr int THICKNESS = 3;
    static constexpr int SHADOW = 8;

    int wh, ww;
    SDL_GetWindowSize(sdl_.window, &ww, &wh);

    SDL_Rect r { BORDER + SHADOW, BORDER + SHADOW, ww - (BORDER * 2), wh - (BORDER * 2) };
    SDL_SetRenderDrawColor(sdl_.ren, 128, 128, 128, 128);
    SDL_RenderFillRect(sdl_.ren, &r);

    r.x -= SHADOW, r.y -= SHADOW;
    SDL_SetRenderDrawColor(sdl_.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(sdl_.ren, &r);

    r.x += THICKNESS, r.y += THICKNESS, r.w -= THICKNESS*2, r.h -= THICKNESS*2;
    SDL_SetRenderDrawColor(sdl_.ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(sdl_.ren, &r);
}

void UICity::draw_area(cl::Game const& G, cl::City const& city, HotSpot const& hotspot, std::string const& name,
        std::vector<cl::Unit::Id> units)
{
    SDL_SetRenderDrawColor(sdl_.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(sdl_.ren, &hotspot.area);
    sdl_.text_large->write(sdl_, name, hotspot.area.x + 4, hotspot.area.y + 8);
    hotspots_.emplace_back(hotspot);

    SDL_Rect r = hotspot.area;

}

