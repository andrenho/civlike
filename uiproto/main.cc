#include "civlike.hh"
#include "ui.hh"

#include "../variants/colonization.hh"

int main()
{
    using namespace cl;
    const GameParameters par;
    const Ruleset ruleset = Ruleset::create_from_cpp(std::move(colonization));
    Game G = cmd::new_game(ruleset, par);

    UI ui;

    while (ui.running()) {
        ui.do_events(G);
        ui.draw(G);
    }
}