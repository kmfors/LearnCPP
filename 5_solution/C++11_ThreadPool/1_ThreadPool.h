#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

// C++11 版的 线程池
namespace BeiChen {
class ThreadsGuard {
public:
    ThreadsGuard(std::vector<std::thread> &v) : _threads(v) {}

    ~ThreadsGuard() {
        for (size_t i = 0; i != _threads.size(); ++i) {
            if (_threads[i].joinable()) {
                _threads[i].join();
            }
        }
    }

private: // 屏蔽掉左值引用、右值引用
  ThreadsGuard(ThreadsGuard &&tg) = delete;
  ThreadsGuard &operator=(ThreadsGuard &&tg) = delete;

  ThreadsGuard(const ThreadsGuard &) = delete;
  ThreadsGuard &operator=(const ThreadsGuard &) = delete;

private:
  std::vector<std::thread> &_threads;
};

class ThreadPool {
public:
    typedef std::function<void()> task_type;

public:
    explicit ThreadPool(int n = 0);

    ~ThreadPool() {
        stop();
        _cond.notify_all();
    }

    void stop() { _stop.store(true, std::memory_order_release); }

    template <class Function, class... Args>
    std::future<typename std::result_of<Function(Args...)>::type> add(Function &&,
                                                                    Args &&...);

private:
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

private:
  std::atomic<bool> _stop;
  std::mutex _mtx;
  std::condition_variable _cond;

  std::queue<task_type> _tasks;
  std::vector<std::thread> _threads;
  zl::ThreadsGuard _tg;
};

// inline ThreadPool::ThreadPool(int n) : _stop(false), _tg(_threads) {
//   int nthreads = n;
//   if (nthreads <= 0) {
//     nthreads = std::thread::hardware_concurrency();
//     nthreads = (nthreads == 0 ? 2 : nthreads);
//   }

//   for (int i = 0; i != nthreads; ++i) {
//     _threads.push_back(std::thread([this] {
//       while (!_stop.load(std::memory_order_acquire)) {
//         task_type task;
//         {
//           std::unique_lock<std::mutex> ulk(this->_mtx);
//           this->_cond.wait(ulk, [this] {
//             return _stop.load(std::memory_order_acquire) ||
//                    !this->_tasks.empty();
//           });
//           if (_stop.load(std::memory_order_acquire))
//             return;
//           task = std::move(this->_tasks.front());
//           this->_tasks.pop();
//         }
//         task();
//       }
//     }));
//   }
// }

template <class Function, class... Args>
std::future<typename std::result_of<Function(Args...)>::type>
ThreadPool::add(Function &&fcn, Args &&...args) {
  typedef typename std::result_of<Function(Args...)>::type return_type;
  typedef std::packaged_task<return_type()> task;

  auto t = std::make_shared<task>(
      std::bind(std::forward<Function>(fcn), std::forward<Args>(args)...));
  auto ret = t->get_future();
  {
    std::lock_guard<std::mutex> lg(_mtx);
    if (_stop.load(std::memory_order_acquire))
      throw std::runtime_error("thread pool has stopped");
    _tasks.emplace([t] { (*t)(); });
  }
  _cond.notify_one();
  return ret;
}
} // namespace zl

#endif /* THREAD_POOL_H */