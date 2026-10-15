#include <cstdio>
#include <mutex>
#include <thread>
#include <vector>

// Lazily initializing a shared resource (a singleton, a cache, a logger)
// the first time it's needed is a common pattern -- but "the first time"
// is ambiguous when many threads can race to be first. Guarding an
// `if (!initialized)` check with an ordinary mutex works, but every
// *subsequent* access still pays the cost of locking that mutex forever,
// just to check a flag that will never change again. std::call_once +
// std::once_flag exist specifically for this: the standard library
// guarantees the callable runs exactly once, and after that, later calls
// on other threads are only a cheap already-done check -- no mutex
// contention once initialization has happened.

std::once_flag init_flag;
int* shared_resource = nullptr;

void ensure_initialized()
{
    std::call_once(init_flag, [] {
        printf("initializing shared_resource (this prints exactly once)\n");
        shared_resource = new int(100);
    });
}

void worker(int id)
{
    ensure_initialized();  // safe to call from every thread; only the first call actually runs the lambda.
    printf("worker %d: shared_resource = %d\n", id, *shared_resource);
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 1; i <= 8; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    delete shared_resource;
}
