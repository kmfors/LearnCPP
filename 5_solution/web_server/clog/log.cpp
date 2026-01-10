#include "log.h"
#include <io.h> // mkdir
#include <cstdarg> // va_list
#include <sys/time.h>
#include <cstring>

Log::Log() {
    deque_ = nullptr;
    writeThread_ = nullptr;
    fp_ = nullptr;

    line_count_ = 0;
    is_async_ = false;
    today_ = 0;
}

Log::~Log() {
    if (writeThread_ && writeThread_->joinable()) {
        while (!deque_->empty()) {
            deque_->flush();
        };
        if (deque_) deque_->close();
        writeThread_->join();
    }
    if (fp_) {
        LockGuard lock(mtx_);
        flush();
        fclose(fp_);
    }
}

int Log::level() {
    LockGuard lock(mtx_);
    return level_;
}

void Log::set_level(int level) {
    LockGuard lock(mtx_);
    level_ = level;
}

void Log::init(int level, const char* path, const char* suffix, int maxQueueSize) {
    is_open_ = true;
    line_count_ = 0;
    level_ = level;
    path_ = path;
    suffix_ = suffix;
    is_async_ = (maxQueueSize > 0);

    if (is_async_ && !deque_) {
        BlockDequeUPtr newDeque(new BlockDeque<std::string>());
        deque_ = std::move(newDeque);

        ThreadUPtr newThread(new std::thread(flushLogThread));
        writeThread_ = std::move(newThread);
    }
    // 虽然localtime不是线程安全，但日志初始化放在单线程进行，也就没关系
    std::time_t timer = std::time(nullptr);
    DateTime dt = *(std::localtime(&timer));
    today_ = dt.tm_mday;

    char fileName[kLogPathLen] = {0};
    snprintf(fileName, kLogPathLen, "%s/%04d-%02d-%02d%s", path_,
        dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, suffix_);
    assert(std::strlen(path_) + 10 + std::strlen(suffix_) < kLogPathLen);

    LockGuard lock(mtx_);
    buf_.retrieveAll();
    if (fp_) { flush(); fclose(fp_);}

    int retry = 0;
    fp_ = fopen(fileName, "a");
    while (!fp_) {
        mkdir(path_);
        fp_ = fopen(fileName, "a");
        if (fp_) break;
        // 重试3次还是失败就退出，防止死循环
        assert(++retry < 3);
    }
}

void Log::write(int level, const char* format,...) {
    TimeVal now = {0, 0};
    gettimeofday(&now, nullptr);
    std::time_t tSec = now.tv_sec;
    std::tm dt;
#ifdef _WIN32
    localtime_s(&dt, &tSec);
#else
    localtime_r(&dt, &tSec);
#endif
    int reserve = kLogNameLen - 72; // 预留72个空间给日志特征，防止溢出截断
    if (today_ != dt.tm_mday || (line_count_ && (line_count_ % kMaxLines == 0)))
    {
        char tail[36] = {0};
        snprintf(tail, sizeof(tail), "%04d-%02d-%02d", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday);

        char newFile[kLogNameLen];
        if (today_ != dt.tm_mday) {
            snprintf(newFile, reserve, "%s/%s%s", path_, tail, suffix_);
            today_ = dt.tm_mday;
            line_count_ = 0;
        } else {
            snprintf(newFile, reserve, "%s/%s-%d%s", path_, tail,
                (line_count_ / kMaxLines), suffix_);
        }
        LockGuard lock(mtx_);
        flush();
        fp_ = fopen(newFile, "a");
    }
    std::va_list vaList;
    {
        LockUnique lock(mtx_);
        line_count_++;
        int n = snprintf(buf_.beginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
            dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday,
            dt.tm_hour, dt.tm_min, dt.tm_sec, now.tv_usec);

        buf_.commitWrite(n);
        appendLogLevelTitle_(level);

        va_start(vaList, format);
        int m = vsnprintf(buf_.beginWrite(), buf_.writableBytes(), format, vaList);
        va_end(vaList);

        buf_.commitWrite(m);
        buf_.append("\n\0", 2);

        if (is_async_ && deque_ && !deque_->full()) {
            deque_->push_back(buf_.retrieveAllAsString());
        } else {
            fputs(buf_.peek(), fp_);
        }
        buf_.retrieveAll();
    }
}


void Log::appendLogLevelTitle_(int level) {
    switch (level) {
    case 0:
        buf_.append("[debug]", 9);
        break;
    case 1:
        buf_.append("[info]", 9);
        break;
    case 2:
        buf_.append("[warn]", 9);
        break;
    case 3:
        buf_.append("[error]", 9);
        break;
    default:
        buf_.append("[info]", 9);
        break;
    }
}

void Log::flush() {
    if (is_async_) {
        deque_->flush();
    }
    std::fflush(fp_);
}

void Log::asyncWrite() {
    std::string str = "";
    while (deque_->pop(str)) {
        LockGuard lock(mtx_);
        fputs(str.c_str(), fp_);
    }
}

Log* Log::instance() {
    static Log inst;
    return &inst;
}

void Log::flushLogThread() {
    Log::instance()->asyncWrite();
}


/*
笔记：
snprintf 的 size 参数已经包含了 null 终止符的位置
char buffer[N];
snprintf(buffer, N, ...);  // 正确：N 是数组大小
snprintf(buffer, N-1, ...);  // 错误：浪费空间

*/