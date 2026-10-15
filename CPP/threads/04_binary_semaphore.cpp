#include <chrono>
#include <cstdio>
#include <semaphore>
#include <thread>

// std::binary_semaphore is std::counting_semaphore<1> -- a semaphore whose
// count only ever holds 0 or 1. Unlike counting_semaphore<3> in
// 03_semaphore.cpp (which throttles *how many* threads may proceed at
// once), a binary_semaphore is typically used as a plain signal/handoff
// between exactly two threads: "go ahead" or "not yet", one bit of state.
//
// Compare this to 02_condition_variable.cpp, which solves a similar
// producer/consumer handoff using a mutex + condition_variable + explicit
// queue + predicate lambda. Here, each semaphore's count directly *is* the
// "is there a value to hand off" signal, so no mutex, predicate, or queue
// is needed -- only a single shared slot protected implicitly by the two
// semaphores' turn-taking:
//   - slot_empty starts "signaled" (1): the buffer starts empty, producer's turn.
//   - slot_full starts "unsignaled" (0): nothing to consume yet.
// The two acquire()/release() pairs force producer and consumer to
// strictly alternate, one value at a time -- a bounded buffer of size 1.

std::binary_semaphore slot_empty(1);  // signaled: the shared slot is free for the producer to fill.
std::binary_semaphore slot_full(0);   // signaled: the shared slot holds a value ready for the consumer.

int shared_value = 0;

void producer()
{
    for (int i = 1; i <= 5; ++i) {
        slot_empty.acquire();  // wait for the consumer to have taken the previous value.
        shared_value = i;
        printf("producer: wrote %d\n", i);
        slot_full.release();  // signal the consumer that a value is ready.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void consumer()
{
    for (int i = 1; i <= 5; ++i) {
        slot_full.acquire();  // wait for the producer to signal a value is ready.
        int value = shared_value;
        printf("consumer: read %d\n", value);
        slot_empty.release();  // signal the producer that the slot is free again.
    }
}

int main()
{
    std::thread p(producer);
    std::thread c(consumer);
    p.join();
    c.join();
}
