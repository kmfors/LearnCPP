#pragma once

#include <memory>
#include <string>
#include <thread>
#include "../buffer/buffer.h"
#include "blockqueue.h"


class Log {
private:
    using BlockDequeUPtr = std::unique_ptr<BlockDeque<std::string>>;
    using ThreadUPtr = std::unique_ptr<std::thread>;
    using FilePtr = FILE*;
    using DateTime = struct tm;
    using TimeVal = struct timeval;
    using LockGuard = std::lock_guard<std::mutex>;
    using LockUnique = std::unique_lock<std::mutex>;
    static constexpr int kLogPathLen = 256;
    static constexpr int kLogNameLen = 256;
    static constexpr int kMaxLines = 5000;
public:
    void init(int level, const char* path = "./log",
        const char* suffix = ".log",
        int maxQueueCapacity = 1024);

    static Log* instance();
    static void flushLogThread();

    void write(int level, const char* format,...);
    void flush();

    int level();
    void set_level(int level);
    bool isOpen() { return is_open_;}

private:
    Log();
    void appendLogLevelTitle_(int level);
    virtual ~Log();
    void asyncWrite();

private:
    std::mutex mtx_;
    BlockDequeUPtr deque_;
    ThreadUPtr writeThread_;
    FilePtr fp_;
    Buffer buf_;

    int line_count_;
    bool is_async_;
    int today_;
    const char* path_;
    const char* suffix_;

    bool is_open_;
    int level_;
    int max_lines_;
};