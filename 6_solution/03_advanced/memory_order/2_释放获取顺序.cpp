
/*
* 若线程 A 中的一个原子存储带标签 memory_order_release ，而线程 B 中来自同一变量的原子加载带标签 memory_order_acquire ，
* 则从线程 A 的视角先发生于原子存储的所有内存写入（非原子及宽松原子的），在线程 B 中成为可见副效应，即一旦原子加载完成，则保证线程 B 能观察到线程 A 写入内存的所有内容。
*
* 同步仅建立在释放和获得同一原子对象的线程之间。其他线程可能看到与被同步线程的一者或两者相异的内存访问顺序。
*
* 互斥锁（例如 std::mutex ）是释放获得同步的例子：线程 A 释放锁而线程 B 获得它时，发生于线程 A 环境的临界区（释放之前）中的所有事件，必须对于执行同一临界区的线程 B （获得之后）可见。
*
*/

#include <thread>
#include <atomic>
#include <cassert>
#include <string>

std::atomic<std::string*> ptr;
int data;

void producer()
{
    std::string* p = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_release);
}

void consumer()
{
    std::string* p2;
    while (!(p2 = ptr.load(std::memory_order_acquire)));
    assert(*p2 == "Hello"); //绝无问题
    assert(data == 42); //绝无问题
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join(); t2.join();
}