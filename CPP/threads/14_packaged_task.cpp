#include <cstdio>
#include <future>
#include <queue>
#include <thread>
#include <vector>

// std::async (07_async.cpp) is convenient but opinionated: it decides when
// and where the task runs (a launch policy) and owns the thread itself.
// std::packaged_task sits one level lower: it just wraps *any* callable so
// that calling it also stores its return value (or exception) into a
// linked std::future -- it says nothing about threads at all. That's
// exactly the piece needed to build your own scheduler (06_thread_pool.cpp
// only handed back void; a real thread pool's submit() wants to return a
// future the caller can query for a result).

int add(int a, int b)
{
    return a + b;
}

int main()
{
    // Bare packaged_task: wrap add, get its future, run it, read the future.
    std::packaged_task<int(int, int)> task(add);
    std::future<int> result = task.get_future();

    std::thread t(std::move(task), 3, 4);  // packaged_task itself decides nothing about *how* it runs.
    t.join();
    printf("packaged_task result: %d\n", result.get());

    // A minimal task queue built from packaged_task: unlike
    // 06_thread_pool.cpp's std::function<void()> tasks, each submission
    // here returns a future the caller can use to retrieve a real result.
    std::queue<std::packaged_task<int()>> task_queue;
    std::vector<std::future<int>> futures;

    for (int i = 1; i <= 3; ++i) {
        std::packaged_task<int()> queued_task([i] { return i * i; });
        futures.push_back(queued_task.get_future());
        task_queue.push(std::move(queued_task));
    }

    while (!task_queue.empty()) {
        std::packaged_task<int()> next = std::move(task_queue.front());
        task_queue.pop();
        next();  // runs synchronously here, on the calling thread; a real pool would hand this to a worker.
    }

    for (auto& f : futures) {
        printf("queued task result: %d\n", f.get());
    }
}
