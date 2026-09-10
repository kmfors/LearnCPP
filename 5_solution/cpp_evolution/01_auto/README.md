# auto / decltype

> 语言特性 | C++11 引入 | C++14、C++17、C++20、C++23 有增强与行为变化

---

## 概述

`auto` 让编译器根据初始化表达式推导变量类型，省去冗长的类型名。
`decltype` 推导表达式的类型，保留引用和 const 信息，与 `auto` 互补。
两者推导规则都源自模板参数推导，但 `decltype` 更精确。
核心注意点：**`auto` 会丢弃顶层 const 和引用，`decltype` 不会**。

---

## 版本演进

| 版本 | 变化 | 影响 |
|------|------|------|
| C++11 | 引入 `auto` 类型推导；引入 `decltype` | 核心特性 |
| C++14 | 函数返回值可自动推导；引入 `decltype(auto)` | 简化泛型代码 |
| C++17 | `auto x{1}` 推导从 `initializer_list` 改为 `int`；支持非类型模板参数 `auto`；配合结构化绑定 | 行为变化，需注意 |
| C++20 | 支持简写函数模板 `void f(auto x)` | 泛型代码简化 |
| C++23 | 引入 `auto(x)` / `auto{x}` 显式 decay-copy；放宽部分推导限制 | 语法便利 |

---

## 核心机制

### auto 推导规则

| 写法 | 推导结果 | 使用场景 |
|------|---------|---------|
| `auto x = expr;` | 值类型，丢弃 const/引用 | 确定要拷贝时 |
| `auto& x = expr;` | 左值引用 | 要修改原对象时 |
| `const auto& x = expr;` | 常量左值引用 | 只读且避免拷贝，最常用 |
| `auto&& x = expr;` | 转发引用 | 泛型代码中保留值类别 |
| `auto x = {1,2,3};` | `initializer_list<int>` | 一般不是预期结果 |
| `auto x{1};` | C++11/14: `initializer_list<int>`；C++17 起: `int` | 版本差异 |

**记忆要点**：`auto` 本身推导为「值」，要保留引用加 `&`，要保留 const 加 `const`。

### decltype 推导规则

`decltype(expr)` 的推导分两种情况：

| 情况 | 推导结果 |
|------|---------|
| `expr` 是变量名（不加括号） | 变量的声明类型，含 const 和引用 |
| `expr` 是表达式（含括号） | 左值 → `T&`；右值 → `T` |

```cpp
int x = 1;
int& r = x;
const int c = 2;

decltype(x)  a = x;    // int
decltype(r)  b = x;    // int&
decltype(c)  d = 2;    // const int
decltype((x)) e = x;   // int&（加了括号，变成表达式）
decltype(x + 1) f = 3; // int（右值）
```

### decltype(auto)

`decltype(auto)` 用 `decltype` 的规则推导 `auto`，保留引用和 const：

```cpp
int x = 1;
int& r = x;

auto           a = r;  // int
decltype(auto) b = r;  // int&
```

**使用场景**：函数返回值需要精确转发时，用 `decltype(auto)` 而非 `auto`。

### auto 与 decltype 对比

| 维度 | auto | decltype |
|------|------|----------|
| 推导依据 | 初始化表达式，按模板推导规则 | 表达式本身，保留引用和 const |
| 是否丢弃引用 | 是 | 否 |
| 是否丢弃顶层 const | 是 | 否 |
| 是否需要初始化 | 是 | 否 |
| 典型用途 | 变量声明、范围 for、lambda | 返回值推导、模板元编程 |
| 组合形式 | — | `decltype(auto)` |

---

## 写法对比

`auto` 的写法从 C++11 到 C++23 逐步增强，每个版本都有新增。按版本拆分为五个文件：

| 文件 | 内容 | 编译标准 |
|------|------|---------|
| `demo_cpp11.cpp` | auto 基础用法、范围 for、lambda | c++11 |
| `demo_cpp14.cpp` | 函数返回类型推导 | c++14 |
| `demo_cpp17.cpp` | `auto x{1}` 行为变化、结构化绑定、非类型模板参数 | c++17 |
| `demo_cpp20.cpp` | 简写函数模板 `void f(auto x)` | c++20 |
| `demo_cpp23.cpp` | `auto(x)` 显式 decay-copy、综合示例 | c++23 |

