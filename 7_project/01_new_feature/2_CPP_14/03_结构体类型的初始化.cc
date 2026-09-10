#include <string>
#include <iostream>

/*
  - C++14 对初始化列表最重要的改进:
    1、聚合类型的成员初始化增强，允许同时使用默认成员初始化器和聚合初始化

*/


struct Person {
    std::string name = "Unknown";
    int age = 0;
    double height = 1.7;
};

int main() {
    // C++14: 正确！可以同时使用默认值和聚合初始化
    Person p1{"Alice", 25, 1.65}; // 提供所有值
    Person p2{"Bob", 30};         // 部分初始化，height 使用默认值 1.7
    Person p3{"Charlie"};         // 只初始化 name，age=0, height=1.7

    std::cout << p2.name << " " << p2.age << " " << p2.height << std::endl;
}

