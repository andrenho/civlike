#ifndef SCREEN_HH
#define SCREEN_HH

#include <SDL2/SDL_events.h>
#include "civlike.hh"

class Resources;

class Screen {
public:
    explicit Screen(Resources& res) : res(res) {}
    virtual ~Screen() = default;

    void do_event(cl::Game& G, SDL_Event* e);
    virtual void draw(cl::Game const& G) const = 0;

protected:
    virtual void screen_event(cl::Game& G, SDL_Event* e) = 0;
    void draw_hotspots(cl::Game& G);

    Resources& res;

    enum class HotSpotArea { OutOfCity };
    struct HotSpotRect {
        HotSpotArea area;
        SDL_Rect    rect;
    };
    struct HotSpotUnit {
        cl::Unit::Id unit;
        SDL_Point    point;
    };
    using HotSpot = std::variant<HotSpotRect, HotSpotUnit>;
    std::vector<HotSpot> hotspots_;
};

#endif //SCREEN_HH
