#include "buffer.h"
#include <cassert>
#include <unistd.h>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/uio.h>
#endif

Buffer::Buffer(std::size_t initialSize)
    : buffer_(kCheapPrepend + initialSize )
    , readPos_(kCheapPrepend)
    , writePos_(kCheapPrepend)
{}


std::size_t Buffer::readableBytes() const {
    return writePos_ - readPos_;
}

std::size_t Buffer::writableBytes() const {
    return buffer_.size() - writePos_;
}

std::size_t Buffer::prependableBytes() const {
    return readPos_;
}

const char* Buffer::peek() const {
    return begin() + readPos_;
}

void Buffer::retrieve(std::size_t len) {
    assert(len <= readableBytes());
    if (len < readableBytes())
        readPos_ += len;
    else
        retrieveAll();
}

void Buffer::retrieveAll() {
    readPos_ = writePos_ = kCheapPrepend;
}

void Buffer::retrieveUntil(const char* end) {
    assert(peek() <= end);
    retrieve(end - peek());
}

std::string Buffer::retrieveAsString(std::size_t len) {
    std::string str(peek(), len);
    retrieve(len);
    return str;
}

std::string Buffer::retrieveAllAsString() {
    return retrieveAsString(readableBytes());
}

const char* Buffer::beginWrite() const {
    return begin() + writePos_;
}

char* Buffer::beginWrite() {
    return begin() + writePos_;
}

void Buffer::commitWrite(std::size_t len) {
    writePos_ += len;
}

void Buffer::append(const std::string& str) {
    append(str.data(), str.length());
}

void Buffer::append(const void* data, std::size_t len) {
    append(static_cast<const char*>(data), len);
}

void Buffer::append(const char* data, std::size_t len) {
    assert(data != nullptr || len == 0);
    ensureWritable(len);
    std::copy(data, data + len, beginWrite());
    commitWrite(len);
}

void Buffer::append(const Buffer& extrabuf) {
    append(extrabuf.peek(), extrabuf.readableBytes());
}

void Buffer::ensureWritable(std::size_t len) {
    if (writableBytes() < len) {
        makeSpace(len);
    }
}

ssize_t Buffer::readFromFd(int fd, int* saveError) {
    const std::size_t writable = writableBytes();
    char extrabuf[65535];

#ifdef _WIN32
    ssize_t len = recv(fd, extrabuf, sizeof(extrabuf), 0);
    if (len < 0) {
        *saveError = WSAGetLastError();
    } else if (static_cast<std::size_t>(len) <= writable) {
        std::copy(extrabuf, extrabuf + len, begin() + writePos_);
        writePos_ += len;
    } else {
        std::copy(extrabuf, extrabuf + writable, begin() + writePos_);
        writePos_ = buffer_.size();
        append(extrabuf, len - writable);
    }
    return len;
#else
    struct iovec iov[2];
    iov[0].iov_base = begin() + writePos_;
    iov[0].iov_len = writable;
    const int iovcnt = ((writable < sizeof(extrabuf)) ? 2 : 1);
    if (iovcnt == 2) {
        iov[1].iov_base = extrabuf;
        iov[1].iov_len = sizeof(extrabuf);
    }
    const ssize_t len = ::readv(fd, iov, iovcnt);
    if (len < 0) {
        *saveError = errno;
    } else if (len <= writable) {
        writePos_ += len;
    } else {
        writePos_ = buffer_.size();
        append(extrabuf, len - writable);
    }
    return len;
#endif
}

ssize_t Buffer::writeToFd(int fd, int* saveError) {
    std::size_t readable = readableBytes();
    if (readable == 0) return 0;

    ssize_t len = ::write(fd, peek(), readable);
    if (len < 0) {
        *saveError = errno;
    } else {
        readPos_ += len;
    }
    return len;
}

char* Buffer::begin() {
    return buffer_.data();
}

const char* Buffer::begin() const {
    return buffer_.data();
}

void Buffer::makeSpace(std::size_t len) {
    if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
        buffer_.resize(writePos_ + len);
    } else {
        std::size_t readable = readableBytes();
        std::copy(begin() + readPos_, begin() + writePos_, begin() + kCheapPrepend);
        readPos_ = kCheapPrepend;
        writePos_ = readPos_ + readable;
    }
}

// SerializeAsString()    // 返回值
// SerializeToString()    // 输出参数