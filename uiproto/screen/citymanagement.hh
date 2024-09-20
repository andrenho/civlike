#ifndef CITYMANAGEMENT_HH
#define CITYMANAGEMENT_HH

#include "screen.hh"

class CityManagement : public Screen {
public:
    using Screen::Screen;

    void screen_event(cl::Game& G, SDL_Event* e) override;
    void screen_draw(cl::Game const& G) const override;

    [[nodiscard]] cl::City const* city() const { return city_; }
    void                          set_city(cl::Game const& G, cl::City const* city);

    void reset_hotspots(cl::Game const& G);

private:
    void draw_city(cl::Game const& G) const;

    cl::City const* city_;
};

#endif //CITYMANAGEMENT_HH
