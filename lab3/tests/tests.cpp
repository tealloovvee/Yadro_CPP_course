#include <cassert>

#include <gtest/gtest.h>

#include "Literals.h"

TEST(RPNTest, Testing) {
    static_assert("Hello, World!"_to_upper == "HELLO, WORLD!");
    static_assert("meow"_to_upper == "MEOW");
    static_assert("abacaba"_to_upper != "ABACABa");
    SUCCEED();
}
