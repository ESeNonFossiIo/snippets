#include <concepts>
#include <cstdio>

// --- subsumption -----------------------------------------------------------
//
// B is defined in terms of A (`A<T> && ...`), so the compiler can see, purely
// from the concepts' definitions, that satisfying B always implies satisfying
// A. That containment relationship is "subsumption", and overload resolution
// uses it as a tiebreaker: given two viable, otherwise-equal overloads, the
// one constrained by the more specific (subsuming) concept wins. This is not
// possible with plain SFINAE traits -- the compiler needs to see the actual
// boolean structure of the constraint, not just that it evaluated to true.

template <typename T>
concept A = requires(T x) { x + x; };

template <typename T>
concept B = A<T> && requires(T x) { x * x; };

void describe(A auto) { std::printf("chose the A overload (addable)\n"); }
void describe(B auto) { std::printf("chose the B overload (addable AND multipliable)\n"); }

// --- plain overloading on disjoint concepts --------------------------------

void classify(std::integral auto) { std::printf("classify(int-like): integral\n"); }
void classify(std::floating_point auto) { std::printf("classify(float-like): floating point\n"); }

int main() {
    describe(5); // int satisfies both A and B -- B wins because it subsumes A
    classify(5);
    classify(5.0);
}
