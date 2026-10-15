#include <concepts>
#include <cstdio>

// (a) trailing-less requires clause after the template parameter list
template <typename T>
    requires std::integral<T>
T increment_a(T x) {
    return x + 1;
}

// (b) constrained-parameter shorthand: the concept stands in for `typename`
template <std::integral T>
T increment_b(T x) {
    return x + 1;
}

// (c) abbreviated function template: no template<> header at all, the
// concept qualifies `auto` directly in the parameter list
auto increment_c(std::integral auto x) {
    return x + 1;
}

// (d) trailing requires clause, after the function's own signature
template <typename T>
T increment_d(T x)
    requires std::integral<T>
{
    return x + 1;
}

int main() {
    std::printf("increment_a(1) = %d\n", increment_a(1));
    std::printf("increment_b(2) = %d\n", increment_b(2));
    std::printf("increment_c(3) = %d\n", increment_c(3));
    std::printf("increment_d(4) = %d\n", increment_d(4));
    std::printf("all four express the same constraint on T -- pick (b) or (c)\n"
                "for a single simple concept, (a) or (d) when the constraint is\n"
                "a longer expression that would crowd the template header.\n");
}
