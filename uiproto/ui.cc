#include "ui.hh"

#include <random>
#include <string>
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

void UI::run(Game& G)
{
    while (true) {

        if (ui_city_.selected_city)
            ui_city_.do_events(G);
        else
            do_events(G);

        process_visual_cues(G);

        draw(G);
        if (ui_city_.selected_city)
            ui_city_.draw(G);

        SDL_RenderPresent(ren_);
    }
}

void UI::do_events(Game& G)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {

            case SDL_QUIT:
                SDL_Quit();
                exit(0);

            case SDL_MOUSEBUTTONDOWN:
                switch (e.button.button) {
                    case 1: select_unit_in_xy(G, (e.button.x - rel_x) / TILE_SIZE, (e.button.y - rel_y) / TILE_SIZE); break;
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
                auto f_unit = G.focused_unit(player_nation_id_);
                if (f_unit) {
                    Unit::Id unit_id = (*f_unit)->id;
                    switch (e.key.keysym.sym) {
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
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "City created", "City created", window_);
                            break;
                    }
                }

                switch (e.key.keysym.sym) {
                    case SDLK_w:     cmd::focus_next(G, player_nation_id_, false); break;
                    case SDLK_SPACE: cmd::end_round(G, player_nation_id_, true); break;
                    case SDLK_q:     SDL_Quit(); exit(0);
                }
            }

        }
    }
}

void UI::select_unit_in_xy(Game& G, int x, int y)
{
    auto o_unit = unit_to_draw(G, Point { x, y }); // TODO - open a unit selector
    if (o_unit) {
        const Unit::Id unit_id = (*o_unit)->id;
        cmd::focus_unit(G, unit_id, false);
    }
}