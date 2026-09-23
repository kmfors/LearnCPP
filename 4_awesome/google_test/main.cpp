
#include <gtest/gtest.h>

TEST(MyTest, Add) {
    EXPECT_EQ(1 + 1, 2);
}

/**
// 布尔
EXPECT_TRUE(cond);
EXPECT_FALSE(cond);

// 相等 / 不等
EXPECT_EQ(a, b);    // a == b
EXPECT_NE(a, b);    // a != b

// 大小比较
EXPECT_LT(a, b);    // a <  b
EXPECT_LE(a, b);    // a <= b
EXPECT_GT(a, b);    // a >  b
EXPECT_GE(a, b);    // a >= b

// 字符串
EXPECT_STREQ("abc", str);   // C 字符串相等
EXPECT_STRNE("abc", str);

// 浮点
EXPECT_FLOAT_EQ(a, b);
EXPECT_NEAR(a, b, 1e-6);    // 误差范围内

// 异常
EXPECT_THROW(stmt, std::runtime_error);
EXPECT_NO_THROW(stmt);

*/