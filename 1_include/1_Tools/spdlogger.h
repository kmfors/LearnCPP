#ifndef _SPDLOGGER_H_
#define _SPDLOGGER_H_

#define SPDLOG_DISABLE_DEFAULT_LOGGER

#include "spdlog/spdlog.h"
#include "spdlog/async_logger.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "utilTools.h"



// ---------------------------------------- spdlog ---------------------------------------------------------
using  sp_spd_log = std::shared_ptr<spdlog::logger>;
using  sp_spd_tp = std::shared_ptr<spdlog::details::thread_pool>;
// 全局日志器
extern std::shared_ptr<spdlog::logger> g_logger;
// 全局日志线程池
extern std::shared_ptr<spdlog::details::thread_pool> g_logger_tp;


#define LOG_TEMPLATE(notes, cust_level, format_str, ...)                                            \
    do {                                                                                            \
        if(g_logger && g_logger->level() <= spdlog::level::notes)                                   \
            g_logger->cust_level("[{}:{}] " format_str, __FUNCTION__, __LINE__, ##__VA_ARGS__);     \
    } while(false)

#define LOGD(format_str, ...)   LOG_TEMPLATE(debug, debug, format_str, ##__VA_ARGS__)
#define LOGI(format_str, ...)   LOG_TEMPLATE(info, info, format_str, ##__VA_ARGS__)
#define LOGW(format_str, ...)   LOG_TEMPLATE(warn, warn, format_str, ##__VA_ARGS__)
#define LOGE(format_str, ...)   LOG_TEMPLATE(err, error, format_str, ##__VA_ARGS__)

#define MLOG_TEMPLATE(notes, cust_level, format_str, ...)                                           \
    do {                                                                                            \
        fmt::print("[{}] [{}:{}] " format_str "\n", util::GetCurrentFormattedTime(),                \
                __FUNCTION__, __LINE__, ##__VA_ARGS__);                                             \
        if(g_logger && g_logger->level() <= spdlog::level::notes)                                   \
            g_logger->cust_level("[{}:{}] " format_str, __FUNCTION__, __LINE__, ##__VA_ARGS__);     \
    } while(false)

#define MLOGD(format_str, ...)   MLOG_TEMPLATE(debug, debug, format_str, ##__VA_ARGS__)
#define MLOGI(format_str, ...)   MLOG_TEMPLATE(info, info, format_str, ##__VA_ARGS__)
#define MLOGW(format_str, ...)   MLOG_TEMPLATE(warn, warn, format_str, ##__VA_ARGS__)
#define MLOGE(format_str, ...)   MLOG_TEMPLATE(err, error, format_str, ##__VA_ARGS__)




#define LOGGER_LEVEL_SET(logger, cust_level)   \
    logger->set_level(spdlog::level::cust_level)
inline bool setLogLevel(int level = 2) {
    if (!g_logger)
        return false;
    if (level == 0)
        LOGGER_LEVEL_SET(g_logger, trace);
    else if (level == 1)
        LOGGER_LEVEL_SET(g_logger, debug);
    else if (level == 2)
        LOGGER_LEVEL_SET(g_logger, info);
    else if (level == 3)
        LOGGER_LEVEL_SET(g_logger, warn);
    else if (level == 4)
        LOGGER_LEVEL_SET(g_logger, err);
    else if (level == 5)
        LOGGER_LEVEL_SET(g_logger, critical);
    else if (level == 6)
        LOGGER_LEVEL_SET(g_logger, off);
    else {
        LOGGER_LEVEL_SET(g_logger, info);
    }
    return true;
}
#undef LOGGER_LEVEL_SET


inline bool CreateLogger(const char *log_path, const char *log_name,
                         int log_level = spdlog::level::info) {

    if (!g_logger && log_level != spdlog::level::off) {

        if (log_path == nullptr || log_name == nullptr) {
            MLOGE("log_path or log_name is nullptr");
            return false;
        }
        int ret = util::CreateDirectory(log_path);
        if (ret != 0) {
            MLOGE("CreateDirectory failed, ret:{}, log_path:{}", ret, log_name);
            return false;
        }
        std::string log_file = fmt::format("{}/{}.log", log_path, log_name);
        size_t queue_size = 65536;
        g_logger_tp = std::make_shared<spdlog::details::thread_pool>(queue_size, 1);
        auto sink = std::make_shared<spdlog::sinks::daily_file_format_sink_mt>(
                log_file, 0, 0);
        g_logger = std::make_shared<spdlog::async_logger>(
                log_name, sink, g_logger_tp, spdlog::async_overflow_policy::block);
        if (!g_logger) {
            MLOGE("Create logger failed, logger:{}, log_name");
            return false;
        }
        spdlog::register_logger(g_logger);
        spdlog::flush_every(std::chrono::milliseconds(500));
        g_logger->flush_on(spdlog::level::err);

        g_logger->set_pattern("[%T.%f] [%l] [%t] %v");
        setLogLevel(log_level);
        MLOGD("Create logger success, logger:{}, log_file:{}", log_name, log_file);
    }
    return true;
}

inline void DropLogger(const char* log_name){
    if(g_logger){
        MLOGI("Destory logger success, logger:{}", log_name);
        g_logger->flush();
        g_logger.reset();
        spdlog::drop(log_name);
        fflush(stdout);
    }
}

// ---------------------------------------- spdlog ---------------------------------------------------------

#endif
