#include <csignal>
#include <functional>
#include <pthread.h>
#include <signal.h>
#include <iostream>
#include <thread>

using namespace std;

// 回调函数
void ProcessExitSignal(int signo){
    std::cout << "receive signo: " << signo << std::endl;
}

class SignalSafe{

public:
    SignalSafe():_on_signal(nullptr){
        sigemptyset(&_set);
    }

    bool Init(function<void(int signo)> callback){
        if(!callback) return false;
        sigaddset(&_set, SIGINT);
        sigaddset(&_set, SIGQUIT);
        sigaddset(&_set, SIGABRT);
        sigaddset(&_set, SIGFPE);
        sigaddset(&_set, SIGSEGV);
        sigaddset(&_set, SIGTERM);
        
        int ret = pthread_sigmask(SIG_BLOCK, &_set, NULL);
        if(ret){
            cerr << "SignalSafe.Init error, ret:" << ret << endl;
            return false;
        }
        _on_signal = callback;
        WaitSignal();
        return true;
    }

    void WaitSignal(){
        auto thread = std::thread([&](){
            int signo = 0;
            int ret = sigwait(&_set,&signo);
            if(ret){
                cerr << "sigwait error, ret:" << ret
                     << ", signo:" << signo << endl;
            }
            if(signo) _on_signal(signo);
        });
        thread.detach();
    }


private:
    sigset_t _set;
    std::function<void(int signo)> _on_signal;
};

// 线程安全信号处理对象
// extern SignalSafe g_process_signal;
SignalSafe g_process_signal;

int main(){

    SignalSafe demo;
    demo.Init(ProcessExitSignal);

    getchar();

    return 0;
}
