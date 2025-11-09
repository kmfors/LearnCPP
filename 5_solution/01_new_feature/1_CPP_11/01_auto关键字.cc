#include <vector>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

/*
  - auto 关键字：根据初始化表达式自动推导变量的类型（编译期间）。
  - auto 声明的变量必须立即初始化。
  - 优点：
    - 避免冗长的类型名，尤其是容器的迭代器、lambda表达式
    - 确保类型正确，避免类型不匹配而发生的隐式转换
    - 泛型编程，若无法预知返回类型，可以使用auto与decltype的结合使用

  - 注意点：
    - auto 会忽略初始化表达式的顶层const和引用，即推导为最纯粹的类型。
      如果需要推导出引用或顶层 const，必须显式指出
    - 在范围 for 循环中使用，简洁安全
*/

// 1、基本使用
void demo1() {
    auto x = 5;         // 推导为 int
    auto y = "hello";   // 推导为 const char*

    std::vector<int> vec = {1, 2, 3};
    auto iter = vec.begin(); // 推导为 std::vector<int>::iterator
}

// 2、const与引用的显式指定
void demo2() {
    // auto &f = 42;      // 错误：不能为非常量引用绑定字面值
    const auto &g = 42;   // 正确：常量引用可以绑定字面值
}

// 3、范围 for 循环
void demo3() {
    std::vector<std::string> words = {"hello", "world", "!\n"};

    // 拷贝元素（成本高）
    for (auto word : words) {
        std::cout << word << " ";
    }

    // 通过常量引用访问元素（推荐，避免拷贝）
    for (const auto &word : words) {
        std::cout << word << " ";
    }

    // 如果需要修改元素，使用非常量引用
    for (auto &word : words) {
        word = "new";
    }
}




int main() {
    demo1();
    demo2();
    demo3();
    return EXIT_SUCCESS;
}