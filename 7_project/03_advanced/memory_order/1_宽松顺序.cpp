#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> cnt = { 0 };

void f()
{
    for (int n = 0; n < 1000; ++n) {
        cnt.fetch_add(1, std::memory_order_relaxed);
    }
}

int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(f);
    }
    for (auto& t : v) {
        t.join();
    }
    std::cout << "Final counter value is " << cnt << '\n';
}

/*
* 带标签 memory_order_relaxed 的原子操作不是同步操作；它们不会为并发的内存访问行为添加顺序约束。它们只保证原子性和修改顺序的一致性。
* 
* 宽松内存顺序的典型的应用是计数器自增，只要求原子性，但不要求顺序或同步
* 
*/