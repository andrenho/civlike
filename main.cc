import civlike;

#include <cstdio>

int main()
{
    const GameParameters par;
    const Ruleset ruleset = Ruleset::create_from_cpp(colonization_ruleset());
    Game game(ruleset, par);
}
