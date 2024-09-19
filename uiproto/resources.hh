#ifndef RESOURCES_HH
#define RESOURCES_HH

#include <memory>

#include "SDL2/SDL.h"
#include "text.hh"
#include "screen/screen.hh"

class Resources {
public:
    Resources(Screen* initial_screen);
    ~Resources();

    Resources(Resources const&) = delete;
    Resources& operator=(Resources const&) = delete;

    SDL_Window*           window;
    SDL_Renderer*         ren;
    std::unique_ptr<Text> text_large;
    std::unique_ptr<Text> text_small;
    Screen*               current_screen;
};

#endif //RESOURCES_HH
