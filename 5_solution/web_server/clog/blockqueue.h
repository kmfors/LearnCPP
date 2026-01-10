#ifndef _BLOCKQUEUE_H_
#define _BLOCKQUEUE_H_

#include <cassert>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

template<class T>
class BlockDeque {
public:
    using LockUnique = std::unique_lock<std::mutex>;
    using LockGuard = std::lock_guard<std::mutex>;

    explicit BlockDeque(size_t maxCapacity = 1000);

    ~BlockDeque();

    void clear();

    bool empty();

    bool full();

    void close();

    size_t size();

    size_t capacity();

    T front();

    T back();

    void push_back(const T& item);

    void push_front(const T &item);

    bool pop(T& item);

    bool pop(T& item, int timeout);

    void flush();

private:
    std::deque<T> deq_;

    size_t capacity_;

    std::mutex mtx_;

    bool isClose_;

    std::condition_variable condConsumer_;

    std::condition_variable condProducer_;
};

template<class T>
BlockDeque<T>::BlockDeque(size_t maxCapacity) : capacity_(maxCapacity) {
    assert(maxCapacity > 0);
    isClose_ = false;
}

template<class T>
BlockDeque<T>::~BlockDeque() {
    close();
}

template<class T>
void BlockDeque<T>::close() {
    {
        LockGuard lock(mtx_);
        deq_.clear();
        isClose_ = true;
    }
    condProducer_.notify_all();
    condConsumer_.notify_all();
}

template<class T>
void BlockDeque<T>::flush() {
    condConsumer_.notify_one();
}

template<class T>
void BlockDeque<T>::clear() {
    LockGuard lock(mtx_);
    deq_.clear();
}

template<class T>
T BlockDeque<T>::front() {
    LockGuard lock(mtx_);
    return deq_.front();
}

template<class T>
T BlockDeque<T>::back() {
    LockGuard lock(mtx_);
    return deq_.back();
}

template<class T>
size_t BlockDeque<T>::capacity() {
    LockGuard lock(mtx_);
    return capacity_;
}

template<class T>
bool BlockDeque<T>::empty() {
    LockGuard lock(mtx_);
    return deq_.empty();
}

template<class T>
bool BlockDeque<T>::full() {
    LockGuard lock(mtx_);
    return deq_.size() >= capacity_;
}

template<class T>
void BlockDeque<T>::push_back(const T& item) {
    LockUnique lock(mtx_);
    while(deq_.size() >= capacity_) {
        condProducer_.wait(lock);
    }
    deq_.push_back(item);
    condConsumer_.notify_one();
}

template<class T>
void BlockDeque<T>::push_front(const T& item) {
    LockUnique lock(mtx_);
    while(deq_.size() >= capacity_) {
        condProducer_.wait(lock);
    }
    deq_.push_front(item);
    condConsumer_.notify_one();
}

template<class T>
bool BlockDeque<T>::pop(T& item) {
    LockUnique lock(mtx_);
    while(deq_.empty()) {
        condConsumer_.wait(lock);
        if (isClose_) { return false;}
    }
    item = deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}

template<class T>
bool BlockDeque<T>::pop(T& item, int timeout) {
    LockUnique lock(mtx_);
    while (deq_.empty()) {
        if (condConsumer_.wait_for(lock, std::chrono::seconds(timeout))
             == std::cv_status::timeout) {
            return false;
        }
        if (isClose_) { return false; }
    }
    item = deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}

/*
 * wait 的语义是：释放锁 → 等待条件 → 重新获取锁，这必须用 unique_lock。
 * std::unique_lock 可以在 wait 期间释放锁，而 std::lock_guard 不支持。
 *
*/



#endif  // _BLOCKQUEUE_H_