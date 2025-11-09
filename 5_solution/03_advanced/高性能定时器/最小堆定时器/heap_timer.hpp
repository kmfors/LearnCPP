#ifndef _LIB_SRC_HEAD_TIMER_H_
#define _LIB_SRC_HEAD_TIMER_H_

#include "timer_common.hpp"
#include <iostream>

#define HEAD_DEFAULT_SIZE 128

// 定时器数据结构的定义
template  <typename _User_Data>
class HeapTimer {
public:
    HeapTimer() = default;
    HeapTimer(int msec) {
        timer.setTimeout(msec);
    }
    ~HeapTimer() {}

    void setTimeout(time_t timeout) {
        timer.setTimeout(timeout);
    }

    time_t getExpire() {
        return timer.getExpire();
    }

    void setUserData(_User_Data* userData) {
        timer.setUserData(userData);
    }

    int getPos() {
        return _pos;
    }

    void setPos(int pos) {
        this->_pos = pos;
    }

    void handleTimeout() {
        timer.handleTimeout();
    }

    using TimeOutCbFunc = void (*)(_User_Data*);
    void setCallBack(TimeOutCbFunc callBack) {
        timer.setCallBack(callBack);
    }

public:
    Timer<_User_Data> timer;
private:
    // 保存该定时器在数组中的位置，便于查找删除
    int _pos;
};
/*
    疑问1：为什么要再将Timer包装呢？新包装的HeapTimer内部嵌合了Timer与pos，整体可以成为我们进行复杂数据结构的操作对象。
        另一方面还可以复用原先的Timer数据结构，定义另外一层更加抽象的数据结构
*/
// 定时容器，使用最小堆实现
template <typename _UData>
class HeapTimerContainer : public ITimerContainer<_UData> {
public:
    HeapTimerContainer();
    HeapTimerContainer(int capacity);
    HeapTimerContainer(HeapTimer<_UData>** initArray, int arrSize, int capacity);
    virtual ~HeapTimerContainer() override;
public:
    virtual void tick() override;
    Timer<_UData>* addTimer(time_t timeout) override;
    void delTimer(Timer<_UData>* timer) override;
    void resetTimer(Timer<_UData>* timer, time_t timeout) override;
    int getMinExpire() override;
    Timer<_UData>* top();
    void popTimer();

private:
    void percolateDown(int hole);
    void percolateUp(int hole);
    void resize();
    bool isEmpty();

private:
    HeapTimer<_UData>** _array;     // 堆数据
    int _capacity;  // 堆数组的容量
    int _size;      // 当前包含的元素
};

template <typename _UData>
HeapTimerContainer<_UData>::HeapTimerContainer() : HeapTimerContainer(HEAD_DEFAULT_SIZE) {}

// 初始化一个大小为cap的空堆
template <typename _UData>
HeapTimerContainer<_UData>::HeapTimerContainer(int capacity) {
    this->_capacity = capacity;
    this->_size = 0;

    // 不理解为什么加个nullptr
    _array = new HeapTimer<_UData>* [capacity]{nullptr};
}

// 用已有数组来初始化堆
template <typename _UData>
HeapTimerContainer<_UData>::HeapTimerContainer(HeapTimer<_UData>** initArray,
    int arrSize, int capacity) : _size(arrSize) {
    if(capacity < arrSize) {
        this->_capacity = capacity = 2 * arrSize;
    }
    _array = new HeapTimer<_UData>*[capacity];
    for (int i = 0; i < capacity; i++) {
        _array[i] = nullptr;
    }
    if (arrSize > 0) {
        for (int i = 0; i < arrSize; i++) {
            _array[i] = initArray[i];
        }
        for (int i = (_size - 1) / 2; i >= 0; i--) {
            percolateDown(i);
        }
    }
}

template<typename _UData>
HeapTimerContainer<_UData>::~HeapTimerContainer() {
    if(_array) {
        for(int i = 0; i < _size; i++) {
            delete _array[i];
        }
        delete []_array;
    }
}       

