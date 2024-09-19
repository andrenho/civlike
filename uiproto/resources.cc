#include "resources.hh"

Resources::Resources(Screen* initial_screen)
    : current_screen(initial_screen)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    window = SDL_CreateWindow("civlike (uiproto)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, 0);
    ren = SDL_CreateRenderer(window, -1, 0);
    TTF_Init();
    text_large = std::make_unique<Text>(ren, 18);
    text_small = std::make_unique<Text>(ren, 12);
}

Resources::~Resources()
{
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();
}