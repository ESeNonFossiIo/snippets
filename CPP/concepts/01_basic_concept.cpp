#include <cstdio>
#include <type_traits>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
T square(T x) {
    return x * x;
}

int main() {
    std::printf("square(6)     = %d\n", square(6));
    std::printf("square(2.5)   = %f\n", square(2.5));
    // square("hi") would fail to compile: const char* does not satisfy Numeric.
    // The compiler rejects it right at the call site, naming the concept
    // that failed, instead of failing deep inside square's body with a
    // stack of substitution-failure noise.
}
