/*
  - auto 关键字新特性的增加：
    1、可以直接使用 auto 来让编译器自动推导函数返回类型
    2、Lambda 表达式的参数类型推导
    3、用于 lambda 的捕获初始化器。在 C++14 之前，lambda 只能捕获已经存在的变量。
       C++14 引入了捕获初始化器，允许在捕获列表中初始化新变量

  - 函数返回类型推导的限制：
    - 函数必须有多个 return 语句时，所有返回表达式的类型必须完全一致
    - 函数内的 return 语句必须在编译时可见（不能有条件分支返回不同类型）
    - 不能用于虚函数和递归函数（除非递归调用前有 return 语句提供类型信息）  
  
    - auto 捕获初始化器的推导，语法说明（可以显式指定类型）
      [captured_var = expression](parameters) -> return_type { body }
*/

// 增强1: 函数返回类型的自动推导
template<typename T, typename U>
auto add(T t, U u) { // 编译器自动推导返回类型
    return t + u;
}

// 增强2：Lambda 表达式的参数类型推导，相当于一个模板函数对象
auto lambda1 = [](auto x, auto y) { return x + y; };

// 增强3：用于 lambda 的捕获初始化器
int x = 10;
auto lambda2 = [value = x]() { return value; }; 




int main() {
    
    return EXIT_SUCCESS;
}