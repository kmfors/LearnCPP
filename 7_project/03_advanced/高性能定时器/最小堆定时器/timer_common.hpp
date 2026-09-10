#ifndef _LIB_SRC_TIMER_COMMON_H_
#define _LIB_SRC_TIMER_COMMON_H_

#include <cstddef>
#include <cstdio>
#include <sys/time.h>

// 获取时间戳 单位：毫秒
time_t getMsec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    // 秒乘1000，微妙除1000，返回的是毫秒
}

// 定时器数据结构的定义
template <typename _User_Data>
class Timer {
public:
    Timer() : _user_data(nullptr), _cb_func(nullptr) {}
    Timer(int msec) : _user_data(nullptr), _cb_func(nullptr) {
        this->_expire = getMsec() + msec;
    }
    ~Timer() {}

    void setTimeout(time_t timeout) {
        this->_expire = getMsec() + timeout;
    }

    time_t getExpire() {
        return _expire;
    }

    void setUserData(_User_Data* userData) {
        this->_user_data = userData;
    }

    void handleTimeout() {
        if (_cb_func) {
            _cb_func(_user_data);
        }
    }

    using TimeOutCbFunc = void (*)(_User_Data*);
    void setCallBack(TimeOutCbFunc callback) {
        this->_cb_func = callback;
    }
private:
    time_t _expire;     // 定时器生效的绝对时间
    _User_Data* _user_data;     // 用户数据
    TimeOutCbFunc _cb_func;     // 超时的回调函数
};
/*
    思考：Timer就是一个简单的定时器，它的功能就是接收用户的数据，并设置过期时间，到达过期时间后回调用户数据的处理操作函数
        expire 可以理解为沙漏，意味着留存的时间不断地流逝，直至终止。

*/


// 定时器容器
template <typename _UData>
class ITimerContainer {
public:
    ITimerContainer() = default;
    virtual ~ITimerContainer() = default;
public:
    virtual void tick() = 0; // 时间标记提醒（滴答）
    virtual Timer<_UData>* addTimer(time_t timeout) = 0;
    virtual void delTimer(Timer<_UData>* timer) = 0;
    virtual void resetTimer(Timer<_UData>* timer, time_t timeout) = 0;
    virtual int getMinExpire() = 0;
};
/*
    思考：前缀 I 在 IContainer 中代表接口（Interface）的意思。在面向对象编程中，接口是一种定义了一组没有实现的方法的引用类型。
        应该是每个连接将会分配一个定时器，然后由一个容器托管。这个托管容器的作用也很简单就是添加删除重置定时器，获取一个最小的过期
        时间。
*/


#endif

