#include <iostream>
#include <type_traits>

int global_value = 42;

int& getGlobal() { return global_value;}
const int& getConstGlobal() {return global_value;}

// 使用 decltype(auto) 精确推导返回类型
decltype(auto) ret_by_refer() {
    return getGlobal();  // 返回 int&
}

decltype(auto) ret_by_const_refer() {
    return getConstGlobal();  // 返回 const int&
}


// 对比：单独使用 auto 会去掉引用和 const 限定符
auto ret_by_value() {
    return getGlobal();  // 返回 int
}

template<typename T>
decltype(auto) forward_like(T&& value) {
    return std::forward<T>(value);  // 完美转发
}

int main() {
    // val1 是 int， val2 是 int&
    auto val1 = ret_by_refer();
    decltype(auto) val2 = ret_by_refer();

    // 修改全局变量
    getGlobal() = 100;
    std::cout << "val1 = " << val1 << std::endl;  // 拷贝的值
    std::cout << "val2 = " << val2 << std::endl;  // 引用的值
    std::cout << "global_value = " << global_value << std::endl;

    // 类型信息验证
    std::cout << std::boolalpha;
    std::cout << "val1 is reference: " << std::is_reference<decltype(val1)>::value << std::endl;
    std::cout << "val2 is reference: " << std::is_reference<decltype(val2)>::value << std::endl;

    return 0;
}