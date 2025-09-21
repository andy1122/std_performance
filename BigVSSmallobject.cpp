#include <benchmark/benchmark.h>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>

struct SmallOrder {
    int price;
    int quantity;
};

struct BigOrder {
    long long order_id;
    int price;
    int quantity;
    std::string broker;
    std::string notes;
    std::string trader_name;
    std::string exchange;
};

struct OrderMeta {
    std::string broker;
    std::string notes;
    std::string trader_name;
    std::string exchange;
};

struct Order {
    long long order_id;
    int price;
    int quantity;
    std::unique_ptr<OrderMeta> meta;
};

template <typename T>
static void BM_SumQuantity(benchmark::State& state) {
    const size_t N = state.range(0);
    std::vector<T> orders(N);
    for (auto& o : orders) {
        o.price = std::rand() % 1000;
        o.quantity = std::rand() % 100;
    }

    for (auto _ : state) {
        int sum = 0;
        for (const auto& o : orders) {
            sum += o.quantity;
        }
        benchmark::DoNotOptimize(sum);
    }
}

BENCHMARK_TEMPLATE(BM_SumQuantity, SmallOrder)->Arg(1'000'000)->Unit(benchmark::kMillisecond);;
BENCHMARK_TEMPLATE(BM_SumQuantity, BigOrder)->Arg(1'000'000)->Unit(benchmark::kMillisecond);;
BENCHMARK_TEMPLATE(BM_SumQuantity, Order)->Arg(1'000'000)->Unit(benchmark::kMillisecond);;

BENCHMARK_MAIN();
