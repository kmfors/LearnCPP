/*
  - decltype 用于推导表达式的类型，但不计算表达式的值。它可以保留表达式的所有类型信息（包括顶层const和引用）
  - decltype 的表达式如果是加上了括号的变量，则得到的类型是引用类型
  - decltype 的表达式的内容如果是解引用操作，则得到的类型是引用类型

  - 用途：
    1、声明返回类型依赖于参数的函数模板
    2、在编译期获取类型信息
    3、推断 lambda 表达式的类型

*/

void demo() {
    int x = 0;
    decltype(x) v1;  // int 类型
    decltype((x)) v2 = x; // int& 类型，因为 (x) 是一个左值表达式

    const int& v3 = x;
    decltype(v3) v4 = x; // const int& 类型

    int* p = &x;
    decltype(p) v5; // int* 类型
    decltype(*p) v6 = x; // int& 类型
}

// 用途1: 推导函数的返回类型（需要 decltype 和尾置返回类型）
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) { 
    return t + u;
}

// 用途2：定义类型别名
using T = decltype(add(2, 3)); 

// 用途3：推断 lambda 表达式的类型
auto lambda = [](int x) { return x * 2; };
decltype(lambda) copy_of_lambda = lambda; // 创建同类型的副本

int main() {

}