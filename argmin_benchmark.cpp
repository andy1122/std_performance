#include <benchmark/benchmark.h>
#include <algorithm> 
#include <immintrin.h> 
#include <random>    
#include <climits>   

#define N (1 << 16) 

typedef __m256i reg; 

alignas(32) int a[N];  


void initialize_array() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000); 

    for (int i = 0; i < N; ++i) {
        a[i] = dis(gen);
    }
}

int argmin_simd(int *a, int n) {
    const reg eight = _mm256_set1_epi32(8);

    reg cur = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    reg min = _mm256_set1_epi32(INT_MAX);
    reg idx = _mm256_setzero_si256();

    for (int i = 0; i < n; i += 8) {
        reg x = _mm256_load_si256((reg*)&a[i]);
        reg mask = _mm256_cmpgt_epi32(min, x);
        idx = _mm256_blendv_epi8(idx, cur, mask);
        min = _mm256_min_epi32(x, min);
        cur = _mm256_add_epi32(cur, eight);
    }

    int min_arr[8], idx_arr[8];
    
    _mm256_storeu_si256((reg*)min_arr, min);
    _mm256_storeu_si256((reg*)idx_arr, idx);

    int k = 0, m = min_arr[0];

    for (int i = 1; i < 8; i++)
        if (min_arr[i] < m)
            m = min_arr[k = i];

    return idx_arr[k];
}

int argmin_std(int *a, int n) {
    int k = std::min_element(a, a + n) - a;
    return k;
}

static void BM_ArgminSIMD(benchmark::State& state) {
    initialize_array();
    for (auto _ : state) {
        int result = argmin_simd(a, N);
        benchmark::DoNotOptimize(result);
    }
}

static void BM_ArgminStd(benchmark::State& state) {
    initialize_array();
    for (auto _ : state) {
        int result = argmin_std(a, N);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ArgminSIMD);
BENCHMARK(BM_ArgminStd);

BENCHMARK_MAIN();
