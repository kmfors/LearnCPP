#include "redis_consumer.h"
#include "fmt/core.h"
#include "utils.h"
#include "sw/redis++/connection.h"
#include <chrono>
#include <memory>
#include <thread>


bool RedisConsumer::init() {
    sw::redis::ConnectionOptions conn_opts;
    conn_opts.host = "192.168.20.10";
    conn_opts.port = 6379;
    conn_opts.socket_timeout = std::chrono::milliseconds(100);
    

    _redis = std::make_shared<redis::Redis>(conn_opts);
    //_redis = std::make_shared<redis::Redis>(conn_opts);
    if(!_redis) {
        fmt::print("redis init failed\n");
        return false;
    }
    fmt::print("redis init success\n");
    return subscribe("test_channel");
}

bool RedisConsumer::subscribe(std::string channel = "test_channel") {
    if (_consumer) return true; 
    _consumer = std::make_shared<redis::Subscriber>(_redis->subscriber());
    if (!_consumer) return false;

    _consumer->on_message([](std::string channel, std::string msg) {
        fmt::print("receive channel:{}, msg:{}\n", channel, msg);
        
    });
    _consumer->subscribe(channel);
    return true;
}

bool RedisConsumer::unsubscribe() {
    _consumer->unsubscribe("pss_secuid_channel");
    return true;
}

void RedisConsumer::consume() {
    if (!_redis || !_consumer) return ;
    _thread = std::make_shared<std::thread>(std::thread([&](){
        while (true) {
            try {
                _consumer->consume();
            } catch (const redis::TimeoutError &e) {
                continue;
            } catch (const sw::redis::Error &err) {
                fmt::print("consume failed: {}\n", err.what());
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    }));
    _thread->detach();
    
}



RedisConsumer::~RedisConsumer(){
    if (_redis) {
        _thread.reset();
        _consumer.reset();
        _redis.reset();
    }
}