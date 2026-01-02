# C++标准库中的list详解

## 1. 概述

`std::list`是C++标准模板库(STL)中的一个双向链表容器，位于`<list>`头文件中。它允许在常数时间内在任意位置插入和删除元素，但不支持随机访问。

## 2. 基本特性

### 数据结构
- **双向链表**：每个元素包含指向前后元素的指针
- **非连续存储**：元素在内存中不是连续存储的
- **动态大小**：可根据需要自动扩展或收缩

### 时间复杂度
| 操作 | 时间复杂度 |
|------|-----------|
| 插入/删除（已知位置） | O(1) |
| 随机访问 | O(n) |
| 查找 | O(n) |
| 排序 | O(n log n) |

## 3. 基本用法

### 3.1 创建和初始化
```cpp
#include <list>
#include <iostream>

int main() {
    // 默认初始化
    std::list<int> list1;

    // 指定大小
    std::list<int> list2(5);  // 5个0

    // 指定大小和初始值
    std::list<int> list3(5, 10);  // 5个10

    // 使用初始化列表 (C++11)
    std::list<int> list4 = {1, 2, 3, 4, 5};

    // 使用迭代器范围
    int arr[] = {1, 2, 3, 4, 5};
    std::list<int> list5(arr, arr + 5);

    // 拷贝构造
    std::list<int> list6(list4);

    return 0;
}
```

### 3.2 常用成员函数

#### 元素访问
```cpp
std::list<int> myList = {1, 2, 3, 4, 5};

// 访问第一个和最后一个元素
std::cout << "Front: " << myList.front() << std::endl;
std::cout << "Back: " << myList.back() << std::endl;

// 注意：list不支持operator[]或at()进行随机访问
// myList[2]  // 错误！
```

#### 容量操作
```cpp
std::list<int> myList;

// 检查是否为空
bool isEmpty = myList.empty();

// 获取元素数量
size_t size = myList.size();

// 改变大小
myList.resize(10);           // 扩展为10个元素，新元素默认初始化为0
myList.resize(15, 5);        // 扩展为15个元素，新元素初始化为5
myList.resize(3);            // 缩减为3个元素
```

#### 修改操作
```cpp
std::list<int> myList;

// 添加元素
myList.push_back(10);        // 在末尾添加
myList.push_front(5);        // 在开头添加
myList.emplace_back(20);     // C++11，在末尾构造元素
myList.emplace_front(1);     // C++11，在开头构造元素

// 插入元素
auto it = myList.begin();
std::advance(it, 2);         // 移动迭代器到第三个位置
myList.insert(it, 15);       // 在第三个位置插入15
myList.insert(it, 3, 8);     // 插入3个8
myList.insert(it, {2, 4, 6});// C++11，插入初始化列表

// 删除元素
myList.pop_back();           // 删除最后一个元素
myList.pop_front();          // 删除第一个元素
myList.erase(it);            // 删除迭代器指向的元素
myList.erase(it1, it2);      // 删除[it1, it2)范围内的元素
myList.clear();              // 清空所有元素

// 特殊删除操作
std::list<int> list = {1, 2, 3, 2, 4, 2, 5};
list.remove(2);              // 删除所有值为2的元素

// 条件删除 (C++11)
list.remove_if([](int n) {
    return n % 2 == 0;       // 删除所有偶数
});
```

## 4. 迭代器操作

```cpp
std::list<int> myList = {1, 2, 3, 4, 5};

// 正向迭代器
std::cout << "Forward traversal: ";
for (auto it = myList.begin(); it != myList.end(); ++it) {
    std::cout << *it << " ";
}
std::cout << std::endl;

// 反向迭代器
std::cout << "Reverse traversal: ";
for (auto rit = myList.rbegin(); rit != myList.rend(); ++rit) {
    std::cout << *rit << " ";
}
std::cout << std::endl;

// 常量迭代器
for (auto cit = myList.cbegin(); cit != myList.cend(); ++cit) {
    // *cit = 10;  // 错误：不能修改常量迭代器指向的值
}

// C++11范围for循环
for (int val : myList) {
    std::cout << val << " ";
}
```

## 5. 特殊链表操作

### 5.1 拼接(splice)
```cpp
std::list<int> list1 = {1, 2, 3};
std::list<int> list2 = {4, 5, 6};

// 将list2的所有元素移动到list1的末尾
auto it = list1.end();
list1.splice(it, list2);  // list2变为空

// 移动单个元素
std::list<int> list3 = {7, 8, 9};
it = list1.begin();
std::advance(it, 2);
list1.splice(it, list3, list3.begin());  // 只移动list3的第一个元素

// 移动一段元素
std::list<int> list4 = {10, 11, 12, 13};
auto first = list4.begin();
auto last = list4.begin();
std::advance(last, 2);
list1.splice(list1.end(), list4, first, last);  // 移动前两个元素
```

