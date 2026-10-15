#include <barrier>
#include <cstdio>
#include <latch>
#include <thread>
#include <vector>

// Both std::latch and std::barrier (C++20) answer "wait here until N
// threads have all arrived" -- a rendezvous point, distinct from a
// semaphore's "wait until a resource is available". They differ in
// lifetime:
//
//   - std::latch is single-use: its counter only ever counts down. Once it
//     hits zero, every wait() returns forever -- perfect for "wait until
//     all workers have finished their one-time setup phase".
//   - std::barrier is reusable: once all N threads arrive, it resets
//     automatically and can be used again for the next phase -- perfect for
//     "run in lockstep across repeated rounds", like a simulation where no
//     thread may start round 2 until every thread finished round 1.

constexpr int kNumWorkers = 4;

void latch_demo()
{
    std::latch setup_done(kNumWorkers);  // counts down from kNumWorkers to 0, exactly once.

    std::vector<std::thread> workers;
    for (int i = 1; i <= kNumWorkers; ++i) {
        workers.emplace_back([i, &setup_done] {
            printf("worker %d: doing one-time setup\n", i);
            setup_done.count_down();      // this worker's setup is done.
            setup_done.wait();            // block until every worker's setup is done.
            printf("worker %d: setup complete everywhere, proceeding\n", i);
        });
    }
    for (auto& t : workers) {
        t.join();
    }
}

void barrier_demo()
{
    // The completion callback runs once per phase, on one of the arriving
    // threads, right after all kNumWorkers reach the barrier and before any
    // of them is released into the next phase.
    std::barrier sync_point(kNumWorkers, [] { printf("-- all workers finished a round --\n"); });

    std::vector<std::thread> workers;
    for (int i = 1; i <= kNumWorkers; ++i) {
        workers.emplace_back([i, &sync_point] {
            for (int round = 1; round <= 3; ++round) {
                printf("worker %d: round %d\n", i, round);
                sync_point.arrive_and_wait();  // block until all kNumWorkers reach this point, then reset.
            }
        });
    }
    for (auto& t : workers) {
        t.join();
    }
}

int main()
{
    printf("== latch: one-time rendezvous ==\n");
    latch_demo();

    printf("\n== barrier: reusable rendezvous across rounds ==\n");
    barrier_demo();
}
