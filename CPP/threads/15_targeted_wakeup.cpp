#include <chrono>
#include <cstdio>
#include <deque>
#include <semaphore>
#include <thread>
#include <vector>

// A semaphore's release() wakes *some* blocked waiter -- the standard gives
// no guarantee about which one, and no API to name a specific thread to
// release. If you need to signal one particular worker rather than "any
// worker", a single shared semaphore is the wrong tool.
//
// The fix is simple: give each worker its own binary_semaphore. Releasing
// worker[i]'s semaphore can only ever wake worker i, because no other
// thread is waiting on it. This also sidesteps the mismatch questions from
// 03/04 entirely -- each semaphore's count is scoped to exactly one
// consumer, so there's never an "extra" or "missing" release to reconcile
// against a pool of threads.

constexpr int kNumWorkers = 4;

int main()
{
    // One binary_semaphore per worker, all starting unsignaled (0): each
    // worker blocks in acquire() until *its own* semaphore is released.
    // std::deque, not std::vector: binary_semaphore has no copy/move
    // constructor, and growing a vector requires relocating existing
    // elements by move/copy; a deque never relocates elements it already
    // holds when it grows, so emplace_back works with no such requirement.
    std::deque<std::binary_semaphore> doorbell;
    for (int i = 0; i < kNumWorkers; ++i) {
        doorbell.emplace_back(0);
    }

    std::vector<std::thread> workers;
    workers.reserve(kNumWorkers);
    for (int i = 0; i < kNumWorkers; ++i) {
        workers.emplace_back([i, &doorbell] {
            doorbell[i].acquire();  // sleeps until *this* worker's doorbell rings.
            printf("worker %d: woke up\n", i);
        });
    }

    // Release a chosen subset, in a chosen order -- something a shared
    // counting_semaphore could never express, since it has no notion of
    // *which* waiter a given release() reaches.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    printf("main: waking worker 2\n");
    doorbell[2].release();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    printf("main: waking worker 0\n");
    doorbell[0].release();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    printf("main: waking workers 1 and 3\n");
    doorbell[1].release();
    doorbell[3].release();

    for (auto& t : workers) {
        t.join();
    }
}
