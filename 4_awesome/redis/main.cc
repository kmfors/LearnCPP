
#include <iostream>
#include <thread>
#include "redis_producer.h"
#include "redis_consumer.h"

using std::string;

void con(){
    
}

void test() {
    RedisProducer rp_ins;
    rp_ins.init();
    RedisConsumer con;
    con.init();
    con.consume();


    string channel = "test_channel";
    string msg = "hello, redis";
    rp_ins.publish(channel, msg);

    string input;
    while(std::cin >> input) {
        if (input == "q")   break;
        rp_ins.publish(channel, msg);
    } 

}


int main() {
    test();
    return 0;
}