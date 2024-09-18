#ifndef SDL_HH
#define SDL_HH

#include <memory>

#include "SDL2/SDL.h"

class Text;

struct SDL {
    SDL_Window*           window;
    SDL_Renderer*         ren;
    std::unique_ptr<Text> text_large;
    std::unique_ptr<Text> text_small;
};

#endif //SDL_HH
