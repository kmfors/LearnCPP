#include "fmt/core.h"
#include "pulsar/Client.h"
#include "pulsar/Consumer.h"
#include "pulsar/MessageIdBuilder.h"
#include "param_option.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;
using namespace pulsar;

class ConsumerClient{
public:
    void destory(){
        //consumer.unsubscribe();
        consumer.close();
    }
    Consumer& get(){
        return consumer;
    }
private:
    Consumer consumer;
};

template <typename T>
void display(T& object){
    cout << "打印容器元素：" << endl;
    for(const auto& iter : object){
        cout << iter << " ";
    }
    cout << endl;
}
class PulsarConsumer {
public:
    PulsarConsumer() : client_("pulsar://192.168.20.10:6650"){}
    ~PulsarConsumer() {
        //worker_.close();
        client_.close();
    }
    void Init() {
        config_.setMessageListener([](Consumer& consumer, const Message& msg){
            stringstream ss;
            ss << msg.getMessageId();
            fmt::print("Get Message Info->TopicName:{}, Subscription:{}, MessageId:{}, getData:{}",
                    msg.getTopicName(), consumer.getSubscriptionName(),
                    ss.str(), msg.getDataAsString());
            //jsonParse(msg.getDataAsString());
            consumer.acknowledgeAsync(msg.getMessageId(), [](Result res){
                cout << ". Ask res: " << res << endl;
            });
        });
        //config_.setConsumerType(pulsar::ConsumerType::ConsumerShared);
        config_.setSubscriptionInitialPosition(InitialPosition::InitialPositionLatest);
    }

    // 容器订阅
    void Subscribe(string subscriptionName, vector<string>& vec_topic, Consumer& worker_) {
        Result result = client_.subscribe(vec_topic, subscriptionName , config_, worker_);
        if(result != ResultOk) {
            cerr << "Error createing subscribe: " << result << endl;;
            return ;
        }
    }
    // 单一topic订阅
    void Subscribe(string subscriptionName ,string topic, Consumer& worker_) {
        Result result = client_.subscribe(topic, subscriptionName , config_, worker_);
        if(result != ResultOk) {
            cerr << "Error createing subscribe: " << result << endl;
            return ;
        }
    }

    // 通过 regex 订阅多个主题时，所有主题必须位于同一命名空间中。
    void SubscribeWithRegex(string subscriptionName ,string regex, Consumer& worker_) {
        Result result = client_.subscribeWithRegex(regex, subscriptionName , config_, worker_);
        if(result != ResultOk) {
            cerr << "Error createing subscribe: " << result << endl;
            return ;
        }
    }
    pulsar::Result Seek(Consumer& worker, const MessageId& msgId){
        worker.pauseMessageListener();
        auto ret = worker.seek(msgId);
        worker.resumeMessageListener();
        return ret;
    }

private:
    Client client_;
    ConsumerConfiguration config_;
};

inline MessageId DeserializeMsgId(const string& msgIdStr){
    char c = 0;
    int64_t one = 0, two = 0;
    int32_t three = 0, four = 0;
    stringstream ss(msgIdStr);
    ss >> c >> one >> c >> two >> c >> three >> c >> four >> c ;
    return MessageIdBuilder().ledgerId(one).entryId(two).batchIndex(three).batchSize(four).build();
}

void test() {
    ConsumerClient c1;
    ConsumerClient c2;
    ConsumerClient c3;
    PulsarConsumer cc;
    cc.Init();

    //--------------- 容器订阅--------------------------------------------------------------------
    //string presix = "persistent://";
    //vector<string> vec_topic;
    //vec_topic.emplace_back(presix + "pss/geek1/EEE-partition");
    //vec_topic.emplace_back(presix + "pss/geek1/EEE-partition-1");
    //vec_topic.emplace_back(presix + "pss/geek1/EEE-partition-2");
    //cc.Subscribe("容器订阅", vec_topic, c1.get());

    // string topic = "persistent://pss/geek1/AAA";
    // string topic1 = "persistent://pss/geek1/BBB";
    // string topic2 = "persistent://pss/geek1/CCC";
    // vector<string> partitions;
    // cc.GetPartsForTopic(topic, partitions);
    // display(partitions);

    // vector<string> partitions1;
    // cc.GetPartsForTopic(topic1, partitions1);
    // display(partitions1);


    // vector<string> partitions2;
    // cc.GetPartsForTopic(topic2, partitions2);
    // display(partitions2);



    //--------------- 单一订阅--------------------------------------------------------------------
    cc.Subscribe("user_c2", "persistent://ellen/free/test1", c1.get());
    // cc.Subscribe("单一订阅2", "persistent://moss/geek/FFF", c2.get());
    // cc.Subscribe("单一订阅3", "persistent://moss/geek/FFF", c3.get());



    //--------------- 正则表达式订阅 ---------------------------------------------------------------
    //cc.SubscribeWithRegex("正则订阅", "persistent://moss/geek/.*", c1.get());

    int ch;
    while(std::cin >> ch && ch != 0){}  

    //pulsar::Result result = cc.Seek(c1.get(), pulsar::MessageId::earliest());
    //if(result != pulsar::ResultOk) {
    //        cerr << "Error seek subscribe: " << result << endl;
    //        //return ;
    //}
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
    c1.destory();
    
}


int main() {
    test();
    return 0;
}



