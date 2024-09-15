#include "ui.hh"

#include <ranges>
namespace r = std::ranges;

using namespace cl;

UI::UI(Nation::Id player_nation_id)
    : player_nation_id_(player_nation_id)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    window_ = SDL_CreateWindow("civlike (uiproto)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_RESIZABLE);
    ren_ = SDL_CreateRenderer(window_, -1, 0);
    text_large_ = std::make_unique<Text>(ren_, 18);
    text_small_ = std::make_unique<Text>(ren_, 12);
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

            case SDL_QUIT:
                SDL_Quit();
                exit(0);

            case SDL_MOUSEBUTTONDOWN:
                switch (e.button.button) {
                    case 1: select_unit_in_xy(game, (e.button.x - rel_x) / TILE_SIZE, (e.button.y - rel_y) / TILE_SIZE); break;
                    case 2: dragging_map_ = true; break;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                switch (e.button.button) {
                    case 2: dragging_map_ = false; break;
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging_map_) {
                    rel_x += e.motion.xrel;
                    rel_y += e.motion.yrel;
                }
                break;

            case SDL_KEYDOWN: {
                auto f_unit = game.focused_unit(player_nation_id_);
                if (f_unit) {
                    Unit::Id unit_id = (*f_unit)->id;
                    switch (e.key.keysym.sym) {
                        case SDLK_KP_1: game.unit_move(unit_id, Direction::SW); break;
                        case SDLK_KP_2: game.unit_move(unit_id, Direction::S); break;
                        case SDLK_KP_3: game.unit_move(unit_id, Direction::SE); break;
                        case SDLK_KP_4: game.unit_move(unit_id, Direction::W); break;
                        case SDLK_KP_6: game.unit_move(unit_id, Direction::E); break;
                        case SDLK_KP_7: game.unit_move(unit_id, Direction::NW); break;
                        case SDLK_KP_8: game.unit_move(unit_id, Direction::N); break;
                        case SDLK_KP_9: game.unit_move(unit_id, Direction::NE); break;
                        case SDLK_f: game.unit_change_state(unit_id, Unit::State::Fortify); break;
                    }
                }

                switch (e.key.keysym.sym) {
                    case SDLK_w: game.focus_next(player_nation_id_); break;
                    case SDLK_SPACE: game.round_end(player_nation_id_); break;
                    case SDLK_q: SDL_Quit(); exit(0);
                }
            }

        }
    }
}

void UI::select_unit_in_xy(Game& game, int x, int y)
{
    auto o_unit = unit_to_draw(game, Point { x, y }); // TODO - open a unit selector
    if (o_unit) {
        const Unit::Id unit_id = (*o_unit)->id;
        game.unit_change_state(unit_id, Unit::State::Normal);
        game.focus_unit(unit_id, false);
    }
}