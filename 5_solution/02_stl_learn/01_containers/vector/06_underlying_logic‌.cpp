#include <iostream>
#include <vector>
#include <string>
using namespace std;

/**
 * std::vector 最根本的底层实现是一个动态增长的数组。它通过在堆上分配连续的内存空间来存储元素。
 * 当执行 push_back、emplace_back 或 insert 等操作导致 size() == capacity() 时，就会触发扩容
 * 扩容步骤：
 *  - 分配新内存：在堆上分配一块更大的连续内存。新的容量大小通常遵循一个增长因子（常见的是旧容量的 1.5 倍 或 2 倍)
 *  - 移动/拷贝元素：将旧内存中的所有元素移动或拷贝到新内存中。
 *  - 对于内置类型（如 int, double）或简单的类，直接按字节拷贝（memcpy 风格）。
 *  - 对于复杂的类类型，会使用移动构造函数（如果 noexcept 为 true）或拷贝构造函数，来避免不必要的深拷贝，提高效率。
 *  - 销毁旧对象并释放旧内存：析构旧内存中的元素（如果是类对象），然后释放原来的那块内存。
 *  - 更新指针：将 _start, _finish, _end_of_storage 指向新的内存区域。
*/

void expansion_print(size_t size, size_t cap, bool& flag, void* ptr) {
    if (flag) {
        if(cap > size) {
            cout << "vec size=" << size << ", capacity=" << cap
                 << ", 触发扩容机制，容量扩至原先的"
                 << static_cast<double>(cap) / (size-1) << "倍"
                 <<", &vec[0]="  << ptr << endl;
        }
    }
    flag = false;
    if(cap == size) { flag = true;}
}

int main(int argc, char* argv[])
{
    vector<int> vec;
    vec.reserve(8);
    bool flag = false;

    for(int i = 0; i < 100; i++) {

        vec.push_back(i); // 在每次扩容后，&vec[0] 的地址都会改变
        expansion_print(vec.size(), vec.capacity(), flag, &vec[0]);
    }

    return 0;
}