#ifndef UICITY_HH
#define UICITY_HH

#include "civlike.hh"

class UICity {
public:
    std::optional<cl::City::Id> selected_city {};

    void do_events(cl::Game& G);
    void draw(cl::Game const& G) const;
};

#endif //UICITY_HH