**核心差异**：
- C++11：引入 auto，但函数返回值不能自动推导，需要尾置返回类型
- C++14：返回值可自动推导
- C++17：`auto x{1}` 从 `initializer_list` 变为 `int`；结构化绑定
- C++20：`auto` 可作函数参数，等价于模板
- C++23：`auto(x)` 显式拷贝，用于泛型代码中显式 decay

---

## 注意事项

### 编译期

| 写法 | 结果 | 文件 |
|------|------|------|
| `auto x;` | 编译错误，必须有初始化 | pitfalls_syntax.cpp |
| `auto` 不能作函数参数（C++20 前） | 编译错误 | pitfalls_syntax.cpp |
| `auto a = arr;`（arr 为数组） | 编译错误，无法推导数组 | pitfalls_syntax.cpp |
| `decltype` 不要求初始化 | 合法，如 `decltype(x) y;` | pitfalls_syntax.cpp |

### 运行期

| 写法 | 反直觉之处 | 文件 |
|------|-----------|------|
| `auto copy = ref;` | 拷贝而非引用，改副本不影响原值 | pitfalls_runtime.cpp |
| `const int c = 5; auto d = c;` | const 被丢弃，d 可修改 | pitfalls_runtime.cpp |
| `auto a = {1,2,3};` | 是 `initializer_list`，不是 vector | pitfalls_runtime.cpp |
| `decltype((x))` | 加括号后变成引用类型 | pitfalls_runtime.cpp |
| `for (auto s : vs)` | 每次拷贝，应用 `const auto&` | pitfalls_runtime.cpp |

### 版本差异

| 写法 | C++11 | C++14 | C++17 | C++20 | C++23 | 文件 |
|------|-------|-------|-------|-------|-------|------|
| `auto x{1};` | `initializer_list<int>` | `initializer_list<int>` | `int` | `int` | `int` | pitfalls_version.cpp |
| `auto f() { return 1; }` | 编译错误 | `int` | `int` | `int` | `int` | pitfalls_version.cpp |
| `void f(auto x);` | 编译错误 | 编译错误 | 编译错误 | 合法 | 合法 | pitfalls_version.cpp |
| `auto(x)` | 不支持 | 不支持 | 不支持 | 不支持 | 合法 | pitfalls_version.cpp |

运行 `./build.sh` 可查看五种标准下的实际输出。

---

## 关联知识

| 知识点 | 关系 |
|--------|------|
| 模板类型推导 | auto 规则几乎一致，学 auto 等于学模板推导的一半 |
| decltype / decltype(auto) | 需要保留引用和 const 时使用，与 auto 互补 |
| 结构化绑定（C++17） | `auto [a, b] = pair;`，auto 的延伸用法 |
| 范围 for | `for (const auto& x : v)`，auto 最常见的搭档 |
| 尾置返回类型 | `auto f() -> int`，C++11 中配合 auto 使用 |
| lambda 表达式 | lambda 参数和返回值常用 auto |
| 简写函数模板（C++20） | `void f(auto x)`，auto 作为参数 |
| `auto(x)`（C++23） | 显式 decay-copy，泛型代码中使用 |

---

## 验证方式

```bash
./build.sh
```

或手动：

```bash
g++ -std=c++11 demo_cpp11.cpp -o d11 && ./d11
g++ -std=c++14 demo_cpp14.cpp -o d14 && ./d14
g++ -std=c++17 demo_cpp17.cpp -o d17 && ./d17
g++ -std=c++20 demo_cpp20.cpp -o d20 && ./d20
g++ -std=c++23 demo_cpp23.cpp -o d23 && ./d23
g++ -std=c++23 demo_decltype.cpp -o dd && ./dd
g++ -std=c++23 demo_decltype_auto.cpp -o dda && ./dda
g++ -std=c++23 pitfalls_syntax.cpp -o ps && ./ps
g++ -std=c++23 pitfalls_runtime.cpp -o pr && ./pr
for std in c++11 c++14 c++17 c++20 c++23; do
    g++ -std=$std pitfalls_version.cpp -o pv && echo "--- $std ---" && ./pv
done
```

---

## 参考

- cppreference: [auto specifier](https://en.cppreference.com/w/cpp/language/auto)
- cppreference: [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)
- cppreference: [Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction)
- cppreference: [Placeholder type specifiers](https://en.cppreference.com/w/cpp/language/placeholder_type_specifier)