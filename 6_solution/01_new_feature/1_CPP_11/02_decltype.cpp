#include <iostream>
#include <string>
using namespace std;

/**
 * ============================================================================
 * KNOWLEDGE: decltype 推导表达式的类型，不计算表达式的值
 *
 * 使用规则：
 *  1.对于变量名，decltype 给出该变量的声明类型
 *  2.对于表达式（非单纯变量名），decltype 给出表达式结果的类型
 *  3.表达式外加括号，得到引用类型
 *  4.表达式解引用操作，得到引用类型
 *
 * decltype 的主要应用：
 *  - 声明与已有变量相同类型的新变量
 *  - 推导模板函数的返回类型（配合尾置返回类型）
 *  - 定义复杂类型的别名
 *  - 推断 lambda 表达式的类型
 *  - 声明与已有变量相同类型的新变量
 *
 * ============================================================================
*/
// 应用1
int g_var = 100;
decltype(g_var) g_temp_var = 50;

// 应用2
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}

// 应用3
using T = decltype(add(2, 3));

// 应用4: 创建同类型的副本
auto lambda = [](int x) { return x * 2; };
decltype(lambda) copy_of_lambda = lambda;

int main()
{
    int i = 42;
    const int ci = 10;
    int& ri = i;
    int* pi = &i;

    // 规则1
    decltype(i) a;              // int
    decltype(ci) b = 20;        // const int (必须初始化)
    decltype(ri) c = i;         // int& (必须引用已存在对象)

    // 规则2
    decltype(i + 0) d;          // int (表达式结果是右值)
    decltype(ri) e = i;         // int& (ri是左值表达式)

    // 规则3
    decltype((i)) f = i;        // int& (因为(i)是左值表达式)
    decltype((ci)) g = ci;      // const int&

    // 规则4
    decltype(*pi) h = i;        // int& (解引用产生左值)

    return 0;
}