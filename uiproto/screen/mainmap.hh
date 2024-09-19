#ifndef MAINMAP_HH
#define MAINMAP_HH
#include "screen.hh"

class MainMap : public Screen {
public:
    void do_event(cl::Game& G, SDL_Event* e) override;
    void draw(cl::Game const& G) const override;
};

#endif //MAINMAP_HH
