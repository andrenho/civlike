#ifndef UI_HH
#define UI_HH

#include <memory>
#include <optional>

#include "SDL2/SDL.h"
#include "text.hh"

#include "civlike.hh"
#include "sdl.hh"
#include "uicity.hh"

class UI {
public:
    explicit UI(cl::Nation::Id player_nation_id);
    ~UI();

    void run(cl::Game& G);

private:
    // events

    void do_events(cl::Game& G);
    void select_unit_in_xy(cl::Game& G, cl::Point tile);
    bool select_city_in_xy(cl::Game& G, cl::Point tile);

    // draw map

    void draw(cl::Game const& G) const;

    void draw_tile(cl::Game const& G, cl::Point p) const;

    void draw_terrain(cl::Game const& G, cl::Point p) const;
    void draw_unit(cl::Game const& G, cl::Unit const& unit, cl::Point displacement={0,0}) const;
    void draw_city(cl::Game const& G, cl::City const& city) const;

    void draw_status(cl::Game const& G) const;

    void process_visual_cues(cl::Game& G);
    void visual_cue_move_unit(cl::Game const& G, cl::MoveUnit const& mu);

    [[nodiscard]] std::optional<cl::Unit const*> unit_to_draw(cl::Game const& G, cl::Point p) const;
    [[nodiscard]] SDL_Rect tile_rect(cl::Point p) const;

    // fields

    cl::Nation::Id              player_nation_id_;
    SDL                         sdl;
    std::optional<cl::Unit::Id> moving_unit_ {};
    int                         rel_x = 0, rel_y = 0;
    bool                        dragging_map_ = false;
    UICity                      ui_city_;

    static constexpr size_t TILE_SIZE = 32;
};

#endif //UI_HH
