#include <cassert>

#include <gtest/gtest.h>

#include "RPN.h"

TEST(RPNTest, Testing) {
    static_assert(("2 3 +"_rpn) == static_cast<int64_t>(2 + 3));
    static_assert(("2 3 + 4 5 + *"_rpn) == static_cast<int64_t>((2 + 3) * (4 + 5)));
    static_assert(("10 3 /"_rpn) == static_cast<int64_t>(10 / 3));
    static_assert(("2 3 ^"_rpn) == static_cast<int64_t>(2 * 2 * 2));
    static_assert(("5 1 2 + 4 * + 3 -"_rpn) == static_cast<int64_t>(5 + (1 + 2) * 4 - 3));
    SUCCEED();
}
