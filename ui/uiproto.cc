#include <stdexcept>
#include <fstream>
#include <sstream>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "civlike.hh"

#define TILE_SIZE 32

static ALLEGRO_FONT* font;
static ALLEGRO_TIMER* timer;

struct Event {
    enum class Type { Quit, Move };

    Type type;
    union {
        cl::Direction move_direction;
    };
};

static std::optional<Event> keypress(cl::Rules const& rules, cl::Game const& game, ALLEGRO_KEYBOARD_EVENT k)
{
    switch (k.keycode) {
        case ALLEGRO_KEY_Q:
            return Event { .type = Event::Type::Quit };
        case ALLEGRO_KEY_1:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::SE };
        case ALLEGRO_KEY_2:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::S };
        case ALLEGRO_KEY_3:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::SW };
        case ALLEGRO_KEY_4:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::W };
        case ALLEGRO_KEY_6:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::E };
        case ALLEGRO_KEY_7:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::NE };
        case ALLEGRO_KEY_8:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::N };
        case ALLEGRO_KEY_9:
            return Event { .type = Event::Type::Move, .move_direction = cl::Direction::NW };
    }

    return {};
}


static cl::Game do_event(cl::Rules const& rules, cl::Game const& game, Event const& e)
{
    switch (e.type) {
        case Event::Type::Move: {
            cl::MoveResponse move = cl::move_focused_unit(rules, game, e.move_direction);
            // TODO - move animation
            return move.game;
        }
        case Event::Type::Quit: break;
    }

    return game;
}


static void draw_tile(cl::Rules const& rules, cl::Game const& game, ssize_t x, ssize_t y)
{
    std::string terrain_id = std::string("") + game.map.terrain.at(y).at(x);
    auto c = rules().terrains()[terrain_id].color().color(game);
    al_draw_filled_rectangle(x * TILE_SIZE, y * TILE_SIZE, (x+1) * TILE_SIZE, (y+1) * TILE_SIZE,
                             al_map_rgb(c.r, c.g, c.b));
    al_draw_rectangle(x * TILE_SIZE, y * TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, al_map_rgb(0, 0, 0), 2);
}

static void draw_unit(cl::Rules const& rules, cl::Game const& game, size_t id, cl::Game::Unit const& unit)
{
    if (id == game.focused_unit.value_or(-1) && (al_get_timer_count(timer) / 20) % 2 == 1)  // blink focused unit
        return;

    auto c = rules().nations()[unit.nation].color().color(game);
    al_draw_rectangle(unit.pos.x * TILE_SIZE, unit.pos.y * TILE_SIZE, (unit.pos.x+1) * TILE_SIZE, (unit.pos.y+1) * TILE_SIZE,
                      al_map_rgb(c.r, c.g, c.b), 3);
    char text[2] = { unit.type.value, 0 };
    al_draw_text(font, al_map_rgb(c.r, c.g, c.b), unit.pos.x * TILE_SIZE + 12, unit.pos.y * TILE_SIZE + 12, 0, text);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        throw std::runtime_error("Usage: ./uiproto RULEFILE");

    std::ifstream t(argv[1]);
    std::stringstream buffer;
    buffer << t.rdbuf();

    const cl::Rules rules(buffer.str());
    auto game = cl::create_game(rules);

    al_init();
    al_install_keyboard();

    timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    ALLEGRO_DISPLAY* disp = al_create_display(800, 600);

    font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    bool done = false;

    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (true)
    {
        al_wait_for_event(queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN: {
                    auto e = keypress(rules, game, event.keyboard);
                    if (e) {
                        if (e->type == Event::Type::Quit)
                            done = true;
                        else
                            game = do_event(rules, game, *e);
                    }
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            for (ssize_t x = 0; x < game.map.size.w; ++x)
                for (ssize_t y = 0; y < game.map.size.h; ++y)
                    draw_tile(rules, game, x, y);
            for (auto const& [id, unit]: game.units)
                draw_unit(rules, game, id, unit);
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}