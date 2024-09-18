#ifndef UICITY_HH
#define UICITY_HH

#include "SDL2/SDL.h"

#include <variant>

#include "civlike.hh"
#include "sdl.hh"

class UICity {
public:
    explicit UICity(SDL const& sdl) : sdl_(sdl) {}

    std::optional<cl::City::Id> selected_city {};

    void do_events(cl::Game& G);
    void draw(cl::Game const& G);

private:
    enum class CityArea { OutOfCity };
    struct HotSpot {
        std::variant<CityArea, cl::Unit::Id> spot;
        SDL_Rect                             area;
    };

    void draw_city(cl::Game const& G, cl::City const& city);
    void draw_area(cl::Game const& G, cl::City const& city, HotSpot const& hotspot, std::string const& name,
        std::vector<cl::Unit::Id> units);

    SDL const& sdl_;
    std::vector<HotSpot> hotspots_;
};

#endif //UICITY_HH
