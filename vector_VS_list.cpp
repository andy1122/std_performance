#include <benchmark/benchmark.h>
#include <vector>
#include <list>
#include <numeric> // For std::iota to initialize vectors and lists

// Size of the container
const size_t N = 1 << 18; // 1 million elements

// Initialize a vector with N elements
static void BM_VectorIteration(benchmark::State& state) {
    std::vector<int> vec(N);
    std::iota(vec.begin(), vec.end(), 0); // Fill vector with 0, 1, 2, ...

    for (auto _ : state) {
        int sum = 0;
        for (const auto& elem : vec) {
            benchmark::DoNotOptimize(sum += elem);
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_VectorIteration);

// Initialize a list with N elements
static void BM_ListIteration(benchmark::State& state) {
    std::list<int> lst(N);
    std::iota(lst.begin(), lst.end(), 0); // Fill list with 0, 1, 2, ...

    for (auto _ : state) {
        int sum = 0;
        for (const auto& elem : lst) {
            benchmark::DoNotOptimize(sum += elem);
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_ListIteration);

BENCHMARK_MAIN();
