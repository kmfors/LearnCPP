#include <iostream>
#include <string>
#include <chrono>
#include <vector>
using namespace std;

/**
 *
 * empty(): 快速检查vector是否为空
 *
 * size(): 获取当前元素数量
 *
 * max_size(): 获取理论上的最大容量
 *
 * reserve(): 预先分配内存，避免多次重新分配
 *
 * capacity(): 查看当前分配的内存容量
 *
 * shrink_to_fit(): 释放多余内存，使capacity等于size
*/

#define HIGH_TIME_POINT(start) \
    auto start = std::chrono::high_resolution_clock::now();
#define HIGH_TIME_DURATION(dur, end, start) \
    HIGH_TIME_POINT(end);\
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

int main(int argc, char* argv[])
{
    const int SIZE = 1000000;

    // 不使用reserve - 性能较差
    HIGH_TIME_POINT(start1);
    std::vector<int> vec1;
    for (int i = 0; i < SIZE; ++i) {
        vec1.push_back(i);
    }
    HIGH_TIME_DURATION(dur1, end1, start1);
    std::cout << "not use reserve: " << dur1.count() << " ms" << '\n';
    std::cout << "final capacity: " << vec1.capacity() << "\n\n";
    // 使用shrink_to_fit优化内存使用
    vec1.shrink_to_fit();
    std::cout << "after shrink_to_fit capacity: " << vec1.capacity() << std::endl;


    // 使用reserve - 性能更好
    HIGH_TIME_POINT(start2);
    std::vector<int> vec2;
    vec2.reserve(SIZE);  // 预先分配足够空间
    for (int i = 0; i < SIZE; ++i) {
        vec2.push_back(i);
    }
    HIGH_TIME_DURATION(dur2, end2, start2);
    std::cout << "use reserve: " << dur2.count() << " ms" << '\n';;
    std::cout << "final capacity: " << vec2.capacity() << "\n\n";

    return 0;
}