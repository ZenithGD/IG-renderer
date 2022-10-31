#include <chrono>
#include <functional>

template <typename Unit>
uint64_t measureTime(std::function<void()> f) {
    auto begin = std::chrono::high_resolution_clock::now();

    // measure execution time
    f();

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<Unit>(end-begin).count();
}