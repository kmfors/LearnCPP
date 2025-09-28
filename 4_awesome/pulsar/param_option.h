#include "fmt/core.h"
#include <string>

using std::string;

struct ParamOption {
    int count = 1;  // 次数
    int speed = 1;  // 延迟时间，秒/次
    string topic = "ellen/free/default";     
    string msg;    
};

int main_start(int argc, char* argv[], ParamOption& param);