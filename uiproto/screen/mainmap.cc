#include "mainmap.hh"

#include "SDL2/SDL.h"
#include "../resources.hh"

using namespace cl;

#pragma region Events

void MainMap::do_event(Game& G, SDL_Event* e)
{
    switch (e->type) {

        case SDL_MOUSEBUTTONDOWN: {
            Point tile { .x = (int)((e->button.x - rel_x) / TILE_SIZE), .y = (int)((e->button.y - rel_y) / TILE_SIZE) };
            switch (e->button.button) {
                case 1:
                    if (!select_city_in_xy(G, tile))
                        select_unit_in_xy(G, tile);
                break;
                case 2: dragging_map_ = true; break;
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:
            switch (e->button.button) {
                case 2: dragging_map_ = false; break;
            }
            break;

        case SDL_MOUSEMOTION:
            if (dragging_map_) {
                rel_x += e->motion.xrel;
                rel_y += e->motion.yrel;
            }
            break;

        case SDL_KEYDOWN: {
            auto f_unit = G.focused_unit(res.player_nation_id);
            if (f_unit) {
                Unit::Id unit_id = (*f_unit)->id;
                switch (e->key.keysym.sym) {
                    case SDLK_KP_1: cmd::move_unit(G, unit_id, Direction::SW); break;
                    case SDLK_KP_2: cmd::move_unit(G, unit_id, Direction::S); break;
                    case SDLK_KP_3: cmd::move_unit(G, unit_id, Direction::SE); break;
                    case SDLK_KP_4: cmd::move_unit(G, unit_id, Direction::W); break;
                    case SDLK_KP_6: cmd::move_unit(G, unit_id, Direction::E); break;
                    case SDLK_KP_7: cmd::move_unit(G, unit_id, Direction::NW); break;
                    case SDLK_KP_8: cmd::move_unit(G, unit_id, Direction::N); break;
                    case SDLK_KP_9: cmd::move_unit(G, unit_id, Direction::NE); break;
                    case SDLK_f:    cmd::fortify_unit(G, unit_id); break;
                    case SDLK_b:
                        cmd::build_city(G, unit_id, "City");
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "City created", "City created", res.window);
                        break;
                }
            }

            switch (e->key.keysym.sym) {
                case SDLK_w:     cmd::focus_next(G, res.player_nation_id, false); break;
                case SDLK_SPACE: cmd::end_round(G, res.player_nation_id, true); break;
                case SDLK_q:     SDL_Quit(); exit(0);
            }
        }

    }
}

void MainMap::select_unit_in_xy(Game& G, Point tile)
{
    auto o_unit = unit_to_draw(G, tile); // TODO - open a unit selector
    if (o_unit) {
        const Unit::Id unit_id = (*o_unit)->id;
        cmd::focus_unit(G, unit_id, false);
    }
}

bool MainMap::select_city_in_xy(Game& G, Point tile)
{
    auto o_city = G.city_in_xy(tile);
    if (o_city)
        ; // TODO ui_city_.selected_city = (*o_city)->id;
    return !!o_city;
}

#pragma endregion

#pragma region Draw

void MainMap::draw(cl::Game const& G) const
{
    // SDL_SetRenderDrawColor(res.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderDrawColor(res.ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(res.ren);

    int ww, wh;
    SDL_GetWindowSize(res.window, &ww, &wh);
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

void MainMap::draw_tile(Game const& G, Point p) const
{
    draw_terrain(G, p);

    if (auto o_unit = unit_to_draw(G, p); o_unit)
        draw_unit(G, **o_unit);

    auto city = G.cities.find_first_value([p](auto const& c) { return c.pos == p; });
    if (city)
        draw_city(G, **city);
}

void MainMap::draw_terrain(Game const& G, Point p) const
{
    const auto t_clr = G.ruleset.terrains[G.tiles.at(p.x).at(p.y).terrain_id].color;
    SDL_SetRenderDrawColor(res.ren, t_clr.r, t_clr.g, t_clr.b, SDL_ALPHA_OPAQUE);
    const SDL_Rect r = tile_rect(p);
    SDL_RenderFillRect(res.ren, &r);
    SDL_SetRenderDrawColor(res.ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(res.ren, r.x, r.y);
}

void MainMap::draw_unit(Game const& G, Unit const& unit, Point displacement) const
{
    const auto color = G.ruleset.nations[unit.nation_id].color;

    // define position
    SDL_Rect r = tile_rect(unit.pos);
    r.x += displacement.x, r.y += displacement.y;

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

void MainMap::draw_city(cl::Game const& G, cl::City const& city) const
{
    SDL_Rect r = tile_rect(city.pos);
    const auto color = G.ruleset.nations[city.nation_id].color;

    // square
    SDL_SetRenderDrawColor(res.ren, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(res.ren, &r);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

void MainMap::process_visual_cues(Game& G)
{
    auto& vc = G.visual_cues;
    while (!vc.empty()) {
        std::visit(overloaded {
            [this, &G] (MoveUnit const& mu) { visual_cue_move_unit(G, mu); },
        }, vc.back());
        vc.pop();
    }
}

void MainMap::visual_cue_move_unit(Game const& G, MoveUnit const& mu)
{
    Unit const& unit = G.units[mu.unit_id];

    moving_unit_ = mu.unit_id;
    for (size_t i = 0; i < TILE_SIZE; i += 2) {
        Point displacement = (-directions.at(mu.direction) * TILE_SIZE) + (directions.at(mu.direction) * i);

        draw(G);
        draw_unit(G, unit, displacement);
        SDL_RenderPresent(res.ren);
    }
    moving_unit_ = {};
}

void MainMap::draw_status(Game const& G) const
{
    int sw, sh;
    SDL_GetWindowSize(res.window, &sw, &sh);

    int x = sw - 250;
    int y = sh - 100;

    y = res.text_large->write("Round: " + std::to_string(G.round_nr), x, y);

    auto f_unit = G.focused_unit(res.player_nation_id);
    if (f_unit) {
        Unit const& unit = **f_unit;
        y = res.text_large->write("Unit #" + std::to_string(unit.id), x, y);
        y = res.text_large->write(G.ruleset.unit_types[unit.unit_type_id].name, x, y);
        res.text_large->write("Moves left: " + std::to_string(unit.moves_left), x, y);
    }
}

#pragma endregion

#pragma region Query

std::optional<Unit const*> MainMap::unit_to_draw(Game const& G, Point p) const
{
    std::optional<Unit const*> unit_to_move;

    const auto units = G.units_in_xy(p);

    // find focused
    const auto focused_unit = G.focused_unit(res.player_nation_id);
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

SDL_Rect MainMap::tile_rect(Point p) const
{
    return {
        .x = (int) (p.x * TILE_SIZE) + rel_x,
        .y = (int) (p.y * TILE_SIZE) + rel_y,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };
}

#pragma endregion
