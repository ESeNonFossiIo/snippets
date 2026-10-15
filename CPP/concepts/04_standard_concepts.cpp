#include <concepts>
#include <cstdio>
#include <string>

struct Point {
    int x, y;
    bool operator==(const Point&) const = default;
    auto operator<=>(const Point&) const = default;
};

// std::same_as / std::convertible_to: constrain a conversion helper so it
// only accepts things that can actually become a T.
template <typename From, typename To>
    requires std::convertible_to<From, To>
To convert(From value) {
    return static_cast<To>(value);
}

// std::integral / std::floating_point: pick the right formatting per family.
void print_number(std::integral auto n) { std::printf("integral: %lld\n", static_cast<long long>(n)); }
void print_number(std::floating_point auto n) { std::printf("floating point: %f\n", static_cast<double>(n)); }

// std::equality_comparable / std::totally_ordered: only meaningful for types
// that actually support == or a full ordering.
template <std::equality_comparable T>
bool same(const T& a, const T& b) {
    return a == b;
}

template <std::totally_ordered T>
const T& larger(const T& a, const T& b) {
    return a < b ? b : a;
}

// std::regular = std::semiregular (default-constructible + copyable) plus
// equality-comparable -- roughly "behaves like a value". std::movable is the
// weaker "can be moved, need not be copied" requirement.
template <std::regular T>
T make_default() {
    return T{};
}

template <std::movable T>
T pass_through(T value) {
    return value;
}

int main() {
    std::printf("convert<double,int>(3.9)   = %d\n", convert<double, int>(3.9));
    print_number(42);
    print_number(3.14);
    std::printf("same(1, 1)                  = %s\n", same(1, 1) ? "true" : "false");
    std::printf("same(std::string(\"a\"),\"b\")  = %s\n",
                same(std::string("a"), std::string("b")) ? "true" : "false");
    std::printf("larger(3, 7)                = %d\n", larger(3, 7));

    Point p1{1, 2}, p2{3, 4};
    Point smaller_first = larger(p1, p2); // Point gets <=> for free via = default
    std::printf("larger({1,2},{3,4})         = {%d,%d}\n", smaller_first.x, smaller_first.y);

    Point p3 = make_default<Point>();
    std::printf("make_default<Point>()       = {%d,%d}\n", p3.x, p3.y);
    std::printf("pass_through(std::string)   = %s\n", pass_through(std::string("moved")).c_str());
}
