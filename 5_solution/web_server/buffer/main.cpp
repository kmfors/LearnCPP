#include "buffer.h"
#include <iostream>
#include <cstring>

#define ENDL "\n\n"

int main() {
    std::cout << "=== Buffer Test ===" << std::endl;

    // 测试 1: 基本构造和可读/可写空间
    Buffer buffer(1024);
    std::cout << "[Test 1] 构造函数和空间查询" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 0)" << std::endl;
    std::cout << "writableBytes: " << buffer.writableBytes() << " (expected: 1024)" << std::endl;
    std::cout << "prependableBytes: " << buffer.prependableBytes() << " (expected: 8)" << ENDL;

    // 测试 2: append 字符串
    std::cout << "[Test 2] append 字符串" << std::endl;
    std::string str1 = "Hello, Buffer!";
    buffer.append(str1);
    std::cout << "After append(\"" << str1 << "\"):" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: " << str1.size() << ")" << std::endl;
    std::cout << "peek(): \"" << buffer.peek() << "\"" << ENDL;

    // 测试 3: retrieve
    std::cout << "[Test 3] retrieve" << std::endl;
    buffer.retrieve(7);
    std::cout << "After retrieve(7):" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 7)" << std::endl;
    std::cout << "peek(): \"" << buffer.peek() << "\"" << ENDL;

    // 测试 4: retrieveAsString
    std::cout << "[Test 4] retrieveAsString" << std::endl;
    std::string part = buffer.retrieveAsString(7);
    std::cout << "retrieveAsString(7): \"" << part << "\"" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 0)" << ENDL;

    // 测试 5: retrieveAll
    std::cout << "[Test 5] retrieveAll" << std::endl;
    buffer.append("Test string for retrieveAll");
    buffer.retrieveAll();
    std::cout << "After retrieveAll():" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 0)" << std::endl;
    std::cout << "writableBytes: " << buffer.writableBytes() << " (expected: 1024)" << ENDL;

    // 测试 6: retrieveAllAsString
    std::cout << "[Test 6] retrieveAllAsString" << std::endl;
    buffer.append("Test string for retrieveAllAsString");
    std::string result = buffer.retrieveAllAsString();
    std::cout << "retrieveAllAsString(): \"" << result << "\"" << std::endl;
    std::cout << "After retrieveAllAsString():" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 0)" << ENDL;

    // 测试 7: beginWrite 直接写入
    std::cout << "[Test 7] beginWrite 直接写入" << std::endl;
    char* writePtr = buffer.beginWrite();
    const char* data2 = "Direct write!";
    std::memcpy(writePtr, data2, strlen(data2));
    buffer.commitWrite(strlen(data2));
    std::cout << "After direct write:" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: " << strlen(data2) << ")" << std::endl;
    std::cout << "peek(): \"" << buffer.peek() << "\"" << ENDL;

    // 测试 8: append Buffer
    std::cout << "[Test 8] append Buffer" << std::endl;
    Buffer buf2(512);
    buf2.append("Appending from buf2!");
    buffer.append(buf2);
    std::cout << "After append(buf2):" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 24)" << ENDL;

    // 测试 9: makeSpace 自动扩容
    std::cout << "[Test 9] makeSpace 自动扩容" << std::endl;
    buffer.retrieveAll();
    std::string largeData(2000, 'X');  // 大于初始容量 1024
    buffer.append(largeData);
    std::cout << "After append(2000 bytes):" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 2000)" << std::endl;
    std::cout << "writableBytes: " << buffer.writableBytes() << " (should be > 0)" << ENDL;

    // 测试 10: prependableBytes 和 retrieveUntil
    std::cout << "[Test 10] prependableBytes 和 retrieveUntil" << std::endl;
    buffer.retrieveAll();
    buffer.append("Hello World!");
    const char* worldPos = buffer.peek() + 6;  // 指向 "World!"
    buffer.retrieveUntil(worldPos);
    std::cout << "After retrieveUntil(\"World!\"):" << std::endl;
    std::cout << "peek(): \"" << buffer.peek() << "\"" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 6)" << ENDL;

    // 测试 11: append 二进制数据
    std::cout << "[Test 11] append 二进制数据" << std::endl;
    const char* binData = "\x01\x02\x03\x04\x05";
    buffer.append(binData, 5);
    std::cout << "After append(5 binary bytes):" << std::endl;
    std::cout << "readableBytes: " << buffer.readableBytes() << " (expected: 11)" << ENDL;

    // 测试 12: ensureWritable
    std::cout << "[Test 12] ensureWritable" << std::endl;
    buffer.retrieveAll();
    buffer.ensureWritable(2048);
    std::cout << "After ensureWritable(2048):" << std::endl;
    std::cout << "writableBytes: " << buffer.writableBytes() << " (should be >= 2048)" << ENDL;

    std::cout << "=== All Tests Passed! ===" << std::endl;
    return 0;
}
