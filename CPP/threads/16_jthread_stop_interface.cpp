#include <chrono>
#include <cstdio>
#include <stop_token>
#include <thread>

// 11_jthread.cpp only shows the *implicit* path: leaving scope calls
// request_stop() then join() automatically, and the callable's stop_token
// parameter is supplied behind the scenes. This example exercises the
// three stop-related member functions std::jthread exposes explicitly:
//
//   - request_stop()   ask the jthread's task to stop, right now, on demand.
//   - get_stop_token()  hand out a *read-only* token: can only be polled or
//                        used to register a stop_callback, never used to
//                        request a stop itself.
//   - get_stop_source() hand out a *copyable, independent* stop_source:
//                        anyone holding a copy can call request_stop() on
//                        it, and it keeps working even after the jthread
//                        that created it is gone (a stop_source has its own
//                        shared state, not tied to the jthread's lifetime).

void periodic_task(std::stop_token stop_token)
{
    int tick = 0;
    while (!stop_token.stop_requested()) {
        printf("periodic_task: tick %d\n", ++tick);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    printf("periodic_task: stop requested, exiting cleanly\n");
}

int main()
{
    // 1. request_stop() -- ask explicitly, instead of waiting for the
    //    destructor to do it when the jthread goes out of scope.
    {
        std::jthread t(periodic_task);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printf("main: calling request_stop() explicitly\n");
        t.request_stop();
        // t's destructor still runs join() here -- request_stop() alone
        // doesn't block; it only flips the flag the task is polling.
    }

    printf("\n");

    // 2. get_stop_token() -- a read-only view of the stop state, for code
    //    that needs to *react* to a stop request but isn't the jthread's
    //    task itself (here: a stop_callback that fires the instant
    //    request_stop() is called on any associated stop_source, from a
    //    thread other than the one polling stop_requested()).
    {
        std::jthread t(periodic_task);
        std::stop_callback on_stop(t.get_stop_token(), [] {
            printf("stop_callback: stop was requested, notifying watchdog\n");
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        t.request_stop();  // stop_callback's lambda runs synchronously, right here.
    }

    printf("\n");

    // 3. get_stop_source() -- an independent, copyable handle. Unlike a
    //    stop_token, it can itself call request_stop(); unlike the jthread
    //    object, copies of it can be handed to unrelated code (here: a
    //    plain std::thread with no connection to the jthread otherwise)
    //    that decides when to stop the jthread's task.
    {
        std::jthread t(periodic_task);
        std::stop_source source = t.get_stop_source();  // independent handle to the same stop state.

        std::thread watchdog([source] {  // capture by value: source is a cheap, shared handle.
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            printf("watchdog: requesting stop via its own stop_source copy\n");
            source.request_stop();
        });
        watchdog.join();
        // t's destructor joins the now-stopping task.
    }
}
