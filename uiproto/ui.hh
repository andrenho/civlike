#ifndef UI_HH
#define UI_HH

#include <memory>
#include <optional>

#include "SDL2/SDL.h"
#include "text.hh"

#include "civlike.hh"

class UI {
public:
    explicit UI(cl::Nation::Id player_nation_id);
    ~UI();

    void run(cl::Game& game);

private:
    void do_events(cl::Game& game);

    void draw(cl::Game const& game) const;
    void draw_tile(cl::Game const& game, cl::Point p) const;
    void draw_unit(cl::Game const& game, cl::Unit const& unit, cl::Point displacement={0,0}) const;
    void draw_status(cl::Game const& game) const;

    void process_visual_cues(cl::Game& game);
    void visual_cue_move_unit(cl::Game const& game, cl::MoveUnit const& mu);

    int write(Text& text_mgr, std::string const& text, int x, int y) const;

    [[nodiscard]] std::optional<cl::Unit const*> unit_to_draw(cl::Game const& game, cl::Point p) const;

    cl::Nation::Id              player_nation_id_;
    SDL_Window*                 window_ = nullptr;
    SDL_Renderer*               ren_ = nullptr;
    std::unique_ptr<Text>       text_large_;
    std::unique_ptr<Text>       text_small_;
    std::optional<cl::Unit::Id> moving_unit_ {};

    static constexpr size_t TILE_SIZE = 32;
};

#endif //UI_HH
