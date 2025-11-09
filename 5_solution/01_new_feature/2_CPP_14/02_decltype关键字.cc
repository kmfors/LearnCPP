#include <vector>
#include <iostream>
/*
  - decltype C++14新特性：
    decltype(auto) 结合了 auto 的便利性和 decltype 的精确性，它使用 decltype 的规则来推导 auto。
    解决 auto 忽略引用的问题

*/

template<typename Container, typename Index>
decltype(auto) access(Container& c, Index i) { // 精确推导类型
    return c[i]; // 返回的是引用类型！
}

int main() {
    std::vector<int> vec = {1, 2, 3};
    access(vec, 1) = 10; // 可以修改元素了
    std::cout << vec[1]; // 输出 10
}





