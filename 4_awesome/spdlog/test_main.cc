#include "spdlogger.h"

// 全局日志器
//std::shared_ptr<spdlog::logger> g_logger;                                                                                                   
// 全局日志线程池
//std::shared_ptr<spdlog::details::thread_pool> g_logger_tp;

sp_spd_log g_logger;
sp_spd_tp g_logger_tp;

int main() {
    //daily_example();
    CreateLogger("./log", "test", 1);

    // LOGD("spdlog::debug!");
    // LOGI("spdlog::info!");
    // LOGW("spdlog::warn!");
    // LOGE("spdlog::error!");

    MLOGD("spdlog::debug!");
    MLOGI("spdlog::info!");
    MLOGW("spdlog::warn!");
    MLOGE("spdlog::error!");
    
    return 0;
}
