#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

/**
 *  - 支持Lambda表达式参数类型的推导
 *  -
*/

int main() {
    // 基础泛型lambda
    auto add = [](auto a, auto b) { return a + b; };

    std::cout << "add(5, 3) = " << add(5, 3) << std::endl;
    std::cout << "add(2.5, 3.7) = " << add(2.5, 3.7) << std::endl;

    // 与STL算法结合
    std::vector<int> numbers = {1, 5, 3, 8, 2, 7};
    auto greaterThan = [](auto threshold) {
        return [threshold](auto value) { return value > threshold; };
    };
    auto count = std::count_if(numbers.begin(), numbers.end(), greaterThan(4));
    std::cout << "Numbers greater than 4: " << count << std::endl;

    return 0;
}