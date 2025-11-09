
/*
  - C++11中，由于没有变量模板。为了实现类似的功能，通常需要借助其他手段，而这些手段往往不够直观和方便。
    1、使用类模板的静态常量成员（本质：结构体对象访问成员）
    2、使用 constexpr 函数模板返回常量值（本质：模板函数）
*/

// C++11 方式1：使用类模板的静态常量成员
template <typename T>
struct Pi {
    static constexpr T value = T(3.1415926535897932384626433L);
};

double circle_area1(double r) {
    return Pi<double>::value * r * r; // 必须通过 ::value 访问
}

// C++11 方式2：使用 constexpr 函数模板返回常量值
template <typename T>
constexpr T pi() {
    return T(3.1415926535897932384626433L);
}

// 使用方式
double circle_area2(double r) {
    return pi<double>() * r * r; // 需要函数调用语法 ()
}

int main() { return 0;}