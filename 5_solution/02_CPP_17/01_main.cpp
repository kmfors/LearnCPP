#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <memory_resource>
#include <numeric>
#include <vector>

// ============================================================
// 基准测量辅助工具
// ============================================================

// 防止编译器把循环优化掉（类似 Google Benchmark 的 DoNotOptimize）
template <typename T>
inline void DoNotOptimize(T const& value) {
    asm volatile("" : : "r,m"(value) : "memory");
}

template <typename T>
inline void DoNotOptimize(T& value) {
    asm volatile("" : "+r,m"(value) : : "memory");
}

// 计时器：返回单次运行耗时（毫秒）
template <typename F>
double MeasureMs(F&& fn) {
    auto start = std::chrono::steady_clock::now();
    fn();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

// 多次运行取中位数，减少噪声影响
template <typename F>
double MeasureMedianMs(F&& fn, int runs = 5) {
    std::vector<double> samples;
    samples.reserve(runs);
    for (int i = 0; i < runs; ++i) {
        samples.push_back(MeasureMs(fn));
    }
    std::sort(samples.begin(), samples.end());
    return samples[samples.size() / 2];  // 中位数
}

// 统一的测试参数
constexpr int kIterations = 1'000'000;  // 100 万次
constexpr int kVectorSize  = 10;         // 每个 vector 10 个 int

// ============================================================
// 测试 1：全局 new/delete（基准）
// ============================================================
TEST(PmrBenchmark, GlobalNewDelete) {
    double ms = MeasureMedianMs([] {
        for (int i = 0; i < kIterations; ++i) {
            std::vector<int> v;
            v.reserve(kVectorSize);
            v.push_back(i);
            DoNotOptimize(v);
        }
    });

    std::cout << "[Global new/delete] median = " << ms << " ms\n";
    EXPECT_GT(ms, 0.0);  // 至少能跑
}

// ============================================================
// 测试 2：monotonic_buffer_resource
// ============================================================
TEST(PmrBenchmark, MonotonicBufferResource) {
    // 缓冲区放在循环外，覆盖整个测试周期
    // 注意：monotonic 池不释放单块，100 万次分配需要足够大的缓冲区
    // 否则会回落到 new_delete_resource，失去测试意义
    constexpr std::size_t kBufferSize = 256 * 1024 * 1024;  // 256 MB
    auto buffer = std::make_unique<std::byte[]>(kBufferSize);

    double ms = MeasureMedianMs([&] {
        std::pmr::monotonic_buffer_resource pool(
            buffer.get(), kBufferSize, std::pmr::new_delete_resource());

        for (int i = 0; i < kIterations; ++i) {
            std::pmr::vector<int> v(&pool);
            v.reserve(kVectorSize);
            v.push_back(i);
            DoNotOptimize(v);
        }
    });

    std::cout << "[monotonic_buffer_resource] median = " << ms << " ms\n";
    EXPECT_GT(ms, 0.0);
}

// ============================================================
// 测试 3：unsynchronized_pool_resource
// ============================================================
TEST(PmrBenchmark, UnsynchronizedPoolResource) {
    double ms = MeasureMedianMs([] {
        std::pmr::unsynchronized_pool_resource pool;

        for (int i = 0; i < kIterations; ++i) {
            std::pmr::vector<int> v(&pool);
            v.reserve(kVectorSize);
            v.push_back(i);
            DoNotOptimize(v);
        }
    });

    std::cout << "[unsynchronized_pool_resource] median = " << ms << " ms\n";
    EXPECT_GT(ms, 0.0);
}

// ============================================================
// 测试 4：synchronized_pool_resource（多线程场景对照）
// ============================================================
TEST(PmrBenchmark, SynchronizedPoolResource) {
    double ms = MeasureMedianMs([] {
        std::pmr::synchronized_pool_resource pool;

        for (int i = 0; i < kIterations; ++i) {
            std::pmr::vector<int> v(&pool);
            v.reserve(kVectorSize);
            v.push_back(i);
            DoNotOptimize(v);
        }
    });

    std::cout << "[synchronized_pool_resource] median = " << ms << " ms\n";
    EXPECT_GT(ms, 0.0);
}

// ============================================================
// 测试 5：pmr::string 对比（验证 pmr 对字符串同样有效）
// ============================================================
TEST(PmrBenchmark, StringComparison) {
    double std_ms = MeasureMedianMs([] {
        for (int i = 0; i < kIterations / 10; ++i) {
            std::string s = "hello world, this is a test string";
            DoNotOptimize(s);
        }
    });

    double pmr_ms = MeasureMedianMs([] {
        std::pmr::unsynchronized_pool_resource pool;
        for (int i = 0; i < kIterations / 10; ++i) {
            std::pmr::string s("hello world, this is a test string", &pool);
            DoNotOptimize(s);
        }
    });

    std::cout << "[std::string]  median = " << std_ms  << " ms\n";
    std::cout << "[pmr::string]  median = " << pmr_ms << " ms\n";
    EXPECT_GT(std_ms, 0.0);
    EXPECT_GT(pmr_ms, 0.0);
}