#ifndef UI_HH
#define UI_HH

#include <memory>

#include "civlike.hh"
#include "resources.hh"
#include "screen/mainmap.hh"
#include "screen/screen.hh"

class UI {
public:
    void do_events(cl::Game& G);
    void draw(cl::Game const& G) const;

    [[nodiscard]] bool running() const { return running_; }

private:
    std::unique_ptr<Screen> main_map = std::make_unique<MainMap>();
    Resources               res_ { main_map.get() };
    bool                    running_ = true;

};

#endif //UI_HH
