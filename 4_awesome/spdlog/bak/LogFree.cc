//#include "spdlog/spdlog.h"
#include <iostream>
#include "utilTools.h"
#include "spdlog/sinks/basic_file_sink.h"


void basic_logfile_example(std::string log_name){
    if(log_name[log_name.size()] == '/')    return;
    size_t pos = log_name.find_last_of('/') + 1;
    log_name.insert(pos, "-");
    log_name.insert(pos, std::to_string(util::getDateNow()));

    try {
        auto logger = spdlog::basic_logger_st("file_logger", log_name);
    } catch (const spdlog::spdlog_ex &ex){
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

int main(){
    basic_logfile_example("logs/qwe.log");

    return 0;
}