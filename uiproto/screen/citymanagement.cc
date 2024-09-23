#include "citymanagement.hh"

#include "../resources.hh"
#include "mainmap.hh"

using namespace cl;

static constexpr int START = 50;
static constexpr int AREA_H = 70;

void CityManagement::screen_event(Game& G, SDL_Event* e)
{

}

void CityManagement::set_city(Game const& G, City const* city)
{
    city_ = city;
    reset_hotspots(G);
}

void CityManagement::reset_hotspots(Game const& G)
{
    hotspots_.clear();

    // out of city

    constexpr int static OUT_OF_CITY_Y = START + 420;

    hotspots_.push_back(HotSpotDraggableTo {
        .to = HotSpotArea::OutOfCity,
        .rect = { START, OUT_OF_CITY_Y, 600, AREA_H },
        .name = "Out of City",
    });

    int x = START + 10;
    for (auto unit: G.units_in_xy(city_->pos)) {
        if (!unit->city) {
            hotspots_.push_back(HotSpotDraggableFrom {
                .from = unit->id,
                .point = { x, OUT_OF_CITY_Y + 30 },
            });
            x += 40;
        }
    }

    // tiles

    constexpr static int TILE_X = 690;
    constexpr static int TILE_Y = 70;
    for (x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            hotspots_.push_back(HotSpotDraggableTo {
                .to = &G.tiles[city_->pos.x + x][city_->pos.y + y],
                .rect = { TILE_X + (int)(x * TILE_SIZE), TILE_Y + (int)(y * TILE_SIZE), TILE_SIZE, TILE_SIZE }
            });
        }
    }

}

void CityManagement::screen_draw(Game const& G) const
{
    res.main_map->screen_draw(G);
    draw_city(G);
    draw_hotspots(G);
}

void CityManagement::draw_city(Game const& G) const
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

void CityManagement::drop(Game& G, DraggableFrom from, DraggableTo to)
{
    if (auto const unit_id = std::get_if<Unit::Id>(&from)) {
        if (auto const tile = std::get_if<Tile const*>(&to)) {
            cmd::move_unit_to_city_tile(G, *unit_id, city_->id, (*tile)->pos);
        }
    }

    reset_hotspots(G);
}
