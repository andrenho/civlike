#ifndef RESOURCES_HH
#define RESOURCES_HH

#include <memory>

#include "SDL2/SDL.h"
#include "text.hh"
#include "screen/mainmap.hh"
#include "screen/screen.hh"

class Resources {
public:
    Resources();
    ~Resources();

    Resources(Resources const&) = delete;
    Resources& operator=(Resources const&) = delete;

    std::unique_ptr<Screen> main_map = std::make_unique<MainMap>(*this);

    SDL_Window*           window;
    SDL_Renderer*         ren;
    std::unique_ptr<Text> text_large;
    std::unique_ptr<Text> text_small;
    Screen*               current_screen = main_map.get();
    const cl::Nation::Id  player_nation_id = cl::Nation::Id { 'E' };
};

#endif //RESOURCES_HH
