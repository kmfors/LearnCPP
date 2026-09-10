#include "1_ThreadPool.h"
#include <atomic>
#include <mutex>
#include <thread>

using namespace BeiChen;

ThreadPool::ThreadPool(int n) : _stop(false), _tg(_threads) {
    int nThreads = n;
    if (nThreads <= 0) {
        nThreads = std::thread::hardware_concurrency();
        nThreads = (nThreads == 0 ? 2 : nThreads);
    }

    for (int i = 0; i != nThreads; ++i) {
        _threads.push_back(std::thread([this]{
            while (!_stop.load(std::memory_order_acquire)) {
                task_type task;
                {
                    std::unique_lock<std::mutex> ulk(this->_mtx);
                    // wait只有满足条件后，才会往下执行
                    this->_cond.wait(ulk, [this]{
                        return _stop.load(std::memory_order_acquire) ||
                            !this->_tasks.empty();
                    });
                    if(_stop.load(std::memory_order_acquire))   return;
                    task = std::move(this->_tasks.front());
                    this->_tasks.pop();
                }
                task();
            }
        }));
    }
}