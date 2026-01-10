# CodeFree

## C++ 类的编程规范

类数据成员命名：
 - 非静态成员：蛇形/小驼峰命名 + 末尾下划线，如 `total_count_`、`totalCount_`

 - 静态非常量：`s_` 前缀 + 蛇形/小驼峰命名，如 `s_instance_count`

 - 静态常量：`k` 前缀 + 大驼峰命名，如 `kMaxSize`

类成员函数命名：

 - 小驼峰命名