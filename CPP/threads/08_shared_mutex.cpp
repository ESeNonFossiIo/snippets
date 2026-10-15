#include <chrono>
#include <cstdio>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

// A plain std::mutex is all-or-nothing: only one thread may hold it at a
// time, whether it's reading or writing. That's wasteful for data that's
// read far more often than it's written (caches, config, lookup tables):
// concurrent readers don't conflict with each other at all, only with a
// writer. std::shared_mutex (C++17) distinguishes the two: any number of
// threads may hold it in "shared" mode (std::shared_lock) simultaneously,
// but a thread requesting "exclusive" mode (std::unique_lock/lock_guard)
// blocks until every reader releases it, and blocks new readers while it
// holds the lock.

std::shared_mutex mtx;
int shared_data = 0;

void reader(int id)
{
    std::shared_lock lock(mtx);  // shared: other readers may hold this concurrently.
    printf("reader %d: sees %d\n", id, shared_data);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));  // held long enough to overlap with other readers.
}

void writer()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // let the readers start first.
    std::unique_lock lock(mtx);  // exclusive: waits for all current readers, blocks new ones.
    shared_data = 42;
    printf("writer: updated shared_data to %d\n", shared_data);
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 1; i <= 4; ++i) {
        threads.emplace_back(reader, i);
    }
    threads.emplace_back(writer);
    for (int i = 5; i <= 8; ++i) {
        threads.emplace_back(reader, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}
