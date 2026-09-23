// 环形缓冲的空满判断：空槽判断
// 先从最简单的类型开始，后面再发展为模板
#include <atomic>


class RingBuffer {
private:
    uint64 buf[1024]; // 1024个容量，存储uint64类型对象
    std::atomic<uint64_t> w_ptr;
    std::atomic<uint64_t> w_ptr;

public:
    // 判空
    bool empty() {
        auto w_index = w_ptr.
    }
};