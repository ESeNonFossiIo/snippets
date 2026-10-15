#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <queue>
#include <thread>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> work_queue;
bool producer_done = false;

void producer()
{
    for (int i = 1; i <= 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            work_queue.push(i);
            printf("producer: pushed %d\n", i);
        }
        cv.notify_one();  // wake the consumer: there is new work waiting.
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        producer_done = true;
    }
    cv.notify_one();  // wake the consumer one last time so it can see we're done.
}

void consumer()
{
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Without the condition_variable, the only way to wait for work would
        // be a busy-wait: loop, lock the mutex, check the queue, unlock,
        // repeat -- burning a whole CPU core just to poll. cv.wait() instead
        // puts the thread to sleep (releasing the mutex while it sleeps) and
        // only wakes it up when notify_one()/notify_all() is called, at
        // which point it re-acquires the mutex and re-checks the predicate
        // (the lambda) to guard against spurious wakeups.
        cv.wait(lock, [] { return !work_queue.empty() || producer_done; });

        if (!work_queue.empty()) {
            int value = work_queue.front();
            work_queue.pop();
            lock.unlock();  // don't hold the mutex while "processing" the value.
            printf("consumer: popped %d\n", value);
        } else if (producer_done) {
            break;
        }
    }
    printf("consumer: done, queue drained\n");
}

int main()
{
    std::thread p(producer);
    std::thread c(consumer);
    p.join();
    c.join();
}
