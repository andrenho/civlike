#ifndef SCREEN_HH
#define SCREEN_HH

#include <optional>
#include <string>

#include <SDL2/SDL.h>
#include "civlike.hh"

class Resources;

class Screen {
public:
    explicit Screen(Resources& res) : res(res) {}
    virtual ~Screen() = default;

    void do_event(cl::Game& G, SDL_Event* e);
    void draw(cl::Game const& G) const;

    static constexpr size_t TILE_SIZE = 32;

protected:
    Resources& res;

    enum class HotSpotArea { OutOfCity };

    using DraggableFrom = std::variant<cl::Unit::Id>;
    using DraggableTo = std::variant<HotSpotArea, cl::Tile const*>;

    struct HotSpotDraggableFrom {
        DraggableFrom from;
        SDL_Point point;
    };

    struct HotSpotDraggableTo {
        DraggableTo to;
        SDL_Rect    rect;
        std::optional<std::string> name;
    };

    using HotSpot = std::variant<HotSpotDraggableFrom, HotSpotDraggableTo>;
    std::vector<HotSpot> hotspots_;

    std::optional<DraggableFrom> dragging_ {};

    // implement these
    virtual void screen_draw(cl::Game const& G) const = 0;
    virtual void screen_event(cl::Game& G, SDL_Event* e) = 0;
    virtual void drop([[maybe_unused]] cl::Game& G, [[maybe_unused]] DraggableFrom from, [[maybe_unused]] DraggableTo to) {}

    void draw_draggable(cl::Game const& G, DraggableFrom from, SDL_Point p) const;
    void draw_unit_at_point(cl::Game const& G, cl::Unit::Id unit_id, SDL_Point p) const;
    void draw_tile_at_point(cl::Game const& G, cl::Tile const& tile, SDL_Point p) const;
    void draw_hotspots(cl::Game const& G) const;
};

#endif //SCREEN_HH
