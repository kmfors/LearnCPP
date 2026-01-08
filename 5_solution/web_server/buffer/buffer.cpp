#include "buffer.h"
#include <cassert>
#include <unistd.h> // write
#include <cstring> // memset
// read
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/uio.h>
#endif

Buffer::Buffer(int initBuffSize) : buffer_(initBuffSize), readPos_(0), writePos_(0) {}

size_t Buffer::ReadableBytes() const {
    return writePos_ - readPos_;
}

size_t Buffer::WriteableBytes() const {
    return buffer_.size() - writePos_;
}

size_t Buffer::PrependableBytes() const {
    return readPos_;
}

const char* Buffer::Peek() const {
    return BeginPtr_() + readPos_;
}

void Buffer::Retrieve(size_t len) {
    assert( len <= ReadableBytes());
    readPos_ += len;
}

void Buffer::RetrieveAll() {
    memset(&buffer_[0], 0, buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}

std::string Buffer::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char* Buffer::BeginWriteConst() const {
    return BeginPtr_() + writePos_;
}

char* Buffer::BeginWrite() {
    return BeginPtr_() + writePos_;
}

void Buffer::HasWritten(size_t len) {
    writePos_ += len;
}

void Buffer::Append(const std::string& str) {
    Append(str.data(), str.length());
}

void Buffer::Append(const void* data, size_t len) {
    assert(data);
    Append(static_cast<const char*>(data), len);
}

void Buffer::Append(const char* str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}

void Buffer::Append(const Buffer& buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

void Buffer::EnsureWriteable(size_t len) {
    if (WriteableBytes() < len) {
        MakeSpace_(len);
    }
    assert(WriteableBytes() >= len);
}

ssize_t Buffer::ReadFd(int fd, int* saveError) {
    const size_t writableSize = WriteableBytes();
    char buf[65535];

#ifdef _WIN32
    // Windows: 使用 recv，用临时缓冲区提高效率
    ssize_t len = recv(fd, buf, sizeof(buf), 0);
    if (len < 0) {
        *saveError = WSAGetLastError();
    } else if (static_cast<size_t>(len) <= writableSize) {
        std::copy(buf, buf + len, BeginPtr_() + writePos_);
        writePos_ += len;
    } else {
        writePos_ = buffer_.size();
        Append(buf, len - writableSize);
    }
    return len;
#else
    // Linux/Mac: 使用 readv 分散读
    struct iovec iov[2];
    iov[0].iov_base = BeginPtr_() + writePos_;
    iov[0].iov_len = writableSize;
    iov[1].iov_base = buf;
    iov[1].iov_len = sizeof(buf);

    const ssize_t len = readv(fd, iov, 2);
    if (len < 0) {
        *saveError = errno;
    } else if (static_cast<size_t>(len) <= writableSize) {
        writePos_ += len;
    } else {
        writePos_ = buffer_.size();
        Append(buf, len - writableSize);
    }
    return len;
#endif
}


ssize_t Buffer::WriteFd(int fd, int* errcode) {
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);
    if (len < 0) {
        *errcode = errno;
    } else {
        readPos_ += len;
    }
    return len;
}

char* Buffer::BeginPtr_() {
    return &*buffer_.begin();
}

const char* Buffer::BeginPtr_() const {
    return &*buffer_.begin();
}

void Buffer::MakeSpace_(size_t len) {
    if (WriteableBytes() + PrependableBytes() < len) {
        buffer_.resize(writePos_ + len +1 );
    } else {
        size_t readableSize = ReadableBytes();
        std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
        readPos_ = 0;
        writePos_ = readPos_ + readableSize;
        assert(readableSize == ReadableBytes());
    }
}