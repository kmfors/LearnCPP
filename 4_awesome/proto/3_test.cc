#include <string>
#include <iostream>
#include "biz_message.pb.h"
#include "spdlogger.h"

sp_spd_log g_logger;
sp_spd_tp g_logger_tp;

using namespace std;

int main() {

    CreateLogger("./log", "test", 1);
    Htpp htpp;
    
    DHC_Second ss;
    //ss.add_detail()->set_biz_name("测试");
    ss.set_biz_name("www");
    
    // if (ss.has_detail()){
    //     std::cout << "has_detail is true" << std::endl;
    // } else {
    //     std::cout << "has_detail is false" << std::endl;
    // }
    htpp.set_msgtype("1111111");
    MLOGI("detail_size(): {}", ss.detail_size());
    if(ss.detail_size() <= 0){
        htpp.set_is_null(true);
        MLOGI("is_null: true");
    } else {
        MLOGI("is_null: false");
    }
    string* htppdata = htpp.mutable_data();

    DHC_Second sec;
    sec.mutable_detail()->CopyFrom(const RepeatedPtrField<DHC_First> &other)
    
   
    MLOGI("debug message1: {}", htpp.ShortDebugString());
    ss.SerializeToString(htppdata);
    MLOGI("Serialize data: {}", htpp.data());
    MLOGI("debug is_null: {}", htpp.is_null());
    MLOGI("debug message2: {}", htpp.ShortDebugString());

    // string message;
    // htpp.SerializeToString(&message);

    // cout << "Serialize:" << message << endl;
    return 0;
}
