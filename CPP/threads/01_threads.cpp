#include <cxxabi.h>
#include <stdio.h>

#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <stdexcept>

int long_computation()
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 100;
}

int main()
{
    auto future = std::async(std::launch::async, long_computation);

    int status = 0;
    std::unique_ptr<char, decltype(&std::free)> demangled(
        abi::__cxa_demangle(typeid(std::decay_t<decltype(future)>).name(), nullptr, nullptr, &status), std::free);
    printf("\n future type = %s \n", status == 0 ? demangled.get() : typeid(std::decay_t<decltype(future)>).name());

    // Poll status using a 500ms timeout loop
    while (future.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready) {
        std::cout << "Task still processing, main thread working...\n";
    }

    std::cout << "Result ready: " << future.get() << "\n";
}
