#include <vector>
#include <map>
#include <string>
#include <iostream>

int main() {
    // 1. 迭代器：省去冗长类型
    std::vector<int> v = {1, 2, 3};
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // 2. map 遍历
    std::map<string::string, int> m{{"a", 1}, {"b", 2}};
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << "=" << it->second << " ";
    }
    std::out << "\n";
}