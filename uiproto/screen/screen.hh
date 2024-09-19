#ifndef SCREEN_HH
#define SCREEN_HH

#include <SDL2/SDL_events.h>
#include "civlike.hh"

class Screen {
public:
    virtual ~Screen() = default;

    virtual void do_event(cl::Game& G, SDL_Event* e) = 0;
    virtual void draw(cl::Game const& G) const = 0;
};

#endif //SCREEN_HH
