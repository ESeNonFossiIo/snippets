# C++ Concurrency

Progressive examples of C++ multithreading, from raw `std::thread` up
through the higher-level tools the standard library added later to solve
specific pain points that raw threads and mutexes leave open.

| File | What it demonstrates |
|------|------------------------|
| [`00_spawn_100_threads.cpp`](00_spawn_100_threads.cpp) | Spawning N raw `std::thread`s, joining them, an `std::atomic<int>` used as a simple completion counter |
| [`01_threads.cpp`](01_threads.cpp) | `std::async` + `std::future`: run a task in the background, poll it with `wait_for` |
| [`02_condition_variable.cpp`](02_condition_variable.cpp) | Producer/consumer with `mutex` + `condition_variable` + queue: sleep instead of busy-poll |
| [`03_semaphore.cpp`](03_semaphore.cpp) | `std::counting_semaphore`: throttle how many threads may hold a bounded resource at once |
| [`04_binary_semaphore.cpp`](04_binary_semaphore.cpp) | The same producer/consumer as `02`, reimplemented as a mutex-free semaphore handoff |
| [`05_atomic.cpp`](05_atomic.cpp) | A data race vs. `std::atomic`; the acquire/release pattern for publishing data across threads |
| [`06_thread_pool.cpp`](06_thread_pool.cpp) | A minimal thread pool: fixed workers pulling `std::function<void()>` tasks off `02`'s cv-protected queue |
| [`07_async.cpp`](07_async.cpp) | `std::async` launch policies, fanning out to many futures, exception propagation through `.get()` |
| [`08_shared_mutex.cpp`](08_shared_mutex.cpp) | `std::shared_mutex`: many concurrent readers, one exclusive writer |
| [`09_call_once.cpp`](09_call_once.cpp) | `std::call_once`/`std::once_flag`: thread-safe one-time initialization |
| [`10_latch_barrier.cpp`](10_latch_barrier.cpp) | `std::latch` (one-shot rendezvous) vs. `std::barrier` (reusable, phased rendezvous) |
| [`11_jthread.cpp`](11_jthread.cpp) | `std::jthread` + `std::stop_token`: auto-joining threads with cooperative cancellation |
| [`12_deadlock_avoidance.cpp`](12_deadlock_avoidance.cpp) | How two mutexes locked in opposite order deadlock, and how `std::scoped_lock` avoids it |
| [`13_false_sharing.cpp`](13_false_sharing.cpp) | Two atomics on the same cache line silently serializing unrelated threads, fixed with `alignas(64)` padding |
| [`14_packaged_task.cpp`](14_packaged_task.cpp) | `std::packaged_task`: wrap any callable so calling it also feeds a `std::future` — the building block under `std::async` and under a real thread pool's `submit()` |
| [`15_targeted_wakeup.cpp`](15_targeted_wakeup.cpp) | Why a shared semaphore can't target a specific thread, and the one-`binary_semaphore`-per-worker pattern that can |
| [`16_jthread_stop_interface.cpp`](16_jthread_stop_interface.cpp) | `std::jthread`'s three explicit stop-related functions: `request_stop()`, `get_stop_token()` (+ `stop_callback`), `get_stop_source()` |

## Building

```bash
make all      # build every example
make test     # build, then run every example
make clean    # remove build artefacts
```

Built with `g++ -std=c++20 -Wall -Wextra -O2 -pthread` (GCC on Fedora
here); every file compiles warning-free.

## Why each tool exists, and when to reach for it

### `std::thread` (`00`)
The lowest-level primitive: one real OS thread per `std::thread`. Fine for
a handful of long-lived workers; expensive to spawn thousands of them for
short-lived tasks (see `06`, thread pools, for the fix).

### `std::async` / `std::future` (`01`, `07`)
The highest-level tool: run a callable, get a `future<T>` back to collect
its result later, without touching a thread, a shared variable, or any
locking yourself. `std::launch::async` forces immediate concurrent
execution; `std::launch::deferred` runs lazily on whichever thread calls
`.get()`/`.wait()`; leaving the policy unspecified lets the implementation
choose either, which is rarely what you want. Exceptions thrown inside the
task are captured and rethrown from `.get()`, so no manual try/catch needs
to cross the thread boundary.

