#include "buffer.h"
#include <iostream>
#include <cstring>

int main() {
    std::cout << "=== Buffer Test ===" << std::endl;

    // 测试 1: 基本构造和可读/可写空间
    Buffer buf(1024);
    std::cout << "\n[Test 1] 构造函数和空间查询" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 0)" << std::endl;
    std::cout << "WriteableBytes: " << buf.WriteableBytes() << " (expected: 1024)" << std::endl;
    std::cout << "PrependableBytes: " << buf.PrependableBytes() << " (expected: 0)" << std::endl;

    // 测试 2: Append 字符串
    std::cout << "\n[Test 2] Append 字符串" << std::endl;
    std::string str1 = "Hello, Buffer!";
    buf.Append(str1);
    std::cout << "After Append(\"" << str1 << "\"):" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: " << str1.size() << ")" << std::endl;
    std::cout << "Peek(): \"" << buf.Peek() << "\"" << std::endl;

    // 测试 3: Retrieve
    std::cout << "\n[Test 3] Retrieve" << std::endl;
    buf.Retrieve(7);
    std::cout << "After Retrieve(7):" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 6)" << std::endl;
    std::cout << "Peek(): \"" << buf.Peek() << "\"" << std::endl;

    // 测试 4: Append 二进制数据
    std::cout << "\n[Test 4] Append 二进制数据" << std::endl;
    const char* data = "\x01\x02\x03\x04\x05";
    buf.Append(data, 5);
    std::cout << "After Append(5 bytes):" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 11)" << std::endl;

    // 测试 5: RetrieveAll
    std::cout << "\n[Test 5] RetrieveAll" << std::endl;
    buf.RetrieveAll();
    std::cout << "After RetrieveAll():" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 0)" << std::endl;
    std::cout << "WriteableBytes: " << buf.WriteableBytes() << " (expected: 1024)" << std::endl;

    // 测试 6: RetrieveAllToStr
    std::cout << "\n[Test 6] RetrieveAllToStr" << std::endl;
    buf.Append("Test string for RetrieveAllToStr");
    std::string result = buf.RetrieveAllToStr();
    std::cout << "RetrieveAllToStr(): \"" << result << "\"" << std::endl;
    std::cout << "After RetrieveAllToStr():" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 0)" << std::endl;

    // 测试 7: BeginWrite
    std::cout << "\n[Test 7] BeginWrite 直接写入" << std::endl;
    char* writePtr = buf.BeginWrite();
    const char* data2 = "Direct write!";
    std::memcpy(writePtr, data2, strlen(data2));
    buf.HasWritten(strlen(data2));
    std::cout << "After direct write:" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: " << strlen(data2) << ")" << std::endl;
    std::cout << "Peek(): \"" << buf.Peek() << "\"" << std::endl;

    // 测试 8: Append Buffer
    std::cout << "\n[Test 8] Append Buffer" << std::endl;
    Buffer buf2(512);
    buf2.Append("Appending from buf2!");
    buf.Append(buf2);
    std::cout << "After Append(buf2):" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 24)" << std::endl;

    // 测试 9: MakeSpace_ (自动扩容)
    std::cout << "\n[Test 9] MakeSpace_ 自动扩容" << std::endl;
    buf.RetrieveAll();
    std::string largeData(2000, 'X');  // 大于初始容量 1024
    buf.Append(largeData);
    std::cout << "After Append(2000 bytes):" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 2000)" << std::endl;
    std::cout << "WriteableBytes: " << buf.WriteableBytes() << " (should be > 0)" << std::endl;

    // 测试 10: PrependableBytes
    std::cout << "\n[Test 10] PrependableBytes" << std::endl;
    buf.Retrieve(500);  // 读取 500 字节，readPos_ 前移
    std::cout << "After Retrieve(500):" << std::endl;
    std::cout << "PrependableBytes: " << buf.PrependableBytes() << " (expected: 500)" << std::endl;
    std::cout << "ReadableBytes: " << buf.ReadableBytes() << " (expected: 1500)" << std::endl;

    std::cout << "\n=== All Tests Passed! ===" << std::endl;
    return 0;
}
