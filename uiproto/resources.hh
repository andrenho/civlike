#ifndef RESOURCES_HH
#define RESOURCES_HH

#include <memory>

#include "SDL2/SDL.h"
#include "text.hh"
#include "screen/citymanagement.hh"
#include "screen/mainmap.hh"
#include "screen/screen.hh"

class Resources {
public:
    Resources();
    ~Resources();

    Resources(Resources const&) = delete;
    Resources& operator=(Resources const&) = delete;

    // screens
    std::unique_ptr<MainMap>        main_map        = std::make_unique<MainMap>(*this);
    std::unique_ptr<CityManagement> city_management = std::make_unique<CityManagement>(*this);

    SDL_Window*           window;
    SDL_Renderer*         ren;
    std::unique_ptr<Text> text_large;
    std::unique_ptr<Text> text_small;
    Screen*               current_screen = main_map.get();
    const cl::Nation::Id  player_nation_id = cl::Nation::Id { 'E' };
};

#endif //RESOURCES_HH
