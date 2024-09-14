#ifndef UI_HH
#define UI_HH

#include <memory>
#include <optional>

#include "SDL2/SDL.h"

#include "civlike.hh"

class UI {
public:
    explicit UI(Nation::Id player_nation_id);
    ~UI();

    void run(Game& game);

private:
    void do_events(Game& game);

    void draw(Game const& game) const;
    void draw_tile(Game const& game, Point p) const;
    void draw_unit(Game const& game, Unit const& unit, Point displacement={0,0}) const;
    void draw_status(Game const& game) const;

    void process_visual_cues(Game& game);
    void visual_cue_move_unit(Game const& game, MoveUnit const& mu);

    [[nodiscard]] std::optional<Unit const*> unit_to_draw(Game const& game, Point p) const;

    Nation::Id                  player_nation_id_;
    SDL_Window*                 window_ = nullptr;
    SDL_Renderer*               ren_ = nullptr;
    std::unique_ptr<class Text> text_;
    std::optional<Unit::Id>     moving_unit_ {};

    static constexpr size_t     TILE_SIZE = 32;
};

#endif //UI_HH
