#ifndef _REDIS_PRODUCER_H_
#define _REDIS_PRODUCER_H_

#include "sw/redis++/redis++.h"
#include "fmt/core.h"
#include <memory>
#include <string>

//using namespace std;
using namespace sw;

class RedisProducer {
public:
    bool init();
    bool publish(std::string &channel, const std::string &msg);

    ~RedisProducer();

private:
    std::shared_ptr<redis::Redis> _redis;
};

#endif