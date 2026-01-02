#include <iostream>
#include <type_traits>
#include <vector>

/**
 * C++14 auto 函数返回类型推导示例
 *
 * 注意：所有return语句必须返回相同的类型
 */

// 基本算术运算
auto add(int a, int b) {
    return a + b;
}

auto multiply(double a, double b) {
    return a * b;
}

// 返回容器
auto createVector() {
    return std::vector<int>{1, 2, 3, 4, 5};
}

auto createString() {
    return std::string{"Hello, C++14!"};
}

// 带模板的auto返回类型
template<typename T>
auto processValue(const T& value) {
    return value * 2;
}

// 递归函数需要显式返回类型
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// 返回auto引用（C++14支持）
auto& getFirstElement(std::vector<int>& vec) {
    return vec[0];
}

int main() {
    // 基本类型推导
    auto result1 = add(5, 3);           // result1 推导为 int
    auto result2 = multiply(2.5, 4.0);  // result2 推导为 double
    auto result3 = createString();      // result3 推导为 std::string

    // 模板函数推导
    auto result4 = processValue(10);    // result4 推导为 int
    auto result5 = processValue(2.5);   // result5 推导为 double

    std::cout << "add(5, 3) = " << result1 << std::endl;
    std::cout << "multiply(2.5, 4.0) = " << result2 << std::endl;
    std::cout << "String: " << result3 << std::endl;
    std::cout << "processValue(10) = " << result4 << std::endl;
    std::cout << "processValue(2.5) = " << result5 << std::endl;

    return 0;
}