#include "redis_producer.h"
#include "fmt/core.h"
#include "utils.h"
#include "sw/redis++/connection.h"


bool RedisProducer::init() {
    // sw::redis::ConnectionOptions conn_opts;
    // conn_opts.host = "192.168.20.10";
    // conn_opts.port = 6379;
    // conn_opts.socket_timeout = std::chrono::milliseconds(100);

    _redis = std::make_shared<redis::Redis>("tcp://192.168.20.10:6379");
    //_redis = std::make_shared<redis::Redis>(conn_opts);
    if(!_redis) {
        fmt::print("redis init failed\n");
        return false;
    }

    fmt::print("redis init success\n");
    return true;
}


bool RedisProducer::publish(std::string &channel, const std::string &msg) {
    
    //std::string all = pubsub_key(channel);
    fmt::print("pubsub channel:{}\n", channel);
    _redis->publish(channel, msg);
    return true;
}

RedisProducer::~RedisProducer(){
    if (_redis) {
        _redis.reset();
    }
}