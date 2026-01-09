#pragma once

#include <vector>
#include <string>

class Buffer {
public:
    // 前置预留空间大小
    static constexpr std::size_t kCheapPrepend = 8;
    // 缓冲区初始大小
    static constexpr std::size_t kInitialSize = 1024;

    explicit Buffer(std::size_t initialSize = 1024);

    ~Buffer() = default;

    // 可读字节数（已写入但未读取的数据）
    std::size_t readableBytes() const;
    // 可写字节数（空闲空间）
    std::size_t writableBytes() const;
    // 可前置字节数（已读取的空间，可复用）
    std::size_t prependableBytes() const;

    // 返回可读数据的起始指针
    const char* peek() const;

    void ensureWritable(std::size_t len);

    void commitWrite(std::size_t len);

    void retrieve(std::size_t len);
    void retrieveUntil(const char* end);
    void retrieveAll();
    std::string retrieveAsString(std::size_t len);
    std::string retrieveAllAsString();


    const char* beginWrite() const;
    char* beginWrite();

    void append(const std::string& str);
    void append(const char* data, std::size_t len);
    void append(const void* data, std::size_t len);
    void append(const Buffer& buff);


    ssize_t readFromFd(int fd, int* saveError);
    ssize_t writeToFd(int fd, int* saveError);

private:
    char* begin();
    const char* begin() const;
    void makeSpace(std::size_t len);

    std::vector<char> buffer_;
    std::size_t readPos_;
    std::size_t writePos_;
};
