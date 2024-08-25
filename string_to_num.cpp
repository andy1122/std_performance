#include <iostream>
#include <string>
#include <cmath>
#include <benchmark/benchmark.h>
#define RUN_BENCHMARK 1


// Optimized string_to_numeric function
int string_to_numeric(std::string& str) {
    int sum = 0;
    for (char x : str) {
        sum = sum * 10 + (x - '0');
    }
    return sum;
}

int std_stoi(std::string& str) {
    return std::stoi(str);
}

// Benchmark for string_to_numeric function
static void bench_string_to_numeric(benchmark::State& state) {
    std::string str = "2147483647"; // Same length string for fair comparison
    for (auto _ : state) {
        benchmark::DoNotOptimize(string_to_numeric(str));
    }
}

// Benchmark for std::stoi function
static void bench_std_stoi(benchmark::State& state) {
    std::string str = "2147483647"; // Same length string for fair comparison
    for (auto _ : state) {
        benchmark::DoNotOptimize(std_stoi(str));
    }
}

// Register the benchmarks
#if RUN_BENCHMARK == 1

BENCHMARK(bench_string_to_numeric);
BENCHMARK(bench_std_stoi);
BENCHMARK_MAIN();

#elif RUN_BENCHMARK == 0

int main() {
    std::string str = "2147483647";
    int val = string_to_numeric(str);
    int val2 = std_stoi(str);
    std::cout << val << " " << val2 << std::endl;
    bool temp =  val == val2;
    std::cout << temp << std::endl;
}

#endif