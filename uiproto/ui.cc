#include "ui.hh"

#include <ranges>
#include <variant>
namespace r = std::ranges;

#include "SDL2/SDL.h"
#include "text.hh"

UI::UI(Nation::Id player_nation_id)
    : player_nation_id_(player_nation_id)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    window_ = SDL_CreateWindow("civlike (uiproto)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_RESIZABLE);
    ren_ = SDL_CreateRenderer(window_, -1, 0);
    text_ = std::make_unique<Text>(ren_);
}

UI::~UI()
{
    SDL_DestroyRenderer(ren_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void UI::run(Game& game)
{
    while (true) {
        do_events(game);
        process_visual_cues(game);
        draw(game);
        SDL_RenderPresent(ren_);
    }
}

void UI::do_events(Game& game)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: SDL_Quit(); exit(0);
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_w: game.focus_next(player_nation_id_); break;
                    case SDLK_KP_1: game.move_focused_unit(player_nation_id_, Direction::SW); break;
                    case SDLK_KP_2: game.move_focused_unit(player_nation_id_, Direction::S); break;
                    case SDLK_KP_3: game.move_focused_unit(player_nation_id_, Direction::SE); break;
                    case SDLK_KP_4: game.move_focused_unit(player_nation_id_, Direction::W); break;
                    case SDLK_KP_6: game.move_focused_unit(player_nation_id_, Direction::E); break;
                    case SDLK_KP_7: game.move_focused_unit(player_nation_id_, Direction::NW); break;
                    case SDLK_KP_8: game.move_focused_unit(player_nation_id_, Direction::N); break;
                    case SDLK_KP_9: game.move_focused_unit(player_nation_id_, Direction::NE); break;
                    case SDLK_q: SDL_Quit(); exit(0);
                }
        }
    }
}

void UI::draw(Game const& game) const
{
    SDL_SetRenderDrawColor(ren_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren_);

    for (int x = 0; x < game.map_size().w; ++x)
        for (int y = 0; y < game.map_size().h; ++y)
            draw_tile(game, Point { x, y });

    draw_status(game);
}

void UI::draw_tile(Game const& game, Point p) const
{
    const auto t_clr = game.ruleset.terrains.at(game.tiles().at(p.x).at(p.y).terrain_id).color;
    SDL_SetRenderDrawColor(ren_, t_clr.r, t_clr.g, t_clr.b, SDL_ALPHA_OPAQUE);
    const SDL_Rect r { .x = (int) (p.x * TILE_SIZE), .y = (int) (p.y * TILE_SIZE), .w = TILE_SIZE, .h = TILE_SIZE };
    SDL_RenderFillRect(ren_, &r);
    SDL_SetRenderDrawColor(ren_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(ren_, r.x, r.y);

    if (auto o_unit = unit_to_draw(game, p); o_unit)
        draw_unit(game, **o_unit);
}

std::optional<Unit const*> UI::unit_to_draw(Game const& game, Point p) const
{
    std::optional<Unit const*> unit_to_move;

    const auto units = game.units_in_xy(p);

    // find focused
    const auto focused_unit = game.focused_unit(player_nation_id_);
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

void UI::draw_unit(Game const& game, Unit const& unit, Point displacement) const
{
    const auto color = game.ruleset.nations.at(unit.nation_id).color;

    // define position
    SDL_Rect r { .x = (int) (unit.pos.x * TILE_SIZE), .y = (int) (unit.pos.y * TILE_SIZE), .w = TILE_SIZE, .h = TILE_SIZE };
    r.x += displacement.x, r.y += displacement.y;

    // square
    SDL_SetRenderDrawColor(ren_, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(ren_, &r);
    ++r.x, ++r.y, r.w -= 2, r.h -= 2;
    SDL_RenderDrawRect(ren_, &r);
    ++r.x, ++r.y, r.w -= 2, r.h -= 2;
    SDL_RenderDrawRect(ren_, &r);

    // letter
    const auto unit_char = std::string(1, game.ruleset.unit_types.at(unit.unit_type_id).char_display);
    const auto [tx, tw, th, _] = text_->text_tx(unit_char, { color.r, color.g, color.b, SDL_ALPHA_OPAQUE });
    r.x += 9, r.y += 4, r.w = tw, r.h = th;
    SDL_RenderCopy(ren_, tx, nullptr, &r);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

void UI::process_visual_cues(Game& game)
{
    auto& vc = game.visual_cues();
    while (!vc.empty()) {
        std::visit(overloaded {
            [this, &game] (MoveUnit const& mu) { visual_cue_move_unit(game, mu); },
        }, vc.back());
        vc.pop();
    }
}

void UI::visual_cue_move_unit(Game const& game, MoveUnit const& mu)
{
    Unit const& unit = game.units().at(mu.unit_id);

    moving_unit_ = mu.unit_id;
    for (size_t i = 0; i < TILE_SIZE; i += 2) {
        Point displacement = (-directions.at(mu.direction) * TILE_SIZE) + (directions.at(mu.direction) * i);

        draw(game);
        draw_unit(game, unit, displacement);
        SDL_RenderPresent(ren_);
    }
    moving_unit_ = {};
}

void UI::draw_status(Game const& game) const
{
    int sw, sh;
    SDL_GetWindowSize(window_, &sw, &sh);

    auto write_line = [this](std::string const& text, int x, int y) {
        const auto [tx, tw, th, lineskip] = text_->text_tx(text, { 0, 0, 0, SDL_ALPHA_OPAQUE });
        const SDL_Rect r { x, y, tw, th };
        SDL_RenderCopy(ren_, tx, nullptr, &r);
        return y + lineskip;
    };

    int x = sw - 250;
    int y = sh - 100;

    y = write_line("Round: " + std::to_string(game.round_nr()), x, y);

    auto f_unit = game.focused_unit(player_nation_id_);
    if (f_unit) {
        Unit const& unit = **f_unit;
        y = write_line(game.ruleset.unit_types.at(unit.unit_type_id).name, x, y);
        y = write_line("Moves left: " + std::to_string(unit.moves_left), x, y);
    }
}