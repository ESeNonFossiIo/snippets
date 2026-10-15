#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>
#include <vector>

// Two atomics are logically independent -- no data race, no shared value --
// but the CPU cache doesn't work at the granularity of individual
// variables, it works in fixed-size cache lines (64 bytes on most x86_64
// hardware). If two atomics land in the *same* cache line, every core
// touching either one has to fight over exclusive ownership of that whole
// line via the cache-coherence protocol, even though the threads never
// actually touch each other's data. This is "false sharing": correctness
// is unaffected, but performance collapses because every write invalidates
// the line in every other core's cache.
//
// The fix is padding: pad each atomic out to its own cache line with
// `alignas(64)` so unrelated atomics never share one.

constexpr int kIterations = 20'000'000;

struct Unpadded
{
    std::atomic<long> a{0};
    std::atomic<long> b{0};  // very likely on the same 64-byte cache line as `a`.
};

struct alignas(64) PaddedAtomic
{
    std::atomic<long> value{0};
};

template <typename Counters>
long run(Counters& counters)
{
    auto start = std::chrono::steady_clock::now();

    std::thread t1([&] {
        for (int i = 0; i < kIterations; ++i) {
            counters.first.fetch_add(1, std::memory_order_relaxed);
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < kIterations; ++i) {
            counters.second.fetch_add(1, std::memory_order_relaxed);
        }
    });
    t1.join();
    t2.join();

    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main()
{
    struct UnpaddedPair
    {
        std::atomic<long>& first;
        std::atomic<long>& second;
    };
    Unpadded unpadded;
    UnpaddedPair unpadded_pair{unpadded.a, unpadded.b};
    long unpadded_ms = run(unpadded_pair);
    printf("two atomics sharing a cache line:   %ld ms\n", unpadded_ms);

    struct PaddedPair
    {
        std::atomic<long>& first;
        std::atomic<long>& second;
    };
    PaddedAtomic padded_a;
    PaddedAtomic padded_b;
    PaddedPair padded_pair{padded_a.value, padded_b.value};
    long padded_ms = run(padded_pair);
    printf("two atomics on separate cache lines: %ld ms\n", padded_ms);
}
