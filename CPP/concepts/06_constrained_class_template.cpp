#include <array>
#include <concepts>
#include <cstdio>
#include <cstddef>
#include <string>
#include <utility>

template <std::movable T, std::size_t Capacity>
class FixedStack {
public:
    // Member function template with its own trailing requires clause: U just
    // needs to be convertible to T, independent of whatever constrains the
    // class itself.
    template <typename U>
        requires std::convertible_to<U, T>
    void push(U&& value) {
        data_[size_++] = T(std::forward<U>(value));
    }

    T pop() {
        return std::move(data_[--size_]);
    }

    const T& top() const { return data_[size_ - 1]; }
    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }

private:
    std::array<T, Capacity> data_{};
    std::size_t size_ = 0;
};

int main() {
    FixedStack<int, 4> ints;
    ints.push(1);
    ints.push(2.0);   // double -> int, allowed by convertible_to<double, int>
    ints.push(3);
    std::printf("ints: size=%zu top=%d\n", ints.size(), ints.top());
    while (!ints.empty()) std::printf("  pop -> %d\n", ints.pop());

    FixedStack<std::string, 4> strings;
    strings.push("first");
    strings.push(std::string("second"));
    std::printf("strings: size=%zu top=%s\n", strings.size(), strings.top().c_str());
    while (!strings.empty()) std::printf("  pop -> %s\n", strings.pop().c_str());
}
