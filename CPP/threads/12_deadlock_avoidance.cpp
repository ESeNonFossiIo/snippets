#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

// Classic deadlock: two threads each need both of two mutexes, but acquire
// them in opposite order. Thread A locks mtx_a then blocks trying to lock
// mtx_b; thread B has already locked mtx_b and blocks trying to lock
// mtx_a. Neither can ever proceed -- a deadlock. (See deadlock_prone_swap()
// below, deliberately never called from main: running it hangs forever.)
//
// std::lock (and std::scoped_lock, its RAII wrapper, C++17) fixes this by
// locking an arbitrary number of mutexes together as a single atomic step,
// internally using a lock-ordering/backoff algorithm so that no matter what
// order the *arguments* are given in, two threads locking the same set of
// mutexes can never deadlock against each other.

std::mutex mtx_a;
std::mutex mtx_b;
int resource_a = 0;
int resource_b = 0;

void deadlock_prone_swap()  // NOT called from main -- kept only to show the bug being fixed.
{
    std::lock_guard<std::mutex> lock_a(mtx_a);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // widen the race window.
    std::lock_guard<std::mutex> lock_b(mtx_b);                   // thread B holds mtx_b, waiting on mtx_a: deadlock.
    std::swap(resource_a, resource_b);
}

void safe_swap(std::mutex& first, std::mutex& second, int& value_first, int& value_second)
{
    std::scoped_lock lock(first, second);  // locks both atomically, in a deadlock-free order internally.
    std::swap(value_first, value_second);
}

int main()
{
    std::thread t1([] {
        for (int i = 0; i < 1000; ++i) {
            safe_swap(mtx_a, mtx_b, resource_a, resource_b);  // args in order (a, b) ...
        }
    });
    std::thread t2([] {
        for (int i = 0; i < 1000; ++i) {
            safe_swap(mtx_b, mtx_a, resource_b, resource_a);  // ... vs (b, a) here: would deadlock with plain lock_guards.
        }
    });

    t1.join();
    t2.join();
    printf("finished 1000 concurrent swaps from each thread with no deadlock\n");
    printf("resource_a = %d, resource_b = %d (both start at 0, so they stay 0 or swap back and forth)\n", resource_a,
           resource_b);
}
