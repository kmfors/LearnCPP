#include "pulsar/Client.h"
#include "pulsar/Consumer.h"
#include "pulsar/MessageIdBuilder.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;
using namespace pulsar;

class ConsumerClient{
public:
    ~ConsumerClient(){
        consumer.close();
    }
    pulsar::Consumer& get(){
        return consumer;
    }
private:
    Consumer consumer;
};

class PulsarConsumer {
public:
    PulsarConsumer() : _client("pulsar://192.168.20.10:6650"){}
    ~PulsarConsumer() {
        //_worker.close();
        _client.close();
    }
    void Init() {
        _config.setMessageListener([](Consumer& consumer, const Message& msg){

            std::cout << "Get Message Info (Topic: " << msg.getTopicName() << ") | " 
            << "(SubscriptionName: " << consumer.getSubscriptionName() << ") ==> ["
            << msg.getDataAsString() << "], msgId:" << msg.getMessageId();

            consumer.acknowledgeAsync(msg.getMessageId(), [](Result res){
                std::cout << ". Ask res: " << res << std::endl;
            });
        });
        //_config.setConsumerType(pulsar::ConsumerType::ConsumerShared);
    }

    // 容器订阅
    void Subscribe(string subscriptionName, vector<string>& vec_topic, pulsar::Consumer& _worker) {
        pulsar::Result result = _client.subscribe(vec_topic, subscriptionName , _config, _worker);
        if(result != pulsar::ResultOk) {
            std::cerr << "Error createing subscribe: " << result;
            return ;
        }
    }
    // 单一topic订阅
    void Subscribe(string subscriptionName ,string topic, pulsar::Consumer& _worker) {
        pulsar::Result result = _client.subscribe(topic, subscriptionName , _config, _worker);
        if(result != pulsar::ResultOk) {
            std::cerr << "Error createing subscribe: " << result;
            return ;
        }
    }

    // 通过 regex 订阅多个主题时，所有主题必须位于同一命名空间中。
    void SubscribeWithRegex(string subscriptionName ,string regex, pulsar::Consumer& _worker) {
        pulsar::Result result = _client.subscribeWithRegex(regex, subscriptionName , _config, _worker);
        if(result != pulsar::ResultOk) {
            std::cerr << "Error createing subscribe: " << result;
            return ;
        }
    }
    pulsar::Result Seek(pulsar::Consumer& worker, const pulsar::MessageId& msgId){
        worker.pauseMessageListener();
        auto ret = worker.seek(msgId);
        worker.resumeMessageListener();
        return ret;
    }

private:
    pulsar::Client _client;
    pulsar::ConsumerConfiguration _config;
};

inline pulsar::MessageId DeserializeMsgId(const string& msgIdStr){
    char c = 0;
    int64_t one = 0, two = 0;
    int32_t three = 0, four = 0;
    stringstream ss(msgIdStr);
    ss >> c >> one >> c >> two >> c >> three >> c >> four >> c ;
    return pulsar::MessageIdBuilder().ledgerId(one).entryId(two).batchIndex(three).batchSize(four).build();
}

void test() {
    ConsumerClient c1;
    ConsumerClient c2;
    ConsumerClient c3;
    PulsarConsumer cc;
    cc.Init();

    //--------------- 容器订阅--------------------------------------------------------------------
    // string presix = "persistent://";
    // vector<string> vec_topic;
    // vec_topic.emplace_back(presix + "moss/ellen/XXX");
    // vec_topic.emplace_back(presix + "moss/ellen/YYY");
    // vec_topic.emplace_back(presix + "moss/ellen/ZZZ");
    // cc.Subscribe("容器订阅", vec_topic, c1.get());



    //--------------- 单一订阅--------------------------------------------------------------------
    cc.Subscribe("单一订阅1", "persistent://moss/geek/FFF", c1.get());
    cc.Subscribe("单一订阅2", "persistent://moss/geek/FFF", c2.get());
    cc.Subscribe("单一订阅3", "persistent://moss/geek/FFF", c3.get());



    //--------------- 正则表达式订阅 ---------------------------------------------------------------
    //cc.SubscribeWithRegex("正则订阅", "persistent://moss/geek/.*", c1.get());

    int ch;
    while(std::cin >> ch && ch != 0){}  

    // pulsar::Result result = cc.Seek(c1, pulsar::MessageId::earliest());
    // if(result != pulsar::ResultOk) {
    //         std::cerr << "Error seek subscribe: " << result;
    //         return ;
    // }
    // result = cc.Seek(c2, pulsar::MessageId::earliest());
    // if(result != pulsar::ResultOk) {
    //         std::cerr << "Error seek subscribe: " << result;
    //         return ;
    // }
    // result = cc.Seek(c3, pulsar::MessageId::earliest());
    // if(result != pulsar::ResultOk) {
    //         std::cerr << "Error seek subscribe: " << result;
    //         return ;
    // }

    // while(true){}   
    
}

int main(int argc, char* argv[]) {
    test();
    return 0;
}
