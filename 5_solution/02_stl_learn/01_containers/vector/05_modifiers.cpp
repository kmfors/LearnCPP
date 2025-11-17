#include <iostream>
#include <vector>
#include <string>
using namespace std;

/**
 * 插入新元素：优先使用 emplace() 和 emplace_back()
 *
 * 添加已有对象：使用 push_back() 和 insert()
 *
 * 删除单个元素：使用 erase() 或 pop_back()
 *
 * 批量删除：使用 erase() 带范围参数
 *
 * 调整大小：使用 resize()
 *
 * 清空容器：使用 clear()
*/

void for_each(const char* str, const vector<int>& vec) {
    std::cout << str << ":" << '\n';
    for(const auto& it : vec) {
        std::cout << it << " ";
    }
    std::cout << "\n\n";
}

int main(int argc, char* argv[])
{
    std::vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for_each("origin vec", vec);

    auto pos = vec.cend();
    vec.insert(pos, 100);
    for_each("insert", vec);

    vec.emplace(vec.end(), 101);
    for_each("emplace", vec);

    vec.push_back(102);
    for_each("push back", vec);

    vec.emplace_back(103);
    for_each("emplace back", vec);

    vec.erase(vec.cbegin(), vec.cbegin() + 2);
    for_each("erase", vec);

    vec.pop_back();
    for_each("pop back", vec);

    vec.resize(15, 0);
    for_each("resize", vec);

    vec.clear();
    std::cout << "After clear: size=" << vec.size() << ", capacity=" << vec.capacity() << '\n';

    return 0;
}