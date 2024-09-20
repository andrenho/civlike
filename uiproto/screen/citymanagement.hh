#ifndef CITYMANAGEMENT_HH
#define CITYMANAGEMENT_HH

#include "screen.hh"

class CityManagement : public Screen {
public:
    using Screen::Screen;

    void screen_event(cl::Game& G, SDL_Event* e) override;
    void draw(cl::Game const& G) const override;

    cl::City const* city;

private:
    void draw_city(cl::Game const& G) const;
};

#endif //CITYMANAGEMENT_HH