template <typename _UData>
void HeapTimerContainer<_UData>::tick() {
    std::cout << "-----------------tick-----------------" << std::endl;
    HeapTimer<_UData>* tmp = _array[0];
    time_t cur = getMsec();
    // 循环处理到期的定时器
    while(!isEmpty()) {
        if(!tmp)    break;
        if(tmp->getExpire() > cur)  break;

        tmp->handleTimeout();
        // 将堆顶元素删除，同时生成新的堆顶定时器
        popTimer();
        tmp = _array[0];
    }
}

// 获取一个定时器
template <typename _UData>
Timer<_UData>* HeapTimerContainer<_UData>::addTimer(time_t timeout) {
    if(_size >= _capacity)  this->resize();

    int hole = _size++;
    HeapTimer<_UData>* timer = new HeapTimer<_UData>(timeout);
    _array[hole] = timer;

    percolateUp(hole);
    return &timer->timer;
}

// 删除目标定时器
template <typename _UData>
void HeapTimerContainer<_UData>::delTimer(Timer<_UData>* timer) {
    if(!timer)  return;

    timer->setCallBack(nullptr);
    timer->setUserData(nullptr);
}

// 重置一个定时器
template <typename _UData>
void HeapTimerContainer<_UData>::resetTimer(Timer<_UData>* timer, time_t timeout) {
    HeapTimer<_UData>* htimer = reinterpret_cast<HeapTimer<_UData>*>(timer);

    int pos = htimer->getPos();
    int lastPos = _size - 1;
    if(pos != lastPos) {
        HeapTimer<_UData>* temp = _array[pos];
        _array[pos] = _array[lastPos];
        _array[lastPos] = temp;
    }
    timer->setTimeout(timeout);

    percolateDown(pos);
    percolateUp(lastPos);
}

// 获取容器中超时值最小的值
template <typename _UData>
int HeapTimerContainer<_UData>::getMinExpire() {
    Timer<_UData>* timer = top();
    if(timer)   return timer->getExpire();
    return -1;
}

// 获取顶部的定时器
template <typename _UData>
Timer<_UData>* HeapTimerContainer<_UData>::top(){
    if(!isEmpty())  return nullptr;
    return &_array[0]->timer;
}

template <typename _UData>
void HeapTimerContainer<_UData>::popTimer() {
    if(!isEmpty())  return;
    if(_array[0]) {
        delete _array[0];
        _array[0] = _array[--_size];
        percolateDown(0);
    }
}

// 最小堆的下滤操作，它确保数组中以第 hole 个节点作为根的子树拥有最小堆性质
template <typename _UData>
void HeapTimerContainer<_UData>::percolateDown(int hole) {
    if(_size == 0)  return ;
    HeapTimer<_UData>* temp = _array[hole];
    int child = 0;
    for(; ((hole * 2 + 1) <= _size - 1 ); hole = child) {
        child = hole * 2 + 1;
        if((child < (_size - 1)) && (_array[child + 1]->getExpire() < _array[child]->getExpire())) {
            child++;
        }
        if(_array[child]->getExpire() < temp->getExpire()) {
            _array[hole] = _array[child];
            _array[hole]->setPos(hole);
        } else {
            break;
        }
    }
    _array[hole] = temp;
    _array[hole]->setPos(hole);
}

template <typename _UData>
void HeapTimerContainer<_UData>::percolateUp(int hole) {
    int parent = 0;
    HeapTimer<_UData>* temp = _array[hole];

    for(; hole > 0; hole = parent) {
        parent = (hole - 1) / 2;
        if(_array[parent]->getExpire() <= temp->getExpire()) {
            break;
        }
        _array[hole] = _array[parent];
        _array[hole]->setPos(hole);
    }
    _array[hole] = temp;
    _array[hole]->setPos(hole);
}

// 将数组的容量扩大一倍
template <typename _UData>
void HeapTimerContainer<_UData>::resize() {
    HeapTimer<_UData>** temp = new HeapTimer<_UData>* [2 * _capacity];
    _capacity = 2 * _capacity;

    for(int i = 0; i < _size; i++) {
        temp[i] = _array[i];
    }
    for(int i = _size; i < _capacity; i++) {
        temp[i] = nullptr;
    }
    delete []_array;
    _array = temp;
}

template <typename _UData>
bool HeapTimerContainer<_UData>::isEmpty() {
    return _size == 0;
}


#endif