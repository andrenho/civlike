import geometry;
import ruleset;
import colonization;

#include <cstdio>

int main()
{
    GameParameters par;
    Ruleset ruleset = Ruleset::create_from_cpp(colonization_ruleset());
    printf("%d\n", ruleset.map_size.optional(ruleset, par)->w);
}
