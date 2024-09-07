#include <benchmark/benchmark.h>
#include <algorithm> 
#include <immintrin.h> 
#include <random>    
#include <iostream>

#define N (1 << 12) 

alignas(32) int a[N];

void initialize_array() {
    for (int i = 0; i < N; ++i) {
        a[i] = i;
    }
}

int get_random_needle() {
    static std::mt19937 rng(std::random_device{}()); // Random number generator
    static std::uniform_int_distribution<int> dist(0, N - 1);
    return dist(rng);
}

int find_std(int* arr, int x, int size) { 
    int ind = std::find(arr, arr + size, x) - arr;
    return (ind != size) ? ind : -1;
}

typedef __m256i reg;

int find_simd(int needle) {
    reg x = _mm256_set1_epi32(needle);

    for (int i = 0; i < N; i += 8) {
        reg y = _mm256_load_si256((reg*)&a[i]);
        reg m = _mm256_cmpeq_epi32(x, y);
        int mask = _mm256_movemask_ps((__m256)m);
        if (mask != 0)
            return i + __builtin_ctz(mask);
    }

    return -1;
}

static void BM_FindStd(benchmark::State& state) {
    initialize_array(); 

    for (auto _ : state) {
        int needle = get_random_needle(); 
        benchmark::DoNotOptimize(find_std(a, needle, N));
    }
}

static void BM_FindSIMD(benchmark::State& state) {
    initialize_array();

    for (auto _ : state) {
        int needle = get_random_needle();
        benchmark::DoNotOptimize(find_simd(needle));
    }
}

BENCHMARK(BM_FindStd);
BENCHMARK(BM_FindSIMD);

BENCHMARK_MAIN();