### `std::mutex` + `std::condition_variable` (`02`)
A mutex alone only answers "is this locked?" — it can't make a thread wait
efficiently for an arbitrary condition ("is the queue non-empty?") without
busy-polling (lock, check, unlock, repeat — burning a whole core). A
`condition_variable` lets a thread sleep (releasing the mutex while
asleep) until `notify_one()`/`notify_all()` wakes it, at which point it
re-acquires the mutex and re-checks a predicate to guard against spurious
wakeups. This pair is the general-purpose tool everything else on this
list specializes.

### `std::counting_semaphore` / `std::binary_semaphore` (`03`, `04`)
A semaphore is a counter with two atomic operations: `acquire()` (blocks
if the count is 0) and `release()` (increments, waking a blocked waiter).
It answers a narrower question than a condition variable: "wait until
fewer than N things are using this resource" — exactly the shape of a
bounded resource pool, with no separate mutex or predicate to manage, and
no ownership (any thread may `release()` a slot a *different* thread
`acquire()`d, unlike a mutex). `binary_semaphore` is the N=1 case, often
used as a plain signal/handoff between two threads, as `04` does in place
of `02`'s mutex+cv+queue.

### `std::atomic` (`05`)
Modifying a plain variable from multiple threads without synchronization
is undefined behavior: `counter++` is really read-modify-write, so two
threads can race and silently lose an update. A mutex fixes this but
means locking around every access. `std::atomic<T>` makes the operation a
single indivisible instruction instead — no blocking, no mutex — and its
memory-order parameter (`relaxed`, `acquire`, `release`, ...) controls
what *other* memory effects are guaranteed visible around it, which is
what lets one thread safely "publish" ordinary (non-atomic) data to
another by gating it behind an atomic flag.

### Thread pools (`06`) and `std::packaged_task` (`14`)
Spawning a `std::thread` per task doesn't scale to many small, short-lived
jobs. A thread pool starts a small fixed set of workers once and has them
pull work off a shared queue — `06` reuses `02`'s exact mutex+cv+queue
pattern, just generalized from "int work items" to "any callable".
`std::packaged_task` is the piece that lets a submitted task hand back a
*result*: it wraps a callable so that calling it also stores its return
value (or exception) into a linked `future`, without deciding anything
about *how or where* it runs — the piece `std::async` builds on top of,
one level up.

### `std::shared_mutex` (`08`)
An ordinary mutex is all-or-nothing, which is wasteful when data is read
far more often than written: concurrent readers don't conflict with each
other at all. `std::shared_mutex` lets any number of threads hold it in
shared/read mode simultaneously, while a writer requesting exclusive mode
waits for all current readers and blocks new ones.

### `std::call_once` (`09`)
Guarding a `if (!initialized)` check with a plain mutex works but means
every *subsequent* access pays for locking a mutex forever just to check
a flag that will never change again. `std::call_once` guarantees the
initializer runs exactly once across all threads, with a cheap check on
every call after the first.

### `std::latch` / `std::barrier` (`10`)
Both are rendezvous points ("wait until N threads all arrive here") — a
different shape than a semaphore's "wait for a free resource slot". A
`latch`'s counter only ever counts down and is single-use: once it hits
zero, every `wait()` returns forever. A `barrier` is reusable: once all N
threads arrive it resets automatically for the next phase, useful for
running in lockstep across repeated rounds.

### `std::jthread` + `std::stop_token` (`11`)
Plain `std::thread` has two footguns: forgetting to `join()` before it
goes out of scope calls `std::terminate`, and there's no standard way to
ask a running thread to stop early. `jthread`'s destructor automatically
requests a stop and joins; if the callable takes a `std::stop_token`, the
`jthread` supplies one automatically for cooperative cancellation.

### Deadlock avoidance (`12`)
Two mutexes locked in opposite order by two threads can deadlock: each
holds one and waits forever for the other. `std::scoped_lock` (or
`std::lock`) locks an arbitrary number of mutexes as a single atomic step
using an internal ordering/backoff algorithm, so the *argument order*
callers happen to use can no longer cause a deadlock between them.

### False sharing (`13`)
Two logically-independent atomics with no data race between them can
still wreck performance if they land on the same CPU cache line: every
core touching either one fights over exclusive ownership of the whole
line via cache-coherence traffic. Padding each one out to its own cache
line (`alignas(64)`) fixes it — `13` measures roughly an order-of-magnitude
speedup from doing so.
