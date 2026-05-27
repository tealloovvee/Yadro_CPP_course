#include <cassert>

#include <gtest/gtest.h>

#include "CRC32.h"

TEST(CRC32Test, Testing) {
    static_assert(crc32("Hello, World!") == 3964322768);
    static_assert(crc32("lloovvee_tea") == 2712729623);
    SUCCEED();
}
