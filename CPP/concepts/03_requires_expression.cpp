#include <concepts>
#include <cstdio>
#include <string>
#include <vector>

// A requires-expression is a boolean predicate built from requirement
// clauses, evaluated purely at compile time -- nothing inside it ever runs.
template <typename T>
concept Container = requires(T c) {
    typename T::value_type;                                    // type requirement
    typename T::iterator;                                       // type requirement
    { c.begin() } -> std::same_as<typename T::iterator>;         // compound requirement
    { c.end() } -> std::same_as<typename T::iterator>;           // compound requirement
    { c.size() } -> std::convertible_to<std::size_t>;            // compound requirement
    // simple requirement: only checks that `c.begin() != c.end()` *compiles*.
    // Its runtime truth value is irrelevant here -- an empty container still
    // satisfies this line, since requires-expressions test well-formedness,
    // not whether the expression would actually evaluate to true.
    c.begin() != c.end();
};

template <Container C>
void describe(const C& c) {
    std::printf("container of size %zu:", static_cast<std::size_t>(c.size()));
    for (const auto& elem : c) {
        if constexpr (std::same_as<C, std::string>) {
            std::printf(" '%c'", elem);
        } else {
            std::printf(" %d", elem);
        }
    }
    std::printf("\n");
}

int main() {
    describe(std::vector<int>{1, 2, 3, 4});
    describe(std::string{"abc"});
    // describe(42) would fail: int has no ::value_type, ::iterator, begin(),
    // end(), or size() -- Container is false for it, and the compiler says
    // exactly that instead of trying (and failing) to instantiate the body.
}
