#pragma once

#include <array>
#include <atomic>

template <typename T, std::size_t MAX_SIZE= 1024>
class RingBufferSPSC {
    static_assert((MAX_SIZE & (MAX_SIZE - 1)) == 0, "MAX_SIZE must be a power of two");
public:
    RingBufferSPSC() : w_seq_(0), r_seq_(0) {}

    // 判空
    bool empty() const {
        auto w = w_seq_.load(std::memory_order_relaxed);
        auto r = r_seq_.load(std::memory_order_relaxed);
        return w == r;
    }

    // 判满
    bool full() const {
        auto w = w_seq_.load(std::memory_order_relaxed);
        auto r = r_seq_.load(std::memory_order_relaxed);
        return (w - r) == MAX_SIZE;
    }

    bool push(const T& value) {
        auto w = w_seq_.load(std::memory_order_relaxed);
        auto r = r_seq_.load(std::memory_order_acquire);
        if ((w - r) == MAX_SIZE) return false;

        buf_[ w & (MAX_SIZE - 1)] = value;
        w_seq_.store(w + 1, std::memory_order_release);

        return true;
    }

    bool pop(T& value) {
        auto w = w_seq_.load(std::memory_order_acquire);
        auto r = r_seq_.load(std::memory_order_relaxed);
        if (w == r) return false;

        value = buf_[ r & (MAX_SIZE - 1)];
        r_seq_.store(r + 1, std::memory_order_release);

        return true;
    }

private:
    std::array<T, MAX_SIZE> buf_;
    alignas(64) std::atomic<std::size_t> w_seq_;
    alignas(64) std::atomic<std::size_t> r_seq_;
};