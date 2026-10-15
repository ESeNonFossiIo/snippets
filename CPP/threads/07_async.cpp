#include <chrono>
#include <cstdio>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>

// std::async is the highest-level concurrency tool in the standard
// library: it runs a callable and hands back a std::future<T> to collect
// its result later, without manually creating a std::thread, a shared
// result variable, or any synchronization -- the future itself handles it.
// 01_threads.cpp already used one future to poll a single background task;
// this example goes further: launch policies, fanning out to many futures,
// and exception propagation.

int compute_square(int x)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return x * x;
}

int divide(int a, int b)
{
    if (b == 0) {
        throw std::invalid_argument("division by zero");
    }
    return a / b;
}

int main()
{
    // std::launch::async forces a new thread to run immediately.
    // std::launch::deferred instead runs lazily, on the calling thread,
    // only when .get()/.wait() is first called -- no concurrency at all.
    // Leaving the policy unspecified (plain std::async(f, ...)) lets the
    // implementation pick either, which is rarely what you want since it
    // makes "does this actually run concurrently?" implementation-defined.
    std::future<int> eager = std::async(std::launch::async, compute_square, 6);
    std::future<int> lazy = std::async(std::launch::deferred, compute_square, 7);

    printf("eager result: %d\n", eager.get());  // was already running in the background.
    printf("lazy result:  %d\n", lazy.get());   // only runs now, on this thread.

    // Fan out several independent tasks at once: each std::async call
    // starts its own thread, so all of these run concurrently.
    std::vector<std::future<int>> futures;
    for (int i = 1; i <= 5; ++i) {
        futures.push_back(std::async(std::launch::async, compute_square, i));
    }
    int sum = 0;
    for (auto& f : futures) {
        sum += f.get();  // blocks until that particular task finishes.
    }
    printf("sum of squares 1..5 = %d\n", sum);

    // An exception thrown inside the async task is captured by the future
    // and rethrown from .get() on whichever thread calls it -- no need for
    // manual try/catch plumbing across the thread boundary.
    std::future<int> will_throw = std::async(std::launch::async, divide, 10, 0);
    try {
        will_throw.get();
    } catch (const std::invalid_argument& e) {
        printf("caught from future: %s\n", e.what());
    }
}
