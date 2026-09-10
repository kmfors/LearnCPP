#include <vector>
#include <iostream>

using std::cout;
using std::endl;

/**
 * 元素访问操作：
 *  - vector的所有元素访问操作都是 O(1) 时间复杂度
 *  - 因为vector使用连续内存布局，可以通过简单的地​址计算直接访问任何元素
 *  - 使得vector在需要频繁随机访问的场景下非常高效
*/

int main() {

    std::vector<int> vec = {10, 20, 30, 40, 50};

    // 1. 下标符 [] 索引访问（无边界检查，性能更高）
    cout << "vec[2] = " << vec[2] << endl;

    // 2. 使用 at() 方法（带边界检查，性能稍低）
    cout << "vec.at(2) = " << vec.at(2) << endl;
    // vec.at(10);  // 访问越界时抛出 std::out_of_range 异常

    // 3. 访问第一个和最后一个元素
    cout << "front: " << vec.front() << endl;
    cout << "back: " << vec.back() << endl;

    // 4. 使用 data() 获取底层数组指针
    int* ptr = vec.data();
    cout << "data[0] = " << ptr[0] << endl;

    return 0;
}