### 5.2 合并(merge)
```cpp
std::list<int> list1 = {1, 3, 5};
std::list<int> list2 = {2, 4, 6};

// 合并两个已排序的链表
list1.sort();
list2.sort();
list1.merge(list2);  // list1: {1, 2, 3, 4, 5, 6}, list2变为空

// 自定义比较函数的合并
std::list<int> list3 = {10, 8, 6};
std::list<int> list4 = {9, 7, 5};
list3.sort(std::greater<int>());
list4.sort(std::greater<int>());
list3.merge(list4, std::greater<int>());  // 降序合并
```

### 5.3 排序(sort)
```cpp
std::list<int> myList = {5, 3, 1, 4, 2};

// 默认升序排序
myList.sort();

// 自定义排序（降序）
myList.sort(std::greater<int>());

// 自定义比较函数
myList.sort([](int a, int b) {
    return a % 2 > b % 2;  // 奇数在前，偶数在后
});
```

### 5.4 去重(unique)
```cpp
std::list<int> myList = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

// 删除连续重复的元素
myList.unique();  // 结果: {1, 2, 3, 4}

// 自定义去重条件
std::list<int> list2 = {1, 2, 3, 5, 6, 8, 9};
list2.unique([](int a, int b) {
    return (b - a) == 1;  // 删除相差1的连续元素
});
```

### 5.5 反转(reverse)
```cpp
std::list<int> myList = {1, 2, 3, 4, 5};
myList.reverse();  // 结果: {5, 4, 3, 2, 1}
```

## 6. 性能考虑

### 优点：
1. **插入/删除高效**：在任意位置插入或删除元素的时间复杂度为O(1)
2. **迭代器稳定性**：插入和删除操作不会使其他元素的迭代器失效
3. **内存使用灵活**：不需要连续内存空间

### 缺点：
1. **不支持随机访问**：访问特定位置元素需要遍历
2. **内存开销大**：每个元素需要额外存储前后指针
3. **缓存不友好**：非连续存储可能导致缓存命中率低

## 7. 使用场景

### 适用场景：
1. 需要频繁在中间插入/删除元素的场景
2. 元素数量动态变化且无法预知最大数量
3. 需要稳定的迭代器（插入/删除不影响其他迭代器）

### 示例应用：
```cpp
// LRU缓存实现
template<typename K, typename V>
class LRUCache {
private:
    std::list<std::pair<K, V>> cache;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> map;
    size_t capacity;

public:
    LRUCache(size_t cap) : capacity(cap) {}

    V get(K key) {
        if (map.find(key) == map.end()) return V();

        // 移动到链表头部
        cache.splice(cache.begin(), cache, map[key]);
        return map[key]->second;
    }

    void put(K key, V value) {
        if (map.find(key) != map.end()) {
            map[key]->second = value;
            cache.splice(cache.begin(), cache, map[key]);
            return;
        }

        if (cache.size() == capacity) {
            map.erase(cache.back().first);
            cache.pop_back();
        }

        cache.emplace_front(key, value);
        map[key] = cache.begin();
    }
};
```

## 8. 与其它容器的比较

| 特性 | std::list | std::vector | std::deque |
|------|-----------|-------------|------------|
| 数据结构 | 双向链表 | 动态数组 | 分段数组 |
| 随机访问 | O(n) | O(1) | O(1) |
| 头部插入/删除 | O(1) | O(n) | O(1) |
| 尾部插入/删除 | O(1) | O(1) | O(1) |
| 中间插入/删除 | O(1) | O(n) | O(n) |
| 内存连续性 | 否 | 是 | 部分连续 |
| 迭代器失效 | 稳定 | 可能失效 | 可能失效 |

## 9. 最佳实践

1. **使用emplace代替insert**：避免不必要的拷贝构造
2. **批量操作使用splice**：避免逐个插入的性能开销
3. **预先分配内存**：如果可以预估大小，使用reserve（实际上list没有reserve，这是vector的特性，这里指合理规划）
4. **选择合适的迭代器**：根据是否需要修改选择const_iterator或iterator

```cpp
// 高效批量操作示例
std::list<int> source = {1, 2, 3, 4, 5};
std::list<int> destination;

// 批量移动比逐个插入高效
destination.splice(destination.end(), source);
```

`std::list`是一个功能强大的容器，特别适合需要频繁插入删除但不需要随机访问的场景。正确理解其特性可以帮你编写出更高效的C++代码。