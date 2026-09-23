// #include <iostream>

// int main() {
//     constexpr std::size_t SIZE = 1024;

//     std::cout << "################################## 性能测试 ############################################"

//     try {
//         // 性能测试
//         test

//     } catch (const std::exception& e) {
//         std::cerr << "发生标准异常：" << e.what() << "\n";
//     } catch (...) {
//         std::cerr << "发生未知异常\n";
//     }
//     std::cout << '\n';

// }

#include <thread>
#include <chrono>
#include <iostream>
#include "gtest/gtest.h"
#include "RingBuffer/seq_spsc.h"



#define GET_TIME_POINT(field) field = std::chrono::steady_clock::now();

void print(
    std::chrono::steady_clock::time_point& write_start,
    std::chrono::steady_clock::time_point& write_end,
    std::chrono::steady_clock::time_point& read_start,
    std::chrono::steady_clock::time_point& read_end,
    int write_count, int read_count
    ) {
    auto write_cost = std::chrono::duration_cast<std::chrono::microseconds>(write_end - write_start);
    auto read_cost = std::chrono::duration_cast<std::chrono::microseconds>(read_end - read_start);

    int write_ops = write_count / write_cost.count();
    int read_ops = read_count / read_cost.count();

    std::cout << "生产者写入: " << write_count << " 个数据，写入耗时: " << write_cost.count() << " us, 写入速度: " << write_ops << " ops/us" << std::endl;
    std::cout << "消费者读取: " << read_count << " 个数据，读取耗时: " << read_cost.count() << " us, 读取速度: " << read_ops << " ops/us" << std::endl;
}

template<size_t BUFFER_SIZE = 1024>
void test_read_write(int write_count) {

    auto spsc = std::make_unique<RingBufferSPSC<int, BUFFER_SIZE>>();

    std::atomic<bool> producer_done{false};
    int read_count{0}; // 读取次数

    // 消费者操作
    std::chrono::steady_clock::time_point read_start;
    std::chrono::steady_clock::time_point read_end;
    std::thread consumer([&]() {
        int msg = 10;
        bool is_first = true;

        while (!producer_done.load(std::memory_order_acquire)) {
            if (spsc->pop(msg)) {
                if (is_first) { GET_TIME_POINT(read_start); is_first = false; }
                read_count++;
            }
        }
        while (spsc->pop(msg)) {
            if (is_first) { GET_TIME_POINT(read_start); is_first = false; }
            read_count++;
        }
        GET_TIME_POINT(read_end);
    });

    // 生产者操作
    int object = 20;
    auto write_start = std::chrono::steady_clock::now();
    for (int i = 0; i < write_count; ++i) {
        while (!spsc->push(object)) {}
    }
    auto write_end = std::chrono::steady_clock::now();

    producer_done.store(true, std::memory_order_release);
    consumer.join();

    print(write_start, write_end, read_start, read_end, write_count, read_count);
}




TEST(MyTest, Test_Read_Write) {
    try {

        std::cout << "\n=== 1K buffer, 2M items ===\n";
        test_read_write<1 * 1024>(2000000);

        std::cout << "\n=== 16K buffer, 2M items ===\n";
        test_read_write<16 * 1024>(2000000);

        std::cout << "\n=== 1M buffer, 2M items ===\n";
        test_read_write<1 * 1024 * 1024>(2000000);

        std::cout << "\n=== 16M buffer, 2M items ===\n";
        test_read_write<16 * 1024 * 1024>(2000000);
    } catch (const std::exception& e) {
        FAIL() << "test_read_write threw: " << e.what();
    } catch (...) {
        FAIL() << "test_read_write threw unknown exception";
    }
}