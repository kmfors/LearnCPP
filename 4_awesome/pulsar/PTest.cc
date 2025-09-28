#include "pulsar/Client.h"
#include "param_option.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace pulsar;

class PulsarProducer {
public:
    PulsarProducer() : client_("pulsar://192.168.20.10:6650"){
        config();
    }
    ~PulsarProducer() {
        worker_.close();
        client_.close();
    }
private:
    void config(){
        config_.setBlockIfQueueFull(true); 
        config_.setCompressionType(CompressionLZ4);
    }

public:
    bool Start(const string& topic) {
        Result result = client_.createProducer(topic, config_, worker_);
        if(result != ResultOk) {
            std::cerr << "Error createing produce: " << result;
            return false;
        }
        return true;
    }
    

    void Send(const string& t_msg) {
        Message msg = MessageBuilder().setContent(t_msg).build();
        worker_.sendAsync(msg, [&t_msg](Result code, const MessageId& msgId){
            std::cout << "Received ack for msg: [" << t_msg << "] with code: "
            << code << " -- MsgId: " << msgId << std::endl;
        });
        msg.getLength();
    }

private:
    Client client_;
    Producer worker_;
    ProducerConfiguration config_;
};

//
void test(const ParamOption& param) {
    string topic = "persistent://" + param.topic;
    PulsarProducer PUser;
    PUser.Start(topic);
    int count = param.count;
    string line;
    while (getline(std::cin, line) && line[0] != 'q') {
        
        PUser.Send(line);
    }
    // while(count--){
    //     PUser.Send(param.msg);
    //     std::this_thread::sleep_for(std::chrono::seconds(param.speed));
    // }  
}


int main(int argc, char* argv[]) {
    ParamOption param {};
    if (main_start(argc, argv, param))  return -1;

    std::thread th1(test, param);    
    th1.join(); 

    

    return 0;
}
