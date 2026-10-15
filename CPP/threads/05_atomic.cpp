#include <atomic>
#include <cstdio>
#include <thread>
#include <vector>

// A plain `int` shared between threads is undefined behavior to modify
// concurrently: `counter++` is really read-modify-write (three steps), so
// two threads can both read the same old value before either writes back,
// silently losing an increment (a "lost update" / data race). Fixing this
// with a mutex works but means locking/unlocking around every single
// increment. std::atomic<T> instead makes the read-modify-write a single
// indivisible hardware instruction (e.g. `lock xadd` on x86) -- no mutex,
// no blocking, just a guarantee that concurrent operations on it can never
// interleave.

constexpr int kNumThreads = 8;
constexpr int kIncrementsPerThread = 100'000;

int unsafe_counter = 0;
std::atomic<int> safe_counter{0};

void increment_both()
{
    for (int i = 0; i < kIncrementsPerThread; ++i) {
        unsafe_counter++;                                 // data race: UB, increments get lost.
        safe_counter.fetch_add(1, std::memory_order_relaxed);  // atomic: every increment counts.
    }
}

int main()
{
    {
        std::vector<std::thread> threads;
        threads.reserve(kNumThreads);
        for (int i = 0; i < kNumThreads; ++i) {
            threads.emplace_back(increment_both);
        }
        for (auto& t : threads) {
            t.join();
        }

        const int expected = kNumThreads * kIncrementsPerThread;
        printf("expected:      %d\n", expected);
        printf("unsafe_counter: %d (lost %d increments to the data race)\n", unsafe_counter,
               expected - unsafe_counter);
        printf("safe_counter:   %d\n", safe_counter.load());
    }

    // A second common atomic pattern: publishing data from one thread to
    // another without a mutex, using the atomic flag itself as the
    // synchronization point. memory_order_release on the write and
    // memory_order_acquire on the read guarantee that once the reader
    // observes ready == true, it also sees every write the writer made
    // *before* setting ready -- not just the flag itself.
    int payload = 0;
    std::atomic<bool> ready{false};

    std::thread writer([&] {
        payload = 42;                              // ordinary write, not itself atomic.
        ready.store(true, std::memory_order_release);  // publishes payload to any acquire-reader.
    });
    std::thread reader([&] {
        while (!ready.load(std::memory_order_acquire)) {
            // spin until the writer publishes.
        }
        printf("reader: saw payload = %d\n", payload);  // guaranteed visible, not a race.
    });
    writer.join();
    reader.join();
}
