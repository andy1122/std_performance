#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

constexpr int ITERATIONS = 100'000'000;

struct Counters {
    std::atomic<int> a;
    std::atomic<int> b;
};

struct AlignedCounters {
    alignas(64) std::atomic<int> a;
    alignas(64) std::atomic<int> b;
};

void run_false_sharing() {
    Counters counters;
    auto t1 = std::thread([&] {
        for (int i = 0; i < ITERATIONS; i++) { ++counters.a; }
    });

    auto t2 = std::thread([&] {
        for (int i = 0; i < ITERATIONS; i++) { ++counters.b; }
    });

    t1.join();
    t2.join();
}

void run_no_false_sharing() {
    AlignedCounters counters;

    auto t1 = std::thread([&] {
        for (int i = 0; i < ITERATIONS; i++) { ++counters.a; }
    });

    auto t2 = std::thread([&] {
        for (int i = 0; i < ITERATIONS; i++) { ++counters.b; }
    });

    t1.join();
    t2.join();
}

int main() {
    using clock = std::chrono::high_resolution_clock;

    auto start = clock::now();
    run_false_sharing();
    auto end = clock::now();
    std::cout << "With false sharing: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    start = clock::now();
    run_no_false_sharing();
    end = clock::now();
    std::cout << "Without false sharing (alignas): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}
