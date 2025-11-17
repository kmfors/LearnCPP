#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 *
*/

#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 1. begin() 和 end() - 正向非const迭代器
    std::cout << "use begin() and end():" << std::endl;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
        *it *= 2;       // 可修改
    }
    std::cout << "\n\n";

    // 2. cbegin() 和 cend() - 正向const迭代器
    std::cout << "use cbegin() and cend():" << std::endl;
    for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
        std::cout << *it << " ";
        // *it = 10;    // 错误！不能修改
    }
    std::cout << "\n\n";

    // 3. rbegin() 和 rend() - 反向非const迭代器
    std::cout << "use rbegin() and rend():" << std::endl;
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << *it << " ";
        *it += 1;       // 可修改
    }
    std::cout << "\n\n";

    // 4. crbegin() 和 crend() - 反向const迭代器
    std::cout << "use crbegin() and crend():" << std::endl;
    for (auto it = vec.crbegin(); it != vec.crend(); ++it) {
        std::cout << *it << " ";
        // *it = 20;    // 错误！不能修改
    }
    std::cout << "\n\n";

    return 0;
}