#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

/**
 * ============================================================================
 * KNOWLEDGE: C++11 auto 关键字使用规则
 *
 * auto 使用规则：
 *  1.推导的变量必须立即初始化（类型推导依赖初始值）
 *  2.常量与非常量的引用推导,const必须显式
 *  3.auto 仅仅支持普通函数与lambda的返回推导，参数推导不支持（C++11限制）
 *  4.一行声明多个变量时，类型必须一致
 *  5.不能用于非静态成员变量（C++11限制）
 *  6.不能用于数组类型推导
 *  7.不能用于模板参数
 *
 * 推导规则：
 * - 忽略顶层const（保留底层const）
 * - 引用类型推导为引用指向的类型
 *
 * 优点：
 *  - 避免冗长的类型名，尤其是容器的迭代器、lambda表达式
 *  - 确保类型正确，避免类型不匹配而发生的隐式转换
 *  - 泛型编程，若无法预知返回类型，可以使用auto与decltype的结合使用
 * 注意点：
 *  - auto 会忽略初始化表达式的顶层const和引用，即推导为最纯粹的类型。
 *  - 如果需要推导出引用或顶层 const，必须显式指出
 *  - 在范围 for 循环中使用，简洁安全
 * ============================================================================
*/

// R5: 不能用于非静态成员变量
struct MyStruct {
    //auto member = 10;         // 错误！C++11不支持
    static const auto static_member = 10;  // 正确：静态成员可以
};

std::vector<int> vec = {1, 2, 3};

int main() {
    // R1
    auto a = 5;                 // 推导为 int
    auto b = "hello, world";    // 推导为 const char*
    auto c = vec.begin();       // 推导为 std::vector<int>::iterator

    // R2
    // auto &c = 42;            // 错误！非常量引用推导，不能绑定字面值
    const auto &d = 42;         // 正确：常量引用推导，可以绑定字面值

    // R3
    auto lam1 = [](int val) { return val * 2; };  // 支持
    //auto lam2 = [](int val) { return val * 2; };  // 错误！C++11不支持

    // R4
    auto x = 1, y = 2, z = 3;       // 正确：都是int
    // auto i = 1, j = 2.0;         // 错误！类型不一致(int和double)

    // R6
    int arr[] = {1, 2, 3};
    auto arr2 = arr;                // arr2 是 int*，不是数组！
    // auto arr3[] = {1, 2, 3};     // 错误！不能声明auto数组

    // R7
    vector<int> vec;
    // vector<auto> v = vec;        // 错误！

    // 实用场景: 范围 for 循环操作
    vector<string> words{"hello", "world", "school", "!\n"};
    for (auto word : words) {           // 拷贝遍历（成本高）
        cout << word << " ";
    }
    for (const auto &word : words) {    // 常量引用遍历（推荐，避免拷贝）
        std::cout << word << " ";
    }
    for (auto &word : words) {          // 修改元素，使用非常量引用，避免拷贝
        word = "new";
    }


    return 0;
}