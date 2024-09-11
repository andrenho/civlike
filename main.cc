import civlike;

#include <cstdio>

int main()
{
    const GameParameters par;
    const Ruleset ruleset = Ruleset::create_from_cpp(colonization_ruleset());
    printf("%d\n", ruleset.map_size.optional(ruleset, par)->w);
}
