#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

constexpr int kDefaultNumThreads = 10;

void worker(const std::size_t i,       //< [in] thread index.
            std::atomic<int>& counter  //< [in,out] atomic counter incremented by each thread when it finishes.
)
{
    std::size_t s = 0;
    printf("\n n.%zu", i);
    for (std::size_t j = 0; j <= 10'000'000; ++j) {
        s += j;
    }
    printf("\n end // %zu, sum = %zu", i, s);
    counter.fetch_add(1, std::memory_order_relaxed);
    printf("\n end // %zu, sum = %zu", i, s);
}

int main(int argc, char** argv)
{
    const unsigned int nConcThreads = std::thread::hardware_concurrency();
    printf("\n core count = %d\n", nConcThreads);

    const int num_threads = argc > 1 ? std::atoi(argv[1]) : kDefaultNumThreads;

    // print number of supported threads
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i, std::ref(counter));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nall " << counter.load() << " threads completed\n";
}
