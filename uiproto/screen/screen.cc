#include "screen.hh"

#include "../resources.hh"

using namespace cl;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

void Screen::do_event(Game& G, SDL_Event* e)
{
    // picking up?
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        SDL_Point click { e->button.x, e->button.y };
        for (auto const& hotspot: hotspots_) {
            if (auto hs = std::get_if<HotSpotDraggableFrom>(&hotspot)) {
                SDL_Rect area { hs->point.x, hs->point.y, TILE_SIZE, TILE_SIZE };
                if (SDL_PointInRect(&click, &area)) {
                    dragging_ = hs->from;
                    break;
                }
            }
        }
    }

    // dropping off?
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT && dragging_) {
        SDL_Point click { e->button.x, e->button.y };
        for (auto const& hotspot: hotspots_) {
            if (auto hs = std::get_if<HotSpotDraggableTo>(&hotspot)) {
                if (SDL_PointInRect(&click, &hs->rect)) {
                    drop(*dragging_, hs->to);
                    break;
                }
            }
        }
        dragging_ = {};
    }

    screen_event(G, e);
}

void Screen::draw(Game const& G) const
{
    screen_draw(G);
}

void Screen::draw_hotspots(Game const& G) const
{
    for (auto const& hotspot: hotspots_) {
        std::visit(overloaded {

            [this](HotSpotDraggableTo const& hs) {
                SDL_SetRenderDrawColor(res.ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(res.ren, &hs.rect);
                if (hs.name)
                    res.text_large->write(*hs.name, hs.rect.x + 5, hs.rect.y + 5);
            },

            [this, G](HotSpotDraggableFrom const& hs) {
                if (dragging_ && *dragging_ == hs.from) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    draw_draggable(G, hs.from, { x - (int)(TILE_SIZE/2), y - (int)(TILE_SIZE/2) });
                } else {
                    draw_draggable(G, hs.from, hs.point);
                }
            },

        }, hotspot);
    }
}

void Screen::draw_draggable(Game const& G, DraggableFrom from, SDL_Point p) const
{
    std::visit(overloaded {
        [this, p, G](Unit::Id const& unit_id) {
            draw_unit_at_point(G, unit_id, p);
        },
    }, from);
}

void Screen::draw_unit_at_point(Game const& G, Unit::Id unit_id, SDL_Point point) const
{
    auto const& unit = G.units[unit_id];
    const auto color = G.ruleset.nations[unit.nation_id].color;

    // define position
    SDL_Rect r = { point.x, point.y, MainMap::TILE_SIZE, MainMap::TILE_SIZE };

    // square
    SDL_SetRenderDrawColor(res.ren, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(res.ren, &r);
    ++r.x, ++r.y, r.w -= 2, r.h -= 2;
    SDL_RenderDrawRect(res.ren, &r);
    ++r.x, ++r.y, r.w -= 2, r.h -= 2;
    SDL_RenderDrawRect(res.ren, &r);

    // unit type letter
    const auto unit_char = std::string(1, G.ruleset.unit_types[unit.unit_type_id].char_display);
    res.text_large->write(unit_char, r.x + 9, r.y + 4);

    // state
    if (unit.state == Unit::State::Fortify)
        res.text_small->write("F", r.x + 20, r.y);
}

void Screen::draw_tile_at_point(Game const& G, Tile const& tile, SDL_Point p) const
{
    const auto t_clr = G.ruleset.terrains[tile.terrain_id].color;
    SDL_SetRenderDrawColor(res.ren, t_clr.r, t_clr.g, t_clr.b, SDL_ALPHA_OPAQUE);
    const SDL_Rect r = { p.x, p.y, TILE_SIZE, TILE_SIZE };
    SDL_RenderFillRect(res.ren, &r);
    SDL_SetRenderDrawColor(res.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(res.ren, r.x, r.y);
}
