#include <cassert>

#include <gtest/gtest.h>

#include "Tuple.h"

TEST(TupleTest, TupleTest) {
    Tuple t(42, 3.1415, "Hello");
    EXPECT_EQ(Get<0>(t), 42);
    EXPECT_DOUBLE_EQ(Get<1>(t), 3.1415);
    EXPECT_EQ(Get<2>(t), "Hello");
    Get<0>(t) = 100;
    Get<1>(t) = 2.71828;
    Get<2>(t) = "World";
    EXPECT_EQ(Get<0>(t), 100);
    EXPECT_DOUBLE_EQ(Get<1>(t), 2.71828);
    EXPECT_EQ(Get<2>(t), "World");
    const auto& const_t = t;
    EXPECT_EQ(Get<0>(const_t), 100);
    EXPECT_DOUBLE_EQ(Get<1>(const_t), 2.71828);
    EXPECT_EQ(Get<2>(const_t), "World");
}
