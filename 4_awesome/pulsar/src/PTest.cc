#include "pulsar/Client.h"
#include <thread>
#include <string>
#include <chrono>
#include <memory>
#include <iostream>

using namespace std;
using namespace pulsar;


class PulsarProducer {
public:
    PulsarProducer() : _client("pulsar://192.168.20.10:6650"){
        config();
    }
    ~PulsarProducer() {
        _worker.close();
        _client.close();
    }
private:
    void config(){
        _config.setBlockIfQueueFull(true); 

    }

public:
    bool Start(string& topic) {
        Result result = _client.createProducer(topic, _config, _worker);
        if(result != ResultOk) {
            std::cerr << "Error createing produce: " << result;
            return false;
        }
        return true;
    }
    

    void Send(string& t_msg) {
        Message msg = MessageBuilder().setContent(t_msg).build();
        _worker.sendAsync(msg, [&t_msg](Result code, const MessageId& msgId){
            std::cout << "Received ack for msg: [" << t_msg << "] with code: "
            << code << " -- MsgId: " << msgId << std::endl;
        });
    }

private:
    Client _client;
    Producer _worker;
    ProducerConfiguration _config;
};

//
void test(string t_topic, string msg, int time) {
    string topic = "persistent://";
    topic.append(t_topic);
    PulsarProducer user;
    user.Start(topic);

    while(time--){
        user.Send(msg);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }  
}


int main(int argc, char* argv[]) {
    if(argc > 3){
        std::cerr << "程序启动参数错误, 正确格式:【target topic message】" << std::endl;
        return 1;
    }
    int time = 5;
    string topic1 = "moss/geek/FFF";
    string topic3 = "moss/new/AAA-partition-2";
    string topic2 = "moss/ellen/XXX";
    string msg1 = "Hello,World";
    string msg2 = "See you again";
    shared_ptr<std::thread> th1;
    shared_ptr<std::thread> th2;
    
    if(argc != 3){
        th1 = make_shared<std::thread>(test, topic1, msg1, time);
        //th2 = make_shared<std::thread>(test, topic2, msg2, time);
    } else {
        th1 = make_shared<std::thread>(test, string(argv[1]), string(argv[2]), time);
        th2 = make_shared<std::thread>(test, string(argv[1]), string(argv[2]), time);
    }

    th1->join();
    //th2->join();

    return 0;
}
