#include <benchmark/benchmark.h>
#include <atomic>
#include <array>
#include <thread>
#include <cstdint>
#include "RingBuffer/seq_spsc.h"

// ============================================================
// 1. 单线程：只测 push 的原始开销（队列永远不满）
// ============================================================
static void BM_RingBuffer_PushOnly(benchmark::State& state) {
    RingBufferSPSC<1024> rb;
    uint32_t v = 0;
    for (auto _ : state) {
        // 队列容量 1024，每轮 push 一次、pop 一次，保证不会满
        rb.push(v++);
        uint32_t out;
        rb.pop(out);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK(BM_RingBuffer_PushOnly);

// ============================================================
// 2. 单线程：push + pop 成对，测单线程下的往返开销
// ============================================================
static void BM_RingBuffer_PingPong(benchmark::State& state) {
    RingBufferSPSC<1024> rb;
    uint32_t out = 0;
    for (auto _ : state) {
        rb.push(42);
        bool ok = rb.pop(out);
        benchmark::DoNotOptimize(ok);
        benchmark::DoNotOptimize(out);
    }
}
BENCHMARK(BM_RingBuffer_PingPong);

// ============================================================
// 3. 多线程：真正的 SPSC 吞吐测试
//    生产者线程不停 push，消费者线程不停 pop，
//    用计数器统计成功次数，最后算吞吐。
// ============================================================
static void BM_RingBuffer_SPSC_Throughput(benchmark::State& state) {
    const size_t max_size = state.range(0);   // 队列容量
    const size_t total_ops = state.range(1);  // 总操作次数

    for (auto _ : state) {
        // 每轮迭代重建队列，保证状态干净
        RingBufferSPSC<1024>* rb = new RingBufferSPSC<1024>();

        std::atomic<bool> start{false};
        std::atomic<uint64_t> produced{0};
        std::atomic<uint64_t> consumed{0};

        std::thread producer([&]() {
            while (!start.load(std::memory_order_acquire)) {}
            uint32_t v = 0;
            while (produced.load(std::memory_order_relaxed) < total_ops) {
                if (rb->push(v++)) {
                    produced.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });

        std::thread consumer([&]() {
            while (!start.load(std::memory_order_acquire)) {}
            uint32_t out;
            while (consumed.load(std::memory_order_relaxed) < total_ops) {
                if (rb->pop(out)) {
                    consumed.fetch_add(1, std::memory_order_relaxed);
                }
            }
        });

        start.store(true, std::memory_order_release);
        producer.join();
        consumer.join();

        delete rb;

        // 告诉 benchmark：这一轮实际处理了 total_ops 个元素
        state.SetItemsProcessed(total_ops);
    }
}
// 参数含义：队列容量固定 1024（模板参数），只扫总操作次数
BENCHMARK(BM_RingBuffer_SPSC_Throughput)
    ->ArgPair(1024, 100000)
    ->ArgPair(1024, 1000000)
    ->ArgPair(1024, 10000000);

BENCHMARK_MAIN();