#include <array>

#include <gtest/gtest.h>

#include <bitset.h>

TEST(BitsetTest, DefaultConstructor) {
    const Bitset bitset;
    EXPECT_EQ(bitset.size(), 0);
    EXPECT_TRUE(bitset.empty());
    EXPECT_EQ(bitset.size(), 0);
}

TEST(BitsetTest, SimpleCases) {
    constexpr size_t maxVal = 19;
    Bitset bitset(maxVal);
    const size_t initialSize = bitset.size();
    EXPECT_GT(initialSize, maxVal);

    constexpr size_t randomIndex = 15;
    EXPECT_TRUE(bitset.empty());
    bitset.set(randomIndex, true);
    EXPECT_FALSE(bitset.empty());
    EXPECT_EQ(bitset.sum(), 1);
    bitset.set(randomIndex, false);
    EXPECT_TRUE(bitset.empty());
    EXPECT_EQ(bitset.sum(), 0);
    EXPECT_EQ(initialSize, bitset.size());

    constexpr size_t randomRangeStart = 5;
    constexpr size_t randomRangeEnd = 10;
    ASSERT_LT(randomRangeStart, randomRangeEnd);

    for (size_t i = randomRangeStart; i < randomRangeEnd; ++i) {
        bitset.set(i, true);
    }
    EXPECT_EQ(bitset.sum(), randomRangeEnd - randomRangeStart);
    EXPECT_TRUE(bitset[randomRangeStart]);
    EXPECT_EQ(bitset[randomRangeStart], bitset.test(randomRangeStart));
    EXPECT_TRUE(bitset[randomRangeEnd - 1]);
    EXPECT_EQ(bitset[randomRangeEnd - 1], bitset.test(randomRangeEnd - 1));
    bitset.reset();
    EXPECT_FALSE(bitset[randomRangeStart]);
    EXPECT_EQ(bitset[randomRangeStart], bitset.test(randomRangeStart));
    EXPECT_FALSE(bitset[randomRangeEnd - 1]);
    EXPECT_EQ(bitset[randomRangeEnd - 1], bitset.test(randomRangeEnd - 1));

    bitset.clear();
    EXPECT_EQ(bitset.size(), 0);
    constexpr size_t bigIndex = 123;
    EXPECT_FALSE(bitset.test(bigIndex));
}

TEST(BitsetTest, RAII) {
    Bitset original(63);
    constexpr size_t index = 5;
    original.set(index, true);
    Bitset copy1(original);
    Bitset copy2;
    copy2 = original;
    EXPECT_TRUE(copy1[index]);
    EXPECT_TRUE(copy2[index]);
    original.set(index, false);
    EXPECT_FALSE(original[index]);
    EXPECT_TRUE(copy1[index]);
    EXPECT_TRUE(copy2[index]);

    ASSERT_TRUE(original.empty());
    constexpr std::array<size_t, 5> indices{1, 6, 0, 12, 55};
    for (size_t i : indices) {
        original.set(i, true);
    }
    const size_t originalSize = original.size();
    Bitset copy3 = std::move(original);
    EXPECT_EQ(originalSize, copy3.size());
    for (size_t i : indices) {
        EXPECT_EQ(copy3[i], true);
    }

    constexpr size_t yetAnotherIndex = 2;
    EXPECT_TRUE(std::find(indices.cbegin(), indices.cend(), yetAnotherIndex) == indices.cend());
    Bitset copy4(5);
    copy4.set(yetAnotherIndex, true);
    copy4 = std::move(copy3);
    EXPECT_FALSE(copy4[yetAnotherIndex]);
    EXPECT_EQ(originalSize, copy4.size());
    for (size_t i : indices) {
        EXPECT_EQ(copy4[i], true);
    }
}

TEST(BitsetTest, OperationsOnSets) {
    constexpr size_t size1 = 15;
    constexpr size_t size2 = 7;
    constexpr std::array<size_t, 6> index1{0, 5, 6, 10, 11, 15};
    constexpr std::array<size_t, 4> index2{0, 1, 5, 7};
    Bitset bitset1(size1);
    Bitset bitset2(size2);
    for (auto i : index1) {
        bitset1.set(i, true);
    }
    for (auto i : index2) {
        bitset2.set(i, true);
    }
    Bitset result = bitset1.union_with(bitset2);
    EXPECT_EQ(result.size(), std::max(bitset1.size(), bitset2.size()));
    for (size_t i = 0; i < std::max(size1, size2); ++i) {
        EXPECT_TRUE(result[i] == (bitset1[i] || bitset2[i]));
    }
    result = bitset1.intersection_with(bitset2);
    EXPECT_EQ(result.size(), std::min(bitset1.size(), bitset2.size()));
    for (size_t i = 0; i < std::min(size1, size2); ++i) {
        EXPECT_TRUE(result[i] == (bitset1[i] && bitset2[i]));
    }
    Bitset bitset3(size1);
    for (size_t i = 0; i < index1.size(); i += 2) {
        bitset3.set(index1[i], true);
    }
    EXPECT_TRUE(bitset3.is_subset(bitset1));
    bitset3.set(3, true);
    EXPECT_FALSE(bitset3.is_subset(bitset1));
    bitset3.set(3, false);
    EXPECT_TRUE(bitset3.is_subset(bitset1));
    bitset3.resize(size1 * 10);
    EXPECT_TRUE(bitset3.is_subset(bitset1));
}

// TODO: add tests for resize: ensure_capacity, shrink