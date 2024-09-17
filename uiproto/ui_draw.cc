#include "ui.hh"

#include <ranges>
#include <variant>
namespace r = std::ranges;

using namespace cl;

#include "text.hh"

void UI::draw(Game const& G) const
{
    // SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderDrawColor(ren_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren_);

    int ww, wh;
    SDL_GetWindowSize(window_, &ww, &wh);
    SDL_Rect screen_r = { 0, 0, ww, wh };

    for (int x = 0; x < G.map_size.w; ++x) {
        for (int y = 0; y < G.map_size.h; ++y) {
            SDL_Rect tile_r = tile_rect(Point { x, y });
            if (!SDL_HasIntersection(&screen_r, &tile_r))
                continue;
            draw_tile(G, Point { x, y });
        }
    }

    draw_status(G);
}

void UI::draw_tile(Game const& G, Point p) const
{
    draw_terrain(G, p);

    if (auto o_unit = unit_to_draw(G, p); o_unit)
        draw_unit(G, **o_unit);

    auto city = G.cities.find_first_value([p](auto const& c) { return c.pos == p; });
    if (city)
        draw_city(G, **city);
}

void UI::draw_terrain(Game const& G, Point p) const
{
    const auto t_clr = G.ruleset.terrains[G.tiles.at(p.x).at(p.y).terrain_id].color;
    SDL_SetRenderDrawColor(ren_, t_clr.r, t_clr.g, t_clr.b, SDL_ALPHA_OPAQUE);
    const SDL_Rect r = tile_rect(p);
    SDL_RenderFillRect(ren_, &r);
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(ren_, r.x, r.y);
}


std::optional<Unit const*> UI::unit_to_draw(Game const& G, Point p) const
{
    std::optional<Unit const*> unit_to_move;

    const auto units = G.units_in_xy(p);

    // find focused
    const auto focused_unit = G.focused_unit(player_nation_id_);
    if (focused_unit && focused_unit.value()->pos == p) {
        const bool blink = (SDL_GetTicks64() / 500) % 2 == 1;
        unit_to_move = blink ? *focused_unit : std::optional<Unit const*>{};
    } else if (!units.empty()) {
        // not focused, return the first found
        unit_to_move = units.at(0);
    } else {
        unit_to_move = {};  // no units found
    }

    if (unit_to_move && moving_unit_ && (*unit_to_move)->id == *moving_unit_)
        unit_to_move = {};

    return unit_to_move;
}

void UI::draw_unit(Game const& G, Unit const& unit, Point displacement) const
{
    const auto color = G.ruleset.nations[unit.nation_id].color;

    // define position
    SDL_Rect r = tile_rect(unit.pos);
    r.x += displacement.x, r.y += displacement.y;

    // square
    SDL_SetRenderDrawColor(ren_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(ren_, &r);
    ++r.x, ++r.y, r.w -= 2, r.h -= 2;
    SDL_RenderDrawRect(ren_, &r);
    ++r.x, ++r.y, r.w -= 2, r.h -= 2;
    SDL_RenderDrawRect(ren_, &r);

    // unit type letter
    const auto unit_char = std::string(1, G.ruleset.unit_types[unit.unit_type_id].char_display);
    write(*text_large_, unit_char, r.x + 9, r.y + 4);

    // state
    if (unit.state == Unit::State::Fortify)
        write(*text_small_, "F", r.x + 20, r.y);
}

void UI::draw_city(cl::Game const& G, cl::City const& city) const
{
    SDL_Rect r = tile_rect(city.pos);
    const auto color = G.ruleset.nations[city.nation_id].color;

    // square
    SDL_SetRenderDrawColor(ren_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(ren_, &r);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

void UI::process_visual_cues(Game& G)
{
    auto& vc = G.visual_cues;
    while (!vc.empty()) {
        std::visit(overloaded {
            [this, &G] (MoveUnit const& mu) { visual_cue_move_unit(G, mu); },
        }, vc.back());
        vc.pop();
    }
}

void UI::visual_cue_move_unit(Game const& G, MoveUnit const& mu)
{
    Unit const& unit = G.units[mu.unit_id];

    moving_unit_ = mu.unit_id;
    for (size_t i = 0; i < TILE_SIZE; i += 2) {
        Point displacement = (-directions.at(mu.direction) * TILE_SIZE) + (directions.at(mu.direction) * i);

        draw(G);
        draw_unit(G, unit, displacement);
        SDL_RenderPresent(ren_);
    }
    moving_unit_ = {};
}

void UI::draw_status(Game const& G) const
{
    int sw, sh;
    SDL_GetWindowSize(window_, &sw, &sh);

    int x = sw - 250;
    int y = sh - 100;

    y = write(*text_large_, "Round: " + std::to_string(G.round_nr), x, y);

    auto f_unit = G.focused_unit(player_nation_id_);
    if (f_unit) {
        Unit const& unit = **f_unit;
        y = write(*text_large_, "Unit #" + std::to_string(unit.id), x, y);
        y = write(*text_large_, G.ruleset.unit_types[unit.unit_type_id].name, x, y);
        write(*text_large_, "Moves left: " + std::to_string(unit.moves_left), x, y);
    }
}

int UI::write(Text& text_mgr, std::string const& text, int x, int y) const
{
    const auto [tx, tw, th, lineskip] = text_mgr.text_tx(text, { 0, 0, 0, SDL_ALPHA_OPAQUE });
    const SDL_Rect r { x, y, tw, th };
    SDL_RenderCopy(ren_, tx, nullptr, &r);
    return y + lineskip;
}

SDL_Rect UI::tile_rect(Point p) const
{
    return {
        .x = (int) (p.x * TILE_SIZE) + rel_x,
        .y = (int) (p.y * TILE_SIZE) + rel_y,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };
}
