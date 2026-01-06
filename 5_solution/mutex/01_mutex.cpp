#include <iostream>
#include <mutex>
#include <thread>

std::mutex g_mutex;
int counter = 0;

void increment() {
    g_mutex.lock();
    counter++;
    std::cout << "Counter value in thread " << std::this_thread::get_id()
        << " is " << counter << std::endl;
    g_mutex.unlock();
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();

    return 0;
}