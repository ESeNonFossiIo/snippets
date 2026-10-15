#include <chrono>
#include <cstdio>
#include <semaphore>
#include <thread>
#include <vector>

// A semaphore is just a counter with two atomic operations: acquire()
// ("P", decrement -- blocks/sleeps if the counter is already 0) and
// release() ("V", increment -- wakes a blocked waiter if any). Unlike
// condition_variable, there's no separate mutex/predicate to manage: the
// count itself *is* the shared state, and it's synchronized internally.
//
// Where a condition_variable answers "wait until some arbitrary predicate
// becomes true", a semaphore answers the narrower question "wait until
// there are fewer than N things using this resource right now" -- which is
// exactly the shape of a *bounded resource pool*: at most N threads may
// hold the resource concurrently, and any thread may release it for any
// other to acquire (no per-thread ownership, unlike a mutex).
//
// std::counting_semaphore<N> was added in C++20 specifically to give this
// common pattern a small, purpose-built primitive instead of re-deriving it
// from mutex + condition_variable + a manual counter + predicate every
// time.

// Number of worker threads that will run concurrently, each contending for the shared pool.
constexpr std::ptrdiff_t kMaxConcurrentWorkers = 3;

//
std::counting_semaphore<kMaxConcurrentWorkers> pool_slots(kMaxConcurrentWorkers);

void worker(int id)
{
    pool_slots.acquire();  // take a slot; blocks if all kMaxConcurrentWorkers are taken.

    printf("worker %d: acquired a slot, working...\n", id);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printf("worker %d: done\n", id);

    pool_slots.release();  // give the slot back so another worker can proceed.
}

int main()
{
    constexpr int kNumWorkers = 8;
    std::vector<std::thread> workers;
    workers.reserve(kNumWorkers);

    for (int i = 1; i <= kNumWorkers; ++i) {
        workers.emplace_back(worker, i);
    }
    for (auto& t : workers) {
        t.join();
    }
}
