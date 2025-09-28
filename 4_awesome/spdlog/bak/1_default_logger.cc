
// 若使用日志宏，则需要定义宏
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

// 关闭spdlog的默认日志器的生成
//#define SPDLOG_DISABLE_DEFAULT_LOGGER

#include "spdlog/spdlog.h"

int main() {

    // 一旦定义SPDLOG_DISABLE_DEFAULT_LOGGER宏，下面的API接口使用则报错
    spdlog::set_level(spdlog::level::debug);

    spdlog::trace("spdlog::trace!");
    spdlog::debug("spdlog::debug!");
    spdlog::info("spdlog::info!");
    spdlog::warn("spdlog::warn!");
    spdlog::error("spdlog::error!");
    spdlog::critical("spdlog::critical!");
    
   
    // Compile time log levels（编译时间日志级别）
    // 注意：这不会改变当前的日志级别（当代码被编译时，只有高于或等于该级别的日志才会被记录）。
    // 使用宏函数前，必须要在引入 spdlog.h 之前定义 SPDLOG_ACTIVE_LEVEL
    SPDLOG_TRACE("SPDLOG_TRACE 宏");
    SPDLOG_DEBUG("SPDLOG_DEBUG 宏");

    return 0;
}
