
// 不构造spdlog的默认日志器
#define SPDLOG_DISABLE_DEFAULT_LOGGER

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void stdout_logger() {
    // create a color multi-threaded logger
    auto out_logger = spdlog::stdout_color_mt("stdout");    
    spdlog::get("stdout")->info("stdout loggers output:{}", "spdlog::get");
    //out_logger->info("")
}

void stderr_logger() {
    // create a color multi-threaded logger  
    auto err_logger = spdlog::stderr_color_mt("stderr");    
    spdlog::get("stderr")->info("stderr loggers output");
}

// st是单线程，mt是多线程
void stdout_custom() {
    // 先注释掉 SPDLOG_DISABLE_DEFAULT_LOGGER 宏
    auto stdout_logger = spdlog::create<spdlog::sinks::stdout_color_sink_st> ("stdout_logger");
    spdlog::get("stdout_logger")->info("hello {}", "world");
    
}

int main() {
    stdout_logger();
    //stdout_deafult();
    //stdout_custom();
    return 0;
}