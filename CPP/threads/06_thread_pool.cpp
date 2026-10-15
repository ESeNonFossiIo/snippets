#include <condition_variable>
#include <cstdio>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// Spawning a new std::thread per task (00_spawn_100_threads.cpp) is
// expensive: each std::thread is a real OS thread, and creating/destroying
// one has real syscall overhead -- fine for 8 long-lived workers, wasteful
// for thousands of small, short-lived jobs. A thread pool ("scheduler")
// solves this by starting a small, fixed set of worker threads once, and
// having them pull tasks off a shared queue for as long as the pool lives
// -- the same building blocks as 02_condition_variable.cpp (mutex + cv +
// queue), just generalized from "int work items" to "any callable".

class ThreadPool
{
public:
    explicit ThreadPool(std::size_t num_workers)
    {
        workers_.reserve(num_workers);
        for (std::size_t i = 0; i < num_workers; ++i) {
            workers_.emplace_back([this, i] { worker_loop(i); });
        }
    }

    // Not copyable/movable: worker threads capture `this` by reference in their loop.
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stopping_ = true;
        }
        cv_.notify_all();  // wake every worker so it can see stopping_ and exit.
        for (auto& t : workers_) {
            t.join();
        }
    }

    void submit(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.push(std::move(task));
        }
        cv_.notify_one();  // wake exactly one idle worker: exactly one task became available.
    }

private:
    void worker_loop(std::size_t worker_id)
    {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx_);
                cv_.wait(lock, [this] { return !tasks_.empty() || stopping_; });

                if (tasks_.empty() && stopping_) {
                    return;  // no more work and shutting down: exit the loop.
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }
            printf("worker %zu: running task\n", worker_id);
            task();
        }
    }

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stopping_ = false;
};

int main()
{
    ThreadPool pool(3);  // fixed pool of 3 worker threads, reused across all submitted tasks.

    for (int i = 1; i <= 9; ++i) {
        pool.submit([i] {
            std::size_t s = 0;
            for (std::size_t j = 0; j <= 1'000'000; ++j) {
                s += j;
            }
            printf("task %d: sum = %zu\n", i, s);
        });
    }

    // Destructor blocks here until every submitted task has been picked up
    // and run, then joins the worker threads.
}
