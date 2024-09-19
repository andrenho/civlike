#ifndef SCREEN_HH
#define SCREEN_HH

#include <SDL2/SDL_events.h>
#include "civlike.hh"

class Resources;

class Screen {
public:
    explicit Screen(Resources const& res) : res(res) {}
    virtual ~Screen() = default;

    virtual void do_event(cl::Game& G, SDL_Event* e) = 0;
    virtual void draw(cl::Game const& G) const = 0;

protected:
    Resources const& res;
};

#endif //SCREEN_HH
