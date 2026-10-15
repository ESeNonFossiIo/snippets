#include <chrono>
#include <cstdio>
#include <stop_token>
#include <thread>

// std::thread has two long-standing footguns: forgetting to join() it
// before it goes out of scope calls std::terminate, and there's no
// standard way to ask a running thread to stop early -- you have to
// build your own atomic<bool> flag and thread it through manually.
// std::jthread (C++20, "joining thread") fixes both: its destructor
// automatically calls request_stop() then join(), and if the callable's
// first parameter is a std::stop_token, the jthread passes one in
// automatically, letting the task cooperatively check "should I stop?"
// without any hand-rolled flag.

void periodic_task(std::stop_token stop_token)
{
    int tick = 0;

    while (!stop_token.stop_requested()) {  // cooperative: the task itsaelf decides when to check.
        printf("tick %d\n", ++tick);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    printf("periodic_task: stop requested, exiting cleanly\n");
}

int main()
{
    {
        std::jthread t(periodic_task);  // starts immediately, stop_token supplied automatically.
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
        // No explicit request_stop()/join() call needed: leaving this scope
        // does both automatically, in the jthread's destructor.
    }
    printf("main: jthread has been stopped and joined\n");
}
