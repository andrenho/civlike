#ifndef MAINMAP_HH
#define MAINMAP_HH
#include "screen.hh"

class MainMap : public Screen {
public:
    using Screen::Screen;

    void do_event(cl::Game& G, SDL_Event* e) override;
    void draw(cl::Game const& G) const override;

    static constexpr size_t TILE_SIZE = 32;

private:
    // events

    void select_unit_in_xy(cl::Game& G, cl::Point tile);
    bool select_city_in_xy(cl::Game& G, cl::Point tile);

    // draw

    void draw_tile(cl::Game const& G, cl::Point p) const;

    void draw_terrain(cl::Game const& G, cl::Point p) const;
    void draw_unit(cl::Game const& G, cl::Unit const& unit, cl::Point displacement={0,0}) const;
    void draw_city(cl::Game const& G, cl::City const& city) const;

    void draw_status(cl::Game const& G) const;

    void process_visual_cues(cl::Game& G);
    void visual_cue_move_unit(cl::Game const& G, cl::MoveUnit const& mu);

    // query

    [[nodiscard]] std::optional<cl::Unit const*> unit_to_draw(cl::Game const& G, cl::Point p) const;
    [[nodiscard]] SDL_Rect tile_rect(cl::Point p) const;

    // fields

    int  rel_x = 0, rel_y = 0;
    bool dragging_map_ = false;
    std::optional<cl::Unit::Id> moving_unit_ {};

};

#endif //MAINMAP_HH
