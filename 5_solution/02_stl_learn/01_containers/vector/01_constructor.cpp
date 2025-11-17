#include <iostream>
#include <vector>

/**
 * vector：实现动态数组的序列容器
*/

int main()
{
    // 1. 默认构造
    std::vector<int> v1;

    // 2. 指定元素数量构造
    std::vector<int> v2(5);     // 5个元素，默认值为0
    std::vector<int> v3(5, 10); // 5个元素，值皆为10

    // 2. 范围构造
    int arr[] = {1, 2, 3, 4};
    std::vector<int> v4(arr, arr + 3);

    // 3. 拷贝构造
    std::vector<int> v5(v4);

    // 4. 初始化列表构造
    std::vector<int> v6 = {1, 2, 3, 4, 5};

    // 5. 移动构造（C++11）
    std::vector<int> v7(move(v6));

    return 0;
}