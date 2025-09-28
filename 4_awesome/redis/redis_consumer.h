#ifndef _REDIS_CONSUMER_H_
#define _REDIS_CONSUMER_H


#include "sw/redis++/redis++.h"
#include "fmt/core.h"
#include <memory>
#include <thread>
#include <string>

//using namespace std;
using namespace sw;

class RedisConsumer {
public:
    bool init();
    bool subscribe(std::string channel);
    bool unsubscribe();
    void consume();

    ~RedisConsumer();

private:
    std::shared_ptr<std::thread> _thread;
    std::shared_ptr<redis::Redis> _redis;
    std::shared_ptr<redis::Subscriber> _consumer;
};






#endif