#include "spdlog/fmt/bundled/core.h"
#include "spdlogger.h"


// 全局日志器
std::shared_ptr<spdlog::logger> g_logger;                                                                                                   
// 全局日志线程池
std::shared_ptr<spdlog::details::thread_pool> g_logger_tp;


int main(int argc, char** argv) {
    // CreateLogger("./log", "pss", 1);

    const char* instanceName = getenv("INSTANCE_NAME");
    if (instanceName == nullptr || strlen(instanceName) <= 0) {
        MLOGE("获取实例名称失败, 环境变量配置名, INSTANCE_NAME");
    }

    
    return 0;
}
