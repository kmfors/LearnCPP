#include <vector>
#include <string>
#include <iostream>
/*
  - 变量模板：它允许我们定义一族变量或静态数据成员
  - 优势改进：
    - 无需函数调用语法
    - 更适用于非字面量类型

  - 变量模板的典型应用场景：
    - 数学常数，例如 pi 与 e
    - 产生特定类型的值
    - 与类型特征（Type Traits）库结合  
*/

// 1、基本语法
template <typename T>
constexpr T pi = T(3.1415926535897932384626433L);

// 或者更复杂的例子
template <class T>
std::vector<T> empty_vector = std::vector<T>{};

// 使用不同的类型实例化变量模板
auto f_pi = pi<float>;       // 类型是 float
auto d_pi = pi<double>;      // 类型是 double
auto ld_pi = pi<long double>; // 类型是 long double

// 这个例子有意思，跟普通变量的定义不一样
auto vec_of_int = empty_vector<int>;    // 得到一个空的 std::vector<int>
auto vec_of_str = empty_vector<std::string>; // 得到一个空的 std::vector<std::string>




int main() { 
    std::cout << f_pi << std::endl ;
    std::cout << d_pi << std::endl ;
    std::cout << ld_pi << std::endl ;

    return 0;
}