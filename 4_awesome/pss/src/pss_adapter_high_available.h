#include <csignal>
#include <functional>
#include <thread>
#include "spdlogger.h"

class SafeSignal {
private:
    sigset_t _set;
    std::function<void(int signo)> _on_signal;
public:
    SafeSignal():_on_signal(nullptr) {
        sigemptyset(&_set);
    };

    bool Init(std::function<void(int signo)> callback) {
        if(!callback)   return false;
        sigaddset(&_set, SIGINT);
        sigaddset(&_set, SIGQUIT);
        sigaddset(&_set, SIGABRT);
        sigaddset(&_set, SIGFPE);
        sigaddset(&_set, SIGSEGV);
        sigaddset(&_set, SIGTERM);
        int ret = pthread_sigmask(SIG_BLOCK, &_set, NULL);
        if(ret) {
            MLOGE("SafeSignal Init Failed, ret:{}", ret);
            return false;
        }
        _on_signal = callback;
        WaitSignal();
        return true;
    }

    void WaitSignal() {
        auto thread = std::thread([&](){
            int signo = 0;
            int ret = sigwait(&_set, &signo);
            if(ret)
                MLOGE("sigwait failed, ret:{}, signo:{}", ret, signo);
            if(signo)   _on_signal(signo);
        });
        thread.detach();
    }
};