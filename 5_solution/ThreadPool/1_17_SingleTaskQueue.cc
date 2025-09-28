#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(size_t threadNums = std::thread::hardware_concurrency()) : stop(false) {
        for (size_t i = 0; i < threadNums; ++i) {
            workers.emplace_back([this]() {
                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this]() {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty())   return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task(); // 执行任务
                }
            });
        }
    }
    template<typename Func, typename ...Args>
    auto enqueue(Func&& f, Args&& ...args) {
        using return_type = std::invoke_result_t<Func, Args...>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
        );
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop) {
                throw std::runtime_error("enqueue on stopped Thread pool");
            }
            tasks.emplace([task = std::move(task)]() {
                (*task)();
            });
        }
        condition.notify_one();
    }
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();// 唤醒所有线程，清理任务
        for (std::thread& worker : workers)
            worker.join();// 阻塞，等待所有线程执行结束
    }
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

int main(){

    exit(EXIT_SUCCESS);
}

