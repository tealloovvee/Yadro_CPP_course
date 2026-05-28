#include <cassert>

#include <gtest/gtest.h>

#include "IsInList.h"

TEST(IsInListTest, IsInListTest) {
    static_assert(IsInList("open"));
    static_assert(IsInList("close"));
    static_assert(!IsInList("push"));
    SUCCEED();
}
